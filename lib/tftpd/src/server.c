#define _XOPEN_SOURCE 500

#include "tftpd/server.h"

#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <task/task.h>

#include "tftpd/err.h"

#define SERVER_STACK_SZ 32768

int tftp_sockfd = -1;
int running = 0;

typedef struct ServerArgs {
  char *errbuf;
  char *root;
  int port;
  Channel *statchan;
} ServerArgs;

void ServerTask(void *args) {
  int err, updated;
  ServerArgs *info;
  struct pollfd fds[1];
  struct sockaddr remote_address;
  socklen_t remote_address_len;
  struct in_addr ipv4_addr;

  info = (ServerArgs *)args;
  taskname("tftpd:server");

  tftp_sockfd = netannounce(UDP, NULL, info->port);
  if (tftp_sockfd < 0) {
    err = errno;
    if (info->errbuf != NULL)
      snprintf(info->errbuf, TFTPD_ERRBUF_SIZE,
               "could not bind to port %d: netannounce: %s\n", info->port,
               strerror(err));
    chansendul(info->statchan, SERVER_NOFD);
    goto server_cleanup;
  }
  chansendul(info->statchan, SERVER_OK);

  printf("Serving TFTP on 0.0.0.0 port %d (tftp://0.0.0.0:%d/) ...\n",
         info->port, info->port);

  fds[0].fd = tftp_sockfd;
  fds[0].events = POLLIN | POLLHUP;
  running = 1;
  while (running) {
    fdwait(tftp_sockfd, 'r');
    updated = poll(fds, 1, 0);

    if (updated == -1) {
      if (errno == EAGAIN) continue;

      err = errno;
      if (info->errbuf != NULL)
        snprintf(info->errbuf, TFTPD_ERRBUF_SIZE,
                 "could not poll socket: poll: %s\n", strerror(err));
      running = 0;
      break;
    }

    if (fds[0].revents & POLLIN) {
      /* TODO: Process packet. For now, just delay and continue. */
      recvfrom(tftp_sockfd, NULL, 0, MSG_WAITALL, &remote_address,
               &remote_address_len);
      if (remote_address.sa_family == AF_INET) {
        ipv4_addr.s_addr = ntohl(*((in_addr_t *)remote_address.sa_data));
        printf("Received message from %s\n", inet_ntoa(ipv4_addr));
      } else if (remote_address.sa_family == AF_INET6) {
        printf("Received message on IPv6\n");
      } else {
        printf("Received message with unknown address family: %d\n",
               remote_address.sa_family);
      }
      continue;
    }

    /* Break loop if fd is closed or invalid. */
    if (fds[0].revents & (POLLHUP | POLLERR | POLLNVAL)) {
      running = 0;
      break;
    }
  }

server_cleanup:
  free(info->root);
  free(info);
  taskexit(EXIT_SUCCESS);
  return;
}

void StopServer(int sig) {
  if (sig == SIGINT) {
    printf("\nKeyboard interrupt received, exiting.\n");
  } else {
    printf("\nUnknown signal received, stopping server regardless.\n");
  }
  running = 0;
  if (tftp_sockfd >= 0) close(tftp_sockfd);
}

int StartServer(const char *root, int port, char *errbuf) {
  int err = 0, stat;
  char *realroot;
  ServerArgs *server_args;

  realroot = realpath(root, NULL);
  if (realroot == NULL) {
    err = errno;
    if (errbuf != NULL)
      snprintf(errbuf, TFTPD_ERRBUF_SIZE,
               "could not resolve path '%s': realpath: %s", root,
               strerror(err));
    return -1;
  }

  taskstate("starting tftpd:server");

  server_args = (ServerArgs *)malloc(sizeof(ServerArgs));
  assert(server_args != NULL && "couldn't malloc() server_args");

  server_args->errbuf = errbuf;
  server_args->root = realroot;
  server_args->port = port;
  server_args->statchan = chancreate(sizeof(void *), 1);

  taskcreate(ServerTask, server_args, SERVER_STACK_SZ);

  taskstate("started tftpd:server");
  stat = chanrecvul(server_args->statchan);
  chanfree(server_args->statchan);
  if (stat != SERVER_OK) {
    return -1;
  }
  signal(SIGINT, StopServer);

  return 0;
}

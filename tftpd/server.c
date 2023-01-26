#define _XOPEN_SOURCE 500

#include "server.h"

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

#define SERVER_STACK_SZ 32768

int tftp_sockfd = -1;
int running = 0;

typedef struct ServerArgs {
  const char *command;
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
    fprintf(stderr, "%s: netannounce: could not bind to port %d: %s\n",
            info->command, info->port, strerror(err));
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
      err = errno;
      fprintf(stderr, "%s: poll failed: %s\n", info->command, strerror(err));
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
    if (fds[0].revents & (POLLHUP | POLLERR | POLLNVAL)) break;
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

int StartServer(const char *root, int port, const char *command) {
  int err = 0, stat;
  char *realroot;
  ServerArgs *server_args;

  realroot = realpath(root, NULL);
  if (realroot == NULL) {
    err = errno;
    fprintf(stderr, "%s: realpath: could not resolve path '%s': %s\n", command,
            root, strerror(err));
    return -1;
  }

  taskstate("starting tftpd:server");

  server_args = (ServerArgs *)malloc(sizeof(ServerArgs));
  assert(server_args != NULL && "couldn't malloc() server_args");

  server_args->command = command;
  server_args->root = realroot;
  server_args->port = port;
  server_args->statchan = chancreate(sizeof(void *), 1);

  taskcreate(ServerTask, server_args, SERVER_STACK_SZ);

  stat = chanrecvul(server_args->statchan);
  chanfree(server_args->statchan);
  if (stat != SERVER_OK) {
    return -1;
  }
  taskstate("started tftpd:server");
  signal(SIGINT, StopServer);

  return 0;
}

#define _XOPEN_SOURCE 500

#include "tftpd/server.h"

#include <assert.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

/* Temporarily here, till Packet Handling is implemented. */
#include <arpa/inet.h>

#include <task/task.h>

#include "server/poll.h"
#include "tftpd/err.h"

#define SERVER_STACK_SIZE 32768

int running = 0;
Channel *notifychan = NULL;
int notify_open = 0;
int server_status = EXIT_SUCCESS;

typedef struct ServerArgs {
  int sockfd;
  char *root;
  char *errbuf;
} ServerArgs;

int GetSocketFd(int port, char *errbuf) {
  int err;
  int sockfd;

  sockfd = netannounce(UDP, NULL, port);
  if (sockfd < 0) {
    err = errno;
    if (errbuf != NULL)
      snprintf(errbuf, TFTPD_ERRBUF_SIZE,
               "could not bind to port %d: netannounce: %s", port,
               strerror(err));
    return -1;
  }

  return sockfd;
}

void ServerTask(void *args) {
  int err;
  ServerArgs *info;
  struct sockaddr remote_address;
  const struct sockaddr_in *rem_addr_in;
  const struct sockaddr_in6 *rem_addr_in6;
  socklen_t remote_address_len;
  unsigned long notify_events;
  char addr_str[INET6_ADDRSTRLEN];
  uint8_t buf[SERVER_BUF_SIZE];
  ssize_t msg_len;

  notifychan = chancreate(sizeof(unsigned long), 1);
  assert(notifychan != NULL && "chancreate() failed");
  notify_open = 1;

  info = (ServerArgs *)args;
  taskname("tftpd:server");

  while (running) {
    PollOnChannel(info->sockfd, notifychan, &notify_open, info->errbuf);

    notify_events = chanrecvul(notifychan);

    /* Skip one cycle if SERVER_INTERRUPT is received. */
    if (notify_events & SERVER_INTERRUPT) continue;

    /* Exit if poll() failed exceptionally. */
    if (notify_events & SERVER_ERR_POLL) {
      server_status = EXIT_FAILURE;
      running = 0;
      break;
    }

    if (notify_events & SERVER_POLLIN) {
      /* TODO: Process packet. For now, just delay and continue. */
      while (
          (msg_len = recvfrom(info->sockfd, buf, SERVER_BUF_SIZE, MSG_WAITALL,
                              &remote_address, &remote_address_len)) == -1 &&
          errno == EAGAIN)
        ;

      if (msg_len == -1) {
        err = errno;
        snprintf(info->errbuf, TFTPD_ERRBUF_SIZE,
                 "could not read UDP datagram: recvfrom: %s", strerror(err));
        server_status = EXIT_FAILURE;
        running = 0;
        break;
      }

      switch (remote_address.sa_family) {
        case AF_INET:
          rem_addr_in = (struct sockaddr_in *)&remote_address;
          inet_ntop(AF_INET, &rem_addr_in->sin_addr, addr_str,
                    INET6_ADDRSTRLEN);
          printf("Received %ld-byte message via IPv4 from %s\n", msg_len,
                 addr_str);
          break;

        case AF_INET6:
          rem_addr_in6 = (struct sockaddr_in6 *)&remote_address;
          inet_ntop(AF_INET6, &rem_addr_in6->sin6_addr, addr_str,
                    INET6_ADDRSTRLEN);
          printf("Received %ld-byte message via IPv6 from %s\n", msg_len,
                 addr_str);
          break;

        default:
          printf("Received %ld-byte message with unknown address family: %d\n",
                 msg_len, remote_address.sa_family);
          break;
      }
      continue;
    }

    /* Break loop if fd is closed or invalid. */
    if (notify_events & (SERVER_POLLHUP | SERVER_POLLERR | SERVER_POLLNVAL)) {
      running = 0;
      break;
    }
  }

  /* Ensure running is set as 0 when breaks out of loop. */
  running = 0;

  /* Perform server cleanup. */
  notify_open = 0;
  chanfree(notifychan);
  free(info->root);
  close(info->sockfd);
  free(info);
  taskexit(server_status);
  return;
}

void StopServer(int sig) {
  static int sigint_count = 0;

  server_status = 128 + sig;

  switch (sig) {
    case SIGTERM:
      fprintf(stderr, "tftpd terminated\n");
      break;

    case SIGINT:
      switch (sigint_count) {
        case 0:
          server_status = EXIT_SUCCESS;
          printf("\nKeyboard interrupt received, exiting.\n");
          break;
        default:
          printf("\nAnother keyboard interrupt received, exiting forcefully.");
          break;
      }
      sigint_count++;
      break;

    default:
      fprintf(
          stderr,
          "tftpd: Unknown signal (%d) received, stopping server regardless.\n",
          sig);
      break;
  }

  running = 0;
  if (notify_open) chansendul(notifychan, SERVER_INTERRUPT);

  if (sig == SIGINT && sigint_count >= 2) taskexitall(server_status);
}

int StartServer(const char *root, int port, char *errbuf) {
  int err = 0;
  int sockfd;
  char *realroot;
  ServerArgs *server_args;

  sockfd = GetSocketFd(port, errbuf);
  if (sockfd == -1) {
    return -1;
  }

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

  server_args->sockfd = sockfd;
  if (server_args->sockfd == -1) {
    free(realroot);
    return -1;
  }
  server_args->root = realroot;
  server_args->errbuf = errbuf;

  running = 1;
  taskcreate(ServerTask, server_args, SERVER_STACK_SIZE + SERVER_BUF_SIZE);

  taskstate("started tftpd:server");
  printf("Serving TFTP on 0.0.0.0 port %d (tftp://0.0.0.0:%d/) ...\n", port,
         port);

  /* Setup signal interrupt handlers. */
  signal(SIGTERM, StopServer);
  signal(SIGINT, StopServer);

  return 0;
}

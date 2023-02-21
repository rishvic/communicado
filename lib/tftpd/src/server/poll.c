#include "server/poll.h"

#include <errno.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <task/task.h>

#include "tftpd/err.h"

#define POLL_STACK_SIZE 32768

typedef struct PollArgs {
  int sockfd;
  Channel *notifychan;
  const int *chanopenp;
  char *errbuf;
} PollArgs;

void PollTask(void *args) {
  int updated, err;
  unsigned long res;
  PollArgs *info;
  struct pollfd fds[1];

  taskname("tftpd:server:poll");
  tasksystem();

  info = (PollArgs *)args;
  fds[0].fd = info->sockfd;
  fds[0].events = POLLIN | POLLHUP;

  taskstate("polling socket");
  do {
    fdwait(info->sockfd, 'r');
    updated = poll(fds, 1, 0);
  } while (updated == -1 && errno == EAGAIN);

  if (updated >= 0) {
    taskstate("polled successfully");
    res = 0;
    if (fds[0].revents & POLLIN) res |= SERVER_POLLIN;
    if (fds[0].revents & POLLERR) res |= SERVER_POLLERR;
    if (fds[0].revents & POLLHUP) res |= SERVER_POLLHUP;
    if (fds[0].revents & POLLNVAL) res |= SERVER_POLLNVAL;
    if (*(info->chanopenp)) chansendul(info->notifychan, res);
  } else {
    err = errno;
    taskstate("poll failed");
    snprintf(info->errbuf, TFTPD_ERRBUF_SIZE, "poll() failed: %s",
             strerror(err));
    if (*(info->chanopenp)) chansendul(info->notifychan, SERVER_ERR_POLL);
  }

  free(info);
  taskexit(EXIT_SUCCESS);
  return;
}

void PollOnChannel(int sockfd, Channel *notifychan, const int *chanopenp,
                   char *errbuf) {
  PollArgs *args;

  args = (PollArgs *)malloc(sizeof(PollArgs));
  args->sockfd = sockfd;
  args->notifychan = notifychan;
  args->chanopenp = chanopenp;
  args->errbuf = errbuf;

  taskcreate(PollTask, args, POLL_STACK_SIZE);
}

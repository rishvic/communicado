#ifndef COMMUNICADO_TFTPD_SERVER_POLL_H_
#define COMMUNICADO_TFTPD_SERVER_POLL_H_

#include <task/task.h>

#define SERVER_POLLIN 0x001UL
#define SERVER_POLLERR 0x008UL
#define SERVER_POLLHUP 0x010UL
#define SERVER_POLLNVAL 0x020UL

#define SERVER_INTERRUPT 0x10000UL
#define SERVER_ERR_POLL 0x20000UL

void PollOnChannel(int sockfd, Channel *notifychan, const int *chanopenp,
                     char *errbuf);

#endif /* COMMUNICADO_TFTPD_SERVER_POLL_H_ */

#ifndef COMMUNICADO_TFTPD_SERVER_H_
#define COMMUNICADO_TFTPD_SERVER_H_

#define SERVER_OK 0
#define SERVER_NOFD 1

extern int tftp_sockfd;
extern int running;

int StartServer(const char *root, int port, const char *command);

#endif /* COMMUNICADO_TFTPD_SERVER_H_ */

#ifndef COMMUNICADO_TFTPD_SERVER_H_
#define COMMUNICADO_TFTPD_SERVER_H_

#define SERVER_BUF_SIZE 5000UL

extern int running;

int StartServer(const char *root, int port, char *errbuf);

#endif /* COMMUNICADO_TFTPD_SERVER_H_ */

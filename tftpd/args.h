#ifndef COMMUNICADO_TFTPD_ARGS_H_
#define COMMUNICADO_TFTPD_ARGS_H_

typedef struct CliArgs {
  int version, help;
  const char *root;
  int port;
} CliArgs;

int GetCliArgs(int argc, char *argv[], CliArgs *args);

#endif /* COMMUNICADO_TFTPD_ARGS_H_ */

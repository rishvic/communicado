#ifndef COMMUNICADO_TFTPD_CLI_H_
#define COMMUNICADO_TFTPD_CLI_H_

#define USAGE_FORMAT                                                    \
  "Usage: %s [OPTION] ...\n"                                            \
  "Run a TFTP server instance.\n"                                       \
  "\n"                                                                  \
  "  -h                         display this help and exit\n"           \
  "  -v                         output version information and exit\n"  \
  "  -d ROOT                    set filesystem root as ROOT%s\n"        \
  "  -p PORT                    set server port (default %d)\n"         \
  "\n"                                                                  \
  "The server reads/writes files, taking ROOT as its root directory.\n" \
  "\n"                                                                  \
  "Exit status:\n"                                                      \
  " %3d  if exits normally (e.g., with a single SIGINT),\n"             \
  " %3d  if any problem (e.g., cannot access directory),\n"             \
  " %3d  if exits forcefully due to multiple SIGINTs,\n"        \
  " %3d  if exits due to SIGTERM.\n"

#define VERSION_FORMAT \
  "%s\n"               \
  "tftpd: TFTP server\n"

void print_usage(const char *command);

void print_version(void);

#endif /* COMMUNICADO_TFTPD_CLI_H_ */

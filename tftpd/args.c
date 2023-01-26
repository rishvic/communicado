#include "args.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int GetCliArgs(int argc, char *argv[], CliArgs *args) {
  const char kOptions[] = ":hvd:p:";
  int c, err = 0;
  char *end;

  args->root = DEFAULT_ROOT_DIR;
  args->port = DEFAULT_PORT;
  args->help = 0;
  args->version = 0;

  while (err == 0 && (c = getopt(argc, argv, kOptions)) != -1) {
    switch (c) {
      case 'd':
        args->root = optarg;
        break;
      case 'p':
        errno = 0;
        args->port = strtoul(optarg, &end, 10);
        err = errno;
        if ((args->port == 0 && err == EINVAL) || *end != '\0') {
          fprintf(stderr, "%s: argument PORT: invalid int value: %s\n", argv[0],
                  optarg);
          err = -1;
        }
        break;

      case 'h':
        args->help = 1;
        break;
      case 'v':
        args->version = 1;
        break;

      case ':':
        fprintf(stderr, "%s: option '-%c' requires an argument\n", argv[0],
                optopt);
        err = -1;
        break;
      case '?':
        fprintf(stderr, "%s: invalid option -- '%c'\n", argv[0], optopt);
        err = -1;
        break;
    }
  }

  return err;
}

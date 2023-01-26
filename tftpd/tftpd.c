#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <stdlib.h>
#include <task/task.h>

#include "args.h"
#include "cli.h"
#include "server.h"

#define MESSAGE "Hello, TFTP!\n"

void taskmain(int argc, char *argv[]) {
  int err;
  CliArgs args;

  /* Command line argument processing */
  err = GetCliArgs(argc, argv, &args);
  if (err != 0) {
    fprintf(stderr, "Try '%s -h' for more information\n", argv[0]);
    taskexit(EXIT_FAILURE);
    return;
  }

  if (args.help) {
    print_usage(argv[0]);
    taskexit(EXIT_SUCCESS);
    return;
  }

  if (args.version) {
    print_version();
    taskexit(EXIT_SUCCESS);
    return;
  }

  /* Start up a server instance */
  err = StartServer(args.root, args.port, argv[0]);
  if (err != 0) {
    fprintf(stderr, "%s: failed while starting server\n", argv[0]);
    taskexit(EXIT_FAILURE);
    return;
  }

  taskexit(EXIT_SUCCESS);
  return;
}

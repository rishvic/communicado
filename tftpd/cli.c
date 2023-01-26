#include "cli.h"

#include <stdio.h>
#include <stdlib.h>

#include "args.h"
#include "config.h"

void print_usage(const char *command) {
  printf(USAGE_FORMAT, command, DEFAULT_ROOT_DIR, DEFAULT_PORT, EXIT_SUCCESS,
         EXIT_FAILURE);
}

void print_version(void) { printf(VERSION_FORMAT, PROJECT_STRING); }

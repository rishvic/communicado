#include "cli.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "args.h"
#include "config.h"

/* Temporarily generate default information macros for usage string. */
#ifdef TFTPD_DEFAULT_ROOT_DIR
#define DEFAULT_ROOT_DIR_INFO " (default " TFTPD_DEFAULT_ROOT_DIR ")"
#else
#define DEFAULT_ROOT_DIR_INFO ""
#endif

void print_usage(const char *command) {
  printf(USAGE_FORMAT, command, DEFAULT_ROOT_DIR_INFO, TFTPD_STANDART_PORT,
         EXIT_SUCCESS, EXIT_FAILURE, 128 + SIGINT, 128 + SIGTERM);
}

void print_version(void) { printf(VERSION_FORMAT, PROJECT_STRING); }

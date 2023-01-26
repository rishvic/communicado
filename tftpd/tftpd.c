#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <task/task.h>

#define MESSAGE "Hello, TFTP!\n"

void taskmain(int argc, char *argv[]) {
  fdnoblock(STDOUT_FILENO);
  fdwrite(STDOUT_FILENO, MESSAGE, strlen(MESSAGE));
}

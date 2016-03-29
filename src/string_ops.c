#include "string_ops.h"
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * A simple interface for saving strings to files and loading them.
 */

// save a string to the given file descriptor
void str_save(char *string, int fd){
  int len = strlen(string) + 1;
  write(fd, &len, sizeof(len));
  write(fd, string, len);
}

// load a string from the file descriptor
char *str_load(int fd){
  int len;
  read(fd, &len, sizeof(len));

  char *string = malloc(len);
  read(fd, string, len);
  return string;
}

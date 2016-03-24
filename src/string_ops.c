#include "string_ops.h"
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void str_save(char *string, int fd){
  int len = strlen(string) + 1;
  write(fd, &len, sizeof(len));
  write(fd, string, len);
}

char *str_load(int fd){
  int len;
  read(fd, &len, sizeof(len));

  char *string = malloc(len);
  read(fd, string, len);
  return string;
}

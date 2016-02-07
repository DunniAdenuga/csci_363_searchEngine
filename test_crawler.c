#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "crawler.h"

#define BUFFSIZE  512

int main(int argc, char **argv){
  char host[BUFFSIZE];
  char path[BUFFSIZE];

  printf("Enter host: ");
  fgets(host, BUFFSIZE, stdin);
  printf("Enter path: ");
  fgets(path, BUFFSIZE, stdin);

  // remove newlines
  host[strlen(host) - 1] = '\0';
  path[strlen(path) - 1] = '\0';

  char *page_code = get_page(host, path);
  printf("%s\n", page_code);
  free(page_code);
  return 0;
}

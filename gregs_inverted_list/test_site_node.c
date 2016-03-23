#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "site_node.h"

int main(){
  char *file = "save.bin";

  printf("Testing sn_create...\n");

  struct site_node *n1 = sn_create("www.bucknell.edu", "/node1", 5);
  struct site_node *n2 = sn_create("www.bucknell.edu", "/node2", 7);

  sn_display(n1);
  sn_display(n2);

  printf("Testing sn_destroy...\n");

  sn_destroy(n1);
  sn_destroy(n2);

  printf("Testing sn_create...\n");

  n1 = sn_create("www.bucknell.edu", "/node1", 5);
  n2 = sn_create("www.bucknell.edu", "/node2", 7);

  int fd = open(file, O_CREAT | O_RDWR | O_TRUNC, 0600);

  if(fd < 0)
    perror("File Open Error");

  printf("Testing sn_save...\n");

  sn_save(n1, fd);
  sn_save(n2, fd);

  printf("Testing sn_destroy...\n");

  sn_destroy(n1);
  sn_destroy(n2);

  close(fd);

  fd = open(file, O_RDWR);

  printf("Testing sn_load...\n");

  n1 = sn_load(fd);
  n2 = sn_load(fd);

  sn_display(n1);
  sn_display(n2);

  sn_destroy(n1);
  sn_destroy(n2);
}

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "word_node.h"

int main(){
  char *file = "save.bin";

  printf("Testing wn_create...\n");

  struct word_node *n1 = wn_create("Bucknell");
  sl_add_tail(wn_get_sites(n1), "www.bucknell.edu", "/bucknell1", 1);
  sl_add_tail(wn_get_sites(n1), "www.bucknell.org", "/bucknell2", 2);

  struct word_node *n2 = wn_create("Website");
  sl_add_tail(wn_get_sites(n2), "www.bucknell.edu", "/home1", 1);
  sl_add_tail(wn_get_sites(n2), "www.bucknell.org", "/home2", 2);

  wn_display(n1);
  wn_display(n2);

  printf("Testing wn_destroy...\n");

  wn_destroy(n1);
  wn_destroy(n2);

  printf("Testing wn_create...\n");

  n1 = wn_create("Bucknell");
  sl_add_tail(wn_get_sites(n1), "www.bucknell.edu", "/bucknell1", 1);
  sl_add_tail(wn_get_sites(n1), "www.bucknell.org", "/bucknell2", 2);


  n2 = wn_create("Website");
  sl_add_tail(wn_get_sites(n2), "www.bucknell.edu", "/home1", 1);
  sl_add_tail(wn_get_sites(n2), "www.bucknell.org", "/home2", 2);

  int fd = open(file, O_CREAT | O_RDWR | O_TRUNC, 0600);

  if(fd < 0)
    perror("File Open Error");

  printf("Testing wn_save...\n");

  wn_save(n1, fd);
  wn_save(n2, fd);

  printf("Testing wn_destroy...\n");

  wn_destroy(n1);
  wn_destroy(n2);

  close(fd);

  fd = open(file, O_RDWR);

  printf("Testing wn_load...\n");

  n1 = wn_load(fd);
  n2 = wn_load(fd);

  wn_display(n1);
  wn_display(n2);

  wn_destroy(n1);
  wn_destroy(n2);
}

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "word_list.h"

int main(){
  char *file = "save.bin";

  printf("Testing wl_create...\n");

  struct word_list *l = wl_create();

  printf("Adding three nodes...\n");

  wl_add(l, "Bucknell", "www.bucknell.edu", "/node1", 1);
  wl_add(l, "Bucknell", "www.bucknell.edu", "/node2", 2);

  wl_add(l, "Website", "www.bucknell.edu", "/home1", 1);
  wl_add(l, "Website", "www.bucknell.edu", "/home2", 2);

  wl_add(l, "Bucknell", "www.bucknell.edu", "/node3", 3);
  wl_add(l, "Website", "www.bucknell.edu", "/node3", 3);

  wl_display(l);

  printf("Testing wn_destroy...\n");

  wl_destroy(l);

  printf("Testing wl_create...\n");

  l = wl_create();

  wl_add(l, "Bucknell", "www.bucknell.edu", "/node1", 1);
  wl_add(l, "Bucknell", "www.bucknell.edu", "/node2", 2);

  wl_add(l, "Website", "www.bucknell.edu", "/home1", 1);
  wl_add(l, "Website", "www.bucknell.edu", "/home2", 2);

  wl_add(l, "Bucknell", "www.bucknell.edu", "/node3", 3);
  wl_add(l, "Website", "www.bucknell.edu", "/node3", 3);

  wl_display(l);


  int fd = open(file, O_CREAT | O_RDWR | O_TRUNC, 0600);

  if(fd < 0)
    perror("File Open Error");

  printf("Testing wl_save...\n");

  wl_save(l, fd);

  printf("Testing wn_destroy...\n");

  wl_destroy(l);

  close(fd);

  printf("Testing wn_load...\n");

  fd = open(file, O_RDWR);

  if(fd < 0)
    perror("File Open Error");

  l = wl_load(fd);

  wl_display(l);

  printf("Testing contains...\n");

  if(wl_contains(l, "Bucknell"))
      printf("   OK\n");
  if(wl_contains(l, "Website"))
      printf("   OK\n");
  if(!wl_contains(l, "Nothing"))
      printf("   OK\n");
  if(!wl_contains(l, "Something"))
      printf("   OK\n");

  printf("Test iter...\n");

  wl_display(l);

  
  wn_display(wl_iter_begin(l));
  wn_display(wl_iter_next(l));

  if(wl_iter_next(l) == NULL)
    printf("    OK on NULL\n");
  else
    printf("    ERROR on NULL\n");

  printf("Testing iter remove...\n");

  printf("    Removing first\n");
  wl_iter_begin(l);
  wl_iter_remove(l);
  wl_display(l);

  printf("    Restoring...\n");
  wl_add(l, "Website", "www.bucknell.edu", "/home1", 1);
  wl_add(l, "Website", "www.bucknell.edu", "/home2", 2);
  wl_add(l, "Website", "www.bucknell.edu", "/node3", 3);
  wl_display(l);

  printf("\n    Removing second\n");
  wl_iter_begin(l);
  wl_iter_next(l);
  wl_iter_remove(l);

  wl_display(l);

  wl_destroy(l);
}

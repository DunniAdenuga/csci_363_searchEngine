#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "site_list.h"

int main(){
  char *file = "save.bin";

  printf("Testing sl_create...\n");

  struct site_list *l = sl_create();

  printf("Adding four nodes...\n");

  sl_add_front(l, "www.bucknell.edu", "/node1", 1);
  sl_add_tail(l, "www.bucknell.edu", "/node2", 2);
  sl_add_front(l, "www.bucknell.edu", "/node3", 3);
  sl_add_tail(l, "www.bucknell.edu", "/node4", 4);

  sl_display(l);

  printf("Testing sn_destroy...\n");

  sl_destroy(l);

  printf("Testing sl_create...\n");

  l = sl_create();

  sl_add_tail(l, "www.bucknell.edu", "/node1", 1);
  sl_add_tail(l, "www.bucknell.edu", "/node2", 2);
  sl_add_tail(l, "www.bucknell.edu", "/node3", 3);
  sl_add_tail(l, "www.bucknell.edu", "/node4", 4);

  sl_display(l);


  int fd = open(file, O_CREAT | O_RDWR | O_TRUNC, 0600);

  if(fd < 0)
    perror("File Open Error");

  printf("Testing sl_save...\n");

  sl_save(l, fd);

  printf("Testing sn_destroy...\n");

  sl_destroy(l);

  close(fd);

  printf("Testing sn_load...\n");

  fd = open(file, O_RDWR);

  if(fd < 0)
    perror("File Open Error");

  l = sl_load(fd);

  sl_display(l);

  printf("Testing contains...\n");

  if(sl_contains(l, "www.bucknell.edu", "/node2"))
      printf("   OK\n");
  if(sl_contains(l, "www.bucknell.edu", "/node1"))
      printf("   OK\n");
  if(sl_contains(l, "www.bucknell.edu", "/node3"))
      printf("   OK\n");
  if(!sl_contains(l, "www.bucknell.org", "/node2"))
      printf("   OK\n");
  if(!sl_contains(l, "www.bucknell.edu", "/node4"))
      printf("   OK\n");

  printf("Test iter...\n");

  sn_display(sl_iter_begin(l));
  sn_display(sl_iter_next(l));
  sn_display(sl_iter_next(l));
  sn_display(sl_iter_next(l));
  if(sl_iter_next(l) == NULL)
    printf("    OK on NULL\n");
  else
    printf("    ERROR on NULL\n");

  printf("Testing iter remove...\n");

  printf("    Removing first\n");
  //sl_iter_begin(l);
  //sl_iter_remove(l);
  sl_remove_head(l);
  sl_display(l);

  sl_add_tail(l, "www.bucknell.edu", "/node3", 3);

  printf("\n    Removing second\n");
  sl_iter_begin(l);
  sl_iter_next(l);
  sl_iter_remove(l);

  sl_display(l);

  sl_destroy(l);
}

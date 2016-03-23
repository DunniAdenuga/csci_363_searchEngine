#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "word_list.h"

int main(){
  char *file = "save.bin";


  printf("\n\n##### Creating Dynamic #####\n\n");

  struct word_list *ld = wl_create();

  printf("Adding two nodes...\n");

  wl_add(ld, "Bucknell", "www.bucknell.edu", "/node1", 1);
  wl_add(ld, "Bucknell", "www.bucknell.edu", "/node2", 2);

  wl_add(ld, "Website", "www.website.edu", "/home1", 1);
  wl_add(ld, "Website", "www.website.edu", "/home2", 2);

  wl_add(ld, "Bucknell", "www.common.edu", "/node3", 3);
  wl_add(ld, "Website", "www.common.edu", "/node3", 3);

  wl_display(ld);



  printf("\n\n##### Converting to static #####\n\n");

  struct word_list_s *ls = wls_create(ld);
  wls_display(ls);


  printf("\n\n##### Searching list #####\n\n");

  printf("Searching for: Bucknell\n");
  struct word_node *node = wls_get_node(ls, "Bucknell");
  if(node == NULL){
    printf("    ERROR -> Node not found\n");
  }else{
    printf("    OK\n");
    wn_display(node);
  }

  printf("Searching for: Website\n");
  node = wls_get_node(ls, "Website");
  if(node == NULL){
    printf("    ERROR -> Node not found\n");
  }else{
    printf("    OK\n");
    wn_display(node);
  }

  printf("Searching for: Adam\n");
  node = wls_get_node(ls, "Adam");
  if(node == NULL){
    printf("    OK -> Node not found\n");
  }else{
    printf("    ERROR -> Node incorrectly found\n");
    wn_display(node);
  }

  printf("Searching for: Carol\n");
  node = wls_get_node(ls, "Carol");
  if(node == NULL){
    printf("    OK -> Node not found\n");
  }else{
    printf("    ERROR -> Node incorrectly found\n");
    wn_display(node);
  }

  printf("Searching for: You\n");
  node = wls_get_node(ls, "You");
  if(node == NULL){
    printf("    OK -> Node not found\n");
  }else{
    printf("    ERROR -> Node incorrectly found\n");
    wn_display(node);
  }



  printf("\n\n##### Destroying Static List #####\n\n");
  wls_destroy(ls);


  printf("\n\n##### Creating Dynamic #####\n\n");

  ld = wl_create();

  printf("Adding three nodes...\n");

  wl_add(ld, "Bucknell", "www.bucknell.edu", "/node1", 1);
  wl_add(ld, "Bucknell", "www.bucknell.edu", "/node2", 2);

  wl_add(ld, "Website", "www.website.edu", "/home1", 1);
  wl_add(ld, "Website", "www.website.edu", "/home2", 2);

  wl_add(ld, "Wiki", "www.wiki.edu", "/wiki1", 4);
  wl_add(ld, "Wiki", "www.wiki.edu", "/wiki2", 4);

  wl_add(ld, "Bucknell", "www.common.edu", "/node3", 3);
  wl_add(ld, "Website", "www.common.edu", "/node3", 3);
  wl_add(ld, "Wiki", "www.common.edu", "/node3", 3);


  wl_display(ld);


  
  printf("\n\n##### Converting to static #####\n\n");

  ls = wls_create(ld);
  wls_display(ls);


  printf("\n\n##### Searching list #####\n\n");

  printf("Searching for: Bucknell\n");
  node = wls_get_node(ls, "Bucknell");
  if(node == NULL){
    printf("    ERROR -> Node not found\n");
  }else{
    printf("    OK\n");
    wn_display(node);
  }

  printf("Searching for: Website\n");
  node = wls_get_node(ls, "Website");
  if(node == NULL){
    printf("    ERROR -> Node not found\n");
  }else{
    printf("    OK\n");
    wn_display(node);
  }

  printf("Searching for: Wiki\n");
  node = wls_get_node(ls, "Wiki");
  if(node == NULL){
    printf("    ERROR -> Node not found\n");
  }else{
    printf("    OK\n");
    wn_display(node);
  }

  printf("Searching for: Adam\n");
  node = wls_get_node(ls, "Adam");
  if(node == NULL){
    printf("    OK -> Node not found\n");
  }else{
    printf("    ERROR -> Node incorrectly found\n");
    wn_display(node);
  }

  printf("Searching for: Carol\n");
  node = wls_get_node(ls, "Carol");
  if(node == NULL){
    printf("    OK -> Node not found\n");
  }else{
    printf("    ERROR -> Node incorrectly found\n");
    wn_display(node);
  }

  printf("Searching for: Wednesday\n");
  node = wls_get_node(ls, "Wednesday");
  if(node == NULL){
    printf("    OK -> Node not found\n");
  }else{
    printf("    ERROR -> Node incorrectly found\n");
    wn_display(node);
  }

  printf("Searching for: You\n");
  node = wls_get_node(ls, "You");
  if(node == NULL){
    printf("    OK -> Node not found\n");
  }else{
    printf("    ERROR -> Node incorrectly found\n");
    wn_display(node);
  }



  
  printf("\n\n##### Checking Contains #####\n\n");

  if(wls_contains(ls, "Bucknell"))
      printf("   OK\n");
  if(wls_contains(ls, "Website"))
      printf("   OK\n");
  if(!wls_contains(ls, "Nothing"))
      printf("   OK\n");
  if(!wls_contains(ls, "Something"))
      printf("   OK\n");


  printf("\n\n##### Destroying List #####\n\n");
  wls_destroy(ls);



  printf("\n\n##### Creating Dynamic #####\n\n");

  ld = wl_create();

  printf("Adding three nodes...\n");

  wl_add(ld, "Bucknell", "www.bucknell.edu", "/node1", 1);
  wl_add(ld, "Website", "www.website.edu", "/home1", 1);
  wl_add(ld, "Something", "www.common.edu", "/node3", 3);
  wl_add(ld, "Carol", "www.common.edu", "/node3", 3);
  wl_add(ld, "Hilarious", "www.common.edu", "/node3", 3);
  wl_add(ld, "Wiki", "www.wiki.edu", "/wiki1", 4);
  wl_add(ld, "Elephant", "www.common.edu", "/node3", 3);


  wl_display(ld);

  
  printf("\n\n##### Converting to static #####\n\n");

  ls = wls_create(ld);
  wls_display(ls);



  printf("\n\n##### Testing Save #####\n\n");

  int fd = open(file, O_CREAT | O_RDWR | O_TRUNC, 0600);

  if(fd < 0)
    perror("File Open Error");

  wls_save(ls, fd);
  wls_destroy(ls);
  close(fd);

  printf("\n\n##### Testing Load #####\n\n");

  fd = open(file, O_RDWR);

  if(fd < 0)
    perror("File Open Error");

  ls = wls_load(fd);
  wls_display(ls);
  wls_destroy(ls);
}

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "inv_list.h"
#include "site_list.h"

int main(){
  char *file = "save.bin";


  printf("\n\n##### Creating Dynamic #####\n\n");

  struct inv_list *l = il_create();
  il_set_dynamic(l);

  printf("Adding two nodes...\n");

  il_add(l, "Bucknell", "www.bucknell.edu", "/node1", 1);
  il_add(l, "Bucknell", "www.bucknell.edu", "/node2", 2);

  il_add(l, "Website", "www.website.edu", "/home1", 1);
  il_add(l, "Website", "www.website.edu", "/home2", 2);

  il_add(l, "Bucknell", "www.common.edu", "/node3", 3);
  il_add(l, "Website", "www.common.edu", "/node3", 3);

  il_display(l);



  printf("\n\n##### Converting to static #####\n\n");

  il_set_static(l);
  il_display(l);


  printf("\n\n##### Searching list #####\n\n");

  printf("Searching for: Bucknell\n");
  struct site_list *sites = il_get_sites(l, "Bucknell");
  if(sites == NULL){
    printf("    ERROR -> Node not found\n");
  }else{
    printf("    OK\n");
    sl_display(sites);
  }

  printf("Searching for: Website\n");
  sites = il_get_sites(l, "Website");
  if(sites == NULL){
    printf("    ERROR -> Node not found\n");
  }else{
    printf("    OK\n");
    sl_display(sites);
  }

  printf("Searching for: Adam\n");
  sites = il_get_sites(l, "Adam");
  if(sites == NULL){
    printf("    OK -> Node not found\n");
  }else{
    printf("    ERROR -> Node incorrectly found\n");
    sl_display(sites);
  }

  printf("Searching for: Carol\n");
  sites = il_get_sites(l, "Carol");
  if(sites == NULL){
    printf("    OK -> Node not found\n");
  }else{
    printf("    ERROR -> Node incorrectly found\n");
    sl_display(sites);
  }

  printf("Searching for: You\n");
  sites = il_get_sites(l, "You");
  if(sites == NULL){
    printf("    OK -> Node not found\n");
  }else{
    printf("    ERROR -> Node incorrectly found\n");
    sl_display(sites);
  }



  printf("\n\n##### Destroying Static List #####\n\n");
  il_destroy(l);


  printf("\n\n##### Creating Dynamic #####\n\n");

  l = il_create();
  il_set_dynamic(l);

  printf("Adding three nodes...\n");

  il_add(l, "Bucknell", "www.bucknell.edu", "/node1", 1);
  il_add(l, "Bucknell", "www.bucknell.edu", "/node2", 2);

  il_add(l, "Website", "www.website.edu", "/home1", 6);
  il_add(l, "Website", "www.website.edu", "/home2", 7);

  il_add(l, "Wiki", "www.wiki.edu", "/wiki1", 4);
  il_add(l, "Wiki", "www.wiki.edu", "/wiki2", 5);

  il_add(l, "Bucknell", "www.common.edu", "/node3", 3);
  il_add(l, "Website", "www.common.edu", "/node3", 3);
  il_add(l, "Wiki", "www.common.edu", "/node3", 3);


  il_display(l);


  
  printf("\n\n##### Converting to static #####\n\n");

  il_set_static(l);
  il_display(l);


  printf("\n\n##### Searching list #####\n\n");

  printf("Searching for: Bucknell\n");
  sites = il_get_sites(l, "Website");
  if(sites == NULL){
    printf("    ERROR -> Node not found\n");
  }else{
    printf("    OK\n");
    sl_display(sites);
  }

  printf("Searching for: Website\n");
  sites = il_get_sites(l, "Website");
  if(sites == NULL){
    printf("    ERROR -> Node not found\n");
  }else{
    printf("    OK\n");
    sl_display(sites);
  }

  printf("Searching for: Wiki\n");
  sites = il_get_sites(l, "Wiki");
  if(sites == NULL){
    printf("    ERROR -> Node not found\n");
  }else{
    printf("    OK\n");
    sl_display(sites);
  }

  printf("Searching for: Adam\n");
  sites = il_get_sites(l, "Adam");
  if(sites == NULL){
    printf("    OK -> Node not found\n");
  }else{
    printf("    ERROR -> Node incorrectly found\n");
    sl_display(sites);
  }

  printf("Searching for: Carol\n");
  sites = il_get_sites(l, "Carol");
  if(sites == NULL){
    printf("    OK -> Node not found\n");
  }else{
    printf("    ERROR -> Node incorrectly found\n");
    sl_display(sites);
  }

  printf("Searching for: Wednesday\n");
  sites = il_get_sites(l, "Wednesday");
  if(sites == NULL){
    printf("    OK -> Node not found\n");
  }else{
    printf("    ERROR -> Node incorrectly found\n");
    sl_display(sites);
  }

  printf("Searching for: You\n");
  sites = il_get_sites(l, "You");
  if(sites == NULL){
    printf("    OK -> Node not found\n");
  }else{
    printf("    ERROR -> Node incorrectly found\n");
    sl_display(sites);
  }



  
  printf("\n\n##### Checking Contains #####\n\n");

  if(il_contains(l, "Bucknell"))
      printf("   OK\n");
  if(il_contains(l, "Website"))
      printf("   OK\n");
  if(!il_contains(l, "Nothing"))
      printf("   OK\n");
  if(!il_contains(l, "Something"))
      printf("   OK\n");


  printf("\n\n##### Destroying List #####\n\n");
  il_destroy(l);



  printf("\n\n##### Creating Dynamic #####\n\n");

  l = il_create();

  printf("Adding three nodes...\n");

  il_add(l, "Bucknell", "www.bucknell.edu", "/node1", 1);
  il_add(l, "Website", "www.website.edu", "/home1", 6);
  il_add(l, "Something", "www.common.edu", "/node3", 3);
  il_add(l, "Carol", "www.common.edu", "/node3", 3);
  il_add(l, "Hilarious", "www.common.edu", "/node3", 3);
  il_add(l, "Wiki", "www.wiki.edu", "/wiki1", 4);
  il_add(l, "Elephant", "www.common.edu", "/node3", 3);


  il_display(l);

  
  printf("\n\n##### Testing Save #####\n\n");

  int fd = open(file, O_CREAT | O_RDWR | O_TRUNC, 0600);

  if(fd < 0)
    perror("File Open Error");

  il_save(l, file);
  il_destroy(l);
  close(fd);

  printf("\n\n##### Testing Load #####\n\n");

  fd = open(file, O_RDWR);

  if(fd < 0)
    perror("File Open Error");

  l = il_load(file);
  il_display(l);
  il_destroy(l);
}

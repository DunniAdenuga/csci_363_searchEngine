#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "site_list.h"

// creates a new site list
struct site_list *sl_create(){
  struct site_list *l = malloc(sizeof(struct site_list));
  l->head = NULL;
  l->count = 0;
  return l;
}

// destroys a site list and frees as necessary
void sl_destroy(struct site_list *l){
  struct site_node *curr = l->head;
  while(curr != NULL){
    struct site_node *next = curr->next;
    sn_destroy(curr);
    curr = next;
  }
  free(l);
}

// loads a list from a file
struct site_list *sl_load(int fd){
  struct site_list *l = malloc(sizeof(struct site_list));
  read(fd, &l->count, sizeof(l->count));

  struct site_node *curr = NULL;
  struct site_node *prev = NULL;

  for(int i = 0; i < l->count; i++){
    curr = sn_load(fd);

    if(prev == NULL){
      l->head = curr;
    }else{
      sn_set_next(prev, curr);
    }
    prev = curr;
  }
  return l;
}

// saves a list to a file
void sl_save(struct site_list *l, int fd){
  write(fd, &l->count, sizeof(l->count));

  struct site_node *curr = l->head;
  while(curr != NULL){
    sn_save(curr, fd);
    curr = sn_get_next(curr);
  }
}

// adds a site to the list of sites
void sl_add(struct site_list *l, char *host, char *path){
  struct site_node *node = sn_create(host, path);
  sn_set_next(node, l->head);
  l->head = node;
  l->count += 1;
}

// remove a site from the list of sites
int sl_remove(struct site_list *l, char *host, char *path){
  sl_iter_begin(l);
  struct site_node *curr = sl_iter_begin(l);

  while(curr != NULL){
    if(strcmp(sn_get_host(curr), host) == 0 && strcmp(sn_get_path(curr), path)){
      return sl_iter_remove(l);
    }
    curr = sn_get_next(curr);
  }
  return 0;
}

// checks for containment within this list
int sl_contains(struct site_list *l, char *host, char *path){
  struct site_node *curr = l->head;
  while(curr != NULL){
    if(strcmp(host, sn_get_host(curr)) == 0 && strcmp(path, sn_get_path(curr)) == 0)
      return 1;

    curr = sn_get_next(curr);
  }
  return 0;
}

// begins iteration through the list
struct site_node *sl_iter_begin(struct site_list *l){
  l->iter = l->head;
  return l->iter;
}

// gets the next node in the list
struct site_node *sl_iter_next(struct site_list *l){
  if(l->iter != NULL){
    l->iter = sn_get_next(l->iter);
  }
  return l->iter;
}

// getst the current node in the iteration
struct site_node *sl_iter_curr(struct site_list *l){
  return l->iter;
}

// removes the current element of the iteration
int sl_iter_remove(struct site_list *l){
  // at the end of the list, do nothing
  if(l->iter == NULL)
    return 0;

  // at the beginning of the list, increment head
  if(l->iter == l->head){
    l->head = sn_get_next(l->head);
    sn_destroy(l->iter);
    l->iter = l->head;
    l->count -= 1;
    return 1;
  }

  // in the middle of the list, find prev and remove
  struct site_node *curr = l->head;
  while(sn_get_next(curr) != l->iter){
    curr = sn_get_next(curr);
  }

  sn_set_next(curr, sn_get_next(l->iter));
  sn_destroy(l->iter);
  l->iter = sn_get_next(curr);
  l->count -= 1;
  return 1;
}

// display the list
void sl_display(struct site_list *l){
  printf("================================\n");
  printf("List with %d nodes:\n", l->count);
  struct site_node *curr = l->head;
  while(curr != NULL){
    sn_display(curr);
    curr = sn_get_next(curr);
  }
  printf("================================\n");
}

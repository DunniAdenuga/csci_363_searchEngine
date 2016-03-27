#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include "inv_list.h"
#include "word_list.h"

// create a new inverted list
struct inv_list *il_create(){
  struct inv_list *l = calloc(1, sizeof(struct inv_list));

  l->ld = wl_create();
  l->dynamic = 1;
  return l;
}

// destroy the inverted list
void il_destroy(struct inv_list *l){
  if(l->dynamic){
    wl_destroy(l->ld);
  }else{
    wls_destroy(l->ls);
  }
  free(l);
}

// determines if two lists are equal
int il_is_equal(struct inv_list *l1, struct inv_list *l2){
  if(l1->dynamic != l2->dynamic)
    return 0;
  if(l1->dynamic && l2->dynamic)
    return wl_is_equal(l1->ld, l2->ld);
  return wls_is_equal(l1->ls, l2->ls);
}

// add a word site pair
int il_add(struct inv_list *l, char *word, char *host, char *path, int freq){
  if(!l->dynamic)
    return 0;
  wl_add(l->ld, word, host, path, freq);
  return 1;
}

// remove a word site pair
int il_remove(struct inv_list *l, char *word, char *host, char *path){
  if(!l->dynamic)
    return 0;
  return wl_remove(l->ld, word, host, path);
}

// remove a entire word
int il_remove_word(struct inv_list *l, char *word){
  if(!l->dynamic)
    return 0;
  return wl_remove_word(l->ld, word);
}

// check for word existence
int il_contains(struct inv_list *l, char *word){
  if(l->dynamic)
    return wl_contains(l->ld, word);
  return wls_contains(l->ls, word);
}

// get the list of sites for a word
struct site_list *il_get_sites(struct inv_list *l, char *word){
  struct word_node *node;

  if(l->dynamic)
    node = wl_get_node(l->ld, word);
  else
    node = wls_get_node(l->ls, word);

  if(node == NULL)
    return NULL;
  return wn_get_sites(node);
}

// state manipulation
void il_set_static(struct inv_list *l){
  if(!l->dynamic)
    return;
  l->ls = wls_create(l->ld);
  l->dynamic = 0;
}

void il_set_dynamic(struct inv_list *l){
  if(l->dynamic)
    return;
  l->ld = wls_revert(l->ls);
  l->dynamic = 1;
}

// saving a file
void il_save(struct inv_list *l, int fd){
  il_set_static(l);
  wls_save(l->ls, fd);
}

// loading a file
struct inv_list *il_load(int fd){
  struct inv_list *l = malloc(sizeof(struct inv_list));

  l->ld = NULL;
  l->ls = wls_load(fd);
  l->dynamic = 0;

  return l;
}

// display
void il_display(struct inv_list *l){
  if(l->dynamic)
    wl_display(l->ld);
  else
    wls_display(l->ls);
}

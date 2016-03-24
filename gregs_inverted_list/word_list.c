#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "word_list.h"
#include "site_node.h"

//----------------
// Dynamic list
//----------------

// create a new word list
struct word_list *wl_create(){
  struct word_list *l = calloc(1, sizeof(struct word_list));
  return l;
}

// destroy a current list
void wl_destroy(struct word_list *l){
  struct word_node *curr = l->head;

  while(curr != NULL){
    struct word_node *next = wn_get_next(curr);
    wn_destroy(curr);
    curr = next;
  }

  free(l);
}

// load a list from the given file descriptor
struct word_list *wl_load(int fd){
  struct word_list *l = calloc(1, sizeof(struct word_list));

  struct word_node *curr = NULL;
  struct word_node *prev = NULL;

  read(fd, &l->count, sizeof(l->count));

  for(int i = 0; i < l->count; i++){
    curr = wn_load(fd);

    if(prev == NULL){
      l->head = curr;
    }else{
      wn_set_next(prev, curr);
    }
    prev = curr;
    curr = wn_get_next(curr);
  }
  return l;
}

// save a list to a file
void wl_save(struct word_list *l, int fd){
  write(fd, &l->count, sizeof(l->count));

  struct word_node *curr = l->head;
  while(curr != NULL){
    wn_save(curr, fd);
    curr = wn_get_next(curr);
  }
}

// adding a word with no check
void wl_add_word(struct word_list *l, char *word){
  struct word_node *node = wn_create(word);
  node->next = l->head;
  l->head = node;
  l->count += 1;
}

// add an occurance of a word
void wl_add(struct word_list *l, char *word, char *host, char *path, int freq){
  struct word_node *curr = l->head;

  while(curr != NULL){
    if(strcmp(wn_get_word(curr), word) == 0){
      struct site_list *sl = wn_get_sites(curr);
      sl_add_tail(sl, host, path, freq);
      return;
    }
    curr = wn_get_next(curr);
  }

  wl_add_word(l, word);
  wl_add(l, word, host, path, freq);
}

// remove an occurrance of a word site pair
int wl_remove(struct word_list *l, char *word, char *host, char *path){
  struct word_node *node = wl_iter_begin(l);
  while(node != NULL){
    if(strcmp(wn_get_word(node), word) == 0){
      return sl_remove(wn_get_sites(node), host, path);
    }
    node = wn_get_next(node);
  }
  return 0;
}

// removes a word from the list
int wl_remove_word(struct word_list *l, char *word){
  struct word_node *node = wl_iter_begin(l);
  while(node != NULL){
    if(strcmp(wn_get_word(node), word) == 0){
      return wl_iter_remove(l);
    }
    node = wn_get_next(node);
  }
  return 0;
}

// check for the existence of a word
int wl_contains(struct word_list *l, char *word){
  if(wl_get_node(l, word) == NULL)
    return 0;
  return 1;
}

// find a node for the given word
struct word_node *wl_get_node(struct word_list *l, char *word){
  struct word_node *curr = l->head;
  while(curr != NULL){
    if(strcmp(wn_get_word(curr), word) == 0)
      return curr;
    curr = wn_get_next(curr);
  }
  return NULL;
}

// initialize iteration
struct word_node *wl_iter_begin(struct word_list *l){
  l->iter = l->head;
  return l->iter;
}

// move to next iteration item
struct word_node *wl_iter_next(struct word_list *l){
  if(l->iter != NULL){
    l->iter = wn_get_next(l->iter);
  }
  return l->iter;
}

// return the current iteration item
struct word_node *wl_iter_curr(struct word_list *l){
  return l->iter;
}

// remove the current iteration object
int wl_iter_remove(struct word_list *l){
  // at the end of the list, do nothing
  if(l->iter == NULL)
    return 0;

  // at the beginning of the list, increment head
  if(l->iter == l->head){
    l->head = wn_get_next(l->head);
    wn_destroy(l->iter);
    l->iter = l->head;
    l->count -= 1;
    return 1;
  }

  // in the middle of the list, find prev and remove
  struct word_node *curr = l->head;
  while(wn_get_next(curr) != l->iter){
    curr = wn_get_next(curr);
  }

  wn_set_next(curr, wn_get_next(l->iter));
  wn_destroy(l->iter);
  l->iter = wn_get_next(curr);
  l->count -= 1;
  return 1;
}

// display
void wl_display(struct word_list *l){
  printf("/////////////////////////////////\n");
  printf("Word List with %d Nodes\n", l->count);
  printf("/////////////////////////////////\n");

  struct word_node *curr = l->head;
  while(curr != NULL){
    wn_display(curr);
    curr = wn_get_next(curr);
  }
  printf("\n\n");
}

//----------------
// Static list
//----------------

char *wls_get_word(struct word_list_s *l, int index){
  return wn_get_word(*(l->list + index));
}

// creates a static list
struct word_list_s *wls_create(struct word_list *l){
  // allocate the static list
  struct word_list_s *ls = malloc(sizeof(struct word_list_s));

  // setup the new list
  ls->count = l->count;
  ls->list = calloc(ls->count, sizeof(struct word_node*));

  // copy node pointers into list
  struct word_node *tmp = l->head;
  for(int i = 0; i < l->count; i++){
    *(ls->list + i) = tmp;
    tmp = wn_get_next(tmp);
  }

  // bubble sort the list (simple and only once)
  for(int i = 0; i < l->count - 1; i++){
    for(int j = 0; j < l->count - (i + 1); j++){
      if(strcmp(wls_get_word(ls, j), wls_get_word(ls, j+1)) > 0){
        tmp = *(ls->list + j);
        *(ls->list + j) = *(ls->list + j+1);
        *(ls->list + j+1) = tmp;
      }
    }
  }

  free(l);  // the dynamic list is no longer usable
  return ls;
}

// revert static list back to dynamic list
struct word_list *wls_revert(struct word_list_s *ls){
  struct word_list *ld = calloc(1, sizeof(struct word_list));
  ld->head = *(ls->list);

  for(int i = 1; i < ls->count; i++){
      wn_set_next(*(ls->list + i-1), *(ls->list + i));
  }

  free(ls);
  return ld;
}

// destroy the list freeing memory as needed
void wls_destroy(struct word_list_s *l){
  for(int i = 0; i < l->count; i++){
    wn_destroy(*(l->list + i));
  }
  free(l->list);
  free(l);
}

// load a list from the given file descriptor
struct word_list_s *wls_load(int fd){
  struct word_list_s *l = malloc(sizeof(struct word_list_s));

  read(fd, &l->count, sizeof(l->count));
  l->list = calloc(l->count, sizeof(struct word_node*));

  for(int i = 0; i < l->count; i++){
    *(l->list + i) = wn_load(fd);
  }

  return l;
}

// save a list to a file
void wls_save(struct word_list_s *l, int fd){
  write(fd, &l->count, sizeof(l->count));

  for(int i = 0; i < l->count; i++){
    wn_save(*(l->list + i), fd);
  }
}


// search the list (binary search)
struct word_node *wls_get_node(struct word_list_s *l, char *word){
  int a = 0;
  int b = l->count - 1;

  while(a < b){
    int c = (b + a)/2;
    int compare = strcmp(wls_get_word(l, c), word);

    if(compare == 0){
      return *(l->list + c);
    }else if(compare < 0){
      a = c + 1;
    }else{
      b = c;
    }
  }

  if(strcmp(wls_get_word(l, a), word) == 0){
    return *(l->list + a);
  }
  return NULL;
}

// check for the words existence 
int wls_contains(struct word_list_s *l, char *word){
  return (wls_get_node(l, word) != NULL);
}

// display
void wls_display(struct word_list_s *l){
  printf("/////////////////////////////////\n");
  printf("Static Word List with %d Nodes\n", l->count);
  printf("/////////////////////////////////\n");

  for(int i = 0; i < l->count; i++){
    wn_display(l->list[i]);
  }
  printf("\n\n");
}


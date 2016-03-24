#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "site_list.h"

// creates a new site list
struct site_list *sl_create(){
  struct site_list *l = malloc(sizeof(struct site_list));
  l->head = NULL;
  l->tail = NULL;
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

// determines if two site lists are equal
int sl_is_equal(struct site_list *l1, struct site_list *l2){
  if(l1->count != l2->count)
    return 0;

  struct site_node *s1 = sl_iter_begin(l1);
  struct site_node *s2 = sl_iter_begin(l2);

  while(s1 != NULL && s2 != NULL){
    if(!sn_is_equal(s1, s2))
      return 0;

    s1 = sl_iter_next(l1);
    s2 = sl_iter_next(l2);
  }

  if(s1 == NULL && s2 == NULL)
    return 1;
  return 0;
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
      sn_set_prev(curr, prev);
    }
    prev = curr;
  }

  l->tail = curr;
  return l;
}

// saves a list to a file
void sl_save(struct site_list *l, int fd){
  write(fd, &l->count, sizeof(l->count));

  struct site_node *curr = l->head;
  while(curr != NULL){
    sn_save(curr, fd);
    curr = curr->next;
  }
}

// adds this word to the list appropriately whether it exists already or not
int sl_add(struct site_list *l, char *host, char *path, int frequency){
  struct site_node *curr = l->head;
  while(curr != NULL){
    if(strcmp(host, curr->host) == 0 && strcmp(path, curr->path) == 0){
      curr->frequency += frequency;
      return curr->frequency;
    }
    curr = curr->next;
  }

  sl_add_tail(l, host, path, frequency);
  return frequency;
}

// adds a site to the front of the site list
void sl_add_front(struct site_list *l, char *host, char *path, int frequency){
  struct site_node *node = sn_create(host, path, frequency);

  sn_set_next(node, l->head);

  if(l->head == NULL){
    l->tail = node;
  }else{
    sn_set_prev(l->head, node);
    sn_set_next(node, l->head);
  }

  l->head = node;
  l->count += 1;
}

// adds a site to the tail of the site list
void sl_add_tail(struct site_list *l, char *host, char *path, int frequency){
  struct site_node *node = sn_create(host, path, frequency);

  if(l->tail == NULL){
    l->head = node;
  }else{
    sn_set_prev(node, l->tail);
    sn_set_next(l->tail, node);
  }

  l->tail = node;
  l->count += 1;
}

// remove the front node from the list
int sl_remove_head(struct site_list *l){
  struct site_node *head = l->head;
  if(l->count == 0){
    return 0;
  }else if(l->count == 1){
    l->head = NULL;
    l->tail = NULL;
  }else{
    l->head->next->prev = NULL;
    l->head = l->head->next;
  }

  l->count -= 1;
  sn_destroy(head);
  return 1;
}

// remove the tail node from the list
int sl_remove_tail(struct site_list *l){
  struct site_node *tail = l->tail;

  if(l->count == 0){
    return 0;
  }else if(l->count == 1){
    l->head = NULL;
    l->tail = NULL;
  }else{
    l->tail->prev->next = NULL;
    l->tail = l->tail->prev;
  }

  l->count -= 1;
  sn_destroy(tail);
  return 1;
}

// remove a site from the list of sites
int sl_remove(struct site_list *l, char *host, char *path){
  sl_iter_begin(l);
  struct site_node *curr = sl_iter_begin(l);

  while(curr != NULL){
    if(strcmp(curr->host, host) == 0 && strcmp(curr->path, path) == 0){
      return sl_iter_remove(l);
    }
    curr = curr->next;
  }
  return 0;
}

// checks for containment within this list
int sl_contains(struct site_list *l, char *host, char *path){
  struct site_node *curr = l->head;
  while(curr != NULL){
    if(strcmp(host, curr->host) == 0 && strcmp(path, curr->path) == 0)
      return 1;

    curr = curr->next;
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
    l->iter = l->iter->next;
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
    return sl_remove_head(l);
  }else if(l->iter == l->tail){
    return sl_remove_tail(l);
  }else{
    l->iter->prev->next = l->iter->next;
    l->iter->next->prev = l->iter->prev;
  }

  l->count -= 1;
  sn_destroy(l->iter);
  return 1;
}

// display the list
void sl_display(struct site_list *l){
  printf("================================\n");
  printf("List with %d nodes:\n", l->count);
  struct site_node *curr = l->head;
  while(curr != NULL){
    sn_display(curr);
    curr = curr->next;
  }
  printf("================================\n");
}

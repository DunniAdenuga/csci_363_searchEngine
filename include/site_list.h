#ifndef __sitelist__h__
#define __sitelist__h__

#include "site_node.h"

/**
 * This set of functions provides an interface to a doubly linked list of
 * websites and frequencies. Its intent is to be used in an inverted list to
 * keep track of sites associated with a word.
 */

// a linked list of sites
struct site_list{
  struct site_node *head;
  struct site_node *tail;
  struct site_node *iter;
  int count;
};

// creates a new list
struct site_list *sl_create();
// frees memory appropriatly for the list
void sl_destroy(struct site_list *l);
// test for equality
int sl_is_equal(struct site_list *l1, struct site_list *l2);


// creates a list from the provided file descriptor
struct site_list *sl_load(int fd);
// saves a list at the provided file descriptor
void sl_save(struct site_list *l, int fd);


// adds a website/frequency combination to the list incrementing an existing
// frequency by "frequency" if the site already exists (avoids duplication)
int sl_add(struct site_list *l, char *host, char *path, int frequency);


// adds website/frequency combinations to the list
void sl_add_front(struct site_list *l, char *host, char *path, int frequency);
void sl_add_tail(struct site_list *l, char *host, char *path, int frequency);


// removes websites from the list
int sl_remove_head(struct site_list *l);
int sl_remove_tail(struct site_list *l);
int sl_remove(struct site_list *l, char *host, char *path);


// test for containment
int sl_contains(struct site_list *l, char *host, char *path);


// functions for iteration
struct site_node *sl_iter_begin(struct site_list *l);
struct site_node *sl_iter_next(struct site_list *l);
struct site_node *sl_iter_curr(struct site_list *l);
int sl_iter_remove(struct site_list *l);

// displays the list
void sl_display(struct site_list *l);

#endif

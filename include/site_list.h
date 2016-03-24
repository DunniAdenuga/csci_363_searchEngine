#ifndef __sitelist__h__
#define __sitelist__h__

#include "site_node.h"

// a linked list of sites
struct site_list{
  struct site_node *head;
  struct site_node *tail;
  struct site_node *iter;
  int count;
};

// management
struct site_list *sl_create();
void sl_destroy(struct site_list *l);
int sl_is_equal(struct site_list *l1, struct site_list *l2);

// file io
struct site_list *sl_load();
void sl_save(struct site_list *l, int fd);

// element manipulation
int sl_add(struct site_list *l, char *host, char *path, int frequency);

void sl_add_front(struct site_list *l, char *host, char *path, int frequency);
void sl_add_tail(struct site_list *l, char *host, char *path, int frequency);

int sl_remove_head(struct site_list *l);
int sl_remove_tail(struct site_list *l);

int sl_remove(struct site_list *l, char *host, char *path);

// check list state
int sl_contains(struct site_list *l, char *host, char *path);

// iteration through elements
struct site_node *sl_iter_begin(struct site_list *l);
struct site_node *sl_iter_next(struct site_list *l);
struct site_node *sl_iter_curr(struct site_list *l);
int sl_iter_remove(struct site_list *l);

// display
void sl_display(struct site_list *l);

#endif

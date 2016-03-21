#ifndef __wordnode__h__
#define __wordnode__h__

#include "site_list.h"
#include "site_node.h"

struct word_node{
  char *word;
  struct site_list *sites;
  struct word_node *next;
};

// creation and destruction
struct word_node *wn_create(char *word);
void wn_destroy(struct word_node *node);

// save/load from file
struct word_node *wn_load(int fd);
void wn_save(struct word_node *node, int fd);

// setters
void wn_set_next(struct word_node *node, struct word_node *next);

// getters
char *wn_get_word(struct word_node *node);
struct site_list *wn_get_sites(struct word_node *node);
struct word_node *wn_get_next(struct word_node *node);

// display
void wn_display(struct word_node *node);

#endif

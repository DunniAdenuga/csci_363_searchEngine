#ifndef __wordnode__h__
#define __wordnode__h__

#include "site_list.h"
#include "site_node.h"

/**
 * These functions provide an interface to a word node. This node associates a
 * word with a list of websites and a frequency for each.
 */

struct word_node{
  char *word;
  struct site_list *sites;
  struct word_node *next;
};

// creates a new word_node
struct word_node *wn_create(char *word);
// frees the memory of a word node
void wn_destroy(struct word_node *node);
// test for equality
int wn_is_equal(struct word_node *n1, struct word_node *n2);

// save and load a node using the file descriptor
struct word_node *wn_load(int fd);
void wn_save(struct word_node *node, int fd);

// sets the next node in the list (may not be needed)
void wn_set_next(struct word_node *node, struct word_node *next);

// returns the specified element of the struct (may not be needed)
char *wn_get_word(struct word_node *node);
struct site_list *wn_get_sites(struct word_node *node);
struct word_node *wn_get_next(struct word_node *node);

// displays the list
void wn_display(struct word_node *node);

#endif

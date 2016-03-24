#ifndef __WORDLIST_H_
#define __WORDLIST_H_

#include "word_node.h"

//-----------------------------
// Dynamic list of words
//-----------------------------

struct word_list{
  struct word_node *head;
  struct word_node *iter;
  int count;
};

// Creation and appending
struct word_list *wl_create();
void wl_destroy(struct word_list *l);
int wl_is_equal(struct word_list *l1, struct word_list *l2);

// save/load from file
struct word_list *wl_load(int fd);
void wl_save(struct word_list *l, int fd);

// alterations
void wl_add(struct word_list *l, char *word, char *host, char *path, int freq);
int wl_remove(struct word_list *l, char *word, char *host, char *path);
int wl_remove_word(struct word_list *l, char *word);

// searching
int wl_contains(struct word_list *l, char *word);
struct word_node *wl_get_node(struct word_list *l, char *word);

// iteration through elements
struct word_node *wl_iter_begin(struct word_list *l);
struct word_node *wl_iter_next(struct word_list *l);
struct word_node *wl_iter_curr(struct word_list *l);
int wl_iter_remove(struct word_list *l);

// display
void wl_display(struct word_list *l);


//---------------------------
// Static list of words
//---------------------------

struct word_list_s{
  struct word_node **list;
  int count;
};

// creates a static list (dynamic one is no longer usable)
struct word_list_s *wls_create(struct word_list *l);
struct word_list *wls_revert(struct word_list_s *l);
void wls_destroy(struct word_list_s *l);
int wls_is_equal(struct word_list_s *l1, struct word_list_s *l2);

// search the list
struct word_node *wls_get_node(struct word_list_s *l, char *word);
int wls_contains(struct word_list_s *l, char *word);

// save/load from file
struct word_list_s *wls_load(int fd);
void wls_save(struct word_list_s *l, int fd);

// display
void wls_display(struct word_list_s *l);

#endif

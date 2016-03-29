#ifndef __WORDLIST_H_
#define __WORDLIST_H_

#include "word_node.h"

/**
 * These functions provide interfaces to two types of lists with the purpose of
 * associating words with site_list lists. The first is a linked list which can
 * be altered easily and the second is a static list which can be searched
 * quickly. The interface also provides a means to convert one form into the
 * other.
 */


//---------------------------
// Dynamic List of words
//---------------------------

struct word_list{
  struct word_node *head;
  struct word_node *iter;
  int count;
};

// create a dynamic list
struct word_list *wl_create();
// destroy a dynamic list
void wl_destroy(struct word_list *l);
// test for equality of dynamic lists
int wl_is_equal(struct word_list *l1, struct word_list *l2);

// save and load a dynamic list using the file descriptor
struct word_list *wl_load(int fd);
void wl_save(struct word_list *l, int fd);


// add a new word/site combination (this avoids duplicates and updates existing as necessary)
void wl_add(struct word_list *l, char *word, char *host, char *path, int freq);
// disassociates a website from a word
int wl_remove(struct word_list *l, char *word, char *host, char *path);
// removes a word from the list entirely
int wl_remove_word(struct word_list *l, char *word);


// checks for the containment of a word in the list
int wl_contains(struct word_list *l, char *word);
// returns the word_node for the given word, NULL if it does not exist
struct word_node *wl_get_node(struct word_list *l, char *word);


// iteration through the list
struct word_node *wl_iter_begin(struct word_list *l);
struct word_node *wl_iter_next(struct word_list *l);
struct word_node *wl_iter_curr(struct word_list *l);
int wl_iter_remove(struct word_list *l);

// displays the list
void wl_display(struct word_list *l);


//---------------------------
// Static list of words
//---------------------------

struct word_list_s{
  struct word_node **list;
  int count;
};

// creates a new static list from a dynamic one (can't alter until reverted)
struct word_list_s *wls_create(struct word_list *l);
// converts a static list back into a dynamic one
struct word_list *wls_revert(struct word_list_s *l);
// frees the memory associated with a list
void wls_destroy(struct word_list_s *l);
// tests for equality
int wls_is_equal(struct word_list_s *l1, struct word_list_s *l2);


// returns the node of a given word, NULL if it does not exist
struct word_node *wls_get_node(struct word_list_s *l, char *word);
// checks for the words containment in the list
int wls_contains(struct word_list_s *l, char *word);

// save and load a static list using the file descriptor
struct word_list_s *wls_load(int fd);
void wls_save(struct word_list_s *l, int fd);

// displays a static list
void wls_display(struct word_list_s *l);

#endif

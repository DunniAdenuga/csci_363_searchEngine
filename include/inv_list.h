#ifndef __INVLIST__H__
#define __INVLIST__H__

/**
 * These functions provide and interface to an inverted list. The list is
 * indexed using words in order to retrieve lists of websites with which the
 * word is associated. 
 *
 * The list has two modes. First, the dynamic mode is used when elements are
 * being added to the list. In this state, a linked list is used to enable easy
 * insertion of elements into the list. The second mode is static. This state
 * converts the linked list into an array so that the words can be searched
 * using a binary search. Note that attempted addition while the list is in
 * static mode will have no effect. Searching is possible while in dynamic
 * mode, but the search will be much slower (generally) than it would be in
 * static mode.
 */

struct inv_list{
  struct word_list *ld;
  struct word_list_s *ls;
  int dynamic;
};

// creates a new inverted list
struct inv_list *il_create();
// destroys an existing inverted list by freeing all allocated memory
void il_destroy(struct inv_list *l);
// determines equality between two lists
int il_is_equal(struct inv_list *l1, struct inv_list *l2);


// adds an occurrance of a word in the specified site with frequency
int il_add(struct inv_list *l, char *word, char *host, char *path, int freq);
// causes a site to no longer be associated with a word
int il_remove(struct inv_list *l, char *word, char *host, char *path);
// removes a word entirely from the list
int il_remove_word(struct inv_list *l, char *word);


// tests for containment
int il_contains(struct inv_list *l, char *word);
// returns the sites associated with a word
struct site_list *il_get_sites(struct inv_list *l, char *word);


// converts to the static state
void il_set_static(struct inv_list *l);
// converts to the dynamic state
void il_set_dynamic(struct inv_list *l);

// saves the current stat of the list to the current position of fd
void il_save(struct inv_list *l, int fd);
// creates a new list from the file beginning at the current position of fd
struct inv_list *il_load(int fd);


// displays the list to the console
void il_display(struct inv_list *l);

#endif

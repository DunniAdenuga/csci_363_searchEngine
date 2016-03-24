#ifndef __INVLIST__H__
#define __INVLIST__H__

struct inv_list{
  struct word_list *ld;
  struct word_list_s *ls;
  int dynamic;
};

// creation and destruction
struct inv_list *il_create();
void il_destroy(struct inv_list *l);
int il_is_equal(struct inv_list *l1, struct inv_list *l2);

// content manipulation
int il_add(struct inv_list *l, char *word, char *host, char *path, int freq);
int il_remove(struct inv_list *l, char *word, char *host, char *path);
int il_remove_word(struct inv_list *l, char *word);

// searching
int il_contains(struct inv_list *l, char *word);
struct site_list *il_get_sites(struct inv_list *l, char *word);

// state manipulation
void il_set_static(struct inv_list *l);
void il_set_dynamic(struct inv_list *l);

// persistence
void il_save(struct inv_list *l, int fd);
struct inv_list *il_load(int fd);

// display
void il_display(struct inv_list *l);

#endif

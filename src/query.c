#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "query.h"
#include "site_list.h"

#define MAXWORDS 20

void append_site_list(struct site_list *dst, struct site_list *src);
struct site_list *join_site_lists(struct site_list **site_lists, int count);
struct site_list **get_site_lists(struct query_interface *q, char **word_list, int count);
void destroy_site_lists(struct site_list **lists, int count);
void destroy_word_list(char **words, int count);
char **get_word_list(char *expression, int count);
int get_word_count(char *expression);

struct query_interface *qi_create(struct inv_list *il){
  struct query_interface *q = calloc(1, sizeof(struct query_interface));
  q->il = il;
  return q;
}

void qi_destroyi(struct query_interface *q) {
	il_destroy(q->il);
  free(q);
}

struct site_list *qi_query_expression(struct query_interface *q, char *expression) {
  int count = get_word_count(expression);
  char **word_list = get_word_list(expression, count);
  struct site_list **site_lists = get_site_lists(q, word_list, count);
	struct site_list *master_list = join_site_lists(site_lists, count);

  destroy_site_lists(site_lists, count);
  destroy_word_list(word_list, count);
	return master_list;
}

struct site_list *qi_query_word(struct query_interface *q, char *word){
  struct site_list *s = il_get_sites(q->il, word);
  if(s == NULL || s->count == 0)
    return sl_create();
  return s;
}

void destroy_query_results(struct site_list *s){
  sl_destroy(s);
}

struct site_list *join_site_lists(struct site_list **site_lists, int count){
  struct site_list *master_list = sl_create();
  for(int i = 0; i < count; i++){
    append_site_list(master_list, site_lists[i]);
  }
  return master_list;
}

void append_site_list(struct site_list *dst, struct site_list *src){
  struct site_node *current = sl_iter_begin(src);
  while(current != NULL){
    sl_add(dst, current->host, current->path, 1);
    current = sl_iter_next(src);
  }
}

struct site_list **get_site_lists(struct query_interface *q, char **word_list, int count){
  struct site_list **lists = calloc(count, sizeof(struct site_list *));
  for(int i = 0; i < count; i++){
    lists[i] = qi_query_word(q, word_list[i]);
    printf("Found Sites:\n");
    sl_display(lists[i]);
  }
  return lists;
}

void destroy_site_lists(struct site_list **lists, int count){
  // don't destroy the lists pointed to because they are sitll in q->il
  for(int i = 0; i < count; i++){
    if(lists[i]->count == 0){
      sl_destroy(lists[i]);
    }
    free(lists);
  }
}

void destroy_word_list(char **words, int count){
  for(int i = 0; i < count; i++){
    free(words[i]);
  }
  free(words);
}

char **get_word_list(char *expression, int count){
  char **words = calloc(count, sizeof(char *));
  char *word;

  for(int i = 0; i < count; i++){
    if(i == 0)
      word = strtok(expression, " ");
    else
      word = strtok(NULL, " ");
    words[i] = calloc(1, strlen(word));
    strcpy(words[i], word);
  }
  return words;
}

int get_word_count(char *expression){
  int count = 0;
  char *str = strtok(expression, " ");
  while(str != NULL){
    count++;
    str = strtok(NULL, " ");
  }
  return count;
}

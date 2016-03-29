#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "query.h"
#include "site_list.h"

#define MAXWORDS 20

/**
 * An interface for querying an inverted list
 */

// manipulation of site lists found during query
static void append_site_list(struct site_list *dst, struct site_list *src);
static struct site_list *join_site_lists(struct site_list **site_lists, int count);
static struct site_list **get_site_lists(struct query_interface *q, char **word_list, int count);
static void destroy_site_lists(struct site_list **lists, int count);

// manipulation of words and word_lists created during query
static void destroy_word_list(char **words, int count);
static char **get_word_list(char *expression, int count);
static int get_word_count(char *expression);

// creates a new query interface
struct query_interface *qi_create(struct inv_list *il){
  struct query_interface *q = calloc(1, sizeof(struct query_interface));
  q->il = il;
  return q;
}

// frees the memory of the query interface
void qi_destroyi(struct query_interface *q) {
	il_destroy(q->il);
  free(q);
}

// retrieves a list of all sites associated with all words in an expression
struct site_list *qi_query_expression(struct query_interface *q, char *expression) {
  int count = get_word_count(expression);
  char **word_list = get_word_list(expression, count);
  struct site_list **site_lists = get_site_lists(q, word_list, count);
	struct site_list *master_list = join_site_lists(site_lists, count);

  destroy_site_lists(site_lists, count);
  destroy_word_list(word_list, count);
	return master_list;
}

// retrieves a list of sites associated with the given word
struct site_list *qi_query_word(struct query_interface *q, char *word){
  struct site_list *s = il_get_sites(q->il, word);
  if(s == NULL || s->count == 0)
    return sl_create();
  return s;
}

// frees memory allocated for query results
void qi_destroy_query_results(struct site_list *s){
  sl_destroy(s);
}

// combines a list of site lists
struct site_list *join_site_lists(struct site_list **site_lists, int count){
  struct site_list *master_list = sl_create();
  for(int i = 0; i < count; i++){
    append_site_list(master_list, site_lists[i]);
  }
  return master_list;
}

// adds one site list to another
void append_site_list(struct site_list *dst, struct site_list *src){
  struct site_node *current = sl_iter_begin(src);
  while(current != NULL){
    sl_add(dst, current->host, current->path, 1);
    current = sl_iter_next(src);
  }
}

// retrieves a list of site lists associated with a list of words
struct site_list **get_site_lists(struct query_interface *q, char **word_list, int count){
  struct site_list **lists = calloc(count, sizeof(struct site_list *));
  for(int i = 0; i < count; i++){
    lists[i] = qi_query_word(q, word_list[i]);
  }
  return lists;
}

// frees the memory of a list of site lists created by "get_site_lists"
void destroy_site_lists(struct site_list **lists, int count){
  // don't destroy the lists pointed to because they are sitll in q->il
  for(int i = 0; i < count; i++){
    if(lists[i]->count == 0){
      sl_destroy(lists[i]);
    }
  }
  free(lists);
}

// frees the memory of a list of words created by "get_word_list"
void destroy_word_list(char **words, int count){
  for(int i = 0; i < count; i++){
    free(words[i]);
  }
  free(words);
}

// returns a list of words found in expression
char **get_word_list(char *expression, int count){
  char *exp_copy = malloc(strlen(expression) + 1);
  strcpy(exp_copy, expression);

  char **words = calloc(count, sizeof(char *));
  char *word;

  for(int i = 0; i < count; i++){
    if(i == 0)
      word = strtok(exp_copy, "+");
    else
      word = strtok(NULL, "+");
    words[i] = calloc(1, strlen(word));
    strcpy(words[i], word);
  }
  free(exp_copy);
  return words;
}

// returns the number of words in the expression
int get_word_count(char *expression){
  char *exp_copy = malloc(strlen(expression) + 1);
  strcpy(exp_copy, expression);
  int count = 0;
  char *str = strtok(exp_copy, "+");
  while(str != NULL){
    count++;
    str = strtok(NULL, "+");
  }
  free(exp_copy);
  return count;
}

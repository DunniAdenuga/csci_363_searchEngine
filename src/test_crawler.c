#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "crawler.h"
#include "inv_list.h"
#include "word_list.h"
#include "site_list.h"

#define SUCCESS 1
#define FAIL    0

#define INITIAL_PAGES_LENGTH  7
#define INITIAL_PAGES "resources/Initial_Pages.txt"
#define STATE_FILE    "resources/State_File.bin"

void create_initial_pages(){
  FILE *f = fopen(INITIAL_PAGES, "w");
  fprintf(f, "%s\n", "http://www.bucknell.edu /");
  fprintf(f, "%s\n", "http://www.eg.bucknell.edu /~csci204/");
  fprintf(f, "%s\n", "http://www.eg.bucknell.edu /~cs206/");
  fprintf(f, "%s\n", "http://www.eg.bucknell.edu /~cs240/2014-spring/index.html");
  fprintf(f, "%s\n", "http://www.eg.bucknell.edu /~csci311/S14/");
  fprintf(f, "%s\n", "http://www.eg.bucknell.edu /~cs363/");
  fprintf(f, "%s\n", "http://www.eg.bucknell.edu /~cs479/");
  fclose(f);
}

void remove_initial_pages(){
  remove(INITIAL_PAGES);
}

void remove_state_file(){
  remove(STATE_FILE);
}

void setup_test(){
  create_initial_pages();
}

void teardown_test(){
  remove_initial_pages();
  remove_state_file();
}

struct crawler *get_simple_crawler(){
  return crawler_create(INITIAL_PAGES, STATE_FILE);
}

void check_test_results(int result, char *test_name){
  if(!result){
    printf("TEST %20s:  FAIL\n", test_name);
    teardown_test();
    exit(0);
  }
  printf("TEST %20s:  GOOD\n", test_name);
}

void run_test(int (*f)(void), char *test_name){
  setup_test();
  check_test_results(f(), test_name);
  teardown_test();
}

void run_crawler_test(int (*f)(struct crawler *c), struct crawler *c, char *test_name){
  check_test_results(f(c), test_name);
}  

struct crawler *basic_crawler(){
  struct crawler *c = get_simple_crawler();
  return c;
}

int test_inv_list_is_empty(struct inv_list *il){
  if(il->dynamic)
    return il->ld->count == 0;
  return il->ls->count == 0;
}

int test_site_list_has_length(struct site_list *sl, int length){
  return sl->count == length;
}

int test_site_list_is_empty(struct site_list *sl){
  return test_site_list_has_length(sl, 0);;
}

int test_string_is_equal(char *correct, char *received){
  return strcmp(correct, received) == 0;
}

int test_create_word_results(struct crawler *c){
  return test_inv_list_is_empty(c->word_results);
}

int test_create_visited(struct crawler *c){
  return test_site_list_is_empty(c->visited);
}

int test_create_queue(struct crawler *c){
  return test_site_list_has_length(c->queue, INITIAL_PAGES_LENGTH);
}

int test_create_initial_pages_file(struct crawler *c){
  return test_string_is_equal(INITIAL_PAGES, c->initial_pages_file);
}

int test_create_crawler_state_file(struct crawler *c){
  return test_string_is_equal(STATE_FILE, c->crawler_state_file);
}

int test_create(){
  struct crawler *c = get_simple_crawler();
  run_crawler_test(test_create_word_results, c, "CREATE_WORD_RESULTS");
  run_crawler_test(test_create_visited, c, "CREATE_VISITED");
  run_crawler_test(test_create_queue, c, "CREATE_QUEUE");
  run_crawler_test(test_create_initial_pages_file, c, "CREATE_PAGES_FILE");
  run_crawler_test(test_create_crawler_state_file, c, "CREATE_STATE_FILE");
  crawler_destroy(c);
  return SUCCESS;
}

int test_is_equal(){
  struct crawler *c1 = get_simple_crawler();
  teardown_test();

  setup_test();
  struct crawler *c2 = get_simple_crawler();
  teardown_test();

  int r1 = crawler_is_equal(c1, c2);

  crawl_up_to(c1, 1);
  crawl_up_to(c2, 1);

  int r2 = crawler_is_equal(c1, c2);

  return r1 && r2;
}

int test_save_and_load(){
  struct crawler *c = get_simple_crawler();
  crawl_up_to(c, 1);
  crawler_save(c);

  struct crawler *c2 = get_simple_crawler();
  int result = crawler_is_equal(c, c2);

  crawler_destroy(c);
  crawler_destroy(c2);

  return result;
}

/*
// query functionality is no longer in crawler
int test_empty_query(){
  struct crawler *c = get_simple_crawler();
  struct site_list *sites = crawler_query(c, "bucknell");
  int result = (sites->count == 0);
  crawler_destroy(c);
  return result;
}


int test_homepage_query(){
  struct crawler *c = get_simple_crawler();
  crawl_up_to(c, 1);

  struct site_list *sites = crawler_query(c, "bucknell");
  struct site_list *actual = sl_create();
  sl_add(actual, "http://www.bucknell.edu", "/", 5);
  int result = sl_is_equal(actual, sites);
  crawler_destroy(c);
  return result;
}
*/

int main(){
  run_test(test_create, "CREATE");
  run_test(test_is_equal, "EQUAL");
  run_test(test_save_and_load, "SAVE_AND_LOAD");
  //run_test(test_empty_query, "QUERY_EMPTY");
  //run_test(test_homepage_query, "QUERY_HOMEPAGE");
  printf("ALL TESTS HAVE PASSED\n");
}

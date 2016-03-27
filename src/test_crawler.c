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
/* -------------------------------------------------------
 * Generates a "results" page using the urls given
 *
 * parameters:
 *   urls:  a string of newline seperated urls to be
 *          included in the results
 *
 * return:
 *   a string of html to be returned as the response
 * -------------------------------------------------------
 */
char *get_response_page(char *urls){
  char *response_page;
  pid_t pid_genPage;

  // pipe for interprocess communication
  int com_to_genPage[2];
  int com_from_genPage[2];

  //while(results != NULL){                  // used for multiple pages
  Pipe(com_to_genPage);                     //create a pipe
  Pipe(com_from_genPage);                   //create a pipe
  pid_genPage = Fork();

  if(pid_genPage > 0){    // parent process
    // close unwanted ends of pipes
    close(com_to_genPage[READ]);
    close(com_from_genPage[WRITE]);

    // write to and read results from child
    char *term = "\nterminate\n";

    write(com_to_genPage[WRITE], urls, strlen(urls));
    write(com_to_genPage[WRITE], term, strlen(term));
 
    //read(com_from_parseURL[READ], &msg_len, sizeof(int));
    response_page = malloc(MAX_PAGE_SIZE);
    int return_len = read(com_from_genPage[READ], response_page, MAX_PAGE_SIZE);
    response_page[return_len] = '\0';

  } else{         // Child process
    // redirect pipes to stdin and stdout
    Dup2(com_to_genPage[READ], STDIN_FILENO);
    Dup2(com_from_genPage[WRITE], STDOUT_FILENO);

    // close unneeded pipes (all of them)
    close(com_to_genPage[WRITE]);
    close(com_from_genPage[READ]);

    // begin the parsing process
    execl("/usr/remote/python-3.2/bin/python3", "/bin/python3", "python/genPage.py", (char *)NULL);
  }
  //}
  
  close(com_to_genPage[WRITE]);
  close(com_to_genPage[READ]);

  if(pid_genPage > 0){
    kill(pid_genPage, 7);
  }

  return response_page;
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

int main(){
  run_test(test_create, "CREATE");
  run_test(test_is_equal, "EQUAL");
  run_test(test_save_and_load, "SAVE_AND_LOAD");
  run_test(test_empty_query, "QUERY_EMPTY");
  run_test(test_homepage_query, "QUERY_HOMEPAGE");
  printf("ALL TESTS HAVE PASSED\n");
}

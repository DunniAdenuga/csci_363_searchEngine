#define _GNU_SOURCE     // we want to use strcasestr()
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include "crawler.h"

#include "inv_list.h"
#include "word_list.h"
#include "site_list.h"

#include "python_interface.h"
#include "http_interface.h"
#include "string_ops.h"
#include "initial_page_reader.h"
#include "stop_list_reader.h"

#define BUFFSIZE	512
#define HTTPPORT   80

#define DELAY_AFTER   50
#define DELAY_FOR     3


// creation and loading helper function
static void cl_load_current_state(struct crawler *c);
static void cl_load_from_file(struct crawler *c);
static void cl_load_initial_pages(struct crawler *c);
static void cl_load_stop_list(struct crawler *c);
static void cl_create_from_initial(struct crawler *c);

// struct attribute management
static void cl_set_initial_pages_file(struct crawler *c, char *initial_pages_file);
static void cl_set_crawler_state_file(struct crawler *c, char *crawler_state_file);
static void cl_set_stop_list_file(struct crawler *c, char *stop_list_file);

// queue management functions
static void cl_enqueue_initial_pages(struct crawler *c, char **hosts, char **paths, int count);
static void cl_enqueue_site_list(struct crawler *c, struct site_list *s);
static void cl_enqueue_page(struct crawler *c, char *host, char *path);
static void cl_dequeue_page(struct crawler *c);

// queue access functions
static int cl_queue_is_empty(struct crawler *c);
static char *cl_peek_host(struct crawler *c);
static char *cl_peek_path(struct crawler *c);

// crawler processing functions
static void cl_process_next_queue_page(struct crawler *c);
static void cl_process_page(struct crawler *c, char *host, char *path);

// crawler state management functions
static void cl_set_for_crawl(struct crawler *c);
static void cl_set_for_query(struct crawler *c);

// crawler disk operations
static void cl_save_to_file(struct crawler *c);
static int cl_has_saved_state(struct crawler *c);
static int cl_get_load_fd(struct crawler *c);
static int cl_get_save_fd(struct crawler *c);



// creates a new crawler loading from disk as appropriate
struct crawler *crawler_create(char *initial_pages_file, char *crawler_state_file, char *stop_list_file){
  struct crawler *c = calloc(1, sizeof(struct crawler));
  cl_set_initial_pages_file(c, initial_pages_file);
  cl_set_crawler_state_file(c, crawler_state_file);
  cl_set_stop_list_file(c, stop_list_file);
  
  cl_load_current_state(c);
  cl_set_for_query(c);

  return c;
}

// creates a crawler from disk specifically
struct crawler *crawler_load(char *crawler_state_file){
  struct crawler *c = calloc(1, sizeof(struct crawler));
  cl_set_crawler_state_file(c, crawler_state_file);
  if(cl_has_saved_state(c)){
    cl_load_from_file(c);
  }else{
    printf("Attempted Crawler Load from Invalid File: %s\n", crawler_state_file);
    exit(1);
  }
  return c;
}

// frees a crawler's memory
void crawler_destroy(struct crawler *c){
  il_destroy(c->word_results);
  sl_destroy(c->visited);
  sl_destroy(c->queue);
  free(c->initial_pages_file);
  free(c->crawler_state_file);
}

// saves a crawler to its state file
void crawler_save(struct crawler *c){
  cl_save_to_file(c);
}

// crawls until the crawler has process total_pages number of pages
int crawl_up_to(struct crawler *c, int total_pages){
  cl_set_for_crawl(c);

  int delay_counter = 0;
  while(c->visited->count < total_pages && c->queue->count > 0){
    cl_process_next_queue_page(c);
    if(delay_counter % DELAY_AFTER == 0){
      sleep(DELAY_FOR);
    }
  }
  cl_set_for_query(c);
  return c->visited->count;
}

// crawls an additional pages number of pages from this point
int crawl_additional(struct crawler *c, int pages){
  return crawl_up_to(c, c->visited->count + pages);
}

// Returns the total number of pages crawled
int crawler_get_crawled_count(struct crawler *c){
  return c->visited->count;
}

// returns the inverted list being maintained by this crawler
struct inv_list *crawler_get_inv_list(struct crawler *c){
  return c->word_results;
}

// test for recursive equality of two crawlers
int crawler_is_equal(struct crawler *c1, struct crawler *c2){
  int r1 = il_is_equal(c1->word_results, c2->word_results);
  int r2 = sl_is_equal(c1->visited, c2->visited);
  int r3 = sl_is_equal(c1->queue, c2->queue);
  int r4 = (strcmp(c1->initial_pages_file, c2->initial_pages_file) == 0);
  int r5 = (strcmp(c1->crawler_state_file, c2->crawler_state_file) == 0);
  return r1 && r2 && r3 && r4 && r5;
}

// displays the data of the crawler recursively
void crawler_display(struct crawler *c){
  printf("######################### Crawler ######################\n");
  il_display(c->word_results);
  sl_display(c->visited);
  sl_display(c->queue);
  printf("INITIAL_PAGES_FILE: %s\n", c->initial_pages_file);
  printf("CRAWLER_STATE_FILE: %s\n", c->crawler_state_file);
  printf("\n\n");
}

// sets up the initial_pages_member for the crawler
void cl_set_initial_pages_file(struct crawler *c, char *initial_pages_file){
  if(c->initial_pages_file != NULL)
    free(c->initial_pages_file);
  c->initial_pages_file = malloc(strlen(initial_pages_file) + 1);
  strcpy(c->initial_pages_file, initial_pages_file);
}

// sets up the crawler_state_file member of the crawler
void cl_set_crawler_state_file(struct crawler *c, char *crawler_state_file){
  if(c->crawler_state_file != NULL)
    free(c->initial_pages_file);
  c->crawler_state_file = malloc(strlen(crawler_state_file) + 1);
  strcpy(c->crawler_state_file, crawler_state_file);
}

// sets up the crawler stop list file
void cl_set_stop_list_file(struct crawler *c, char *stop_list_file){
  if(c->stop_list_file != NULL)
    free(c->stop_list_file);
  c->stop_list_file = malloc(strlen(stop_list_file) + 1);
  strcpy(c->stop_list_file, stop_list_file);
}

// loads state from crawler_state_file if it exists or initial if not
void cl_load_current_state(struct crawler *c){
  if(cl_has_saved_state(c)){
    cl_load_from_file(c);
  }else{
    cl_create_from_initial(c);
  }
}

// loads the crawler from the crawler_state_file binary file
void cl_load_from_file(struct crawler *c){
  int fd = cl_get_load_fd(c);
  c->word_results = il_load(fd);
  c->stop_list = il_load(fd);
  c->visited = sl_load(fd);
  c->queue = sl_load(fd);
  c->initial_pages_file = str_load(fd);
  c->crawler_state_file = str_load(fd);
  c->stop_list_file = str_load(fd);
  close(fd);
}

// saves the crawler to the crawler_state_file binary file
void cl_save_to_file(struct crawler *c){
  int fd = cl_get_save_fd(c);
  il_save(c->word_results, fd);
  il_save(c->stop_list, fd);
  sl_save(c->visited, fd);
  sl_save(c->queue, fd);
  str_save(c->initial_pages_file, fd);
  str_save(c->crawler_state_file, fd);
  close(fd);
}

// creates a crawler from the initial urls in initial_urls_file
void cl_create_from_initial(struct crawler *c){
  c->word_results = il_create();
  c->stop_list = il_create();
  c->visited = sl_create();
  c->queue = sl_create();

  cl_load_initial_pages(c);
  cl_load_stop_list(c);
}

// loads the initial pages into the processing queue
void cl_load_initial_pages(struct crawler *c){
  char **hosts = get_initial_hosts(c->initial_pages_file);
  char **paths = get_initial_paths(c->initial_pages_file);
  int count = get_initial_page_count(c->initial_pages_file);
  cl_enqueue_initial_pages(c, hosts, paths, count);
  free_initial_pages(hosts, paths, c->initial_pages_file);
}

// loads the initial stop list from the file
void cl_load_stop_list(struct crawler *c){
  il_set_dynamic(c->stop_list);
  char **words = get_stop_words(c->stop_list_file);
  int word_count = get_stop_word_count(c->stop_list_file);
  for(int i = 0; i < word_count; i++){
    il_add(c->stop_list, words[i], "", "", 0);
  }
  free_stop_words(words, c->stop_list_file);
  il_set_static(c->stop_list);
}

// enqueues each of the initial pages into the processing queue
void cl_enqueue_initial_pages(struct crawler *c, char **hosts, char **paths, int count){
  for(int i = 0; i < count; i++){
    cl_enqueue_page(c, hosts[i], paths[i]);
  }
}

// enqueues an entire site list into the processing list
void cl_enqueue_site_list(struct crawler *c, struct site_list *s){
  sl_iter_begin(s);
  while(sl_iter_curr(s) != NULL){
    struct site_node *node = sl_iter_curr(s);
    cl_enqueue_page(c, node->host, node->path);
    sl_iter_next(s);
  }
}

// enqueues a single page into the processing list
void cl_enqueue_page(struct crawler *c, char *host, char *path){
  if(!sl_contains(c->visited, host, path)){
    sl_add(c->queue, host, path, 0);
  }
}

// removes the next page from the processing list
void cl_dequeue_page(struct crawler *c){
  sl_remove_head(c->queue);
}

// adds the sites from the site_list to be associated with the given word
void cl_add_site_list(struct crawler *c, char *word, struct site_list *s){
  struct site_node *site = sl_iter_begin(s);
  while(site != NULL){
    il_add(c->word_results, word, site->host, site->path, site->frequency);
    site = sl_iter_next(s);
  }
}

// adds a word_list to the crawler's records
void cl_add_word_list(struct crawler *c, struct word_list *w){
  struct word_node *w_node = wl_iter_begin(w);
  while(w_node != NULL){
    if(!il_contains(c->stop_list, w_node->word))
      cl_add_site_list(c, w_node->word, w_node->sites);
    w_node = wl_iter_next(w);
  }
}

// determines whether the processing queue is empty
int cl_queue_is_empty(struct crawler *c){
  return c->queue->count == 0;
}

// returns the host of the next site to be processed
char *cl_peek_host(struct crawler *c){
  if(cl_queue_is_empty(c))
    return NULL;
  return c->queue->head->host;
}

// returns the path of the next site to be processed
char *cl_peek_path(struct crawler *c){
  if(cl_queue_is_empty(c))
    return NULL;
  return c->queue->head->path;
}

// process and removes the next site in the processing queue
void cl_process_next_queue_page(struct crawler *c){
  if(cl_queue_is_empty(c))
    return;

  char *host = cl_peek_host(c);
  char *path = cl_peek_path(c);

  cl_process_page(c, host, path);
  cl_dequeue_page(c);
}

// processes the specified page adding its urls and words to the records
void cl_process_page(struct crawler *c, char *host, char *path){
  if(sl_contains(c->visited, host, path))
    return;

  char *page_content = get_page_content(host, path);
  struct site_list *sites = get_urls_wrapper(host, page_content);
  struct word_list *words = get_words_wrapper(host, path, page_content);

  cl_enqueue_site_list(c, sites);
  cl_add_word_list(c, words);

  sl_destroy(sites);
  wl_destroy(words);

  sl_add(c->visited, host, path, 0);
}

// sets the crawler for crawling (makes list dynamic)
void cl_set_for_crawl(struct crawler *c){
  il_set_dynamic(c->word_results);
}

// sets the crawler for queries (makes list static)
void cl_set_for_query(struct crawler *c){
  il_set_static(c->word_results);
}

// determines whether the crawler has a saved state
int cl_has_saved_state(struct crawler *c){
  return access( c->crawler_state_file, F_OK ) != -1;
}

// returns a file descriptor for the file where the crawler will be saved
int cl_get_save_fd(struct crawler *c){
  int fd = open(c->crawler_state_file, O_CREAT | O_WRONLY | O_TRUNC, 0600);

  if(fd < 0){
    perror("File Open Error");
    exit(1);
  } 
  return fd;
}

// returns a file descriptor from which the crawler should be loaded
int cl_get_load_fd(struct crawler *c){
  int fd = open(c->crawler_state_file, O_RDONLY, 0600);

  if(fd < 0){
    perror("File Open Error");
    exit(1);
  }
  return fd;
}

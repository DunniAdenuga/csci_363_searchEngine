/*
 * Original version from Doug Comer's book "Internetworking with TCP/IP"
 * Revised for CSCI 363
 * Xiannong Meng <xmeng@bucknell.edu>
 * 2015-12-11
 */

/* webclient.c */

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
#include "initial_page_reader.h"
#include "string_ops.h"

#define BUFFSIZE	512
#define HTTPPORT   80

#define DELAY_AFTER   50
#define DELAY_FOR     3


//---------------------------------------------
// fillin functions to be defined other places
//---------------------------------------------

struct site_list *get_page_links(char *host, char *path){
  struct site_list *s = sl_create();
  sl_add(s, "www.bucknell.edu", "/", 0);
  return s;
}

struct word_list *get_page_words(char *host, char *path){
  struct word_list *w = wl_create();
  wl_add(w, "bucknell", host, path, 5);
  return w;
}

//---------------------------------------------

// functions defined in the file that aren't in the header file
static void cl_load_current_state(struct crawler *c);
static void cl_load_from_file(struct crawler *c);
static void cl_create_from_initial(struct crawler *c);

static void cl_set_initial_pages_file(struct crawler *c, char *initial_pages_file);
static void cl_set_crawler_state_file(struct crawler *c, char *crawler_state_file);

static void cl_load_initial_pages(struct crawler *c);
static void cl_enqueue_initial_pages(struct crawler *c, char **hosts, char **paths, int count);
static void cl_enqueue_site_list(struct crawler *c, struct site_list *s);
static void cl_enqueue_page(struct crawler *c, char *host, char *path);
static void cl_dequeue_page(struct crawler *c);

static int cl_queue_is_empty(struct crawler *c);
static char *cl_peek_host(struct crawler *c);
static char *cl_peek_path(struct crawler *c);

static void cl_process_next_queue_page(struct crawler *c);
static void cl_process_page(struct crawler *c, char *host, char *path);

static void cl_set_for_crawl(struct crawler *c);
static void cl_set_for_query(struct crawler *c);

static void cl_save_to_file(struct crawler *c);
static int cl_has_saved_state(struct crawler *c);
static int cl_get_load_fd(struct crawler *c);
static int cl_get_save_fd(struct crawler *c);



struct crawler *crawler_create(char *initial_pages_file, char *crawler_state_file){
  struct crawler *c = calloc(1, sizeof(struct crawler));
  cl_set_initial_pages_file(c, initial_pages_file);
  cl_set_crawler_state_file(c, crawler_state_file);
  
  cl_load_current_state(c);
  cl_set_for_query(c);

  return c;
}

void crawler_destroy(struct crawler *c){
  il_destroy(c->word_results);
  sl_destroy(c->visited);
  sl_destroy(c->queue);
  free(c->initial_pages_file);
  free(c->crawler_state_file);
}

void crawler_save(struct crawler *c){
  cl_save_to_file(c);
}

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

int crawl_additional(struct crawler *c, int pages){
  return crawl_up_to(c, c->visited->count + pages);
}

const struct inv_list *crawler_get_inv_list(struct crawler *c){
  return c->word_results;
}

int crawler_is_equal(struct crawler *c1, struct crawler *c2){
  int r1 = il_is_equal(c1->word_results, c2->word_results);
  int r2 = sl_is_equal(c1->visited, c2->visited);
  int r3 = sl_is_equal(c1->queue, c2->queue);
  int r4 = (strcmp(c1->initial_pages_file, c2->initial_pages_file) == 0);
  int r5 = (strcmp(c1->crawler_state_file, c2->crawler_state_file) == 0);
  return r1 && r2 && r3 && r4 && r5;
}

void crawler_display(struct crawler *c){
  printf("######################### Crawler ######################\n");
  il_display(c->word_results);
  sl_display(c->visited);
  sl_display(c->queue);
  printf("INITIAL_PAGES_FILE: %s\n", c->initial_pages_file);
  printf("CRAWLER_STATE_FILE: %s\n", c->crawler_state_file);
  printf("\n\n");
}

void cl_set_initial_pages_file(struct crawler *c, char *initial_pages_file){
  c->initial_pages_file = malloc(strlen(initial_pages_file) + 1);
  strcpy(c->initial_pages_file, initial_pages_file);
}

void cl_set_crawler_state_file(struct crawler *c, char *crawler_state_file){
  c->crawler_state_file = malloc(strlen(crawler_state_file) + 1);
  strcpy(c->crawler_state_file, crawler_state_file);
}

void cl_load_current_state(struct crawler *c){
  if(cl_has_saved_state(c)){
    cl_load_from_file(c);
  }else{
    cl_create_from_initial(c);
  }
}

void cl_load_from_file(struct crawler *c){
  int fd = cl_get_load_fd(c);
  c->word_results = il_load(fd);
  c->visited = sl_load(fd);
  c->queue = sl_load(fd);
  c->initial_pages_file = str_load(fd);
  c->crawler_state_file = str_load(fd);
  close(fd);
}

void cl_save_to_file(struct crawler *c){
  int fd = cl_get_save_fd(c);
  il_save(c->word_results, fd);
  sl_save(c->visited, fd);
  sl_save(c->queue, fd);
  str_save(c->initial_pages_file, fd);
  str_save(c->crawler_state_file, fd);
  close(fd);
}

void cl_create_from_initial(struct crawler *c){
  c->word_results = il_create();
  c->visited = sl_create();
  c->queue = sl_create();

  cl_load_initial_pages(c);
}

void cl_load_initial_pages(struct crawler *c){
  char **hosts = get_initial_hosts(c->initial_pages_file);
  char **paths = get_initial_paths(c->initial_pages_file);
  int count = get_initial_page_count(c->initial_pages_file);
  cl_enqueue_initial_pages(c, hosts, paths, count);
  free_initial_pages(hosts, paths, c->initial_pages_file);
}

void cl_enqueue_initial_pages(struct crawler *c, char **hosts, char **paths, int count){
  for(int i = 0; i < count; i++){
    cl_enqueue_page(c, hosts[i], paths[i]);
  }
}

void cl_enqueue_site_list(struct crawler *c, struct site_list *s){
  sl_iter_begin(s);
  while(sl_iter_curr(s) != NULL){
    struct site_node *node = sl_iter_curr(s);
    cl_enqueue_page(c, node->host, node->path);
    sl_iter_next(s);
  }
}

void cl_enqueue_page(struct crawler *c, char *host, char *path){
  if(!sl_contains(c->visited, host, path)){
    sl_add(c->queue, host, path, 0);
  }
}

void cl_dequeue_page(struct crawler *c){
  sl_remove_head(c->queue);
}

void cl_add_site_list(struct crawler *c, char *word, struct site_list *s){
  struct site_node *site = sl_iter_begin(s);
  while(site != NULL){
    il_add(c->word_results, word, site->host, site->path, site->frequency);
    site = sl_iter_next(s);
  }
}

void cl_add_word_list(struct crawler *c, struct word_list *w){
  struct word_node *w_node = wl_iter_begin(w);
  while(w_node != NULL){
    cl_add_site_list(c, w_node->word, w_node->sites);
    w_node = wl_iter_next(w);
  }
}

int cl_queue_is_empty(struct crawler *c){
  return c->queue->count == 0;
}

char *cl_peek_host(struct crawler *c){
  if(cl_queue_is_empty(c))
    return NULL;
  return c->queue->head->host;
}

char *cl_peek_path(struct crawler *c){
  if(cl_queue_is_empty(c))
    return NULL;
  return c->queue->head->path;
}

void cl_process_next_queue_page(struct crawler *c){
  if(cl_queue_is_empty(c))
    return;

  char *host = cl_peek_host(c);
  char *path = cl_peek_path(c);

  cl_process_page(c, host, path);
  cl_dequeue_page(c);
}

void cl_process_page(struct crawler *c, char *host, char *path){
  if(sl_contains(c->visited, host, path))
    return;

  struct site_list *sites = get_page_links(host, path);
  struct word_list *words = get_page_words(host, path);

  cl_enqueue_site_list(c, sites);
  cl_add_word_list(c, words);

  sl_destroy(sites);
  wl_destroy(words);

  sl_add(c->visited, host, path, 0);
}

void cl_set_for_crawl(struct crawler *c){
  il_set_dynamic(c->word_results);
}

void cl_set_for_query(struct crawler *c){
  il_set_static(c->word_results);
}

int cl_has_saved_state(struct crawler *c){
  return access( c->crawler_state_file, F_OK ) != -1;
}

int cl_get_save_fd(struct crawler *c){
  int fd = open(c->crawler_state_file, O_CREAT | O_WRONLY, 0600);

  if(fd < 0){
    perror("File Open Error");
    exit(1);
  } 
  return fd;
}

int cl_get_load_fd(struct crawler *c){
  int fd = open(c->crawler_state_file, O_CREAT | O_RDONLY, 0600);

  if(fd < 0){
    perror("File Open Error");
    exit(1);
  }
  return fd;
}

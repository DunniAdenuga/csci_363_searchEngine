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
#include "tcplib.h"
#include "initial_page_reader.h"

#define BUFFSIZE	512
#define HTTPPORT   80

#define DELAY_AFTER   50
#define DELAY_FOR     3

#define INIT_PAGE_FILE      "initial_pages.txt"
#define CRAWLER_SAVE_FILE   "crawler_state.bin"

struct crawler{
  struct inv_list *word_results;
  struct site_list *visited;
  struct site_list *queue;
};

// value used throughout this file
struct crawler web_crawler = NULL;

// functions defined in the file
struct crawler *cl_load_current_state();
struct crawler *cl_load_from_file();
struct crawler *cl_create_from_initial();

void cl_load_initial_pages(struct crawler *c);
void cl_enqueue_initial_pages(struct crawler *c, char **hosts, char **paths, int count);
void cl_enqueue_site_list(struct crawler *c, char *host, char *path, struct site_list s){
void cl_enqueue_page(struct crawler *c, host, path);
void cl_dequeue_page(struct crawler *c);
void cl_peek_host(struct crawler *c);
void cl_peek_path(struct crawler *c);

void cl_process_queue_page(struct crawler *c);
void cl_process_page(struct crawler *c, char *host, char *path);

void cl_save_to_file(struct crawler*);
int cl_has_saved_state();
int cl_get_load_fd();
int cl_get_save_fd();

void init_crawler(){
  web_crawler = cl_load_current_state();
  il_set_static(web_crawler->word_results);
}

int crawl_up_to(int total_pages){
  cl_set_dynamic(web_crawler->word_results);
  int delay_counter = 0;
  while(web_crawler->visited->count < total_pages && web_crawler->queue->count > 0){
    cl_process_queue_page(web_crawler);
    if(delay_counter % DELAY_AFTER == 0){
      sleep(DELAY_FOR);
    }
  }
  cl_set_static(web_crawler->word_results);
}

void crawl_additional(int pages){
  if(web_crawler == NULL)
    return 0;

  return crawl_up_to(web_crawler->visited->count + pages);
}

struct site_list *query(char *word){
  return il_get_sites(web_crawler->word_results, word);
}

struct crawler *cl_load_current_state(){
  if(cl_has_saved_state()){
    return cl_load_from_file();
  }else{
    return cl_create_from_initial();
  }
}

struct crawler *cl_load_from_file(){
  struct crawler *c = calloc(sizeof(struct crawler));

  int fd = cl_get_load_fd();
  c->inv_list = il_load(fd);
  c->visited = sl_load(fd);
  c->queue = sl_load(fd);
  close(fd);
}

struct crawler *cl_create_from_initial(){
  struct crawler *c = malloc(sizeof(struct crawler));
  c->inv_list = il_create();
  c->visited = sl_create();
  c->queue = sl_create();

  cl_load_initial_pages(c);
}

void cl_load_initial_pages(struct crawler *c){
  char **hosts = get_initial_hosts();
  char **paths = get_initial_paths();
  int count = get_initial_page_count();
  cl_enqueue_initial_pages(c, hosts, paths, count);
  free_initial_pages(hosts, paths, count);
}

void cl_enqueue_initial_pages(struct crawler c, char **hosts, char **paths, int count){
  for(int i = 0; i < count; i++){
    cl_enqueue_page(c, hosts[i], paths[i]);
  }
}

void cl_enqueue_site_list(struct crawler *c, char *host, char *path, struct site_list s){
  sl_iter_begin(s);
  while(sl_iter_current(s) != NULL){
    struct site_node *node = sl_iter_current(s)
    cl_enqueue_page(c, node->host, node->path);
    sl_iter_next(s);
  }
}

void cl_enqueue_page(struct crawler c, host, path){
  if(!sl_contains(c->visited, host, path)){
    sl_add(c->queue, host, path, 0);
  }
}

void cl_dequeue_page(struct crawler *c){
  sl_remove_head(c->queue);
}

void cl_add_page_info(struct crawler *c, char *host, char *path, struct word_list *w){
  // need the work from Dunni to complete this
}

void cl_peek_host(struct crawler *c){
  if(c->visited->head == NULL)
    return NULL;
  return c->visited->head->host;
}

void cl_peek_path(struct crawler *c){
  if(c->visited->head == NULL)
    return NULL;
  return c->visited->head->path;
}

void cl_process_queue_page(struct crawler *c){
  char *host = cl_peek_host(c);
  char *path = cl_peek_path(c);
  cl_dequeue_page(c);

  cl_process_page(c, host, path);
}

void cl_process_page(struct crawler *c, char *host, char *path){
  if(sl_contains(c->visited, host, path))
    return;
  struct site_list *s = get_page_links(host, path);
  struct word_list *w = get_page_words(host, path);

  cl_enqueue_site_list(s);
  cl_add_page_info(c, host, path, w);

  sl_add(c->visited, host, path, 0);
}

void cl_save_to_file(struct crawler*){
  int fd = cl_get_save_fd();
  il_save(crawler->inv_list);
  sl_save(crawler->visited);
  sl_save(crawler->queue);
  close(fd);
}

int cl_has_saved_state(){
  return access( CRAWLER_SAVE_FILE, F_OK ) != -1;
}

int cl_get_save_fd(){
  int fd = open(CRAWLER_SAVE_FILE, O_CREAT | O_WRONLY, 0600);

  if(fd < 0){
    perror("File Open Error");
    exit(1);
  } 
  return fd;
}

int cl_get_load_fd(){
  int fd = open(CRAWLER_SAVE_FILE, O_CREAT | O_RDONLY, 0600);

  if(fd < 0){
    perror("File Open Error");
    exit(1);
  }
  return fd;
}

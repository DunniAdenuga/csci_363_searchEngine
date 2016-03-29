#include "crawler.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * This is a program that initializes runs a crawler in order to setup a
 * crawler_state_file to be used within the webserver. The server should load a
 * new crawler from the same file as this crawler's crawler_state file in order
 * to make use of the crawling that has taken place from within this program.
 */

int main(int argc, char *argv[]){
  if(argc != 2){
    printf("Ussage: %s <max_pages_to_crawl>\n", argv[0]);
    exit(0);
  }

  struct crawler *crawler = crawler_create("resources/initial_pages.txt", "resources/crawler_state.bin", "resources/stop_words.txt");
  printf("Successfully Created Crawler\n\n");

  int crawl_initial = crawler_get_crawled_count(crawler);
  int crawl_additional = atoi(argv[1]);
  int crawl_total = crawl_initial + crawl_additional;
  int increment = 1;

  // main loop incrementing by <increment> pages
  for(int i = crawl_initial; i < crawl_total; i += increment){
    crawl_up_to(crawler, i);
    crawler_save(crawler);
    printf("Crawled %d pages...\n", i);
  }

  // catch any remainder
  if(crawler_get_crawled_count(crawler) < crawl_total){
    crawl_up_to(crawler, crawl_total);
    crawler_save(crawler);
    printf("Crawled %d pages...\n", crawl_total);
  }

  crawler_destroy(crawler);
}

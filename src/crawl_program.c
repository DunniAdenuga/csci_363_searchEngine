#include "crawler.h"
#include <stdio.h>

#define CRAWL_NUMBER 1000 

int main(){
  struct crawler *crawler = crawler_create("resources/initial_pages.txt", "resources/crawler_state.bin");
  printf("Successfully Created Crawler\n\n");
  for(int i = 0; i < CRAWL_NUMBER; i += 10){
    crawl_up_to(crawler, i);
    crawler_save(crawler);
    printf("Crawled %d pages...\n", i);
  }
  crawler_destroy(crawler);
}

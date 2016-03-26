#include "inv_list.h"
#include "site_list.h"
#include "word_list.h"
#include "pages.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX 9999

char *page_content;

char* control(char *initial_host, char *initial_path){
 page_content = get_page(initial_host, initial_path);
}
struct word_list*  wrap_get_url(char *page_content, char *page_host){
  struct word_list* results = il_create();
  char urls[MAX] = get_urls(page_content, page_host);
  char* token;
  char* token2;

  token = strtok(urls, "\n");
  token2 = strtok(NULL, "\n");
  il_add(results, NULL, token, token2, 0);

  do{
    token = strtok(NULL, "\n");
    token2 = strtok(NULL, "\n");
    il_add(results, NULL, token, token2, 0);
  }while(token2 != NULL) ;

  return results;

}

struct inv_list* wrap_get_words(char* page_content){
 struct inv_list* results = il_create();
  char urls[MAX] = get_words(page_content, page);
  char* token;
  char* token2;

  token = strtok(urls, "\n");
  token2 = strtok(NULL, "\n");
  il_add(results, token, NULL, , 0);

  do{
    token = strtok(NULL, "\n");
    token2 = strtok(NULL, "\n");
    il_add(results, NULL, token, token2, 0);
  }while(token2 != NULL) ;

  return results;


}

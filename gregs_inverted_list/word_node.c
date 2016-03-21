#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "word_node.h"

// create a new word node for the given word
struct word_node *wn_create(char *word){
  struct word_node *node = malloc(sizeof(struct word_node));

  // initialize the word
  node->word = malloc(strlen(word) + 1);
  strcpy(node->word, word);
  
  // initialize the site list
  node->sites = sl_create();
  return node;
}

// destroys a node freeing memory as needed
void wn_destroy(struct word_node *node){
  sl_destroy(node->sites);
  free(node->word);
  free(node);
}

// loads a node from the given file descriptor
struct word_node *wn_load(int fd){
  struct word_node *node = malloc(sizeof(struct word_node));

  int word_len;
  read(fd, &word_len, sizeof(word_len));

  node->word = malloc(word_len);
  read(fd, node->word, word_len);

  node->sites = sl_load(fd);

  node->next = NULL;
  return node;
}

// saves a node to the file descriptor
void wn_save(struct word_node *node, int fd){
  int word_len = strlen(node->word) + 1;

  write(fd, &word_len, sizeof(word_len));
  write(fd, node->word, word_len);

  sl_save(node->sites, fd);
}

// setters
void wn_set_next(struct word_node *node, struct word_node *next){
  node->next = next;
}

// getters
char *wn_get_word(struct word_node *node){
  return node->word;
}

struct site_list *wn_get_sites(struct word_node *node){
  return node->sites;
}

struct word_node *wn_get_next(struct word_node *node){
  return node->next;
}

// display
void wn_display(struct word_node *node){
  printf("******  %s  ******\n", node->word);
  sl_display(wn_get_sites(node));
  printf("\n");
}

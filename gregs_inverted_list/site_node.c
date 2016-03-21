#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "site_node.h"

// creates a new site node
struct site_node *sn_create(char *host, char *path){
  // calloc sets all values to NULL
  struct site_node *node = calloc(1, sizeof(struct site_node));
  sn_set_host(node, host);
  sn_set_path(node, path);

  return node;
}

// handles memory for the destruction of a site node
void sn_destroy(struct site_node *node){
  free(node->host);
  free(node->path);
  free(node);
}

// load a node from a file
struct site_node *sn_load(int fd){
  struct site_node *node = calloc(1, sizeof(struct site_node));

  int host_len;
  int path_len;

  read(fd, &host_len, sizeof(host_len));
  read(fd, &path_len, sizeof(path_len));

  node->host = malloc(host_len);
  node->path = malloc(path_len);

  read(fd, node->host, host_len);
  read(fd, node->path, path_len);

  return node;
}

// save the node to a file descriptor (does not save "next" pointer)
void sn_save(struct site_node *node, int fd){
  int host_len = strlen(node->host) + 1;
  int path_len = strlen(node->path) + 1;

  write(fd, &host_len, sizeof(host_len));
  write(fd, &path_len, sizeof(path_len));
  write(fd, node->host, host_len);
  write(fd, node->path, path_len);
}

// sets the value the next site_node in the list
void sn_set_next(struct site_node *node, struct site_node *next){
  node->next = next;
}

// sets the host for a node
void sn_set_host(struct site_node *node, char *host){
  if(node->host != NULL){
    free(node->host);
  }

  node->host = malloc(strlen(host) + 1);
  strcpy(node->host, host);
}

// sets the path for a node
void sn_set_path(struct site_node *node, char *path){
  if(node->path != NULL){
    free(node->path);
  }

  node->path = malloc(strlen(path) + 1);
  strcpy(node->path, path);
}

// gets the next site_node in the list
struct site_node *sn_get_next(struct site_node *node){
  return node->next;
}

// gets the host for this site_node
char *sn_get_host(struct site_node *node){
  return node->host;
}

// gets the path for this site_node
char *sn_get_path(struct site_node *node){
  return node->path;
}

// display
void sn_display(struct site_node *node){
  printf("site_node => host: %s   ***   path: %s\n", node->host, node->path);
}

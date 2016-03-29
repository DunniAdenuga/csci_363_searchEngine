#ifndef __sitenode__h__
#define __sitenode__h__

/**
 * An interface to a node used for a site_list
 */

struct site_node{
  char *host;
  char *path;
  int frequency;
  struct site_node *prev;
  struct site_node *next;
};

// creates a new node
struct site_node *sn_create(char *host, char *path, int frequency);
// frees the memory of a node
void sn_destroy(struct site_node *node);
// tests for node equality
int sn_is_equal(struct site_node *n1, struct site_node *n2);


// loads a node from the specified file descriptor
struct site_node *sn_load(int fd);
// saves a node to the specified file descriptor
void sn_save(struct site_node *node, int fd);


// sets the values of the struct elements properly
void sn_set_next(struct site_node *node, struct site_node *next);
void sn_set_prev(struct site_node *node, struct site_node *prev);
void sn_set_host(struct site_node *node, char *host);
void sn_set_path(struct site_node *node, char *path);

// displays the node
void sn_display(struct site_node *node);

#endif

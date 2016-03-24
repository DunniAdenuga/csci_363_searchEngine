#ifndef __sitenode__h__
#define __sitenode__h__

// a node containing one site
struct site_node{
  char *host;
  char *path;
  int frequency;
  struct site_node *prev;
  struct site_node *next;
};

// creation and destruction
struct site_node *sn_create(char *host, char *path, int frequency);
void sn_destroy(struct site_node *node);

// save/load from disk
struct site_node *sn_load(int fd);
void sn_save(struct site_node *node, int fd);

// manage data structure
void sn_set_next(struct site_node *node, struct site_node *next);
void sn_set_prev(struct site_node *node, struct site_node *prev);
void sn_set_host(struct site_node *node, char *host);
void sn_set_path(struct site_node *node, char *path);

// display
void sn_display(struct site_node *node);

#endif

#ifndef CRAWLER_H
#define CRAWLER_H

struct crawler{
  struct inv_list *word_results;
  struct site_list *visited;
  struct site_list *queue;
  char *initial_pages_file;
  char *crawler_state_file;
};

struct crawler *crawler_create(char *initial_pages_file, char *crawler_state_file);
void crawler_destroy(struct crawler *c);
void crawler_save(struct crawler *c);

int crawl_up_to(struct crawler *c, int total_pages);
int crawl_additional(struct crawler *c, int pages);

struct site_list *crawler_query(struct crawler *c, char *word);
int crawler_is_equal(struct crawler *c1, struct crawler *c2);

void crawler_display(struct crawler *c);

#endif

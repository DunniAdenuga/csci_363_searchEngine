#ifndef CRAWLER_H
#define CRAWLER_H

/**
 * This file encapsulates the functionality of a web crawler. The crawler is
 * created with a file that conains the initial pages to be used and crawls
 * pages as specified. Its inverted list can be retrieved and searched after
 * crawling has taken place.
 *
 * The initial pages file should be of the format:
 * <host> <path>\n
 * eg:
 * http://www.bucknell.edu /
 * http://www.bucknell.edu /~csci203/
 * http://www.bucknell.edu /~csci204/
 *
 * The crawler_state_file should be created only by calling "crawler_save". 
 * After this save call, subsequent crawlers with the same crawler_state_file
 * will be created with the state which was saved.
 */

struct crawler{
  struct inv_list *word_results;
  struct inv_list *stop_list;
  struct site_list *visited;
  struct site_list *queue;
  char *initial_pages_file;
  char *crawler_state_file;
  char *stop_list_file;
};

// Creates a new crawler with the files to be used for initial pages and state storage
struct crawler *crawler_create(char *initial_pages_file, char *crawler_state_file, char *stop_list_file);

// Loads a crawler from the state file exiting on invalid
struct crawler *crawler_load(char *crawler_state_file);

// Frees the memory of a crawler
void crawler_destroy(struct crawler *c);

// Saves the current state of the crawler to a file
void crawler_save(struct crawler *c);

// Crawls pages until the total pages craled is "total_pages"
int crawl_up_to(struct crawler *c, int total_pages);

// Crawls an additional "pages" number of pages from the current state
int crawl_additional(struct crawler *c, int pages);

// Returns the total number of pages crawled
int crawler_get_crawled_count(struct crawler *c);

// Determines recursively if two crawlers are equal
int crawler_is_equal(struct crawler *c1, struct crawler *c2);

// Returns the inverted list to be used for querying
struct inv_list *crawler_get_inv_list(struct crawler *c);

// Displays recursively the content of a crawler struct
void crawler_display(struct crawler *c);

#endif

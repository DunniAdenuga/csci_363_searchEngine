#ifndef CRAWLER_H
#define CRAWLER_H

void init_crawler();

int crawl_up_to(int total_pages);
int crawl_additional(int pages);

struct site_list *query(char *word);

#endif

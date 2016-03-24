#ifndef __INITIAL_PAGE_READER_H_
#define __INITIAL_PAGE_READER_H_

int get_initial_page_count();
char **get_initial_hosts();
char **get_initial_paths();

void free_initial_pages(char **hosts, char **paths);

#endif

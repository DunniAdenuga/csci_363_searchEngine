#ifndef __INITIAL_PAGE_READER_H_
#define __INITIAL_PAGE_READER_H_

int get_initial_page_count(char *file_name);
char **get_initial_hosts(char *file_name);
char **get_initial_paths(char *file_name);

void free_initial_pages(char **hosts, char **paths, char *file_name);

#endif

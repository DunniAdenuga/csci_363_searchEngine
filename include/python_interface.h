#ifndef __PYTHON_INTERFACE_H_
#define __PYTHON_INTERFACE_H_

struct site_list*  get_urls_wrapper(char *page_content);
struct word_list* get_words_wrapper(char *host, char *path, char* page_content);
char* wrap_get_responsePage(struct site_list* list);

#endif

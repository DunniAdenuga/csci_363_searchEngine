#ifndef __PYTHON_INTERFACE_H_
#define __PYTHON_INTERFACE_H_

/**
 * This file provides wrapper functions which interface to the python programs
 * used in this project.
 */

// returns a list of urls found in the html page "page_content"
struct site_list *get_urls_wrapper(char *host, char *page_content);

// returns a list of words found in the html page "page_content"
// The host and path arguments are used to populate the word_list for later reference
struct word_list *get_words_wrapper(char *host, char *path, char* page_content);

// returns a query response containing the urls in "list"
char *get_response_page_wrapper(struct site_list* list);

#endif

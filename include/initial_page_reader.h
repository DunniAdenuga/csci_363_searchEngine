#ifndef __INITIAL_PAGE_READER_H_
#define __INITIAL_PAGE_READER_H_

/**
 * These functions provide a convenient interface to read the content of a file
 * containing the initial urls to be used by a crawler. It allows the retrieval
 * of the total count of hosts and paths as well as a means of loading those
 * hosts and paths. Lastly, it provides a memory management function for when
 * the data is no longer needed.
 *
 * file format:
 * <host> <path>\n
 *
 * example:
 * http://www.bucknell.edu /
 * http://eg.www.bucknell.edu /csci204/
 */

// returns the number of pages found in the file (file must be formated correctly)
int get_initial_page_count(char *file_name);

// returns the hosts found in the file
char **get_initial_hosts(char *file_name);

// returns the paths found in the file
char **get_initial_paths(char *file_name);

// frees memory allocated by "get_initial_hosts" and "get_initial_paths"
void free_initial_pages(char **hosts, char **paths, char *file_name);

#endif

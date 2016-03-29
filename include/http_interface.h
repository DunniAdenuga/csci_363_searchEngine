#ifndef __HTTP_INTERFACE_H_
#define __HTTP_INTERFACE_H_

/**
 * This collection of functions provides a simple way to interface to servers
 * using the html protocol. Simply provide a host (without the protocol
 * "http://" or https://") and the function will return the html for the page.
 */

char *get_page_content(char *host, char *path);

#endif

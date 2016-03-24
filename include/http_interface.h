#ifndef __HTTP_INTERFACE_H_
#define __HTTP_INTERFACE_H_

char *get_page(char *host, char *path);
int make_http_request(char *host, char *path, char *svc);
char *get_http_response(int sock_fd);

#endif

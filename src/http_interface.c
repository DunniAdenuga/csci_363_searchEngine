#include "http_interface.h"
#include "tcplib.h"

#include <string.h>
#include <stdio.h>

#define HTTPPORT 80
#define BUFFSIZE 100000

/**
 * A collection of functions which assist in retrieving html page content.
 */

// helper functions used locally
static int make_http_request(char *host, char *path, char *svc);
static char *get_http_response(int sock_fd);
static char *get_host_without_protocol(char *host);


// get the html from the page of host and path
char *get_page_content(char *host, char *path){
  char *host_without_protocol = get_host_without_protocol(host);
  int sock_fd = make_http_request(host_without_protocol, path, "GET");
  return get_http_response(sock_fd);
}

// make a request to the server and return the socket fd
int make_http_request(char *host, char *path, char *svc)
{
  int	port = HTTPPORT;
  int	conn;
  int	len;

  char out_buff[512];
	
  /* contact the web server */
  conn = socketClient(host, port);
  if (conn < 0){
    exit(1);
  }

  /* send an HTTP/1.1 request to the webserver */

  sprintf(out_buff, "%s %s HTTP/1.1\r\nHost: %s\r\n", svc, path, host);
  strcat(out_buff, "User-Agent: self-module\r\n");
  strcat(out_buff, "Accept: text/html,applicaiton/xhtml+xml\r\n");
  strcat(out_buff, "Accept-Language: en-us,en;q=0.5\r\n");
  //	strcat(out_buff, "Accept-Encoding: gzip,defalte\r\n");
  strcat(out_buff, "Accept-Charset: ISO-8859-1,utf-8;q=0.7\r\n");
  strcat(out_buff, "Keep-Alive: timeout=60\r\n");
  //  strcat(out_buff, "Connection: keep-alive\r\n");
  strcat(out_buff, "Connection: close\r\n");
  strcat(out_buff, "\r\n");

  len = strlen(out_buff);
  (void) send(conn, out_buff, len, 0);
  return conn;
}

// receive an http response on a file descriptor where a request has been made
char *get_http_response(int sock_fd){
  char *out_buff = malloc(BUFFSIZE);
  int len = recv(sock_fd, out_buff, BUFFSIZE-1, 0);
  out_buff[len] = '\0';             // null terminate

  return out_buff;
}

// strips the http and https protocol specifiers from a host name
char *get_host_without_protocol(char *host){
  char *http = "http://";
  char *https = "https://";
  char *myhost = host;

  if(strncmp(host, http, strlen(http)) == 0){
    myhost = host + strlen(http);
  }else if(strncmp(host, https, strlen(https)) == 0){
    myhost = host + strlen(https);
  }
  return myhost;
}


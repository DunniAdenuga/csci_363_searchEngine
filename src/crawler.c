/*
 * Original version from Doug Comer's book "Internetworking with TCP/IP"
 * Revised for CSCI 363
 * Xiannong Meng <xmeng@bucknell.edu>
 * 2015-12-11
 */

/* webclient.c */

#define _GNU_SOURCE     // we want to use strcasestr()
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "tcplib.h"

#define BUFFSIZE	512
#define HTTPPORT   80

// function definitions

char *get_page(char *host, char *path);
char *get_response(int sock_fd);
char *str_join(char **buf);


int str_expand(char *buf, int cur_size);
int make_request(char *host, char *path, char *svc);

// get the html from the page of host and path
char *get_page(char *host, char *path){
  int sock_fd = make_request(host, path, "GET");
  return get_response(sock_fd);
}

// make a request to the server and return the socket fd
int make_request(char *host, char *path, char *svc)
{
  int	port = HTTPPORT;
  int	conn;
  int	len;

  char out_buff[512];
	
  /* contact the web server */
  conn = socketClient(host, port);
  if (conn < 0) 
    exit(1);

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

// get the response of a server connected on the provided socket fd
char *get_response(int sock_fd){
  char *out_buff[2048];
  int i; 
  int len;

  out_buff[0] = malloc(BUFFSIZE);

  // collect page text
  for(i = 0; (len = recv(sock_fd, out_buff[i], BUFFSIZE-1, 0)) > 0; i++){
    out_buff[i][len] = '\0';             // null terminate
    out_buff[i+1] = malloc(BUFFSIZE);    // prepare next buffer
    //printf("%s\n", out_buff[i]);
  }

  return str_join(out_buff);              // out_buff buffers freed by str_join
}

// join the strings of a **buff into a single string and free the buffers
char *str_join(char **buff){
  char *out_buff;
  int i;
  int total = 0;

  // calculate total space required
  for(i = 0; buff[i] != NULL; i++){
    total += strlen(buff[i]);
  }

  // initialize the buffer
  out_buff = malloc(total + 1);
  out_buff[0] = '\0';

  // concatenate the strings and free
  for(i = 0; buff[i] != NULL; i++){
    strcat(out_buff, buff[i]);
    free(buff[i]);
  }

  return out_buff;
}

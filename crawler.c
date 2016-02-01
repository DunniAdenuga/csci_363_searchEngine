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


char *make_request(char *host, char *path, char *svc);    // request service from web server
char *get_host(char *page_name);
char *get_path(char *page_name);

char *get_page(char *page_name){
  char *host = get_host(page_name);
  char *path = get_path(page_name);
  return make_request(host, path, "GET");
}

char *get_host(char *page_name){
  return "www.bucknell.edu";
}

char *get_path(char *page_name){
  return "/";
}

char *make_request(char *host, char *path, char *svc)
{
  int	port = HTTPPORT;
  int	conn;
  int	len;
  int app = HTTPPORT;

  int cur_size = BUFFSIZE;
  int cur_space = cur_size;

  char in_buff[BUFFSIZE];
  char *out_buff = malloc(cur_size);
	
  /* contact the web server */
  conn = socketClient(host, app);
  if (conn < 0) 
    exit(1);

  /* send an HTTP/1.1 request to the webserver */

  sprint(out_buff, "%s %s HTTP/1.1\r\nHost: %s\r\n", svc, path, host);
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

  /* dump all data received from the server to stdout */

  while((len = recv(conn, in_buff, BUFFSIZE-1, 0)) > 0){
    // increase buffer size if necessary
    in_buff[len] = '\0';    // null ternimate string
    cur_space -= len;
    if(cur_space <= 0){
      int added = str_expand(out_buff, cur_size);
      cur_size += added;
      cur_space += added;
    }

    strcat(out_buff, in_buff);
  }

  return out_buff;
}

int str_expand(char *buff, int cur_size){
  int new_size = 2*cur_size;
  char *new_buff = malloc(new_size);
  strcpy(new_buff, buff);
  free(buff);
  buff = new_buff;
  return new_size - cur_size;
}

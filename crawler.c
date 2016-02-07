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
char *get_page(char *page_name);
char *make_request(char *host, char *path, char *svc);
char *get_host(char *page_name);
char *get_path(char *page_name);
int str_expand(char *buf, int cur_size);
char *str_join(char **buf, int total_len);


/*int main(int argc, char **argv){
  char page_name[BUFFSIZE];
  fgets(page_name, BUFFSIZE, stdin);

  char *page_code = get_page(page_name);
  printf("%s\n", page_code);
  return 0;
  }*/

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

  char in_buff[BUFFSIZE];
  char out_buff[512];
  char *page_buff[2048];
	
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

  /* dump all data received from the server to stdout */
  int i = 0;
  int total = 0;
  page_buff[0] = malloc(BUFFSIZE);
  while((len = recv(conn, in_buff, BUFFSIZE-1, 0)) > 0){
    page_buff[i][len] = '\0';
    printf("%s\n", page_buff[i]);
    i++;
    total += len;
    page_buff[i] = malloc(BUFFSIZE);
  }

  return str_join(page_buff, total + 1);
}

char *str_join(char **buff, int total_len){
  char *out_buff = malloc(total_len);
  out_buff[0] = '\0';
  int i;
  for(i = 0; buff[i] != NULL; i++){
    strcat(out_buff, buff[i]);
    free(buff[i]);
  }
  return out_buff;
}

int str_expand(char *buff, int cur_size){
  int new_size = 2*cur_size;
  char *new_buff = calloc(new_size, sizeof(char));
  strcpy(new_buff, buff);
  buff = new_buff;
  return new_size - cur_size;
}

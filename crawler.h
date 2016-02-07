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



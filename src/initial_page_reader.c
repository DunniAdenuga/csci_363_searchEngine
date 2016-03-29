#include "initial_page_reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define BUFF_SIZE   1024

/**
 * A collection of functions for reading the urls stored in the initial_pages
 * text file (described in the header file).
 */

// element level commands
void skip_element_at_current_position(FILE *fp);
void read_element_at_current_position(FILE *fp, char **result);

// line level commands
void get_path_from_current_line(FILE *fp, char **result);
void get_host_from_current_line(FILE *fp, char **result);

// file opening commands
int get_initial_fd(char *file_name);
FILE *get_initial_fp(char *file_name);


// gets the number of pages stored in the file
int get_initial_page_count(char *file_name){
  FILE *fp = get_initial_fp(file_name);
  int count = 0;
  char *buff = NULL;
  size_t size = 0;

  while(getline(&buff, &size, fp) != -1){
    count++;
  }

  free(buff);
  fclose(fp);
  return count;
}

// gets a list of hosts from the file
char **get_initial_hosts(char *file_name){
  int count = get_initial_page_count(file_name);
  char **hosts = calloc(count, sizeof(char *));
  FILE *fp = get_initial_fp(file_name);

  for(int i = 0; i < count; i++){
    get_host_from_current_line(fp, &hosts[i]);
  }

  fclose(fp);
  return hosts;
}

// gets a list of paths from the file
char **get_initial_paths(char *file_name){
  int count = get_initial_page_count(file_name);
  char **paths = calloc(count, sizeof(char *));
  FILE *fp = get_initial_fp(file_name);

  for(int i = 0; i < count; i++){
    get_path_from_current_line(fp, &paths[i]);
  }

  fclose(fp);
  return paths;
}

// frees the memory allocated in "get_initial_hosts" and "get_initial_paths"
void free_initial_pages(char **hosts, char **paths, char *file_name){
  int count = get_initial_page_count(file_name);
  for(int i = 0; i < count; i++){
    free(hosts[i]);
    free(paths[i]);
  }
  free(hosts);
  free(paths);
}

void get_host_from_current_line(FILE *fp, char **result){
  read_element_at_current_position(fp, result);
  skip_element_at_current_position(fp);
}

void get_path_from_current_line(FILE *fp, char **result){
  skip_element_at_current_position(fp);
  read_element_at_current_position(fp, result);
}

void skip_element_at_current_position(FILE *fp){
  char *buff = malloc(BUFF_SIZE);
  fscanf(fp, "%s", buff);
  free(buff);
}

void read_element_at_current_position(FILE *fp, char **result){
  char *buff = malloc(BUFF_SIZE);
  fscanf(fp, "%s", buff);
  *result = malloc(strlen(buff) + 1);
  strcpy(*result, buff);
  free(buff);
}

// opens a file pointer appropriately for the file
FILE *get_initial_fp(char *file_name){
  FILE *fp = fdopen(get_initial_fd(file_name), "r");
  if(fp == NULL){
    perror("File Pointer Open Error");
  }
  return fp;
}

// opens a file descriptor appropriately for the file
int get_initial_fd(char *file_name){
  int fd = open(file_name, O_RDONLY, 0600);

  if(fd < 0){
    perror("File Descriptor Open Error");
    exit(1);
  } 
  return fd;
}

#include "initial_page_reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define BUFF_SIZE   1024

void skip_element_at_current_position(FILE *fp);
void read_element_at_current_position(FILE *fp, char **result);

void get_path_from_current_line(FILE *fp, char **result);
void get_host_from_current_line(FILE *fp, char **result);

int get_initial_fd(char *file_name);
FILE *get_initial_fp(char *file_name);


// functions defined in header
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

void free_initial_pages(char **hosts, char **paths, char *file_name){
  int count = get_initial_page_count(file_name);
  for(int i = 0; i < count; i++){
    free(hosts[i]);
    free(paths[i]);
  }
  free(hosts);
  free(paths);
}

// helper functions
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

FILE *get_initial_fp(char *file_name){
  FILE *fp = fdopen(get_initial_fd(file_name), "r");
  if(fp == NULL){
    perror("File Pointer Open Error");
  }
  return fp;
}

int get_initial_fd(char *file_name){
  int fd = open(file_name, O_RDONLY, 0600);

  if(fd < 0){
    perror("File Descriptor Open Error");
    exit(1);
  } 
  return fd;
}

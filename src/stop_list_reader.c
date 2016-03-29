#include "stop_list_reader.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#define BUFFSIZE 1024

/**
 * A collection of functions for reading the words stored in the stop list test
 * file (described in the header file).
 */

// internal helper functions
static void get_word_from_current_line(FILE *fp, char **words);
static int get_stop_word_fd(char *file_name);
static FILE *get_stop_word_fp(char *file_name);

// returns the number of words in the file
int get_stop_word_count(char *file_name){
  FILE *fp = get_stop_word_fp(file_name);
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

// returns the words in the file
char **get_stop_words(char *file_name){
  int count = get_stop_word_count(file_name);
  char **words = calloc(count, sizeof(char *));
  FILE *fp = get_stop_word_fp(file_name);

  for(int i = 0; i < count; i++){
    get_word_from_current_line(fp, &words[i]);
  }

  fclose(fp);
  return words;
}

// frees the memory allocated in "get_stop_words"
void free_stop_words(char **words, char *file_name){
  int count = get_stop_word_count(file_name);
  for(int i = 0; i < count; i++){
    free(words[i]);
  }
  free(words);
}

// reads the word on the current line
void get_word_from_current_line(FILE *fp, char **result){
  char buff[BUFFSIZE];
  fscanf(fp, "%s", buff);
  *result = malloc(strlen(buff) + 1);
  strcpy(*result, buff);
}

// opens a file pointer appropriately for the file
FILE *get_stop_word_fp(char *file_name){
  FILE *fp = fdopen(get_stop_word_fd(file_name), "r");
  if(fp == NULL){
    perror("File Pointer Open Error");
  }
  return fp;
}

// opens a file descriptor appropriately for the file
int get_stop_word_fd(char *file_name){
  int fd = open(file_name, O_RDONLY, 0600);

  if(fd < 0){
    perror("File Descriptor Open Error");
    exit(1);
  } 
  return fd;
}

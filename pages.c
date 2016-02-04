#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "crawler.h"
extern char **environ;

//gcc  embedpy.o $(/usr/bin/python2.7-config --ldflags)

#define MAX_PAGE_SIZE 9999
#define STDOUT 1
#define STDIN 0

int fd[2];//for pipe

int main(int argc, char* argv[]){
  if(argc < 2){
    exit(1);
  }
  char* initial_page = malloc(MAX_PAGE_SIZE);
  char* results = malloc(MAX_PAGE_SIZE);
  initial_page = argv[1];//first page to start search from
  
  results = get_page(initial_page);
  while(results != NULL){
  Pipe(fd);//create a pipe
   pid = Fork();

  
  
 
  if(pid > 0){//parent process
    dup2(0, fd[1]);
    write(fd[1], results, strlen(results));
    read(fd[0], initial_page, MAX_PAGE_SIZE);
  }

  else{
    execl("python", "python", "parseURL.py");
    dup2(1, fd[0]);
  }
  }
}

int Fork(void)
{
  int pidy = fork();
  if(-1 == pidy)
{
  perror("Error! No child process was created.");
  //printf("errno = %d.\n", errno);
  exit(-1);
}
  return pidy;
}

int Pipe(int pipefd[2])
{
  if(pipe(pipefd) == -1)
    {
      perror("Pipe Failed");
      exit(-1);
    }
  return 1;
}

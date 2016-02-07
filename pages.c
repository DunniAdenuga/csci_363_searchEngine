#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "crawler.h"
extern char **environ;

//gcc  embedpy.o $(/usr/bin/python2.7-config --ldflags)

#define MAX_PAGE_SIZE 9999
#define STDOUT 1
#define STDIN 0
#define WRITE 1
#define READ 0

int Fork(void);
int Pipe(int*);

int main(int argc, char* argv[]){
  pid_t pid_parseURL;
  //int status;

  int com_to_parseURL[2];        // pipe to parseURL
  int com_from_parseURL[2];      // pipe from parseURL

  if(argc < 3){
    printf("USAGE: ./pages <PAGE_HOST> <PAGE_PATH\n");
    exit(1);
  }

  //char* initial_page = malloc(MAX_PAGE_SIZE);
  char* results = malloc(MAX_PAGE_SIZE);
  char* stuff = malloc(MAX_PAGE_SIZE);

  char *initial_host = argv[1];       //first page to start search from
  char *initial_path = argv[2];       //first page to start search from
  
  results = get_page(initial_host, initial_path);
  printf("%s\n", results);

  //while(results != NULL){                  // used for multiple pages
  Pipe(com_to_parseURL);                     //create a pipe
  Pipe(com_from_parseURL);                   //create a pipe
  pid_parseURL = Fork();

  if(pid_parseURL > 0){    // parent process
    
    write(com_to_parseURL[WRITE], results, strlen(results));
    close(com_to_parseURL[WRITE]);
    read(com_from_parseURL[READ], stuff, 5);
    close(com_from_parseURL[READ]);
    printf("%s\n", stuff);

  } else{         // Child process
    dup2(STDIN, com_to_parseURL[READ]);//this is the right format
    dup2( STDOUT, com_from_parseURL[WRITE]);//this is the right format
    execl("/usr/remote/python-3.2/bin/python3", "/bin/python3", "parseURL.py", (char *)NULL);
  }
  //}
  
  sleep(3);
  if(pid_parseURL > 0){
    kill(pid_parseURL, 7);
  }
  return 1;
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

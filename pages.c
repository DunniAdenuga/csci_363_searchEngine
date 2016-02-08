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
#define WRITE 1
#define READ 0

int Fork(void);
int Pipe(int*);
int Dup2(int, int);

int main(int argc, char* argv[]){
  pid_t pid_parseURL;
  //int status;

  int com_to_parseURL[2];        // pipe to parseURL
  int com_from_parseURL[2];      // pipe from parseURL

  if(argc < 3){
    printf("USAGE: ./pages <PAGE_HOST> <PAGE_PATH\n");
    exit(1);
  }

  char* results;
  char* stuff = malloc(MAX_PAGE_SIZE);

  char *initial_host = argv[1];       //first page to start search from
  char *initial_path = argv[2];       //first page to start search from
  
  results = get_page(initial_host, initial_path);

  //while(results != NULL){                  // used for multiple pages
  Pipe(com_to_parseURL);                     //create a pipe
  Pipe(com_from_parseURL);                   //create a pipe
  pid_parseURL = Fork();

  if(pid_parseURL > 0){    // parent process
    // close unwanted ends of pipes
    close(com_to_parseURL[READ]);
    close(com_from_parseURL[WRITE]);

    // write to and read results from child
    write(com_to_parseURL[WRITE], results, strlen(results));
    read(com_from_parseURL[READ], stuff, sizeof(stuff));

    free(results);

    // print the output
    printf("Read from the parser: %s\n", stuff);

  } else{         // Child process
    // redirect pipes to stdin and stdout
    Dup2(com_to_parseURL[READ], STDIN_FILENO);
    Dup2(com_from_parseURL[WRITE], STDOUT_FILENO);

    // close unneeded pipes (all of them)
    close(com_to_parseURL[WRITE]);
    close(com_from_parseURL[READ]);

    // begin the parsing process
    execl("/usr/remote/python-3.2/bin/python3", "/bin/python3", "parseURL.py", (char *)NULL);
  }
  //}
  
  close(com_to_parseURL[WRITE]);
  close(com_to_parseURL[READ]);

  sleep(3);
  printf("After sleep: %d\n", pid_parseURL);
  if(pid_parseURL > 0){
    kill(pid_parseURL, 7);
  }
  return 1;
}

int Fork(void)
{
  int pid = fork();
  if(pid == -1)
  {
    perror("Error! No child process was created.");
    //printf("errno = %d.\n", errno);
    exit(-1);
  }
  return pid;
}

int Pipe(int pipefd[2])
{
  int result = pipe(pipefd);
  if(result == -1)
  {
    perror("Pipe Failed");
    exit(-1);
  }
  return result;
}

int Dup2(int oldfd, int newfd){
  int result = dup2(oldfd, newfd);
  if(result == -1){
    perror("Failed Dup2");
    exit(-1);
  }
  return result;
}

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

void print_urls(char *, ssize_t);

int Fork(void);
int Pipe(int*);
int Dup2(int, int);

int main(int argc, char* argv[]){
  pid_t pid_parseURL;
  //int status;

  int com_to_parseURL[2];        // pipe to parseURL
  int com_from_parseURL[2];      // pipe from parseURL

  if(argc < 3){
    printf("USAGE: ./pages <PAGE_HOST> <PAGE_PATH>\n");
    exit(1);
  }

  char* results;
  char* stuff;

  char *initial_host = argv[1];       //first page to start search from
  char *initial_path = argv[2];       //first page to start search from
 
  //char host[strlen(initial_host)];
  //memcpy(host, initial_path,
 
  //char * host = "www.MICHAEL.edu";

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
    char *term = "\nterminate\n";

    write(com_to_parseURL[WRITE], initial_host, strlen(initial_host));
    write(com_to_parseURL[WRITE], "\n", 1);

    write(com_to_parseURL[WRITE], results, strlen(results));
    write(com_to_parseURL[WRITE], term, strlen(term));
 
    //read(com_from_parseURL[READ], &msg_len, sizeof(int));
    stuff = malloc(MAX_PAGE_SIZE);
    int return_len = read(com_from_parseURL[READ], stuff, MAX_PAGE_SIZE);
    stuff[return_len] = '\0';
    free(results);

    // print the output
    print_urls(stuff, return_len);
    fflush(stdout);

  } else{         // Child process
    // redirect pipes to stdin and stdout
    Dup2(com_to_parseURL[READ], STDIN_FILENO);
    Dup2(com_from_parseURL[WRITE], STDOUT_FILENO);

    // close unneeded pipes (all of them)
    close(com_to_parseURL[WRITE]);
    close(com_from_parseURL[READ]);

    // begin the parsing process
    execl("/usr/remote/python-3.2/bin/python3", "/bin/python3", "python/parseURL.py", (char *)NULL);
  }
  //}
  
  close(com_to_parseURL[WRITE]);
  close(com_to_parseURL[READ]);

  sleep(3);

  if(pid_parseURL > 0){
    kill(pid_parseURL, 7);
  }
  return 1;
}

void print_urls(char *urls, ssize_t size){
  FILE *in_urls = fmemopen(urls, size, "r");
  
  char host[512];
  char path[2048];

  char *host_return;
  char *path_return;

  host_return = fgets(host, sizeof(host), in_urls);
  path_return = fgets(path, sizeof(path), in_urls);

  host[strlen(host) - 1] = '\0';
  path[strlen(path) - 1] = '\0';

  while( host_return != NULL && path_return != NULL ){
    printf("%s%s\n", host, path);

    host_return = fgets(host, sizeof(host), in_urls);
    path_return = fgets(path, sizeof(path), in_urls);

    host[strlen(host) - 1] = '\0';
    path[strlen(path) - 1] = '\0';
  }
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

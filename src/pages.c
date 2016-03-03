#include "pages.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "crawler.h"
#include "queryFinder.h"

extern char **environ;

//gcc  embedpy.o $(/usr/bin/python2.7-config --ldflags)

#define MAX_PAGE_SIZE 80000 
#define WRITE 1
#define READ 0

// wrapper functions
int Fork(void);
int Pipe(int*);
int Dup2(int, int);

// parsing functions
char *get_urls(char *page_content, char *page_host);
char *get_words(char *page_content);

// helper functions
void print_urls(char *page_urls);
void add_words(char *page_words);
void query_loop();

void init_crawler(char *initial_host, char *initial_path){
  // get the page results
  char *page_content = get_page(initial_host, initial_path);
  char *page_urls = get_urls(page_content, initial_host);
  char *page_words = get_words(page_content);

  printf("%s\n", page_words);

  // add words to memory
  add_words(page_words);

  free(page_content);
  free(page_urls);
  free(page_words);
}

char *get_results_urls(char *query){
  if(searchQuery(query)){
    return "https://www.bucknell.edu/\n";
  }else{
    return NULL;
  }
}

char *get_response_page(char *urls){
  char *response_page;
  pid_t pid_genPage;

  // pipe for interprocess communication
  int com_to_genPage[2];
  int com_from_genPage[2];

  //while(results != NULL){                  // used for multiple pages
  Pipe(com_to_genPage);                     //create a pipe
  Pipe(com_from_genPage);                   //create a pipe
  pid_genPage = Fork();

  if(pid_genPage > 0){    // parent process
    // close unwanted ends of pipes
    close(com_to_genPage[READ]);
    close(com_from_genPage[WRITE]);

    // write to and read results from child
    char *term = "\nterminate\n";

    write(com_to_genPage[WRITE], urls, strlen(urls));
    write(com_to_genPage[WRITE], term, strlen(term));
 
    //read(com_from_parseURL[READ], &msg_len, sizeof(int));
    response_page = malloc(MAX_PAGE_SIZE);
    int return_len = read(com_from_genPage[READ], response_page, MAX_PAGE_SIZE);
    response_page[return_len] = '\0';

    // print the output
    // print_urls(stuff, return_len);
    // fflush(stdout);

  } else{         // Child process
    // redirect pipes to stdin and stdout
    Dup2(com_to_genPage[READ], STDIN_FILENO);
    Dup2(com_from_genPage[WRITE], STDOUT_FILENO);

    // close unneeded pipes (all of them)
    close(com_to_genPage[WRITE]);
    close(com_from_genPage[READ]);

    // begin the parsing process
    execl("/usr/remote/python-3.2/bin/python3", "/bin/python3", "python/genPage.py", (char *)NULL);
  }
  //}
  
  close(com_to_genPage[WRITE]);
  close(com_to_genPage[READ]);

  if(pid_genPage > 0){
    kill(pid_genPage, 7);
  }

  return response_page;
}

void print_urls(char *urls){
  ssize_t size = strlen(urls);
  FILE *in_urls = fmemopen(urls, size, "r");
  
  char host[512];
  char path[2048];

  // get first host and path from urls string
  char *host_return = fgets(host, sizeof(host), in_urls);
  char *path_return = fgets(path, sizeof(path), in_urls);

  // strip newline
  host[strlen(host) - 1] = '\0';
  path[strlen(path) - 1] = '\0';

  // loop through all host, path pairs
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

/*
 * Runs a python script which will extract the urls from a web page.
 * The script reads a web page on the standard input and outputs
 * the discovered urls on the standard output.
 *
 * page_content   - A null terminated string containing the web page
 * return         - A string with one discovered url per line
 */
char *get_urls(char *page_content, char *page_host){
  char *urls;
  pid_t pid_parseURL;

  // pipe for interprocess communication
  int com_to_parseURL[2];
  int com_from_parseURL[2];

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

    write(com_to_parseURL[WRITE], page_host, strlen(page_host));
    write(com_to_parseURL[WRITE], "\n", 1);

    write(com_to_parseURL[WRITE], page_content, strlen(page_content));
    write(com_to_parseURL[WRITE], term, strlen(term));
 
    //read(com_from_parseURL[READ], &msg_len, sizeof(int));
    urls = malloc(MAX_PAGE_SIZE);
    int return_len = read(com_from_parseURL[READ], urls, MAX_PAGE_SIZE);
    urls[return_len] = '\0';

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

  if(pid_parseURL > 0){
    kill(pid_parseURL, 7);
  }

  return urls;
}

/*
 * Runs a python script which will extract the words from a web page.
 * The script reads a web page on the standard input and outputs
 * the discovered words on the standard output.
 *
 * page_content   - A null terminated string containing the web page
 * return         - A string with one discovered word per line
 */
char *get_words(char *page_content){
  char *words;
  pid_t pid_parseText;

  // pipe for interprocess communication
  int com_to_parseText[2];
  int com_from_parseText[2];

  //while(results != NULL){                  // used for multiple pages
  Pipe(com_to_parseText);                     //create a pipe
  Pipe(com_from_parseText);                   //create a pipe
  pid_parseText = Fork();

  if(pid_parseText > 0){    // parent process
    // close unwanted ends of pipes
    close(com_to_parseText[READ]);
    close(com_from_parseText[WRITE]);

    // write to and read results from child
    char *term = "\nterminate\n";

    write(com_to_parseText[WRITE], page_content, strlen(page_content));
    write(com_to_parseText[WRITE], term, strlen(term));
 
    //read(com_from_parseURL[READ], &msg_len, sizeof(int));
    words = malloc(MAX_PAGE_SIZE);
    int return_len = read(com_from_parseText[READ], words, MAX_PAGE_SIZE);
    words[return_len] = '\0';

    // print the output
    // print_urls(stuff, return_len);
    // fflush(stdout);

  } else{         // Child process
    // redirect pipes to stdin and stdout
    Dup2(com_to_parseText[READ], STDIN_FILENO);
    Dup2(com_from_parseText[WRITE], STDOUT_FILENO);

    // close unneeded pipes (all of them)
    close(com_to_parseText[WRITE]);
    close(com_from_parseText[READ]);

    // begin the parsing process
    execl("/usr/remote/python-3.2/bin/python3", "/bin/python3", "python/parseText.py", (char *)NULL);
  }
  //}
  
  close(com_to_parseText[WRITE]);
  close(com_to_parseText[READ]);

  if(pid_parseText > 0){
    kill(pid_parseText, 7);
  }

  return words;
}

void add_words(char *words){
  initFinder();

  FILE *in_words = fmemopen(words, strlen(words), "r");
  char word[80];

  // get first word and strip newline
  char *word_return = fgets(word, sizeof(word), in_words);
  words[strlen(word) - 1] = '\0';

  // loop through all host, path pairs
  while( word_return != NULL ){
    char *new_word = malloc(strlen(word) + 1);
    strcpy(new_word, word);
    insertWord(new_word);

    word_return = fgets(word, sizeof(word), in_words);
    word[strlen(word) - 1] = '\0';
  }
}

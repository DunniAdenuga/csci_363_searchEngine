#include "inv_list.h"
#include "site_list.h"
#include "word_list.h"
#include "pages.h"
#include "python_interface.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#define MAX 80000
#define MAX_PAGE_SIZE 80000

#define WRITE 1
#define READ 0

char *page_content;

char *get_urls(char *page_content);
char *get_words(char *page_content);

int Fork(void);
int Pipe(int pipefd[2]);
int Dup2(int oldfd, int newfd);

/*char* control(char *initial_host, char *initial_path){
 page_content = get_page(initial_host, initial_path);
 }*/
struct site_list* get_urls_wrapper(char *page_content){
  struct site_list* results = sl_create();
  char *urls = get_urls(page_content);
  char* host;
  char* path;

  host = strtok(urls, "\n");
  path = strtok(NULL, "\n");
  sl_add(results, host, path, 0);

  do{
    host = strtok(NULL, "\n");
    path = strtok(NULL, "\n");
    sl_add(results, host, path, 0);
  }while(path != NULL) ;

  free(urls);
  return results;
}

struct word_list* get_words_wrapper(char *host, char *path, char *page_content){
  struct word_list* results = wl_create();
  char *urls = get_words(page_content);
  char* freq_str;
  char* word;
  int freq;

  freq_str = strtok(urls, "\n");
  word = strtok(NULL, "\n");
  freq = atoi(freq_str);

  wl_add(results, word, host, path, freq);

  do{
    freq_str = strtok(NULL, "\n");
    word = strtok(NULL, "\n");
    freq = atoi(freq_str);
    wl_add(results, word, host, path, freq);
  }while(word != NULL) ;

  free(urls);
  return results;
}

char* get_response_page_wrapper(struct site_list* list){
  struct site_node* node;
  char urls[MAX];
  sl_iter_begin(list);
  while((node = sl_iter_curr(list)) != NULL){
    strcat(urls, node->host);
    strcat(urls, node->path);
    strcat(urls, "\n");
    sl_iter_next(list);
  }
  
  return get_response_page(urls);
}

/* -------------------------------------------------------
 * Generates a "results" page using the urls given
 *
 * parameters:
 *   urls:  a string of newline seperated urls to be
 *          included in the results
 *
 * return:
 *   a string of html to be returned as the response
 * -------------------------------------------------------
 */
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


  /* -------------------------------------------------------
 * Runs a python script which will extract the urls from 
 * a web page. The script reads a web page on the standard 
 * input and outputs the discovered urls on the standard 
 * output.
 *
 * parameters:
 *   page_content: A null terminated string containing 
 *                 the web page
 *
 * return: 
 *   A string with one discovered url per line
 * -------------------------------------------------------
 */
char *get_urls(char *page_content){
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

/* -------------------------------------------------------
 * Runs a python script which will extract the words from 
 * a web page. The script reads a web page on the standard 
 * input and outputs the discovered words on the standard 
 * output.
 *
 * parameters:
 *   page_content: A null terminated string containing 
 *                 the web page
 *
 * return:
 *   A string with one discovered word per line
 * -------------------------------------------------------
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

/* -------------------------------------------------------
 * A wrapper for the fork function
 * -------------------------------------------------------
 */
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

/* -------------------------------------------------------
 * A wrapper for the pipe function
 * -------------------------------------------------------
 */
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

/* -------------------------------------------------------
 * A wrapper for the dup2 function
 * -------------------------------------------------------
 */
int Dup2(int oldfd, int newfd){
  int result = dup2(oldfd, newfd);
  if(result == -1){
    perror("Failed Dup2");
    exit(-1);
  }
  return result;
}

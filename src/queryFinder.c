#include "stdio.h"
 #include <sys/types.h>
 #include <sys/stat.h>
 #include <fcntl.h>
 #include "dlist.h"
 #include "dnode.h"
#include<string.h>
#include<stdlib.h>
void putWordsinList(void);
bool searchQuery(char* target);
void createStopList(void);
bool searchStopList(char* word);
void sortListOfWords(void);
bool dotheWork(char * query);
void traverse_forward(struct dlist *l);

 //file descriptor of file that stores content of word-page
struct dlist* stopWordList = NULL;
struct dlist* listOfWords = NULL;

void insertWord(char *word){
    if(searchStopList(word) == false){
      dlist_add_back(listOfWords, (void *)word);	 
    }
}

void putWordsinList(void){
  FILE *fd;

  fd = fopen("something.txt", "r");
  int c;
  do {
    char*  oneword = malloc(50);
    c = fscanf(fd,"%s",oneword); /* got one word from the file */
    if(searchStopList(oneword) == false){
      dlist_add_back(listOfWords, (void *)oneword);	 

    }
  } while (c != EOF);              /* repeat until EOF           */

  fclose(fd);
}

bool searchQuery(char* target){
  if(strcmp(target, dlist_iter_begin(listOfWords)->data) == 0){
    return true;
  }
  while(dlist_iter_has_next(listOfWords) != false){
    if(strcmp(target, dlist_iter_next(listOfWords)->data) == 0){
      return true;
    }
  }
  return false;

}

void createStopList(void){
  FILE *fd;

  int c;
  fd = fopen("resources/stop-words.txt", "r");
  char* one = malloc(50); 
  c = fscanf(fd,"%s",one); /* got one word from the file */
  dlist_add_back(stopWordList, one);
  //printf("%s\n", one);
  while(c != EOF)
  {
    char*  oneword = malloc(50);  
    dlist_add_back(stopWordList, oneword);
    c = fscanf(fd,"%s",oneword); /* got one word from the file */
    //printf("%s\n", oneword);
  } //while (c != EOF);              /* repeat until EOF           */
  //printf("here\n");
  fclose(fd);
}
bool searchStopList(char* word){
  if(strcmp(word, dlist_iter_begin(stopWordList)->data) == 0){
    return true;
  }
  while(dlist_iter_has_next(stopWordList) != false){
    if(strcmp(word, dlist_iter_next(stopWordList)->data) == 0){
      return true;
    }
  }
  return false;
}

void sortListOfWords(void){
  /* Checking for empty list */
  if (listOfWords == NULL)
    return;
  char temp[350] ;
  bool sorted ; 
  struct dnode* start;
  do{
    sorted = false;
    start = listOfWords->front;
    //char* word = listOfWords->iter->data;
    dlist_iter_begin(listOfWords);
    while(start->next != NULL){
      if (strcmp(start->data, start->next->data) > 0) {
        strcpy(temp, start->data);
        strcpy(start->data, start->next->data);
        strcpy(start->next->data, temp);

        sorted = true;
      }
      start = start->next;

    }
  }while(sorted);
}

void initFinder(){
  stopWordList = dlist_create();
  listOfWords = dlist_create();
  createStopList();
}

bool dotheWork(char * query){
  stopWordList = dlist_create();
  listOfWords = dlist_create();
  createStopList();
  traverse_forward(stopWordList);
  putWordsinList();

  traverse_forward(listOfWords);
  sortListOfWords();
  traverse_forward(listOfWords);
  return searchQuery(query);
}

void traverse_forward(struct dlist *l) {
  char *str;
  printf("forward traversal\n");
  for (str = dlist_iter_begin(l)->data; l->iter->next != NULL; str = (char *) dlist_iter_next(l)->data)	{ 
    printf("string = %s\n", str);
  }
}

/*
int main(){
  //traverse_forward(stopWordList);
  //traverse_forward(listOfWords);
  if(dotheWork("cnn") == true){
    printf("true\n");
  }
  else{
    printf("false\n");
  }
}
*/

#include "stdio.h"
 #include <sys/types.h>
 #include <sys/stat.h>
 #include <fcntl.h>
 #include "dlist.h"
 #include "dnode.h"
#include<string.h>
#include<stdlib.h>

FILE *fd; //file descriptor of file that stores content of word-page
struct dlist* stopWordList = dlist_create();
struct dlist* listOfWords = dlist_create();

void putWordsinList(void){
  char oneword[100];
  fd = fopen("something.txt", "r");
  do {
      c = fscanf(fp1,"%s",oneword); /* got one word from the file */
      if(searchStopList(oneword) == false){
      dlist_add_back(listOfWords, oneword);
      }
   } while (c != EOF);              /* repeat until EOF           */

   fclose(fd);
}

bool searchQuery(char* target){
if(strcmp(target, dlist_iter_begin(listOfWords)) == 0){
    return true;
  }
  while(dlist_iter_has_next(listOfWords) != false){
    if(strcmp(target, dlist_iter_next(listOfWords)) == 0){
    return true;
  }
  }
  return false;

}

void createStopList(void){
  char oneword[100];
  fd = fopen("stop-word.txt", "r");
  do {
      c = fscanf(fp1,"%s",oneword); /* got one word from the file */
      dlist_add_back(stopWordList, oneword);
   } while (c != EOF);              /* repeat until EOF           */

   fclose(fd);
}
bool searchStopList(char* word){
  if(strcmp(word, dlist_iter_begin(stopWordList)) == 0){
    return true;
  }
  while(dlist_iter_has_next(stopWordList) != false){
    if(strcmp(word, dlist_iter_next(stopWordList)) == 0){
    return true;
  }
  }
  return false;
}

void sortListOfWords(void){
  void* temp;
  int n = (int) dlist_num_elems(listOfWords);
  for (int i = 0; i < n; i++) {
      for (int j = 0; j < n - 1; j++) {
	char* word = listOfWords->iter->data; 
	if (strcmp(word, dlist_iter_next(listOfWords)) > 0) {
            strcpy(temp, word);
            strcpy(dlist_iter_prev(listOfWords), str[j + 1]);
            strcpy(str[j + 1], temp);
         }
      }
  }
}

  bool dotheWork(char * query){
    createStopList();
    putWordsinList();
    sortListOfWords();
    return searchQuery(query);
  }

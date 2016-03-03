/*Dunni , Michael and Greg*/
#include "stdio.h"
 #include <sys/types.h>
 #include <sys/stat.h>
 #include <fcntl.h>
 #include "dlist.h"
 #include "dnode.h"
#include<string.h>
#include<stdlib.h>

/*listOfWords - words in a webpage
  stopWordList - stop words */
/*
Inserts word into listOfWords one at a time 
 */
void insertWord(char *word);
/*
Assumes words are in a file and inserts them into listOfWords at once
 */
void putWordsinList(void);
/*
Sees if word is in listOfWords
 */
bool searchQuery(char* target);
/*
Create a stop-words list 
 */
void createStopList(void);
/*
see if a word is in the stopWordList before adding to list Of Words
 */
bool searchStopList(char* word);
/*
Sort
*/
void sortListOfWords(void);
/*
  creates initial empty lists
*/
void initFinder();

/*
Manages whole Program
Create, sort and search for query.
 */
bool dotheWork(char * query);

/*
Print out list
 */
void traverse_forward(struct dlist *l);

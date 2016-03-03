#include "stdio.h"
 #include <sys/types.h>
 #include <sys/stat.h>
 #include <fcntl.h>
 #include "dlist.h"
 #include "dnode.h"
#include<string.h>
#include<stdlib.h>

void insertWord(char *word);
void putWordsinList(void);
bool searchQuery(char* target);
void createStopList(void);
bool searchStopList(char* word);
void sortListOfWords(void);
void initFinder();
bool dotheWork(char * query);
void traverse_forward(struct dlist *l);

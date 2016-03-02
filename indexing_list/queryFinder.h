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

CC = gcc
CFLAGS = -std=c99 -Wall -g #-DDEBUG


EXECS = dlisttest queryFinder

all: $(EXECS)


dnode.o: dnode.h dnode.c
	$(CC) $(CFLAGS) -c dnode.c -o dnode.o

dlist.o: dlist.h dlist.c dnode.h
	$(CC) $(CFLAGS) -c dlist.c -o dlist.o

dlisttest: dlisttest.c dnode.o dlist.o
	$(CC) $(CFLAGS) dnode.o dlist.o dlisttest.c -o dlisttest

queryFinder: queryFinder.c dnode.o dlist.o
	$(CC) $(CFLAGS) dnode.o dlist.o queryFinder.c -o queryFinder

clean:
	/bin/rm -f dnode.o dlist.o $(EXECS) core* *~ semantic.cache


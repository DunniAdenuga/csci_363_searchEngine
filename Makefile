
CC = gcc -I ./include
CFLAGS = -std=gnu99 -Wall -g #-DDEBUG

INC = ./include
SRC = ./src
OBJ = ./obj
DOC = ./doc
BIN = ./bin

vpath %.h ./include
vpath %.c ./src

CRAWLEROBJS = $(OBJ)/crawler.o $(OBJ)/tcplib.o $(OBJ)/test_crawler.o
PAGEOBJS = $(OBJ)/crawler.o $(OBJ)/tcplib.o $(OBJ)/pages.o $(OBJ)/queryFinder.o $(OBJ)/dlist.o $(OBJ)/dnode.o
SERVEROBJS = $(OBJ)/webserver.o $(OBJ)/send_eof.o $(OBJ)/readln.o $(PAGEOBJS)

EXECS = test_crawler webserver

all: $(EXECS)

doc:
	doxygen

# build all object files
$(OBJ)/%.o: %.c
	$(CC) $< $(CFLAGS) -c -o $@

#$(OBJ)/dnode.o: $(INC)/dnode.h $(SRC)/dnode.c
	#$(CC) $(CFLAGS) -c $(SRC)/dnode.c -o $(OBJ)/dnode.o

#$(OBJ)/dlist.o: $(INC)/dlist.h $(SRC)/dlist.c $(INC)/dnode.h
	#$(CC) $(CFLAGS) -c $(SRC)/dlist.c -o $(OBJ)/dlist.o

#$(OBJ)/queryFinder.o: $(SRC)/queryFinder.c $(OBJ)/dnode.o $(OBJ)/dlist.o
	#$(CC) -o $(BIN)/$@ $(CFLAGS) $(OBJ)/dnode.o $(OBJ)/dlist.o $(SRC)/queryFinder.c

test_crawler: $(CRAWLEROBJS)
	$(CC) -o $(BIN)/$@ $(LFLAGS) $(CRAWLEROBJS)

webserver: $(SERVEROBJS)
	$(CC) -o $(BIN)/$@ $(LFLAGS) $(SERVEROBJS)

.PHONY: clean
clean:
	/bin/rm -rf $(BIN)/* $(OBJ)/* core* *~

cleanswap:
	/bin/rm -f \.*swp
	/bin/rm -f /src/\.*swp


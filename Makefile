
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
PAGEOBJS = $(OBJ)/crawler.o $(OBJ)/tcplib.o $(OBJ)/pages.o
SERVEROBJS = $(OBJ)/webserver.o $(OBJ)/tcplib.o $(OBJ)/send_eof.o $(OBJ)/readln.o

EXECS = test_crawler pages webserver

all: $(EXECS)

doc:
	doxygen

# build all object files
$(OBJ)/%.o: %.c
	$(CC) $< $(CFLAGS) -c -o $@

test_crawler: $(CRAWLEROBJS)
	$(CC) -o $(BIN)/$@ $(LFLAGS) $(CRAWLEROBJS)

pages: $(PAGEOBJS)
	$(CC) -o $(BIN)/$@ $(LFLAGS) $(PAGEOBJS)

webserver: $(SERVEROBJS)
	$(CC) -o $(BIN)/$@ $(LFLAGS) $(SERVEROBJS)


.PHONY: clean
clean:
	/bin/rm -rf $(BIN)/* $(OBJ)/* core* *~

cleanswap:
	/bin/rm -f \.*swp
	/bin/rm -f /src/\.*swp


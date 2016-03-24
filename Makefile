
CC = gcc -I ./include
CFLAGS = -std=gnu99 -Wall -g #-DDEBUG

INC = ./include
SRC = ./src
OBJ = ./obj
DOC = ./doc
BIN = ./bin

vpath %.h ./include
vpath %.c ./src

OBJ_SITE_LIST = $(OBJ)/site_list.o $(OBJ)/site_node.o
OBJ_WORD_LIST = $(OBJ)/word_list.o $(OBJ)/word_node.o $(OBJ_SITE_LIST)
OBJ_INV_LIST = $(OBJ)/inv_list.o $(OBJ_WORD_LIST)

OBJ_CRAWLER = $(OBJ)/initial_page_reader.o $(OBJ)/string_ops.o $(OBJ_INV_LIST) $(OBJ)/crawler.o 
OBJ_TEST_CRAWLER = $(OBJ_CRAWLER) $(OBJ)/test_crawler.o

#PAGEOBJS = $(OBJ)/crawler.o $(OBJ)/tcplib.o $(OBJ)/pages.o $(OBJ)/queryFinder.o $(OBJ)/dlist.o $(OBJ)/dnode.o
#SERVEROBJS = $(OBJ)/webserver.o $(OBJ)/send_eof.o $(OBJ)/readln.o $(PAGEOBJS)

EXECS = test_crawler

all: mkdirs $(EXECS)

doc:
	doxygen

# build all object files
$(OBJ)/%.o: %.c
	$(CC) $< $(CFLAGS) -c -o $@

mkdirs:
	mkdir -p $(OBJ)
	mkdir -p $(DOC)
	mkdir -p $(BIN)

test_crawler: $(OBJ_TEST_CRAWLER)
	$(CC) -o $(BIN)/$@ $(LFLAGS) $(OBJ_TEST_CRAWLER)

#test_crawler: $(CRAWLEROBJS)
	#$(CC) -o $(BIN)/$@ $(LFLAGS) $(OBJ_CRAWLER)

#webserver: $(SERVEROBJS)
	#$(CC) -o $(BIN)/$@ $(LFLAGS) $(SERVEROBJS)

.PHONY: clean
clean:
	/bin/rm -rf $(BIN) $(OBJ) core* *~

cleanswap:
	/bin/rm -f \.*swp */\.*swp
	/bin/rm -f /$(SRC)/\.*swp
	/bin/rm -f /$(INC)/\.*swp



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
OBJ_PY_INT = $(OBJ)/python_interface.o $(OBJ)/tcplib.o

OBJ_CRAWLER = $(OBJ)/http_interface.o $(OBJ)/initial_page_reader.o $(OBJ)/string_ops.o $(OBJ_PY_INT) $(OBJ_INV_LIST) $(OBJ)/crawler.o 
OBJ_TEST_CRAWLER = $(OBJ_CRAWLER) $(OBJ)/test_crawler.o

SERVEROBJS = $(OBJ_CRAWLER) $(OBJ)/query.o $(OBJ)/webserver.o $(OBJ)/send_eof.o
OBJ_CRAWL_PROGRAM = $(OBJ_CRAWLER) $(OBJ)/crawl_program.o

EXECS = webserver crawl_program

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

#test_crawler: $(OBJ_TEST_CRAWLER)
	#$(CC) -o $(BIN)/$@ $(LFLAGS) $(OBJ_TEST_CRAWLER)

crawl_program: $(OBJ_CRAWL_PROGRAM)
	$(CC) -o $(BIN)/$@ $(LFLAGS) $(OBJ_CRAWL_PROGRAM)

webserver: $(SERVEROBJS)
	$(CC) -o $(BIN)/$@ $(SRC)/readln.c $(LFLAGS) $(SERVEROBJS)

.PHONY: clean
clean:
	/bin/rm -rf $(BIN) $(OBJ) core* *~

cleanswap:
	/bin/rm -f \.*swp */\.*swp
	/bin/rm -f /$(SRC)/\.*swp
	/bin/rm -f /$(INC)/\.*swp


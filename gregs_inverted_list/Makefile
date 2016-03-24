CC = gcc
CFLAGS = -std=c99 -g -Wall
EXECS = test_site_node test_site_list test_word_node test_word_list test_word_list_s test_inv_list

BIN = bin

all: mkdirs $(EXECS)

mkdirs:
	mkdir -p $(BIN)

site_node.o: site_node.h site_node.c
	$(CC) $(CFLAGS) -c site_node.c

site_list.o: site_list.h site_list.c
	$(CC) $(CFLAGS) -c site_list.c

word_node.o: word_node.h word_node.c
	$(CC) $(CFLAGS) -c word_node.c

word_list.o: word_list.h word_list.c
	$(CC) $(CFLAGS) -c word_list.c

inv_list.o: inv_list.h inv_list.c
	$(CC) $(CFLAGS) -c inv_list.c

test_site_node: site_node.o test_site_node.c
	$(CC) $(CFLAGS) site_node.o test_site_node.c -o $(BIN)/test_site_node

test_site_list: site_list.o site_node.o test_site_list.c
	$(CC) $(CFLAGS) site_node.o site_list.o test_site_list.c -o $(BIN)/test_site_list

test_word_node: site_list.o site_node.o word_node.o test_word_node.c
	$(CC) $(CFLAGS) site_node.o site_list.o word_node.o test_word_node.c -o $(BIN)/test_word_node

test_word_list: word_list.o word_node.o site_list.o site_node.o test_word_list.c
	$(CC) $(CFLAGS) word_list.o word_node.o site_list.o site_node.o test_word_list.c -o $(BIN)/test_word_list

test_word_list_s: word_list.o word_node.o site_list.o site_node.o test_word_list_s.c
	$(CC) $(CFLAGS) word_list.o word_node.o site_list.o site_node.o test_word_list_s.c -o $(BIN)/test_word_list_s

test_inv_list: inv_list.o word_list.o word_node.o site_list.o site_node.o test_inv_list.c
	$(CC) $(CFLAGS) inv_list.o word_list.o word_node.o site_list.o site_node.o test_inv_list.c -o $(BIN)/test_inv_list

clean:
	/bin/rm -rf *.o core* *~
	/bin/rm -rf $(EXECS)
	/bin/rm -rf $(BIN)

cleanswap:
	/bin/rm -f \.*swp */\.*swp

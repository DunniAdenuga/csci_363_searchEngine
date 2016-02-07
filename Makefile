CC = gcc
CRAWLEROBJS = crawler.o tcplib.o test_crawler.o
PAGEOBJS = crawler.o tcplib.o pages.o

OBJS = $(CRAWLEROBJS) $(PAGEOBJS)
CFLAGS = -Wall -g
LFLAGS = 

EXECS = test_crawler pages

all: $(EXECS)

%.o: &.c
	$(CC) -c $(CFLAGS) $<

test_crawler: $(CRAWLEROBJS)
	$(CC) -o $@ $(LFLAGS) $(CRAWLEROBJS)

pages: $(PAGEOBJS)
	$(CC) -o $@ $(LGLAGS) $(PAGEOBJS)

clean:
	/bin/rm -f $(OBJS) $(EXECS) core* *~

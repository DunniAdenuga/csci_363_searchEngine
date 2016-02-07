CC = gcc
CRAWLEROBJS = crawler.o tcplib.o pages.o

OBJS = $(CRAWLEROBJS)
CFLAGS = -Wall -g
LFLAGS = 

EXECS = crawler pages

all: $(EXECS)

%.o: &.c
	$(CC) -c $(CFLAGS) $<

crawler: $(CRAWLEROBJS)
	$(CC) -o $@ $(LFLAGS) $(CRAWLEROBJS)

pages: $(CRAWLEROBJS)
	$(CC) $(CFLAGS) -o $@ $(CRAWLEROBJS)

clean:
	/bin/rm -f $(OBJS) $(EXECS) core* *~

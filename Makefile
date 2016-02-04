CC = gcc
CRAWLEROBJS = crawler.o tcplib.o

OBJS = $(CRAWLEROBJS)
CFLAGS = -Wall -g
LFLAGS = 

EXECS = crawler

all: $(EXECS)

%.o: &.c
	$(CC) -c $(CFLAGS) $<

crawler: $(CRAWLEROBJS)
	$(CC) -o $@ $(LFLAGS) $(CRAWLEROBJS)

clean:
	/bin/rm -f $(OBJS) $(EXECS) core* *~


CC ?= gcc
CFLAGS ?= -std=c99 -Wall -pedantic-errors


.PHONY: clean all

all: guanodump guanowrite

clean:
	rm -f *.o guanodump
	rm -f ./*~

guanodump: guano.o
	$(CC) $(CFLAGS) -o guanodump guanodump.c guano.o

guanowrite: guano.o guanowrite.o
	$(CC) $(CFLAGS) -o guanowrite guanowrite.o guano.o

%.o: %.c
	$(CC) -c $(CFLAGS) $<

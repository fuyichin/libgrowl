CC = gcc
CFLAGS = -Wall -ansi -g
CFLAGS += -I../src

all: growlnotify

libgrowl.o: ../src/libgrowl.c
	$(CC) -c ../src/libgrowl.c -o growlnotify.o -I../src

growlnotify: growlnotify.o
	$(CC) -o growlnotify growlnotify.o -I../src -L../src -lgrowl

clean:
	rm -f growlnotify
	rm -f *.o

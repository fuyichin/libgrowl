CC = gcc
CFLAGS = -Wall -ansi -g
CFLAGS += -I../src

all: growlnotify

libgrowl.o: ../src/libgrowl.c
	$(CC) -c ../src/libgrowl.c -I../src

growlnotify: growlnotify.o libgrowl.o
#	$(CC) -o growlnotify growlnotify.o -I../src -L../src -lgrowl
	$(CC) -o growlnotify growlnotify.o libgrowl.o

clean:
	rm -f growlnotify
	rm -f *.o

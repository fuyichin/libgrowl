CC = gcc
CFLAGS = -Wall -ansi -g

all: libgrowl.o
	ar -r libgrowl.a libgrowl.o

libgrowl: libgrowl.o
	$(CC) -c -o libgrowl.o libgrowl.c


clean:
	rm -f libgrowl.o libgrowl.a
CC = gcc
CFLAGS += -Wall -g
CFLAGS += -I../src

all: growlnotify GrowlServerHack

libgrowl.o: ../src/libgrowl.c
	$(CC) $(CFLAGS) -c ../src/libgrowl.c -I../src

growlnotify: growlnotify.o libgrowl.o
#	$(CC) -o growlnotify growlnotify.o -I../src -L../src -lgrowl
	$(CC) -o growlnotify growlnotify.o libgrowl.o

serverhack.o: serverhack.c
	$(CC) $(CFLAGS) -c serverhack.c `pkg-config --cflags libnotify`

GrowlServerHack: serverhack.o libgrowl.o
	$(CC) -o GrowlServerHack serverhack.o libgrowl.o `pkg-config --libs libnotify`

clean:
	rm -f growlnotify
	rm -f *.o

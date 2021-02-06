CC = gcc
CFLAGS = -Wall
CFLAGS += -I../src
CMOCKA_LIB_PATH=/usr/local/lib
#program=./gntp_decode_reply_test
program=gntp_decode_reply_test

all: $(program)

libgrowl.o: ../src/libgrowl.c
	$(CC) -c ../src/libgrowl.c -I../src

gntp_decode_reply_test: gntp_decode_reply_test.o libgrowl.o
	$(CC) -o gntp_decode_reply_test gntp_decode_reply_test.o libgrowl.o -L$(CMOCKA_LIB_PATH) -lcmocka

test: $(program)
	# run unit test
	./run_unit_test.sh $(program)

clean:
	rm -f $(program)
	rm -f *.o

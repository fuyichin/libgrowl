CC = gcc
CFLAGS = -Wall
CFLAGS += -I../src
CMOCKA_LIB_PATH=/usr/local/lib
#program=./gntp_decode_reply_test
program=gntp_decode_reply_test \
gntp_get_header_value_test \
check_default_setting_test \
libgrowl_minimalist_mode_test \
libgrowl_unix_get_system_name_test \
libgrowl_set_growl_server_test \
bsd_get_platform_name_test \

all: $(program)

libgrowl.o: ../src/libgrowl.c
	$(CC) $(CFLAGS) -c ../src/libgrowl.c -I../src

gntp_decode_reply_test: gntp_decode_reply_test.o libgrowl.o
	$(CC) -o gntp_decode_reply_test gntp_decode_reply_test.o libgrowl.o -L$(CMOCKA_LIB_PATH) -lcmocka

check_default_setting_test: check_default_setting_test.o libgrowl.o
	$(CC) -o check_default_setting_test check_default_setting_test.o libgrowl.o -L$(CMOCKA_LIB_PATH) -lcmocka

gntp_get_header_value_test: gntp_get_header_value_test.o libgrowl.o
	$(CC) -o gntp_get_header_value_test gntp_get_header_value_test.o libgrowl.o -L$(CMOCKA_LIB_PATH) -lcmocka

libgrowl_minimalist_mode_test: libgrowl_minimalist_mode_test.o libgrowl.o
	$(CC) -o libgrowl_minimalist_mode_test libgrowl_minimalist_mode_test.o libgrowl.o -L$(CMOCKA_LIB_PATH) -lcmocka

libgrowl_unix_get_system_name_test: libgrowl_unix_get_system_name_test.o libgrowl.o
	$(CC) -o libgrowl_unix_get_system_name_test libgrowl_unix_get_system_name_test.o libgrowl.o -L$(CMOCKA_LIB_PATH) -lcmocka

libgrowl_set_growl_server_test: libgrowl_set_growl_server_test.o libgrowl.o
	$(CC) -o libgrowl_set_growl_server_test libgrowl_set_growl_server_test.o libgrowl.o -L$(CMOCKA_LIB_PATH) -lcmocka

bsd_get_platform_name_test: bsd_get_platform_name_test.o libgrowl.o
	$(CC) -o bsd_get_platform_name_test bsd_get_platform_name_test.o libgrowl.o -L$(CMOCKA_LIB_PATH) -lcmocka

check: $(program)
# run unit test
	@sh ./run_unit_test.sh "$(program)"

test: $(program)
# run unit test
	@sh ./run_unit_test.sh "$(program)"

clean:
	rm -f $(program)
	rm -f *.o

AM_CPPFLAGS = -I$(top_srcdir)/src

program=gntp_decode_reply_test \
check_default_setting_test \
gntp_get_header_value_test \
libgrowl_minimalist_mode_test \
libgrowl_set_growl_server_test \
bsd_get_platform_name_test

check_PROGRAMS=$(program)
### this will execute the test
TESTS=$(program)

gntp_decode_reply_test_CFLAGS=$(CMOCKA_CFLAGS)
gntp_decode_reply_test_LDADD=$(CMOCKA_LIBS)
gntp_decode_reply_test_SOURCES=gntp_decode_reply_test.c ../src/libgrowl.c

check_default_setting_test_CFLAGS=$(CMOCKA_CFLAGS)
check_default_setting_test_LDADD=$(CMOCKA_LIBS)
check_default_setting_test_SOURCES=check_default_setting_test.c ../src/libgrowl.c

gntp_get_header_value_test_test_CFLAGS=$(CMOCKA_CFLAGS)
gntp_get_header_value_test_LDADD=$(CMOCKA_LIBS)
gntp_get_header_value_test_SOURCES=gntp_get_header_value_test.c ../src/libgrowl.c

libgrowl_minimalist_mode_test_CFLAGS=$(CMOCKA_CFLAGS)
libgrowl_minimalist_mode_test_LDADD=$(CMOCKA_LIBS)
libgrowl_minimalist_mode_test_SOURCES=libgrowl_minimalist_mode_test.c ../src/libgrowl.c

libgrowl_set_growl_server_test_CFLAGS=$(CMOCKA_CFLAGS)
libgrowl_set_growl_server_test_LDADD=$(CMOCKA_LIBS)
libgrowl_set_growl_server_test_SOURCES=libgrowl_set_growl_server_test.c ../src/libgrowl.c

bsd_get_platform_name_test_CFLAGS=$(CMOCKA_CFLAGS)
bsd_get_platform_name_test_LDADD=$(CMOCKA_LIBS)
bsd_get_platform_name_test_SOURCES=bsd_get_platform_name_test.c ../src/libgrowl.c

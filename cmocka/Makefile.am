AM_CPPFLAGS = -I$(top_srcdir)/src
check_PROGRAMS = bsd_get_platform_name_test gntp_decode_reply_test \
	libgrowl_minimalist_mode_test check_default_setting_test \
	gntp_get_header_value_test libgrowl_set_growl_server_test
TESTS = bsd_get_platform_name_test gntp_decode_reply_test \
	libgrowl_minimalist_mode_test check_default_setting_test \
	gntp_get_header_value_test libgrowl_set_growl_server_test

# this execute the test results, can replace with unit_test.sh?? No!

bsd_get_platform_name_test_CFLAGS =  -I$(top_srcdir)/src
bsd_get_platform_name_test_LDFLAGS = -lcmocka
gntp_decode_reply_test_CFLAGS =  -I$(top_srcdir)/src
gntp_decode_reply_test_LDFLAGS = -lcmocka
libgrowl_minimalist_mode_test_CFLAGS =  -I$(top_srcdir)/src
libgrowl_minimalist_mode_test_LDFLAGS = -lcmocka
check_default_setting_test_CFLAGS =  -I$(top_srcdir)/src
check_default_setting_test_LDFLAGS = -lcmocka
gntp_get_header_value_test_CFLAGS =  -I$(top_srcdir)/src
gntp_get_header_value_test_LDFLAGS = -lcmocka
libgrowl_set_growl_server_test_CFLAGS =  -I$(top_srcdir)/src
libgrowl_set_growl_server_test_LDFLAGS = -lcmocka

bsd_get_platform_name_test_SOURCES = bsd_get_platform_name_test.c ../src/libgrowl.c
gntp_decode_reply_test_SOURCES = gntp_decode_reply_test.c ../src/libgrowl.c
libgrowl_minimalist_mode_test_SOURCES = libgrowl_minimalist_mode_test.c ../src/libgrowl.c
check_default_setting_test_SOURCES = check_default_setting_test.c ../src/libgrowl.c
gntp_get_header_value_test_SOURCES = gntp_get_header_value_test.c ../src/libgrowl.c
libgrowl_set_growl_server_test_SOURCES = libgrowl_set_growl_server_test.c ../src/libgrowl.c

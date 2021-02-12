/**
 * libgrowl_set_growl_server_test.c
 * @author hooichee<fuyichin@gmail.com>
 *
 *  Created by Chin Hooi Chee on 2/9/21.
 *  Copyright © 2021 Chin Hooi Chee. All rights reserved.
 */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmocka.h"
#include <stdio.h>
#include <string.h>
#include "libgrowl.h"

char *libgrowl_get_host(void);
int libgrowl_get_port(void);
void libgrowl_set_growl_server(char *name);

static void test_set_growl_server_host_ip_1(void **state) {

	(void) state; /* unused */

	libgrowl_set_growl_server("127.0.0.2:23052");
	assert_int_equal(23052,libgrowl_get_port());
	assert_true(strcmp("127.0.0.2",libgrowl_get_host())==0);
}

static void test_set_growl_server_host_ip_2(void **state) {

	(void) state; /* unused */

	libgrowl_set_growl_server("127.0.0.1:23053"); /* set to default */
	libgrowl_set_growl_server("192.168.56.101:");
	assert_int_equal(23053,libgrowl_get_port());
	assert_true(strcmp("192.168.56.101",libgrowl_get_host())==0);
}

static void test_set_growl_server_host_ip_3(void **state) {

	(void) state; /* unused */

	libgrowl_set_growl_server("127.0.0.1:23053"); /* set to default */
	libgrowl_set_growl_server("192.168.56.102");
	assert_int_equal(23053,libgrowl_get_port());
	assert_true(strcmp("192.168.56.102",libgrowl_get_host())==0);
}

static void test_set_growl_server_host_ip_4(void **state) {

	(void) state; /* unused */

	libgrowl_set_growl_server("127.0.0.1:23053"); /* set to default */
	libgrowl_set_growl_server("localhost:23052");
	assert_int_equal(23052,libgrowl_get_port());
	assert_true(strcmp("127.0.0.1",libgrowl_get_host())==0);
}

static void test_set_growl_server_host_ip_5(void **state) {

	(void) state; /* unused */

	libgrowl_set_growl_server("127.0.0.1:23053"); /* set to default */
	libgrowl_set_growl_server("localhost");
	assert_int_equal(23053,libgrowl_get_port());
	assert_true(strcmp("127.0.0.1",libgrowl_get_host())==0);
}

int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_set_growl_server_host_ip_1),
		cmocka_unit_test(test_set_growl_server_host_ip_2),
		cmocka_unit_test(test_set_growl_server_host_ip_3),
		cmocka_unit_test(test_set_growl_server_host_ip_4),
		cmocka_unit_test(test_set_growl_server_host_ip_5),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}


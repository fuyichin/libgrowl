/**
 * param_test.c
 * @author hooichee<fuyichin@gmail.com>
 *
 *  Created by Chin Hooi Chee on 3/2/21.
 *  Copyright © 2021 Chin Hooi Chee. All rights reserved.
 */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmocka.h"
#include <stdio.h>
#include <string.h>
#include "param.h"

static void test_get_param_verbose_level0(void **state) {
	char * args[]= {"test_verbose"};

	(void) state; /* unused */

	assert_int_equal(0, get_param_verbose_level(args, 1));
}

static void test_get_param_verbose_level1(void **state) {
	char * args[]= {"test_verbose", "-v"};

	(void) state; /* unused */

	assert_int_equal(1, get_param_verbose_level(args, 2));
}

static void test_get_param_verbose_level1a(void **state) {
	char * args[]= {"test_verbose", "-m"};

	(void) state; /* unused */

	assert_int_equal(0, get_param_verbose_level(args, 2));
}

static void test_get_param_verbose_level2(void **state) {
	char * args[]= {"test_verbose", "-vv"};

	(void) state; /* unused */

	assert_int_equal(2, get_param_verbose_level(args, 2));
}

static void test_get_param_mode_console_1(void **state) {
	char *args[]= {"GrwolServerHack" };
	int argc= 1;
	(void) state; /* unused */

	assert_int_equal(0,get_param_mode(args,argc,"--console"));
}

static void test_get_param_mode_console_2(void **state) {
	char *args[]= {"GrwolServerHack", "--console" };
	int argc= 2;
	(void) state; /* unused */

	assert_int_equal(1,get_param_mode(args,argc,"--console"));
}

static void test_get_param_mode_console_3(void **state) {
	char *args[]= {"GrwolServerHack", "--console", "off" };
	int argc= 3;
	(void) state; /* unused */

	assert_int_equal(0,get_param_mode(args,argc,"--console"));
}

int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_get_param_verbose_level0),
		cmocka_unit_test(test_get_param_verbose_level1),
		cmocka_unit_test(test_get_param_verbose_level1a),
		cmocka_unit_test(test_get_param_verbose_level2),
		cmocka_unit_test(test_get_param_mode_console_1),
		cmocka_unit_test(test_get_param_mode_console_2),
		cmocka_unit_test(test_get_param_mode_console_3),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}

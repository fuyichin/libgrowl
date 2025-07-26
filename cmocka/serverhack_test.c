/**
 * serverhack_test.c
 * @author hooichee<fuyichin@gmail.com>
 *
 *  Created by Chin Hooi Chee on 26/7/2025.
 *  Modified by Chin Hooi Chee on 26/7/2025
 *  Copyright © 2025 Chin Hooi Chee. All rights reserved.
 */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmocka.h"
#include <stdio.h>
//#include "libgrowl.h"
//#include "mygntp.h"

int get_param_mode(char *args[], int argc, char *mode);

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
		cmocka_unit_test(test_get_param_mode_console_1),
		cmocka_unit_test(test_get_param_mode_console_2),
		cmocka_unit_test(test_get_param_mode_console_3),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}


/**
 * check_default_setting_test.c
 * @author hooichee<fuyichin@gmail.com>
 *
 *  Created by Chin Hooi Chee on 2/7/21.
 *  Copyright © 2021 Chin Hooi Chee. All rights reserved.
 */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmocka.h"
#include <stdio.h>
#include <string.h>
#include "libgrowl.h"

int libgrowl_get_debug_level(void);
char *libgrowl_get_host(void);
int check_default_setting(void);

static void test_check_default_setting(void **state) {

	(void) state; /* unused */

	assert_int_equal(1,libgrowl_get_debug_level());
	assert_true(strcmp("127.0.0.1",libgrowl_get_host())==0);
}

int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_check_default_setting),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}


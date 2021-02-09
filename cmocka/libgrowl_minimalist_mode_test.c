/**
 * libgrowl_monimalist_mode_test.c
 * @author hooichee<fuyichin@gmail.com>
 *
 *  Created by Chin Hooi Chee on 2/8/21.
 *  Copyright © 2021 Chin Hooi Chee. All rights reserved.
 */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmocka.h"
#include <stdio.h>
#include "libgrowl.h"

void libgrowl_set_minimalist_mode(int);
int libgrowl_get_minimalist_mode(void);

static void test_true_and_false(void **state) {

	(void) state; /* unused */

	assert_false(0);
	assert_true(!0);
}

static void test_set_minimalist_mode(void **state) {

	(void) state; /* unused */

	libgrowl_set_minimalist_mode(0); /* false */
	assert_false(libgrowl_get_minimalist_mode());
	libgrowl_set_minimalist_mode(!0); /* true */
	assert_true(libgrowl_get_minimalist_mode());
}

int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_true_and_false),
		cmocka_unit_test(test_set_minimalist_mode),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}


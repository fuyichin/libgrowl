/**
 * libgrowl_unix_get_system_name_test.c
 * @author hooichee<fuyichin@gmail.com>
 *
 *  Created by Chin Hooi Chee on 6/7/25.
 *  Copyright © 2021 Chin Hooi Chee. All rights reserved.
 */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmocka.h"
#include <stdio.h>
#include "libgrowl.h"

void unix_get_system_name(char *, int);

static void test_string_overflow(void **state) {
#ifdef _LINUX
	char system_name[5+1];
#else
	char system_name[80];
#endif
	(void) state; /* unused */

#ifndef _WIN32
	unix_get_system_name(system_name, sizeof(system_name)-1);
	printf("name=[%s]\n", system_name);
#ifdef _LINUX
	assert_true(strcmp("Linux",system_name)==0);
#endif
#else
	/* not going to test for Windows */
	assert_false(0);
#endif
}

int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_string_overflow),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}


/**
 * bsd_get_platform_name_test.c
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
#include "libgrowl.h"

void bsd_get_platform_name_version(char *, char *);
void libgrowl_set_debug_level(int);

static void test_bsd_get_platform_name_version(void **state) {
	char platform_name[80], platform_version[80];

	(void) state; /* unused */

	libgrowl_set_debug_level(0);
	platform_name[0]= '\0'; platform_version[0]= '\0';
	bsd_get_platform_name_version(platform_name,platform_version);
#ifdef _LINUX
	assert_true(strcmp("Linux",platform_name)==0);
#else
#ifdef _Darwin
	assert_true(strcmp("macOS",platform_name)==0 || strcmp("Mac OS X",platform_name)==0);
#else
	assert_true(!0);
#endif
#endif
}

int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_bsd_get_platform_name_version),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}

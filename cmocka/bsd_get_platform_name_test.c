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

static void test_bsd_get_platform_name_version(void **state) {
	char platform_name[80], platform_version[80];

	(void) state; /* unused */

	platform_name[0]= '\0'; platform_version[0]= '\0';
	bsd_get_platform_name_version(platform_name,platform_version);
#ifdef _LINUX
	assert_true(strcmp("Linux",platform_name)==0);
#else
#ifdef _macOS
//	printf("platform name=[%s]\n", platform_name);
	assert_true(strcmp("macOS",platform_name)==0);
#else
	assert_true(strcmp("Mac OS X",platform_name)==0);
#endif  /* _macOS */
#endif
}

int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_bsd_get_platform_name_version),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}

/**
 * gntp_get_value_header_test.c
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

char *gntp_get_header_value(char *value, int size, char *header_code, char *message);

static void test_gntp_get_header_value_reply_notify_ok(void **state) {
	char msg_reply[]="\
GNTP/1.0 -OK NONE\r\n\
Response-Action: NOTIFY\r\n\
Origin-Software-Name: Growl\r\n\
Origin-Software-Version: 2.1.3\r\n\
Origin-Platform-Version: 10.13.6\r\n\
Origin-Machine-Name: purplesnow\r\n\
Origin-Platform-Name: Mac OS X\r\n\
\r\n";
	char *header_codes[]= {
		"Response-Action",
		"Origin-Software-Name",
		"Origin-Machine-Name"
	};
	char *header_values[]= {
		"NOTIFY",
		"Growl",
		"purplesnow"
	};
	char value[80+1];
	int i;

	(void) state; /* unused */

	for (i=0; i<3; i++) {
//fprintf(stdout, "**checking header code [%s]\n", header_codes[i]);		
		gntp_get_header_value(value, sizeof(value),header_codes[i],msg_reply);
		assert_int_equal(0,strcmp(value,header_values[i]));
		}
}

static void test_gntp_get_header_value_reply_303(void **state) {
	char msg_reply[]="\
GNTP/1.0 -ERROR NONE\r\n\
Error-Code: 303\r\n\
Error-Description: Required header missing (Notification-Title)\r\n\
Origin-Machine-Name: BLUE-PC\r\n\
Origin-Software-Name: Growl/Win\r\n\
Origin-Software-Version: 2.0.9.1\r\n\
Origin-Platform-Name: Microsoft Windows NT 6.1.7601 Service Pack 1\r\n\
Origin-Platform-Version: 6.1.7601.65536\r\n\
X-Message-Daemon: Growl/Win\r\n\
X-Timestamp: 2/6/2021 10:34:23 PM\r\n\
\r\n";
	char *header_codes[]= {
		"Error-Code",
		"Origin-Machine-Name",
		"X-Timestamp"
	};
	char *header_values[]= {
		"303",
		"BLUE-PC",
		"2/6/2021 10:34:23 PM"
	};
	char value[80+1];
	int i;

	(void) state; /* unused */

	for (i=0; i<3; i++) {
		gntp_get_header_value(value, sizeof(value),header_codes[i],msg_reply);
		assert_int_equal(0,strcmp(value,header_values[i]));
		}
}

int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_gntp_get_header_value_reply_notify_ok),
		cmocka_unit_test(test_gntp_get_header_value_reply_303),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}


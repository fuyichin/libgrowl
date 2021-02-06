/**
 * gntp_decode_reply_test.c
 * @author hooichee<fuyichin@gmail.com>
 *
 *  Created by Chin Hooi Chee on 2/6/21.
 *  Copyright © 2021 Chin Hooi Chee. All rights reserved.
 */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmocka.h"
#include <stdio.h>
#include "libgrowl.h"

/* this structure define twice in here and libgrowl.c */
struct gntp_message {
	int status;  /* 0-ok */
};

#define GNTP_OK  0
#define GNTP_FAIL  -1

void gntp_decode_reply(char *, struct gntp_message *);

static void test_gntp_decode_reply_ok(void **state) {
	char msg_reply[]="\
GNTP/1.0 -OK NONE\r\n\
Response-Action: REGISTER\r\n\
Origin-Software-Name: Growl\r\n\
Origin-Software-Version: 2.1.3\r\n\
Origin-Platform-Version: 10.13.6\r\n\
Origin-Machine-Name: purplesnow\r\n\
Origin-Platform-Name: Mac OS X\r\n\
\r\n";
	struct gntp_message msg;

	(void) state; /* unused */

	gntp_decode_reply(msg_reply, &msg);
	assert_int_equal(GNTP_OK,msg.status);

}

static void test_gntp_decode_reply_fail(void **state) {
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
	struct gntp_message msg;

	(void) state; /* unused */

	gntp_decode_reply(msg_reply, &msg);
	assert_int_equal(GNTP_FAIL,msg.status);

}

int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_gntp_decode_reply_ok),
		cmocka_unit_test(test_gntp_decode_reply_fail),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}


/**
 * gntp_decode_message_test.c
 * @author hooichee<fuyichin@gmail.com>
 *
 *  Created by Chin Hooi Chee on 2/6/21.
 *  Modified by Chin Hooi Chee on 6/10/2025
 *  Copyright © 2021 Chin Hooi Chee. All rights reserved.
 */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmocka.h"
#include <stdio.h>
#include "libgrowl.h"
#include "mygntp.h"

static void test_gntp_decode_message_ok(void **state) {
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

	gntp_decode_message(msg_reply, &msg);
	assert_int_equal(GNTP_OK,msg.type);

}

static void test_gntp_decode_message_fail(void **state) {
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

	gntp_decode_message(msg_reply, &msg);
	assert_int_equal(GNTP_FAIL,msg.type);

}

static void test_gntp_decode_message_register(void **state) {
	char msg_reply[]="\
GNTP/1.0 REGISTER NONE\r\n\
Origin-Machine-Name: purplesnow.local\r\n\
Origin-Software-Name: libgrowl\r\n\
Origin-Software-Version: 0.1\r\n\
Origin-Platform-Name: Mac OS X\r\n\
Origin-Platform-Version: 10.13.6\r\n\
Application-Name: libgrowl\r\n\
Notifications-Count: 1\r\n\
\
Notification-Name: libgrowl\r\n\
Notification-Display-Name: libgrowl\r\n\
Notification-Enabled: True\r\n\
\r\n";
	struct gntp_message msg;

	(void) state; /* unused */

	gntp_decode_message(msg_reply, &msg);
	assert_int_equal(GNTP_REGISTER,msg.type);

}

static void test_gntp_decode_message_notify(void **state) {
	char msg_reply[]="\
GNTP/1.0 NOTIFY NONE\r\n\
Origin-Machine-Name: purplesnow.local\r\n\
Origin-Software-Name: libgrowl\r\n\
Origin-Software-Version: 0.1\r\n\
Origin-Platform-Name: Mac OS X\r\n\
Origin-Platform-Version: 10.13.6\r\n\
Application-Name: libgrowl\r\n\
Notification-Name: libgrowl\r\n\
Notification-Title: libgrowl\r\n\
Notification-Text: Hello, libgrowl.\r\n\
Notification-Sticky: False\r\n\
Notification-Priority: 0\r\n\
\r\n";
	struct gntp_message msg;

	(void) state; /* unused */

	gntp_decode_message(msg_reply, &msg);
	assert_int_equal(GNTP_NOTIFY,msg.type);

}

int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_gntp_decode_message_ok),
		cmocka_unit_test(test_gntp_decode_message_fail),
		cmocka_unit_test(test_gntp_decode_message_register),
		cmocka_unit_test(test_gntp_decode_message_notify),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}


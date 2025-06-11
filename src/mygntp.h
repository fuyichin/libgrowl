/**
 * mygntp.h
 * @author hooichee<fuyichin@gmail.com>
 *
 *  Created by Chin Hooi Chee on 6/10/2025.
 *  Modified by Chin Hooi Chee on 6/10/2025
 *  Copyright © 2021 Chin Hooi Chee. All rights reserved.
 */

struct gntp_message {
	int type;  /* 0-ok */
};

#define GNTP_OK  0
#define GNTP_FAIL  -1
#define GNTP_REGISTER  1
#define GNTP_NOTIFY  2

void gntp_decode_message(char *, struct gntp_message *);

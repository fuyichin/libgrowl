/* //
 //  grownotify.c
 //  libgrowl
 //
 //  Created by Chin Hooi Chee on 2/5/21.
 //  Copyright © 2021 Chin Hooi Chee. All rights reserved.
 // */
#include <stdio.h>
#include <string.h>
#include "libgrowl.h"

int main(int argc, char *argv[])
{
	char *application= "growlnotify";
	char *notification= "libgrowl";
	char **notifications;
	int status;
	char *title, *text, *port_str;
	
	/* process parameters */
	libgrowl_set_debug(0);
	title= "libgrowl"; /* title is required */
	text= "Hello, libgrowl"; /* message is optional */
	if (argc==1) {
		/* no parameter */
	}
	else {
		int i= 0, counter= 0;
		for (i=1; i<argc; i++) {
			if (strcmp(argv[i],"-v")==0) {
				libgrowl_set_debug(1);
				continue;
			}
			else if (strcmp(argv[i],"-vv")==0) {
				libgrowl_set_debug(2);
				continue;
			}
			else if (argv[i][0]=='-')
				continue;
			
			counter++;
			switch(counter) {
				case 1:
					printf("case 1:i=%d\n", i);
					/* puts(argv[1]); */
					title= argv[i];
					text= "";
					break;
				case 2:
					text= argv[i];
					break;
				case 3:
					port_str= argv[i];
					break;
				case 4:
					application= argv[i];
					break;
				case 5:
					notification= argv[i];
					break;
				default:
					break;
					
			}
			
		}
	}
	
	notifications= (char *[]){notification, "\0"};
	status= growl_register_notifications(application, notifications);
	if (status!=!0) {
		fprintf(stderr, "register fail.\n");
		return -1;
	}
	
	status= growl_send_notification(application, notifications[0],title, text);
	if (status!=!0) {
		fprintf(stderr, "send notification fail.\n");
		return -2;
	}
	
	return 0;
}

/**
 * libgrowl.c
 * @author hooichee<fuyichin@gmail.com>
 *
 *  Created by Chin Hooi Chee on 2/5/21.
 *  Copyright © 2021 Chin Hooi Chee. All rights reserved.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/utsname.h>   /* Header for 'uname'  */
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h> /* inet_addr */
#include "libgrowl.h"

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX  80
#endif

#define GNTP_OK  0
#define GNTP_FAIL  -1

#ifndef min
#define min(a,b) (a<b)?(a):(b)
#endif

struct gntp_message {
	int status;  /* 0-ok */
};

static int debug_level= 2;  /* 0-no debug */
static char *g_host= "127.0.0.1"; /* localhost */
static int g_port= GROWL_DEFAULT_PORT; /* GROWL_DEFAULT_PORT */

static char *g_platform_name= NULL;

void libgrowl_set_debug(int n)
{
	debug_level= n;
}

/* this infomration retrieve information from unix kernet
 name return Darwin not Mac OS
 */
void unix_get_system_name(char *name, int len)
{
	int n;
	struct utsname uname_pointer;
	uname(&uname_pointer);
	n= min(len,(int)strlen(uname_pointer.sysname));
	strncpy(name, uname_pointer.sysname, n);
	name[n]= '\0';
}

int get_string_array_size(char **name)
{
	int i= 1;
	char *str1= name[0];
	while (str1[0]!='\0')
		str1= *(name+i++); /* name[i]; i++; */
	return i-1;
}

/**
 *  reply. end with \0
 */
void gntp_decode_reply(char *reply, struct gntp_message *msg)
{
	const char s[]= "\n";
	char *token;
	
	msg->status= GNTP_FAIL;
	token= strtok(reply, s);
	while (token!=NULL) {
		if (strncmp(token,"GNTP/",4)==0) {
			/* check for -OK */
			if (strstr(token,"-OK")!=NULL)
				msg->status= GNTP_OK;
			break;
		}
		
		token= strtok(NULL, s);
	}
	
}

/* function start with gntp used by internal */
/**
 *  data: gntp message send out, return message can be store here
 *  d_size: size of data buffer, not content, Content size can be obtain from strlen(d_size)
 *
 *  return 0-sucess
 */
int gntp_send(char *data, int d_size, char *host, int port, char *reply, int reply_len)
{
	int data_len; /* send or recieve */
	int sockfd;
	socklen_t to_len;
	struct sockaddr_in addr;
	
	struct gntp_message gntp_reply;
	char server_reply[2000];
	int return_status= 0; /* 0-sucess */
	
	/* create socket */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd==-1) {
		fprintf(stderr, "socket creation failed....\n");
		return 1;
	}
	memset(&addr, 0, sizeof(addr));  /* clear addr */
	
	addr.sin_family = AF_INET;                  /* host byte order */
	addr.sin_port = htons(port);                /* short, network byte order */
	addr.sin_addr.s_addr = inet_addr(host);
	/* ???    memset(addr.sin_zero, 0, sizeof(addr.sin_zero)); */
	
	to_len = sizeof(struct sockaddr);
	
	if (debug_level>=1)
		fprintf(stdout, "connect to %s port %d\n", g_host, g_port);
	
	if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr))!=0) {
		fprintf(stderr, "connection to the server failed...\n");
		close(sockfd);
		return 2;
	}
	else {
		if (debug_level>=1)
			fprintf(stdout, "connected to the server %s:%d...\n", host, port);
	}
	/* actually send the packet */
	/*    bytes_sent= sendto(sockfd, data, d_size, 0, (struct sockaddr *) &addr, to_len);
	 write(sockfd, data, d_size);
	 bytes_sent= d_size;
	 */
	data_len= (int)send(sockfd, data, strlen(data), 0);
	
	if (data_len<0) {
		fprintf(stderr, "sending fail...\n");
		if (debug_level>=2) {
			fprintf(stdout, "[%s]\n", data);
			fprintf(stdout, "socket id=%d, d_size=%d, to_len=%d \n", sockfd, d_size, to_len);
		}
		/* need to close socket? */
		close(sockfd);
		return 3;
	}
	
	if (debug_level>=1)
		fprintf(stdout, "Data send (%d bytes)\n", data_len);
	
	memset(data,0,d_size);
	memset(server_reply, 0, sizeof(server_reply));
	/* receive reply from server */
	data_len= (int)recv(sockfd,server_reply,2000,0);
	if (data_len<0) {
		fprintf(stderr, "read reply fail (socket=%d)(%d)\n", sockfd, data_len);
		close(sockfd);
		return 4;
	}
	
	if (debug_level>=1)
		fprintf(stdout, "Data receive (%d bytes)\n", data_len);
	
	if (debug_level>=2) {
		fprintf(stdout, "[%s]\n", server_reply);
	}
	
	gntp_decode_reply(server_reply,&gntp_reply);
	if (gntp_reply.status==GNTP_OK) {
		if (debug_level>=1)
			fprintf(stdout, "gntp reply success.\n");
	}
	else {
		if (debug_level>=1)
			fprintf(stdout, "gntp reply fail.\n");
		return_status= -1;
	}
	close(sockfd);
	
	return return_status;
}

void gntp_format_register_message(char *msg, int msg_size, char *app, char **name)
{
	char *str= *name;
	printf("name=%s", str);
}

/**
 *  register growl notification(s)
 *  @author  <a href="mailto:fuyichin@gmail.com">hooichee</a>
 *  @param app  Application name eg. growlnotify
 *  @param notifications  notification names to be registered, always end with "\0" eg. {"libgrowl", "send_message", "\0"}
 *  @return 0-fail, !0-success
 */
int growl_register_notifications(char *app, char **notifications)
{
	char message[800]; /* package message */
	char text[800]; /* tmp buffer, use for return status as well */
	char hostname[HOST_NAME_MAX];
	char system_name[80];
	int notification_count= get_string_array_size(notifications);
	int i, status;
	
	/* print all notification name */
	if (debug_level>=2) {
		int i= 1;
		char *str1= notifications[0];
		printf("notification name:\n");
		while (str1[0]!='\0') {
			printf("%d:name=%s\n", i, str1);
			str1= *(notifications+i++); /* name[i]; i++; */
		}
	}
	
	/*    gntp_format_register_message(message, sizeof(message), app, name); */
	/* gntp, format register message */
	message[0]= '\0';
	sprintf(text, "%s %s %s%s", GNTP_VERSION, MESSAGETYPE_REGISTER, ENCRYPTION_NONE, LINE_BREAK);
	strcat(message, text);
	gethostname(hostname, HOST_NAME_MAX);
	if (debug_level>=2)
		printf("homename=%s\n", hostname);
	sprintf(text, "%s: %s%s", HEADER_ORIGIN_MACHINE_NAME, hostname, LINE_BREAK);
	strcat(message, text);
	sprintf(text, "%s: %s%s", HEADER_ORIGIN_SOFTWARE_NAME, app, LINE_BREAK);
	strcat(message, text);
	sprintf(text, "%s: %s%s", HEADER_ORIGIN_SOFTWARE_VERSION, LIBGROWL_VERSION, LINE_BREAK);
	strcat(message, text);
	
	unix_get_system_name(system_name,sizeof(system_name));
	if (g_platform_name!=NULL) {
		/* these 2 are optional */
		sprintf(text, "%s: %s%s", HEADER_ORIGIN_PLATFORM_NAME, "Mac OS X", LINE_BREAK);
		strcat(message, text);
		sprintf(text, "%s: %s%s", HEADER_ORIGIN_PLATFORM_VERSION, "10.13.6", LINE_BREAK);
		strcat(message, text);
	}
	sprintf(text, "%s: %s%s", HEADER_APPLICATION_NAME, app, LINE_BREAK);
	strcat(message, text);
	sprintf(text, "%s: %d%s%s", HEADER_NOTIFICATIONS_COUNT, notification_count,
			LINE_BREAK, LINE_BREAK);
	strcat(message, text);
	
	for (i=0; i<notification_count; i++) {
		sprintf(text, "%s: %s%s", HEADER_NOTIFICATION_NAME, notifications[i], LINE_BREAK);
		strcat(message, text);
		sprintf(text, "%s: %s%s", HEADER_NOTIFICATION_DISPLAY_NAME, notifications[i], LINE_BREAK);
		strcat(message, text);
		sprintf(text, "%s: %s%s%s", HEADER_NOTIFICATION_ENABLED, "True", LINE_BREAK,
				LINE_BREAK);
		strcat(message, text);
	}
	
	if (debug_level>=1) {
		fprintf(stdout, "debug_level=%d\n", debug_level);
		fprintf(stdout, "send register notification: [%s]\n", notifications[0]);
	}
	if (debug_level>=2) {
		fprintf(stdout, "msg:\n[%s]\n", message);
		fprintf(stdout, "len:%d\n", (int)strlen(message));
	}
	
	/* send package */
	status= gntp_send(message, (int)strlen(message), g_host, g_port, text, sizeof(text));
	if (status!=0) /* error */
		return 0;
	
	return !0;
}

/**
 *  send growl notification
 *  @author  <a href="mailto:fuyichin@gmail.com">hooichee</a>
 *  @param app  Application name eg. growlnotify
 *  @param notification  notification name (must registered earlier with the server) eg. libgrowl
 *  @param title required, can not be empty, some server may reject
 *  @param content message
 *  @return 0-fail, !0-success
 */
int growl_send_notification(char *app, char *notification, char *title, char *content)
{
	char message[800]; /* package message */
	char text[800]; /* tmp buffer, use for return status as well */
	char hostname[HOST_NAME_MAX];
	char system_name[80];
	int status;
	
	/* print all notification name */
	if (debug_level>=2) {
		printf("name=%s\n", notification);
	}
	
	/* gntp, format notification message */
	message[0]= '\0';
	sprintf(text, "%s %s %s%s", GNTP_VERSION, MESSAGETYPE_NOTIFY, ENCRYPTION_NONE, LINE_BREAK);
	strcat(message, text);
	gethostname(hostname, HOST_NAME_MAX);
	if (debug_level>=2)
		printf("homename=%s\n", hostname);
	sprintf(text, "%s: %s%s", HEADER_ORIGIN_MACHINE_NAME, hostname, LINE_BREAK);
	strcat(message, text);
	sprintf(text, "%s: %s%s", HEADER_ORIGIN_SOFTWARE_NAME, app, LINE_BREAK);
	strcat(message, text);
	sprintf(text, "%s: %s%s", HEADER_ORIGIN_SOFTWARE_VERSION, LIBGROWL_VERSION, LINE_BREAK);
	strcat(message, text);
	
	unix_get_system_name(system_name,sizeof(system_name)); /* this get back Darwin not macOS */
	if (g_platform_name!=NULL) {
		/* these 2 are optional */
		sprintf(text, "%s: %s%s", HEADER_ORIGIN_PLATFORM_NAME, "Mac OS X", LINE_BREAK);
		strcat(message, text);
		sprintf(text, "%s: %s%s", HEADER_ORIGIN_PLATFORM_VERSION, "10.13.6", LINE_BREAK);
		strcat(message, text);
	}
	sprintf(text, "%s: %s%s", HEADER_APPLICATION_NAME, app, LINE_BREAK);
	strcat(message, text);
	sprintf(text, "%s: %s%s", HEADER_NOTIFICATION_NAME, notification, LINE_BREAK);
	strcat(message, text);
	
	sprintf(text, "%s: %s%s", HEADER_NOTIFICATION_TITLE, title, LINE_BREAK);
	strcat(message, text);
	sprintf(text, "%s: %s%s", HEADER_NOTIFICATION_TEXT, content, LINE_BREAK);
	strcat(message, text);
	sprintf(text, "%s: %s%s", HEADER_NOTIFICATION_STICKY, "False", LINE_BREAK);
	strcat(message, text);
	sprintf(text, "%s: %s%s", HEADER_NOTIFICATION_PRIORITY, "0", LINE_BREAK);
	strcat(message, text);
	strcat(message, LINE_BREAK);  /* ending */
	
	if (debug_level>=1)
		fprintf(stdout, "send notification: [%s]\n", notification);
	if (debug_level>=2)
		printf("data:\n[%s]\n", message);
	
	/* send package */
	status= gntp_send(message, (int)strlen(message), g_host, g_port, text, sizeof(text));
	if (status!=0)
		return 0;
	
	return !0;
}


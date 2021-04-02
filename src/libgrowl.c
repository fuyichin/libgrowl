/**
 * @file libgrowl.c
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
#include <netdb.h> /* macos? */
#include "libgrowl.h"

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX  80
#endif

#define GNTP_OK  0
#define GNTP_FAIL  -1

#ifndef min
#define min(a,b) (a<b)?(a):(b)
#endif

#ifndef FALSE
#define FALSE  0
#define TRUE   !FALSE
#endif

#define MINIMALIST_GROWL  FALSE

struct gntp_message {
	int status;  /* 0-ok */
};

static int debug_level= 1;  /* 0-no debug, 1,2 simple info, 3-for developers */
static char *g_host= "127.0.0.1"; /* localhost:port */
static int g_port= GROWL_DEFAULT_PORT; /* GROWL_DEFAULT_PORT */
static char *g_host_name="", *g_host_ip= "127.0.0.1";
static int g_minimalist_mode= MINIMALIST_GROWL;
static char *g_platform_name= NULL;


void libgrowl_set_debug_level(int n)
{
	debug_level= n;
}

int libgrowl_get_debug_level(void)
{
	return debug_level;
}

/*
void libgrowl_set_host(char *name)
{
	if (strlen(name)>=sizeof(g_host));
} */

char *libgrowl_get_host(void)
{
	return g_host; /* can I do like this? */
}

char *libgrowl_get_host_ip(void) /* this one not used anymore? */
{
	return g_host_ip;
}

int libgrowl_get_port(void)
{
	return g_port;
}

void libgrowl_set_minimalist_mode(int mode)
{
	g_minimalist_mode= mode;
}

int libgrowl_get_minimalist_mode(void)
{
	return g_minimalist_mode;
}

/**
 *  set growl server name eg. 127.0.0.1, localhost:23053, 127.0.0.1:23053
 *  @author  <a href="mailto:fuyichin@gmail.com">hooichee</a>
 *  @param name  server host name, ip or ip:port
 */
void libgrowl_set_growl_server(char *name)
{
	static char port[15+1]= "";
	static char host[40+1]= "";
/*	char *sample= "127.0.0.1:23053"; */
	char *scanner=name+strlen(name);
	int  len= 0;

	while (scanner!=name) {
		/* scan from the back */
		if ((*scanner)==':' && len>0) {
			strcpy(port,scanner+1);
			len= 0; /* reset for host name */
			}
		len++;
		scanner--;
		}
	memset(host, 0, sizeof(host));
	strncpy(host,name,len);

	/* if host is not ip, convert to ip */
{
	/* this works for macos */
	struct hostent *hp= gethostbyname(host);
	if (hp!=NULL) {
		strcpy(host, inet_ntoa(*(struct in_addr*)(hp->h_addr_list[0])));
	}
}
	
	g_host= host;
	g_host_name= g_host; /* not used */
	if (port[0]=='\0')
		g_port= GROWL_DEFAULT_PORT;
	else
		g_port= atoi(port);
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

/**
 *  retrieve header value in GNTP protocol
 *  @author  <a href="mailto:fuyichin@gmail.com">hooichee</a>
 *  @param value  return value
 *  @param size  size of data, to control out of bound
 *  @param message gntop protocol message
 *  @return header code value eg. 
 *
 *  return 0-sucess
 */
char *gntp_get_header_value(char *value, int size, char *header_code, char *message) 
{
	const char s[]= "\n";
	char *token;
	char msg1[1024];
	char key[80+1]= "lemon:";

	/* init */
	value[0]= '\0'; /* value= "" will point to NULL, or can use value[0]= '\0'; or strcpy(value,""); */

	/* boundry check */
	if (strlen(header_code)>=sizeof(key)+1)
		return "";
	strcpy(key,header_code);
	strcat(key,": ");

	/* boundry check */
	if (strlen(message)>=sizeof(msg1))
		return "";
	strcpy(msg1,message);  /* copy to a buffer for strtok */

	/* split message with \r\n */
    token= strtok(msg1,s);
	if (debug_level>=3)
		fprintf(stdout, "checking key [%s] in the message list:\n", key);
	while(token!=NULL) {
	    token[strlen(token)-1]= '\0'; /* replace \r with \w for further processing */

		if (strncmp(key,token,strlen(key))==0 && strlen(token)>strlen(key)) { 
			/* boundry check to prevent p_value point to '\0', 'lemon:' with empty value */
			char *p_value= (token+strlen(key)); /* apple:red\0, after the key is the value */
			int pvalue_len= strlen(p_value);

			if (size>pvalue_len+1) {/* check value buffer is big enough */
				strcpy(value,p_value);
			}
			else {
				strncpy(value,p_value,size);
				value[size-1]= '\0'; /* part of the value will be trim */
			}
			break;
		}

        token= strtok(NULL,s);
	}
	if (debug_level>=3) {
		if (value[0]=='\0') {
			fprintf(stdout, "key [%s] not found!!!\n", key);
		}
		else
			fprintf(stdout, "key [%s] found.\n", key);
	}
	return value;
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
	char server_reply[2000], tmp_reply[2000];
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

	strcpy(tmp_reply,server_reply); /* gntp_decode_reply will change the value of server_reply */	
	gntp_decode_reply(tmp_reply,&gntp_reply);
	if (gntp_reply.status==GNTP_OK) {
		if (debug_level>=1)
			fprintf(stdout, "gntp reply success.\n");
	}
	else {
		if (debug_level>=1) {
			fprintf(stdout, "gntp reply fail.\n");
			/* print error code and reason */
			fprintf(stdout,
				"GNTP error code: %s\n", gntp_get_header_value(data,d_size,HEADER_ERROR_CODE,server_reply));
			fprintf(stdout,
				"Description: %s\n", gntp_get_header_value(data,d_size,HEADER_ERROR_DESCRIPTION,server_reply));
		}
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

/* this is for mac os, bsd platform */
/**
 *  @brief  get the platfrom name and version info
*/
void bsd_get_platform_name_version(char *platform, char *version)
{
    char cmd[64];
    FILE* stdoutFile;

#ifdef _LINUX
	sprintf(cmd, "uname -s");
#else
	sprintf(cmd, "sw_vers -productName");
#endif
    stdoutFile = popen(cmd, "r") ;
    if (stdoutFile!=NULL) {
        char buff[16] ;
        char *out = fgets(buff, sizeof(buff), stdoutFile) ;
        pclose(stdoutFile) ;
		/* sscanf(out, "%s", platform); */
		/* debug */
		if (out)
			strcpy(platform,out);
		if (platform[strlen(platform)-1]=='\n')
			platform[strlen(platform)-1]='\0';
    }

#ifdef _LINUX
	sprintf(cmd, "uname -r");
#else
    sprintf(cmd, "sw_vers -productVersion");
#endif
    stdoutFile = popen(cmd, "r") ;
    if (stdoutFile) {
        char buff[16] ;
        char *out = fgets(buff, sizeof(buff), stdoutFile) ;
        pclose(stdoutFile) ;

        if (out)
			strcpy(version,out);
		if (version[strlen(version)-1]=='\n')
			version[strlen(version)-1]='\0';
    }
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
	if (libgrowl_get_minimalist_mode()==FALSE) {
		char system_name[80], system_version[80];

		gethostname(hostname, HOST_NAME_MAX);
		if (debug_level>=2)
			printf("homename=%s\n", hostname);
		sprintf(text, "%s: %s%s", HEADER_ORIGIN_MACHINE_NAME, hostname, LINE_BREAK);
		strcat(message, text);
		sprintf(text, "%s: %s%s", HEADER_ORIGIN_SOFTWARE_NAME, app, LINE_BREAK);
		strcat(message, text);
		sprintf(text, "%s: %s%s", HEADER_ORIGIN_SOFTWARE_VERSION, LIBGROWL_VERSION, LINE_BREAK);
		strcat(message, text);

		strcpy(system_name, "unknown"); /* initialize */
		strcpy(system_version, "unknown");
		/* unix_get_system_name(system_name,sizeof(system_name));  */ /* this get back Darwin not macOS */
		bsd_get_platform_name_version(system_name,system_version); /* bsd/macos only? */
		g_platform_name= system_name;
		if (g_platform_name!=NULL) {
			/* these 2 are optional */
			sprintf(text, "%s: %s%s", HEADER_ORIGIN_PLATFORM_NAME, system_name, LINE_BREAK);
			strcat(message, text);
			sprintf(text, "%s: %s%s", HEADER_ORIGIN_PLATFORM_VERSION, system_version, LINE_BREAK);
			strcat(message, text);
			}
		}
/* debug
	else { 
		fprintf(stdout, "** minimalist is TRUE(in libgrowl %d, FALSE=%d)\n", 
			libgrowl_get_minimalist_mode(), FALSE);
		return 0;
		}
*/

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
	status= gntp_send(message, (int)strlen(message), 
		libgrowl_get_host(), libgrowl_get_port(), text, sizeof(text));
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
	int status;
	
	/* print all notification name */
	if (debug_level>=2) {
		printf("name=%s\n", notification);
	}
	
	/* gntp, format notification message */
	message[0]= '\0';
	sprintf(text, "%s %s %s%s", GNTP_VERSION, MESSAGETYPE_NOTIFY, ENCRYPTION_NONE, LINE_BREAK);
	strcat(message, text);
	if (libgrowl_get_minimalist_mode()==FALSE) {
		char system_name[80], system_version[80];

		gethostname(hostname, HOST_NAME_MAX);
		if (debug_level>=2)
			printf("homename=%s\n", hostname);
		sprintf(text, "%s: %s%s", HEADER_ORIGIN_MACHINE_NAME, hostname, LINE_BREAK);
		strcat(message, text);
		sprintf(text, "%s: %s%s", HEADER_ORIGIN_SOFTWARE_NAME, app, LINE_BREAK);
		strcat(message, text);
		sprintf(text, "%s: %s%s", HEADER_ORIGIN_SOFTWARE_VERSION, LIBGROWL_VERSION, LINE_BREAK);
		strcat(message, text);
		

		strcpy(system_name, "unknown"); /* initialize */
		strcpy(system_version, "unknown");
		/* unix_get_system_name(system_name,sizeof(system_name)); */ /* this get back Darwin not macOS */
		bsd_get_platform_name_version(system_name,system_version); /* bsd/macos only? */
		g_platform_name= system_name;
		if (g_platform_name!=NULL) {
			/* these 2 are optional */
			sprintf(text, "%s: %s%s", HEADER_ORIGIN_PLATFORM_NAME, system_name, LINE_BREAK); /* Mac OS X */
			strcat(message, text);
			sprintf(text, "%s: %s%s", HEADER_ORIGIN_PLATFORM_VERSION, system_version, LINE_BREAK); /* 10.13.6 */
			strcat(message, text);
		}
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


/*
This GrowlHackServer, expecting
REGISTER. Always reply ok. 
NOTIFY. reply ok and send message to dbus.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#ifdef _LIBNOTIFY
#include <libnotify/notify.h>
#endif
#include "libgrowl.h"
#include "mygntp.h"

/* from libgrowl.c */
// char *gntp_get_header_value(char *value, int size, char *header_code, char *message) 
char *gntp_get_header_value(char *, int, char *, char *);

char *serverhack_format_message_ok(char *, char *);
char *serverhack_format_message_fail(char *, char *, char *);
void output_message_console(char *title, char *message);

int get_param_mode(char *args[], int argc, char *mode);

#ifndef FALSE
#define FALSE 0
#define TRUE !FALSE
#endif
#define PORT 23053
#define MAX_BUFFER_SIZE 1024

#ifdef _LIBNOTIFY
int console_mode= FALSE;
#else
/* no libnotify, console_mode= TRUE */
int console_mode= TRUE;
#endif
int show_line_number= FALSE;

/* This program looks a bit complicated and confusing, please
   refer to the documentation of the program flow at the end of this file
*/
#ifdef _SERVERHACK /* till end of the file */
#ifndef _TEST
int main(int argc, char *argv[]) {
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	socklen_t addrlen = sizeof(address);
	char buffer[MAX_BUFFER_SIZE] = {0};
	pid_t pid;

	// This is for testing
	if (argc>=2) {
		if (strcmp(argv[1],"--test")==0)
			return 0;
	}

	if (console_mode==FALSE)
		console_mode= get_param_mode(argv, argc, "--console");
	if (show_line_number==FALSE)
		show_line_number= get_param_mode(argv, argc, "--set-nu");

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 1) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// Binding the socket to the address
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	// Listening for incoming connections
	if (listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	printf("Server listening on port %d\n", PORT);

	while (1) {
		// Accepting a connection
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0) {
			perror("accept");
			continue; // Continue to accept other connections even if one fails
		}

		// Forking a new process to handle the connection
		pid = fork();
		if (pid < 0) {
			perror("fork failed");
			close(new_socket); // Close socket in parent on fork failure
			continue; // Continue accepting connections
		}

		if (pid == 0) { /* child process */
			char *response;
			char tmp_message[2000];
			struct gntp_message gntp_message;
			char value[160];
#ifdef _LIBNOTIFY
			NotifyNotification *growl_notification; /* just forward the message to dbus */
#endif

			// Child process
			close(server_fd); // Close the listening socket in the child
#ifdef _DEBUG
			printf("===== child process started to read =====\n");
#endif
			valread= read(new_socket, buffer, MAX_BUFFER_SIZE);
#ifdef _DEBUG
			printf("Child process handling request, read %d bytes: \n", valread);
#endif

			/* process REGISTER */
			strcpy(tmp_message, buffer);  /* decode_message will changed the buffer value */
			gntp_decode_message(tmp_message,&gntp_message);
#ifdef _DEBUG
			printf("GNTP message type=%d\n", gntp_message.type);
			if (gntp_message.type==GNTP_REGISTER)
				printf("The GNTP message request is REGISTER\n");
#endif
			// get Application-Name
			if (gntp_message.type==GNTP_REGISTER) {
				strcpy(tmp_message, buffer); /* get_header_value will change the buffer value */
				value[0]= '\0';
				gntp_get_header_value(value, sizeof(value), "Application-Name", tmp_message);
#ifdef _DEBUG
				printf("application name=%s\n", value);
#endif
			}

			/* process gntp messages from here */
			if (gntp_message.type==GNTP_REGISTER) {
				// reply register ok
				serverhack_format_message_ok(tmp_message, "REGISTER");
				response= tmp_message;
				send(new_socket, response, strlen(response), 0);
			} 
			else if (gntp_message.type==GNTP_NOTIFY) {
				/* process NOTIFY */
				char message_title[80], message_text[80]; 

				// reply notify ok
				serverhack_format_message_ok(tmp_message, "NOTIFY");
				response= tmp_message;
				send(new_socket, response, strlen(response), 0);

				// get msg_title and msg_text, whic is the notify title and text
				strcpy(tmp_message, buffer);  /* decode_message will changed the buffer value */
				gntp_get_header_value(message_title, sizeof(message_title), "Notification-Title", tmp_message);
				strcpy(tmp_message, buffer);
				gntp_get_header_value(message_text, sizeof(message_text), "Notification-Text", tmp_message);

				if (strcmp(message_title,"")==0)
					strcpy(message_title,"Growl Server Hack");
#ifdef _LIBNOTIFY
				if (!console_mode) {
					/* send notification to dbus */
					notify_init("growlserverhack"); /* create libnotify init */
					growl_notification= notify_notification_new( /* send notification message */
						message_title, message_text, "dialog-information");
					notify_notification_show(growl_notification, NULL);
				} 
				else {
					/* console_mode */
					output_message_console(message_title, message_text);
					output_message_console(message_title, message_text);
//					printf("<%s>[%s]\n", message_title, message_text);
				}
#else
				/* output to console display */
				if (console_mode) {
					output_message_console(message_title, message_text);
					output_message_console(message_title, message_text);
//					printf("<%s>[%s]\n", message_title, message_text);
				}
#endif	
				}
			else {
				// reply fail
				serverhack_format_message_fail(tmp_message, "405", "unknown message");
				response= tmp_message;
				send(new_socket, response, strlen(response), 0);
			} /* end of processing gntp messages */

           close(new_socket); // Close the client socket
#ifdef _DEBUG
			printf("child process to be terminated\n");
#endif
			exit(EXIT_SUCCESS); // Terminate child process
		} else {  /* end of child process */
			// Parent process
			close(new_socket); // Close the client socket in the parent
			wait(NULL); // Wait for any child process to finish (to prevent zombies)
		} /* end of child and parent process */
	} /* end of while loop */

	close(server_fd); // This line is unreachable in the current loop structure
	return 0;
}
#endif /* _TEST */

/*
*  quick hack. from reply message ok
*/
char *serverhack_format_message_ok(char *message, char *action)
{
	char text[800]; /* tmp buffer, use for return status as well */

	message[0]= '\0';
	sprintf(text, "%s %s %s%s", GNTP_VERSION, "-OK", ENCRYPTION_NONE, LINE_BREAK);
	strcat(message, text);

	sprintf(text, "%s: %s%s", "Response-Action", action, LINE_BREAK);
	strcat(message, text);

	sprintf(text, "%s: %s%s", "Origin-Software-Name", "GrowlHackServer", LINE_BREAK);
	strcat(message, text);

	return message;
}

/*
*  quick hack. from reply message fail
*/
char *serverhack_format_message_fail(char *message, char *code, char *description)
{
	char text[800]; /* tmp buffer, use for return status as well */

	message[0]= '\0';
	sprintf(text, "%s %s %s%s", GNTP_VERSION, "-ERROR", ENCRYPTION_NONE, LINE_BREAK);
	strcat(message, text);

	sprintf(text, "%s: %s%s", "Error-Code", code, LINE_BREAK);
	strcat(message, text);

	sprintf(text, "%s: %s%s", "Error-Description", description, LINE_BREAK);
	strcat(message, text);

	return message;
}

/* output to console */
void output_message_console(char *title, char *message) {
	static int line_count= 0;
	if (show_line_number)
		printf("%d:<%s>[%s]\n", line_count, title, message);
	else
		printf("<%s>[%s]\n", title, message);
	++line_count;
}

#else
/* no server hack */
#ifndef _TEST
	int main() {
		printf("Server hack is not enable, try --configure --enable-serverhack before compile\n");
		return 1;
	}
#endif /* _TEST */
#endif /* _SERVERHACK */

int get_param_mode(char *args[], int argc, char *mode) {
	int result= FALSE;
	int i;

	for (i=0; i<argc; i++) {
		if (strcmp(args[i],mode)==0) {
			result= TRUE;
			/* if extra value after mode name, check is 0|off */
			if (i+1<argc)
				if (strcmp(args[i+1],"0")==0 || strcmp(args[i+1],"off")==0) {
					result= FALSE;
					break;
				}
		}
	}
	return result;
}

/* explanation of the program flow

if -D_SERVERHACK not enable, this program do nothing, it is empty.

// if -D_SERVERHACK
while(1) {
	// socket server listing to a client connection.

	// fork out a child

	// child process client request 1. init 2. notify message

	// parent socket continue listing 
}

// if -D_LIBNOTIFY, child forward notify message to dbus
// if -D_LIBNOTIFY + console_mode=1, output message to console display

// if no libnotify, just console_mode=1, output message to console display

// when _TEST is enabled, it is used for cmocka unit test
// it remove 2 main() from this program, 1. main() for server hack 2. main() no server hack

// for testing [--disable-serverhack] (default)
// for production --enable-serverhack, --disable-libnotify output message to console
*/
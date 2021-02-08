/**
 * libgrowl.h
 * @author hooichee<fuyichin@gmail.com>
 *
 *  Created by Chin Hooi Chee on 2/5/21.
 *  Copyright © 2021 Chin Hooi Chee. All rights reserved.
 */
#ifndef libgrowl_h
#define libgrowl_h
#endif /* libgrowl_h */

#define GROWL_DEFAULT_PORT 23053
#define LIBGROWL_VERSION  "0.1.0"

#define ENCRYPTION_NONE  "NONE"
#define GNTP_VERSION  "GNTP/1.0"
#define HEADER_APPLICATION_ICON  "Application-Icon"  /* optional */
#define HEADER_APPLICATION_NAME  "Application-Name"  /* required */
#define HEADER_NOTIFICATION_ENABLED  "Notification-Enabled" /* optional, default is False? */
/**
 * Optional - The icon to display with the notification.
 */
#define HEADER_NOTIFICATION_ICON  "Notification-Icon"
/**
 * Optional - A unique ID for the notification. If present, serves as a hint
 * to the notification system that this notification should replace any
 * existing on-screen notification with the same ID. This can be used to
 * update an existing notification. The notification system may ignore this
 * hint.
 */
#define HEADER_NOTIFICATION_ID  "Notification-ID"
#define HEADER_NOTIFICATION_NAME  "Notification-Name" /* required */
#define HEADER_NOTIFICATION_DISPLAY_NAME  "Notification-Display-Name" /* optional */
/* higher number indicates higher priority */
#define HEADER_NOTIFICATION_PRIORITY  "Notification-Priority" /* optional, -2 to 2, default 0 */
#define HEADER_NOTIFICATION_STICKY  "Notification-Sticky" /* optional, True-notification remain on the screen, default is False */
#define HEADER_NOTIFICATION_TEXT  "Notification-Text" /* optional, default to '' */
#define HEADER_NOTIFICATION_TITLE  "Notification-Title" /* required */
#define HEADER_NOTIFICATIONS_COUNT  "Notifications-Count" /* required */
#define HEADER_ORIGIN_MACHINE_NAME  "Origin-Machine-Name" /* optional eg. my-computer */
#define HEADER_ORIGIN_PLATFORM_NAME  "Origin-Platform-Name" /* optional eg. Mac OS X */
#define HEADER_ORIGIN_PLATFORM_VERSION  "Origin-Platform-Version" /* optional eg. 10.13.6 */
#define HEADER_ORIGIN_SOFTWARE_NAME  "Origin-Software-Name" /* optional eg. libgrowl, growlnotify */
#define HEADER_ORIGIN_SOFTWARE_VERSION  "Origin-Software-Version" /* optional eg. '0.1.0' */
#define LINE_BREAK  "\r\n" /* line break */
#define MESSAGETYPE_NOTIFY  "NOTIFY"
#define MESSAGETYPE_REGISTER  "REGISTER"

void libgrowl_set_debug_level(int);
int growl_register_notifications(char *, char **);
int growl_send_notification(char *, char *, char *, char *);

#!/bin/sh
# check configure help has --disable-libnotify
../configure --help | grep "\--disable-libnotify"
if [ $? -ne 0 ]; then
  exit 1
fi  
# check configure --disable-libnotify has no _LIBNOTIFY in CFLAGS

exit 0
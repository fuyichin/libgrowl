AM_CPPFLAGS=-I$(top_srcdir)/src
bin_PROGRAMS=growlnotify 
# this will be compile but not installed
check_PROGRAMS=GrowlServerHack

growlnotify_SOURCES=growlnotify.c ../src/libgrowl.c

GrowlServerHack_CFLAGS=$(LIBNOTIFY_CFLAGS)
GrowlServerHack_LDADD=$(LIBNOTIFY_LIBS)
GrowlServerHack_SOURCES=serverhack.c param.c ../src/libgrowl.c

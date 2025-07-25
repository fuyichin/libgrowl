AM_CPPFLAGS=-I$(top_srcdir)/src
bin_PROGRAMS=growlnotify GrowlServerHack

growlnotify_SOURCES=growlnotify.c ../src/libgrowl.c

GrowlServerHack_CFLAGS=$(LIBNOTIFY_CFLAGS)
GrowlServerHack_LDADD=$(LIBNOTIFY_LIBS)
GrowlServerHack_SOURCES=serverhack.c ../src/libgrowl.c

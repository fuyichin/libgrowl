AUTOMAKE_OPTIONS=1.4

SUBDIRS=src examples cmocka 

clean-all: maintainer-clean-local

maintainer-clean-local:
	rm -rf aclocal.m4 configure configure~ Makefile.in autotools m4
	rm -f autoscan.log
	rm -rf autom4te.cache
	rm -f config.log 
	rm -rf .deps src/.deps examples/.deps cmocka/.deps
	rm -rf src/.dirstamp
	rm -rf test-driver
 
AUTOMAKE_OPTIONS= 1.4
ACLOCAL_AMFLAGS = -I m4

#pkgconfigdir = @PKG_CONF_DIR@
# now hard code
#pkgconfigdir = @prefix@/lib/pkgconfig
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libgrowl.pc

SUBDIRS = src examples cmocka

#bin_PROGRAMS = myconfig_guess
dist_data_DATA = myconfig_guess

# extra files to be included in full distribution
EXTRA_DIST = \
	autogen.sh \
	ChangeRequest control \
	doxygen.conf LICENSE \
	Makefile.unix \
	README.md \
	NEWS ROADMAP TODO \
	src/Makefile.unix examples/Makefile.unix \
	cmocka/Makefile.unix \
	cmocka/run_unit_test.sh

clean-all: maintainer-clean-local

maintainer-clean-local:
	rm -rf aclocal.m4 configure configure~ Makefile.in autotools m4
	rm -rf autom4te.cache
	rm -f config.log config.status
	rm -rf .deps src/.deps examples/.deps cmocka/.deps
	rm -rf src/.dirstamp

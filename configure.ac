#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

#AC_PREREQ([2.71])
AC_INIT([libgrowl], [0.1.0], [fuyichin@gmail.com])
AM_INIT_AUTOMAKE([foreign subdir-objects])
AC_CONFIG_SRCDIR([src/libgrowl.c])

# Checks for programs.
# AC_PROG_CC([gcc cc tcc])
AC_PROG_CC([gcc cc])
AC_PROG_INSTALL
AC_PROG_RANLIB
AC_PROG_MAKE_SET

# Only use -Wall if we have gcc or tcc
echo "***$GCC ***"
echo "***tcc=$TCC ***"
if test "x$GCC" = "xyes" || "x$TCC" = "xyes"; then
  if test -z "`echo "$CFLAGS" | grep "\-Wall" 2> /dev/null`" ; then
    CFLAGS="$CFLAGS -Wall"
  fi
else
  if test -z "`cc -V | grep "WorkShop Compiler" 2> /dev/null`" ; then
    CFLAGS="$CFLAGS -xCC"
  fi
fi

# Checks for libraries.
# FIXME: Replace 'main' with a function in '-lcmocka':
AC_CHECK_LIB([cmocka], [main])

# Checks for header files.
# AC_CHECK_HEADERS([arpa/inet.h netdb.h sys/socket.h unistd.h])

# AC_CANONICAL_HOST is needed to access the 'host_os' variable
AC_CANONICAL_HOST  

# manual enable-macos or enable-linux
# if config.guess or check OS type not working
AC_ARG_ENABLE(linux,
[  --enable-linux           Enable Linux OS [default=disabled]], linux_mode="yes",
	linux_mode="no")

if test "x$linux_mode" = xyes; then
	CFLAGS="$CFLAGS -D_LINUX"
	AC_MSG_RESULT([*** linux os mode enable as user request ***])
	linux_mode=yes
	host_os=Linux
#else
#	AC_MSG_RESULT([*** linux os mode disable as default ***])
fi

AC_ARG_ENABLE(macos,
[  --enable-macos           Enable Mac OS [default=disabled]], macos_mode="yes",
	macos_mode="no")

if test "x$macos_mode" = xyes; then
	CFLAGS="$CFLAGS -D_Darwin"
	AC_MSG_RESULT([*** mac os mode enable as user request ***])
	macos_mode=yes
	host_os=darwin
fi

# if manual define host_os, no need to auto check
if test "x$host_os" = x; then
# check OS type

case "${host_os}" in
	linux*)
		CFLAGS="$CFLAGS -DLinuxOS"
		;;
	*Linux*)
echo "** found Linux OS **"
		CFLAGS="$CFLAGS -DLinuxOS -D_LINUX"
		;;
	solaris*)
		CFLAGS="$CFLAGS -DSunOS -DSolaris"
		;;
	aix*)
		CFLAGS="$CFLAGS -DAIX"
		;;
	osx*)
		CFLAGS="$CFLAGS -DMacOS -DOSX -D_Darwin"
		;;
	*darwin*)
echo "** found Darwin kernel **"
		CFLAGS="$CFLAGS -DMacOS -DOSX -D_Darwin"
		;;
	*)
		;;
esac

fi  # end of manual set host_os

# Checks for library functions.
#AC_CHECK_FUNCS([strcspn strstr uname])

AC_CONFIG_FILES([Makefile
		libgrowl.pc
		src/Makefile
		examples/Makefile
		cmocka/Makefile])
AC_OUTPUT

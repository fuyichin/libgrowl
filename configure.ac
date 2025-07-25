#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT([libgrowl], [0.1.0], [fuyichin@gmail.com])
AM_INIT_AUTOMAKE([foreign subdir-objects])
AC_CONFIG_SRCDIR([src/libgrowl.c])

# Checks for programs.
AC_PROG_CC([gcc cc tcc])
AC_PROG_RANLIB

# Only use -Wall if we have gcc
if test "x$GCC" = "xyes"; then
  if test -z "`echo "$CFLAGS" | grep "\-Wall" 2> /dev/null`" ; then
    CFLAGS="$CFLAGS -Wall"
  fi
elif test "`tcc -v | grep tcc`" 2> /dev/null; then
    CFLAGS="$CFLAGS -Wall"
else
  if test "`cc -V | grep "WorkShop Compiler" 2> /dev/null`" ; then
    CFLAGS="$CFLAGS -xCC"
  fi
fi

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

# enable debug mode
debug_mode="no"
AC_ARG_ENABLE(debug,
[ --enable-debug    Enable debug mode [default=disabled]], debug_mode="yes",
    debug_mode="no")

if test "x$debug_mode" = "xno"; then
    AC_MSG_RESULT([*** debug mode disable as default ***])
else
    CFLAGS="$CFLAGS -D_DEBUG"
    AC_MSG_RESULT([*** debug mode enable as user request ***])
fi

# enable cmocka test framework
cmocka_mode="yes"
AC_ARG_ENABLE(cmocka,
[ --disable-cmocka    Disable cmocka mode [default=enabled]], cmocka_mode="no",
    cmocka_mode="yes")

if test "x$cmocka_mode" = "xno"; then
    AC_MSG_RESULT([*** cmocka mode disable as user request ***])
else
  AC_MSG_RESULT([*** cmocka mode enable as default ***])
  # check for pkg-config
  PKG_PROG_PKG_CONFIG
  ### generate CMOCKA_CFLAGS, CMOCKA_LIBS
  PKG_CHECK_MODULES([CMOCKA],[cmocka])
fi

# detect os platform
if test -x "./config.guess"; then
host_os=`./config.guess`
fi
echo "*** host_os $host_os"
if test "x$host_os" = "x"; then
# echo "host os blank"
# This is old type checking, new use $os_host AM_PROG_LIBTOOL
AC_CHECK_HEADER(sys/strsun.h, CFLAGS="$CFLAGS -DSunOS -DSolaris")
AC_CHECK_HEADER(getopt.h, CFLAGS="$CFLAGS -DLinuxOS -D_LINUX")
else
  echo " *** host_os $host_os *** "
  case ${host_os} in
  *linux* )
    CFLAGS="$CFLAGS -DLinuxOS -D_LINUX"
    ;;
  *darwin*)
    CFLAGS="$CFLAGS -DMacOS -DOSX -DDarwin -DTERMIOS"
    ;;
  *)
    CFLAGS="$CFLAGS"
    ;;
  esac
fi

AC_CONFIG_FILES([Makefile src/Makefile cmocka/Makefile]
  examples/Makefile)
AC_OUTPUT

#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([rgb2yuv-intrinsics], [1.0], [g6@tec.com])
AC_CONFIG_SRCDIR([src/rgb2yuv-intrinsics.cpp])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CXX

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CHECK_FUNCS([gettimeofday])

AC_OUTPUT(Makefile doc/Makefile src/Makefile)

#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(gcjx, 0.1, [tromey@redhat.com])
AC_CONFIG_SRCDIR([name.cc])
AM_INIT_AUTOMAKE
AC_CONFIG_HEADER([gcjx-config.h])
AM_MAINTAINER_MODE

AC_ARG_WITH([bootclasspath],
  AS_HELP_STRING([--with-booclasspath=PATH], [set default boot classpath]),
  AC_DEFINE(BOOTCLASSPATH, "$withval", [Default -bootclasspath]))

AC_ARG_ENABLE([gcjx-debugging],
  AS_HELP_STRING([--with-gcjx-debugging], [enable debugging code]),
  [if test "$enableval" != no; then
    AC_DEFINE(_GLIBCXX_DEBUG, 1, [enable libstdc++ checking])
  fi])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_LIBTOOL
AM_PROG_CC_C_O

# Checks for libraries.
AC_CHECK_LIB([iconv], [iconv])
AC_CHECK_LIB([m], [sin])
AC_CHECK_LIB([z], [inflate])

AC_C_BIGENDIAN

AC_DEFINE(_GLIBCPP_USE_C99, 1, [enable c99 features])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

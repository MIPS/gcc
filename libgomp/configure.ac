m4_define([GOMP_MAJOR], [0])
m4_define([GOMP_MINOR], [1])
m4_define([GOMP_MICRO], [0])

# *****************************************************************************

AC_PREREQ(2.59)
AC_INIT([libgomp], [GOMP_MAJOR.GOMP_MINOR.GOMP_MICRO])
#AC_CONFIG_AUX_DIR(..)
AM_INIT_AUTOMAKE(libgomp, GOMP_MAJOR.GOMP_MINOR.GOMP_MICRO)
AM_CONFIG_HEADER(config.h)
AC_CONFIG_SRCDIR([lib/libgomp.h])

# Check the compiler.
# The same as in boehm-gc and libstdc++. Have to borrow it from there.
# We must force CC to /not/ be precious variables; otherwise
# the wrong, non-multilib-adjusted value will be used in multilibs.
# As a side effect, we have to subst CFLAGS ourselves.

m4_rename([_AC_ARG_VAR_PRECIOUS],[real_PRECIOUS])
m4_define([_AC_ARG_VAR_PRECIOUS],[])
AC_PROG_CC
m4_rename([real_PRECIOUS],[_AC_ARG_VAR_PRECIOUS])

AC_SUBST(CFLAGS)

# Add -Wall if we are using GCC.
if test "x$GCC" = "xyes"; then
  CFLAGS="$CFLAGS -Wall"
fi

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB

# Configure libtool
#AC_MSG_NOTICE([====== Starting libtool configuration])
AC_LIBTOOL_DLOPEN
AM_PROG_LIBTOOL
AC_SUBST(enable_shared)
AC_SUBST(enable_static)
#AC_MSG_NOTICE([====== Finished libtool configuration]) ; sleep 10

# Checks for libraries.
AC_CHECK_LIB(m, sqrt,,AC_MSG_WARN(Missing libm - not all samples will link
	properly))

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h math.h sys/sysinfo.h string.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_HEADER_TIME

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([gettimeofday memset sqrt])

# Write our Makefile.
AC_CONFIG_FILES(Makefile)
AC_SUBST(ac_aux_dir)
AC_OUTPUT

# *****************************************************************************


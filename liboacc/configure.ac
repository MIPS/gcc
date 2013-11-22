
AC_PREREQ([2.64])
AC_INIT([OpenACC Runtime Library], [1.0], [dark_beer@rambler.ru])

AC_MSG_CHECKING([for --enable-version-specific-runtime-libs])
LIBOACC_ENABLE(version-specific-runtime-libs, no, ,
   [Specify that runtime libraries should be installed in a compiler-specific directory],
   permit yes|no)
AC_MSG_RESULT($enable_version_specific_runtime_libs)

# We would like our source tree to be readonly. However when releases or
# pre-releases are generated, the flex/bison generated files as well as the
# various formats of manuals need to be included along with the rest of the
# sources.  Therefore we have --enable-generated-files-in-srcdir to do 
# just that.
AC_MSG_CHECKING([for --enable-generated-files-in-srcdir])
LIBOACC_ENABLE(generated-files-in-srcdir, no, ,
   [put copies of generated files in source dir intended for creating source 
    tarballs for users without texinfo bison or flex.],
   permit yes|no)
AC_MSG_RESULT($enable_generated_files_in_srcdir)
AM_CONDITIONAL(GENINSRC, test "$enable_generated_files_in_srcdir" = yes)


# -------
# -------

# Gets build, host, target, *_vendor, *_cpu, *_os, etc.
#
# You will slowly go insane if you do not grok the following fact:  when
# building this library, the top-level /target/ becomes the library's /host/.
#
# configure then causes --target to default to --host, exactly like any
# other package using autoconf.  Therefore, 'target' and 'host' will
# always be the same.  This makes sense both for native and cross compilers
# just think about it for a little while.  :-)
#
# Also, if this library is being configured as part of a cross compiler, the
# top-level configure script will pass the "real" host as $with_cross_host.
#
# Do not delete or change the following two lines.  For why, see
# http://gcc.gnu.org/ml/libstdc++/2003-07/msg00451.html
AC_CANONICAL_SYSTEM
target_alias=${target_alias-$host_alias}

AM_INIT_AUTOMAKE([1.11.0 foreign])
AM_ENABLE_MULTILIB(, ..)

# Calculate toolexeclibdir
# Also toolexecdir, though it's only used in toolexeclibdir
case ${enable_version_specific_runtime_libs} in
  yes)
    # Need the gcc compiler version to know where to install libraries
    # and header files if --enable-version-specific-runtime-libs option
    # is selected.
    toolexecdir='$(libdir)/gcc/$(target_alias)'
    toolexeclibdir='$(toolexecdir)/$(gcc_version)$(MULTISUBDIR)'
    ;;
  no)
    if test -n "$with_cross_host" &&
       test x"$with_cross_host" != x"no"; then
      # Install a library built with a cross compiler in tooldir, not libdir.
      toolexecdir='$(exec_prefix)/$(target_alias)'
      toolexeclibdir='$(toolexecdir)/lib'
    else
      toolexecdir='$(libdir)/gcc-lib/$(target_alias)'
      toolexeclibdir='$(libdir)'
    fi
    multi_os_directory=`$CC -print-multi-os-directory`
    case $multi_os_directory in
      .) ;; # Avoid trailing /.
      *) toolexeclibdir=$toolexeclibdir/$multi_os_directory ;;
    esac
    ;;
esac
AC_SUBST(toolexecdir)
AC_SUBST(toolexeclibdir)

AC_CONFIG_SRCDIR([liboacc.c])

# Checks for options.
opencl=
openclinc=
opencllib=

AC_MSG_CHECKING([for OpenCL options])
AC_ARG_WITH(opencl,
 [AS_HELP_STRING([--with-opencl=PATH], [specify prefix directory for installed OpenCL SDK.])],
 [opencl=${withval}])
AC_ARG_WITH(opencl-include,
 [AS_HELP_STRING([--with-opencl-include=PATH], [specify directory for installed OpenCL SDK include files])],
 [openclinc=${withval}])
AC_ARG_WITH(opencl-lib,
 [AS_HELP_STRING([--with-opencl-lib=PATH], [specify directory for intalled OpenCL SDK library files])],
 [opencllib=${withval}])

if test "x$openclinc" == x ; then
	if test "x$opencl" != x ; then
		openclinc="${opencl}/include"
	else
		AC_MSG_ERROR([This library requires OpenCL SDK path specified in --with-opencl option], 1)
	fi
fi

if test "x$opencllib" == x ; then
	if test "x$opencl" != x ; then
		opencllib="${opencl}/lib"
	fi
fi
AC_MSG_RESULT([$openclinc])

AC_SUBST(openclinc)
AC_SUBST(opencllib)

AC_MSG_CHECKING([for thread model used by GCC])
THREADS=`$CC -v 2>&1 | sed -n 's/^Thread model: //p'`
if test x"$THREADS" == x; then
   THREADS=no
fi
AC_MSG_RESULT([$THREADS])

# THREADCFLAGS=
THREADLIBS=

case "$THREADS" in
 no | none | single)
    THREADS=none
    ;;
 posix | pthreads)
    THREADS=posix
    THREADLIBS=-lpthread
    ;;
 *)
    AC_MSG_ERROR($THREADS library is not supported)
    ;;
esac

# AC_SUBST(THREADCFLAGS)
AC_SUBST(THREADLIBS)


# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_CC_C_O
AM_PROG_LIBTOOL
libtool_VERSION=1:0:0
AC_SUBST(libtool_VERSION)

AM_MAINTAINER_MODE

# Checks for libraries.
# Create a spec file, so that compile/link tests don't fail
test -f libgfortran.spec || touch libgfortran.spec
FCFLAGS="$FCFLAGS -L."

# We need gfortran to compile parts of the library
AM_CONDITIONAL([USE_FORTRAN], [test "$ac_cv_fc_compiler_gnu" = yes])
# We can't use AC_PROG_FC because it expects a fully working gfortran.
#AC_PROG_FC(gfortran)
case `echo $GFORTRAN` in
  -* | no* )
    FC=no ;;
  *)
    set dummy $GFORTRAN; ac_word=$2
    if test -x "$ac_word"; then
      FC="$GFORTRAN"
    else
      FC=no
    fi ;;
esac
AC_PROG_FC(gfortran)
FCFLAGS="$FCFLAGS -Wall -L../libgfortran/.libs"
# Checks for header files.
AC_CHECK_HEADERS([stdlib.h])
if test "x$THREADS" == xposix ; then
AC_CHECK_HEADERS([pthread.h])
fi

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC

save_CFLAGS="$CFLAGS"

CFLAGS="$CFLAGS -I$openclinc"
AC_COMPILE_IFELSE(
 [AC_LANG_PROGRAM(
  [#if __CYGWIN__ && !_WIN32
   #define _WIN32 1
   #endif
   #ifdef MAC
   #include <OpenCL/cl.h>
   #else  
   #include <CL/cl.h>
   #endif
   ],
  [cl_platform_id platform; clGetPlatformIDs(1, &platform, NULL);])],
 [], [AC_MSG_ERROR([OpenCL SDK must be installed to build liboacc library])])

if test "x$THREADS" == xposix ; then
CFLAGS="$CFLAGS -pthread"
AC_COMPILE_IFELSE(
  [AC_LANG_PROGRAM(
    [#include <pthread.h>
     void *f(void *d) { return NULL; }],
    [pthread_t t; pthread_create(&t,NULL,f,NULL);])],
  [], [AC_MSG_ERROR([Compiler configured to use posix threads but header file is missing])])
fi

CFLAGS="$save_CFLAGS"
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile liboacc.spec])
AC_OUTPUT

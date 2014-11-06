# Plugins for offload execution.
#
# Copyright (C) 2014 Free Software Foundation, Inc.
#
# Contributed by Mentor Embedded.
#
# This file is part of the GNU OpenMP Library (libgomp).
#
# Libgomp is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# Libgomp is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
# more details.
#
# Under Section 7 of GPL version 3, you are granted additional
# permissions described in the GCC Runtime Library Exception, version
# 3.1, as published by the Free Software Foundation.
#
# You should have received a copy of the GNU General Public License and
# a copy of the GCC Runtime Library Exception along with this program;
# see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
# <http://www.gnu.org/licenses/>.

AC_PREREQ([2.64])
AC_INIT([Plugins for GNU OpenMP Runtime Library], [1.0], ,[libgomp-plugins])
AC_CONFIG_HEADER([config.h])

AC_CANONICAL_SYSTEM
target_alias=${target_alias-$host_alias}
AC_SUBST(target_alias)

AM_INIT_AUTOMAKE([1.9.0 foreign no-dist])

AM_MAINTAINER_MODE

AC_PROG_CC
AC_PROG_CXX

AC_MSG_CHECKING([for --enable-version-specific-runtime-libs])
AC_ARG_ENABLE([version-specific-runtime-libs],
  AC_HELP_STRING([--enable-version-specific-runtime-libs],
		 [Specify that runtime libraries should be installed in a compiler-specific directory]),
  [case "$enableval" in
    yes) enable_version_specific_runtime_libs=yes ;;
    no)  enable_version_specific_runtime_libs=no ;;
    *)   AC_MSG_ERROR([Unknown argument to enable/disable version-specific libs]);;
   esac],
  [enable_version_specific_runtime_libs=no])
AC_MSG_RESULT($enable_version_specific_runtime_libs)

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

AC_LIBTOOL_DLOPEN
AM_PROG_LIBTOOL
# Forbid libtool to hardcode RPATH, because we want to be able to specify
# library search directory using LD_LIBRARY_PATH
hardcode_into_libs=no
AC_SUBST(toolexecdir)
AC_SUBST(toolexeclibdir)

libtool_VERSION=1:0:0
AC_SUBST(libtool_VERSION)

# Look for the CUDA driver package.
CUDA_DRIVER_INCLUDE=
CUDA_DRIVER_LIB=
AC_SUBST(CUDA_DRIVER_INCLUDE)
AC_SUBST(CUDA_DRIVER_LIB)
CUDA_DRIVER_CPPFLAGS=
CUDA_DRIVER_LDFLAGS=
AC_ARG_WITH(cuda-driver,
	[AS_HELP_STRING([--with-cuda-driver=PATH],
		[specify prefix directory for installed CUDA driver package.
		 Equivalent to --with-cuda-driver-include=PATH/include
		 plus --with-cuda-driver-lib=PATH/lib])])
AC_ARG_WITH(cuda-driver-include,
	[AS_HELP_STRING([--with-cuda-driver-include=PATH],
		[specify directory for installed CUDA driver include files])])
AC_ARG_WITH(cuda-driver-lib,
	[AS_HELP_STRING([--with-cuda-driver-lib=PATH],
		[specify directory for the installed CUDA driver library])])
if test "x$with_cuda_driver" != x; then
  CUDA_DRIVER_INCLUDE=$with_cuda_driver/include
  CUDA_DRIVER_LIB=$with_cuda_driver/lib
fi
if test "x$with_cuda_driver_include" != x; then
  CUDA_DRIVER_INCLUDE=$with_cuda_driver_include
fi
if test "x$with_cuda_driver_lib" != x; then
  CUDA_DRIVER_LIB=$with_cuda_driver_lib
fi
if test "x$CUDA_DRIVER_INCLUDE" != x; then
  CUDA_DRIVER_CPPFLAGS=-I$CUDA_DRIVER_INCLUDE
fi
if test "x$CUDA_DRIVER_LIB" != x; then
  CUDA_DRIVER_LDFLAGS=-L$CUDA_DRIVER_LIB
fi

PLUGIN_NVPTX=0
PLUGIN_NVPTX_CPPFLAGS=
PLUGIN_NVPTX_LDFLAGS=
PLUGIN_NVPTX_LIBS=
AC_SUBST(PLUGIN_NVPTX)
AC_SUBST(PLUGIN_NVPTX_CPPFLAGS)
AC_SUBST(PLUGIN_NVPTX_LDFLAGS)
AC_SUBST(PLUGIN_NVPTX_LIBS)

for accel in `echo $enable_offload_targets | sed -e 's#,# #g'`; do
  case "$accel" in
    nvptx*)
      PLUGIN_NVPTX=$accel
      PLUGIN_NVPTX_CPPFLAGS=$CUDA_DRIVER_CPPFLAGS
      PLUGIN_NVPTX_LDFLAGS=$CUDA_DRIVER_LDFLAGS
      PLUGIN_NVPTX_LIBS='-lcuda'

      PLUGIN_NVPTX_save_CPPFLAGS=$CPPFLAGS
      CPPFLAGS="$PLUGIN_NVPTX_CPPFLAGS $CPPFLAGS"
      PLUGIN_NVPTX_save_LDFLAGS=$LDFLAGS
      LDFLAGS="$PLUGIN_NVPTX_LDFLAGS $LDFLAGS"
      PLUGIN_NVPTX_save_LIBS=$LIBS
      LIBS="$PLUGIN_NVPTX_LIBS $LIBS"
      AC_LINK_IFELSE(
	[AC_LANG_PROGRAM(
	  [#include "cuda.h"],
	  [CUresult r = cuCtxPushCurrent (NULL);])],
	[PLUGIN_NVPTX=1])
      CPPFLAGS=$PLUGIN_NVPTX_save_CPPFLAGS
      LDFLAGS=$PLUGIN_NVPTX_save_LDFLAGS
      LIBS=$PLUGIN_NVPTX_save_LIBS
      case $PLUGIN_NVPTX in
	nvptx*)
	  PLUGIN_NVPTX=0
	  AC_MSG_ERROR([CUDA driver package required for nvptx support])
	  ;;
      esac
      ;;
  esac
done
AM_CONDITIONAL([PLUGIN_NVPTX], [test $PLUGIN_NVPTX = 1])
AC_DEFINE_UNQUOTED([PLUGIN_NVPTX], [$PLUGIN_NVPTX],
		  [Define to 1 if the NVIDIA plugin is built, 0 if not.])

. ${srcdir}/../configure.tgt

AC_SUBST(config_path)

AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([libgomp-test-support.exp])
AC_OUTPUT

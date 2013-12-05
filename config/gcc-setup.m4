dnl Set up various stuffs for GCC.
AC_DEFUN([GCC_SETUP], [
  dnl Specify the local prefix
  local_prefix=
  AC_ARG_WITH(local-prefix,
  [AS_HELP_STRING([--with-local-prefix=DIR],
		  [specifies directory to put local include])],
  [case "${withval}" in
  yes)	AC_MSG_ERROR(bad value ${withval} given for local include directory prefix) ;;
  no)	;;
  *)	local_prefix=$with_local_prefix ;;
  esac])

  dnl Default local prefix if it is empty
  if test x$local_prefix = x; then
	local_prefix=/usr/local
  fi

  AC_ARG_WITH([native-system-header-dir],
    [  --with-native-system-header-dir=dir
                            use dir as the directory to look for standard
                            system header files in.  Defaults to /usr/include.],
  [
   case ${with_native_system_header_dir} in
   yes|no) AC_MSG_ERROR([bad value ${withval} given for --with-native-system-header-dir]) ;;
   /* | [[A-Za-z]]:[[\\/]]*) ;;
   *) AC_MSG_ERROR([--with-native-system-header-dir argument ${withval} must be an absolute directory]) ;;
   esac
   configured_native_system_header_dir="${withval}"
  ], [configured_native_system_header_dir=])

  AC_ARG_WITH(build-sysroot,
    [AS_HELP_STRING([--with-build-sysroot=sysroot],
                    [use sysroot as the system root during the build])],
    [if test x"$withval" != x ; then
       SYSROOT_CFLAGS_FOR_TARGET="--sysroot=$withval"
     fi],
    [SYSROOT_CFLAGS_FOR_TARGET=])

  if test "x$prefix" = xNONE; then
   test_prefix=/usr/local
  else
   test_prefix=$prefix
  fi
  if test "x$exec_prefix" = xNONE; then
   test_exec_prefix=$test_prefix
  else
   test_exec_prefix=$exec_prefix
  fi

  AC_ARG_WITH(sysroot,
  [AS_HELP_STRING([[--with-sysroot[=DIR]]],
		  [search for usr/lib, usr/include, et al, within DIR])],
  [
   case ${with_sysroot} in
   yes) TARGET_SYSTEM_ROOT='${exec_prefix}/${target_noncanonical}/sys-root' ;;
   *) TARGET_SYSTEM_ROOT=$with_sysroot ;;
   esac

   TARGET_SYSTEM_ROOT_DEFINE='-DTARGET_SYSTEM_ROOT=\"$(TARGET_SYSTEM_ROOT)\"'
   CROSS_SYSTEM_HEADER_DIR='$(TARGET_SYSTEM_ROOT)$${sysroot_headers_suffix}$(NATIVE_SYSTEM_HEADER_DIR)'

   case ${TARGET_SYSTEM_ROOT} in
   "${test_prefix}"|"${test_prefix}/"*|\
   "${test_exec_prefix}"|"${test_exec_prefix}/"*|\
   '${prefix}'|'${prefix}/'*|\
   '${exec_prefix}'|'${exec_prefix}/'*)
     t="$TARGET_SYSTEM_ROOT_DEFINE -DTARGET_SYSTEM_ROOT_RELOCATABLE"
     TARGET_SYSTEM_ROOT_DEFINE="$t"
     ;;
   esac
  ], [
   TARGET_SYSTEM_ROOT=
   TARGET_SYSTEM_ROOT_DEFINE=
   CROSS_SYSTEM_HEADER_DIR='$(gcc_tooldir)/sys-include'
  ])

  if test -n "$configured_native_system_header_dir"; then
    native_system_header_dir=$configured_native_system_header_dir
  else
    native_system_header_dir=/usr/include

    case $target in
    i[34567]86-pc-msdosdjgpp*)
      native_system_header_dir=/dev/env/DJDIR/include
      ;;
    i[34567]86-*-mingw* | x86_64-*-mingw*)
      native_system_header_dir=/mingw/include
      ;;
    *-*-linux* | *-*-kfreebsd*-gnu | *-*-knetbsd*-gnu | *-*-kopensolaris*-gnu)
      ;;
    *-*-gnu* | spu-*-elf*)
      native_system_header_dir=/include
      ;;
    esac
  fi
  NATIVE_SYSTEM_HEADER_DIR="$native_system_header_dir"

  dnl Define macro CROSS_DIRECTORY_STRUCTURE in compilation if this is a
  dnl cross-compiler which does not use the native headers and libraries.
  dnl Also use all.cross instead of all.internal and adjust SYSTEM_HEADER_DIR.
  GCC_CROSS=
  GCC_ALL=all.internal
  SYSTEM_HEADER_DIR='$(NATIVE_SYSTEM_HEADER_DIR)'

  if test "x$with_build_sysroot" != x; then
    build_system_header_dir=$with_build_sysroot'$${sysroot_headers_suffix}$(NATIVE_SYSTEM_HEADER_DIR)'
  else
    dnl This value is used, even on a native system, because
    dnl CROSS_SYSTEM_HEADER_DIR is just
    dnl $(TARGET_SYSTEM_ROOT)$(NATIVE_SYSTEM_HEADER_DIR).
    build_system_header_dir='$(CROSS_SYSTEM_HEADER_DIR)'
  fi

  if test x$host != x$target; then
	GCC_CROSS="-DCROSS_DIRECTORY_STRUCTURE"
	GCC_ALL=all.cross
	SYSTEM_HEADER_DIR=$build_system_header_dir
	case "$host","$target" in
	dnl Darwin crosses can use the host system's libraries and headers,
	dnl because of the fat library support.  Of course, it must be the
	dnl same version of Darwin on both sides.  Allow the user to
	dnl just say --target=foo-darwin without a version number to mean
	dnl "the version on this system".
	    *-*-darwin*,*-*-darwin*)
		hostos=`echo $host | sed 's/.*-darwin/darwin/'`
		targetos=`echo $target | sed 's/.*-darwin/darwin/'`
		if test $hostos = $targetos -o $targetos = darwin ; then
		    GCC_CROSS=
		    SYSTEM_HEADER_DIR='$(NATIVE_SYSTEM_HEADER_DIR)'
		    with_headers=yes
		fi
		;;

	    i?86-*-*,x86_64-*-* \
	    | powerpc*-*-*,powerpc64*-*-*)
		GCC_CROSS="$CROSS -DNATIVE_CROSS" ;;
	esac

	case $target in
		*-*-mingw*)
			if test "x$with_headers" = x; then
				with_headers=yes
			fi
			;;
		*)
			;;
	esac
  elif test "x$TARGET_SYSTEM_ROOT" != x; then
        SYSTEM_HEADER_DIR=$build_system_header_dir
  fi

  if test x$host != x$target || test "x$TARGET_SYSTEM_ROOT" != x; then
    if test "x$with_headers" != x; then
      target_header_dir=$with_headers
    elif test "x$with_sysroot" = x; then
      target_header_dir="${test_exec_prefix}/${target_noncanonical}/sys-include"
    elif test "x$with_build_sysroot" != "x"; then
      target_header_dir="${with_build_sysroot}${native_system_header_dir}"
    elif test "x$with_sysroot" = xyes; then
      target_header_dir="${test_exec_prefix}/${target_noncanonical}/sys-root${native_system_header_dir}"
    else
      target_header_dir="${with_sysroot}${native_system_header_dir}"
    fi
  else
    target_header_dir=${native_system_header_dir}
  fi

  dnl Find default assembler

  dnl With GNU as
  AC_ARG_WITH(gnu-as,
  [AS_HELP_STRING([--with-gnu-as], [arrange to work with GNU as])],
   gas_flag="$with_gnu_as",
   gas_flag=no)

  AC_ARG_WITH(as,
  [AS_HELP_STRING([--with-as], [arrange to use the specified as (full pathname)])],
   DEFAULT_ASSEMBLER="$with_as")
  if test x"${DEFAULT_ASSEMBLER+set}" = x"set"; then
    if test ! -x "$DEFAULT_ASSEMBLER"; then
      AC_MSG_ERROR([cannot execute: $DEFAULT_ASSEMBLER: check --with-as or env. var. DEFAULT_ASSEMBLER])
    elif $DEFAULT_ASSEMBLER -v < /dev/null 2>&1 | grep GNU > /dev/null; then
      gas_flag=yes
    fi
  fi

  AC_MSG_CHECKING([whether a default assembler was specified])
  if test x"${DEFAULT_ASSEMBLER+set}" = x"set"; then
    if test x"$gas_flag" = x"no"; then
      AC_MSG_RESULT([yes ($DEFAULT_ASSEMBLER)])
    else
      AC_MSG_RESULT([yes ($DEFAULT_ASSEMBLER - GNU as)])
    fi
  else
    AC_MSG_RESULT(no)
  fi

  dnl Identify the assembler which will work hand-in-glove with the newly
  dnl built GCC, so that we can examine its features.  This is the assembler
  dnl which will be driven by the driver program.

  dnl If build != host, and we aren't building gas in-tree, we identify a
  dnl  build->target assembler and hope that it will have the same features
  dnl as the host->target assembler we'll be using.
  gcc_cv_gas_major_version=
  gcc_cv_gas_minor_version=
  gcc_cv_gas_patch_version=
  gcc_cv_as_gas_srcdir=`echo $srcdir | sed -e 's,/gcc$,,'`/gas

  m4_pattern_allow([AS_FOR_TARGET])dnl
  AS_VAR_SET_IF(gcc_cv_as,, [
  if test -x "$DEFAULT_ASSEMBLER"; then
	gcc_cv_as="$DEFAULT_ASSEMBLER"
  elif test -f $gcc_cv_as_gas_srcdir/configure.in \
       && test -f ../gas/Makefile \
       && test x$build = x$host; then
	  gcc_cv_as=../gas/as-new$build_exeext
  elif test -x as$build_exeext; then
	dnl Build using assembler in the current directory.
	gcc_cv_as=./as$build_exeext
  elif ( set dummy $AS_FOR_TARGET; test -x $[2] ); then
        gcc_cv_as="$AS_FOR_TARGET"
  elif test "$AS_FOR_TARGET" = "\$(AS)"; then
        gcc_cv_as="$AS"
  else
        AC_PATH_PROG(gcc_cv_as, $AS_FOR_TARGET)
  fi])

  AC_MSG_CHECKING(what assembler to use)
  if test "$gcc_cv_as" = ../gas/as-new$build_exeext; then
    dnl Single tree build which includes gas.  We want to prefer it
    dnl over whatever linker top-level may have detected, since
    dnl we'll use what we're building after installation anyway.
    AC_MSG_RESULT(newly built gas)
    in_tree_gas=yes
    gcc_cv_as_bfd_srcdir=`echo $srcdir | sed -e 's,/gcc$,,'`/bfd
    for f in $gcc_cv_as_bfd_srcdir/configure \
	     $gcc_cv_as_gas_srcdir/configure \
	     $gcc_cv_as_gas_srcdir/configure.in \
	     $gcc_cv_as_gas_srcdir/Makefile.in ; do
      gcc_cv_gas_version=`sed -n -e 's/^[[ 	]]*VERSION=[[^0-9A-Za-z_]]*\([[0-9]]*\.[[0-9]]*.*\)/VERSION=\1/p' < $f`
      if test x$gcc_cv_gas_version != x; then
        break
      fi
    done
    case $gcc_cv_gas_version in
    VERSION=[[0-9]]*) ;;
    *) AC_MSG_ERROR([[cannot find version of in-tree assembler]]);;
    esac
    gcc_cv_gas_major_version=`expr "$gcc_cv_gas_version" : "VERSION=\([[0-9]]*\)"`
    gcc_cv_gas_minor_version=`expr "$gcc_cv_gas_version" : "VERSION=[[0-9]]*\.\([[0-9]]*\)"`
    gcc_cv_gas_patch_version=`expr "$gcc_cv_gas_version" : "VERSION=[[0-9]]*\.[[0-9]]*\.\([[0-9]]*\)"`
    in_tree_gas_is_elf=no
    if grep 'obj_format = elf' ../gas/Makefile > /dev/null \
       || (grep 'obj_format = multi' ../gas/Makefile \
           && grep 'extra_objects =.* obj-elf' ../gas/Makefile) > /dev/null
    then
      in_tree_gas_is_elf=yes
    fi
  else
    AC_MSG_RESULT($gcc_cv_as)
    in_tree_gas=no
    as_ver=`$gcc_cv_as --version 2>/dev/null | sed 1q`
    if echo "$as_ver" | grep GNU > /dev/null; then
      changequote(,)dnl
      as_vers=`echo $as_ver | sed -n \
	  -e 's,^.*[	 ]\([0-9][0-9]*\.[0-9][0-9]*.*\)$,\1,p'`
      gcc_cv_gas_major_version=`expr "$as_vers" : '\([0-9]*\)'`
      gcc_cv_gas_minor_version=`expr "$as_vers" : '[0-9]*\.\([0-9]*\)'`
      gcc_cv_gas_patch_version=`expr "$as_vers" : '[0-9]*\.[0-9]*\.\([0-9]*\)'`
      changequote([,])dnl
    fi
  fi

  if test x$gcc_cv_gas_major_version != x; then
    AC_MSG_CHECKING([for GNU binutils version])
    case $gcc_cv_gas_patch_version in
    "") gcc_cv_gas_patch_version="0" ;;
    esac
    gcc_cv_gas_vers=`expr \( \( $gcc_cv_gas_major_version \* 1000 \) \
			    + $gcc_cv_gas_minor_version \) \* 1000 \
			    + $gcc_cv_gas_patch_version`
    AC_MSG_RESULT([$gcc_cv_gas_major_version.$gcc_cv_gas_minor_version.$gcc_cv_gas_patch_version])
  fi

  dnl Determine the version of glibc, if any, used on the target.
  AC_REQUIRE([AC_PROG_EGREP])
  AC_MSG_CHECKING([for target glibc version])
  AC_ARG_WITH([glibc-version],
    [AS_HELP_STRING([--with-glibc-version=M.N],
      [assume GCC used with glibc version M.N or later])], [
if [echo "$with_glibc_version" | grep '^[0-9][0-9]*\.[0-9][0-9]*$']; then
  glibc_version_major=`echo "$with_glibc_version" | sed -e 's/\..*//'`
  glibc_version_minor=`echo "$with_glibc_version" | sed -e 's/.*\.//'`
else
  AC_MSG_ERROR([option --with-glibc-version requires a version number M.N])
fi], [
glibc_version_major=0
glibc_version_minor=0
[if test -f $target_header_dir/features.h \
  && glibc_version_major_define=`$EGREP '^[ 	]*#[ 	]*define[ 	]+__GLIBC__[ 	]+[0-9]' $target_header_dir/features.h` \
  && glibc_version_minor_define=`$EGREP '^[ 	]*#[ 	]*define[ 	]+__GLIBC_MINOR__[ 	]+[0-9]' $target_header_dir/features.h`; then
  glibc_version_major=`echo "$glibc_version_major_define" | sed -e 's/.*__GLIBC__[ 	]*//'`
  glibc_version_minor=`echo "$glibc_version_minor_define" | sed -e 's/.*__GLIBC_MINOR__[ 	]*//'`
fi]])
  AC_MSG_RESULT([$glibc_version_major.$glibc_version_minor])

  dnl Determine the version of Linux kernel, if any, used on the target.
  AC_REQUIRE([AC_PROG_AWK])
  AC_REQUIRE([AC_PROG_GREP])
  AC_MSG_CHECKING([for target Linux kernel version])
  AC_ARG_WITH([kernel-version],
    [AS_HELP_STRING([--with-kernel-version=M.N.P],
      [assume GCC used with Linux kernel version M.N.P or later])], [
if [echo "$with_kernel_version" | grep '^[0-9][0-9]*\.[0-9][0-9]*\.[0-9][0-9]*$']; then
  kernel_version_major=`echo "$with_kernel_version" | sed -e '/\([0-9]*\)\.\([0-9]*\)\.\([0-9]*\).*/\1/'`
  kernel_version_minor=`echo "$with_kernel_version" | sed -e '/\([0-9]*\)\.\([0-9]*\)\.\([0-9]*\).*/\2/'`
  kernel_version_patch=`echo "$with_kernel_version" | sed -e '/\([0-9]*\)\.\([0-9]*\)\.\([0-9]*\).*/\3/'`
else
  AC_MSG_ERROR([option --with-kernel-version requires a version number M.N.P])
fi], [
kernel_version_major=0
kernel_version_minor=0
kernel_version_patch=0
[if test -f $target_header_dir/linux/version.h \
  && kernel_version_code=`$GREP LINUX_VERSION_CODE $target_header_dir/linux/version.h | $AWK '{ print $ 3 }'`; then
  kernel_version_major=`echo "$kernel_version_code" | $AWK '{ print and (rshift ($ 1, 16), 0xff) }'`
  kernel_version_minor=`echo "$kernel_version_code" | $AWK '{ print and (rshift ($ 1, 8), 0xff) }'`
  kernel_version_patch=`echo "$kernel_version_code" | $AWK '{ print and ($ 1, 0xff) }'`
fi]])
  AC_MSG_RESULT([$kernel_version_major.$kernel_version_minor.$kernel_version_patch])
])

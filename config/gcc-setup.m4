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

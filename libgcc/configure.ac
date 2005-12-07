dnl Process this file with autoconf to produce a configure script.

sinclude(../config/acx.m4)
sinclude(../config/no-executables.m4)

AC_PREREQ(2.59)
AC_INIT([GNU C Runtime Library], 1.0,,[libgcc])
AC_CONFIG_SRCDIR([static-object.mk])

AC_ARG_WITH(target-subdir,
[  --with-target-subdir=SUBDIR      Configuring in a subdirectory for target])
AC_ARG_WITH(cross-host,
[  --with-cross-host=HOST           Configuring with a cross compiler])

if test "${srcdir}" = "."; then
  if test -n "${with_build_subdir}"; then
    libgcc_topdir="${srcdir}/../.."
    with_target_subdir=
  elif test -z "${with_target_subdir}"; then
    libgcc_topdir="${srcdir}/.."
  else
    if test "${with_target_subdir}" != "."; then
      libgcc_topdir="${srcdir}/${with_multisrctop}../.."
    else
      libgcc_topdir="${srcdir}/${with_multisrctop}.."
    fi
  fi
else
  libgcc_topdir="${srcdir}/.."
fi
AC_SUBST(libgcc_topdir)
AC_CONFIG_AUX_DIR($libgcc_topdir)

AC_ARG_ENABLE(shared,
[  --disable-shared        don't provide a shared libgcc],
[
  case $enable_shared in
  yes | no) ;;
  *)
    enable_shared=no
    IFS="${IFS= 	}"; ac_save_ifs="$IFS"; IFS="${IFS}:,"
    for pkg in $enableval; do
      if test "X$pkg" = "Xgcc" || test "X$pkg" = "Xlibgcc"; then
	enable_shared=yes
      fi
    done
    IFS="$ac_save_ifs"
    ;;
  esac
], [enable_shared=yes])
AC_SUBST(enable_shared)

AC_ARG_WITH(slibdir,
[  --with-slibdir=DIR      shared libraries in DIR [LIBDIR]],
slibdir="$with_slibdir",
if test "${enable_version_specific_runtime_libs+set}" = set; then
  slibdir='$(libsubdir)'
elif test "$host" != "$target"; then
  slibdir='$(build_tooldir)/lib'
else
  slibdir='$(libdir)'
fi)
AC_SUBST(slibdir)

AC_PROG_INSTALL

AC_PROG_AWK
# We need awk; bail out if it's missing.
case ${AWK} in
  "") AC_MSG_ERROR([can't build without awk, bailing out]) ;;
esac

AC_CANONICAL_HOST
ACX_NONCANONICAL_HOST

dnl These must be called before AM_PROG_LIBTOOL, because it may want
dnl to call AC_CHECK_PROG.
AC_CHECK_TOOL(AR, ar)
AC_CHECK_TOOL(NM, nm)
AC_CHECK_TOOL(RANLIB, ranlib, :)

GCC_NO_EXECUTABLES
AC_PROG_CC
AC_PROG_CPP_WERROR

# Collect host-machine-specific information.
. ${srcdir}/config.gcc

# Conditionalize the makefile for this target machine.
hmake_file_=
for f in ${hmake_file}
do
	if test -f ${srcdir}/config/$f
	then
		hmake_file_="${hmake_file_} \$(srcdir)/config/$f"
	fi
done
hmake_file="${hmake_file_}"
AC_SUBST(hmake_file)

# We need multilib support.
AC_CONFIG_FILES([Makefile])
AC_CONFIG_COMMANDS([default],
  [[test -z "$CONFIG_HEADERS" || echo timestamp > stamp-h
if test -n "$CONFIG_FILES"; then
  # FIXME: We shouldn't need to set ac_file
  ac_file=Makefile
  . ${libgcc_topdir}/config-ml.in
fi]],
[[srcdir=${srcdir}
host=${host}
target=${target}
with_target_subdir=${with_target_subdir}
with_multisubdir=${with_multisubdir}
ac_configure_args="--enable-multilib ${ac_configure_args}"
CONFIG_SHELL=${CONFIG_SHELL-/bin/sh}
libgcc_topdir=${libgcc_topdir}
CC="${CC}"
]])
AC_OUTPUT

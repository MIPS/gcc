# Configure script for gcc/ada/Makefile.in, which builds libada,
# and gcc/ada/Make-tools.in, which builds gnattools.

#Copyright 1994-2004 Free Software Foundation, Inc.

#This file is part of GCC.

#GCC is free software; you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation; either version 2, or (at your option)
#any later version.

#GCC is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.

#You should have received a copy of the GNU General Public License
#along with GCC; see the file COPYING.  If not, write to
#the Free Software Foundation, 59 Temple Place - Suite 330,
#Boston, MA 02111-1307, USA.

# Eventually, this should migrate to the appropriate directories.
# There's a certain amount of gunk because it hasn't yet.

AC_PREREQ(2.59)
AC_INIT
AC_CONFIG_SRCDIR(calendar.ads)
AC_CONFIG_FILES(Makefile Make-tools)

AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_CANONICAL_TARGET

# Converted from hideous Make-isms by Nathanael Nerode.

# TRACE and THREAD_KIND; we expect these to be passed down from above,
# for now (
case "${TRACE}" in
  "")
    TRACE=no
    ;;
esac
case "${THREAD_KIND}" in
  "")
    THREAD_KIND=native
    ;;
esac

# Programs, extensions, VMS, etc.
# -------------------------------
# Default .so extension is .so, but not on some systems
soext='.so'
AC_SUBST(soext)
# VMS doesn't like hyphens, and has other quirks
hyphen='-'
AC_SUBST(hyphen)
vms_host=no
AC_SUBST(vms_host)

# AR is usually AR, but different on ia64-vms.
# This should be autoconfiscated to use, e.g., AC_PROG_AR.
AC_SUBST(AR)
AC=ar

# Things ported from GCC's configure.ac
AC_SUBST(CROSS)
CROSS=
if test x$host != x$target ; then
  CROSS="-DCROSS_COMPILE"
fi

AC_PROG_RANLIB
case "${host}" in
*-*-darwin*)
  # By default, the Darwin ranlib will not treat common symbols as
  # definitions when  building the archive table of contents.  Other
  # ranlibs do that; pass an option to the Darwin ranlib that makes
  # it behave similarly.
  ranlib_flags="-c"
  ;;
*)
  ranlib_flags=""
esac
AC_SUBST(ranlib_flags)

# When building Ada code on Alpha, we need exc_resume which is usually in
# -lexc.  So test for it.
save_LIBS="$LIBS"
LIBS=
AC_SEARCH_LIBS(exc_resume, exc)
GNAT_LIBEXC="$LIBS"
LIBS="$save_LIBS"
AC_SUBST(GNAT_LIBEXC)

# GNAT file choice defaults
# -------------------------

# LIBGNAT_TARGET_PAIRS is a list of pairs of filenames.
# The members of each pair must be separated by a '<' and no whitespace.
# Each pair must be separated by some amount of whitespace from the following
# pair.
AC_SUBST(LIBGNAT_TARGET_PAIRS)

# Non-tasking case:
LIBGNAT_TARGET_PAIRS="a-intnam.ads<a-intnam-dummy.ads \
s-inmaop.adb<s-inmaop-dummy.adb \
s-intman.adb<s-intman-dummy.adb \
s-osinte.ads<s-osinte-dummy.ads \
s-osprim.adb<s-osprim-posix.adb \
s-taprop.adb<s-taprop-dummy.adb \
s-taspri.ads<s-taspri-dummy.ads"

# Default shared object option. Note that we rely on the fact that the "soname"
# option will always be present and last in this flag, so that we can have
# $(SO_OPTS)libgnat-x.xx
AC_SUBST(SO_OPTS)
SO_OPTS="-Wl,-soname,"

# Default gnatlib-shared target.
# By default, equivalent to gnatlib.
# Set to gnatlib-shared-default, gnatlib-shared-dual, or a platform specific
# target when supported.
AC_SUBST([GNATLIB_SHARED])
GNATLIB_SHARED=gnatlib

# default value for gnatmake's target dependent file
AC_SUBST(MLIB_TGT)
MLIB_TGT='mlib-tgt'

# By default, do not distribute prefix.o (in libgccprefix), since it is only
# needed by external GNAT tools such as gnatdist and Glide.
# Override this variable on native platforms when needed.
AC_SUBST(PREFIX_OBJS)
PREFIX_OBJS=

# Library version, extracted from sources
lib_version=`grep ' Library_version : ' ${srcdir}/gnatvsn.ads \
             | sed -e 's/.*"\(.*\)".*/\1/'`
AC_SUBST(LIBRARY_VERSION)

# More stuff overridden by specific targets, with blank defaults
THREADSLIB=
AC_SUBST(THREADSLIB)
MISCLIB=
AC_SUBST(MISCLIB)
SYMLIB=
AC_SUBST(SYMLIB)
GMEM_LIB=
AC_SUBST(GMEM_LIB)
TGT_LIB=
AC_SUBST(TGT_LIB)

EXTRA_GNATRTL_NONTASKING_OBJS=
AC_SUBST(EXTRA_GNATRTL_NONTASKING_OBJS)
EXTRA_GNATRTL_TASKING_OBJS=
AC_SUBST(EXTRA_GNATRTL_TASKING_OBJS)
EXTRA_LIBGNAT_SRCS=
AC_SUBST(EXTRA_LIBGNAT_SRCS)
EXTRA_LIBGNAT_OBJS=
AC_SUBST(EXTRA_LIBGNAT_OBJS)
EXTRA_GNATMAKE_OBJS=
AC_SUBST(EXTRA_GNATMAKE_OBJS)

# Per-host case statement
# -----------------------

case "${host}" in
  alpha*-dec-vms* | alpha*-dec-openvms* | alpha*-dec-vms* \
  | alpha*-hp-vms* | alpha*-hp-openvms* | alpha*-hp-vms* \
  | ia64*-dec-vms* | ia64*-dec-openvms* | ia64*-dec-vms* \
  | ia64*-hp-vms* | ia64*-hp-openvms* | ia64*-hp-vms*)
    vms_host=yes
    soext='.exe'
    hyphen='_'
    case ${target} in
      ia64-hp-vms* | ia64-hp-openvms*)
        AR=iar
      ;;
    esac
    ;;
esac

# Per-target case statement (very large)
# -------------------------
case "${target}" in
*86-*-os2 | *86-*-OS2 | *86-*-os2_emx)
  LIBGNAT_TARGET_PAIRS="\
  a-excpol.adb<a-excpol-abort.adb \
  a-intnam.ads<a-intnam-dummy.ads \
  a-numaux.adb<a-numaux-x86.adb \
  a-numaux.ads<a-numaux-x86.ads \
  s-inmaop.adb<s-inmaop-dummy.adb \
  s-interr.adb<s-interr-dummy.adb \
  s-intman.adb<s-intman-dummy.adb \
  s-mastop.adb<s-mastop-x86.adb \
  s-osinte.adb<s-osinte-os2.adb \
  s-osinte.ads<s-osinte-os2.ads \
  s-osprim.adb<s-osprim-os2.adb \
  s-parame.adb<s-parame-os2.adb \
  system.ads<system-os2.ads \
  s-taprop.adb<s-taprop-os2.adb \
  s-taspri.ads<s-taspri-os2.ads"

  EXTRA_GNATRTL_NONTASKING_OBJS="\
  i-os2err.o \
  i-os2lib.o \
  i-os2syn.o \
  i-os2thr.o"
  ;;

*86-*-interix*)

  LIBGNAT_TARGET_PAIRS="\
  a-excpol.adb<a-excpol-interix.adb \
  a-intnam.ads<a-intnam-interix.ads \
  a-numaux.adb<a-numaux-x86.adb \
  a-numaux.ads<a-numaux-x86.ads \
  g-soccon.ads<g-soccon-interix.ads \
  s-inmaop.adb<s-inmaop-posix.adb \
  s-intman.adb<s-intman-posix.adb \
  s-mastop.adb<s-mastop-x86.adb \
  s-osinte.adb<s-osinte-fsu.adb \
  s-osinte.ads<s-osinte-interix.ads \
  s-osprim.adb<s-osprim-unix.adb \
  s-taprop.adb<s-taprop-posix.adb \
  system.ads<system-interix.ads \
  s-taspri.ads<s-taspri-posix.ads \
  s-tpopsp.adb<s-tpopsp-posix.adb"

  THREADSLIB="-lgthreads -lmalloc"
  PREFIX_OBJS='$(PREFIX_REAL_OBJS)'
  ;;

# sysv5uw is SCO UnixWare 7
*86-*-sysv5uw*)
  LIBGNAT_TARGET_PAIRS="\
  a-excpol.adb<a-excpol-abort.adb \
  a-intnam.ads<a-intnam-unixware.ads \
  a-numaux.adb<a-numaux-x86.adb \
  a-numaux.ads<a-numaux-x86.ads \
  s-inmaop.adb<s-inmaop-posix.adb \
  s-intman.adb<s-intman-posix.adb \
  s-mastop.adb<s-mastop-x86.adb \
  s-osinte.ads<s-osinte-unixware.ads \
  s-osinte.adb<s-osinte-unixware.adb \
  s-osprim.adb<s-osprim-unix.adb \
  s-taprop.adb<s-taprop-posix.adb \
  s-taspri.ads<s-taspri-posix.ads \
  s-tpopsp.adb<s-tpopsp-posix-foreign.adb \
  system.ads<system-unixware.ads \
  g-soccon.ads<g-soccon-unixware.ads \
  g-soliop.ads<g-soliop-unixware.ads"

  THREADSLIB="-lthread"
  PREFIX_OBJS='$(PREFIX_REAL_OBJS)'
  SO_OPTS="-Wl,-h,"
  GNATLIB_SHARED="gnatlib-shared-dual"
  LIBRARY_VERSION="${lib_version}"
  ;;

# vxworks on alpha
alpha*-dec-vx*)
  LIBGNAT_TARGET_PAIRS="\
  a-sytaco.ads<a-sytaco-vxworks.ads \
  a-sytaco.adb<a-sytaco-vxworks.adb \
  a-intnam.ads<a-intnam-vxworks.ads \
  a-numaux.ads<a-numaux-vxworks.ads \
  s-inmaop.adb<s-inmaop-posix.adb \
  s-interr.adb<s-interr-vxworks.adb \
  s-intman.ads<s-intman-vxworks.ads \
  s-intman.adb<s-intman-vxworks.adb \
  s-osinte.adb<s-osinte-vxworks.adb \
  s-osinte.ads<s-osinte-vxworks.ads \
  s-osprim.adb<s-osprim-vxworks.adb \
  s-parame.ads<s-parame-vxworks.ads \
  s-stchop.adb<s-stchop-vxworks.adb \
  s-taprop.adb<s-taprop-vxworks.adb \
  s-tpopsp.adb<s-tpopsp-vxworks.adb \
  s-taspri.ads<s-taspri-vxworks.ads \
  s-vxwork.ads<s-vxwork-alpha.ads \
  g-soccon.ads<g-soccon-vxworks.ads \
  g-socthi.ads<g-socthi-vxworks.ads \
  g-socthi.adb<g-socthi-vxworks.adb \
  system.ads<system-vxworks-alpha.ads"

  EXTRA_GNATRTL_NONTASKING_OBJS="i-vxwork.o i-vxwoio.o"
  EXTRA_GNATRTL_TASKING_OBJS="s-vxwork.o"
  ;;

# m68k vxworks
m68k*-wrs-vx*)
  LIBGNAT_TARGET_PAIRS="\
  a-sytaco.ads<a-sytaco-vxworks.ads \
  a-sytaco.adb<a-sytaco-vxworks.adb \
  a-intnam.ads<a-intnam-vxworks.ads \
  a-numaux.ads<a-numaux-vxworks.ads \
  s-inmaop.adb<s-inmaop-posix.adb \
  s-interr.adb<s-interr-vxworks.adb \
  s-intman.ads<s-intman-vxworks.ads \
  s-intman.adb<s-intman-vxworks.adb \
  s-osinte.adb<s-osinte-vxworks.adb \
  s-osinte.ads<s-osinte-vxworks.ads \
  s-osprim.adb<s-osprim-vxworks.adb \
  s-parame.ads<s-parame-vxworks.ads \
  s-stchop.adb<s-stchop-vxworks.adb \
  s-taprop.adb<s-taprop-vxworks.adb \
  s-taspri.ads<s-taspri-vxworks.ads \
  s-tpopsp.adb<s-tpopsp-vxworks.adb \
  s-vxwork.ads<s-vxwork-m68k.ads \
  g-soccon.ads<g-soccon-vxworks.ads \
  g-socthi.ads<g-socthi-vxworks.ads \
  g-socthi.adb<g-socthi-vxworks.adb \
  system.ads<system-vxworks-m68k.ads"

  EXTRA_GNATRTL_NONTASKING_OBJS="i-vxwork.o i-vxwoio.o"
  EXTRA_GNATRTL_TASKING_OBJS="s-vxwork.o"

  case "${TRACE}" in
    yes)
      LIBGNAT_TARGET_PAIRS="${LIBGNAT_TARGET_PAIRS} \
      s-traces.adb<s-traces-default.adb \
      s-trafor.adb<s-trafor-default.adb \
      s-trafor.ads<s-trafor-default.ads \
      s-tratas.adb<s-tratas-default.adb \
      s-tfsetr.adb<s-tfsetr-vxworks.adb"
      ;;
  esac
  ;;

# powerpc vxworks
powerpc*-wrs-vxworks)
  LIBGNAT_TARGET_PAIRS="\
  a-sytaco.ads<a-sytaco-vxworks.ads \
  a-sytaco.adb<a-sytaco-vxworks.adb \
  a-intnam.ads<a-intnam-vxworks.ads \
  a-numaux.ads<a-numaux-vxworks.ads \
  s-inmaop.adb<s-inmaop-posix.adb \
  s-interr.adb<s-interr-vxworks.adb \
  s-intman.ads<s-intman-vxworks.ads \
  s-intman.adb<s-intman-vxworks.adb \
  s-osinte.adb<s-osinte-vxworks.adb \
  s-osinte.ads<s-osinte-vxworks.ads \
  s-osprim.adb<s-osprim-vxworks.adb \
  s-parame.ads<s-parame-vxworks.ads \
  s-stchop.adb<s-stchop-vxworks.adb \
  s-taprop.adb<s-taprop-vxworks.adb \
  s-taspri.ads<s-taspri-vxworks.ads \
  s-tpopsp.adb<s-tpopsp-vxworks.adb \
  s-vxwork.ads<s-vxwork-ppc.ads \
  g-soccon.ads<g-soccon-vxworks.ads \
  g-socthi.ads<g-socthi-vxworks.ads \
  g-socthi.adb<g-socthi-vxworks.adb \
  system.ads<system-vxworks-ppc.ads"

  case "${TRACE}" in
    yes)
      LIBGNAT_TARGET_PAIRS="${LIBGNAT_TARGET_PAIRS} \
      s-traces.adb<s-traces-default.adb \
      s-trafor.adb<s-trafor-default.adb \
      s-trafor.ads<s-trafor-default.ads \
      s-tratas.adb<s-tratas-default.adb \
      s-tfsetr.adb<s-tfsetr-vxworks.adb"
    ;;
  esac

  EXTRA_GNATRTL_NONTASKING_OBJS="i-vxwork.o i-vxwoio.o s-vxwexc.o"
  EXTRA_GNATRTL_TASKING_OBJS="s-vxwork.o"
  ;;

# sparc vxworks
sparc*-wrs-vx*)
  LIBGNAT_TARGET_PAIRS="\
  a-sytaco.ads<a-sytaco-vxworks.ads \
  a-sytaco.adb<a-sytaco-vxworks.adb \
  a-intnam.ads<a-intnam-vxworks.ads \
  a-numaux.ads<a-numaux-vxworks.ads \
  s-inmaop.adb<s-inmaop-posix.adb \
  s-interr.adb<s-interr-vxworks.adb \
  s-intman.ads<s-intman-vxworks.ads \
  s-intman.adb<s-intman-vxworks.adb \
  s-osinte.adb<s-osinte-vxworks.adb \
  s-osinte.ads<s-osinte-vxworks.ads \
  s-osprim.adb<s-osprim-vxworks.adb \
  s-parame.ads<s-parame-vxworks.ads \
  s-stchop.adb<s-stchop-vxworks.adb \
  s-taprop.adb<s-taprop-vxworks.adb \
  s-taspri.ads<s-taspri-vxworks.ads \
  s-tpopsp.adb<s-tpopsp-vxworks.adb \
  s-vxwork.ads<s-vxwork-sparcv9.ads \
  g-soccon.ads<g-soccon-vxworks.ads \
  g-socthi.ads<g-socthi-vxworks.ads \
  g-socthi.adb<g-socthi-vxworks.adb \
  system.ads<system-vxworks-sparcv9.ads"

  EXTRA_GNATRTL_NONTASKING_OBJS="i-vxwork.o i-vxwoio.o"
  EXTRA_GNATRTL_TASKING_OBJS="s-vxwork.o"
  ;;

# x86 vxworks
*86-wrs-vxworks)
  LIBGNAT_TARGET_PAIRS="\
  a-sytaco.ads<a-sytaco-vxworks.ads \
  a-sytaco.adb<a-sytaco-vxworks.adb \
  a-intnam.ads<a-intnam-vxworks.ads \
  i-vxwork.ads<i-vxwork-x86.ads \
  s-inmaop.adb<s-inmaop-posix.adb \
  s-interr.adb<s-interr-vxworks.adb \
  s-intman.ads<s-intman-vxworks.ads \
  s-intman.adb<s-intman-vxworks.adb \
  a-numaux.adb<a-numaux-x86.adb \
  a-numaux.ads<a-numaux-x86.ads \
  s-osinte.adb<s-osinte-vxworks.adb \
  s-osinte.ads<s-osinte-vxworks.ads \
  s-osprim.adb<s-osprim-vxworks.adb \
  s-parame.ads<s-parame-vxworks.ads \
  s-stchop.adb<s-stchop-vxworks.adb \
  s-taprop.adb<s-taprop-vxworks.adb \
  s-taspri.ads<s-taspri-vxworks.ads \
  s-tpopsp.adb<s-tpopsp-vxworks.adb \
  s-vxwork.ads<s-vxwork-x86.ads \
  g-soccon.ads<g-soccon-vxworks.ads \
  g-socthi.ads<g-socthi-vxworks.ads \
  g-socthi.adb<g-socthi-vxworks.adb \
  system.ads<system-vxworks-x86.ads"

  case "${TRACE}" in
    yes)
      LIBGNAT_TARGET_PAIRS="${LIBGNAT_TARGET_PAIRS} \
      s-traces.adb<s-traces-default.adb \
      s-trafor.adb<s-trafor-default.adb \
      s-trafor.ads<s-trafor-default.ads \
      s-tratas.adb<s-tratas-default.adb \
      s-tfsetr.adb<s-tfsetr-vxworks.adb"
      ;;
  esac

  EXTRA_GNATRTL_NONTASKING_OBJS="i-vxwork.o i-vxwoio.o"
  EXTRA_GNATRTL_TASKING_OBJS="s-vxwork.o"
  ;;

# xscale vxworks or coff
xscale*-wrs-vx* | xscale*-wrs-coff)
  LIBGNAT_TARGET_PAIRS="\
  a-sytaco.ads<a-sytaco-vxworks.ads \
  a-sytaco.adb<a-sytaco-vxworks.adb \
  a-intnam.ads<a-intnam-vxworks.ads \
  a-numaux.ads<a-numaux-vxworks.ads \
  s-inmaop.adb<s-inmaop-posix.adb \
  s-interr.adb<s-interr-vxworks.adb \
  s-intman.ads<s-intman-vxworks.ads \
  s-intman.adb<s-intman-vxworks.adb \
  s-osinte.adb<s-osinte-vxworks.adb \
  s-osinte.ads<s-osinte-vxworks.ads \
  s-osprim.adb<s-osprim-vxworks.adb \
  s-parame.ads<s-parame-vxworks.ads \
  s-stchop.adb<s-stchop-vxworks.adb \
  s-taprop.adb<s-taprop-vxworks.adb \
  s-taspri.ads<s-taspri-vxworks.ads \
  s-tpopsp.adb<s-tpopsp-vxworks.adb \
  s-vxwork.ads<s-vxwork-xscale.ads \
  g-soccon.ads<g-soccon-vxworks.ads \
  g-socthi.ads<g-socthi-vxworks.ads \
  g-socthi.adb<g-socthi-vxworks.adb \
  system.ads<system-elf-sparc.ads"

  EXTRA_GNATRTL_NONTASKING_OBJS="i-vxwork.o i-vxwoio.o"
  EXTRA_GNATRTL_TASKING_OBJS="s-vxwork.o"
  ;;

mips*-wrs-vx*)
  LIBGNAT_TARGET_PAIRS="\
  a-sytaco.ads<a-sytaco-vxworks.ads \
  a-sytaco.adb<a-sytaco-vxworks.adb \
  a-intnam.ads<a-intnam-vxworks.ads \
  a-numaux.ads<a-numaux-vxworks.ads \
  s-inmaop.adb<s-inmaop-posix.adb \
  s-interr.adb<s-interr-vxworks.adb \
  s-intman.ads<s-intman-vxworks.ads \
  s-intman.adb<s-intman-vxworks.adb \
  s-osinte.adb<s-osinte-vxworks.adb \
  s-osinte.ads<s-osinte-vxworks.ads \
  s-osprim.adb<s-osprim-vxworks.adb \
  s-parame.ads<s-parame-vxworks.ads \
  s-stchop.adb<s-stchop-vxworks.adb \
  s-taprop.adb<s-taprop-vxworks.adb \
  s-taspri.ads<s-taspri-vxworks.ads \
  s-tpopsp.adb<s-tpopsp-vxworks.adb \
  s-vxwork.ads<s-vxwork-mips.ads \
  g-soccon.ads<g-soccon-vxworks.ads \
  g-socthi.ads<g-socthi-vxworks.ads \
  g-socthi.adb<g-socthi-vxworks.adb \
  system.ads<system-vxworks-mips.ads"

  EXTRA_GNATRTL_NONTASKING_OBJS="i-vxwork.o i-vxwoio.o"
  EXTRA_GNATRTL_TASKING_OBJS="s-vxwork.o"
  ;;

sparc-sun-solaris*)
  LIBGNAT_TARGET_PAIRS="\
  a-intnam.ads<a-intnam-solaris.ads \
  s-inmaop.adb<s-inmaop-posix.adb \
  s-intman.adb<s-intman-solaris.adb \
  s-osinte.adb<s-osinte-solaris.adb \
  s-osinte.ads<s-osinte-solaris.ads \
  s-osprim.adb<s-osprim-solaris.adb \
  s-parame.adb<s-parame-solaris.adb \
  s-taprop.adb<s-taprop-solaris.adb \
  s-tasinf.adb<s-tasinf-solaris.adb \
  s-tasinf.ads<s-tasinf-solaris.ads \
  s-taspri.ads<s-taspri-solaris.ads \
  s-tpopsp.adb<s-tpopsp-solaris.adb \
  g-soccon.ads<g-soccon-solaris.ads \
  g-soliop.ads<g-soliop-solaris.ads \
  system.ads<system-solaris-sparc.ads"

  THREADSLIB="-lposix4 -lthread"
  MISCLIB="-lposix4 -lnsl -lsocket"
  SYMLIB='$(ADDR2LINE_SYMLIB)'
  SO_OPTS="-Wl,-h,"
  GNATLIB_SHARED="gnatlib-shared-dual"
  GMEM_LIB="gmemlib"
  PREFIX_OBJS='$(PREFIX_REAL_OBJS)'
  LIBRARY_VERSION="${lib_version}"

  case ${THREAD_KIND} in
  fsu | FSU)
    LIBGNAT_TARGET_PAIRS="\
    a-intnam.ads<a-intnam-solaris.ads \
    s-inmaop.adb<s-inmaop-posix.adb \
    s-intman.adb<s-intman-solaris.adb \
    s-osinte.adb<s-osinte-fsu.adb \
    s-osinte.ads<s-osinte-solaris-fsu.ads \
    s-osprim.adb<s-osprim-solaris.adb \
    s-taprop.adb<s-taprop-posix.adb \
    s-taspri.ads<s-taspri-posix.ads \
    s-tpopsp.adb<s-tpopsp-posix.adb \
    g-soccon.ads<g-soccon-solaris.ads \
    g-soliop.ads<g-soliop-solaris.ads \
    system.ads<system-solaris-sparc.ads"

    THREADSLIB="-lgthreads -lmalloc"
    ;;

  pthread | PTHREAD)
    LIBGNAT_TARGET_PAIRS="\
    a-intnam.ads<a-intnam-solaris.ads \
    s-inmaop.adb<s-inmaop-posix.adb \
    s-intman.adb<s-intman-posix.adb \
    s-osinte.adb<s-osinte-posix.adb \
    s-osinte.ads<s-osinte-solaris-posix.ads \
    s-osprim.adb<s-osprim-solaris.adb \
    s-taprop.adb<s-taprop-posix.adb \
    s-taspri.ads<s-taspri-posix.ads \
    s-tpopsp.adb<s-tpopsp-posix-foreign.adb \
    g-soccon.ads<g-soccon-solaris.ads \
    g-soliop.ads<g-soliop-solaris.ads \
    system.ads<system-solaris-sparc.ads"

    THREADSLIB="-lposix4 -lpthread"
  ;;

  m64)
    LIBGNAT_TARGET_PAIRS="\
    a-intnam.ads<a-intnam-solaris.ads \
    s-inmaop.adb<s-inmaop-posix.adb \
    s-intman.adb<s-intman-solaris.adb \
    s-osinte.adb<s-osinte-solaris.adb \
    s-osinte.ads<s-osinte-solaris.ads \
    s-osprim.adb<s-osprim-solaris.adb \
    s-parame.adb<s-parame-solaris.adb \
    s-taprop.adb<s-taprop-solaris.adb \
    s-tasinf.adb<s-tasinf-solaris.adb \
    s-tasinf.ads<s-tasinf-solaris.ads \
    s-taspri.ads<s-taspri-solaris.ads \
    s-tpopsp.adb<s-tpopsp-solaris.adb \
    g-soccon.ads<g-soccon-solaris.ads \
    g-soliop.ads<g-soliop-solaris.ads \
    system.ads<system-solaris-sparcv9.ads"
    ;;
  esac
  ;;

*86-*-solaris2*)
  LIBGNAT_TARGET_PAIRS="\
  a-numaux.adb<a-numaux-x86.adb \
  a-numaux.ads<a-numaux-x86.ads \
  a-intnam.ads<a-intnam-solaris.ads \
  s-inmaop.adb<s-inmaop-posix.adb \
  s-intman.adb<s-intman-solaris.adb \
  s-mastop.adb<s-mastop-x86.adb \
  s-osinte.adb<s-osinte-solaris.adb \
  s-osinte.ads<s-osinte-solaris.ads \
  s-osprim.adb<s-osprim-solaris.adb \
  s-parame.adb<s-parame-solaris.adb \
  s-taprop.adb<s-taprop-solaris.adb \
  s-tasinf.adb<s-tasinf-solaris.adb \
  s-tasinf.ads<s-tasinf-solaris.ads \
  s-taspri.ads<s-taspri-solaris.ads \
  s-tpopsp.adb<s-tpopsp-solaris.adb \
  g-soccon.ads<g-soccon-solaris.ads \
  g-soliop.ads<g-soliop-solaris.ads \
  system.ads<system-solaris-x86.ads"

  THREADSLIB="-lposix4 -lthread"
  MISCLIB="-lposix4 -lnsl -lsocket"
  SO_OPTS="-Wl,-h,"
  GNATLIB_SHARED="gnatlib-shared-dual"
  PREFIX_OBJS='$(PREFIX_REAL_OBJS)'
  LIBRARY_VERSION="${lib_version}"
  ;;

*86-*-linux*)
  LIBGNAT_TARGET_PAIRS="\
  a-intnam.ads<a-intnam-linux.ads \
  a-numaux.adb<a-numaux-x86.adb \
  a-numaux.ads<a-numaux-x86.ads \
  s-inmaop.adb<s-inmaop-posix.adb \
  s-intman.adb<s-intman-posix.adb \
  s-mastop.adb<s-mastop-x86.adb \
  s-osinte.adb<s-osinte-posix.adb \
  s-osinte.ads<s-osinte-linux.ads \
  s-osprim.adb<s-osprim-posix.adb \
  s-taprop.adb<s-taprop-linux.adb \
  s-taspri.ads<s-taspri-linux.ads \
  s-tpopsp.adb<s-tpopsp-posix-foreign.adb \
  s-parame.adb<s-parame-linux.adb \
  system.ads<system-linux-x86.ads"

  SYMLIB='$(ADDR2LINE_SYMLIB)'
  THREADSLIB="-lpthread"
  GNATLIB_SHARED="gnatlib-shared-dual"
  GMEM_LIB="gmemlib"
  PREFIX_OBJS='$(PREFIX_REAL_OBJS)'
  LIBRARY_VERSION="${lib_version}"

  case ${THREAD_KIND} in
  fsu | FSU)
    LIBGNAT_TARGET_PAIRS="\
    a-intnam.ads<a-intnam-linux.ads \
    a-numaux.adb<a-numaux-x86.adb \
    a-numaux.ads<a-numaux-x86.ads \
    s-inmaop.adb<s-inmaop-posix.adb \
    s-intman.adb<s-intman-posix.adb \
    s-mastop.adb<s-mastop-x86.adb \
    s-osinte.adb<s-osinte-fsu.adb \
    s-osinte.ads<s-osinte-linux-fsu.ads \
    s-osprim.adb<s-osprim-posix.adb \
    s-taprop.adb<s-taprop-posix.adb \
    s-taspri.ads<s-taspri-posix.ads \
    s-tpopsp.adb<s-tpopsp-posix.adb \
    system.ads<system-linux-x86.ads"

    THREADSLIB="-lgthreads -lmalloc"
    ;;
  esac
  ;;

*86-*-freebsd*)
  LIBGNAT_TARGET_PAIRS="\
  a-intnam.ads<a-intnam-freebsd.ads \
  a-numaux.adb<a-numaux-x86.adb \
  a-numaux.ads<a-numaux-x86.ads \
  g-soccon.ads<g-soccon-freebsd.ads \
  s-inmaop.adb<s-inmaop-posix.adb \
  s-intman.adb<s-intman-posix.adb \
  s-mastop.adb<s-mastop-x86.adb \
  s-osinte.adb<s-osinte-freebsd.adb \
  s-osinte.ads<s-osinte-freebsd.ads \
  s-osprim.adb<s-osprim-posix.adb \
  s-taprop.adb<s-taprop-posix.adb \
  s-taspri.ads<s-taspri-posix.ads \
  s-tpopsp.adb<s-tpopsp-posix.adb \
  system.ads<system-freebsd-x86.ads"

  GNATLIB_SHARED="gnatlib-shared-dual"

  SYMLIB='$(ADDR2LINE_SYMLIB)'
  THREADSLIB="-lc_r"
  GMEM_LIB="gmemlib"
  PREFIX_OBJS='$(PREFIX_REAL_OBJS)'
  LIBRARY_VERSION="${lib_version}"
  ;;

s390*-*-linux*)
  case ${target_cpu} in
  s390x)
    LIBGNAT_TARGET_PAIRS="\
    a-intnam.ads<a-intnam-linux.ads \
    s-inmaop.adb<s-inmaop-posix.adb \
    s-intman.adb<s-intman-posix.adb \
    s-osinte.adb<s-osinte-posix.adb \
    s-osinte.ads<s-osinte-linux.ads \
    s-osprim.adb<s-osprim-posix.adb \
    s-taprop.adb<s-taprop-linux.adb \
    s-taspri.ads<s-taspri-linux.ads \
    s-tpopsp.adb<s-tpopsp-posix-foreign.adb \
    s-parame.adb<s-parame-linux.adb \
    system.ads<system-linux-s390x.ads"
    ;;
  *) # Not s390x
    LIBGNAT_TARGET_PAIRS="\
    a-intnam.ads<a-intnam-linux.ads \
    s-inmaop.adb<s-inmaop-posix.adb \
    s-intman.adb<s-intman-posix.adb \
    s-osinte.adb<s-osinte-posix.adb \
    s-osinte.ads<s-osinte-linux.ads \
    s-osprim.adb<s-osprim-posix.adb \
    s-taprop.adb<s-taprop-linux.adb \
    s-taspri.ads<s-taspri-linux.ads \
    s-tpopsp.adb<s-tpopsp-posix-foreign.adb \
    s-parame.adb<s-parame-linux.adb \
    system.ads<system-linux-s390.ads"
    ;;
  esac

  THREADSLIB="-lpthread"
  GNATLIB_SHARED="gnatlib-shared-dual"
  PREFIX_OBJS='$(PREFIX_REAL_OBJS)'
  LIBRARY_VERSION="${lib_version}"
  ;;

mips-sgi-irix*)
  case ${target} in
  mips-sgi-irix6*)
    LIBGNAT_TARGET_PAIRS="\
    a-intnam.ads<a-intnam-irix.ads \
    s-inmaop.adb<s-inmaop-posix.adb \
    s-intman.adb<s-intman-irix.adb \
    s-mastop.adb<s-mastop-irix.adb \
    s-osinte.adb<s-osinte-irix.adb \
    s-osinte.ads<s-osinte-irix.ads \
    s-osprim.adb<s-osprim-posix.adb \
    s-proinf.adb<s-proinf-irix-athread.adb \
    s-proinf.ads<s-proinf-irix-athread.ads \
    s-taprop.adb<s-taprop-irix.adb \
    s-tasinf.ads<s-tasinf-irix.ads \
    s-taspri.ads<s-taspri-posix.ads \
    s-tpopsp.adb<s-tpopsp-posix.adb \
    s-traceb.adb<s-traceb-mastop.adb \
    g-soccon.ads<g-soccon-irix.ads \
    system.ads<system-irix-n32.ads"

    THREADSLIB="-lpthread"
    GNATLIB_SHARED="gnatlib-shared-default"
    ;;
  *) # Not irix 6
    LIBGNAT_TARGET_PAIRS="\
    a-intnam.ads<a-intnam-irix.ads \
    s-inmaop.adb<s-inmaop-dummy.adb \
    s-interr.adb<s-interr-sigaction.adb \
    s-intman.adb<s-intman-irix-athread.adb \
    s-mastop.adb<s-mastop-irix.adb \
    s-osinte.adb<s-osinte-irix.adb \
    s-osinte.ads<s-osinte-irix-athread.ads \
    s-osprim.adb<s-osprim-posix.adb \
    s-proinf.adb<s-proinf-irix-athread.adb \
    s-proinf.ads<s-proinf-irix-athread.ads \
    s-taprop.adb<s-taprop-irix-athread.adb \
    s-tasinf.adb<s-tasinf-irix-athread.adb \
    s-tasinf.ads<s-tasinf-irix-athread.ads \
    s-taspri.ads<s-taspri-posix.ads \
    s-traceb.adb<s-traceb-mastop.adb \
    g-soccon.ads<g-soccon-irix.ads \
    system.ads<system-irix-o32.ads"
    ;;
  esac

  TGT_LIB="-lexc"
  MISCLIB="-lexc"
  PREFIX_OBJS='$(PREFIX_REAL_OBJS)'
  LIBRARY_VERSION="${lib_version}"
  ;;

hppa*-hp-hpux10*)
  LIBGNAT_TARGET_PAIRS="\
  a-excpol.adb<a-excpol-abort.adb \
  a-intnam.ads<a-intnam-hpux.ads \
  s-inmaop.adb<s-inmaop-posix.adb \
  s-interr.adb<s-interr-sigaction.adb \
  s-intman.adb<s-intman-posix.adb \
  s-osinte.adb<s-osinte-hpux-dce.adb \
  s-osinte.ads<s-osinte-hpux-dce.ads \
  s-parame.ads<s-parame-hpux.ads \
  s-osprim.adb<s-osprim-posix.adb \
  s-taprop.adb<s-taprop-hpux-dce.adb \
  s-taspri.ads<s-taspri-hpux-dce.ads \
  s-tpopsp.adb<s-tpopsp-posix.adb \
  g-soccon.ads<g-soccon-hpux.ads \
  system.ads<system-hpux.ads"

  PREFIX_OBJS='$(PREFIX_REAL_OBJS)'
  ;;

hppa*-hp-hpux11*)
  LIBGNAT_TARGET_PAIRS="\
  a-intnam.ads<a-intnam-hpux.ads \
  s-inmaop.adb<s-inmaop-posix.adb \
  s-intman.adb<s-intman-posix.adb \
  s-osinte.adb<s-osinte-posix.adb \
  s-osinte.ads<s-osinte-hpux.ads \
  s-parame.ads<s-parame-hpux.ads \
  s-osprim.adb<s-osprim-posix.adb \
  s-traceb.adb<s-traceb-hpux.adb \
  s-taprop.adb<s-taprop-posix.adb \
  s-taspri.ads<s-taspri-posix.ads \
  s-tpopsp.adb<s-tpopsp-posix-foreign.adb \
  g-soccon.ads<g-soccon-hpux.ads \
  system.ads<system-hpux.ads"

  TGT_LIB="/usr/lib/libcl.a"
  THREADSLIB="-lpthread"
  SYMLIB='$(ADDR2LINE_SYMLIB)'
  GMEM_LIB="gmemlib"
  soext='.sl'
  SO_OPTS="-Wl,+h,"
  PREFIX_OBJS='$(PREFIX_REAL_OBJS)'
  GNATLIB_SHARED="gnatlib-shared-dual"
  LIBRARY_VERSION="${lib_version}"

  case ${THREAD_KIND} in
  dce | DCE)
    LIBGNAT_TARGET_PAIRS="\
    a-excpol.adb<a-excpol-abort.adb \
    a-intnam.ads<a-intnam-hpux.ads \
    s-inmaop.adb<s-inmaop-posix.adb \
    s-interr.adb<s-interr-sigaction.adb \
    s-intman.adb<s-intman-posix.adb \
    s-osinte.adb<s-osinte-hpux-dce.adb \
    s-osinte.ads<s-osinte-hpux-dce.ads \
    s-parame.ads<s-parame-hpux.ads \
    s-osprim.adb<s-osprim-posix.adb \
    s-taprop.adb<s-taprop-hpux-dce.adb \
    s-taspri.ads<s-taspri-hpux-dce.ads \
    s-tpopsp.adb<s-tpopsp-posix.adb \
    g-soccon.ads<g-soccon-hpux.ads \
    system.ads<system-hpux.ads"

    TGT_LIB=
    THREADSLIB="-lcma"
    ;;
  esac
  ;;

*-ibm-aix*)
  LIBGNAT_TARGET_PAIRS="\
  a-intnam.ads<a-intnam-aix.ads \
  s-inmaop.adb<s-inmaop-posix.adb \
  s-intman.adb<s-intman-posix.adb \
  s-osinte.adb<s-osinte-aix.adb \
  s-osinte.ads<s-osinte-aix.ads \
  s-osprim.adb<s-osprim-posix.adb \
  s-taprop.adb<s-taprop-posix.adb \
  s-taspri.ads<s-taspri-posix.ads \
  s-tpopsp.adb<s-tpopsp-posix.adb \
  g-soccon.ads<g-soccon-aix.ads \
  system.ads<system-aix.ads"

  THREADSLIB="-lpthreads"
  PREFIX_OBJS='$(PREFIX_REAL_OBJS)'

  case ${THREAD_KIND} in
  fsu | FSU)
    LIBGNAT_TARGET_PAIRS="\
    a-intnam.ads<a-intnam-aix.ads \
    s-inmaop.adb<s-inmaop-posix.adb \
    s-intman.adb<s-intman-posix.adb \
    s-osinte.adb<s-osinte-fsu.adb \
    s-osinte.ads<s-osinte-aix-fsu.ads \
    s-osprim.adb<s-osprim-posix.adb \
    s-taprop.adb<s-taprop-posix.adb \
    s-taspri.ads<s-taspri-posix.ads \
    s-tpopsp.adb<s-tpopsp-posix.adb \
    g-soccon.ads<g-soccon-aix.ads \
    system.ads<system-aix.ads"

    THREADSLIB="-lgthreads -lmalloc"
    ;;
  esac

  GMEM_LIB="gmemlib"
  SYMLIB='$(ADDR2LINE_SYMLIB)'
  ;;

*-*-lynxos)
  case ${target} in
  *86-*-lynxos)
    LIBGNAT_TARGET_PAIRS="\
    a-numaux.adb<a-numaux-x86.adb \
    a-numaux.ads<a-numaux-x86.ads \
    a-intnam.ads<a-intnam-lynxos.ads \
    s-mastop.adb<s-mastop-x86.adb \
    s-inmaop.adb<s-inmaop-posix.adb \
    s-intman.adb<s-intman-posix.adb \
    s-osinte.adb<s-osinte-lynxos.adb \
    s-osinte.ads<s-osinte-lynxos.ads \
    s-osprim.adb<s-osprim-posix.adb \
    s-taprop.adb<s-taprop-lynxos.adb \
    s-taspri.ads<s-taspri-lynxos.ads \
    s-tpopsp.adb<s-tpopsp-lynxos.adb \
    system.ads<system-lynxos-x86.ads"

    PREFIX_OBJS='$(PREFIX_REAL_OBJS)'
    ;;
  *) # lynxos but not x86
    LIBGNAT_TARGET_PAIRS="\
    a-intnam.ads<a-intnam-lynxos.ads \
    s-inmaop.adb<s-inmaop-posix.adb \
    s-intman.adb<s-intman-posix.adb \
    s-osinte.adb<s-osinte-lynxos-3.adb \
    s-osinte.ads<s-osinte-lynxos-3.ads \
    s-osprim.adb<s-osprim-posix.adb \
    s-taprop.adb<s-taprop-posix.adb \
    s-taspri.ads<s-taspri-posix.ads \
    s-tpopsp.adb<s-tpopsp-posix.adb \
    system.ads<system-lynxos-ppc.ads"

    case ${THREAD_KIND} in
    pthread | PTHREAD)
      LIBGNAT_TARGET_PAIRS="\
      a-intnam.ads<a-intnam-lynxos.ads \
      s-inmaop.adb<s-inmaop-posix.adb \
      s-intman.adb<s-intman-posix.adb \
      s-osinte.adb<s-osinte-lynxos.adb \
      s-osinte.ads<s-osinte-lynxos.ads \
      s-osprim.adb<s-osprim-posix.adb \
      s-taprop.adb<s-taprop-lynxos.adb \
      s-taspri.ads<s-taspri-lynxos.ads \
      s-tpopsp.adb<s-tpopsp-lynxos.adb \
      system.ads<system-lynxos-ppc.ads"
      ;;
    esac
    ;;
  esac
  ;;

*-*-rtems*)
  LIBGNAT_TARGET_PAIRS="\
  a-intnam.ads<4rintnam.ads \
  s-inmaop.adb<s-inmaop-posix.adb \
  s-intman.adb<s-intman-posix.adb \
  s-osinte.adb<5rosinte.adb \
  s-osinte.ads<5rosinte.ads \
  s-osprim.adb<s-osprim-posix.adb \
  s-parame.adb<5rparame.adb \
  s-taprop.adb<s-taprop-posix.adb \
  s-taspri.ads<s-taspri-posix.ads \
  s-tpopsp.adb<5rtpopsp.adb"
  ;;

alpha*-dec-osf*)
  LIBGNAT_TARGET_PAIRS="\
  a-intnam.ads<a-intnam-tru64.ads \
  s-inmaop.adb<s-inmaop-posix.adb \
  s-intman.adb<s-intman-posix.adb \
  s-mastop.adb<s-mastop-tru64.adb \
  s-osinte.adb<s-osinte-tru64.adb \
  s-osinte.ads<s-osinte-tru64.ads \
  s-osprim.adb<s-osprim-unix.adb \
  s-taprop.adb<s-taprop-tru64.adb \
  s-tasinf.ads<s-tasinf-tru64.ads \
  s-taspri.ads<s-taspri-tru64.ads \
  s-tpopsp.adb<s-tpopsp-posix-foreign.adb \
  s-traceb.adb<s-traceb-mastop.adb \
  g-soccon.ads<g-soccon-tru64.ads \
  system.ads<system-tru64.ads"

  GMEM_LIB="gmemlib"
  SYMLIB='$(ADDR2LINE_SYMLIB)'
  THREADSLIB="-lpthread -lmach -lexc -lrt"
  PREFIX_OBJS='$(PREFIX_REAL_OBJS)'
  GNATLIB_SHARED="gnatlib-shared-default"
  LIBRARY_VERSION="${lib_version}"
  ;;

alpha*-dec-vms* | alpha*-dec-openvms* | alpha*-dec-vms* \
| alpha*-hp-vms* | alpha*-hp-openvms* | alpha*-hp-vms* \
| ia64*-dec-vms* | ia64*-dec-openvms* | ia64*-dec-vms* \
| ia64*-hp-vms* | ia64*-hp-openvms* | ia64*-hp-vms*)

  LIBGNAT_TARGET_PAIRS="\
  a-caldel.adb<a-caldel-vms.adb \
  a-calend.adb<a-calend-vms.adb \
  a-calend.ads<a-calend-vms.ads \
  a-dirval.adb<a-dirval-vms.adb \
  a-excpol.adb<a-excpol-abort.adb \
  a-intnam.ads<a-intnam-vms.ads \
  a-numaux.ads<a-numaux-vms.ads \
  g-expect.adb<g-expect-vms.adb \
  g-soccon.ads<g-soccon-vms.adb \
  g-socthi.ads<g-socthi-vms.ads \
  g-socthi.adb<g-socthi-vms.adb \
  g-trasym.adb<g-trasym-vms.adb \
  i-cstrea.adb<i-cstrea-vms.adb \
  i-cpp.adb<i-cpp-vms.adb \
  interfac.ads<interfac-vms.ads \
  s-asthan.adb<s-asthan-vms.adb \
  s-inmaop.adb<s-inmaop-vms.adb \
  s-interr.adb<s-interr-vms.adb \
  s-intman.adb<s-intman-vms.adb \
  s-intman.ads<s-intman-vms.ads \
  s-osprim.adb<s-osprim-vms.adb \
  s-osprim.ads<s-osprim-vms.ads \
  s-taprop.adb<s-taprop-vms.adb \
  s-taspri.ads<s-taspri-vms.ads \
  s-tpopsp.adb<s-tpopsp-posix.adb \
  s-tpopde.adb<s-tpopde-vms.adb \
  s-tpopde.ads<s-tpopde-vms.ads \
  s-traent.adb<s-traent-vms.adb \
  s-traent.ads<s-traent-vms.ads"

  case ${target} in
  ia64-hp-vms* | ia64-hp-openvms*)
    LIBGNAT_TARGET_PAIRS="${LIBGNAT_TARGET_PAIRS}\
    s-auxdec.ads<s-auxdec-vms_64.ads \
    s-crtl.ads<s-crtl-vms.ads \
    s-osinte.adb<s-osinte-vms-ia64.adb \
    s-osinte.ads<s-osinte-vms-ia64.ads \
    system.ads<system-vms_64.ads"
    ;;
  alpha*-dec-vms* | alpha*-dec-openvms* | alpha*-dec-alphavms*)
    LIBGNAT_TARGET_PAIRS="${LIBGNAT_TARGET_PAIRS}\
    s-crtl.ads<s-crtl-vms.ads \
    s-osinte.adb<s-osinte-vms.adb \
    s-osinte.ads<s-osinte-vms.ads \
    s-vaflop.adb<s-vaflop-vms-alpha.adb \
    system.ads<system-vms-zcx.ads"
    ;;
  esac

  case ${THREAD_KIND} in
  express | EXPRESS)
    LIBGNAT_TARGET_PAIRS="${LIBGNAT_TARGET_PAIRS}\
    s-parame.ads<s-parame-vms-restrict.ads"
    ;;
  *)
    LIBGNAT_TARGET_PAIRS="${LIBGNAT_TARGET_PAIRS}\
    s-parame.ads<s-parame-vms.ads"
    ;;
  esac

  GNATLIB_SHARED="gnatlib-shared-vms"

  case ${target} in
  alpha*-dec-vms* | alpha*-dec-openvms* | alpha*-dec-alphavms*)
    EXTRA_LIBGNAT_SRCS="vmshandler.asm"
    EXTRA_LIBGNAT_OBJS="vmshandler.o"
    ;;
  esac

  EXTRA_GNATRTL_TASKING_OBJS="s-tpopde.o"

  # NCN MAKEISMS
  # This command transforms (YYYYMMDD) into YY,MMDD
  # GSMATCH_VERSION :=$(shell grep "^ *Gnat_Static_Version_String" $(fsrcpfx)gnatvsn.ads | sed -e 's/.*(\(.*\)).*/\1/' -e 's/\(..\)\(..\)\(....\)/\2,\3/')
  # TOOLS_LIBS_LO :=--for-linker=sys\\$$\$$library:trace.exe

  # Mangle library version differently.
  LIBRARY_VERSION=`echo ${lib_version} | sed -s 's/\./\_/g'`
  ;;

*-*-cygwin32* | *-*-mingw32* | *-*-pe)
  LIBGNAT_TARGET_PAIRS="\
  a-calend.adb<a-calend-mingw.adb \
  a-dirval.adb<a-dirval-mingw.adb \
  a-excpol.adb<a-excpol-abort.adb \
  a-intnam.ads<a-intnam-mingw.ads \
  a-numaux.adb<a-numaux-x86.adb \
  a-numaux.ads<a-numaux-x86.ads \
  s-gloloc.adb<s-gloloc-mingw.adb \
  s-inmaop.adb<s-inmaop-dummy.adb \
  s-interr.adb<s-interr-sigaction.adb \
  s-intman.adb<s-intman-mingw.adb \
  s-mastop.adb<s-mastop-x86.adb \
  s-memory.adb<s-memory-mingw.adb \
  s-osinte.ads<s-osinte-mingw.ads \
  s-osprim.adb<s-osprim-mingw.adb \
  s-taprop.adb<s-taprop-mingw.adb \
  s-taspri.ads<s-taspri-mingw.ads \
  s-parame.adb<s-parame-mingw.adb \
  g-socthi.ads<g-socthi-mingw.ads \
  g-socthi.adb<g-socthi-mingw.adb \
  g-soccon.ads<g-soccon-mingw.ads \
  g-soliop.ads<g-soliop-mingw.ads \
  system.ads<system-mingw.ads"

  MISCLIB="-lwsock32"
  SYMLIB='$(ADDR2LINE_SYMLIB)'
  GMEM_LIB="gmemlib"
  PREFIX_OBJS='$(PREFIX_REAL_OBJS)'
  EXTRA_GNATMAKE_OBJS="mdll.o mdll-utl.o mdll-fil.o"
  EXTRA_GNATRTL_NONTASKING_OBJS="g-regist.o"
  soext='.dll'
  # ??? This will be replaced by gnatlib-shared-dual-win32 when GNAT auto-import
  # support for array/record will be done.
  GNATLIB_SHARED="gnatlib-shared-win32"
  LIBRARY_VERSION="${lib_version}"
  ;;

powerpc*-*-linux*)
  LIBGNAT_TARGET_PAIRS="\
  a-intnam.ads<a-intnam-linux.ads \
  s-inmaop.adb<s-inmaop-posix.adb \
  s-intman.adb<s-intman-posix.adb \
  s-osinte.adb<s-osinte-posix.adb \
  s-osinte.ads<s-osinte-linux.ads \
  s-osprim.adb<s-osprim-posix.adb \
  s-taprop.adb<s-taprop-linux.adb \
  s-taspri.ads<s-taspri-linux.ads \
  s-tpopsp.adb<s-tpopsp-posix-foreign.adb \
  s-parame.adb<s-parame-linux.adb \
  system.ads<system-linux-ppc.ads"

  SYMLIB='$(ADDR2LINE_SYMLIB)'
  THREADSLIB="-lpthread"
  GNATLIB_SHARED="gnatlib-shared-dual"
  GMEM_LIB="gmemlib"
  PREFIX_OBJS='$(PREFIX_REAL_OBJS)'
  LIBRARY_VERSION="${lib_version}"
  ;;

*ia64-*-linux*)
  LIBGNAT_TARGET_PAIRS="\
  a-intnam.ads<a-intnam-linux.ads \
  a-numaux.ads<a-numaux-libc-x86.ads \
  s-inmaop.adb<s-inmaop-posix.adb \
  s-intman.adb<s-intman-posix.adb \
  s-osinte.ads<s-osinte-linux.ads \
  s-osinte.adb<s-osinte-posix.adb \
  s-osprim.adb<s-osprim-posix.adb \
  s-taprop.adb<s-taprop-linux.adb \
  s-tpopsp.adb<s-tpopsp-posix-foreign.adb \
  s-taspri.ads<s-taspri-linux.ads \
  system.ads<system-linux-ia64.ads"

  MISCLIB=
  THREADSLIB="-lpthread"
  GNATLIB_SHARED="gnatlib-shared-dual"
  PREFIX_OBJS='$(PREFIX_REAL_OBJS)'
  LIBRARY_VERSION="${lib_version}"
  ;;

*x86_64-*-linux*)
  LIBGNAT_TARGET_PAIRS="\
  a-intnam.ads<a-intnam-linux.ads \
  a-numaux.adb<a-numaux-x86.adb \
  a-numaux.ads<a-numaux-x86.ads \
  s-inmaop.adb<s-inmaop-posix.adb \
  s-intman.adb<s-intman-posix.adb \
  s-osinte.ads<s-osinte-linux.ads \
  s-osinte.adb<s-osinte-posix.adb \
  s-osprim.adb<s-osprim-posix.adb \
  s-taprop.adb<s-taprop-linux.adb \
  s-tpopsp.adb<s-tpopsp-posix-foreign.adb \
  s-taspri.ads<s-taspri-linux.ads \
  system.ads<system-linux-x86_64.ads"

  SYMLIB='$(ADDR2LINE_SYMLIB)'
  THREADSLIB="-lpthread"
  GNATLIB_SHARED="gnatlib-shared-dual"
  GMEM_LIB="gmemlib"
  PREFIX_OBJS='$(PREFIX_REAL_OBJS)'
  LIBRARY_VERSION="${lib_version}"
  ;;

powerpc-*-darwin*)
  LIBGNAT_TARGET_PAIRS="\
  a-intnam.ads<a-intnam-darwin.ads \
  s-inmaop.adb<s-inmaop-posix.adb \
  s-intman.adb<s-intman-posix.adb \
  s-osinte.adb<s-osinte-darwin.adb \
  s-osinte.ads<s-osinte-darwin.ads \
  s-osprim.adb<s-osprim-posix.adb \
  s-taprop.adb<s-taprop-posix.adb \
  s-taspri.ads<s-taspri-posix.ads \
  s-tpopsp.adb<s-tpopsp-posix-foreign.adb \
  g-soccon.ads<g-soccon-aix.ads \
  system.ads<system-darwin-ppc.ads"

  GNATLIB_SHARED="gnatlib-shared-darwin"
  SO_OPTS="-Wl,-flat_namespace"
  RANLIB="ranlib -c"
  GMEM_LIB="gmemlib"

  LIBRARY_VERSION="${lib_version}"

  soext='.dylib'
  ;;
esac

AC_OUTPUT


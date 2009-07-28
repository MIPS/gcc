
AC_PREREQ(2.59)
AC_INIT([libstd], [0.1], [embednet-devel@lists.codex.cro.st.com])

AC_CONFIG_AUX_DIR([ac_scripts])

AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE([1.9.0 foreign])

AC_CONFIG_SRCDIR([include/assert.h])
AM_CONFIG_HEADER([config.h])

case $target_cpu in
   cil32)
     enable_cli_lib=yes
   ;;
   *)
     AC_ARG_ENABLE(native-lib,
       AS_HELP_STRING([--enable-native-lib],
                      [build native version of libstd  (default no)]),
                      ,
                      enable_native_lib=no)
     AC_ARG_ENABLE(opt-native-lib,
       AS_HELP_STRING([--enable-opt-native-lib],
                      [build optimized version of libstd (default no)]),
                      ,
                      enable_opt_native_lib=no)
   ;;
esac
AM_CONDITIONAL(CLI_LIB, test x$enable_cli_lib = xyes)
AM_CONDITIONAL(NATIVE_LIB, test x$enable_native_lib = xyes)
AM_CONDITIONAL(OPT_NATIVE_LIB, test x$enable_opt_native_lib = xyes)

case $target_cpu in
   armv5e)
     libnativedir="\$(libdir)/arm"
   ;;
   cil32)
     #libcildir="\$(libdir)/cil"
     libcildir="\$(libdir)"
   ;;
   *86)
     libnativedir="\$(libdir)/i686"
   ;;
   *)
     libnativedir="\$(libdir)/$target_cpu"
   ;;
esac
libciloptdir="\$(libdir)/cil/opt"
AC_SUBST(libnativedir)
AC_SUBST(libcildir)
AC_SUBST(libciloptdir)


# Checks for programs.
if test x"$CFLAGS" = x; then CFLAGS="-O2"; fi

#if target is cil32 toolchain probably still incomplete (missing standard libs)
case $target_cpu in
   cil32)
     CFLAGS="-nostdlib -lgcc4net.dll $CFLAGS"
   ;;
esac

AC_PROG_CC
AC_C_INLINE
AC_C_RESTRICT
AC_PROG_INSTALL
#AC_LIBTOOL_TAGS([]) enable when switch to libtool 1.6
AC_PROG_LIBTOOL

AC_ARG_WITH(mcs,
	    AS_HELP_STRING([--with-mcs=mcs_binary_path], [use this mcs]),
	    MCS=$withval)
AC_CHECK_PROG(MCS,mcs,mcs,mcs-not-found)

AC_CONFIG_FILES([Makefile include/Makefile src/Makefile src_opt/Makefile])

AC_OUTPUT

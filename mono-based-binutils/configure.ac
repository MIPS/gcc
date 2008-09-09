
AC_PREREQ(2.59)
AC_INIT(cil-binutils, 0.1)

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([ac_scripts])

AC_CANONICAL_TARGET

AC_CONFIG_SRCDIR(scripts/as.in)

AM_INIT_AUTOMAKE([1.9.0 foreign])

AM_MAINTAINER_MODE

if test $target_cpu != cil32; then
   AC_ERROR([Wrong target, target must be cil32])
fi

lbindir="\$(prefix)/lbin"
AC_SUBST(lbindir)

AC_PROG_LN_S

AC_PATH_PROG(MONO, mono, mono)

AC_ARG_WITH(ilasm,
	    AS_HELP_STRING([--with-ilasm=as_binary_path], [use this ilasm]),
	    CIL_AS=$withval)
AC_PATH_PROGS(CIL_AS, [ ilasm.pnet ilasm ], ilasm-pnet-not-found)

AC_ARG_WITH(mcs,
	    AS_HELP_STRING([--with-mcs=mcs_binary_path], [use this mcs]),
	    MCS=$withval)
AC_CHECK_PROG(MCS, mcs, mcs, mcs-not-found)

AC_CONFIG_FILES([Makefile scripts/Makefile tools/Makefile])
AC_OUTPUT

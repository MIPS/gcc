
AC_PREREQ(2.59)
AC_INIT(cil-binutils, 0.1)

AC_CONFIG_AUX_DIR([ac_scripts])

AC_CANONICAL_TARGET

AC_CONFIG_SRCDIR(as.in)

AM_INIT_AUTOMAKE([1.9.0 foreign])

AM_MAINTAINER_MODE

if test $target_cpu != cil32; then
   AC_ERROR([Wrong target, target must be cil32])
fi

AC_PROG_LN_S

# Specify the local prefix
local_prefix=
AC_ARG_WITH(local-prefix,
            [  --with-local-prefix=DIR specifies directory to put local include],
            [case "${withval}" in
               yes)	AC_MSG_ERROR(bad value ${withval} given for local include directory prefix) ;;
               no)	;;
               *)	local_prefix=$with_local_prefix ;;
            esac],
            local_prefix=$prefix/cil32)
AC_SUBST(local_prefix)

AC_ARG_WITH(ilasm,
	    AS_HELP_STRING([--with-ilasm=as_binary_path], [use this ilasm]),
	    CIL_AS=$withval)
AC_PATH_PROGS(CIL_AS, [ ilasm.pnet ilasm ], ilasm-pnet-not-found)

if test "$CIL_AS" = ilasm-pnet-not-found ; then
   AC_MSG_ERROR([DotGNU ilasm not found])
fi

if test `$CIL_AS --version | grep "Southern Storm Software" | wc -l` = 0  ; then
   AC_MSG_ERROR([wrong ilasm, it must be the one from DotGNU project])
fi

AC_ARG_WITH(ilalink,
	    AS_HELP_STRING([--with-ilalink=ld_binary_path], [use this ilalink]),
	    CIL_LD=$withval)
AC_PATH_PROG(CIL_LD, ilalink, ilalink-not-found)

if test "$CIL_LD" = ilalink-not-found ; then
   AC_MSG_ERROR([DotGNU ilalink not found])
fi

if test "x$OPENSYSTEM_C_DLL" = x ; then
if test -f /usr/lib/cscc/lib/OpenSystem.C.dll ; then
   OPENSYSTEM_C_DLL=/usr/lib/cscc/lib/OpenSystem.C.dll
else
   AC_MSG_ERROR([OPENSYSTEM_C_DLL Not found])
fi
else
   OPENSYSTEM_C_DLL=$OPENSYSTEM_C_DLL
fi
AC_SUBST(OPENSYSTEM_C_DLL)

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

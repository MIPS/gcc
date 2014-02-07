dnl A function to check MPX is enabled

AC_DEFUN([ENABLE_MPX],
[
  AC_ARG_ENABLE(mpx,
  [AS_HELP_STRING([--enable-mpx],
		  [Enable MPX])],
  enable_mpx=$enableval,
  enable_mpx=no
  )
  AC_SUBST(enable_mpx)
])

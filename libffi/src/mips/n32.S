   n32.S - Copyright (c) 1996, 1998  Red Hat, Inc.
#include <fficonfig.h>
#define SIZEOF_FRAME	( 8 * FFI_SIZEOF_ARG )
	REG_S	$fp, SIZEOF_FRAME - 2*FFI_SIZEOF_ARG($sp)	# Save frame pointer
	REG_S	ra, SIZEOF_FRAME - 1*FFI_SIZEOF_ARG($sp)	# Save return address
	REG_S	bytes, 2*FFI_SIZEOF_ARG($fp) # bytes
	REG_S	flags, 3*FFI_SIZEOF_ARG($fp) # flags
	REG_S	raddr, 4*FFI_SIZEOF_ARG($fp) # raddr
	REG_S	fn,    5*FFI_SIZEOF_ARG($fp) # fn
	bge	bytes, 4 * FFI_SIZEOF_ARG, bigger	
	LI	v0, 4 * FFI_SIZEOF_ARG
	ADDU	t4, v0, 2 * FFI_SIZEOF_ARG -1	# make sure it is aligned 
	and	v0, t4, -2 * FFI_SIZEOF_ARG		# to a proper boundry.
	ADDU	a0, $sp, 0      # 4 * FFI_SIZEOF_ARG
	ADDU	a3, $fp, 3 * FFI_SIZEOF_ARG
	#	ADDU	$sp, $sp, 4 * FFI_SIZEOF_ARG	# adjust $sp to new args
	REG_L	t6, 2*FFI_SIZEOF_ARG($fp)
	# Is it bigger than 8 * FFI_SIZEOF_ARG?
	dadd	t7, $0, 8 * FFI_SIZEOF_ARG
	REG_L	t4, 3*FFI_SIZEOF_ARG($fp)  # load the flags word
	REG_L	a0, 0*FFI_SIZEOF_ARG(t9)
	l.s	$f12, 0*FFI_SIZEOF_ARG(t9)
	l.d	$f12, 0*FFI_SIZEOF_ARG(t9)
	REG_L	a1, 1*FFI_SIZEOF_ARG(t9)
	l.s	$f13, 1*FFI_SIZEOF_ARG(t9)	
	l.d	$f13, 1*FFI_SIZEOF_ARG(t9)	
	REG_L	a2, 2*FFI_SIZEOF_ARG(t9)
	l.s	$f14, 2*FFI_SIZEOF_ARG(t9)	
	l.d	$f14, 2*FFI_SIZEOF_ARG(t9)	
	REG_L	a3, 3*FFI_SIZEOF_ARG(t9)
	l.s	$f15, 3*FFI_SIZEOF_ARG(t9)	
	l.d	$f15, 3*FFI_SIZEOF_ARG(t9)	
	REG_L	a4, 4*FFI_SIZEOF_ARG(t9)
	l.s	$f16, 4*FFI_SIZEOF_ARG(t9)	
	l.d	$f16, 4*FFI_SIZEOF_ARG(t9)	
	REG_L	a5, 5*FFI_SIZEOF_ARG(t9)
	l.s	$f17, 5*FFI_SIZEOF_ARG(t9)	
	l.d	$f17, 5*FFI_SIZEOF_ARG(t9)	
	REG_L	a6, 6*FFI_SIZEOF_ARG(t9)
	l.s	$f18, 6*FFI_SIZEOF_ARG(t9)	
	l.d	$f18, 6*FFI_SIZEOF_ARG(t9)	
	REG_L	a7, 7*FFI_SIZEOF_ARG(t9)
 	l.s	$f19, 7*FFI_SIZEOF_ARG(t9)	
 	l.d	$f19, 7*FFI_SIZEOF_ARG(t9)	
	REG_L	t9, 5*FFI_SIZEOF_ARG($fp)
	REG_L	t5, 4*FFI_SIZEOF_ARG($fp)
	REG_L	t4, 4*FFI_SIZEOF_ARG($fp)
	REG_L	t4, 4*FFI_SIZEOF_ARG($fp)
	REG_L	t4, 4*FFI_SIZEOF_ARG($fp)
	REG_L	t4, 4*FFI_SIZEOF_ARG($fp)
	REG_L	t4, 4*FFI_SIZEOF_ARG($fp)
	REG_L	t4, 4*FFI_SIZEOF_ARG($fp)
	REG_L	t4, 4*FFI_SIZEOF_ARG($fp)
	REG_L	t4, 4*FFI_SIZEOF_ARG($fp)
	REG_L	t4, 4*FFI_SIZEOF_ARG($fp)
	REG_L	t4, 4*FFI_SIZEOF_ARG($fp)
	REG_L	t4, 4*FFI_SIZEOF_ARG($fp)
	REG_L	$fp, SIZEOF_FRAME - 2*FFI_SIZEOF_ARG($sp) # Restore frame pointer
	REG_L	ra, SIZEOF_FRAME - 1*FFI_SIZEOF_ARG($sp)  # Restore return address

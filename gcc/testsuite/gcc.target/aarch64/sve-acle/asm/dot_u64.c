/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** dot_u64_tied1:
**	udot	z0\.d, z16\.h, z17\.h
**	ret
*/
TEST_DUAL_Z (dot_u64_tied1, svuint64_t, svuint16_t,
	     z0 = svdot_u64 (z0, z16, z17),
	     z0 = svdot (z0, z16, z17))

/*
** dot_u64_tied2:
**	mov	(z[0-9]+)\.d, z16\.d
**	movprfx	z16, z0
**	udot	z16\.d, \1\.h, z17\.h
**	ret
*/
TEST_DUAL_Z (dot_u64_tied2, svuint64_t, svuint16_t,
	     z16_res = svdot_u64 (z0, z16, z17),
	     z16_res = svdot (z0, z16, z17))

/*
** dot_u64_tied3:
**	mov	(z[0-9]+)\.d, z17\.d
**	movprfx	z17, z0
**	udot	z17\.d, z16\.h, \1\.h
**	ret
*/
TEST_DUAL_Z (dot_u64_tied3, svuint64_t, svuint16_t,
	     z17_res = svdot_u64 (z0, z16, z17),
	     z17_res = svdot (z0, z16, z17))

/*
** dot_u64_untied:
**	movprfx	z0, z1
**	udot	z0\.d, z16\.h, z17\.h
**	ret
*/
TEST_DUAL_Z (dot_u64_untied, svuint64_t, svuint16_t,
	     z0 = svdot_u64 (z1, z16, z17),
	     z0 = svdot (z1, z16, z17))

/*
** dot_w0_u64_tied1:
**	mov	(z[0-9]+\.h), w0
**	udot	z0\.d, z16\.h, \1
**	ret
*/
TEST_DUAL_ZS (dot_w0_u64_tied1, svuint64_t, svuint16_t, uint16_t,
	      z0 = svdot_n_u64 (z0, z16, x0),
	      z0 = svdot (z0, z16, x0))

/*
** dot_w0_u64_tied2:
**	mov	(z[0-9]+\.h), w0
**	mov	(z[0-9]+)\.d, z16\.d
**	movprfx	z16, z0
**	udot	z16\.d, \2\.h, \1
**	ret
*/
TEST_DUAL_ZS (dot_w0_u64_tied2, svuint64_t, svuint16_t, uint16_t,
	      z16_res = svdot_n_u64 (z0, z16, x0),
	      z16_res = svdot (z0, z16, x0))

/*
** dot_w0_u64_untied:
**	mov	(z[0-9]+\.h), w0
**	movprfx	z0, z1
**	udot	z0\.d, z16\.h, \1
**	ret
*/
TEST_DUAL_ZS (dot_w0_u64_untied, svuint64_t, svuint16_t, uint16_t,
	      z0 = svdot_n_u64 (z1, z16, x0),
	      z0 = svdot (z1, z16, x0))

/*
** dot_h0_u64_untied:
**	mov	(z[0-9]+\.h), h0
**	movprfx	z1, z0
**	udot	z1\.d, z16\.h, \1
**	ret
*/
TEST_DUAL_ZS (dot_h0_u64_untied, svuint64_t, svuint16_t, uint16_t,
	      z1 = svdot_n_u64 (z0, z16, d0),
	      z1 = svdot (z0, z16, d0))

/*
** dot_2_u64_untied:
**	mov	(z[0-9]+\.h), #2
**	movprfx	z0, z1
**	udot	z0\.d, z16\.h, \1
**	ret
*/
TEST_DUAL_Z (dot_2_u64_untied, svuint64_t, svuint16_t,
	     z0 = svdot_n_u64 (z1, z16, 2),
	     z0 = svdot (z1, z16, 2))

/*
** dot_m1_u64:
**	mov	(z[0-9]+)\.b, #-1
**	udot	z0\.d, z16\.h, \1\.h
**	ret
*/
TEST_DUAL_Z (dot_m1_u64, svuint64_t, svuint16_t,
	     z0 = svdot_n_u64 (z0, z16, -1),
	     z0 = svdot (z0, z16, -1))


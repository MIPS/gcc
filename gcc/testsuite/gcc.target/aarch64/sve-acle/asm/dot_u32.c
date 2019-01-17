/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** dot_u32_tied1:
**	udot	z0\.s, z16\.b, z17\.b
**	ret
*/
TEST_DUAL_Z (dot_u32_tied1, svuint32_t, svuint8_t,
	     z0 = svdot_u32 (z0, z16, z17),
	     z0 = svdot (z0, z16, z17))

/*
** dot_u32_tied2:
**	mov	(z[0-9]+)\.d, z16\.d
**	movprfx	z16, z0
**	udot	z16\.s, \1\.b, z17\.b
**	ret
*/
TEST_DUAL_Z (dot_u32_tied2, svuint32_t, svuint8_t,
	     z16_res = svdot_u32 (z0, z16, z17),
	     z16_res = svdot (z0, z16, z17))

/*
** dot_u32_tied3:
**	mov	(z[0-9]+)\.d, z17\.d
**	movprfx	z17, z0
**	udot	z17\.s, z16\.b, \1\.b
**	ret
*/
TEST_DUAL_Z (dot_u32_tied3, svuint32_t, svuint8_t,
	     z17_res = svdot_u32 (z0, z16, z17),
	     z17_res = svdot (z0, z16, z17))

/*
** dot_u32_untied:
**	movprfx	z0, z1
**	udot	z0\.s, z16\.b, z17\.b
**	ret
*/
TEST_DUAL_Z (dot_u32_untied, svuint32_t, svuint8_t,
	     z0 = svdot_u32 (z1, z16, z17),
	     z0 = svdot (z1, z16, z17))

/*
** dot_w0_u32_tied1:
**	mov	(z[0-9]+\.b), w0
**	udot	z0\.s, z16\.b, \1
**	ret
*/
TEST_DUAL_ZS (dot_w0_u32_tied1, svuint32_t, svuint8_t, uint8_t,
	      z0 = svdot_n_u32 (z0, z16, x0),
	      z0 = svdot (z0, z16, x0))

/*
** dot_w0_u32_tied2:
**	mov	(z[0-9]+\.b), w0
**	mov	(z[0-9]+)\.d, z16\.d
**	movprfx	z16, z0
**	udot	z16\.s, \2\.b, \1
**	ret
*/
TEST_DUAL_ZS (dot_w0_u32_tied2, svuint32_t, svuint8_t, uint8_t,
	      z16_res = svdot_n_u32 (z0, z16, x0),
	      z16_res = svdot (z0, z16, x0))

/*
** dot_w0_u32_untied:
**	mov	(z[0-9]+\.b), w0
**	movprfx	z0, z1
**	udot	z0\.s, z16\.b, \1
**	ret
*/
TEST_DUAL_ZS (dot_w0_u32_untied, svuint32_t, svuint8_t, uint8_t,
	      z0 = svdot_n_u32 (z1, z16, x0),
	      z0 = svdot (z1, z16, x0))

/*
** dot_b0_u32_untied:
**	mov	(z[0-9]+\.b), b0
**	movprfx	z1, z0
**	udot	z1\.s, z16\.b, \1
**	ret
*/
TEST_DUAL_ZS (dot_b0_u32_untied, svuint32_t, svuint8_t, uint8_t,
	      z1 = svdot_n_u32 (z0, z16, d0),
	      z1 = svdot (z0, z16, d0))

/*
** dot_2_u32_untied:
**	mov	(z[0-9]+\.b), #2
**	movprfx	z0, z1
**	udot	z0\.s, z16\.b, \1
**	ret
*/
TEST_DUAL_Z (dot_2_u32_untied, svuint32_t, svuint8_t,
	     z0 = svdot_n_u32 (z1, z16, 2),
	     z0 = svdot (z1, z16, 2))

/*
** dot_m1_u32:
**	mov	(z[0-9]+\.b), #-1
**	udot	z0\.s, z16\.b, \1
**	ret
*/
TEST_DUAL_Z (dot_m1_u32, svuint32_t, svuint8_t,
	     z0 = svdot_n_u32 (z0, z16, -1),
	     z0 = svdot (z0, z16, -1))


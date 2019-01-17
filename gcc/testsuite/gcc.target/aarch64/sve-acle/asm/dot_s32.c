/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** dot_s32_tied1:
**	sdot	z0\.s, z16\.b, z17\.b
**	ret
*/
TEST_DUAL_Z (dot_s32_tied1, svint32_t, svint8_t,
	     z0 = svdot_s32 (z0, z16, z17),
	     z0 = svdot (z0, z16, z17))

/*
** dot_s32_tied2:
**	mov	(z[0-9]+)\.d, z16\.d
**	movprfx	z16, z0
**	sdot	z16\.s, \1\.b, z17\.b
**	ret
*/
TEST_DUAL_Z (dot_s32_tied2, svint32_t, svint8_t,
	     z16_res = svdot_s32 (z0, z16, z17),
	     z16_res = svdot (z0, z16, z17))

/*
** dot_s32_tied3:
**	mov	(z[0-9]+)\.d, z17\.d
**	movprfx	z17, z0
**	sdot	z17\.s, z16\.b, \1\.b
**	ret
*/
TEST_DUAL_Z (dot_s32_tied3, svint32_t, svint8_t,
	     z17_res = svdot_s32 (z0, z16, z17),
	     z17_res = svdot (z0, z16, z17))

/*
** dot_s32_untied:
**	movprfx	z0, z1
**	sdot	z0\.s, z16\.b, z17\.b
**	ret
*/
TEST_DUAL_Z (dot_s32_untied, svint32_t, svint8_t,
	     z0 = svdot_s32 (z1, z16, z17),
	     z0 = svdot (z1, z16, z17))

/*
** dot_w0_s32_tied1:
**	mov	(z[0-9]+\.b), w0
**	sdot	z0\.s, z16\.b, \1
**	ret
*/
TEST_DUAL_ZS (dot_w0_s32_tied1, svint32_t, svint8_t, int8_t,
	      z0 = svdot_n_s32 (z0, z16, x0),
	      z0 = svdot (z0, z16, x0))

/*
** dot_w0_s32_tied2:
**	mov	(z[0-9]+\.b), w0
**	mov	(z[0-9]+)\.d, z16\.d
**	movprfx	z16, z0
**	sdot	z16\.s, \2\.b, \1
**	ret
*/
TEST_DUAL_ZS (dot_w0_s32_tied2, svint32_t, svint8_t, int8_t,
	      z16_res = svdot_n_s32 (z0, z16, x0),
	      z16_res = svdot (z0, z16, x0))

/*
** dot_w0_s32_untied:
**	mov	(z[0-9]+\.b), w0
**	movprfx	z0, z1
**	sdot	z0\.s, z16\.b, \1
**	ret
*/
TEST_DUAL_ZS (dot_w0_s32_untied, svint32_t, svint8_t, int8_t,
	      z0 = svdot_n_s32 (z1, z16, x0),
	      z0 = svdot (z1, z16, x0))

/*
** dot_b0_s32_untied:
**	mov	(z[0-9]+\.b), b0
**	movprfx	z1, z0
**	sdot	z1\.s, z16\.b, \1
**	ret
*/
TEST_DUAL_ZS (dot_b0_s32_untied, svint32_t, svint8_t, int8_t,
	      z1 = svdot_n_s32 (z0, z16, d0),
	      z1 = svdot (z0, z16, d0))

/*
** dot_2_s32_untied:
**	mov	(z[0-9]+\.b), #2
**	movprfx	z0, z1
**	sdot	z0\.s, z16\.b, \1
**	ret
*/
TEST_DUAL_Z (dot_2_s32_untied, svint32_t, svint8_t,
	     z0 = svdot_n_s32 (z1, z16, 2),
	     z0 = svdot (z1, z16, 2))

/*
** dot_m1_s32:
**	mov	(z[0-9]+\.b), #-1
**	sdot	z0\.s, z16\.b, \1
**	ret
*/
TEST_DUAL_Z (dot_m1_s32, svint32_t, svint8_t,
	     z0 = svdot_n_s32 (z0, z16, -1),
	     z0 = svdot (z0, z16, -1))


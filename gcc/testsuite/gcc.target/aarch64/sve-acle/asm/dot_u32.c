/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** dot_u32_tied1:
**	udot	z0\.s, z6\.b, z7\.b
**	ret
*/
TEST_DUAL_Z (dot_u32_tied1, svuint32_t, svuint8_t,
	     z0 = svdot_u32 (z0, z6, z7),
	     z0 = svdot (z0, z6, z7))

/*
** dot_u32_tied2:
**	mov	(z[0-9]+)\.d, z6\.d
**	movprfx	z6, z0
**	udot	z6\.s, \1\.b, z7\.b
**	ret
*/
TEST_DUAL_Z (dot_u32_tied2, svuint32_t, svuint8_t,
	     z6_res = svdot_u32 (z0, z6, z7),
	     z6_res = svdot (z0, z6, z7))

/*
** dot_u32_tied3:
**	mov	(z[0-9]+)\.d, z7\.d
**	movprfx	z7, z0
**	udot	z7\.s, z6\.b, \1\.b
**	ret
*/
TEST_DUAL_Z (dot_u32_tied3, svuint32_t, svuint8_t,
	     z7_res = svdot_u32 (z0, z6, z7),
	     z7_res = svdot (z0, z6, z7))

/*
** dot_u32_untied:
**	movprfx	z0, z1
**	udot	z0\.s, z6\.b, z7\.b
**	ret
*/
TEST_DUAL_Z (dot_u32_untied, svuint32_t, svuint8_t,
	     z0 = svdot_u32 (z1, z6, z7),
	     z0 = svdot (z1, z6, z7))

/*
** dot_w0_u32_tied1:
**	mov	(z[0-9]+\.b), w0
**	udot	z0\.s, z6\.b, \1
**	ret
*/
TEST_DUAL_ZS (dot_w0_u32_tied1, svuint32_t, svuint8_t, uint8_t,
	      z0 = svdot_n_u32 (z0, z6, x0),
	      z0 = svdot (z0, z6, x0))

/*
** dot_w0_u32_tied2:
**	mov	(z[0-9]+\.b), w0
**	mov	(z[0-9]+)\.d, z6\.d
**	movprfx	z6, z0
**	udot	z6\.s, \2\.b, \1
**	ret
*/
TEST_DUAL_ZS (dot_w0_u32_tied2, svuint32_t, svuint8_t, uint8_t,
	      z6_res = svdot_n_u32 (z0, z6, x0),
	      z6_res = svdot (z0, z6, x0))

/*
** dot_w0_u32_untied:
**	mov	(z[0-9]+\.b), w0
**	movprfx	z0, z1
**	udot	z0\.s, z6\.b, \1
**	ret
*/
TEST_DUAL_ZS (dot_w0_u32_untied, svuint32_t, svuint8_t, uint8_t,
	      z0 = svdot_n_u32 (z1, z6, x0),
	      z0 = svdot (z1, z6, x0))

/*
** dot_b4_u32_untied:
**	mov	(z[0-9]+\.b), b4
**	movprfx	z0, z1
**	udot	z0\.s, z6\.b, \1
**	ret
*/
TEST_DUAL_ZS (dot_b4_u32_untied, svuint32_t, svuint8_t, uint8_t,
	      z0 = svdot_n_u32 (z1, z6, d4),
	      z0 = svdot (z1, z6, d4))

/*
** dot_2_u32_untied:
**	mov	(z[0-9]+\.b), #2
**	movprfx	z0, z1
**	udot	z0\.s, z6\.b, \1
**	ret
*/
TEST_DUAL_Z (dot_2_u32_untied, svuint32_t, svuint8_t,
	     z0 = svdot_n_u32 (z1, z6, 2),
	     z0 = svdot (z1, z6, 2))

/*
** dot_m1_u32:
**	mov	(z[0-9]+\.b), #-1
**	udot	z0\.s, z6\.b, \1
**	ret
*/
TEST_DUAL_Z (dot_m1_u32, svuint32_t, svuint8_t,
	     z0 = svdot_n_u32 (z0, z6, -1),
	     z0 = svdot (z0, z6, -1))

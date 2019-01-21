/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** dot_lane_0_u32_tied1:
**	udot	z0\.s, z6\.b, z7\.b\[0\]
**	ret
*/
TEST_DUAL_Z (dot_lane_0_u32_tied1, svuint32_t, svuint8_t,
	     z0 = svdot_lane_u32 (z0, z6, z7, 0),
	     z0 = svdot_lane (z0, z6, z7, 0))

/*
** dot_lane_0_u32_tied2:
**	mov	(z[0-9]+)\.d, z6\.d
**	movprfx	z6, z0
**	udot	z6\.s, \1\.b, z7\.b\[0\]
**	ret
*/
TEST_DUAL_Z (dot_lane_0_u32_tied2, svuint32_t, svuint8_t,
	     z6_res = svdot_lane_u32 (z0, z6, z7, 0),
	     z6_res = svdot_lane (z0, z6, z7, 0))

/*
** dot_lane_0_u32_tied3:
**	mov	(z[0-9]+)\.d, z6\.d
**	movprfx	z6, z0
**	udot	z6\.s, z7\.b, \1\.b\[0\]
**	ret
*/
TEST_DUAL_Z (dot_lane_0_u32_tied3, svuint32_t, svuint8_t,
	     z6_res = svdot_lane_u32 (z0, z7, z6, 0),
	     z6_res = svdot_lane (z0, z7, z6, 0))

/*
** dot_lane_0_u32_untied:
**	movprfx	z0, z1
**	udot	z0\.s, z6\.b, z7\.b\[0\]
**	ret
*/
TEST_DUAL_Z (dot_lane_0_u32_untied, svuint32_t, svuint8_t,
	     z0 = svdot_lane_u32 (z1, z6, z7, 0),
	     z0 = svdot_lane (z1, z6, z7, 0))

/*
** dot_lane_1_u32:
**	udot	z0\.s, z6\.b, z7\.b\[1\]
**	ret
*/
TEST_DUAL_Z (dot_lane_1_u32, svuint32_t, svuint8_t,
	     z0 = svdot_lane_u32 (z0, z6, z7, 1),
	     z0 = svdot_lane (z0, z6, z7, 1))

/*
** dot_lane_2_u32:
**	udot	z0\.s, z6\.b, z7\.b\[2\]
**	ret
*/
TEST_DUAL_Z (dot_lane_2_u32, svuint32_t, svuint8_t,
	     z0 = svdot_lane_u32 (z0, z6, z7, 2),
	     z0 = svdot_lane (z0, z6, z7, 2))

/*
** dot_lane_3_u32:
**	udot	z0\.s, z6\.b, z7\.b\[3\]
**	ret
*/
TEST_DUAL_Z (dot_lane_3_u32, svuint32_t, svuint8_t,
	     z0 = svdot_lane_u32 (z0, z6, z7, 3),
	     z0 = svdot_lane (z0, z6, z7, 3))

/*
** dot_lane_z8_u32:
**	str	d8, \[sp, -16\]!
**	mov	(z[0-7])\.d, z8\.d
**	udot	z0\.s, z6\.b, \1\.b\[1\]
**	ldr	d8, \[sp\], 16
**	ret
*/
TEST_DUAL_LANE_REG (dot_lane_z8_u32, svuint32_t, svuint8_t, z8,
		    z0 = svdot_lane_u32 (z0, z6, z8, 1),
		    z0 = svdot_lane (z0, z6, z8, 1))

/*
** dot_lane_z16_u32:
**	mov	(z[0-7])\.d, z16\.d
**	udot	z0\.s, z6\.b, \1\.b\[1\]
**	ret
*/
TEST_DUAL_LANE_REG (dot_lane_z16_u32, svuint32_t, svuint8_t, z16,
		    z0 = svdot_lane_u32 (z0, z6, z16, 1),
		    z0 = svdot_lane (z0, z6, z16, 1))

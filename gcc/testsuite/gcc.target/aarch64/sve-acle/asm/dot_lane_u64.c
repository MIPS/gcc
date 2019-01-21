/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** dot_lane_0_u64_tied1:
**	udot	z0\.d, z6\.h, z7\.h\[0\]
**	ret
*/
TEST_DUAL_Z (dot_lane_0_u64_tied1, svuint64_t, svuint16_t,
	     z0 = svdot_lane_u64 (z0, z6, z7, 0),
	     z0 = svdot_lane (z0, z6, z7, 0))

/*
** dot_lane_0_u64_tied2:
**	mov	(z[0-9]+)\.d, z6\.d
**	movprfx	z6, z0
**	udot	z6\.d, \1\.h, z7\.h\[0\]
**	ret
*/
TEST_DUAL_Z (dot_lane_0_u64_tied2, svuint64_t, svuint16_t,
	     z6_res = svdot_lane_u64 (z0, z6, z7, 0),
	     z6_res = svdot_lane (z0, z6, z7, 0))

/*
** dot_lane_0_u64_tied3:
**	mov	(z[0-9]+)\.d, z6\.d
**	movprfx	z6, z0
**	udot	z6\.d, z7\.h, \1\.h\[0\]
**	ret
*/
TEST_DUAL_Z (dot_lane_0_u64_tied3, svuint64_t, svuint16_t,
	     z6_res = svdot_lane_u64 (z0, z7, z6, 0),
	     z6_res = svdot_lane (z0, z7, z6, 0))

/*
** dot_lane_0_u64_untied:
**	movprfx	z0, z1
**	udot	z0\.d, z6\.h, z7\.h\[0\]
**	ret
*/
TEST_DUAL_Z (dot_lane_0_u64_untied, svuint64_t, svuint16_t,
	     z0 = svdot_lane_u64 (z1, z6, z7, 0),
	     z0 = svdot_lane (z1, z6, z7, 0))

/*
** dot_lane_1_u64:
**	udot	z0\.d, z6\.h, z7\.h\[1\]
**	ret
*/
TEST_DUAL_Z (dot_lane_1_u64, svuint64_t, svuint16_t,
	     z0 = svdot_lane_u64 (z0, z6, z7, 1),
	     z0 = svdot_lane (z0, z6, z7, 1))

/*
** dot_lane_z15_u64:
**	str	d15, \[sp, -16\]!
**	udot	z0\.d, z6\.h, z15\.h\[1\]
**	ldr	d15, \[sp\], 16
**	ret
*/
TEST_DUAL_LANE_REG (dot_lane_z15_u64, svuint64_t, svuint16_t, z15,
		    z0 = svdot_lane_u64 (z0, z6, z15, 1),
		    z0 = svdot_lane (z0, z6, z15, 1))

/*
** dot_lane_z16_u64:
**	mov	(z[0-9]|z1[0-5])\.d, z16\.d
**	udot	z0\.d, z6\.h, \1\.h\[1\]
**	ret
*/
TEST_DUAL_LANE_REG (dot_lane_z16_u64, svuint64_t, svuint16_t, z16,
		    z0 = svdot_lane_u64 (z0, z6, z16, 1),
		    z0 = svdot_lane (z0, z6, z16, 1))

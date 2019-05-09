/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** dot_lane_0_s64_tied1:
**	sdot	z0\.d, z16\.h, z17\.h\[0\]
**	ret
*/
TEST_DUAL_Z (dot_lane_0_s64_tied1, svint64_t, svint16_t,
	     z0 = svdot_lane_s64 (z0, z16, z17, 0),
	     z0 = svdot_lane (z0, z16, z17, 0))

/*
** dot_lane_0_s64_tied2:
**	mov	(z[0-9]+)\.d, z16\.d
**	movprfx	z16, z0
**	sdot	z16\.d, \1\.h, z17\.h\[0\]
**	ret
*/
TEST_DUAL_Z (dot_lane_0_s64_tied2, svint64_t, svint16_t,
	     z16_res = svdot_lane_s64 (z0, z16, z17, 0),
	     z16_res = svdot_lane (z0, z16, z17, 0))

/*
** dot_lane_0_s64_tied3:
**	mov	(z[0-9]+)\.d, z16\.d
**	movprfx	z16, z0
**	sdot	z16\.d, z17\.h, \1\.h\[0\]
**	ret
*/
TEST_DUAL_Z (dot_lane_0_s64_tied3, svint64_t, svint16_t,
	     z16_res = svdot_lane_s64 (z0, z17, z16, 0),
	     z16_res = svdot_lane (z0, z17, z16, 0))

/*
** dot_lane_0_s64_untied:
**	movprfx	z0, z1
**	sdot	z0\.d, z16\.h, z17\.h\[0\]
**	ret
*/
TEST_DUAL_Z (dot_lane_0_s64_untied, svint64_t, svint16_t,
	     z0 = svdot_lane_s64 (z1, z16, z17, 0),
	     z0 = svdot_lane (z1, z16, z17, 0))

/*
** dot_lane_1_s64:
**	sdot	z0\.d, z16\.h, z17\.h\[1\]
**	ret
*/
TEST_DUAL_Z (dot_lane_1_s64, svint64_t, svint16_t,
	     z0 = svdot_lane_s64 (z0, z16, z17, 1),
	     z0 = svdot_lane (z0, z16, z17, 1))

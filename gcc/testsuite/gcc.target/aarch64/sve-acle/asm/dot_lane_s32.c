/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** dot_lane_0_s32_tied1:
**	sdot	z0\.s, z16\.b, z17\.b\[0\]
**	ret
*/
TEST_DUAL_Z (dot_lane_0_s32_tied1, svint32_t, svint8_t,
	     z0 = svdot_lane_s32 (z0, z16, z17, 0),
	     z0 = svdot_lane (z0, z16, z17, 0))

/*
** dot_lane_0_s32_tied2:
**	mov	(z[0-9]+)\.d, z16\.d
**	movprfx	z16, z0
**	sdot	z16\.s, \1\.b, z17\.b\[0\]
**	ret
*/
TEST_DUAL_Z (dot_lane_0_s32_tied2, svint32_t, svint8_t,
	     z16_res = svdot_lane_s32 (z0, z16, z17, 0),
	     z16_res = svdot_lane (z0, z16, z17, 0))

/*
** dot_lane_0_s32_tied3:
**	mov	(z[0-9]+)\.d, z16\.d
**	movprfx	z16, z0
**	sdot	z16\.s, z17\.b, \1\.b\[0\]
**	ret
*/
TEST_DUAL_Z (dot_lane_0_s32_tied3, svint32_t, svint8_t,
	     z16_res = svdot_lane_s32 (z0, z17, z16, 0),
	     z16_res = svdot_lane (z0, z17, z16, 0))

/*
** dot_lane_0_s32_untied:
**	movprfx	z0, z1
**	sdot	z0\.s, z16\.b, z17\.b\[0\]
**	ret
*/
TEST_DUAL_Z (dot_lane_0_s32_untied, svint32_t, svint8_t,
	     z0 = svdot_lane_s32 (z1, z16, z17, 0),
	     z0 = svdot_lane (z1, z16, z17, 0))

/*
** dot_lane_1_s32:
**	sdot	z0\.s, z16\.b, z17\.b\[1\]
**	ret
*/
TEST_DUAL_Z (dot_lane_1_s32, svint32_t, svint8_t,
	     z0 = svdot_lane_s32 (z0, z16, z17, 1),
	     z0 = svdot_lane (z0, z16, z17, 1))

/*
** dot_lane_2_s32:
**	sdot	z0\.s, z16\.b, z17\.b\[2\]
**	ret
*/
TEST_DUAL_Z (dot_lane_2_s32, svint32_t, svint8_t,
	     z0 = svdot_lane_s32 (z0, z16, z17, 2),
	     z0 = svdot_lane (z0, z16, z17, 2))

/*
** dot_lane_3_s32:
**	sdot	z0\.s, z16\.b, z17\.b\[3\]
**	ret
*/
TEST_DUAL_Z (dot_lane_3_s32, svint32_t, svint8_t,
	     z0 = svdot_lane_s32 (z0, z16, z17, 3),
	     z0 = svdot_lane (z0, z16, z17, 3))

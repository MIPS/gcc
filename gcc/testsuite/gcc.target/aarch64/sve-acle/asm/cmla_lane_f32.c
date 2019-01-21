/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** cmla_lane_0_0_f32_tied1:
**	fcmla	z0\.s, z1\.s, z2\.s\[0\], #0
**	ret
*/
TEST_UNIFORM_Z (cmla_lane_0_0_f32_tied1, svfloat32_t,
		z0 = svcmla_lane_f32 (z0, z1, z2, 0, 0),
		z0 = svcmla_lane (z0, z1, z2, 0, 0))

/*
** cmla_lane_0_0_f32_tied2:
**	mov	(z[0-9]+)\.d, z0\.d
**	movprfx	z0, z1
**	fcmla	z0\.s, \1\.s, z2\.s\[0\], #0
**	ret
*/
TEST_UNIFORM_Z (cmla_lane_0_0_f32_tied2, svfloat32_t,
		z0 = svcmla_lane_f32 (z1, z0, z2, 0, 0),
		z0 = svcmla_lane (z1, z0, z2, 0, 0))

/*
** cmla_lane_0_0_f32_tied3:
**	mov	(z[0-9]+)\.d, z0\.d
**	movprfx	z0, z1
**	fcmla	z0\.s, z2\.s, \1\.s\[0\], #0
**	ret
*/
TEST_UNIFORM_Z (cmla_lane_0_0_f32_tied3, svfloat32_t,
		z0 = svcmla_lane_f32 (z1, z2, z0, 0, 0),
		z0 = svcmla_lane (z1, z2, z0, 0, 0))

/*
** cmla_lane_0_0_f32_untied:
**	movprfx	z0, z1
**	fcmla	z0\.s, z2\.s, z3\.s\[0\], #0
**	ret
*/
TEST_UNIFORM_Z (cmla_lane_0_0_f32_untied, svfloat32_t,
		z0 = svcmla_lane_f32 (z1, z2, z3, 0, 0),
		z0 = svcmla_lane (z1, z2, z3, 0, 0))

/*
** cmla_lane_0_90_f32_tied1:
**	fcmla	z0\.s, z1\.s, z2\.s\[0\], #90
**	ret
*/
TEST_UNIFORM_Z (cmla_lane_0_90_f32_tied1, svfloat32_t,
		z0 = svcmla_lane_f32 (z0, z1, z2, 0, 90),
		z0 = svcmla_lane (z0, z1, z2, 0, 90))

/*
** cmla_lane_0_90_f32_tied2:
**	mov	(z[0-9]+)\.d, z0\.d
**	movprfx	z0, z1
**	fcmla	z0\.s, \1\.s, z2\.s\[0\], #90
**	ret
*/
TEST_UNIFORM_Z (cmla_lane_0_90_f32_tied2, svfloat32_t,
		z0 = svcmla_lane_f32 (z1, z0, z2, 0, 90),
		z0 = svcmla_lane (z1, z0, z2, 0, 90))

/*
** cmla_lane_0_90_f32_tied3:
**	mov	(z[0-9]+)\.d, z0\.d
**	movprfx	z0, z1
**	fcmla	z0\.s, z2\.s, \1\.s\[0\], #90
**	ret
*/
TEST_UNIFORM_Z (cmla_lane_0_90_f32_tied3, svfloat32_t,
		z0 = svcmla_lane_f32 (z1, z2, z0, 0, 90),
		z0 = svcmla_lane (z1, z2, z0, 0, 90))

/*
** cmla_lane_0_90_f32_untied:
**	movprfx	z0, z1
**	fcmla	z0\.s, z2\.s, z3\.s\[0\], #90
**	ret
*/
TEST_UNIFORM_Z (cmla_lane_0_90_f32_untied, svfloat32_t,
		z0 = svcmla_lane_f32 (z1, z2, z3, 0, 90),
		z0 = svcmla_lane (z1, z2, z3, 0, 90))

/*
** cmla_lane_0_180_f32_tied1:
**	fcmla	z0\.s, z1\.s, z2\.s\[0\], #180
**	ret
*/
TEST_UNIFORM_Z (cmla_lane_0_180_f32_tied1, svfloat32_t,
		z0 = svcmla_lane_f32 (z0, z1, z2, 0, 180),
		z0 = svcmla_lane (z0, z1, z2, 0, 180))

/*
** cmla_lane_0_180_f32_tied2:
**	mov	(z[0-9]+)\.d, z0\.d
**	movprfx	z0, z1
**	fcmla	z0\.s, \1\.s, z2\.s\[0\], #180
**	ret
*/
TEST_UNIFORM_Z (cmla_lane_0_180_f32_tied2, svfloat32_t,
		z0 = svcmla_lane_f32 (z1, z0, z2, 0, 180),
		z0 = svcmla_lane (z1, z0, z2, 0, 180))

/*
** cmla_lane_0_180_f32_tied3:
**	mov	(z[0-9]+)\.d, z0\.d
**	movprfx	z0, z1
**	fcmla	z0\.s, z2\.s, \1\.s\[0\], #180
**	ret
*/
TEST_UNIFORM_Z (cmla_lane_0_180_f32_tied3, svfloat32_t,
		z0 = svcmla_lane_f32 (z1, z2, z0, 0, 180),
		z0 = svcmla_lane (z1, z2, z0, 0, 180))

/*
** cmla_lane_0_180_f32_untied:
**	movprfx	z0, z1
**	fcmla	z0\.s, z2\.s, z3\.s\[0\], #180
**	ret
*/
TEST_UNIFORM_Z (cmla_lane_0_180_f32_untied, svfloat32_t,
		z0 = svcmla_lane_f32 (z1, z2, z3, 0, 180),
		z0 = svcmla_lane (z1, z2, z3, 0, 180))

/*
** cmla_lane_0_270_f32_tied1:
**	fcmla	z0\.s, z1\.s, z2\.s\[0\], #270
**	ret
*/
TEST_UNIFORM_Z (cmla_lane_0_270_f32_tied1, svfloat32_t,
		z0 = svcmla_lane_f32 (z0, z1, z2, 0, 270),
		z0 = svcmla_lane (z0, z1, z2, 0, 270))

/*
** cmla_lane_0_270_f32_tied2:
**	mov	(z[0-9]+)\.d, z0\.d
**	movprfx	z0, z1
**	fcmla	z0\.s, \1\.s, z2\.s\[0\], #270
**	ret
*/
TEST_UNIFORM_Z (cmla_lane_0_270_f32_tied2, svfloat32_t,
		z0 = svcmla_lane_f32 (z1, z0, z2, 0, 270),
		z0 = svcmla_lane (z1, z0, z2, 0, 270))

/*
** cmla_lane_0_270_f32_tied3:
**	mov	(z[0-9]+)\.d, z0\.d
**	movprfx	z0, z1
**	fcmla	z0\.s, z2\.s, \1\.s\[0\], #270
**	ret
*/
TEST_UNIFORM_Z (cmla_lane_0_270_f32_tied3, svfloat32_t,
		z0 = svcmla_lane_f32 (z1, z2, z0, 0, 270),
		z0 = svcmla_lane (z1, z2, z0, 0, 270))

/*
** cmla_lane_0_270_f32_untied:
**	movprfx	z0, z1
**	fcmla	z0\.s, z2\.s, z3\.s\[0\], #270
**	ret
*/
TEST_UNIFORM_Z (cmla_lane_0_270_f32_untied, svfloat32_t,
		z0 = svcmla_lane_f32 (z1, z2, z3, 0, 270),
		z0 = svcmla_lane (z1, z2, z3, 0, 270))

/*
** cmla_lane_z7_f32_untied:
**	movprfx	z0, z1
**	fcmla	z0\.s, z2\.s, z7\.s\[1\], #270
**	ret
*/
TEST_DUAL_Z (cmla_lane_z7_f32_untied, svfloat32_t, svfloat32_t,
	     z0 = svcmla_lane_f32 (z1, z2, z7, 1, 270),
	     z0 = svcmla_lane (z1, z2, z7, 1, 270))

/*
** cmla_lane_z15_f32_untied:
**	str	d15, \[sp, -16\]!
**	movprfx	z0, z1
**	fcmla	z0\.s, z2\.s, z15\.s\[1\], #270
**	ldr	d15, \[sp\], 16
**	ret
*/
TEST_DUAL_LANE_REG (cmla_lane_z15_f32_untied, svfloat32_t, svfloat32_t, z15,
		    z0 = svcmla_lane_f32 (z1, z2, z15, 1, 270),
		    z0 = svcmla_lane (z1, z2, z15, 1, 270))

/*
** cmla_lane_z16_f32_untied:
**	mov	(z[0-9]|z1[0-5])\.d, z16\.d
**	movprfx	z0, z1
**	fcmla	z0\.s, z2\.s, \1\.s\[1\], #270
**	ret
*/
TEST_DUAL_LANE_REG (cmla_lane_z16_f32_untied, svfloat32_t, svfloat32_t, z16,
		    z0 = svcmla_lane_f32 (z1, z2, z16, 1, 270),
		    z0 = svcmla_lane (z1, z2, z16, 1, 270))

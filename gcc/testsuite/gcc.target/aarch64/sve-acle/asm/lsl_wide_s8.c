/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** lsl_wide_s8_m_tied1:
**	lsl	z0\.b, p0/m, z0\.b, z16\.d
**	ret
*/
TEST_WIDE_Z (lsl_wide_s8_m_tied1, svint8_t,
	     z0 = svlsl_wide_s8_m (p0, z0, z16),
	     z0 = svlsl_wide_m (p0, z0, z16))

/*
** lsl_wide_s8_m_tied2:
**	mov	(z[0-9]+\.d), z16\.d
**	movprfx	z16, z0
**	lsl	z16\.b, p0/m, z16\.b, \1
**	ret
*/
TEST_WIDE_Z (lsl_wide_s8_m_tied2, svint8_t,
	     z16_res = svlsl_wide_s8_m (p0, z0, z16),
	     z16_res = svlsl_wide_m (p0, z0, z16))

/*
** lsl_wide_s8_m_untied:
**	movprfx	z0, z1
**	lsl	z0\.b, p0/m, z0\.b, z16\.d
**	ret
*/
TEST_WIDE_Z (lsl_wide_s8_m_untied, svint8_t,
	     z0 = svlsl_wide_s8_m (p0, z1, z16),
	     z0 = svlsl_wide_m (p0, z1, z16))

/*
** lsl_wide_s8_z_tied1:
**	movprfx	z0\.b, p0/z, z0\.b
**	lsl	z0\.b, p0/m, z0\.b, z16\.d
**	ret
*/
TEST_WIDE_Z (lsl_wide_s8_z_tied1, svint8_t,
	     z0 = svlsl_wide_s8_z (p0, z0, z16),
	     z0 = svlsl_wide_z (p0, z0, z16))

/*
** lsl_wide_s8_z_tied2:
**	mov	(z[0-9]+\.d), z16\.d
**	movprfx	z16\.b, p0/z, z0\.b
**	lsl	z16\.b, p0/m, z16\.b, \1
**	ret
*/
TEST_WIDE_Z (lsl_wide_s8_z_tied2, svint8_t,
	     z16_res = svlsl_wide_s8_z (p0, z0, z16),
	     z16_res = svlsl_wide_z (p0, z0, z16))

/*
** lsl_wide_s8_z_untied:
**	movprfx	z0\.b, p0/z, z1\.b
**	lsl	z0\.b, p0/m, z0\.b, z16\.d
**	ret
*/
TEST_WIDE_Z (lsl_wide_s8_z_untied, svint8_t,
	     z0 = svlsl_wide_s8_z (p0, z1, z16),
	     z0 = svlsl_wide_z (p0, z1, z16))

/*
** lsl_wide_s8_x_tied1:
**	lsl	z0\.b, z0\.b, z16\.d
**	ret
*/
TEST_WIDE_Z (lsl_wide_s8_x_tied1, svint8_t,
	     z0 = svlsl_wide_s8_x (p0, z0, z16),
	     z0 = svlsl_wide_x (p0, z0, z16))

/*
** lsl_wide_s8_x_tied2:
**	lsl	z16\.b, z0\.b, z16\.d
**	ret
*/
TEST_WIDE_Z (lsl_wide_s8_x_tied2, svint8_t,
	     z16_res = svlsl_wide_s8_x (p0, z0, z16),
	     z16_res = svlsl_wide_x (p0, z0, z16))

/*
** lsl_wide_s8_x_untied:
**	lsl	z0\.b, z1\.b, z16\.d
**	ret
*/
TEST_WIDE_Z (lsl_wide_s8_x_untied, svint8_t,
	     z0 = svlsl_wide_s8_x (p0, z1, z16),
	     z0 = svlsl_wide_x (p0, z1, z16))

/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** lsl_wide_s16_m_tied1:
**	lsl	z0\.h, p0/m, z0\.h, z6\.d
**	ret
*/
TEST_WIDE_Z (lsl_wide_s16_m_tied1, svint16_t,
	     z0 = svlsl_wide_s16_m (p0, z0, z6),
	     z0 = svlsl_wide_m (p0, z0, z6))

/*
** lsl_wide_s16_m_tied2:
**	mov	(z[0-9]+\.d), z6\.d
**	movprfx	z6, z0
**	lsl	z6\.h, p0/m, z6\.h, \1
**	ret
*/
TEST_WIDE_Z (lsl_wide_s16_m_tied2, svint16_t,
	     z6_res = svlsl_wide_s16_m (p0, z0, z6),
	     z6_res = svlsl_wide_m (p0, z0, z6))

/*
** lsl_wide_s16_m_untied:
**	movprfx	z0, z1
**	lsl	z0\.h, p0/m, z0\.h, z6\.d
**	ret
*/
TEST_WIDE_Z (lsl_wide_s16_m_untied, svint16_t,
	     z0 = svlsl_wide_s16_m (p0, z1, z6),
	     z0 = svlsl_wide_m (p0, z1, z6))

/*
** lsl_wide_s16_z_tied1:
**	movprfx	z0\.h, p0/z, z0\.h
**	lsl	z0\.h, p0/m, z0\.h, z6\.d
**	ret
*/
TEST_WIDE_Z (lsl_wide_s16_z_tied1, svint16_t,
	     z0 = svlsl_wide_s16_z (p0, z0, z6),
	     z0 = svlsl_wide_z (p0, z0, z6))

/*
** lsl_wide_s16_z_tied2:
**	mov	(z[0-9]+\.d), z6\.d
**	movprfx	z6\.h, p0/z, z0\.h
**	lsl	z6\.h, p0/m, z6\.h, \1
**	ret
*/
TEST_WIDE_Z (lsl_wide_s16_z_tied2, svint16_t,
	     z6_res = svlsl_wide_s16_z (p0, z0, z6),
	     z6_res = svlsl_wide_z (p0, z0, z6))

/*
** lsl_wide_s16_z_untied:
**	movprfx	z0\.h, p0/z, z1\.h
**	lsl	z0\.h, p0/m, z0\.h, z6\.d
**	ret
*/
TEST_WIDE_Z (lsl_wide_s16_z_untied, svint16_t,
	     z0 = svlsl_wide_s16_z (p0, z1, z6),
	     z0 = svlsl_wide_z (p0, z1, z6))

/*
** lsl_wide_s16_x_tied1:
**	lsl	z0\.h, z0\.h, z6\.d
**	ret
*/
TEST_WIDE_Z (lsl_wide_s16_x_tied1, svint16_t,
	     z0 = svlsl_wide_s16_x (p0, z0, z6),
	     z0 = svlsl_wide_x (p0, z0, z6))

/*
** lsl_wide_s16_x_tied2:
**	lsl	z6\.h, z0\.h, z6\.d
**	ret
*/
TEST_WIDE_Z (lsl_wide_s16_x_tied2, svint16_t,
	     z6_res = svlsl_wide_s16_x (p0, z0, z6),
	     z6_res = svlsl_wide_x (p0, z0, z6))

/*
** lsl_wide_s16_x_untied:
**	lsl	z0\.h, z1\.h, z6\.d
**	ret
*/
TEST_WIDE_Z (lsl_wide_s16_x_untied, svint16_t,
	     z0 = svlsl_wide_s16_x (p0, z1, z6),
	     z0 = svlsl_wide_x (p0, z1, z6))

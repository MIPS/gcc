/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** lsl_wide_u8_m_tied1:
**	lsl	z0\.b, p0/m, z0\.b, z6\.d
**	ret
*/
TEST_WIDE_Z (lsl_wide_u8_m_tied1, svuint8_t,
	     z0 = svlsl_wide_u8_m (p0, z0, z6),
	     z0 = svlsl_wide_m (p0, z0, z6))

/*
** lsl_wide_u8_m_tied2:
**	mov	(z[0-9]+\.d), z6\.d
**	movprfx	z6, z0
**	lsl	z6\.b, p0/m, z6\.b, \1
**	ret
*/
TEST_WIDE_Z (lsl_wide_u8_m_tied2, svuint8_t,
	     z6_res = svlsl_wide_u8_m (p0, z0, z6),
	     z6_res = svlsl_wide_m (p0, z0, z6))

/*
** lsl_wide_u8_m_untied:
**	movprfx	z0, z1
**	lsl	z0\.b, p0/m, z0\.b, z6\.d
**	ret
*/
TEST_WIDE_Z (lsl_wide_u8_m_untied, svuint8_t,
	     z0 = svlsl_wide_u8_m (p0, z1, z6),
	     z0 = svlsl_wide_m (p0, z1, z6))

/*
** lsl_wide_u8_z_tied1:
**	movprfx	z0\.b, p0/z, z0\.b
**	lsl	z0\.b, p0/m, z0\.b, z6\.d
**	ret
*/
TEST_WIDE_Z (lsl_wide_u8_z_tied1, svuint8_t,
	     z0 = svlsl_wide_u8_z (p0, z0, z6),
	     z0 = svlsl_wide_z (p0, z0, z6))

/*
** lsl_wide_u8_z_tied2:
**	mov	(z[0-9]+\.d), z6\.d
**	movprfx	z6\.b, p0/z, z0\.b
**	lsl	z6\.b, p0/m, z6\.b, \1
**	ret
*/
TEST_WIDE_Z (lsl_wide_u8_z_tied2, svuint8_t,
	     z6_res = svlsl_wide_u8_z (p0, z0, z6),
	     z6_res = svlsl_wide_z (p0, z0, z6))

/*
** lsl_wide_u8_z_untied:
**	movprfx	z0\.b, p0/z, z1\.b
**	lsl	z0\.b, p0/m, z0\.b, z6\.d
**	ret
*/
TEST_WIDE_Z (lsl_wide_u8_z_untied, svuint8_t,
	     z0 = svlsl_wide_u8_z (p0, z1, z6),
	     z0 = svlsl_wide_z (p0, z1, z6))

/*
** lsl_wide_u8_x_tied1:
**	lsl	z0\.b, z0\.b, z6\.d
**	ret
*/
TEST_WIDE_Z (lsl_wide_u8_x_tied1, svuint8_t,
	     z0 = svlsl_wide_u8_x (p0, z0, z6),
	     z0 = svlsl_wide_x (p0, z0, z6))

/*
** lsl_wide_u8_x_tied2:
**	lsl	z6\.b, z0\.b, z6\.d
**	ret
*/
TEST_WIDE_Z (lsl_wide_u8_x_tied2, svuint8_t,
	     z6_res = svlsl_wide_u8_x (p0, z0, z6),
	     z6_res = svlsl_wide_x (p0, z0, z6))

/*
** lsl_wide_u8_x_untied:
**	lsl	z0\.b, z1\.b, z6\.d
**	ret
*/
TEST_WIDE_Z (lsl_wide_u8_x_untied, svuint8_t,
	     z0 = svlsl_wide_u8_x (p0, z1, z6),
	     z0 = svlsl_wide_x (p0, z1, z6))

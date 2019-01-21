/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** lsl_wide_u32_m_tied1:
**	lsl	z0\.s, p0/m, z0\.s, z6\.d
**	ret
*/
TEST_WIDE_Z (lsl_wide_u32_m_tied1, svuint32_t,
	     z0 = svlsl_wide_u32_m (p0, z0, z6),
	     z0 = svlsl_wide_m (p0, z0, z6))

/*
** lsl_wide_u32_m_tied2:
**	mov	(z[0-9]+\.d), z6\.d
**	movprfx	z6, z0
**	lsl	z6\.s, p0/m, z6\.s, \1
**	ret
*/
TEST_WIDE_Z (lsl_wide_u32_m_tied2, svuint32_t,
	     z6_res = svlsl_wide_u32_m (p0, z0, z6),
	     z6_res = svlsl_wide_m (p0, z0, z6))

/*
** lsl_wide_u32_m_untied:
**	movprfx	z0, z1
**	lsl	z0\.s, p0/m, z0\.s, z6\.d
**	ret
*/
TEST_WIDE_Z (lsl_wide_u32_m_untied, svuint32_t,
	     z0 = svlsl_wide_u32_m (p0, z1, z6),
	     z0 = svlsl_wide_m (p0, z1, z6))

/*
** lsl_wide_u32_z_tied1:
**	movprfx	z0\.s, p0/z, z0\.s
**	lsl	z0\.s, p0/m, z0\.s, z6\.d
**	ret
*/
TEST_WIDE_Z (lsl_wide_u32_z_tied1, svuint32_t,
	     z0 = svlsl_wide_u32_z (p0, z0, z6),
	     z0 = svlsl_wide_z (p0, z0, z6))

/*
** lsl_wide_u32_z_tied2:
**	mov	(z[0-9]+\.d), z6\.d
**	movprfx	z6\.s, p0/z, z0\.s
**	lsl	z6\.s, p0/m, z6\.s, \1
**	ret
*/
TEST_WIDE_Z (lsl_wide_u32_z_tied2, svuint32_t,
	     z6_res = svlsl_wide_u32_z (p0, z0, z6),
	     z6_res = svlsl_wide_z (p0, z0, z6))

/*
** lsl_wide_u32_z_untied:
**	movprfx	z0\.s, p0/z, z1\.s
**	lsl	z0\.s, p0/m, z0\.s, z6\.d
**	ret
*/
TEST_WIDE_Z (lsl_wide_u32_z_untied, svuint32_t,
	     z0 = svlsl_wide_u32_z (p0, z1, z6),
	     z0 = svlsl_wide_z (p0, z1, z6))

/*
** lsl_wide_u32_x_tied1:
**	lsl	z0\.s, z0\.s, z6\.d
**	ret
*/
TEST_WIDE_Z (lsl_wide_u32_x_tied1, svuint32_t,
	     z0 = svlsl_wide_u32_x (p0, z0, z6),
	     z0 = svlsl_wide_x (p0, z0, z6))

/*
** lsl_wide_u32_x_tied2:
**	lsl	z6\.s, z0\.s, z6\.d
**	ret
*/
TEST_WIDE_Z (lsl_wide_u32_x_tied2, svuint32_t,
	     z6_res = svlsl_wide_u32_x (p0, z0, z6),
	     z6_res = svlsl_wide_x (p0, z0, z6))

/*
** lsl_wide_u32_x_untied:
**	lsl	z0\.s, z1\.s, z6\.d
**	ret
*/
TEST_WIDE_Z (lsl_wide_u32_x_untied, svuint32_t,
	     z0 = svlsl_wide_u32_x (p0, z1, z6),
	     z0 = svlsl_wide_x (p0, z1, z6))

/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** lsr_wide_u32_m_tied1:
**	lsr	z0\.s, p0/m, z0\.s, z16\.d
**	ret
*/
TEST_WIDE_Z (lsr_wide_u32_m_tied1, svuint32_t,
	     z0 = svlsr_wide_u32_m (p0, z0, z16),
	     z0 = svlsr_wide_m (p0, z0, z16))

/*
** lsr_wide_u32_m_tied2:
**	mov	(z[0-9]+\.d), z16\.d
**	movprfx	z16, z0
**	lsr	z16\.s, p0/m, z16\.s, \1
**	ret
*/
TEST_WIDE_Z (lsr_wide_u32_m_tied2, svuint32_t,
	     z16_res = svlsr_wide_u32_m (p0, z0, z16),
	     z16_res = svlsr_wide_m (p0, z0, z16))

/*
** lsr_wide_u32_m_untied:
**	movprfx	z0, z1
**	lsr	z0\.s, p0/m, z0\.s, z16\.d
**	ret
*/
TEST_WIDE_Z (lsr_wide_u32_m_untied, svuint32_t,
	     z0 = svlsr_wide_u32_m (p0, z1, z16),
	     z0 = svlsr_wide_m (p0, z1, z16))

/*
** lsr_wide_u32_z_tied1:
**	movprfx	z0\.s, p0/z, z0\.s
**	lsr	z0\.s, p0/m, z0\.s, z16\.d
**	ret
*/
TEST_WIDE_Z (lsr_wide_u32_z_tied1, svuint32_t,
	     z0 = svlsr_wide_u32_z (p0, z0, z16),
	     z0 = svlsr_wide_z (p0, z0, z16))

/*
** lsr_wide_u32_z_tied2:
**	mov	(z[0-9]+\.d), z16\.d
**	movprfx	z16\.s, p0/z, z0\.s
**	lsr	z16\.s, p0/m, z16\.s, \1
**	ret
*/
TEST_WIDE_Z (lsr_wide_u32_z_tied2, svuint32_t,
	     z16_res = svlsr_wide_u32_z (p0, z0, z16),
	     z16_res = svlsr_wide_z (p0, z0, z16))

/*
** lsr_wide_u32_z_untied:
**	movprfx	z0\.s, p0/z, z1\.s
**	lsr	z0\.s, p0/m, z0\.s, z16\.d
**	ret
*/
TEST_WIDE_Z (lsr_wide_u32_z_untied, svuint32_t,
	     z0 = svlsr_wide_u32_z (p0, z1, z16),
	     z0 = svlsr_wide_z (p0, z1, z16))

/*
** lsr_wide_u32_x_tied1:
**	lsr	z0\.s, z0\.s, z16\.d
**	ret
*/
TEST_WIDE_Z (lsr_wide_u32_x_tied1, svuint32_t,
	     z0 = svlsr_wide_u32_x (p0, z0, z16),
	     z0 = svlsr_wide_x (p0, z0, z16))

/*
** lsr_wide_u32_x_tied2:
**	lsr	z16\.s, z0\.s, z16\.d
**	ret
*/
TEST_WIDE_Z (lsr_wide_u32_x_tied2, svuint32_t,
	     z16_res = svlsr_wide_u32_x (p0, z0, z16),
	     z16_res = svlsr_wide_x (p0, z0, z16))

/*
** lsr_wide_u32_x_untied:
**	lsr	z0\.s, z1\.s, z16\.d
**	ret
*/
TEST_WIDE_Z (lsr_wide_u32_x_untied, svuint32_t,
	     z0 = svlsr_wide_u32_x (p0, z1, z16),
	     z0 = svlsr_wide_x (p0, z1, z16))

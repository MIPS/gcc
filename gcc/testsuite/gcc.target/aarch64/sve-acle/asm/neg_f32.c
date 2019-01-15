/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** neg_f32_m_tied12:
**	fneg	z0\.s, p0/m, z0\.s
**	ret
*/
TEST_UNIFORM_Z (neg_f32_m_tied12, svfloat32_t,
		z0 = svneg_f32_m (z0, p0, z0),
		z0 = svneg_m (z0, p0, z0))

/*
** neg_f32_m_tied1:
**	fneg	z0\.s, p0/m, z1\.s
**	ret
*/
TEST_UNIFORM_Z (neg_f32_m_tied1, svfloat32_t,
		z0 = svneg_f32_m (z0, p0, z1),
		z0 = svneg_m (z0, p0, z1))

/*
** neg_f32_m_tied2:
**	mov	(z[0-9]+)\.d, z0\.d
**	movprfx	z0, z1
**	fneg	z0\.s, p0/m, \1\.s
**	ret
*/
TEST_UNIFORM_Z (neg_f32_m_tied2, svfloat32_t,
		z0 = svneg_f32_m (z1, p0, z0),
		z0 = svneg_m (z1, p0, z0))

/*
** neg_f32_m_untied:
**	movprfx	z0, z2
**	fneg	z0\.s, p0/m, z1\.s
**	ret
*/
TEST_UNIFORM_Z (neg_f32_m_untied, svfloat32_t,
		z0 = svneg_f32_m (z2, p0, z1),
		z0 = svneg_m (z2, p0, z1))

/*
** neg_f32_z_tied1:
**	mov	(z[0-9]+)\.d, z0\.d
**	movprfx	z0\.s, p0/z, z0\.s
**	fneg	z0\.s, p0/m, \1\.s
**	ret
*/
TEST_UNIFORM_Z (neg_f32_z_tied1, svfloat32_t,
		z0 = svneg_f32_z (p0, z0),
		z0 = svneg_z (p0, z0))

/*
** neg_f32_z_untied:
**	movprfx	z0\.s, p0/z, z0\.s
**	fneg	z0\.s, p0/m, z1\.s
**	ret
*/
TEST_UNIFORM_Z (neg_f32_z_untied, svfloat32_t,
		z0 = svneg_f32_z (p0, z1),
		z0 = svneg_z (p0, z1))

/*
** neg_f32_x_tied1:
**	fneg	z0\.s, p0/m, z0\.s
**	ret
*/
TEST_UNIFORM_Z (neg_f32_x_tied1, svfloat32_t,
		z0 = svneg_f32_x (p0, z0),
		z0 = svneg_x (p0, z0))

/*
** neg_f32_x_untied:
**	fneg	z2\.s, p0/m, z0\.s
**	ret
*/
TEST_UNIFORM_Z (neg_f32_x_untied, svfloat32_t,
		z2 = svneg_f32_x (p0, z0),
		z2 = svneg_x (p0, z0))

/*
** ptrue_neg_f32_x_tied1:
**	ptrue	(p[0-7])\.s[^\n]*
**	fneg	z0\.s, \1/m, z0\.s
**	ret
*/
TEST_UNIFORM_Z (ptrue_neg_f32_x_tied1, svfloat32_t,
		z0 = svneg_f32_x (svptrue_b32 (), z0),
		z0 = svneg_x (svptrue_b32 (), z0))

/*
** ptrue_neg_f32_x_untied:
**	ptrue	(p[0-7])\.s[^\n]*
**	fneg	z2\.s, \1/m, z0\.s
**	ret
*/
TEST_UNIFORM_Z (ptrue_neg_f32_x_untied, svfloat32_t,
		z2 = svneg_f32_x (svptrue_b32 (), z0),
		z2 = svneg_x (svptrue_b32 (), z0))

/*
** ptrue_b8_neg_f32_x_tied1:
**	ptrue	(p[0-7])\.s[^\n]*
**	fneg	z0\.s, \1/m, z0\.s
**	ret
*/
TEST_UNIFORM_Z (ptrue_b8_neg_f32_x_tied1, svfloat32_t,
		z0 = svneg_f32_x (svptrue_b8 (), z0),
		z0 = svneg_x (svptrue_b8 (), z0))

/*
** ptrue_b8_neg_f32_x_untied:
**	ptrue	(p[0-7])\.s[^\n]*
**	fneg	z2\.s, \1/m, z0\.s
**	ret
*/
TEST_UNIFORM_Z (ptrue_b8_neg_f32_x_untied, svfloat32_t,
		z2 = svneg_f32_x (svptrue_b8 (), z0),
		z2 = svneg_x (svptrue_b8 (), z0))

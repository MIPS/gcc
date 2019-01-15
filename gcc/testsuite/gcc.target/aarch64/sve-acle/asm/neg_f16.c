/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** neg_f16_m_tied12:
**	fneg	z0\.h, p0/m, z0\.h
**	ret
*/
TEST_UNIFORM_Z (neg_f16_m_tied12, svfloat16_t,
		z0 = svneg_f16_m (z0, p0, z0),
		z0 = svneg_m (z0, p0, z0))

/*
** neg_f16_m_tied1:
**	fneg	z0\.h, p0/m, z1\.h
**	ret
*/
TEST_UNIFORM_Z (neg_f16_m_tied1, svfloat16_t,
		z0 = svneg_f16_m (z0, p0, z1),
		z0 = svneg_m (z0, p0, z1))

/*
** neg_f16_m_tied2:
**	mov	(z[0-9]+)\.d, z0\.d
**	movprfx	z0, z1
**	fneg	z0\.h, p0/m, \1\.h
**	ret
*/
TEST_UNIFORM_Z (neg_f16_m_tied2, svfloat16_t,
		z0 = svneg_f16_m (z1, p0, z0),
		z0 = svneg_m (z1, p0, z0))

/*
** neg_f16_m_untied:
**	movprfx	z0, z2
**	fneg	z0\.h, p0/m, z1\.h
**	ret
*/
TEST_UNIFORM_Z (neg_f16_m_untied, svfloat16_t,
		z0 = svneg_f16_m (z2, p0, z1),
		z0 = svneg_m (z2, p0, z1))

/*
** neg_f16_z_tied1:
**	mov	(z[0-9]+)\.d, z0\.d
**	movprfx	z0\.h, p0/z, z0\.h
**	fneg	z0\.h, p0/m, \1\.h
**	ret
*/
TEST_UNIFORM_Z (neg_f16_z_tied1, svfloat16_t,
		z0 = svneg_f16_z (p0, z0),
		z0 = svneg_z (p0, z0))

/*
** neg_f16_z_untied:
**	movprfx	z0\.h, p0/z, z0\.h
**	fneg	z0\.h, p0/m, z1\.h
**	ret
*/
TEST_UNIFORM_Z (neg_f16_z_untied, svfloat16_t,
		z0 = svneg_f16_z (p0, z1),
		z0 = svneg_z (p0, z1))

/*
** neg_f16_x_tied1:
**	fneg	z0\.h, p0/m, z0\.h
**	ret
*/
TEST_UNIFORM_Z (neg_f16_x_tied1, svfloat16_t,
		z0 = svneg_f16_x (p0, z0),
		z0 = svneg_x (p0, z0))

/*
** neg_f16_x_untied:
**	fneg	z2\.h, p0/m, z0\.h
**	ret
*/
TEST_UNIFORM_Z (neg_f16_x_untied, svfloat16_t,
		z2 = svneg_f16_x (p0, z0),
		z2 = svneg_x (p0, z0))

/*
** ptrue_neg_f16_x_tied1:
**	ptrue	(p[0-7])\.h[^\n]*
**	fneg	z0\.h, \1/m, z0\.h
**	ret
*/
TEST_UNIFORM_Z (ptrue_neg_f16_x_tied1, svfloat16_t,
		z0 = svneg_f16_x (svptrue_b16 (), z0),
		z0 = svneg_x (svptrue_b16 (), z0))

/*
** ptrue_neg_f16_x_untied:
**	ptrue	(p[0-7])\.h[^\n]*
**	fneg	z2\.h, \1/m, z0\.h
**	ret
*/
TEST_UNIFORM_Z (ptrue_neg_f16_x_untied, svfloat16_t,
		z2 = svneg_f16_x (svptrue_b16 (), z0),
		z2 = svneg_x (svptrue_b16 (), z0))

/*
** ptrue_b8_neg_f16_x_tied1:
**	ptrue	(p[0-7])\.h[^\n]*
**	fneg	z0\.h, \1/m, z0\.h
**	ret
*/
TEST_UNIFORM_Z (ptrue_b8_neg_f16_x_tied1, svfloat16_t,
		z0 = svneg_f16_x (svptrue_b8 (), z0),
		z0 = svneg_x (svptrue_b8 (), z0))

/*
** ptrue_b8_neg_f16_x_untied:
**	ptrue	(p[0-7])\.h[^\n]*
**	fneg	z2\.h, \1/m, z0\.h
**	ret
*/
TEST_UNIFORM_Z (ptrue_b8_neg_f16_x_untied, svfloat16_t,
		z2 = svneg_f16_x (svptrue_b8 (), z0),
		z2 = svneg_x (svptrue_b8 (), z0))

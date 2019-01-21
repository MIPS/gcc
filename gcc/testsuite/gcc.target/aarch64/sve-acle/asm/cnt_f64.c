/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** cnt_f64_m_tied1:
**	cnt	z0\.d, p0/m, z6\.d
**	ret
*/
TEST_DUAL_Z (cnt_f64_m_tied1, svuint64_t, svfloat64_t,
	     z0 = svcnt_f64_m (z0, p0, z6),
	     z0 = svcnt_m (z0, p0, z6))

/*
** cnt_f64_m_untied:
**	movprfx	z0, z1
**	cnt	z0\.d, p0/m, z6\.d
**	ret
*/
TEST_DUAL_Z (cnt_f64_m_untied, svuint64_t, svfloat64_t,
	     z0 = svcnt_f64_m (z1, p0, z6),
	     z0 = svcnt_m (z1, p0, z6))

/*
** cnt_f64_z:
**	movprfx	z0\.d, p0/z, z6\.d
**	cnt	z0\.d, p0/m, z6\.d
**	ret
*/
TEST_DUAL_Z (cnt_f64_z, svuint64_t, svfloat64_t,
	     z0 = svcnt_f64_z (p0, z6),
	     z0 = svcnt_z (p0, z6))

/*
** cnt_f64_x:
**	cnt	z0\.d, p0/m, z6\.d
**	ret
*/
TEST_DUAL_Z (cnt_f64_x, svuint64_t, svfloat64_t,
	     z0 = svcnt_f64_x (p0, z6),
	     z0 = svcnt_x (p0, z6))

/*
** ptrue_cnt_f64_x:
**	ptrue	(p[0-7])\.d[^\n]*
**	cnt	z0\.d, \1/m, z6\.d
**	ret
*/
TEST_DUAL_Z (ptrue_cnt_f64_x, svuint64_t, svfloat64_t,
	     z0 = svcnt_f64_x (svptrue_b64 (), z6),
	     z0 = svcnt_x (svptrue_b64 (), z6))

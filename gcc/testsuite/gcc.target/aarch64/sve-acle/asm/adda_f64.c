/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** adda_x0_x0_f64:
**	fmov	(d[0-9]+), x0
**	fadda	\1, p0, \1, z2\.d
**	fmov	x0, \1
**	ret
*/
TEST_FOLD_LEFT (adda_x0_x0_f64, float64_t, svfloat64_t,
		x0 = svadda_f64 (p0, x0, z2),
		x0 = svadda (p0, x0, z2))

/*
** adda_x0_x1_f64:
**	fmov	(d[0-9]+), x1
**	fadda	\1, p0, \1, z2\.d
**	fmov	x0, \1
**	ret
*/
TEST_FOLD_LEFT (adda_x0_x1_f64, float64_t, svfloat64_t,
		x0 = svadda_f64 (p0, x1, z2),
		x0 = svadda (p0, x1, z2))

/*
** adda_x0_d0_f64:
**	fmov	(d[0-9]+), d0
**	fadda	\1, p0, \1, z2\.d
**	fmov	x0, \1
**	ret
*/
TEST_FOLD_LEFT (adda_x0_d0_f64, float64_t, svfloat64_t,
		x0 = svadda_f64 (p0, d0, z2),
		x0 = svadda (p0, d0, z2))

/*
** adda_x0_d1_f64:
**	fmov	(d[0-9]+), d1
**	fadda	\1, p0, \1, z2\.d
**	fmov	x0, \1
**	ret
*/
TEST_FOLD_LEFT (adda_x0_d1_f64, float64_t, svfloat64_t,
		x0 = svadda_f64 (p0, d1, z2),
		x0 = svadda (p0, d1, z2))

/*
** adda_d0_x0_f64:
**	fmov	d0, x0
**	fadda	d0, p0, d0, z2\.d
**	ret
*/
TEST_FOLD_LEFT (adda_d0_x0_f64, float64_t, svfloat64_t,
		d0 = svadda_f64 (p0, x0, z2),
		d0 = svadda (p0, x0, z2))

/*
** adda_d0_x1_f64:
**	fmov	d0, x1
**	fadda	d0, p0, d0, z2\.d
**	ret
*/
TEST_FOLD_LEFT (adda_d0_x1_f64, float64_t, svfloat64_t,
		d0 = svadda_f64 (p0, x1, z2),
		d0 = svadda (p0, x1, z2))

/*
** adda_d0_d0_f64:
**	fadda	d0, p0, d0, z2\.d
**	ret
*/
TEST_FOLD_LEFT (adda_d0_d0_f64, float64_t, svfloat64_t,
		d0 = svadda_f64 (p0, d0, z2),
		d0 = svadda (p0, d0, z2))

/*
** adda_d0_d1_f64:
**	fmov	d0, d1
**	fadda	d0, p0, d0, z2\.d
**	ret
*/
TEST_FOLD_LEFT (adda_d0_d1_f64, float64_t, svfloat64_t,
		d0 = svadda_f64 (p0, d1, z2),
		d0 = svadda (p0, d1, z2))

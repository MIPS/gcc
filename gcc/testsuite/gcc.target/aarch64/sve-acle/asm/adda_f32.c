/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** adda_x0_x0_f32:
**	fmov	(s[0-9]+), w0
**	fadda	\1, p0, \1, z2\.s
**	fmov	w0, \1
**	ret
*/
TEST_FOLD_LEFT (adda_x0_x0_f32, float32_t, svfloat32_t,
		x0 = svadda_f32 (p0, x0, z2),
		x0 = svadda (p0, x0, z2))

/*
** adda_x0_x1_f32:
**	fmov	(s[0-9]+), w1
**	fadda	\1, p0, \1, z2\.s
**	fmov	w0, \1
**	ret
*/
TEST_FOLD_LEFT (adda_x0_x1_f32, float32_t, svfloat32_t,
		x0 = svadda_f32 (p0, x1, z2),
		x0 = svadda (p0, x1, z2))

/*
** adda_x0_d0_f32:
**	fmov	(s[0-9]+), s0
**	fadda	\1, p0, \1, z2\.s
**	fmov	w0, \1
**	ret
*/
TEST_FOLD_LEFT (adda_x0_d0_f32, float32_t, svfloat32_t,
		x0 = svadda_f32 (p0, d0, z2),
		x0 = svadda (p0, d0, z2))

/*
** adda_x0_d1_f32:
**	fmov	(s[0-9]+), s1
**	fadda	\1, p0, \1, z2\.s
**	fmov	w0, \1
**	ret
*/
TEST_FOLD_LEFT (adda_x0_d1_f32, float32_t, svfloat32_t,
		x0 = svadda_f32 (p0, d1, z2),
		x0 = svadda (p0, d1, z2))

/*
** adda_d0_x0_f32:
**	fmov	s0, w0
**	fadda	s0, p0, s0, z2\.s
**	ret
*/
TEST_FOLD_LEFT (adda_d0_x0_f32, float32_t, svfloat32_t,
		d0 = svadda_f32 (p0, x0, z2),
		d0 = svadda (p0, x0, z2))

/*
** adda_d0_x1_f32:
**	fmov	s0, w1
**	fadda	s0, p0, s0, z2\.s
**	ret
*/
TEST_FOLD_LEFT (adda_d0_x1_f32, float32_t, svfloat32_t,
		d0 = svadda_f32 (p0, x1, z2),
		d0 = svadda (p0, x1, z2))

/*
** adda_d0_d0_f32:
**	fadda	s0, p0, s0, z2\.s
**	ret
*/
TEST_FOLD_LEFT (adda_d0_d0_f32, float32_t, svfloat32_t,
		d0 = svadda_f32 (p0, d0, z2),
		d0 = svadda (p0, d0, z2))

/*
** adda_d0_d1_f32:
**	fmov	s0, s1
**	fadda	s0, p0, s0, z2\.s
**	ret
*/
TEST_FOLD_LEFT (adda_d0_d1_f32, float32_t, svfloat32_t,
		d0 = svadda_f32 (p0, d1, z2),
		d0 = svadda (p0, d1, z2))

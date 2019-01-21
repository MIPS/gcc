/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** adda_x0_x0_f16:
**	fmov	h([0-9]+), w0
**	fadda	h\1, p0, h\1, z2\.h
**	umov	w0, v\1\.h\[0\]
**	ret
*/
TEST_FOLD_LEFT (adda_x0_x0_f16, float16_t, svfloat16_t,
		x0 = svadda_f16 (p0, x0, z2),
		x0 = svadda (p0, x0, z2))

/*
** adda_x0_x1_f16:
**	fmov	h([0-9]+), w1
**	fadda	h\1, p0, h\1, z2\.h
**	umov	w0, v\1\.h\[0\]
**	ret
*/
TEST_FOLD_LEFT (adda_x0_x1_f16, float16_t, svfloat16_t,
		x0 = svadda_f16 (p0, x1, z2),
		x0 = svadda (p0, x1, z2))

/*
** adda_x0_d0_f16:
**	mov	v([0-9]+)\.h\[0\], v0\.h\[0\]
**	fadda	h\1, p0, h\1, z2\.h
**	umov	w0, v\1\.h\[0\]
**	ret
*/
TEST_FOLD_LEFT (adda_x0_d0_f16, float16_t, svfloat16_t,
		x0 = svadda_f16 (p0, d0, z2),
		x0 = svadda (p0, d0, z2))

/*
** adda_x0_d1_f16:
**	mov	v([0-9]+)\.h\[0\], v1\.h\[0\]
**	fadda	h\1, p0, h\1, z2\.h
**	umov	w0, v\1\.h\[0\]
**	ret
*/
TEST_FOLD_LEFT (adda_x0_d1_f16, float16_t, svfloat16_t,
		x0 = svadda_f16 (p0, d1, z2),
		x0 = svadda (p0, d1, z2))

/*
** adda_d0_x0_f16:
**	fmov	h0, w0
**	fadda	h0, p0, h0, z2\.h
**	ret
*/
TEST_FOLD_LEFT (adda_d0_x0_f16, float16_t, svfloat16_t,
		d0 = svadda_f16 (p0, x0, z2),
		d0 = svadda (p0, x0, z2))

/*
** adda_d0_x1_f16:
**	fmov	h0, w1
**	fadda	h0, p0, h0, z2\.h
**	ret
*/
TEST_FOLD_LEFT (adda_d0_x1_f16, float16_t, svfloat16_t,
		d0 = svadda_f16 (p0, x1, z2),
		d0 = svadda (p0, x1, z2))

/*
** adda_d0_d0_f16:
**	fadda	h0, p0, h0, z2\.h
**	ret
*/
TEST_FOLD_LEFT (adda_d0_d0_f16, float16_t, svfloat16_t,
		d0 = svadda_f16 (p0, d0, z2),
		d0 = svadda (p0, d0, z2))

/*
** adda_d0_d1_f16:
**	mov	v0\.h\[0\], v1\.h\[0\]
**	fadda	h0, p0, h0, z2\.h
**	ret
*/
TEST_FOLD_LEFT (adda_d0_d1_f16, float16_t, svfloat16_t,
		d0 = svadda_f16 (p0, d1, z2),
		d0 = svadda (p0, d1, z2))

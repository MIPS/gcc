/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** minv_x0_s16:
**	sminv	h([0-9]+), p0, z0\.h
**	umov	w0, v\1\.h\[0\]
**	ret
*/
TEST_REDUCTION_X (minv_x0_s16, int16_t, svint16_t,
		  x0 = svminv_s16 (p0, z0),
		  x0 = svminv (p0, z0))

/*
** minv_d0_s16_tied:
**	sminv	h0, p0, z0\.h
**	ret
*/
TEST_REDUCTION_D (minv_d0_s16_tied, int16_t, svint16_t,
		  d0 = svminv_s16 (p0, z0),
		  d0 = svminv (p0, z0))

/*
** minv_d0_s16_untied:
**	sminv	h0, p0, z1\.h
**	ret
*/
TEST_REDUCTION_D (minv_d0_s16_untied, int16_t, svint16_t,
		  d0 = svminv_s16 (p0, z1),
		  d0 = svminv (p0, z1))

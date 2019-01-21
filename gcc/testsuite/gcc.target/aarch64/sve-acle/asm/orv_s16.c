/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** orv_x0_s16:
**	orv	h([0-9]+), p0, z0\.h
**	umov	w0, v\1\.h\[0\]
**	ret
*/
TEST_REDUCTION_X (orv_x0_s16, int16_t, svint16_t,
		  x0 = svorv_s16 (p0, z0),
		  x0 = svorv (p0, z0))

/*
** orv_d0_s16_tied:
**	orv	h0, p0, z0\.h
**	ret
*/
TEST_REDUCTION_D (orv_d0_s16_tied, int16_t, svint16_t,
		  d0 = svorv_s16 (p0, z0),
		  d0 = svorv (p0, z0))

/*
** orv_d0_s16_untied:
**	orv	h0, p0, z1\.h
**	ret
*/
TEST_REDUCTION_D (orv_d0_s16_untied, int16_t, svint16_t,
		  d0 = svorv_s16 (p0, z1),
		  d0 = svorv (p0, z1))

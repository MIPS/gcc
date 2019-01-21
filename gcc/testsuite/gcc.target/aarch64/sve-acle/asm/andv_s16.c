/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** andv_x0_s16:
**	andv	h([0-9]+), p0, z0\.h
**	umov	w0, v\1\.h\[0\]
**	ret
*/
TEST_REDUCTION_X (andv_x0_s16, int16_t, svint16_t,
		  x0 = svandv_s16 (p0, z0),
		  x0 = svandv (p0, z0))

/*
** andv_d0_s16_tied:
**	andv	h0, p0, z0\.h
**	ret
*/
TEST_REDUCTION_D (andv_d0_s16_tied, int16_t, svint16_t,
		  d0 = svandv_s16 (p0, z0),
		  d0 = svandv (p0, z0))

/*
** andv_d0_s16_untied:
**	andv	h0, p0, z1\.h
**	ret
*/
TEST_REDUCTION_D (andv_d0_s16_untied, int16_t, svint16_t,
		  d0 = svandv_s16 (p0, z1),
		  d0 = svandv (p0, z1))

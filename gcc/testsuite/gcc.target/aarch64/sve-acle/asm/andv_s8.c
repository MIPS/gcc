/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** andv_x0_s8:
**	andv	b([0-9]+), p0, z0\.b
**	umov	w0, v\1\.b\[0\]
**	ret
*/
TEST_REDUCTION_X (andv_x0_s8, int8_t, svint8_t,
		  x0 = svandv_s8 (p0, z0),
		  x0 = svandv (p0, z0))

/*
** andv_d0_s8_tied:
**	andv	b0, p0, z0\.b
**	ret
*/
TEST_REDUCTION_D (andv_d0_s8_tied, int8_t, svint8_t,
		  d0 = svandv_s8 (p0, z0),
		  d0 = svandv (p0, z0))

/*
** andv_d0_s8_untied:
**	andv	b0, p0, z1\.b
**	ret
*/
TEST_REDUCTION_D (andv_d0_s8_untied, int8_t, svint8_t,
		  d0 = svandv_s8 (p0, z1),
		  d0 = svandv (p0, z1))

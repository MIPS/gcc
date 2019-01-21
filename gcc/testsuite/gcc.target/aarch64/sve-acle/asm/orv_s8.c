/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** orv_x0_s8:
**	orv	b([0-9]+), p0, z0\.b
**	umov	w0, v\1\.b\[0\]
**	ret
*/
TEST_REDUCTION_X (orv_x0_s8, int8_t, svint8_t,
		  x0 = svorv_s8 (p0, z0),
		  x0 = svorv (p0, z0))

/*
** orv_d0_s8_tied:
**	orv	b0, p0, z0\.b
**	ret
*/
TEST_REDUCTION_D (orv_d0_s8_tied, int8_t, svint8_t,
		  d0 = svorv_s8 (p0, z0),
		  d0 = svorv (p0, z0))

/*
** orv_d0_s8_untied:
**	orv	b0, p0, z1\.b
**	ret
*/
TEST_REDUCTION_D (orv_d0_s8_untied, int8_t, svint8_t,
		  d0 = svorv_s8 (p0, z1),
		  d0 = svorv (p0, z1))

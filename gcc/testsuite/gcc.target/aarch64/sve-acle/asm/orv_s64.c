/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** orv_x0_s64:
**	orv	(d[0-9]+), p0, z0\.d
**	fmov	x0, \1
**	ret
*/
TEST_REDUCTION_X (orv_x0_s64, int64_t, svint64_t,
		  x0 = svorv_s64 (p0, z0),
		  x0 = svorv (p0, z0))

/*
** orv_d0_s64_tied:
**	orv	d0, p0, z0\.d
**	ret
*/
TEST_REDUCTION_D (orv_d0_s64_tied, int64_t, svint64_t,
		  d0 = svorv_s64 (p0, z0),
		  d0 = svorv (p0, z0))

/*
** orv_d0_s64_untied:
**	orv	d0, p0, z1\.d
**	ret
*/
TEST_REDUCTION_D (orv_d0_s64_untied, int64_t, svint64_t,
		  d0 = svorv_s64 (p0, z1),
		  d0 = svorv (p0, z1))

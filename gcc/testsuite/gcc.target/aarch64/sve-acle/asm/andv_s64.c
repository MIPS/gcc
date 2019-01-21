/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** andv_x0_s64:
**	andv	(d[0-9]+), p0, z0\.d
**	fmov	x0, \1
**	ret
*/
TEST_REDUCTION_X (andv_x0_s64, int64_t, svint64_t,
		  x0 = svandv_s64 (p0, z0),
		  x0 = svandv (p0, z0))

/*
** andv_d0_s64_tied:
**	andv	d0, p0, z0\.d
**	ret
*/
TEST_REDUCTION_D (andv_d0_s64_tied, int64_t, svint64_t,
		  d0 = svandv_s64 (p0, z0),
		  d0 = svandv (p0, z0))

/*
** andv_d0_s64_untied:
**	andv	d0, p0, z1\.d
**	ret
*/
TEST_REDUCTION_D (andv_d0_s64_untied, int64_t, svint64_t,
		  d0 = svandv_s64 (p0, z1),
		  d0 = svandv (p0, z1))

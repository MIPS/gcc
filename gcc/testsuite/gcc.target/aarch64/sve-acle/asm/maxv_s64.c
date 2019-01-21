/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** maxv_x0_s64:
**	smaxv	(d[0-9]+), p0, z0\.d
**	fmov	x0, \1
**	ret
*/
TEST_REDUCTION_X (maxv_x0_s64, int64_t, svint64_t,
		  x0 = svmaxv_s64 (p0, z0),
		  x0 = svmaxv (p0, z0))

/*
** maxv_d0_s64_tied:
**	smaxv	d0, p0, z0\.d
**	ret
*/
TEST_REDUCTION_D (maxv_d0_s64_tied, int64_t, svint64_t,
		  d0 = svmaxv_s64 (p0, z0),
		  d0 = svmaxv (p0, z0))

/*
** maxv_d0_s64_untied:
**	smaxv	d0, p0, z1\.d
**	ret
*/
TEST_REDUCTION_D (maxv_d0_s64_untied, int64_t, svint64_t,
		  d0 = svmaxv_s64 (p0, z1),
		  d0 = svmaxv (p0, z1))

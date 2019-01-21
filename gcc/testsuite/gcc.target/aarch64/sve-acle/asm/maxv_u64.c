/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** maxv_x0_u64:
**	umaxv	(d[0-9]+), p0, z0\.d
**	fmov	x0, \1
**	ret
*/
TEST_REDUCTION_X (maxv_x0_u64, uint64_t, svuint64_t,
		  x0 = svmaxv_u64 (p0, z0),
		  x0 = svmaxv (p0, z0))

/*
** maxv_d0_u64_tied:
**	umaxv	d0, p0, z0\.d
**	ret
*/
TEST_REDUCTION_D (maxv_d0_u64_tied, uint64_t, svuint64_t,
		  d0 = svmaxv_u64 (p0, z0),
		  d0 = svmaxv (p0, z0))

/*
** maxv_d0_u64_untied:
**	umaxv	d0, p0, z1\.d
**	ret
*/
TEST_REDUCTION_D (maxv_d0_u64_untied, uint64_t, svuint64_t,
		  d0 = svmaxv_u64 (p0, z1),
		  d0 = svmaxv (p0, z1))

/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** minv_x0_u64:
**	uminv	(d[0-9]+), p0, z0\.d
**	fmov	x0, \1
**	ret
*/
TEST_REDUCTION_X (minv_x0_u64, uint64_t, svuint64_t,
		  x0 = svminv_u64 (p0, z0),
		  x0 = svminv (p0, z0))

/*
** minv_d0_u64_tied:
**	uminv	d0, p0, z0\.d
**	ret
*/
TEST_REDUCTION_D (minv_d0_u64_tied, uint64_t, svuint64_t,
		  d0 = svminv_u64 (p0, z0),
		  d0 = svminv (p0, z0))

/*
** minv_d0_u64_untied:
**	uminv	d0, p0, z1\.d
**	ret
*/
TEST_REDUCTION_D (minv_d0_u64_untied, uint64_t, svuint64_t,
		  d0 = svminv_u64 (p0, z1),
		  d0 = svminv (p0, z1))

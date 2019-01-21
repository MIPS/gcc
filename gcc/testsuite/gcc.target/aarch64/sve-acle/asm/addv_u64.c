/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** addv_x0_u64:
**	uaddv	(d[0-9]+), p0, z0\.d
**	fmov	x0, \1
**	ret
*/
TEST_REDUCTION_X (addv_x0_u64, uint64_t, svuint64_t,
		  x0 = svaddv_u64 (p0, z0),
		  x0 = svaddv (p0, z0))

/*
** addv_d0_u64_tied:
**	uaddv	d0, p0, z0\.d
**	ret
*/
TEST_REDUCTION_D (addv_d0_u64_tied, uint64_t, svuint64_t,
		  d0 = svaddv_u64 (p0, z0),
		  d0 = svaddv (p0, z0))

/*
** addv_d0_u64_untied:
**	uaddv	d0, p0, z1\.d
**	ret
*/
TEST_REDUCTION_D (addv_d0_u64_untied, uint64_t, svuint64_t,
		  d0 = svaddv_u64 (p0, z1),
		  d0 = svaddv (p0, z1))

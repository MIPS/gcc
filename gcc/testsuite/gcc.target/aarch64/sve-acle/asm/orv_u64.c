/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** orv_x0_u64:
**	orv	(d[0-9]+), p0, z0\.d
**	fmov	x0, \1
**	ret
*/
TEST_REDUCTION_X (orv_x0_u64, uint64_t, svuint64_t,
		  x0 = svorv_u64 (p0, z0),
		  x0 = svorv (p0, z0))

/*
** orv_d0_u64_tied:
**	orv	d0, p0, z0\.d
**	ret
*/
TEST_REDUCTION_D (orv_d0_u64_tied, uint64_t, svuint64_t,
		  d0 = svorv_u64 (p0, z0),
		  d0 = svorv (p0, z0))

/*
** orv_d0_u64_untied:
**	orv	d0, p0, z1\.d
**	ret
*/
TEST_REDUCTION_D (orv_d0_u64_untied, uint64_t, svuint64_t,
		  d0 = svorv_u64 (p0, z1),
		  d0 = svorv (p0, z1))

/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** eorv_x0_u64:
**	eorv	(d[0-9]+), p0, z0\.d
**	fmov	x0, \1
**	ret
*/
TEST_REDUCTION_X (eorv_x0_u64, uint64_t, svuint64_t,
		  x0 = sveorv_u64 (p0, z0),
		  x0 = sveorv (p0, z0))

/*
** eorv_d0_u64_tied:
**	eorv	d0, p0, z0\.d
**	ret
*/
TEST_REDUCTION_D (eorv_d0_u64_tied, uint64_t, svuint64_t,
		  d0 = sveorv_u64 (p0, z0),
		  d0 = sveorv (p0, z0))

/*
** eorv_d0_u64_untied:
**	eorv	d0, p0, z1\.d
**	ret
*/
TEST_REDUCTION_D (eorv_d0_u64_untied, uint64_t, svuint64_t,
		  d0 = sveorv_u64 (p0, z1),
		  d0 = sveorv (p0, z1))

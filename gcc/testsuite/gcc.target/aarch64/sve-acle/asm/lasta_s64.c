/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** lasta_x0_s64:
**	lasta	x0, p0, z0\.d
**	ret
*/
TEST_REDUCTION_X (lasta_x0_s64, int64_t, svint64_t,
		  x0 = svlasta_s64 (p0, z0),
		  x0 = svlasta (p0, z0))

/*
** lasta_d0_s64_tied:
**	lasta	d0, p0, z0\.d
**	ret
*/
TEST_REDUCTION_D (lasta_d0_s64_tied, int64_t, svint64_t,
		  d0 = svlasta_s64 (p0, z0),
		  d0 = svlasta (p0, z0))

/*
** lasta_d0_s64_untied:
**	lasta	d0, p0, z1\.d
**	ret
*/
TEST_REDUCTION_D (lasta_d0_s64_untied, int64_t, svint64_t,
		  d0 = svlasta_s64 (p0, z1),
		  d0 = svlasta (p0, z1))

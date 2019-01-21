/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** addv_x0_s8:
**	saddv	(d[0-9]+), p0, z0\.b
**	fmov	x0, \1
**	ret
*/
TEST_REDUCTION_X (addv_x0_s8, int64_t, svint8_t,
		  x0 = svaddv_s8 (p0, z0),
		  x0 = svaddv (p0, z0))

/*
** addv_d0_s8_tied:
**	saddv	d0, p0, z0\.b
**	ret
*/
TEST_REDUCTION_D (addv_d0_s8_tied, int64_t, svint8_t,
		  d0 = svaddv_s8 (p0, z0),
		  d0 = svaddv (p0, z0))

/*
** addv_d0_s8_untied:
**	saddv	d0, p0, z1\.b
**	ret
*/
TEST_REDUCTION_D (addv_d0_s8_untied, int64_t, svint8_t,
		  d0 = svaddv_s8 (p0, z1),
		  d0 = svaddv (p0, z1))

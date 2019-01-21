/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** addv_x0_s16:
**	saddv	(d[0-9]+), p0, z0\.h
**	fmov	x0, \1
**	ret
*/
TEST_REDUCTION_X (addv_x0_s16, int64_t, svint16_t,
		  x0 = svaddv_s16 (p0, z0),
		  x0 = svaddv (p0, z0))

/*
** addv_d0_s16_tied:
**	saddv	d0, p0, z0\.h
**	ret
*/
TEST_REDUCTION_D (addv_d0_s16_tied, int64_t, svint16_t,
		  d0 = svaddv_s16 (p0, z0),
		  d0 = svaddv (p0, z0))

/*
** addv_d0_s16_untied:
**	saddv	d0, p0, z1\.h
**	ret
*/
TEST_REDUCTION_D (addv_d0_s16_untied, int64_t, svint16_t,
		  d0 = svaddv_s16 (p0, z1),
		  d0 = svaddv (p0, z1))

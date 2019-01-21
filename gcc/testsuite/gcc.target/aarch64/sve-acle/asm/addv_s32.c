/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** addv_x0_s32:
**	saddv	(d[0-9]+), p0, z0\.s
**	fmov	x0, \1
**	ret
*/
TEST_REDUCTION_X (addv_x0_s32, int64_t, svint32_t,
		  x0 = svaddv_s32 (p0, z0),
		  x0 = svaddv (p0, z0))

/*
** addv_d0_s32_tied:
**	saddv	d0, p0, z0\.s
**	ret
*/
TEST_REDUCTION_D (addv_d0_s32_tied, int64_t, svint32_t,
		  d0 = svaddv_s32 (p0, z0),
		  d0 = svaddv (p0, z0))

/*
** addv_d0_s32_untied:
**	saddv	d0, p0, z1\.s
**	ret
*/
TEST_REDUCTION_D (addv_d0_s32_untied, int64_t, svint32_t,
		  d0 = svaddv_s32 (p0, z1),
		  d0 = svaddv (p0, z1))

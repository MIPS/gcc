/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** addv_x0_u32:
**	uaddv	(d[0-9]+), p0, z0\.s
**	fmov	x0, \1
**	ret
*/
TEST_REDUCTION_X (addv_x0_u32, uint64_t, svuint32_t,
		  x0 = svaddv_u32 (p0, z0),
		  x0 = svaddv (p0, z0))

/*
** addv_d0_u32_tied:
**	uaddv	d0, p0, z0\.s
**	ret
*/
TEST_REDUCTION_D (addv_d0_u32_tied, uint64_t, svuint32_t,
		  d0 = svaddv_u32 (p0, z0),
		  d0 = svaddv (p0, z0))

/*
** addv_d0_u32_untied:
**	uaddv	d0, p0, z1\.s
**	ret
*/
TEST_REDUCTION_D (addv_d0_u32_untied, uint64_t, svuint32_t,
		  d0 = svaddv_u32 (p0, z1),
		  d0 = svaddv (p0, z1))

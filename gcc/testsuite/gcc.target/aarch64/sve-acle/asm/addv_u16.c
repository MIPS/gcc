/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** addv_x0_u16:
**	uaddv	(d[0-9]+), p0, z0\.h
**	fmov	x0, \1
**	ret
*/
TEST_REDUCTION_X (addv_x0_u16, uint64_t, svuint16_t,
		  x0 = svaddv_u16 (p0, z0),
		  x0 = svaddv (p0, z0))

/*
** addv_d0_u16_tied:
**	uaddv	d0, p0, z0\.h
**	ret
*/
TEST_REDUCTION_D (addv_d0_u16_tied, uint64_t, svuint16_t,
		  d0 = svaddv_u16 (p0, z0),
		  d0 = svaddv (p0, z0))

/*
** addv_d0_u16_untied:
**	uaddv	d0, p0, z1\.h
**	ret
*/
TEST_REDUCTION_D (addv_d0_u16_untied, uint64_t, svuint16_t,
		  d0 = svaddv_u16 (p0, z1),
		  d0 = svaddv (p0, z1))

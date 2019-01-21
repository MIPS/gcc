/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** andv_x0_u32:
**	andv	(s[0-9]+), p0, z0\.s
**	fmov	w0, \1
**	ret
*/
TEST_REDUCTION_X (andv_x0_u32, uint32_t, svuint32_t,
		  x0 = svandv_u32 (p0, z0),
		  x0 = svandv (p0, z0))

/*
** andv_d0_u32_tied:
**	andv	s0, p0, z0\.s
**	ret
*/
TEST_REDUCTION_D (andv_d0_u32_tied, uint32_t, svuint32_t,
		  d0 = svandv_u32 (p0, z0),
		  d0 = svandv (p0, z0))

/*
** andv_d0_u32_untied:
**	andv	s0, p0, z1\.s
**	ret
*/
TEST_REDUCTION_D (andv_d0_u32_untied, uint32_t, svuint32_t,
		  d0 = svandv_u32 (p0, z1),
		  d0 = svandv (p0, z1))

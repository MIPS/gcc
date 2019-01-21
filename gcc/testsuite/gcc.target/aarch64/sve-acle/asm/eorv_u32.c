/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** eorv_x0_u32:
**	eorv	(s[0-9]+), p0, z0\.s
**	fmov	w0, \1
**	ret
*/
TEST_REDUCTION_X (eorv_x0_u32, uint32_t, svuint32_t,
		  x0 = sveorv_u32 (p0, z0),
		  x0 = sveorv (p0, z0))

/*
** eorv_d0_u32_tied:
**	eorv	s0, p0, z0\.s
**	ret
*/
TEST_REDUCTION_D (eorv_d0_u32_tied, uint32_t, svuint32_t,
		  d0 = sveorv_u32 (p0, z0),
		  d0 = sveorv (p0, z0))

/*
** eorv_d0_u32_untied:
**	eorv	s0, p0, z1\.s
**	ret
*/
TEST_REDUCTION_D (eorv_d0_u32_untied, uint32_t, svuint32_t,
		  d0 = sveorv_u32 (p0, z1),
		  d0 = sveorv (p0, z1))

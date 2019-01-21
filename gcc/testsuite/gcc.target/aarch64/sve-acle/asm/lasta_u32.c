/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** lasta_x0_u32:
**	lasta	w0, p0, z0\.s
**	ret
*/
TEST_REDUCTION_X (lasta_x0_u32, uint32_t, svuint32_t,
		  x0 = svlasta_u32 (p0, z0),
		  x0 = svlasta (p0, z0))

/*
** lasta_d0_u32_tied:
**	lasta	s0, p0, z0\.s
**	ret
*/
TEST_REDUCTION_D (lasta_d0_u32_tied, uint32_t, svuint32_t,
		  d0 = svlasta_u32 (p0, z0),
		  d0 = svlasta (p0, z0))

/*
** lasta_d0_u32_untied:
**	lasta	s0, p0, z1\.s
**	ret
*/
TEST_REDUCTION_D (lasta_d0_u32_untied, uint32_t, svuint32_t,
		  d0 = svlasta_u32 (p0, z1),
		  d0 = svlasta (p0, z1))

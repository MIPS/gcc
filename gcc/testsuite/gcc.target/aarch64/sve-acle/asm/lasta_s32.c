/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** lasta_x0_s32:
**	lasta	w0, p0, z0\.s
**	ret
*/
TEST_REDUCTION_X (lasta_x0_s32, int32_t, svint32_t,
		  x0 = svlasta_s32 (p0, z0),
		  x0 = svlasta (p0, z0))

/*
** lasta_d0_s32_tied:
**	lasta	s0, p0, z0\.s
**	ret
*/
TEST_REDUCTION_D (lasta_d0_s32_tied, int32_t, svint32_t,
		  d0 = svlasta_s32 (p0, z0),
		  d0 = svlasta (p0, z0))

/*
** lasta_d0_s32_untied:
**	lasta	s0, p0, z1\.s
**	ret
*/
TEST_REDUCTION_D (lasta_d0_s32_untied, int32_t, svint32_t,
		  d0 = svlasta_s32 (p0, z1),
		  d0 = svlasta (p0, z1))

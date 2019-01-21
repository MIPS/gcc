/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** lasta_x0_s8:
**	lasta	w0, p0, z0\.b
**	ret
*/
TEST_REDUCTION_X (lasta_x0_s8, int8_t, svint8_t,
		  x0 = svlasta_s8 (p0, z0),
		  x0 = svlasta (p0, z0))

/*
** lasta_d0_s8_tied:
**	lasta	b0, p0, z0\.b
**	ret
*/
TEST_REDUCTION_D (lasta_d0_s8_tied, int8_t, svint8_t,
		  d0 = svlasta_s8 (p0, z0),
		  d0 = svlasta (p0, z0))

/*
** lasta_d0_s8_untied:
**	lasta	b0, p0, z1\.b
**	ret
*/
TEST_REDUCTION_D (lasta_d0_s8_untied, int8_t, svint8_t,
		  d0 = svlasta_s8 (p0, z1),
		  d0 = svlasta (p0, z1))

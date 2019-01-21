/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** lastb_x0_s32:
**	lastb	w0, p0, z0\.s
**	ret
*/
TEST_REDUCTION_X (lastb_x0_s32, int32_t, svint32_t,
		  x0 = svlastb_s32 (p0, z0),
		  x0 = svlastb (p0, z0))

/*
** lastb_d0_s32_tied:
**	lastb	s0, p0, z0\.s
**	ret
*/
TEST_REDUCTION_D (lastb_d0_s32_tied, int32_t, svint32_t,
		  d0 = svlastb_s32 (p0, z0),
		  d0 = svlastb (p0, z0))

/*
** lastb_d0_s32_untied:
**	lastb	s0, p0, z1\.s
**	ret
*/
TEST_REDUCTION_D (lastb_d0_s32_untied, int32_t, svint32_t,
		  d0 = svlastb_s32 (p0, z1),
		  d0 = svlastb (p0, z1))

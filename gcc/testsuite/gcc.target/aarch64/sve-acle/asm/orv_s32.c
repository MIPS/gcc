/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** orv_x0_s32:
**	orv	(s[0-9]+), p0, z0\.s
**	fmov	w0, \1
**	ret
*/
TEST_REDUCTION_X (orv_x0_s32, int32_t, svint32_t,
		  x0 = svorv_s32 (p0, z0),
		  x0 = svorv (p0, z0))

/*
** orv_d0_s32_tied:
**	orv	s0, p0, z0\.s
**	ret
*/
TEST_REDUCTION_D (orv_d0_s32_tied, int32_t, svint32_t,
		  d0 = svorv_s32 (p0, z0),
		  d0 = svorv (p0, z0))

/*
** orv_d0_s32_untied:
**	orv	s0, p0, z1\.s
**	ret
*/
TEST_REDUCTION_D (orv_d0_s32_untied, int32_t, svint32_t,
		  d0 = svorv_s32 (p0, z1),
		  d0 = svorv (p0, z1))

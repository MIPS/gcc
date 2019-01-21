/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** minv_x0_s32:
**	sminv	(s[0-9]+), p0, z0\.s
**	fmov	w0, \1
**	ret
*/
TEST_REDUCTION_X (minv_x0_s32, int32_t, svint32_t,
		  x0 = svminv_s32 (p0, z0),
		  x0 = svminv (p0, z0))

/*
** minv_d0_s32_tied:
**	sminv	s0, p0, z0\.s
**	ret
*/
TEST_REDUCTION_D (minv_d0_s32_tied, int32_t, svint32_t,
		  d0 = svminv_s32 (p0, z0),
		  d0 = svminv (p0, z0))

/*
** minv_d0_s32_untied:
**	sminv	s0, p0, z1\.s
**	ret
*/
TEST_REDUCTION_D (minv_d0_s32_untied, int32_t, svint32_t,
		  d0 = svminv_s32 (p0, z1),
		  d0 = svminv (p0, z1))

/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** maxv_x0_s32:
**	smaxv	(s[0-9]+), p0, z0\.s
**	fmov	w0, \1
**	ret
*/
TEST_REDUCTION_X (maxv_x0_s32, int32_t, svint32_t,
		  x0 = svmaxv_s32 (p0, z0),
		  x0 = svmaxv (p0, z0))

/*
** maxv_d0_s32_tied:
**	smaxv	s0, p0, z0\.s
**	ret
*/
TEST_REDUCTION_D (maxv_d0_s32_tied, int32_t, svint32_t,
		  d0 = svmaxv_s32 (p0, z0),
		  d0 = svmaxv (p0, z0))

/*
** maxv_d0_s32_untied:
**	smaxv	s0, p0, z1\.s
**	ret
*/
TEST_REDUCTION_D (maxv_d0_s32_untied, int32_t, svint32_t,
		  d0 = svmaxv_s32 (p0, z1),
		  d0 = svmaxv (p0, z1))

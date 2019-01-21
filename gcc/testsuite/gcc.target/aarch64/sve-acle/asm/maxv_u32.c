/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** maxv_x0_u32:
**	umaxv	(s[0-9]+), p0, z0\.s
**	fmov	w0, \1
**	ret
*/
TEST_REDUCTION_X (maxv_x0_u32, uint32_t, svuint32_t,
		  x0 = svmaxv_u32 (p0, z0),
		  x0 = svmaxv (p0, z0))

/*
** maxv_d0_u32_tied:
**	umaxv	s0, p0, z0\.s
**	ret
*/
TEST_REDUCTION_D (maxv_d0_u32_tied, uint32_t, svuint32_t,
		  d0 = svmaxv_u32 (p0, z0),
		  d0 = svmaxv (p0, z0))

/*
** maxv_d0_u32_untied:
**	umaxv	s0, p0, z1\.s
**	ret
*/
TEST_REDUCTION_D (maxv_d0_u32_untied, uint32_t, svuint32_t,
		  d0 = svmaxv_u32 (p0, z1),
		  d0 = svmaxv (p0, z1))

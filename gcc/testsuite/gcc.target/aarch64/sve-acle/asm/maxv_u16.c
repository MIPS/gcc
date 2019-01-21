/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** maxv_x0_u16:
**	umaxv	h([0-9]+), p0, z0\.h
**	umov	w0, v\1\.h\[0\]
**	ret
*/
TEST_REDUCTION_X (maxv_x0_u16, uint16_t, svuint16_t,
		  x0 = svmaxv_u16 (p0, z0),
		  x0 = svmaxv (p0, z0))

/*
** maxv_d0_u16_tied:
**	umaxv	h0, p0, z0\.h
**	ret
*/
TEST_REDUCTION_D (maxv_d0_u16_tied, uint16_t, svuint16_t,
		  d0 = svmaxv_u16 (p0, z0),
		  d0 = svmaxv (p0, z0))

/*
** maxv_d0_u16_untied:
**	umaxv	h0, p0, z1\.h
**	ret
*/
TEST_REDUCTION_D (maxv_d0_u16_untied, uint16_t, svuint16_t,
		  d0 = svmaxv_u16 (p0, z1),
		  d0 = svmaxv (p0, z1))

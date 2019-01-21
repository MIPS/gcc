/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** andv_x0_u16:
**	andv	h([0-9]+), p0, z0\.h
**	umov	w0, v\1\.h\[0\]
**	ret
*/
TEST_REDUCTION_X (andv_x0_u16, uint16_t, svuint16_t,
		  x0 = svandv_u16 (p0, z0),
		  x0 = svandv (p0, z0))

/*
** andv_d0_u16_tied:
**	andv	h0, p0, z0\.h
**	ret
*/
TEST_REDUCTION_D (andv_d0_u16_tied, uint16_t, svuint16_t,
		  d0 = svandv_u16 (p0, z0),
		  d0 = svandv (p0, z0))

/*
** andv_d0_u16_untied:
**	andv	h0, p0, z1\.h
**	ret
*/
TEST_REDUCTION_D (andv_d0_u16_untied, uint16_t, svuint16_t,
		  d0 = svandv_u16 (p0, z1),
		  d0 = svandv (p0, z1))

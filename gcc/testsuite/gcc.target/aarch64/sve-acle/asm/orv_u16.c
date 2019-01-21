/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** orv_x0_u16:
**	orv	h([0-9]+), p0, z0\.h
**	umov	w0, v\1\.h\[0\]
**	ret
*/
TEST_REDUCTION_X (orv_x0_u16, uint16_t, svuint16_t,
		  x0 = svorv_u16 (p0, z0),
		  x0 = svorv (p0, z0))

/*
** orv_d0_u16_tied:
**	orv	h0, p0, z0\.h
**	ret
*/
TEST_REDUCTION_D (orv_d0_u16_tied, uint16_t, svuint16_t,
		  d0 = svorv_u16 (p0, z0),
		  d0 = svorv (p0, z0))

/*
** orv_d0_u16_untied:
**	orv	h0, p0, z1\.h
**	ret
*/
TEST_REDUCTION_D (orv_d0_u16_untied, uint16_t, svuint16_t,
		  d0 = svorv_u16 (p0, z1),
		  d0 = svorv (p0, z1))

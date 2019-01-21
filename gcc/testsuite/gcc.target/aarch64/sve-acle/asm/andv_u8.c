/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** andv_x0_u8:
**	andv	b([0-9]+), p0, z0\.b
**	umov	w0, v\1\.b\[0\]
**	ret
*/
TEST_REDUCTION_X (andv_x0_u8, uint8_t, svuint8_t,
		  x0 = svandv_u8 (p0, z0),
		  x0 = svandv (p0, z0))

/*
** andv_d0_u8_tied:
**	andv	b0, p0, z0\.b
**	ret
*/
TEST_REDUCTION_D (andv_d0_u8_tied, uint8_t, svuint8_t,
		  d0 = svandv_u8 (p0, z0),
		  d0 = svandv (p0, z0))

/*
** andv_d0_u8_untied:
**	andv	b0, p0, z1\.b
**	ret
*/
TEST_REDUCTION_D (andv_d0_u8_untied, uint8_t, svuint8_t,
		  d0 = svandv_u8 (p0, z1),
		  d0 = svandv (p0, z1))

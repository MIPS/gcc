/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** orv_x0_u8:
**	orv	b([0-9]+), p0, z0\.b
**	umov	w0, v\1\.b\[0\]
**	ret
*/
TEST_REDUCTION_X (orv_x0_u8, uint8_t, svuint8_t,
		  x0 = svorv_u8 (p0, z0),
		  x0 = svorv (p0, z0))

/*
** orv_d0_u8_tied:
**	orv	b0, p0, z0\.b
**	ret
*/
TEST_REDUCTION_D (orv_d0_u8_tied, uint8_t, svuint8_t,
		  d0 = svorv_u8 (p0, z0),
		  d0 = svorv (p0, z0))

/*
** orv_d0_u8_untied:
**	orv	b0, p0, z1\.b
**	ret
*/
TEST_REDUCTION_D (orv_d0_u8_untied, uint8_t, svuint8_t,
		  d0 = svorv_u8 (p0, z1),
		  d0 = svorv (p0, z1))

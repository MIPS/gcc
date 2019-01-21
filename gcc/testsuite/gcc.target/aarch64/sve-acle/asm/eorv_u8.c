/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** eorv_x0_u8:
**	eorv	b([0-9]+), p0, z0\.b
**	umov	w0, v\1\.b\[0\]
**	ret
*/
TEST_REDUCTION_X (eorv_x0_u8, uint8_t, svuint8_t,
		  x0 = sveorv_u8 (p0, z0),
		  x0 = sveorv (p0, z0))

/*
** eorv_d0_u8_tied:
**	eorv	b0, p0, z0\.b
**	ret
*/
TEST_REDUCTION_D (eorv_d0_u8_tied, uint8_t, svuint8_t,
		  d0 = sveorv_u8 (p0, z0),
		  d0 = sveorv (p0, z0))

/*
** eorv_d0_u8_untied:
**	eorv	b0, p0, z1\.b
**	ret
*/
TEST_REDUCTION_D (eorv_d0_u8_untied, uint8_t, svuint8_t,
		  d0 = sveorv_u8 (p0, z1),
		  d0 = sveorv (p0, z1))

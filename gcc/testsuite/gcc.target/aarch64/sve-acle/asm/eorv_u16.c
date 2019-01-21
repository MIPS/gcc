/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** eorv_x0_u16:
**	eorv	h([0-9]+), p0, z0\.h
**	umov	w0, v\1\.h\[0\]
**	ret
*/
TEST_REDUCTION_X (eorv_x0_u16, uint16_t, svuint16_t,
		  x0 = sveorv_u16 (p0, z0),
		  x0 = sveorv (p0, z0))

/*
** eorv_d0_u16_tied:
**	eorv	h0, p0, z0\.h
**	ret
*/
TEST_REDUCTION_D (eorv_d0_u16_tied, uint16_t, svuint16_t,
		  d0 = sveorv_u16 (p0, z0),
		  d0 = sveorv (p0, z0))

/*
** eorv_d0_u16_untied:
**	eorv	h0, p0, z1\.h
**	ret
*/
TEST_REDUCTION_D (eorv_d0_u16_untied, uint16_t, svuint16_t,
		  d0 = sveorv_u16 (p0, z1),
		  d0 = sveorv (p0, z1))

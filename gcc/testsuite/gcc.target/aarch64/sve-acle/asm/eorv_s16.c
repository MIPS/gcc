/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** eorv_x0_s16:
**	eorv	h([0-9]+), p0, z0\.h
**	umov	w0, v\1\.h\[0\]
**	ret
*/
TEST_REDUCTION_X (eorv_x0_s16, int16_t, svint16_t,
		  x0 = sveorv_s16 (p0, z0),
		  x0 = sveorv (p0, z0))

/*
** eorv_d0_s16_tied:
**	eorv	h0, p0, z0\.h
**	ret
*/
TEST_REDUCTION_D (eorv_d0_s16_tied, int16_t, svint16_t,
		  d0 = sveorv_s16 (p0, z0),
		  d0 = sveorv (p0, z0))

/*
** eorv_d0_s16_untied:
**	eorv	h0, p0, z1\.h
**	ret
*/
TEST_REDUCTION_D (eorv_d0_s16_untied, int16_t, svint16_t,
		  d0 = sveorv_s16 (p0, z1),
		  d0 = sveorv (p0, z1))

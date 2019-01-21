/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** lastb_x0_u16:
**	lastb	w0, p0, z0\.h
**	ret
*/
TEST_REDUCTION_X (lastb_x0_u16, uint16_t, svuint16_t,
		  x0 = svlastb_u16 (p0, z0),
		  x0 = svlastb (p0, z0))

/*
** lastb_d0_u16_tied:
**	lastb	h0, p0, z0\.h
**	ret
*/
TEST_REDUCTION_D (lastb_d0_u16_tied, uint16_t, svuint16_t,
		  d0 = svlastb_u16 (p0, z0),
		  d0 = svlastb (p0, z0))

/*
** lastb_d0_u16_untied:
**	lastb	h0, p0, z1\.h
**	ret
*/
TEST_REDUCTION_D (lastb_d0_u16_untied, uint16_t, svuint16_t,
		  d0 = svlastb_u16 (p0, z1),
		  d0 = svlastb (p0, z1))

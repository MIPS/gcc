/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** lsr_wide_u16_m_tied1:
**	lsr	z0\.h, p0/m, z0\.h, z6\.d
**	ret
*/
TEST_WIDE_Z (lsr_wide_u16_m_tied1, svuint16_t,
	     z0 = svlsr_wide_u16_m (p0, z0, z6),
	     z0 = svlsr_wide_m (p0, z0, z6))

/*
** lsr_wide_u16_m_tied2:
**	mov	(z[0-9]+\.d), z6\.d
**	movprfx	z6, z0
**	lsr	z6\.h, p0/m, z6\.h, \1
**	ret
*/
TEST_WIDE_Z (lsr_wide_u16_m_tied2, svuint16_t,
	     z6_res = svlsr_wide_u16_m (p0, z0, z6),
	     z6_res = svlsr_wide_m (p0, z0, z6))

/*
** lsr_wide_u16_m_untied:
**	movprfx	z0, z1
**	lsr	z0\.h, p0/m, z0\.h, z6\.d
**	ret
*/
TEST_WIDE_Z (lsr_wide_u16_m_untied, svuint16_t,
	     z0 = svlsr_wide_u16_m (p0, z1, z6),
	     z0 = svlsr_wide_m (p0, z1, z6))

/*
** lsr_wide_u16_z_tied1:
**	movprfx	z0\.h, p0/z, z0\.h
**	lsr	z0\.h, p0/m, z0\.h, z6\.d
**	ret
*/
TEST_WIDE_Z (lsr_wide_u16_z_tied1, svuint16_t,
	     z0 = svlsr_wide_u16_z (p0, z0, z6),
	     z0 = svlsr_wide_z (p0, z0, z6))

/*
** lsr_wide_u16_z_tied2:
**	mov	(z[0-9]+\.d), z6\.d
**	movprfx	z6\.h, p0/z, z0\.h
**	lsr	z6\.h, p0/m, z6\.h, \1
**	ret
*/
TEST_WIDE_Z (lsr_wide_u16_z_tied2, svuint16_t,
	     z6_res = svlsr_wide_u16_z (p0, z0, z6),
	     z6_res = svlsr_wide_z (p0, z0, z6))

/*
** lsr_wide_u16_z_untied:
**	movprfx	z0\.h, p0/z, z1\.h
**	lsr	z0\.h, p0/m, z0\.h, z6\.d
**	ret
*/
TEST_WIDE_Z (lsr_wide_u16_z_untied, svuint16_t,
	     z0 = svlsr_wide_u16_z (p0, z1, z6),
	     z0 = svlsr_wide_z (p0, z1, z6))

/*
** lsr_wide_u16_x_tied1:
**	lsr	z0\.h, z0\.h, z6\.d
**	ret
*/
TEST_WIDE_Z (lsr_wide_u16_x_tied1, svuint16_t,
	     z0 = svlsr_wide_u16_x (p0, z0, z6),
	     z0 = svlsr_wide_x (p0, z0, z6))

/*
** lsr_wide_u16_x_tied2:
**	lsr	z6\.h, z0\.h, z6\.d
**	ret
*/
TEST_WIDE_Z (lsr_wide_u16_x_tied2, svuint16_t,
	     z6_res = svlsr_wide_u16_x (p0, z0, z6),
	     z6_res = svlsr_wide_x (p0, z0, z6))

/*
** lsr_wide_u16_x_untied:
**	lsr	z0\.h, z1\.h, z6\.d
**	ret
*/
TEST_WIDE_Z (lsr_wide_u16_x_untied, svuint16_t,
	     z0 = svlsr_wide_u16_x (p0, z1, z6),
	     z0 = svlsr_wide_x (p0, z1, z6))

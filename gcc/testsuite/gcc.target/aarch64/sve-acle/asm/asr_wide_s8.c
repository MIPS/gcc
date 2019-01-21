/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** asr_wide_s8_m_tied1:
**	asr	z0\.b, p0/m, z0\.b, z6\.d
**	ret
*/
TEST_WIDE_Z (asr_wide_s8_m_tied1, svint8_t,
	     z0 = svasr_wide_s8_m (p0, z0, z6),
	     z0 = svasr_wide_m (p0, z0, z6))

/*
** asr_wide_s8_m_tied2:
**	mov	(z[0-9]+\.d), z6\.d
**	movprfx	z6, z0
**	asr	z6\.b, p0/m, z6\.b, \1
**	ret
*/
TEST_WIDE_Z (asr_wide_s8_m_tied2, svint8_t,
	     z6_res = svasr_wide_s8_m (p0, z0, z6),
	     z6_res = svasr_wide_m (p0, z0, z6))

/*
** asr_wide_s8_m_untied:
**	movprfx	z0, z1
**	asr	z0\.b, p0/m, z0\.b, z6\.d
**	ret
*/
TEST_WIDE_Z (asr_wide_s8_m_untied, svint8_t,
	     z0 = svasr_wide_s8_m (p0, z1, z6),
	     z0 = svasr_wide_m (p0, z1, z6))

/*
** asr_wide_s8_z_tied1:
**	movprfx	z0\.b, p0/z, z0\.b
**	asr	z0\.b, p0/m, z0\.b, z6\.d
**	ret
*/
TEST_WIDE_Z (asr_wide_s8_z_tied1, svint8_t,
	     z0 = svasr_wide_s8_z (p0, z0, z6),
	     z0 = svasr_wide_z (p0, z0, z6))

/*
** asr_wide_s8_z_tied2:
**	mov	(z[0-9]+\.d), z6\.d
**	movprfx	z6\.b, p0/z, z0\.b
**	asr	z6\.b, p0/m, z6\.b, \1
**	ret
*/
TEST_WIDE_Z (asr_wide_s8_z_tied2, svint8_t,
	     z6_res = svasr_wide_s8_z (p0, z0, z6),
	     z6_res = svasr_wide_z (p0, z0, z6))

/*
** asr_wide_s8_z_untied:
**	movprfx	z0\.b, p0/z, z1\.b
**	asr	z0\.b, p0/m, z0\.b, z6\.d
**	ret
*/
TEST_WIDE_Z (asr_wide_s8_z_untied, svint8_t,
	     z0 = svasr_wide_s8_z (p0, z1, z6),
	     z0 = svasr_wide_z (p0, z1, z6))

/*
** asr_wide_s8_x_tied1:
**	asr	z0\.b, z0\.b, z6\.d
**	ret
*/
TEST_WIDE_Z (asr_wide_s8_x_tied1, svint8_t,
	     z0 = svasr_wide_s8_x (p0, z0, z6),
	     z0 = svasr_wide_x (p0, z0, z6))

/*
** asr_wide_s8_x_tied2:
**	asr	z6\.b, z0\.b, z6\.d
**	ret
*/
TEST_WIDE_Z (asr_wide_s8_x_tied2, svint8_t,
	     z6_res = svasr_wide_s8_x (p0, z0, z6),
	     z6_res = svasr_wide_x (p0, z0, z6))

/*
** asr_wide_s8_x_untied:
**	asr	z0\.b, z1\.b, z6\.d
**	ret
*/
TEST_WIDE_Z (asr_wide_s8_x_untied, svint8_t,
	     z0 = svasr_wide_s8_x (p0, z1, z6),
	     z0 = svasr_wide_x (p0, z1, z6))

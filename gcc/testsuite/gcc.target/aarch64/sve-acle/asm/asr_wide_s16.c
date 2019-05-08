/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** asr_wide_s16_m_tied1:
**	asr	z0\.h, p0/m, z0\.h, z16\.d
**	ret
*/
TEST_WIDE_Z (asr_wide_s16_m_tied1, svint16_t,
	     z0 = svasr_wide_s16_m (p0, z0, z16),
	     z0 = svasr_wide_m (p0, z0, z16))

/*
** asr_wide_s16_m_tied2:
**	mov	(z[0-9]+\.d), z16\.d
**	movprfx	z16, z0
**	asr	z16\.h, p0/m, z16\.h, \1
**	ret
*/
TEST_WIDE_Z (asr_wide_s16_m_tied2, svint16_t,
	     z16_res = svasr_wide_s16_m (p0, z0, z16),
	     z16_res = svasr_wide_m (p0, z0, z16))

/*
** asr_wide_s16_m_untied:
**	movprfx	z0, z1
**	asr	z0\.h, p0/m, z0\.h, z16\.d
**	ret
*/
TEST_WIDE_Z (asr_wide_s16_m_untied, svint16_t,
	     z0 = svasr_wide_s16_m (p0, z1, z16),
	     z0 = svasr_wide_m (p0, z1, z16))

/*
** asr_wide_s16_z_tied1:
**	movprfx	z0\.h, p0/z, z0\.h
**	asr	z0\.h, p0/m, z0\.h, z16\.d
**	ret
*/
TEST_WIDE_Z (asr_wide_s16_z_tied1, svint16_t,
	     z0 = svasr_wide_s16_z (p0, z0, z16),
	     z0 = svasr_wide_z (p0, z0, z16))

/*
** asr_wide_s16_z_tied2:
**	mov	(z[0-9]+\.d), z16\.d
**	movprfx	z16\.h, p0/z, z0\.h
**	asr	z16\.h, p0/m, z16\.h, \1
**	ret
*/
TEST_WIDE_Z (asr_wide_s16_z_tied2, svint16_t,
	     z16_res = svasr_wide_s16_z (p0, z0, z16),
	     z16_res = svasr_wide_z (p0, z0, z16))

/*
** asr_wide_s16_z_untied:
**	movprfx	z0\.h, p0/z, z1\.h
**	asr	z0\.h, p0/m, z0\.h, z16\.d
**	ret
*/
TEST_WIDE_Z (asr_wide_s16_z_untied, svint16_t,
	     z0 = svasr_wide_s16_z (p0, z1, z16),
	     z0 = svasr_wide_z (p0, z1, z16))

/*
** asr_wide_s16_x_tied1:
**	asr	z0\.h, z0\.h, z16\.d
**	ret
*/
TEST_WIDE_Z (asr_wide_s16_x_tied1, svint16_t,
	     z0 = svasr_wide_s16_x (p0, z0, z16),
	     z0 = svasr_wide_x (p0, z0, z16))

/*
** asr_wide_s16_x_tied2:
**	asr	z16\.h, z0\.h, z16\.d
**	ret
*/
TEST_WIDE_Z (asr_wide_s16_x_tied2, svint16_t,
	     z16_res = svasr_wide_s16_x (p0, z0, z16),
	     z16_res = svasr_wide_x (p0, z0, z16))

/*
** asr_wide_s16_x_untied:
**	asr	z0\.h, z1\.h, z16\.d
**	ret
*/
TEST_WIDE_Z (asr_wide_s16_x_untied, svint16_t,
	     z0 = svasr_wide_s16_x (p0, z1, z16),
	     z0 = svasr_wide_x (p0, z1, z16))

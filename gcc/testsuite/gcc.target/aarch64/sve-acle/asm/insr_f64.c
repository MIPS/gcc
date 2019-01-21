/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** insr_x0_f64_tied1:
**	insr	z0\.d, x0
**	ret
*/
TEST_UNIFORM_ZS (insr_x0_f64_tied1, svfloat64_t, double,
		 z0 = svinsr_n_f64 (z0, x0),
		 z0 = svinsr (z0, x0))

/*
** insr_x0_f64_untied:
**	movprfx	z0, z1
**	insr	z0\.d, x0
**	ret
*/
TEST_UNIFORM_ZS (insr_x0_f64_untied, svfloat64_t, double,
		 z0 = svinsr_n_f64 (z1, x0),
		 z0 = svinsr (z1, x0))

/*
** insr_d4_f64_tied1:
**	insr	z0\.d, d4
**	ret
*/
TEST_UNIFORM_ZS (insr_d4_f64_tied1, svfloat64_t, double,
		 z0 = svinsr_n_f64 (z0, d4),
		 z0 = svinsr (z0, d4))

/*
** insr_d4_f64_untied:
**	movprfx	z0, z1
**	insr	z0\.d, d4
**	ret
*/
TEST_UNIFORM_ZS (insr_d4_f64_untied, svfloat64_t, double,
		 z0 = svinsr_n_f64 (z1, d4),
		 z0 = svinsr (z1, d4))

/*
** insr_0_f64_tied1:
**	insr	z0\.d, xzr
**	ret
*/
TEST_UNIFORM_Z (insr_0_f64_tied1, svfloat64_t,
		z0 = svinsr_n_f64 (z0, 0),
		z0 = svinsr (z0, 0))

/*
** insr_0_f64_untied:
**	movprfx	z0, z1
**	insr	z0\.d, xzr
**	ret
*/
TEST_UNIFORM_Z (insr_0_f64_untied, svfloat64_t,
		z0 = svinsr_n_f64 (z1, 0),
		z0 = svinsr (z1, 0))

/*
** insr_1_f64:
**	fmov	(d[0-9]+), #?1\.0(?:e\+0)?
**	insr	z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (insr_1_f64, svfloat64_t,
		z0 = svinsr_n_f64 (z0, 1),
		z0 = svinsr (z0, 1))

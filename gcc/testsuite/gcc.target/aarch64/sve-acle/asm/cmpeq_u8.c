/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** cmpeq_u8_tied:
**	cmpeq	p0\.b, p0/z, (z0\.b, z1\.b|z1\.b, z0\.b)
**	ret
*/
TEST_COMPARE_Z (cmpeq_u8_tied, svuint8_t,
		p0 = svcmpeq_u8 (p0, z0, z1),
		p0 = svcmpeq (p0, z0, z1))

/*
** cmpeq_u8_untied:
**	cmpeq	p0\.b, p1/z, (z0\.b, z1\.b|z1\.b, z0\.b)
**	ret
*/
TEST_COMPARE_Z (cmpeq_u8_untied, svuint8_t,
		p0 = svcmpeq_u8 (p1, z0, z1),
		p0 = svcmpeq (p1, z0, z1))

/*
** cmpeq_x0_u8:
**	mov	(z[0-9]+\.d), x0
**	cmpeq	p0\.b, p1/z, z0\.b, \1
**	ret
*/
TEST_COMPARE_ZS (cmpeq_x0_u8, svuint8_t, uint64_t,
		 p0 = svcmpeq_n_u8 (p1, z0, x0),
		 p0 = svcmpeq (p1, z0, x0))

/*
** cmpeq_d4_u8:
**	mov	(z[0-9]+\.d), d4
**	cmpeq	p0\.b, p1/z, z0\.b, \1
**	ret
*/
TEST_COMPARE_ZS (cmpeq_d4_u8, svuint8_t, uint64_t,
		 p0 = svcmpeq_n_u8 (p1, z0, d4),
		 p0 = svcmpeq (p1, z0, d4))

/*
** cmpeq_0_u8:
**	cmpeq	p0\.b, p1/z, z0\.b, #0
**	ret
*/
TEST_COMPARE_ZS (cmpeq_0_u8, svuint8_t, uint64_t,
		 p0 = svcmpeq_n_u8 (p1, z0, 0),
		 p0 = svcmpeq (p1, z0, 0))

/*
** cmpeq_1_u8:
**	cmpeq	p0\.b, p1/z, z0\.b, #1
**	ret
*/
TEST_COMPARE_ZS (cmpeq_1_u8, svuint8_t, uint64_t,
		 p0 = svcmpeq_n_u8 (p1, z0, 1),
		 p0 = svcmpeq (p1, z0, 1))

/*
** cmpeq_15_u8:
**	cmpeq	p0\.b, p1/z, z0\.b, #15
**	ret
*/
TEST_COMPARE_ZS (cmpeq_15_u8, svuint8_t, uint64_t,
		 p0 = svcmpeq_n_u8 (p1, z0, 15),
		 p0 = svcmpeq (p1, z0, 15))

/*
** cmpeq_16_u8:
**	mov	(z[0-9]+\.d), #16
**	cmpeq	p0\.b, p1/z, z0\.b, \1
**	ret
*/
TEST_COMPARE_ZS (cmpeq_16_u8, svuint8_t, uint64_t,
		 p0 = svcmpeq_n_u8 (p1, z0, 16),
		 p0 = svcmpeq (p1, z0, 16))

/*
** cmpeq_m1_u8:
**	cmpeq	p0\.b, p1/z, z0\.b, #-1
**	ret
*/
TEST_COMPARE_ZS (cmpeq_m1_u8, svuint8_t, uint64_t,
		 p0 = svcmpeq_n_u8 (p1, z0, -1),
		 p0 = svcmpeq (p1, z0, -1))

/*
** cmpeq_m16_u8:
**	cmpeq	p0\.b, p1/z, z0\.b, #-16
**	ret
*/
TEST_COMPARE_ZS (cmpeq_m16_u8, svuint8_t, uint64_t,
		 p0 = svcmpeq_n_u8 (p1, z0, -16),
		 p0 = svcmpeq (p1, z0, -16))

/*
** cmpeq_m17_u8:
**	mov	(z[0-9]+\.d), #-17
**	cmpeq	p0\.b, p1/z, z0\.b, \1
**	ret
*/
TEST_COMPARE_ZS (cmpeq_m17_u8, svuint8_t, uint64_t,
		 p0 = svcmpeq_n_u8 (p1, z0, -17),
		 p0 = svcmpeq (p1, z0, -17))

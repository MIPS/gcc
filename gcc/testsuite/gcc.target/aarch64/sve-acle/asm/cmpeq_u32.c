/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** cmpeq_u32_tied:
**	cmpeq	p0\.s, p0/z, (z0\.s, z1\.s|z1\.s, z0\.s)
**	ret
*/
TEST_COMPARE_Z (cmpeq_u32_tied, svuint32_t,
		p0 = svcmpeq_u32 (p0, z0, z1),
		p0 = svcmpeq (p0, z0, z1))

/*
** cmpeq_u32_untied:
**	cmpeq	p0\.s, p1/z, (z0\.s, z1\.s|z1\.s, z0\.s)
**	ret
*/
TEST_COMPARE_Z (cmpeq_u32_untied, svuint32_t,
		p0 = svcmpeq_u32 (p1, z0, z1),
		p0 = svcmpeq (p1, z0, z1))

/*
** cmpeq_x0_u32:
**	mov	(z[0-9]+\.d), x0
**	cmpeq	p0\.s, p1/z, z0\.s, \1
**	ret
*/
TEST_COMPARE_ZS (cmpeq_x0_u32, svuint32_t, uint64_t,
		 p0 = svcmpeq_n_u32 (p1, z0, x0),
		 p0 = svcmpeq (p1, z0, x0))

/*
** cmpeq_d4_u32:
**	mov	(z[0-9]+\.d), d4
**	cmpeq	p0\.s, p1/z, z0\.s, \1
**	ret
*/
TEST_COMPARE_ZS (cmpeq_d4_u32, svuint32_t, uint64_t,
		 p0 = svcmpeq_n_u32 (p1, z0, d4),
		 p0 = svcmpeq (p1, z0, d4))

/*
** cmpeq_0_u32:
**	cmpeq	p0\.s, p1/z, z0\.s, #0
**	ret
*/
TEST_COMPARE_ZS (cmpeq_0_u32, svuint32_t, uint64_t,
		 p0 = svcmpeq_n_u32 (p1, z0, 0),
		 p0 = svcmpeq (p1, z0, 0))

/*
** cmpeq_1_u32:
**	cmpeq	p0\.s, p1/z, z0\.s, #1
**	ret
*/
TEST_COMPARE_ZS (cmpeq_1_u32, svuint32_t, uint64_t,
		 p0 = svcmpeq_n_u32 (p1, z0, 1),
		 p0 = svcmpeq (p1, z0, 1))

/*
** cmpeq_15_u32:
**	cmpeq	p0\.s, p1/z, z0\.s, #15
**	ret
*/
TEST_COMPARE_ZS (cmpeq_15_u32, svuint32_t, uint64_t,
		 p0 = svcmpeq_n_u32 (p1, z0, 15),
		 p0 = svcmpeq (p1, z0, 15))

/*
** cmpeq_16_u32:
**	mov	(z[0-9]+\.d), #16
**	cmpeq	p0\.s, p1/z, z0\.s, \1
**	ret
*/
TEST_COMPARE_ZS (cmpeq_16_u32, svuint32_t, uint64_t,
		 p0 = svcmpeq_n_u32 (p1, z0, 16),
		 p0 = svcmpeq (p1, z0, 16))

/*
** cmpeq_m1_u32:
**	cmpeq	p0\.s, p1/z, z0\.s, #-1
**	ret
*/
TEST_COMPARE_ZS (cmpeq_m1_u32, svuint32_t, uint64_t,
		 p0 = svcmpeq_n_u32 (p1, z0, -1),
		 p0 = svcmpeq (p1, z0, -1))

/*
** cmpeq_m16_u32:
**	cmpeq	p0\.s, p1/z, z0\.s, #-16
**	ret
*/
TEST_COMPARE_ZS (cmpeq_m16_u32, svuint32_t, uint64_t,
		 p0 = svcmpeq_n_u32 (p1, z0, -16),
		 p0 = svcmpeq (p1, z0, -16))

/*
** cmpeq_m17_u32:
**	mov	(z[0-9]+\.d), #-17
**	cmpeq	p0\.s, p1/z, z0\.s, \1
**	ret
*/
TEST_COMPARE_ZS (cmpeq_m17_u32, svuint32_t, uint64_t,
		 p0 = svcmpeq_n_u32 (p1, z0, -17),
		 p0 = svcmpeq (p1, z0, -17))

/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** cmpgt_s8_tied:
** (
**	cmpgt	p0\.b, p0/z, z0\.b, z1\.b
** |
**	cmplt	p0\.b, p0/z, z1\.b, z0\.b
** )
**	ret
*/
TEST_COMPARE_Z (cmpgt_s8_tied, svint8_t,
		p0 = svcmpgt_s8 (p0, z0, z1),
		p0 = svcmpgt (p0, z0, z1))

/*
** cmpgt_s8_untied:
** (
**	cmpgt	p0\.b, p1/z, z0\.b, z1\.b
** |
**	cmplt	p0\.b, p1/z, z1\.b, z0\.b
** )
**	ret
*/
TEST_COMPARE_Z (cmpgt_s8_untied, svint8_t,
		p0 = svcmpgt_s8 (p1, z0, z1),
		p0 = svcmpgt (p1, z0, z1))

/*
** cmpgt_x0_s8:
**	mov	(z[0-9]+\.d), x0
**	cmpgt	p0\.b, p1/z, z0\.b, \1
**	ret
*/
TEST_COMPARE_ZS (cmpgt_x0_s8, svint8_t, int64_t,
		 p0 = svcmpgt_n_s8 (p1, z0, x0),
		 p0 = svcmpgt (p1, z0, x0))

/*
** cmpgt_d4_s8:
**	mov	(z[0-9]+\.d), d4
**	cmpgt	p0\.b, p1/z, z0\.b, \1
**	ret
*/
TEST_COMPARE_ZS (cmpgt_d4_s8, svint8_t, int64_t,
		 p0 = svcmpgt_n_s8 (p1, z0, d4),
		 p0 = svcmpgt (p1, z0, d4))

/*
** cmpgt_0_s8:
**	cmpgt	p0\.b, p1/z, z0\.b, #0
**	ret
*/
TEST_COMPARE_ZS (cmpgt_0_s8, svint8_t, int64_t,
		 p0 = svcmpgt_n_s8 (p1, z0, 0),
		 p0 = svcmpgt (p1, z0, 0))

/*
** cmpgt_1_s8:
**	cmpgt	p0\.b, p1/z, z0\.b, #1
**	ret
*/
TEST_COMPARE_ZS (cmpgt_1_s8, svint8_t, int64_t,
		 p0 = svcmpgt_n_s8 (p1, z0, 1),
		 p0 = svcmpgt (p1, z0, 1))

/*
** cmpgt_15_s8:
**	cmpgt	p0\.b, p1/z, z0\.b, #15
**	ret
*/
TEST_COMPARE_ZS (cmpgt_15_s8, svint8_t, int64_t,
		 p0 = svcmpgt_n_s8 (p1, z0, 15),
		 p0 = svcmpgt (p1, z0, 15))

/*
** cmpgt_16_s8:
**	mov	(z[0-9]+\.d), #16
**	cmpgt	p0\.b, p1/z, z0\.b, \1
**	ret
*/
TEST_COMPARE_ZS (cmpgt_16_s8, svint8_t, int64_t,
		 p0 = svcmpgt_n_s8 (p1, z0, 16),
		 p0 = svcmpgt (p1, z0, 16))

/*
** cmpgt_m1_s8:
**	cmpgt	p0\.b, p1/z, z0\.b, #-1
**	ret
*/
TEST_COMPARE_ZS (cmpgt_m1_s8, svint8_t, int64_t,
		 p0 = svcmpgt_n_s8 (p1, z0, -1),
		 p0 = svcmpgt (p1, z0, -1))

/*
** cmpgt_m16_s8:
**	cmpgt	p0\.b, p1/z, z0\.b, #-16
**	ret
*/
TEST_COMPARE_ZS (cmpgt_m16_s8, svint8_t, int64_t,
		 p0 = svcmpgt_n_s8 (p1, z0, -16),
		 p0 = svcmpgt (p1, z0, -16))

/*
** cmpgt_m17_s8:
**	mov	(z[0-9]+\.d), #-17
**	cmpgt	p0\.b, p1/z, z0\.b, \1
**	ret
*/
TEST_COMPARE_ZS (cmpgt_m17_s8, svint8_t, int64_t,
		 p0 = svcmpgt_n_s8 (p1, z0, -17),
		 p0 = svcmpgt (p1, z0, -17))

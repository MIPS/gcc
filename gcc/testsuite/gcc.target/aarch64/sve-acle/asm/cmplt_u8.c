/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** cmplt_u8_tied:
** (
**	cmphi	p0\.b, p0/z, z1\.b, z0\.b
** |
**	cmplo	p0\.b, p0/z, z0\.b, z1\.b
** )
**	ret
*/
TEST_COMPARE_Z (cmplt_u8_tied, svuint8_t,
		p0 = svcmplt_u8 (p0, z0, z1),
		p0 = svcmplt (p0, z0, z1))

/*
** cmplt_u8_untied:
** (
**	cmphi	p0\.b, p1/z, z1\.b, z0\.b
** |
**	cmplo	p0\.b, p1/z, z0\.b, z1\.b
** )
**	ret
*/
TEST_COMPARE_Z (cmplt_u8_untied, svuint8_t,
		p0 = svcmplt_u8 (p1, z0, z1),
		p0 = svcmplt (p1, z0, z1))

/*
** cmplt_x0_u8:
**	mov	(z[0-9]+\.d), x0
**	cmplo	p0\.b, p1/z, z0\.b, \1
**	ret
*/
TEST_COMPARE_ZS (cmplt_x0_u8, svuint8_t, uint64_t,
		 p0 = svcmplt_n_u8 (p1, z0, x0),
		 p0 = svcmplt (p1, z0, x0))

/*
** cmplt_d4_u8:
**	mov	(z[0-9]+\.d), d4
**	cmplo	p0\.b, p1/z, z0\.b, \1
**	ret
*/
TEST_COMPARE_ZS (cmplt_d4_u8, svuint8_t, uint64_t,
		 p0 = svcmplt_n_u8 (p1, z0, d4),
		 p0 = svcmplt (p1, z0, d4))

/*
** cmplt_0_u8:
**	cmplo	p0\.b, p1/z, z0\.b, #0
**	ret
*/
TEST_COMPARE_ZS (cmplt_0_u8, svuint8_t, uint64_t,
		 p0 = svcmplt_n_u8 (p1, z0, 0),
		 p0 = svcmplt (p1, z0, 0))

/*
** cmplt_1_u8:
**	cmplo	p0\.b, p1/z, z0\.b, #1
**	ret
*/
TEST_COMPARE_ZS (cmplt_1_u8, svuint8_t, uint64_t,
		 p0 = svcmplt_n_u8 (p1, z0, 1),
		 p0 = svcmplt (p1, z0, 1))

/*
** cmplt_15_u8:
**	cmplo	p0\.b, p1/z, z0\.b, #15
**	ret
*/
TEST_COMPARE_ZS (cmplt_15_u8, svuint8_t, uint64_t,
		 p0 = svcmplt_n_u8 (p1, z0, 15),
		 p0 = svcmplt (p1, z0, 15))

/*
** cmplt_16_u8:
**	cmplo	p0\.b, p1/z, z0\.b, #16
**	ret
*/
TEST_COMPARE_ZS (cmplt_16_u8, svuint8_t, uint64_t,
		 p0 = svcmplt_n_u8 (p1, z0, 16),
		 p0 = svcmplt (p1, z0, 16))

/*
** cmplt_127_u8:
**	cmplo	p0\.b, p1/z, z0\.b, #127
**	ret
*/
TEST_COMPARE_ZS (cmplt_127_u8, svuint8_t, uint64_t,
		 p0 = svcmplt_n_u8 (p1, z0, 127),
		 p0 = svcmplt (p1, z0, 127))

/*
** cmplt_128_u8:
**	mov	(z[0-9]+\.d), #128
**	cmplo	p0\.b, p1/z, z0\.b, \1
**	ret
*/
TEST_COMPARE_ZS (cmplt_128_u8, svuint8_t, uint64_t,
		 p0 = svcmplt_n_u8 (p1, z0, 128),
		 p0 = svcmplt (p1, z0, 128))

/*
** cmplt_m1_u8:
**	mov	(z[0-9]+)\.b, #-1
**	cmplo	p0\.b, p1/z, z0\.b, \1\.d
**	ret
*/
TEST_COMPARE_ZS (cmplt_m1_u8, svuint8_t, uint64_t,
		 p0 = svcmplt_n_u8 (p1, z0, -1),
		 p0 = svcmplt (p1, z0, -1))

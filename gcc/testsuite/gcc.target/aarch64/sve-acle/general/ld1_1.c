/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include <arm_sve.h>

/*
 * nop1:
 *	ret
 */
void nop1 (int8_t *s) { svld1 (svptrue_b8 (), s); }

/*
 * nop2:
 *	ret
 */
void nop2 (svbool_t pg, int16_t *s) { svld1 (pg, s); }

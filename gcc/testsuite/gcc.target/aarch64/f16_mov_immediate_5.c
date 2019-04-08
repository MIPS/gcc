/* { dg-do compile } */
/* { dg-options "-O" } */
/* { dg-require-effective-target arm_v8_2a_fp16_scalar_ok } */
/* { dg-add-options arm_v8_2a_fp16_scalar } */

#include "f16_mov_immediate_2.c"

/* { dg-final { scan-assembler-times "movi\tv\[0-9\]+\\\.4h, ?#0"         1 } } */
/* { dg-final { scan-assembler-times "movi\tv\[0-9\]+\\\.4h, 0x80, lsl 8" 1 } } */
/* { dg-final { scan-assembler-times "movi\tv\[0-9\]+\\\.4h, 0x5c, lsl 8" 1 } } */
/* { dg-final { scan-assembler-times "movi\tv\[0-9\]+\\\.4h, 0x7c, lsl 8" 1 } } */

/* { dg-final { scan-assembler-times {fmov\th[0-9]+, #?1.7e\+1}           1 } } */

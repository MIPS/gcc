/* { dg-do compile } */
/* { dg-options "-O" } */

#include "flt_mov_immediate_1.c"

/* { dg-final { scan-assembler-times "movi\tv\[0-9\]+\\\.2s, ?#0"           1 } } */
/* { dg-final { scan-assembler-times "movi\tv\[0-9\]+\\\.2s, 0x80, lsl 24"  1 } } */
/* { dg-final { scan-assembler-times "movi\tv\[0-9\]+\\\.2s, 0x80, lsl 24"  1 } } */

/* { dg-final { scan-assembler-times "mov\tw\[0-9\]+, 48128"                1 } } */
/* { dg-final { scan-assembler-times "movk\tw\[0-9\]+, 0x47f0, lsl 16"      1 } } */

/* { dg-final { scan-assembler-times "fmov\ts\[0-9\]+, 2\\\.0e\\\+0"  1 } } */

/* { dg-final { scan-assembler-times "mov\tw\[0-9\]+, 16435"                1 } } */
/* { dg-final { scan-assembler-times "movk\tw\[0-9\]+, 0xc69c, lsl 16"      1 } } */


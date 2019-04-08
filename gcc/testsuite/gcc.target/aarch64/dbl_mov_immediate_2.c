/* { dg-do compile } */
/* { dg-options "-O -mno-pc-relative-literal-loads" } */
/* { dg-skip-if "Tiny model won't generate adrp" { *-*-* } { "-mcmodel=tiny" } { "" } } */

#include "dbl_mov_immediate_1.c"

/* { dg-final { scan-assembler-times "movi\td\[0-9\]+, #?0"                 1 } } */

/* { dg-final { scan-assembler-times "adrp\tx\[0-9\]+, \.LC\[0-9\]"         2 } } */
/* { dg-final { scan-assembler-times "ldr\td\[0-9\]+, \\\[x\[0-9\], #:lo12:\.LC\[0-9\]\\\]" 2 } } */

/* { dg-final { scan-assembler-times "fmov\td\[0-9\]+, 1\\\.5e\\\+0"        1 } } */

/* { dg-final { scan-assembler-times "mov\tx\[0-9\]+, 25838523252736"       1 } } */
/* { dg-final { scan-assembler-times "movk\tx\[0-9\]+, 0x40fe, lsl 48"      1 } } */
/* { dg-final { scan-assembler-times "mov\tx\[0-9\]+, -9223372036854775808" 1 } } */
/* { dg-final { scan-assembler-times "fmov\td\[0-9\]+, x\[0-9\]+"           2 } } */


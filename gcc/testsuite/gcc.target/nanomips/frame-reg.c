/* { dg-do compile } */
/* { dg-options "-march=32r6 -fpic -mcmodel=auto -fasynchronous-unwind-tables" } */
/* { dg-additional-options "-mrelax-saverestore-gp" } */

extern int val;

int foo (void) { return val; }

/* { dg-final {scan-assembler-times "\t\.reloc\t.LFB\[0-9\]+\[\\+1|\]*,R_NANOMIPS_FRAME_REG,foo\n" 2 } } */
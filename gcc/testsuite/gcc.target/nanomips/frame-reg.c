/* { dg-do compile } */
/* { dg-options "-march=32r6 -fpic -mcmodel=auto" } */
/* { dg-additional-options "-mrelax-saverestore-gp -fasynchronous-unwind-tables" } */

extern int bar();

int foo (void) { return bar(); }

/* { dg-final {scan-assembler-times "\t\.reloc\t.LFF\[0-9\]+\[\\+1|\]*,R_NANOMIPS_FRAME_REG,foo\n" 2 } } */
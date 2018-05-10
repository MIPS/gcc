/* { dg-do compile } */
/* { dg-options "-march=32r6 -mno-pcrel -mno-pid" } */
/* { dg-final { scan-assembler-not "\\\.module\tpid" } } */
/* { dg-final { scan-assembler-not "\\\.module\tpcrel" } } */

void foo (void) { }

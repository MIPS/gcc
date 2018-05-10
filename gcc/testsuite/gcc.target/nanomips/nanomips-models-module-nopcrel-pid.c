/* { dg-do compile } */
/* { dg-options "-march=32r6 -mno-pcrel -mpid" } */
/* { dg-final { scan-assembler "\\\.module\tpid" } } */
/* { dg-final { scan-assembler-not "\\\.module\tpcrel" } } */

void foo (void) { }

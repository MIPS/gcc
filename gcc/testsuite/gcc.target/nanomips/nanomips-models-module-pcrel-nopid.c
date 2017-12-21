/* { dg-do compile } */
/* { dg-options "-march=32r6 -mpcrel -mno-pid" } */
/* { dg-final { scan-assembler-not "\\\.module\tpid" } } */
/* { dg-final { scan-assembler "\\\.module\tpcrel" } } */

void foo (void) { }

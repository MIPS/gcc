/* { dg-do compile } */
/* { dg-options "-march=32r6 -fpic" } */
/* { dg-final { scan-assembler "\\\.module\tpic1" } } */

void foo (void) { }

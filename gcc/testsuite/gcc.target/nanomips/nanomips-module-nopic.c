/* { dg-do compile } */
/* { dg-options "-march=32r6 -fno-pic" } */
/* { dg-final { scan-assembler "\\\.module\tpic0" } } */

void foo (void) { }

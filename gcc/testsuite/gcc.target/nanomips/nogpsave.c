/* { dg-do compile } */
/* { dg-options "-fpic" } */
/* { dg-final { scan-assembler-not "\\\$gp" } } */

void foo (void) { }

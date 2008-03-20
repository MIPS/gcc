/* { dg-do compile } */
/* { dg-options "-mpreferred-stack-boundary=4" } */
/* { dg-final { scan-assembler-not "andl\[^\\n\]*-64,\[^\\n\]*sp" } } */

typedef int aligned __attribute__((aligned(64)));

aligned
foo (void) { }

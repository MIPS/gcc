/* { dg-do compile } */
/* { dg-options "-mpreferred-stack-boundary=4" } */
/* { dg-final { scan-assembler-not "andl\[^\\n\]*-64,\[^\\n\]*sp" } } */

struct bar
{
  int x;
} __attribute__((aligned(64)));


struct bar
foo (void) { }

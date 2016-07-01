/* { dg-do run { target { powerpc*-*-linux* && lp64 } } } */
/* { dg-skip-if "" { powerpc*-*-darwin* } { "*" } { "" } } */
/* { dg-skip-if "" { powerpc*-*-*spe* } { "*" } { "" } } */
/* { dg-require-effective-target p8vector_hw } */
/* { dg-skip-if "do not override -mcpu" { powerpc*-*-* } { "-mcpu=*" } { "-mcpu=power8" } } */
/* { dg-options "-mcpu=power8 -O2" } */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <altivec.h>

extern void test_element (vector long, long, long) __attribute__((__noinline__));
extern vector long deoptimize (vector long) __attribute__((__noinline__));

vector long deoptimize (vector long a)
{
  return a;
}

void
test_element (vector long a, long e, long n)
{
  long v = vec_extract (a, n);
  if (v != n)
    abort ();
}

int
main (void)
{
  vector long a = { 10, -20 };

  a = deoptimize (a);
  test_element (a, vec_extract (a, 0), 0);
  test_element (a, vec_extract (a, 1), 1);
  return 0;
}

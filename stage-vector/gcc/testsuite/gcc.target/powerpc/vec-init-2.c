/* { dg-do run { target { powerpc*-*-linux* && lp64 } } } */
/* { dg-require-effective-target vsx_hw } */
/* { dg-options "-O2 -mvsx" } */

#include <stdlib.h>
#include <stddef.h>
#include <altivec.h>

extern void check (vector long a)       __attribute__((__noinline__));
extern vector int pack (long a, long b) __attribute__((__noinline__));

void
check (vector long a)
{
  static const long expected[] = { 2L, -3L };
  size_t i;

  for (i = 0; i < 2; i++)
    if (vec_extract (a, i) != expected[i])
      abort ();
}

vector long
pack (long a, long b)
{
  return (vector long) { a, b };
}

vector long sv = (vector long) { 2L, -3L };

int main (void)
{
  check (sv);
  check (pack (2L, -3L));
  return 0;
}

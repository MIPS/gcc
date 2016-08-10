/* { dg-do run { target { powerpc*-*-linux* } } } */
/* { dg-require-effective-target vsx_hw } */
/* { dg-options "-O2 -mvsx" } */

#include <stdlib.h>
#include <stddef.h>
#include <altivec.h>

extern void check (vector int a)
  __attribute__((__noinline__));

extern vector int pack (int a, int b, int c, int d)
  __attribute__((__noinline__));

extern void pack2 (vector int *p, int a, int b, int c, int d)
  __attribute__((__noinline__));

void
check (vector int a)
{
  static const int expected[] = { -1, 2, 0, -3 };
  size_t i;

  for (i = 0; i < 4; i++)
    if (vec_extract (a, i) != expected[i])
      abort ();
}

vector int
pack (int a, int b, int c, int d)
{
  return (vector int) { a, b, c, d };
}

void
pack2 (vector int *p, int a, int b, int c, int d)
{
  *p = (vector int) { a, b, c, d };
}

vector int sv = (vector int) { -1, 2, 0, -3 };
vector int sv2;

int main (void)
{
  check (sv);
  check (pack (-1, 2, 0, -3));
  pack2 (&sv2, -1, 2, 0, -3);
  check (sv2);
  return 0;
}

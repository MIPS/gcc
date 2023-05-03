/* Check that typedef alignment does not affect passing of function
   parameters. */
/* { dg-do run { target { "mips*-*-linux*" } } } */

#include <assert.h>

typedef struct ui8
{
  unsigned v[8];
} uint8 __attribute__ ((aligned(64)));

unsigned
callee (int x, uint8 a)
{
  return a.v[0];
}

uint8
identity (uint8 in)
{
  return in;
}

int
main (void)
{
  uint8 vec = {{1, 2, 3, 4, 5, 6, 7, 8}};
  uint8 temp = identity (vec);
  unsigned temp2 = callee (1, identity (vec));
  assert (callee (1, temp) == 1);
  assert (temp2 == 1);
  return 0;
}

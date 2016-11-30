/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-O3 -march=armv8-a+sve" } */

#include "sve_speculative_27.c"

#define N 60
#define COUNT 37

short array1[N];
short array2[N];

int
main (void)
{
  int expected = 0;
  for (unsigned int i = 0; i < N; ++i)
    {
      array1[i] = 2 * i + (i & 1) + 1;
      array2[i] = i * ((i & 3) + 1);
      if (i < COUNT)
	expected += array2[i] * i;
      asm volatile ("");
    }
  array1[COUNT] = 0;
  int res = foo (array1, array2);
  if (res != expected)
    __builtin_abort ();
  return 0;
}

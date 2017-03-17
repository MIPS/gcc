/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize -march=armv8-a+sve" } */

#include "sve_struct_vect_21.c"

volatile int x;

#undef TEST_LOOP
#define TEST_LOOP(NAME, TYPE)				\
  {							\
    TYPE out[N];					\
    TYPE in[N * 3];					\
    for (int i = 0; i < N; ++i)				\
      out[i] = i * 7 / 2;				\
    for (int i = 0; i < N * 3; ++i)			\
      in[i] = i * 9 / 2;				\
    NAME (out, in);					\
    for (int i = 0; i < N; ++i)				\
      {							\
	TYPE expected = i * 7 / 2 + in[i * 3];		\
	if (out[i] != expected)				\
	  __builtin_abort ();				\
	x += 1;						\
      }							\
  }

int
main (void)
{
  TEST (test);
  return 0;
}

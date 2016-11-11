/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize -fno-tree-dce -ffast-math -march=armv8-a+sve" } */

#include "sve_mask_struct_load_1.c"

#define N 100

volatile int x;

#undef TEST_LOOP
#define TEST_LOOP(NAME, OUTTYPE, INTYPE, MASKTYPE)	\
  {							\
    OUTTYPE out[N];					\
    INTYPE in[N * 2];					\
    MASKTYPE mask[N];					\
    for (int i = 0; i < N; ++i)				\
      {							\
	out[i] = i * 7 / 2;				\
	mask[i] = i % 5 <= i % 3;			\
      }							\
    for (int i = 0; i < N * 2; ++i)			\
      in[i] = i * 9 / 2;				\
    NAME##_2 (out, in, mask, N);			\
    for (int i = 0; i < N; ++i)				\
      {							\
	OUTTYPE if_true = in[i * 2] + in[i * 2 + 1];	\
	OUTTYPE if_false = i * 7 / 2;			\
	if (out[i] != (mask[i] ? if_true : if_false))	\
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

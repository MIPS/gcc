/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize -fno-tree-dce -ffast-math -march=armv8-a+sve" } */

#include "sve_mask_struct_store_2.c"

#define N 100

volatile int x;

#undef TEST_LOOP
#define TEST_LOOP(NAME, OUTTYPE, INTYPE, MASKTYPE)		\
  {								\
    OUTTYPE out[N * 3];						\
    INTYPE in[N];						\
    MASKTYPE mask[N];						\
    for (int i = 0; i < N; ++i)					\
      {								\
	in[i] = i * 7 / 2;					\
	mask[i] = i % 5 <= i % 3;				\
      }								\
    for (int i = 0; i < N * 3; ++i)				\
      out[i] = i * 9 / 2;					\
    NAME##_3 (out, in, mask, N);				\
    for (int i = 0; i < N * 3; ++i)				\
      {								\
	OUTTYPE if_true = in[i / 3];				\
	OUTTYPE if_false = i * 9 / 2;				\
	if (out[i] != (mask[i / 3] ? if_true : if_false))	\
	  __builtin_abort ();					\
	x += 1;							\
      }								\
  }

int
main (void)
{
  TEST (test);
  return 0;
}

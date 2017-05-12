/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-O2 -ftree-vectorize -fno-inline -march=armv8-a+sve -fno-vect-cost-model" } */

#include "sve_cap_1.c"

#define N 129

#define F(X) (((X) % 5) * (X))

#define TEST_LOOP(TYPE, M)			\
  {						\
    TYPE a[N + M];				\
    for (int i = 0; i < N + M; ++i)		\
      a[i] = F (i);				\
    f_##TYPE##_##M (a + M, N);			\
    for (int i = 0; i < N; ++i)			\
      {						\
	TYPE x = a[i];				\
	TYPE y = F (i + M);			\
	if (a[i + M] != (TYPE) (x + y))		\
	  __builtin_abort ();			\
      }						\
  }

#define TEST_LOOPS(TYPE) \
  TEST_LOOP (TYPE, 1) \
  TEST_LOOP (TYPE, 2) \
  TEST_LOOP (TYPE, 5)

int
main (void)
{
  TEST_LOOPS (char);
  TEST_LOOPS (short);
  TEST_LOOPS (float);
  TEST_LOOPS (double);
}

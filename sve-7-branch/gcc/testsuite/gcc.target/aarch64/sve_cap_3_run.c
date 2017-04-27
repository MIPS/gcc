/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-O2 -ftree-vectorize -fno-inline -march=armv8-a+sve -fno-vect-cost-model" } */

#include "sve_cap_3.c"

#define N 129

#define F(X) (((X) % 5) * (X))
#define G(X) ((X) ^ 39)

#define TEST_LOOP(TYPE, M)				\
  {							\
    TYPE a[(N + M) * 2];				\
    for (int i = 0; i < N + M; ++i)			\
      {							\
        a[i * 2] = F (i);				\
        a[i * 2 + 1] = G (i);				\
      }							\
    f_##TYPE##_##M (a + M * 2, N);			\
    for (int i = 0; i < N; ++i)				\
      {							\
	TYPE x0 = a[i * 2];				\
	TYPE y0 = F (i + M);				\
	TYPE x1 = a[i * 2 + 1];				\
	TYPE y1 = G (i + M);				\
	if (a[(i + M) * 2] != (TYPE) (y0 + x0)		\
	    || a[(i + M) * 2 + 1] != (TYPE) (y1 - x1))	\
	  __builtin_abort ();				\
      }							\
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

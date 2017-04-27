/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-O2 -ftree-vectorize -fno-inline -march=armv8-a+sve -fno-tree-loop-distribute-patterns" } */
/* { dg-timeout 60 } */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

/* Make sure that in cases where
    n <= TYPE_MAX && n > (TYPE_MAX + 1 - (sizeof (SVE_VEC) / sizeof (TYPE)))
   that we don't iterate more times than we should due to overflow in the last
   iteration of loop.  If n == TYPE_MAX we could spin forever.  */

#define SIMPLE_LOOP(TYPE)			\
TYPE foo_##TYPE (TYPE n, TYPE * __restrict__ a)	\
{						\
  TYPE i;					\
  TYPE v = 0;					\
  for (i = 0; i < n; i++)			\
    v += a[i];					\
  return v;					\
}

SIMPLE_LOOP (uint8_t)
SIMPLE_LOOP (uint16_t)

/* Minimum architected SVE vector = 128 bits, i.e. 16 bytes. Just choose
   something that meets the critera shown above.  */
#define N_uint8_t  (UCHAR_MAX - 1)
#define N_uint16_t (USHRT_MAX - 1)

#define N_MAX 1024
#define DEF_VAR(TYPE)						\
  TYPE *a_##TYPE = (TYPE *) malloc (N_##TYPE * sizeof (TYPE));	\
  for (i = 0; i < N_##TYPE; i++)				\
    a_##TYPE[i] = 1;						\
  TYPE r_##TYPE;

#define TEST_SIMPLE_LOOP(TYPE) r_##TYPE = foo_##TYPE (N_##TYPE, a_##TYPE);

#define VERIFY(TYPE)							\
  if (r_##TYPE != N_##TYPE)						\
    {									\
      fprintf (stderr, "r_" #TYPE " = %ld\n", (uint64_t) r_##TYPE);	\
      abort ();								\
    }

int main ()
{
  int i;
  DEF_VAR (uint8_t)
  DEF_VAR (uint16_t)

  /* We only test 8 and 16 bit as others take too long.  */
  TEST_SIMPLE_LOOP (uint8_t)
  TEST_SIMPLE_LOOP (uint16_t)

  VERIFY (uint8_t)
  VERIFY (uint16_t)

  return 0;
}

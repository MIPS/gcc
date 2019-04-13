/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-O2 -ftree-vectorize" } */

#include "maxmin_3.c"

#define TEST_LOOP(TYPE, NAME, SUFFIX, CMP_OP, CONST)		\
  {								\
    TYPE a[NUM_ELEMS (TYPE)];					\
    TYPE r[NUM_ELEMS (TYPE)];					\
    TYPE b = CONST;						\
    for (int i = 0; i < NUM_ELEMS (TYPE); ++i)			\
      {								\
	a[i] = (i % 3 + (i / 5) * 63) * (i & 1 ? 1 : -1);	\
	asm volatile ("" ::: "memory");				\
      }								\
    test_##NAME##_##SUFFIX##_##TYPE (r, a);			\
    for (int i = 0; i < NUM_ELEMS (TYPE); ++i)			\
      if (r[i] != (a[i] CMP_OP b ? a[i] : b))			\
	__builtin_abort ();					\
  }

int main ()
{
  TEST_ALL (TEST_LOOP)
  return 0;
}

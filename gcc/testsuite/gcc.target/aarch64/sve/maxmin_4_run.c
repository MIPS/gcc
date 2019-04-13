/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-O2 -ftree-vectorize" } */

#include "maxmin_4.c"

#define TEST_LOOP(TYPE, NAME, CMP_OP, CONST)			\
  {								\
    TYPE a[NUM_ELEMS (TYPE)];					\
    TYPE r[NUM_ELEMS (TYPE)];					\
    TYPE b = CONST;						\
    for (int i = 0; i < NUM_ELEMS (TYPE); i++)			\
      {								\
	a[i] = i % 3 + (i / 5) * 63;				\
	asm volatile ("" ::: "memory");				\
      }								\
    fun_##NAME##_##CONST##_##TYPE (r, a);			\
    for (int i = 0; i < NUM_ELEMS (TYPE); i++)			\
      if (r[i] != (a[i] CMP_OP b ? a[i] : b))			\
	__builtin_abort ();					\
  }

int main ()
{
  TEST_ALL (TEST_LOOP)
  return 0;
}

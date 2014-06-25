/* { dg-do compile } */
/* { dg-require-effective-target aarch64_atomic } */
/* { dg-final { scan-assembler-times "cas\\t" 2 } } */
/* { dg-final { scan-assembler-times "casa\\t" 2 } } */
/* { dg-final { scan-assembler-times "casal\\t" 4 } } */

int v = 0;
int expected = 0;
int max = ~0;
int desired = ~0;
int zero = 0;

#define STRONG 0
#define WEAK 1

int
foo_1()
{
  return __atomic_compare_exchange_n (&v, &expected, max, STRONG,
                                      __ATOMIC_RELAXED, __ATOMIC_RELAXED);
}

int
foo_2()
{
  return __atomic_compare_exchange_n (&v, &expected, 0, STRONG,
                                      __ATOMIC_ACQUIRE, __ATOMIC_RELAXED);
}

int
foo_3()
{
  return __atomic_compare_exchange_n (&v, &expected, 0, STRONG,
                                      __ATOMIC_RELEASE, __ATOMIC_ACQUIRE);
}

int
foo_4()
{
  return __atomic_compare_exchange_n (&v, &expected, desired, STRONG,
                                      __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
}

/* Now test the generic version.  */

int
foo_5()
{
  v = 0;
  return __atomic_compare_exchange (&v, &expected, &max, STRONG,
                                    __ATOMIC_RELAXED, __ATOMIC_RELAXED);
}

int
foo_6()
{
  v = 0;
  return __atomic_compare_exchange (&v, &expected, &zero, STRONG,
                                    __ATOMIC_ACQUIRE, __ATOMIC_RELAXED);
}

int
foo_7()
{
  v = 0;
  return __atomic_compare_exchange (&v, &expected, &zero, STRONG,
                                    __ATOMIC_RELEASE, __ATOMIC_ACQUIRE);
}

int
foo_8()
{
  v = 0;
  return __atomic_compare_exchange (&v, &expected, &desired, STRONG,
                                    __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
}

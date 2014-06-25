/* { dg-do compile } */
/* { dg-require-effective-target aarch64_atomic } */
/* { dg-final { scan-assembler-times "casb\\t" 2 } } */
/* { dg-final { scan-assembler-times "casab\\t" 2 } } */
/* { dg-final { scan-assembler-times "casalb\\t" 4 } } */

char v = 0;
char expected = 0;
char max = ~0;
char desired = ~0;
char zero = 0;

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

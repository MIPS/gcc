/* { dg-do assemble } */
/* { dg-options "-O2 -ftree-vectorize -march=armv8-a+sve -ffast-math --save-temps" } */

#define TEST_LOOP(NAME, TYPE)			\
  TYPE __attribute__ ((noinline))		\
  NAME (TYPE **indices, int n)			\
  {						\
    TYPE sum = 0;				\
    for (int i = 0; i < n; ++i)			\
      sum += *indices[i];			\
    return sum;					\
  }

TEST_LOOP (f_s64, long)
TEST_LOOP (f_u64, unsigned long)
TEST_LOOP (f_f64, double)

/* { dg-final { scan-assembler-times {\tld1d\tz[0-9]+\.d, p[0-7]/z, \[z[0-9]+\.d\]} 3 } } */

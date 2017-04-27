/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize -fno-inline -march=armv8-a+sve -fno-vect-cost-model" } */

#define LOOP(TYPE)				\
  void						\
  f_##TYPE##_1 (TYPE *a, int n)			\
  {						\
    for (int i = 0; i < n; ++i)			\
      a[i] += a[i - 1];				\
  }						\
						\
  void						\
  f_##TYPE##_2 (TYPE *a, int n)			\
  {						\
    for (int i = 0; i < n; ++i)			\
      a[i] += a[i - 2];				\
  }						\
						\
  void						\
  f_##TYPE##_5 (TYPE *a, int n)			\
  {						\
    for (int i = 0; i < n; ++i)			\
      a[i] += a[i - 5];				\
  }

LOOP (char)
LOOP (short)
LOOP (float)
LOOP (double)

/* { dg-final { scan-assembler-times {\tstrb\t} 1 } } */
/* { dg-final { scan-assembler-times {\tstrh\t} 1 } } */
/* { dg-final { scan-assembler-times {\tstr\ts[0-9]+} 1 } } */
/* { dg-final { scan-assembler-times {\tstr\td[0-9]+} 1 } } */

/* { dg-final { scan-assembler-times {\tld1b\tz[0-9]+\.b, p[0-7]/z, \[x[0-9]+, x[0-9]+\]} 4 } } */
/* { dg-final { scan-assembler-times {\tld1h\tz[0-9]+\.h, p[0-7]/z, \[x[0-9]+, x[0-9]+, lsl 1\]} 4 } } */
/* { dg-final { scan-assembler-times {\tld1w\tz[0-9]+\.s, p[0-7]/z, \[x[0-9]+, x[0-9]+, lsl 2\]} 4 } } */
/* { dg-final { scan-assembler-times {\tld1d\tz[0-9]+\.d, p[0-7]/z, \[x[0-9]+, x[0-9]+, lsl 3\]} 4 } } */

/* { dg-final { scan-assembler-times {\tst1b\tz[0-9]+\.b, p[0-7], \[x[0-9]+, x[0-9]+\]} 2 } } */
/* { dg-final { scan-assembler-times {\tst1h\tz[0-9]+\.h, p[0-7], \[x[0-9]+, x[0-9]+, lsl 1\]} 2 } } */
/* { dg-final { scan-assembler-times {\tst1w\tz[0-9]+\.s, p[0-7], \[x[0-9]+, x[0-9]+, lsl 2\]} 2 } } */
/* { dg-final { scan-assembler-times {\tst1d\tz[0-9]+\.d, p[0-7], \[x[0-9]+, x[0-9]+, lsl 3\]} 2 } } */

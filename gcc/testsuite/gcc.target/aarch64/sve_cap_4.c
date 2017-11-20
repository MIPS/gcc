/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize -fno-inline -march=armv8-a+sve -fno-vect-cost-model" } */

#define LOOP(TYPE)				\
  void						\
  f_##TYPE##_1 (TYPE *a, int n)			\
  {						\
    for (int i = 0; i < n; ++i)			\
      a[i * 5] += a[i * 5 - 5];			\
  }						\
						\
  void						\
  f_##TYPE##_2 (TYPE *a, int n)			\
  {						\
    for (int i = 0; i < n; ++i)			\
      a[i * 5] += a[i * 5 - 10];		\
  }						\
						\
  void						\
  f_##TYPE##_5 (TYPE *a, int n)			\
  {						\
    for (int i = 0; i < n; ++i)			\
      a[i * 5] += a[i * 5 - 25];		\
  }

LOOP (char)
LOOP (short)
LOOP (float)
LOOP (double)

/* At the moment we can't use extending loads and truncating stores.
   Please add ld and st scan-assemblers below if that changes.  */
/* { dg-final { scan-assembler-times {\tstrb\t} 1 { xfail *-*-* } } } */
/* { dg-final { scan-assembler-times {\tstrh\t} 1 { xfail *-*-* } } } */
/* { dg-final { scan-assembler-times {\tstr\ts[0-9]+} 1 } } */
/* { dg-final { scan-assembler-times {\tstr\td[0-9]+} 1 } } */

/* { dg-final { scan-assembler-times {\tld1w\tz[0-9]+\.s, p[0-7]/z, \[x[0-9]+, z[0-9]+\.s, sxtw 2\]} 4 } } */
/* { dg-final { scan-assembler-times {\tld1d\tz[0-9]+\.d, p[0-7]/z, \[x[0-9]+, z[0-9]+\.d, lsl 3\]} 4 } } */

/* { dg-final { scan-assembler-times {\tst1w\tz[0-9]+\.s, p[0-7], \[x[0-9]+, z[0-9]+\.s, sxtw 2\]} 2 } } */
/* { dg-final { scan-assembler-times {\tst1d\tz[0-9]+\.d, p[0-7], \[x[0-9]+, z[0-9]+\.d, lsl 3\]} 2 } } */

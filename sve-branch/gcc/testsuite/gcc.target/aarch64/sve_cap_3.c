/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize -fno-inline -march=armv8-a+sve -fno-vect-cost-model" } */

#define LOOP(TYPE)				\
  void __attribute__ ((weak))			\
  f_##TYPE##_1 (TYPE *a, int n)			\
  {						\
    for (int i = 0; i < n; ++i)			\
      {						\
	a[i * 2] += a[i * 2 - 2];		\
	a[i * 2 + 1] -= a[i * 2 - 1];		\
      }						\
  }						\
						\
  void __attribute__ ((weak))			\
  f_##TYPE##_2 (TYPE *a, int n)			\
  {						\
    for (int i = 0; i < n; ++i)			\
      {						\
	a[i * 2] += a[i * 2 - 4];		\
	a[i * 2 + 1] -= a[i * 2 - 3];		\
      }						\
  }						\
						\
  void __attribute__ ((weak))			\
  f_##TYPE##_5 (TYPE *a, int n)			\
  {						\
    for (int i = 0; i < n; ++i)			\
      {						\
	a[i * 2] += a[i * 2 - 10];		\
	a[i * 2 + 1] -= a[i * 2 - 9];		\
      }						\
  }

LOOP (char)
LOOP (short)
LOOP (float)
LOOP (double)

/* { dg-final { scan-assembler-times {\tstrb\t} 2 } } */
/* { dg-final { scan-assembler-times {\tstrh\t} 2 } } */
/* { dg-final { scan-assembler-times {\tstp\ts[0-9]+} 1 } } */
/* { dg-final { scan-assembler-times {\tstp\td[0-9]+} 1 } } */

/* { dg-final { scan-assembler-times {\tld2b\t} 4 } } */
/* { dg-final { scan-assembler-times {\tld2h\t} 4 } } */
/* { dg-final { scan-assembler-times {\tld2w\t} 4 } } */
/* { dg-final { scan-assembler-times {\tld2d\t} 4 } } */

/* { dg-final { scan-assembler-times {\tst2b\t} 2 } } */
/* { dg-final { scan-assembler-times {\tst2h\t} 2 } } */
/* { dg-final { scan-assembler-times {\tst2w\t} 2 } } */
/* { dg-final { scan-assembler-times {\tst2d\t} 2 } } */

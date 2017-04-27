/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize -fopenmp-simd -march=armv8-a+sve -msve-vector-bits=256" } */

#ifndef N
#define N 32
#endif

#define TEST_LOOP(NAME, TYPE, VALUE)		\
  void						\
  NAME (TYPE *data)				\
  {						\
    _Pragma ("omp simd")			\
    for (int i = 0; i < N / sizeof (TYPE); ++i)	\
      data[i] = VALUE;				\
  }

TEST_LOOP (uc, unsigned char, 1)
TEST_LOOP (sc, signed char, 2)
TEST_LOOP (us, unsigned short, 3)
TEST_LOOP (ss, signed short, 4)
TEST_LOOP (ui, unsigned int, 5)
TEST_LOOP (si, signed int, 6)
TEST_LOOP (ul, unsigned long, 7)
TEST_LOOP (sl, signed long, 8)
TEST_LOOP (f, float, 1.0f)
TEST_LOOP (d, double, 2.0)

/* { dg-final { scan-assembler-times {\tmov\tz[0-9]+\.b, #1\n} 1 } } */
/* { dg-final { scan-assembler-times {\tmov\tz[0-9]+\.b, #2\n} 1 } } */
/* { dg-final { scan-assembler-times {\tmov\tz[0-9]+\.h, #3\n} 1 } } */
/* { dg-final { scan-assembler-times {\tmov\tz[0-9]+\.h, #4\n} 1 } } */
/* { dg-final { scan-assembler-times {\tmov\tz[0-9]+\.s, #5\n} 1 } } */
/* { dg-final { scan-assembler-times {\tmov\tz[0-9]+\.s, #6\n} 1 } } */
/* { dg-final { scan-assembler-times {\tmov\tz[0-9]+\.d, #7\n} 1 } } */
/* { dg-final { scan-assembler-times {\tmov\tz[0-9]+\.d, #8\n} 1 } } */
/* { dg-final { scan-assembler-times {\tfmov\tz[0-9]+\.s, #1\.0e\+0\n} 1 } } */
/* { dg-final { scan-assembler-times {\tfmov\tz[0-9]+\.d, #2\.0e\+0\n} 1 } } */

/* { dg-final { scan-assembler-times {\tptrue\tp[0-7]\.b, vl32\n} 2 } } */
/* { dg-final { scan-assembler-times {\tptrue\tp[0-7]\.h, vl16\n} 2 } } */
/* { dg-final { scan-assembler-times {\tptrue\tp[0-7]\.s, vl8\n} 3 } } */
/* { dg-final { scan-assembler-times {\tptrue\tp[0-7]\.d, vl4\n} 3 } } */

/* { dg-final { scan-assembler-times {\tst1b\tz[0-9]+\.b,} 2 } } */
/* { dg-final { scan-assembler-times {\tst1h\tz[0-9]+\.h,} 2 } } */
/* { dg-final { scan-assembler-times {\tst1w\tz[0-9]+\.s,} 3 } } */
/* { dg-final { scan-assembler-times {\tst1d\tz[0-9]+\.d,} 3 } } */

/* { dg-final { scan-assembler-not {\twhile} } } */
/* { dg-final { scan-assembler-not {\tb} } } */
/* { dg-final { scan-assembler-not {\tcmp} } } */
/* { dg-final { scan-assembler-not {\tindex} } } */

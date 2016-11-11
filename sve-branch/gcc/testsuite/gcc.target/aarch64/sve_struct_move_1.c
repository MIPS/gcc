/* { dg-do compile } */
/* { dg-options "-O -march=armv8-a+sve -msve-vector-bits=256" } */

typedef char v32qi __attribute__((vector_size(32)));
typedef struct { v32qi a[2]; } v64qi;

typedef short v16hi __attribute__((vector_size(32)));
typedef struct { v16hi a[2]; } v32hi;

typedef int v8si __attribute__((vector_size(32)));
typedef struct { v8si a[2]; } v16si;

typedef long v4di __attribute__((vector_size(32)));
typedef struct { v4di a[2]; } v8di;

typedef float v8sf __attribute__((vector_size(32)));
typedef struct { v8sf a[2]; } v16sf;

typedef double v4df __attribute__((vector_size(32)));
typedef struct { v4df a[2]; } v8df;

#define TEST_TYPE(TYPE, REG1, REG2)			\
  void							\
  f1_##TYPE (TYPE *a)					\
  {							\
    register TYPE x asm (#REG1) = a[0];			\
    asm volatile ("# test " #TYPE " 1 %0" :: "w" (x));	\
    register TYPE y asm (#REG2) = x;			\
    asm volatile ("# test " #TYPE " 2 %0, %1, %2"	\
		  : "=&w" (x) : "0" (x), "w" (y));	\
    a[1] = x;						\
  }							\
  /* This must compile, but we don't care how.  */	\
  void							\
  f2_##TYPE (TYPE *a)					\
  {							\
    TYPE x = a[0];					\
    x.a[0][3] = 1;					\
    x.a[1][2] = 12;					\
    asm volatile ("# %0" :: "w" (x));			\
  }							\
  void							\
  f3_##TYPE (TYPE *a, int i)				\
  {							\
    TYPE x = a[0];					\
    x.a[0][i] = 1;					\
    asm volatile ("# %0" :: "w" (x));			\
  }							\
  void							\
  f4_##TYPE (TYPE *a, int i, int j)			\
  {							\
    TYPE x = a[0];					\
    x.a[i][j] = 44;					\
    asm volatile ("# %0" :: "w" (x));			\
  }

TEST_TYPE (v64qi, z0, z2)
TEST_TYPE (v32hi, z5, z7)
TEST_TYPE (v16si, z10, z12)
TEST_TYPE (v8di, z15, z17)
TEST_TYPE (v16sf, z20, z23)
TEST_TYPE (v8df, z28, z30)

/* { dg-final { scan-assembler {\tld1b\tz0.b, p[0-7]/z, \[x0\]\n} } } */
/* { dg-final { scan-assembler {\tld1b\tz1.b, p[0-7]/z, \[x0, #1, mul vl\]\n} } } */
/* { dg-final { scan-assembler { test v64qi 1 z0\n} } } */
/* { dg-final { scan-assembler {\tmov\tz2.d, z0.d\n} } } */
/* { dg-final { scan-assembler {\tmov\tz3.d, z1.d\n} } } */
/* { dg-final { scan-assembler { test v64qi 2 z0, z0, z2\n} } } */
/* { dg-final { scan-assembler {\tst1b\tz0.b, p[0-7], \[x0, #2, mul vl\]\n} } } */
/* { dg-final { scan-assembler {\tst1b\tz1.b, p[0-7], \[x0, #3, mul vl\]\n} } } */

/* { dg-final { scan-assembler {\tld1h\tz5.h, p[0-7]/z, \[x0\]\n} } } */
/* { dg-final { scan-assembler {\tld1h\tz6.h, p[0-7]/z, \[x0, #1, mul vl\]\n} } } */
/* { dg-final { scan-assembler { test v32hi 1 z5\n} } } */
/* { dg-final { scan-assembler {\tmov\tz7.d, z5.d\n} } } */
/* { dg-final { scan-assembler {\tmov\tz8.d, z6.d\n} } } */
/* { dg-final { scan-assembler { test v32hi 2 z5, z5, z7\n} } } */
/* { dg-final { scan-assembler {\tst1h\tz5.h, p[0-7], \[x0, #2, mul vl\]\n} } } */
/* { dg-final { scan-assembler {\tst1h\tz6.h, p[0-7], \[x0, #3, mul vl\]\n} } } */

/* { dg-final { scan-assembler {\tld1w\tz10.s, p[0-7]/z, \[x0\]\n} } } */
/* { dg-final { scan-assembler {\tld1w\tz11.s, p[0-7]/z, \[x0, #1, mul vl\]\n} } } */
/* { dg-final { scan-assembler { test v16si 1 z10\n} } } */
/* { dg-final { scan-assembler {\tmov\tz12.d, z10.d\n} } } */
/* { dg-final { scan-assembler {\tmov\tz13.d, z11.d\n} } } */
/* { dg-final { scan-assembler { test v16si 2 z10, z10, z12\n} } } */
/* { dg-final { scan-assembler {\tst1w\tz10.s, p[0-7], \[x0, #2, mul vl\]\n} } } */
/* { dg-final { scan-assembler {\tst1w\tz11.s, p[0-7], \[x0, #3, mul vl\]\n} } } */

/* { dg-final { scan-assembler {\tld1d\tz15.d, p[0-7]/z, \[x0\]\n} } } */
/* { dg-final { scan-assembler {\tld1d\tz16.d, p[0-7]/z, \[x0, #1, mul vl\]\n} } } */
/* { dg-final { scan-assembler { test v8di 1 z15\n} } } */
/* { dg-final { scan-assembler {\tmov\tz17.d, z15.d\n} } } */
/* { dg-final { scan-assembler {\tmov\tz18.d, z16.d\n} } } */
/* { dg-final { scan-assembler { test v8di 2 z15, z15, z17\n} } } */
/* { dg-final { scan-assembler {\tst1d\tz15.d, p[0-7], \[x0, #2, mul vl\]\n} } } */
/* { dg-final { scan-assembler {\tst1d\tz16.d, p[0-7], \[x0, #3, mul vl\]\n} } } */

/* { dg-final { scan-assembler {\tld1w\tz20.s, p[0-7]/z, \[x0\]\n} } } */
/* { dg-final { scan-assembler {\tld1w\tz21.s, p[0-7]/z, \[x0, #1, mul vl\]\n} } } */
/* { dg-final { scan-assembler { test v16sf 1 z20\n} } } */
/* { dg-final { scan-assembler {\tmov\tz23.d, z20.d\n} } } */
/* { dg-final { scan-assembler {\tmov\tz24.d, z21.d\n} } } */
/* { dg-final { scan-assembler { test v16sf 2 z20, z20, z23\n} } } */
/* { dg-final { scan-assembler {\tst1w\tz20.s, p[0-7], \[x0, #2, mul vl\]\n} } } */
/* { dg-final { scan-assembler {\tst1w\tz21.s, p[0-7], \[x0, #3, mul vl\]\n} } } */

/* { dg-final { scan-assembler {\tld1d\tz28.d, p[0-7]/z, \[x0\]\n} } } */
/* { dg-final { scan-assembler {\tld1d\tz29.d, p[0-7]/z, \[x0, #1, mul vl\]\n} } } */
/* { dg-final { scan-assembler { test v8df 1 z28\n} } } */
/* { dg-final { scan-assembler {\tmov\tz30.d, z28.d\n} } } */
/* { dg-final { scan-assembler {\tmov\tz31.d, z29.d\n} } } */
/* { dg-final { scan-assembler { test v8df 2 z28, z28, z30\n} } } */
/* { dg-final { scan-assembler {\tst1d\tz28.d, p[0-7], \[x0, #2, mul vl\]\n} } } */
/* { dg-final { scan-assembler {\tst1d\tz29.d, p[0-7], \[x0, #3, mul vl\]\n} } } */

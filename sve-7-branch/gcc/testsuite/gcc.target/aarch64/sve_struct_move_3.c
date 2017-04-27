/* { dg-do compile } */
/* { dg-options "-O -march=armv8-a+sve -msve-vector-bits=256" } */

typedef char v32qi __attribute__((vector_size(32)));
typedef struct { v32qi a[4]; } v128qi;

typedef short v16hi __attribute__((vector_size(32)));
typedef struct { v16hi a[4]; } v64hi;

typedef int v8si __attribute__((vector_size(32)));
typedef struct { v8si a[4]; } v32si;

typedef long v4di __attribute__((vector_size(32)));
typedef struct { v4di a[4]; } v16di;

typedef float v8sf __attribute__((vector_size(32)));
typedef struct { v8sf a[4]; } v32sf;

typedef double v4df __attribute__((vector_size(32)));
typedef struct { v4df a[4]; } v16df;

#define TEST_TYPE(TYPE, REG1, REG2) \
  void \
  f_##TYPE (TYPE *a) \
  { \
    register TYPE x asm (#REG1) = a[0]; \
    asm volatile ("# test " #TYPE " 1 %0" :: "w" (x)); \
    register TYPE y asm (#REG2) = x; \
    asm volatile ("# test " #TYPE " 2 %0, %1, %2" \
		  : "=&w" (x) : "0" (x), "w" (y)); \
    a[1] = x; \
  }

TEST_TYPE (v128qi, z0, z4)
TEST_TYPE (v64hi, z6, z2)
TEST_TYPE (v32si, z12, z16)
TEST_TYPE (v16di, z17, z13)
TEST_TYPE (v32sf, z20, z16)
TEST_TYPE (v16df, z24, z28)

/* { dg-final { scan-assembler {\tld1b\tz0.b, p[0-7]/z, \[x0\]\n} } } */
/* { dg-final { scan-assembler {\tld1b\tz1.b, p[0-7]/z, \[x0, #1, mul vl\]\n} } } */
/* { dg-final { scan-assembler {\tld1b\tz2.b, p[0-7]/z, \[x0, #2, mul vl\]\n} } } */
/* { dg-final { scan-assembler {\tld1b\tz3.b, p[0-7]/z, \[x0, #3, mul vl\]\n} } } */
/* { dg-final { scan-assembler { test v128qi 1 z0\n} } } */
/* { dg-final { scan-assembler {\tmov\tz4.d, z0.d\n} } } */
/* { dg-final { scan-assembler {\tmov\tz5.d, z1.d\n} } } */
/* { dg-final { scan-assembler {\tmov\tz6.d, z2.d\n} } } */
/* { dg-final { scan-assembler {\tmov\tz7.d, z3.d\n} } } */
/* { dg-final { scan-assembler { test v128qi 2 z0, z0, z4\n} } } */
/* { dg-final { scan-assembler {\tst1b\tz0.b, p[0-7], \[x0, #4, mul vl\]\n} } } */
/* { dg-final { scan-assembler {\tst1b\tz1.b, p[0-7], \[x0, #5, mul vl\]\n} } } */
/* { dg-final { scan-assembler {\tst1b\tz2.b, p[0-7], \[x0, #6, mul vl\]\n} } } */
/* { dg-final { scan-assembler {\tst1b\tz3.b, p[0-7], \[x0, #7, mul vl\]\n} } } */

/* { dg-final { scan-assembler {\tld1h\tz6.h, p[0-7]/z, \[x0\]\n} } } */
/* { dg-final { scan-assembler {\tld1h\tz7.h, p[0-7]/z, \[x0, #1, mul vl\]\n} } } */
/* { dg-final { scan-assembler {\tld1h\tz8.h, p[0-7]/z, \[x0, #2, mul vl\]\n} } } */
/* { dg-final { scan-assembler {\tld1h\tz9.h, p[0-7]/z, \[x0, #3, mul vl\]\n} } } */
/* { dg-final { scan-assembler { test v64hi 1 z6\n} } } */
/* { dg-final { scan-assembler {\tmov\tz2.d, z6.d\n} } } */
/* { dg-final { scan-assembler {\tmov\tz3.d, z7.d\n} } } */
/* { dg-final { scan-assembler {\tmov\tz4.d, z8.d\n} } } */
/* { dg-final { scan-assembler {\tmov\tz5.d, z9.d\n} } } */
/* { dg-final { scan-assembler { test v64hi 2 z6, z6, z2\n} } } */
/* { dg-final { scan-assembler {\tst1h\tz6.h, p[0-7], \[x0, #4, mul vl\]\n} } } */
/* { dg-final { scan-assembler {\tst1h\tz7.h, p[0-7], \[x0, #5, mul vl\]\n} } } */
/* { dg-final { scan-assembler {\tst1h\tz8.h, p[0-7], \[x0, #6, mul vl\]\n} } } */
/* { dg-final { scan-assembler {\tst1h\tz9.h, p[0-7], \[x0, #7, mul vl\]\n} } } */

/* { dg-final { scan-assembler {\tld1w\tz12.s, p[0-7]/z, \[x0\]\n} } } */
/* { dg-final { scan-assembler {\tld1w\tz13.s, p[0-7]/z, \[x0, #1, mul vl\]\n} } } */
/* { dg-final { scan-assembler {\tld1w\tz14.s, p[0-7]/z, \[x0, #2, mul vl\]\n} } } */
/* { dg-final { scan-assembler {\tld1w\tz15.s, p[0-7]/z, \[x0, #3, mul vl\]\n} } } */
/* { dg-final { scan-assembler { test v32si 1 z12\n} } } */
/* { dg-final { scan-assembler {\tmov\tz16.d, z12.d\n} } } */
/* { dg-final { scan-assembler {\tmov\tz17.d, z13.d\n} } } */
/* { dg-final { scan-assembler {\tmov\tz18.d, z14.d\n} } } */
/* { dg-final { scan-assembler {\tmov\tz19.d, z15.d\n} } } */
/* { dg-final { scan-assembler { test v32si 2 z12, z12, z16\n} } } */
/* { dg-final { scan-assembler {\tst1w\tz12.s, p[0-7], \[x0, #4, mul vl\]\n} } } */
/* { dg-final { scan-assembler {\tst1w\tz13.s, p[0-7], \[x0, #5, mul vl\]\n} } } */
/* { dg-final { scan-assembler {\tst1w\tz14.s, p[0-7], \[x0, #6, mul vl\]\n} } } */
/* { dg-final { scan-assembler {\tst1w\tz15.s, p[0-7], \[x0, #7, mul vl\]\n} } } */

/* { dg-final { scan-assembler {\tld1d\tz17.d, p[0-7]/z, \[x0\]\n} } } */
/* { dg-final { scan-assembler {\tld1d\tz18.d, p[0-7]/z, \[x0, #1, mul vl\]\n} } } */
/* { dg-final { scan-assembler {\tld1d\tz19.d, p[0-7]/z, \[x0, #2, mul vl\]\n} } } */
/* { dg-final { scan-assembler {\tld1d\tz20.d, p[0-7]/z, \[x0, #3, mul vl\]\n} } } */
/* { dg-final { scan-assembler { test v16di 1 z17\n} } } */
/* { dg-final { scan-assembler {\tmov\tz13.d, z17.d\n} } } */
/* { dg-final { scan-assembler {\tmov\tz14.d, z18.d\n} } } */
/* { dg-final { scan-assembler {\tmov\tz15.d, z19.d\n} } } */
/* { dg-final { scan-assembler {\tmov\tz16.d, z20.d\n} } } */
/* { dg-final { scan-assembler { test v16di 2 z17, z17, z13\n} } } */
/* { dg-final { scan-assembler {\tst1d\tz17.d, p[0-7], \[x0, #4, mul vl\]\n} } } */
/* { dg-final { scan-assembler {\tst1d\tz18.d, p[0-7], \[x0, #5, mul vl\]\n} } } */
/* { dg-final { scan-assembler {\tst1d\tz19.d, p[0-7], \[x0, #6, mul vl\]\n} } } */
/* { dg-final { scan-assembler {\tst1d\tz20.d, p[0-7], \[x0, #7, mul vl\]\n} } } */

/* { dg-final { scan-assembler {\tld1w\tz20.s, p[0-7]/z, \[x0\]\n} } } */
/* { dg-final { scan-assembler {\tld1w\tz21.s, p[0-7]/z, \[x0, #1, mul vl\]\n} } } */
/* { dg-final { scan-assembler {\tld1w\tz22.s, p[0-7]/z, \[x0, #2, mul vl\]\n} } } */
/* { dg-final { scan-assembler {\tld1w\tz23.s, p[0-7]/z, \[x0, #3, mul vl\]\n} } } */
/* { dg-final { scan-assembler { test v32sf 1 z20\n} } } */
/* { dg-final { scan-assembler {\tmov\tz16.d, z20.d\n} } } */
/* { dg-final { scan-assembler {\tmov\tz17.d, z21.d\n} } } */
/* { dg-final { scan-assembler {\tmov\tz18.d, z22.d\n} } } */
/* { dg-final { scan-assembler {\tmov\tz19.d, z23.d\n} } } */
/* { dg-final { scan-assembler { test v32sf 2 z20, z20, z16\n} } } */
/* { dg-final { scan-assembler {\tst1w\tz20.s, p[0-7], \[x0, #4, mul vl\]\n} } } */
/* { dg-final { scan-assembler {\tst1w\tz21.s, p[0-7], \[x0, #5, mul vl\]\n} } } */
/* { dg-final { scan-assembler {\tst1w\tz22.s, p[0-7], \[x0, #6, mul vl\]\n} } } */
/* { dg-final { scan-assembler {\tst1w\tz23.s, p[0-7], \[x0, #7, mul vl\]\n} } } */

/* { dg-final { scan-assembler {\tld1d\tz24.d, p[0-7]/z, \[x0\]\n} } } */
/* { dg-final { scan-assembler {\tld1d\tz25.d, p[0-7]/z, \[x0, #1, mul vl\]\n} } } */
/* { dg-final { scan-assembler {\tld1d\tz26.d, p[0-7]/z, \[x0, #2, mul vl\]\n} } } */
/* { dg-final { scan-assembler {\tld1d\tz27.d, p[0-7]/z, \[x0, #3, mul vl\]\n} } } */
/* { dg-final { scan-assembler { test v16df 1 z24\n} } } */
/* { dg-final { scan-assembler {\tmov\tz28.d, z24.d\n} } } */
/* { dg-final { scan-assembler {\tmov\tz29.d, z25.d\n} } } */
/* { dg-final { scan-assembler {\tmov\tz30.d, z26.d\n} } } */
/* { dg-final { scan-assembler {\tmov\tz31.d, z27.d\n} } } */
/* { dg-final { scan-assembler { test v16df 2 z24, z24, z28\n} } } */
/* { dg-final { scan-assembler {\tst1d\tz24.d, p[0-7], \[x0, #4, mul vl\]\n} } } */
/* { dg-final { scan-assembler {\tst1d\tz25.d, p[0-7], \[x0, #5, mul vl\]\n} } } */
/* { dg-final { scan-assembler {\tst1d\tz26.d, p[0-7], \[x0, #6, mul vl\]\n} } } */
/* { dg-final { scan-assembler {\tst1d\tz27.d, p[0-7], \[x0, #7, mul vl\]\n} } } */

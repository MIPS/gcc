/* { dg-do compile } */
/* FIXME: With -O3 we don't generate reductions as the compiler unrolls the outer loop
   and processes the rows in parallel, performing in order reductions on the inner loop.  */
/* { dg-options "-std=c++11 -O2 -ftree-vectorize -fno-inline -march=armv8-a+sve" } */

#include <math.h>

#define NUM_ELEMS(TYPE) (int)(5 * (256 / sizeof (TYPE)) + 3)

/* TODO: Test with inner loop = n * NUM_ELEMS(TYPE).  */
#define DEF_REDUC_PLUS(TYPE)\
void reduc_plus_##TYPE (TYPE (*__restrict__ a)[NUM_ELEMS(TYPE)], TYPE *__restrict__ r, int n)\
{\
  for (int i = 0; i < n; i++)\
    {\
      r[i] = 0;\
      for (int j = 0; j < NUM_ELEMS(TYPE); j++)\
        r[i] += a[i][j];\
    }\
}\

DEF_REDUC_PLUS (float)
DEF_REDUC_PLUS (double)

#define DEF_REDUC_MAXMIN(TYPE,FUN)\
void reduc_##FUN (TYPE (*__restrict__ a)[NUM_ELEMS(TYPE)], TYPE *__restrict__ r, int n)\
{\
  for (int i = 0; i < n; i++)\
    {\
      r[i] = a[i][0];\
      for (int j = 0; j < NUM_ELEMS(TYPE); j++)\
        r[i] = FUN (a[i][j], r[i]);\
    }\
}\

DEF_REDUC_MAXMIN (float,  fmaxf)
DEF_REDUC_MAXMIN (double, fmax)
DEF_REDUC_MAXMIN (float,  fminf)
DEF_REDUC_MAXMIN (double, fmin)

/* { dg-final { scan-assembler-times {\tfadda\ts[0-9]+, p[0-7], s[0-9]+, z[0-9]+\.s} 1 } } */
/* { dg-final { scan-assembler-times {\tfadda\td[0-9]+, p[0-7], d[0-9]+, z[0-9]+\.d} 1 } } */

/* { dg-final { scan-assembler-times {\tfmaxnmv\ts[0-9]+, p[0-7], z[0-9]+\.s} 1 { xfail *-*-* } } } */
/* { dg-final { scan-assembler-times {\tfmaxnmv\td[0-9]+, p[0-7], z[0-9]+\.d} 1 { xfail *-*-* } } } */
/* { dg-final { scan-assembler-times {\tfminnmv\ts[0-9]+, p[0-7], z[0-9]+\.s} 1 { xfail *-*-* } } } */
/* { dg-final { scan-assembler-times {\tfminnmv\td[0-9]+, p[0-7], z[0-9]+\.d} 1 { xfail *-*-* } } } */


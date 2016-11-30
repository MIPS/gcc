/* { dg-do compile } */
/* { dg-options "-std=c++11 -O2 -ftree-vectorize -fno-inline -march=armv8-a+sve" } */

#include <math.h>

#define NUM_ELEMS(TYPE) (int)(5 * (256 / sizeof (TYPE)) + 3)

#define DEF_REDUC_PLUS(TYPE)\
TYPE reduc_plus_##TYPE (TYPE *__restrict__ a, TYPE *__restrict__ b)\
{\
  TYPE r = 0, q = 3;\
  for (int i = 0; i < NUM_ELEMS(TYPE); i++)\
    {\
      r += a[i];\
      q -= b[i];\
    }\
  return r * q;\
}\

DEF_REDUC_PLUS (float)
DEF_REDUC_PLUS (double)

#define DEF_REDUC_MAXMIN(TYPE,FUN)\
TYPE reduc_##FUN (TYPE *__restrict__ a, TYPE *__restrict__ b)\
{\
  TYPE r = a[0], q = b[0];\
  for (int i = 0; i < NUM_ELEMS(TYPE); i++)\
    {\
      r = FUN (a[i], r);\
      q = FUN (b[i], q);\
    }\
  return r * q;\
}\

DEF_REDUC_MAXMIN (float,  fmaxf)
DEF_REDUC_MAXMIN (double, fmax)
DEF_REDUC_MAXMIN (float,  fminf)
DEF_REDUC_MAXMIN (double, fmin)


/* { dg-final { scan-assembler-times {\tfadda\ts[0-9]+, p[0-7], s[0-9]+, z[0-9]+\.s} 2 } } */
/* { dg-final { scan-assembler-times {\tfadda\td[0-9]+, p[0-7], d[0-9]+, z[0-9]+\.d} 2 } } */

/* { dg-final { scan-assembler-times {\tfmaxnmv\ts[0-9]+, p[0-7], z[0-9]+\.s} 2 { xfail *-*-* } } } */
/* { dg-final { scan-assembler-times {\tfmaxnmv\td[0-9]+, p[0-7], z[0-9]+\.d} 2 { xfail *-*-* } } } */
/* { dg-final { scan-assembler-times {\tfminnmv\ts[0-9]+, p[0-7], z[0-9]+\.s} 2 { xfail *-*-* } } } */
/* { dg-final { scan-assembler-times {\tfminnmv\td[0-9]+, p[0-7], z[0-9]+\.d} 2 { xfail *-*-* } } } */


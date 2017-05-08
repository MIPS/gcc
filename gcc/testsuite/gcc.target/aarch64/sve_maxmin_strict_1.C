/* { dg-do compile } */
/* { dg-options "-std=c++11 -O2 -ftree-vectorize -fno-inline -march=armv8-a+sve" } */

#include <math.h>

#define NUM_ELEMS(TYPE) (320 / sizeof (TYPE))

#define DEF_MAXMIN(TYPE,FUN)					\
void test_##FUN (TYPE *__restrict__ r, TYPE *__restrict__ a,	\
                 TYPE *__restrict__ b)				\
{								\
  for (int i = 0; i < NUM_ELEMS (TYPE); i++)			\
    r[i] = FUN (a[i], b[i]);					\
}

DEF_MAXMIN (float, fmaxf)
DEF_MAXMIN (double, fmax)

DEF_MAXMIN (float, fminf)
DEF_MAXMIN (double, fmin)

/* { dg-final { scan-assembler-times {\tfmaxnm\tz[0-9]+\.s, p[0-7]/m, z[0-9]+\.s, z[0-9]+\.s\n} 1 } } */
/* { dg-final { scan-assembler-times {\tfmaxnm\tz[0-9]+\.d, p[0-7]/m, z[0-9]+\.d, z[0-9]+\.d\n} 1 } } */

/* { dg-final { scan-assembler-times {\tfminnm\tz[0-9]+\.s, p[0-7]/m, z[0-9]+\.s, z[0-9]+\.s\n} 1 } } */
/* { dg-final { scan-assembler-times {\tfminnm\tz[0-9]+\.d, p[0-7]/m, z[0-9]+\.d, z[0-9]+\.d\n} 1 } } */

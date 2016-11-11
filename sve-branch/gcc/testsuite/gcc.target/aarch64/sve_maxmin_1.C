/* { dg-do compile } */
/* { dg-options "-std=c++11 -O2 -ftree-vectorize -ffast-math -fno-inline -march=armv8-a+sve" } */

#include <stdint.h>

#define NUM_ELEMS(TYPE) (320 / sizeof (TYPE))

#define DEF_MAXMIN(TYPE,NAME,CMP_OP)					\
void fun_##NAME##TYPE (TYPE *__restrict__ r, TYPE *__restrict__ a,	\
                       TYPE *__restrict__ b)				\
{									\
  for (int i = 0; i < NUM_ELEMS (TYPE); i++)				\
    r[i] = a[i] CMP_OP b[i] ? a[i] : b[i];				\
}

DEF_MAXMIN (int8_t, max, >)
DEF_MAXMIN (int16_t, max, >)
DEF_MAXMIN (int32_t, max, >)
DEF_MAXMIN (int64_t, max, >)
DEF_MAXMIN (uint8_t, max, >)
DEF_MAXMIN (uint16_t, max, >)
DEF_MAXMIN (uint32_t, max, >)
DEF_MAXMIN (uint64_t, max, >)
DEF_MAXMIN (float, max, >)
DEF_MAXMIN (double, max, >)

DEF_MAXMIN (int8_t, min, <)
DEF_MAXMIN (int16_t, min, <)
DEF_MAXMIN (int32_t, min, <)
DEF_MAXMIN (int64_t, min, <)
DEF_MAXMIN (uint8_t, min, <)
DEF_MAXMIN (uint16_t, min, <)
DEF_MAXMIN (uint32_t, min, <)
DEF_MAXMIN (uint64_t, min, <)
DEF_MAXMIN (float, min, <)
DEF_MAXMIN (double, min, <)

/* { dg-final { scan-assembler-times {\tsmax\tz[0-9]+\.b, p[0-7]/m, z[0-9]+\.b, z[0-9]+\.b\n} 1 } } */
/* { dg-final { scan-assembler-times {\tsmax\tz[0-9]+\.h, p[0-7]/m, z[0-9]+\.h, z[0-9]+\.h\n} 1 } } */
/* { dg-final { scan-assembler-times {\tsmax\tz[0-9]+\.s, p[0-7]/m, z[0-9]+\.s, z[0-9]+\.s\n} 1 } } */
/* { dg-final { scan-assembler-times {\tsmax\tz[0-9]+\.d, p[0-7]/m, z[0-9]+\.d, z[0-9]+\.d\n} 1 } } */

/* { dg-final { scan-assembler-times {\tumax\tz[0-9]+\.b, p[0-7]/m, z[0-9]+\.b, z[0-9]+\.b\n} 1 } } */
/* { dg-final { scan-assembler-times {\tumax\tz[0-9]+\.h, p[0-7]/m, z[0-9]+\.h, z[0-9]+\.h\n} 1 } } */
/* { dg-final { scan-assembler-times {\tumax\tz[0-9]+\.s, p[0-7]/m, z[0-9]+\.s, z[0-9]+\.s\n} 1 } } */
/* { dg-final { scan-assembler-times {\tumax\tz[0-9]+\.d, p[0-7]/m, z[0-9]+\.d, z[0-9]+\.d\n} 1 } } */

/* { dg-final { scan-assembler-times {\tfmaxnm\tz[0-9]+\.s, p[0-7]/m, z[0-9]+\.s, z[0-9]+\.s\n} 1 } } */
/* { dg-final { scan-assembler-times {\tfmaxnm\tz[0-9]+\.d, p[0-7]/m, z[0-9]+\.d, z[0-9]+\.d\n} 1 } } */

/* { dg-final { scan-assembler-times {\tsmin\tz[0-9]+\.b, p[0-7]/m, z[0-9]+\.b, z[0-9]+\.b\n} 1 } } */
/* { dg-final { scan-assembler-times {\tsmin\tz[0-9]+\.h, p[0-7]/m, z[0-9]+\.h, z[0-9]+\.h\n} 1 } } */
/* { dg-final { scan-assembler-times {\tsmin\tz[0-9]+\.s, p[0-7]/m, z[0-9]+\.s, z[0-9]+\.s\n} 1 } } */
/* { dg-final { scan-assembler-times {\tsmin\tz[0-9]+\.d, p[0-7]/m, z[0-9]+\.d, z[0-9]+\.d\n} 1 } } */

/* { dg-final { scan-assembler-times {\tumin\tz[0-9]+\.b, p[0-7]/m, z[0-9]+\.b, z[0-9]+\.b\n} 1 } } */
/* { dg-final { scan-assembler-times {\tumin\tz[0-9]+\.h, p[0-7]/m, z[0-9]+\.h, z[0-9]+\.h\n} 1 } } */
/* { dg-final { scan-assembler-times {\tumin\tz[0-9]+\.s, p[0-7]/m, z[0-9]+\.s, z[0-9]+\.s\n} 1 } } */
/* { dg-final { scan-assembler-times {\tumin\tz[0-9]+\.d, p[0-7]/m, z[0-9]+\.d, z[0-9]+\.d\n} 1 } } */

/* { dg-final { scan-assembler-times {\tfminnm\tz[0-9]+\.s, p[0-7]/m, z[0-9]+\.s, z[0-9]+\.s\n} 1 } } */
/* { dg-final { scan-assembler-times {\tfminnm\tz[0-9]+\.d, p[0-7]/m, z[0-9]+\.d, z[0-9]+\.d\n} 1 } } */

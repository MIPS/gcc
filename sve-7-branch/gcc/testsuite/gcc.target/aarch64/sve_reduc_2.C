/* { dg-do compile } */
/* { dg-options "-std=c++11 -O2 -ftree-vectorize -ffast-math -fno-inline -march=armv8-a+sve" } */

#include <stdint.h>

#define NUM_ELEMS(TYPE) (1024 / sizeof (TYPE))

#define DEF_REDUC_PLUS(TYPE)						\
void reduc_plus_##TYPE (TYPE (*__restrict__ a)[NUM_ELEMS (TYPE)],	\
			TYPE *__restrict__ r, int n)			\
{									\
  for (int i = 0; i < n; i++)						\
    {									\
      r[i] = 0;								\
      for (int j = 0; j < NUM_ELEMS (TYPE); j++)			\
        r[i] += a[i][j];						\
    }									\
}

DEF_REDUC_PLUS (int8_t)
DEF_REDUC_PLUS (int16_t)
DEF_REDUC_PLUS (int32_t)
DEF_REDUC_PLUS (int64_t)
DEF_REDUC_PLUS (uint8_t)
DEF_REDUC_PLUS (uint16_t)
DEF_REDUC_PLUS (uint32_t)
DEF_REDUC_PLUS (uint64_t)
DEF_REDUC_PLUS (float)
DEF_REDUC_PLUS (double)

#define DEF_REDUC_MAXMIN(TYPE,NAME,CMP_OP)				\
void reduc_##NAME##TYPE (TYPE (*__restrict__ a)[NUM_ELEMS (TYPE)],	\
			 TYPE *__restrict__ r, int n)			\
{									\
  for (int i = 0; i < n; i++)						\
    {									\
      r[i] = a[i][0];							\
      for (int j = 0; j < NUM_ELEMS (TYPE); j++)			\
        r[i] = a[i][j] CMP_OP r[i] ? a[i][j] : r[i];			\
    }									\
}

DEF_REDUC_MAXMIN (int8_t, max, >)
DEF_REDUC_MAXMIN (int16_t, max, >)
DEF_REDUC_MAXMIN (int32_t, max, >)
DEF_REDUC_MAXMIN (int64_t, max, >)
DEF_REDUC_MAXMIN (uint8_t, max, >)
DEF_REDUC_MAXMIN (uint16_t, max, >)
DEF_REDUC_MAXMIN (uint32_t, max, >)
DEF_REDUC_MAXMIN (uint64_t, max, >)
DEF_REDUC_MAXMIN (float, max, >)
DEF_REDUC_MAXMIN (double, max, >)

DEF_REDUC_MAXMIN (int8_t, min, <)
DEF_REDUC_MAXMIN (int16_t, min, <)
DEF_REDUC_MAXMIN (int32_t, min, <)
DEF_REDUC_MAXMIN (int64_t, min, <)
DEF_REDUC_MAXMIN (uint8_t, min, <)
DEF_REDUC_MAXMIN (uint16_t, min, <)
DEF_REDUC_MAXMIN (uint32_t, min, <)
DEF_REDUC_MAXMIN (uint64_t, min, <)
DEF_REDUC_MAXMIN (float, min, <)
DEF_REDUC_MAXMIN (double, min, <)

#define DEF_REDUC_BITWISE(TYPE,NAME,BIT_OP)\
void reduc_##NAME##TYPE (TYPE (*__restrict__ a)[NUM_ELEMS(TYPE)], TYPE *__restrict__ r, int n)\
{\
  for (int i = 0; i < n; i++)\
    {\
      r[i] = a[i][0];\
      for (int j = 0; j < NUM_ELEMS(TYPE); j++)\
        r[i] BIT_OP a[i][j];\
    }\
}\

DEF_REDUC_BITWISE (int8_t, and, &=)
DEF_REDUC_BITWISE (int16_t, and, &=)
DEF_REDUC_BITWISE (int32_t, and, &=)
DEF_REDUC_BITWISE (int64_t, and, &=)
DEF_REDUC_BITWISE (uint8_t, and, &=)
DEF_REDUC_BITWISE (uint16_t, and, &=)
DEF_REDUC_BITWISE (uint32_t, and, &=)
DEF_REDUC_BITWISE (uint64_t, and, &=)

DEF_REDUC_BITWISE (int8_t, ior, |=)
DEF_REDUC_BITWISE (int16_t, ior, |=)
DEF_REDUC_BITWISE (int32_t, ior, |=)
DEF_REDUC_BITWISE (int64_t, ior, |=)
DEF_REDUC_BITWISE (uint8_t, ior, |=)
DEF_REDUC_BITWISE (uint16_t, ior, |=)
DEF_REDUC_BITWISE (uint32_t, ior, |=)
DEF_REDUC_BITWISE (uint64_t, ior, |=)

DEF_REDUC_BITWISE (int8_t, xor, ^=)
DEF_REDUC_BITWISE (int16_t, xor, ^=)
DEF_REDUC_BITWISE (int32_t, xor, ^=)
DEF_REDUC_BITWISE (int64_t, xor, ^=)
DEF_REDUC_BITWISE (uint8_t, xor, ^=)
DEF_REDUC_BITWISE (uint16_t, xor, ^=)
DEF_REDUC_BITWISE (uint32_t, xor, ^=)
DEF_REDUC_BITWISE (uint64_t, xor, ^=)

/* { dg-final { scan-assembler-times {\tuaddv\td[0-9]+, p[0-7], z[0-9]+\.b\n} 1 } } */
/* { dg-final { scan-assembler-times {\tuaddv\td[0-9]+, p[0-7], z[0-9]+\.h\n} 1 } } */
/* { dg-final { scan-assembler-times {\tuaddv\td[0-9]+, p[0-7], z[0-9]+\.s\n} 2 } } */
/* { dg-final { scan-assembler-times {\tuaddv\td[0-9]+, p[0-7], z[0-9]+\.d\n} 2 } } */
/* { dg-final { scan-assembler-times {\tfaddv\ts[0-9]+, p[0-7], z[0-9]+\.s\n} 1 } } */
/* { dg-final { scan-assembler-times {\tfaddv\td[0-9]+, p[0-7], z[0-9]+\.d\n} 1 } } */

/* { dg-final { scan-assembler-times {\tsmaxv\tb[0-9]+, p[0-7], z[0-9]+\.b\n} 1 } } */
/* { dg-final { scan-assembler-times {\tsmaxv\th[0-9]+, p[0-7], z[0-9]+\.h\n} 1 } } */
/* { dg-final { scan-assembler-times {\tsmaxv\ts[0-9]+, p[0-7], z[0-9]+\.s\n} 1 } } */
/* { dg-final { scan-assembler-times {\tsmaxv\td[0-9]+, p[0-7], z[0-9]+\.d\n} 1 } } */
/* { dg-final { scan-assembler-times {\tumaxv\tb[0-9]+, p[0-7], z[0-9]+\.b\n} 1 } } */
/* { dg-final { scan-assembler-times {\tumaxv\th[0-9]+, p[0-7], z[0-9]+\.h\n} 1 } } */
/* { dg-final { scan-assembler-times {\tumaxv\ts[0-9]+, p[0-7], z[0-9]+\.s\n} 1 } } */
/* { dg-final { scan-assembler-times {\tumaxv\td[0-9]+, p[0-7], z[0-9]+\.d\n} 1 } } */
/* { dg-final { scan-assembler-times {\tfmaxnmv\ts[0-9]+, p[0-7], z[0-9]+\.s\n} 1 } } */
/* { dg-final { scan-assembler-times {\tfmaxnmv\td[0-9]+, p[0-7], z[0-9]+\.d\n} 1 } } */

/* { dg-final { scan-assembler-times {\tsminv\tb[0-9]+, p[0-7], z[0-9]+\.b\n} 1 } } */
/* { dg-final { scan-assembler-times {\tsminv\th[0-9]+, p[0-7], z[0-9]+\.h\n} 1 } } */
/* { dg-final { scan-assembler-times {\tsminv\ts[0-9]+, p[0-7], z[0-9]+\.s\n} 1 } } */
/* { dg-final { scan-assembler-times {\tsminv\td[0-9]+, p[0-7], z[0-9]+\.d\n} 1 } } */
/* { dg-final { scan-assembler-times {\tuminv\tb[0-9]+, p[0-7], z[0-9]+\.b\n} 1 } } */
/* { dg-final { scan-assembler-times {\tuminv\th[0-9]+, p[0-7], z[0-9]+\.h\n} 1 } } */
/* { dg-final { scan-assembler-times {\tuminv\ts[0-9]+, p[0-7], z[0-9]+\.s\n} 1 } } */
/* { dg-final { scan-assembler-times {\tuminv\td[0-9]+, p[0-7], z[0-9]+\.d\n} 1 } } */
/* { dg-final { scan-assembler-times {\tfminnmv\ts[0-9]+, p[0-7], z[0-9]+\.s\n} 1 } } */
/* { dg-final { scan-assembler-times {\tfminnmv\td[0-9]+, p[0-7], z[0-9]+\.d\n} 1 } } */

/* { dg-final { scan-assembler-times {\tandv\tb[0-9]+, p[0-7], z[0-9]+\.b\n} 2 } } */
/* { dg-final { scan-assembler-times {\tandv\th[0-9]+, p[0-7], z[0-9]+\.h\n} 2 } } */
/* { dg-final { scan-assembler-times {\tandv\ts[0-9]+, p[0-7], z[0-9]+\.s\n} 2 } } */
/* { dg-final { scan-assembler-times {\tandv\td[0-9]+, p[0-7], z[0-9]+\.d\n} 2 } } */

/* { dg-final { scan-assembler-times {\torv\tb[0-9]+, p[0-7], z[0-9]+\.b\n} 2 } } */
/* { dg-final { scan-assembler-times {\torv\th[0-9]+, p[0-7], z[0-9]+\.h\n} 2 } } */
/* { dg-final { scan-assembler-times {\torv\ts[0-9]+, p[0-7], z[0-9]+\.s\n} 2 } } */
/* { dg-final { scan-assembler-times {\torv\td[0-9]+, p[0-7], z[0-9]+\.d\n} 2 } } */

/* { dg-final { scan-assembler-times {\teorv\tb[0-9]+, p[0-7], z[0-9]+\.b\n} 2 } } */
/* { dg-final { scan-assembler-times {\teorv\th[0-9]+, p[0-7], z[0-9]+\.h\n} 2 } } */
/* { dg-final { scan-assembler-times {\teorv\ts[0-9]+, p[0-7], z[0-9]+\.s\n} 2 } } */
/* { dg-final { scan-assembler-times {\teorv\td[0-9]+, p[0-7], z[0-9]+\.d\n} 2 } } */

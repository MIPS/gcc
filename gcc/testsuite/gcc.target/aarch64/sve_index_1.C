/* { dg-do compile } */
/* { dg-options "-std=c++11 -O2 -ftree-vectorize -fno-inline -march=armv8-a+sve -msve-vector-bits=256" } */

#include <stdint.h>

#define NUM_ELEMS(TYPE) (32 / sizeof (TYPE))

#define DEF_LOOP(TYPE,BASE,STEP,SUFFIX)		\
void loop_##TYPE##SUFFIX (TYPE *__restrict__ a)	\
{						\
  for (TYPE i = 0; i < NUM_ELEMS (TYPE); ++i)	\
    a[i] = TYPE (BASE) + TYPE (i * (STEP));	\
}

#define DEF_LOOPS_ALL_UNSIGNED_TYPES(BASE,STEP,SUFFIX)	\
DEF_LOOP (uint8_t,  BASE, STEP, SUFFIX)			\
DEF_LOOP (uint16_t, BASE, STEP, SUFFIX)			\
DEF_LOOP (uint32_t, BASE, STEP, SUFFIX)			\
DEF_LOOP (uint64_t, BASE, STEP, SUFFIX)

#define DEF_LOOPS_ALL_SIGNED_TYPES(BASE,STEP,SUFFIX)	\
DEF_LOOP (int8_t,  BASE, STEP, SUFFIX)			\
DEF_LOOP (int16_t, BASE, STEP, SUFFIX)			\
DEF_LOOP (int32_t, BASE, STEP, SUFFIX)			\
DEF_LOOP (int64_t, BASE, STEP, SUFFIX)

/* Immediate Loops.  */
DEF_LOOPS_ALL_UNSIGNED_TYPES (0, 1, b0s1)
DEF_LOOPS_ALL_SIGNED_TYPES (0, 1, b0s1)
DEF_LOOPS_ALL_UNSIGNED_TYPES (0, 15, b0s15)
DEF_LOOPS_ALL_SIGNED_TYPES (0, 15, b0s15)
DEF_LOOPS_ALL_SIGNED_TYPES (0, -1, b0sm1)
DEF_LOOPS_ALL_SIGNED_TYPES (0, -16, b0sm16)
DEF_LOOPS_ALL_SIGNED_TYPES (-16, 1, bm16s1)
DEF_LOOPS_ALL_UNSIGNED_TYPES (15, 1, b15s1)
DEF_LOOPS_ALL_SIGNED_TYPES (15, 1, b15s1)

/* Non Immediate Loops.  */
DEF_LOOPS_ALL_UNSIGNED_TYPES (0, 16, b0s16)
DEF_LOOPS_ALL_SIGNED_TYPES (0, 16, b0s16)
DEF_LOOPS_ALL_SIGNED_TYPES (0, -17, b0sm17)
DEF_LOOPS_ALL_SIGNED_TYPES (-17, 1, bm17s1)
DEF_LOOPS_ALL_UNSIGNED_TYPES (16, 1, b16s1)
DEF_LOOPS_ALL_SIGNED_TYPES (16, 1, b16s1)
DEF_LOOPS_ALL_UNSIGNED_TYPES (16, 16, b16s16)
DEF_LOOPS_ALL_SIGNED_TYPES (16, 16, b16s16)
DEF_LOOPS_ALL_SIGNED_TYPES (-17, -17, bm17sm17)

/* { dg-final { scan-assembler-times {\tindex\tz[0-9]+\.b, #0, #1\n} 2 } } */
/* { dg-final { scan-assembler-times {\tindex\tz[0-9]+\.b, #0, #15\n} 2 } } */
/* { dg-final { scan-assembler-times {\tindex\tz[0-9]+\.b, #0, #-1\n} 1 } } */
/* { dg-final { scan-assembler-times {\tindex\tz[0-9]+\.b, #0, #-16\n} 1 } } */
/* { dg-final { scan-assembler-times {\tindex\tz[0-9]+\.b, #-16, #1\n} 1 } } */
/* { dg-final { scan-assembler-times {\tindex\tz[0-9]+\.b, #15, #1\n} 2 } } */
/* { dg-final { scan-assembler-times {\tindex\tz[0-9]+\.b, #0, w[0-9]+\n} 3 } } */
/* { dg-final { scan-assembler-times {\tindex\tz[0-9]+\.b, w[0-9]+, #1\n} 3 } } */
/* { dg-final { scan-assembler-times {\tindex\tz[0-9]+\.b, w[0-9]+, w[0-9]+\n} 3 } } */

/* { dg-final { scan-assembler-times {\tindex\tz[0-9]+\.h, #0, #1\n} 2 } } */
/* { dg-final { scan-assembler-times {\tindex\tz[0-9]+\.h, #0, #15\n} 2 } } */
/* { dg-final { scan-assembler-times {\tindex\tz[0-9]+\.h, #0, #-1\n} 1 } } */
/* { dg-final { scan-assembler-times {\tindex\tz[0-9]+\.h, #0, #-16\n} 1 } } */
/* { dg-final { scan-assembler-times {\tindex\tz[0-9]+\.h, #-16, #1\n} 1 } } */
/* { dg-final { scan-assembler-times {\tindex\tz[0-9]+\.h, #15, #1\n} 2 } } */
/* { dg-final { scan-assembler-times {\tindex\tz[0-9]+\.h, #0, w[0-9]+\n} 3 } } */
/* { dg-final { scan-assembler-times {\tindex\tz[0-9]+\.h, w[0-9]+, #1\n} 3 } } */
/* { dg-final { scan-assembler-times {\tindex\tz[0-9]+\.h, w[0-9]+, w[0-9]+\n} 3 } } */

/* { dg-final { scan-assembler-times {\tindex\tz[0-9]+\.s, #0, #1\n} 2 } } */
/* { dg-final { scan-assembler-times {\tindex\tz[0-9]+\.s, #0, #15\n} 2 } } */
/* { dg-final { scan-assembler-times {\tindex\tz[0-9]+\.s, #0, #-1\n} 1 } } */
/* { dg-final { scan-assembler-times {\tindex\tz[0-9]+\.s, #0, #-16\n} 1 } } */
/* { dg-final { scan-assembler-times {\tindex\tz[0-9]+\.s, #-16, #1\n} 1 } } */
/* { dg-final { scan-assembler-times {\tindex\tz[0-9]+\.s, #15, #1\n} 2 } } */
/* { dg-final { scan-assembler-times {\tindex\tz[0-9]+\.s, #0, w[0-9]+\n} 3 } } */
/* { dg-final { scan-assembler-times {\tindex\tz[0-9]+\.s, w[0-9]+, #1\n} 3 } } */
/* { dg-final { scan-assembler-times {\tindex\tz[0-9]+\.s, w[0-9]+, w[0-9]+\n} 3 } } */

/* { dg-final { scan-assembler-times {\tindex\tz[0-9]+\.d, #0, #1\n} 2 } } */
/* { dg-final { scan-assembler-times {\tindex\tz[0-9]+\.d, #0, #15\n} 2 } } */
/* { dg-final { scan-assembler-times {\tindex\tz[0-9]+\.d, #0, #-1\n} 1 } } */
/* { dg-final { scan-assembler-times {\tindex\tz[0-9]+\.d, #0, #-16\n} 1 } } */
/* { dg-final { scan-assembler-times {\tindex\tz[0-9]+\.d, #-16, #1\n} 1 } } */
/* { dg-final { scan-assembler-times {\tindex\tz[0-9]+\.d, #15, #1\n} 2 } } */
/* { dg-final { scan-assembler-times {\tindex\tz[0-9]+\.d, #0, x[0-9]+\n} 3 } } */
/* { dg-final { scan-assembler-times {\tindex\tz[0-9]+\.d, x[0-9]+, #1\n} 3 } } */
/* { dg-final { scan-assembler-times {\tindex\tz[0-9]+\.d, x[0-9]+, x[0-9]+\n} 3 } } */

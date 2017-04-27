/* { dg-do assemble } */
/* { dg-options "-std=c99 -O2 -ftree-vectorize -march=armv8-a+sve --save-temps" } */

#define DO_REG_OPS(TYPE)						\
void ashiftr_##TYPE (signed TYPE* dst, signed TYPE src, int count)	\
{									\
  for (int i = 0; i < count; ++i)					\
    dst[i] = dst[i] >> src;						\
}									\
void lshiftr_##TYPE (unsigned TYPE* dst, unsigned TYPE src, int count)	\
{									\
  for (int i = 0; i < count; ++i)					\
    dst[i] = dst[i] >> src;						\
}									\
void lshiftl_##TYPE (unsigned TYPE* dst, unsigned TYPE src, int count)	\
{									\
  for (int i = 0; i < count; ++i)					\
    dst[i] = dst[i] << src;						\
}									\
void vashiftr_##TYPE (signed TYPE* dst, signed TYPE* src, int count)	\
{									\
  for (int i = 0; i < count; ++i)					\
    dst[i] = dst[i] >> src[i];						\
}									\
void vlshiftr_##TYPE (unsigned TYPE* dst, unsigned TYPE* src, int count) \
{									\
  for (int i = 0; i < count; ++i)					\
    dst[i] = dst[i] >> src[i];						\
}									\
void vlshiftl_##TYPE (unsigned TYPE* dst, unsigned TYPE* src, int count) \
{									\
  for (int i = 0; i < count; ++i)					\
    dst[i] = dst[i] << src[i];						\
}

#define DO_IMMEDIATE_OPS(VALUE, TYPE, NAME)				\
void vashiftr_imm_##NAME##_##TYPE (signed TYPE* dst, int count)		\
{									\
  for (int i = 0; i < count; ++i)					\
    dst[i] = dst[i] >> VALUE;						\
}									\
void vlshiftr_imm_##NAME##_##TYPE (unsigned TYPE* dst, int count)	\
{									\
  for (int i = 0; i < count; ++i)					\
    dst[i] = dst[i] >> VALUE;						\
}									\
void vlshiftl_imm_##NAME##_##TYPE (unsigned TYPE* dst, int count)	\
{									\
  for (int i = 0; i < count; ++i)					\
    dst[i] = dst[i] << VALUE;						\
}

DO_REG_OPS (int);

DO_IMMEDIATE_OPS (0, char, 0);
DO_IMMEDIATE_OPS (5, char, 5);
DO_IMMEDIATE_OPS (7, char, 7);

DO_IMMEDIATE_OPS (0, short, 0);
DO_IMMEDIATE_OPS (5, short, 5);
DO_IMMEDIATE_OPS (15, short, 15);

DO_IMMEDIATE_OPS (0, int, 0);
DO_IMMEDIATE_OPS (5, int, 5);
DO_IMMEDIATE_OPS (31, int, 31);

DO_IMMEDIATE_OPS (0, long, 0);
DO_IMMEDIATE_OPS (5, long, 5);
DO_IMMEDIATE_OPS (63, long, 63);

/* { dg-final { scan-assembler-times {\tasr\tz[0-9]+\.s, p[0-7]/m, z[0-9]+\.s, z[0-9]+\.s\n} 2 } } */
/* { dg-final { scan-assembler-times {\tlsr\tz[0-9]+\.s, p[0-7]/m, z[0-9]+\.s, z[0-9]+\.s\n} 2 } } */
/* { dg-final { scan-assembler-times {\tlsl\tz[0-9]+\.s, p[0-7]/m, z[0-9]+\.s, z[0-9]+\.s\n} 2 } } */

/* { dg-final { scan-assembler-times {\tasr\tz[0-9]+\.b, z[0-9]+\.b, #5\n} 1 } } */
/* { dg-final { scan-assembler-times {\tlsr\tz[0-9]+\.b, z[0-9]+\.b, #5\n} 1 } } */
/* { dg-final { scan-assembler-times {\tlsl\tz[0-9]+\.b, z[0-9]+\.b, #5\n} 1 } } */
/* { dg-final { scan-assembler-times {\tasr\tz[0-9]+\.b, z[0-9]+\.b, #7\n} 1 } } */
/* { dg-final { scan-assembler-times {\tlsr\tz[0-9]+\.b, z[0-9]+\.b, #7\n} 1 } } */
/* { dg-final { scan-assembler-times {\tlsl\tz[0-9]+\.b, z[0-9]+\.b, #7\n} 1 } } */

/* { dg-final { scan-assembler-times {\tasr\tz[0-9]+\.h, z[0-9]+\.h, #5\n} 1 } } */
/* { dg-final { scan-assembler-times {\tlsr\tz[0-9]+\.h, z[0-9]+\.h, #5\n} 1 } } */
/* { dg-final { scan-assembler-times {\tlsl\tz[0-9]+\.h, z[0-9]+\.h, #5\n} 1 } } */
/* { dg-final { scan-assembler-times {\tasr\tz[0-9]+\.h, z[0-9]+\.h, #15\n} 1 } } */
/* { dg-final { scan-assembler-times {\tlsr\tz[0-9]+\.h, z[0-9]+\.h, #15\n} 1 } } */
/* { dg-final { scan-assembler-times {\tlsl\tz[0-9]+\.h, z[0-9]+\.h, #15\n} 1 } } */

/* { dg-final { scan-assembler-times {\tasr\tz[0-9]+\.s, z[0-9]+\.s, #5\n} 1 } } */
/* { dg-final { scan-assembler-times {\tlsr\tz[0-9]+\.s, z[0-9]+\.s, #5\n} 1 } } */
/* { dg-final { scan-assembler-times {\tlsl\tz[0-9]+\.s, z[0-9]+\.s, #5\n} 1 } } */
/* { dg-final { scan-assembler-times {\tasr\tz[0-9]+\.s, z[0-9]+\.s, #31\n} 1 } } */
/* { dg-final { scan-assembler-times {\tlsr\tz[0-9]+\.s, z[0-9]+\.s, #31\n} 1 } } */
/* { dg-final { scan-assembler-times {\tlsl\tz[0-9]+\.s, z[0-9]+\.s, #31\n} 1 } } */

/* { dg-final { scan-assembler-times {\tasr\tz[0-9]+\.d, z[0-9]+\.d, #5\n} 1 } } */
/* { dg-final { scan-assembler-times {\tlsr\tz[0-9]+\.d, z[0-9]+\.d, #5\n} 1 } } */
/* { dg-final { scan-assembler-times {\tlsl\tz[0-9]+\.d, z[0-9]+\.d, #5\n} 1 } } */
/* { dg-final { scan-assembler-times {\tasr\tz[0-9]+\.d, z[0-9]+\.d, #63\n} 1 } } */
/* { dg-final { scan-assembler-times {\tlsr\tz[0-9]+\.d, z[0-9]+\.d, #63\n} 1 } } */
/* { dg-final { scan-assembler-times {\tlsl\tz[0-9]+\.d, z[0-9]+\.d, #63\n} 1 } } */

/* { dg-do assemble } */
/* { dg-options "-std=c99 -O3 -march=armv8-a+sve --save-temps" } */

#define DO_OPS(TYPE)					\
void vneg_##TYPE (TYPE* dst, TYPE* src, int count)	\
{							\
  for (int i = 0; i < count; ++i)			\
    dst[i] = -src[i];					\
}

DO_OPS (char)
DO_OPS (int)
DO_OPS (long)

/* { dg-final { scan-assembler-times {\tneg\tz[0-9]+\.b, p[0-7]/m, z[0-9]+\.b\n} 1 } } */
/* { dg-final { scan-assembler-times {\tneg\tz[0-9]+\.s, p[0-7]/m, z[0-9]+\.s\n} 1 } } */
/* { dg-final { scan-assembler-times {\tneg\tz[0-9]+\.d, p[0-7]/m, z[0-9]+\.d\n} 1 } } */

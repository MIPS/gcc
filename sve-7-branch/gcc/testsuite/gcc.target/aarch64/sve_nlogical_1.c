/* { dg-do assemble } */
/* { dg-options "-std=c99 -O3 -march=armv8-a+sve --save-temps" } */

#define DO_VNLOGICAL(TYPE)						\
void __attribute__ ((weak))						\
vnlogical_not_##TYPE (TYPE *dst, unsigned long count)			\
{									\
  for (int i = 0; i < count; i++)					\
    dst[i] = ~dst[i];							\
}									\
									\
void __attribute__ ((weak))						\
vnlogical_bic_##TYPE (TYPE *dst, TYPE *src, unsigned long count)	\
{									\
  for (int i = 0; i < count; i++)					\
    dst[i] = dst[i] & ~src[i];						\
}

DO_VNLOGICAL (char)
DO_VNLOGICAL (short)
DO_VNLOGICAL (int)
DO_VNLOGICAL (long)

/* { dg-final { scan-assembler-times {\tnot\tz[0-9]+\.b, p[0-7]/m, z[0-9]+\.b\n} 1 } } */
/* { dg-final { scan-assembler-times {\tnot\tz[0-9]+\.h, p[0-7]/m, z[0-9]+\.h\n} 1 } } */
/* { dg-final { scan-assembler-times {\tnot\tz[0-9]+\.s, p[0-7]/m, z[0-9]+\.s\n} 1 } } */
/* { dg-final { scan-assembler-times {\tnot\tz[0-9]+\.d, p[0-7]/m, z[0-9]+\.d\n} 1 } } */
/* { dg-final { scan-assembler-times {\tbic\tz[0-9]+\.d, z[0-9]+\.d, z[0-9]+\.d\n} 4 } } */

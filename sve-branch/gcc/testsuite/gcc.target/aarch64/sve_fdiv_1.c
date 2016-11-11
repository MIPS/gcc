/* { dg-do assemble } */
/* { dg-options "-O3 -march=armv8-a+sve -msve-vector-bits=256 --save-temps" } */

typedef float v8sf __attribute__((vector_size(32)));
typedef double v4df __attribute__((vector_size(32)));

#define DO_OP(TYPE)                        \
void vdiv##TYPE (TYPE *dst, TYPE src1)     \
{                                          \
  *dst = *dst / src1;                      \
}                                          \
void vdivr##TYPE (TYPE *_dst, TYPE _src1)  \
{                                          \
  register TYPE dst  asm("z0");            \
  register TYPE src1 asm("z2");            \
  register TYPE src2 asm("z4");            \
  dst = *_dst;                             \
  asm volatile ("" :: "w" (dst));          \
  src1 = _src1;                            \
  asm volatile ("" :: "w" (src1));         \
  dst = src1 / dst;                        \
  *_dst = dst;                             \
}

DO_OP (v8sf)
DO_OP (v4df)

/* { dg-final { scan-assembler-times {\tfdiv\tz[0-9]+\.s, p[0-7]/m, z[0-9]+\.s, z[0-9]+\.s\n} 1 } } */
/* { dg-final { scan-assembler-times {\tfdivr\tz[0-9]+\.s, p[0-7]/m, z[0-9]+\.s, z[0-9]+\.s\n} 1 } } */

/* { dg-final { scan-assembler-times {\tfdiv\tz[0-9]+\.d, p[0-7]/m, z[0-9]+\.d, z[0-9]+\.d\n} 1 } } */
/* { dg-final { scan-assembler-times {\tfdivr\tz[0-9]+\.d, p[0-7]/m, z[0-9]+\.d, z[0-9]+\.d\n} 1 } } */

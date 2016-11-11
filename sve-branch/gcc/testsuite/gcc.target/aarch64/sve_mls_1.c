/* { dg-do assemble } */
/* { dg-options "-O3 -march=armv8-a+sve -msve-vector-bits=256 --save-temps" } */

typedef char v32qi __attribute__((vector_size(32)));
typedef short v16hi __attribute__((vector_size(32)));
typedef int v8si __attribute__((vector_size(32)));
typedef long v4di __attribute__((vector_size(32)));

#define DO_OP(TYPE)                                   \
void vmls##TYPE (TYPE *_dst, TYPE _src1, TYPE _src2)  \
{                                                     \
  register TYPE dst  asm("z0");                       \
  register TYPE src1 asm("z2");                       \
  register TYPE src2 asm("z4");                       \
  dst = *_dst;                                        \
  asm volatile ("" :: "w" (dst));                     \
  src1 = _src1;                                       \
  asm volatile ("" :: "w" (src1));                    \
  src2 = _src2;                                       \
  asm volatile ("" :: "w" (src2));                    \
  dst = dst - (src1 * src2);                          \
  asm volatile ("" :: "w" (dst));                     \
  *_dst = dst;                                        \
}

DO_OP (v32qi)
DO_OP (v16hi)
DO_OP (v8si)
DO_OP (v4di)

/* { dg-final { scan-assembler-times {\tmls\tz0.b, p[0-7]/m, z2.b, z4.b\n} 1 } } */
/* { dg-final { scan-assembler-times {\tmls\tz0.h, p[0-7]/m, z2.h, z4.h\n} 1 } } */
/* { dg-final { scan-assembler-times {\tmls\tz0.s, p[0-7]/m, z2.s, z4.s\n} 1 } } */
/* { dg-final { scan-assembler-times {\tmls\tz0.d, p[0-7]/m, z2.d, z4.d\n} 1 } } */

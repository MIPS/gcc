/* { dg-do assemble } */
/* { dg-options "-O3 -march=armv8-a+sve -msve-vector-bits=256 --save-temps" } */

typedef float v8sf __attribute__((vector_size(32)));
typedef double v4df __attribute__((vector_size(32)));

#define DO_OP(TYPE)					\
void vfnmls##TYPE (TYPE *_dst, TYPE _src1, TYPE _src2)	\
{							\
  register TYPE dst  asm("z0");				\
  register TYPE src1 asm("z2");				\
  register TYPE src2 asm("z4");				\
  dst = *_dst;						\
  asm volatile ("" :: "w" (dst));			\
  src1 = _src1;						\
  asm volatile ("" :: "w" (src1));			\
  src2 = _src2;						\
  asm volatile ("" :: "w" (src2));			\
  dst = (-dst) + (src1 * src2);				\
  asm volatile ("" :: "w" (dst));			\
  *_dst = dst;						\
}

DO_OP (v8sf)
DO_OP (v4df)

/* { dg-final { scan-assembler-times {\tfnmls\tz0.s, p[0-7]/m, z2.s, z4.s\n} 1 } } */
/* { dg-final { scan-assembler-times {\tfnmls\tz0.d, p[0-7]/m, z2.d, z4.d\n} 1 } } */

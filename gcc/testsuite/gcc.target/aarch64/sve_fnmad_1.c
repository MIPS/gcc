/* { dg-do assemble } */
/* { dg-options "-O3 -march=armv8-a+sve -msve-vector-bits=256 --save-temps" } */

typedef _Float16 v16hf __attribute__((vector_size(32)));
typedef float v8sf __attribute__((vector_size(32)));
typedef double v4df __attribute__((vector_size(32)));

#define DO_OP(TYPE)						\
void vmad##TYPE (TYPE *x, TYPE y, TYPE z)			\
{								\
  register TYPE dst  asm("z0");					\
  register TYPE src1 asm("z2");					\
  register TYPE src2 asm("z4");					\
  dst = *x;							\
  src1 = y;							\
  src2 = z;							\
  asm volatile ("" :: "w" (dst), "w" (src1), "w" (src2));	\
  dst = (-dst * src1) - src2;					\
  asm volatile ("" :: "w" (dst));				\
  *x = dst;							\
}

DO_OP (v16hf)
DO_OP (v8sf)
DO_OP (v4df)

/* { dg-final { scan-assembler-times {\tfnmad\tz0\.h, p[0-7]/m, z2\.h, z4\.h\n} 1 } } */
/* { dg-final { scan-assembler-times {\tfnmad\tz0\.s, p[0-7]/m, z2\.s, z4\.s\n} 1 } } */
/* { dg-final { scan-assembler-times {\tfnmad\tz0\.d, p[0-7]/m, z2\.d, z4\.d\n} 1 } } */

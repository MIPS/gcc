/* { dg-do assemble } */
/* { dg-options {-std=c99 -O3 -march=armv8-a+sve --save-temps} } */

#define DO_REGREG_OPS(TYPE, OP, NAME)				\
void varith_##TYPE##_##NAME (TYPE* dst, TYPE* src, int count)	\
{								\
  for (int i = 0; i < count; ++i)				\
    dst[i] = dst[i] OP src[i];					\
}

#define DO_IMMEDIATE_OPS(VALUE, TYPE, OP, NAME)		\
void varithimm_##NAME##_##TYPE (TYPE* dst, int count)	\
{							\
  for (int i = 0; i < count; ++i)			\
    dst[i] = dst[i] OP VALUE;				\
}

#define DO_ARITH_OPS(TYPE, OP, NAME)			\
DO_REGREG_OPS (TYPE, OP, NAME);				\
DO_IMMEDIATE_OPS (0, TYPE, OP, NAME ## 0);		\
DO_IMMEDIATE_OPS (25, TYPE, OP, NAME ## 25);		\
DO_IMMEDIATE_OPS (125, TYPE, OP, NAME ## 125);		\
DO_IMMEDIATE_OPS (128, TYPE, OP, NAME ## 128);		\
DO_IMMEDIATE_OPS (-1, TYPE, OP, NAME ## minus1);	\
DO_IMMEDIATE_OPS (-5, TYPE, OP, NAME ## minus5);	\
DO_IMMEDIATE_OPS (-128, TYPE, OP, NAME ## minus128);	\
DO_IMMEDIATE_OPS (-129, TYPE, OP, NAME ## minus129);

DO_ARITH_OPS (char, *, mul)
DO_ARITH_OPS (short, *, mul)
DO_ARITH_OPS (int, *, mul)
DO_ARITH_OPS (long, *, mul)

/* { dg-final { scan-assembler-times {\tmul\tz[0-9]+\.b, p[0-7]/m, z[0-9]+\.b, z[0-9]+\.b\n} 1 } } */
/* { dg-final { scan-assembler-times {\tmul\tz[0-9]+\.b, z[0-9]+\.b, #25\n} 1 } } */
/* { dg-final { scan-assembler-times {\tmul\tz[0-9]+\.b, z[0-9]+\.b, #125\n} 1 } } */
/* { dg-final { scan-assembler-times {\tmul\tz[0-9]+\.b, z[0-9]+\.b, #127\n} 1 } } */
/* { dg-final { scan-assembler-not {\tmul\tz[0-9]+\.b, z[0-9]+\.b, #128\n} } } */
/* { dg-final { scan-assembler-not {\tmul\tz[0-9]+\.b, z[0-9]+\.b, #-1\n} } } */
/* { dg-final { scan-assembler-times {\tmul\tz[0-9]+\.b, z[0-9]+\.b, #-5\n} 1 } } */
/* { dg-final { scan-assembler-times {\tmul\tz[0-9]+\.b, z[0-9]+\.b, #-128\n} 2 } } */

/* 1 for the REGREG test, #1 each for the scan-assembler-nots
   (-129 and 128).  */
/* { dg-final { scan-assembler-times {\tmul\tz[0-9]+\.h, p[0-7]/m, z[0-9]+\.h, z[0-9]+\.h\n} 3 } } */
/* { dg-final { scan-assembler-times {\tmul\tz[0-9]+\.h, z[0-9]+\.h, #25\n} 1 } } */
/* { dg-final { scan-assembler-times {\tmul\tz[0-9]+\.h, z[0-9]+\.h, #125\n} 1 } } */
/* { dg-final { scan-assembler-not {\tmul\tz[0-9]+\.h, z[0-9]+\.h, #128\n} } } */
/* { dg-final { scan-assembler-times {\tmul\tz[0-9]+\.h, z[0-9]+\.h, #-5\n} 1 } } */
/* { dg-final { scan-assembler-times {\tmul\tz[0-9]+\.h, z[0-9]+\.h, #-128\n} 1 } } */
/* { dg-final { scan-assembler-not {\tmul\tz[0-9]+\.h, z[0-9]+\.h, #-129\n} } } */

/* { dg-final { scan-assembler-times {\tmul\tz[0-9]+\.s, p[0-7]/m, z[0-9]+\.s, z[0-9]+\.s\n} 3 } } */
/* { dg-final { scan-assembler-times {\tmul\tz[0-9]+\.s, z[0-9]+\.s, #25\n} 1 } } */
/* { dg-final { scan-assembler-times {\tmul\tz[0-9]+\.s, z[0-9]+\.s, #125\n} 1 } } */
/* { dg-final { scan-assembler-not {\tmul\tz[0-9]+\.s, z[0-9]+\.s, #128\n} } } 5B*/
/* { dg-final { scan-assembler-times {\tmul\tz[0-9]+\.s, z[0-9]+\.s, #-5\n} 1 } } */
/* { dg-final { scan-assembler-times {\tmul\tz[0-9]+\.s, z[0-9]+\.s, #-128\n} 1 } } */
/* { dg-final { scan-assembler-not {\tmul\tz[0-9]+\.s, z[0-9]+\.s, #-129\n} } } */

/* { dg-final { scan-assembler-times {\tmul\tz[0-9]+\.d, p[0-7]/m, z[0-9]+\.d, z[0-9]+\.d\n} 3 } } */
/* { dg-final { scan-assembler-times {\tmul\tz[0-9]+\.d, z[0-9]+\.d, #25\n} 1 } } */
/* { dg-final { scan-assembler-times {\tmul\tz[0-9]+\.d, z[0-9]+\.d, #125\n} 1 } } */
/* { dg-final { scan-assembler-not {\tmul\tz[0-9]+\.d, z[0-9]+\.d, #128\n} } } */
/* { dg-final { scan-assembler-times {\tmul\tz[0-9]+\.d, z[0-9]+\.d, #-5\n} 1 } } */
/* { dg-final { scan-assembler-times {\tmul\tz[0-9]+\.d, z[0-9]+\.d, #-128\n} 1 } } */
/* { dg-final { scan-assembler-not {\tmul\tz[0-9]+\.d, z[0-9]+\.d, #-129\n} } } */

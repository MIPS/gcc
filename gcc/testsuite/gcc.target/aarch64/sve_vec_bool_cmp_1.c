/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize -fno-inline -march=armv8-a+sve" } */

#include <stdint.h>
#include <stdbool.h>

#define VEC_BOOL_CMPNE(VARTYPE, INDUCTYPE)				\
void									\
vec_bool_cmpne##VARTYPE##INDUCTYPE (VARTYPE *dst, VARTYPE *src,		\
				    INDUCTYPE start, INDUCTYPE n,	\
				    INDUCTYPE mask)			\
{									\
  INDUCTYPE i;								\
  for (i = 0; i < n; i++)						\
    {									\
      bool lhs = i >= start;						\
      bool rhs = (i & mask) != 0x3D;					\
      if (lhs != rhs)							\
        dst[i] = src[i];						\
    }									\
}

#define VEC_BOOL_CMPEQ(VARTYPE, INDUCTYPE)				\
void									\
vec_bool_cmpeq##VARTYPE##INDUCTYPE (VARTYPE *dst, VARTYPE *src,		\
				    INDUCTYPE start, INDUCTYPE n,	\
				    INDUCTYPE mask)			\
{									\
  INDUCTYPE i;								\
  for (i = 0; i < n; i++)						\
    {									\
      bool lhs = i >= start;						\
      bool rhs = (i & mask) != 0x3D;					\
      if (lhs == rhs)							\
        dst[i] = src[i];						\
    }									\
}

VEC_BOOL_CMPNE (uint8_t, uint8_t)
VEC_BOOL_CMPNE (uint16_t, uint16_t)
VEC_BOOL_CMPNE (uint32_t, uint32_t)
VEC_BOOL_CMPNE (uint64_t, uint64_t)
VEC_BOOL_CMPNE (float, uint32_t)
VEC_BOOL_CMPNE (double, uint64_t)

VEC_BOOL_CMPEQ (uint8_t, uint8_t)
VEC_BOOL_CMPEQ (uint16_t, uint16_t)
VEC_BOOL_CMPEQ (uint32_t, uint32_t)
VEC_BOOL_CMPEQ (uint64_t, uint64_t)
VEC_BOOL_CMPEQ (float, uint32_t)
VEC_BOOL_CMPEQ (double, uint64_t)

/* Both CMPNE and CMPEQ loops will contain an exclusive predicate or.  */
/* { dg-final { scan-assembler-times {\teors?\tp[0-9]*\.b, p[0-7]/z, p[0-9]*\.b, p[0-9]*\.b\n} 12 } } */
/* CMPEQ will also contain a predicate not operation.  */
/* { dg-final { scan-assembler-times {\tnot\tp[0-9]*\.b, p[0-7]/z, p[0-9]*\.b\n} 6 } } */

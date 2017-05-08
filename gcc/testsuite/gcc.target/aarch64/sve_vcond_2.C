/* { dg-do compile } */
/* { dg-options "-std=c++11 -O2 -ftree-vectorize -march=armv8-a+sve -fno-inline -fno-ipa-icf" } */

#include <stdint.h>

#define NUM_ELEMS(TYPE) (32 / sizeof (TYPE))

#define DEF_VCOND(DATA_TYPE, CMP_TYPE, COND, SUFFIX)			\
  void vcond_##CMP_TYPE##SUFFIX (DATA_TYPE *__restrict__ r,		\
				 DATA_TYPE *__restrict__ a,		\
				 DATA_TYPE *__restrict__ b,		\
				 CMP_TYPE *__restrict__ x,		\
				 CMP_TYPE *__restrict__ y,		\
				 int n)					\
  {									\
    for (int i = 0; i < n; i++)						\
      {									\
	CMP_TYPE yval = y[i], xval = x[i];				\
	DATA_TYPE aval = a[i], bval = b[i];				\
	r[i] = xval COND yval ? aval : bval;				\
      }									\
  }

#define DEF_VCOND_IMM(DATA_TYPE, CMP_TYPE, COND, IMM, SUFFIX)	\
  void vcond_imm_##CMP_TYPE##SUFFIX (DATA_TYPE *__restrict__ r,	\
				     DATA_TYPE *__restrict__ a,	\
				     DATA_TYPE *__restrict__ b,	\
				     CMP_TYPE *__restrict__ x,	\
				     int n)			\
  {								\
    for (int i = 0; i < n; i++)					\
      {								\
	CMP_TYPE xval = x[i];					\
	DATA_TYPE aval = a[i], bval = b[i];			\
	r[i] = xval COND (CMP_TYPE) IMM ? aval : bval;		\
      }								\
  }

#define DEF_VCOND_SIGNED_ALL(COND, SUFFIX)			\
  DEF_VCOND (int8_t, int8_t, COND, SUFFIX)			\
  DEF_VCOND (int16_t, int16_t, COND, SUFFIX)			\
  DEF_VCOND (int32_t, int32_t, COND, SUFFIX)			\
  DEF_VCOND (int64_t, int64_t, COND, SUFFIX)			\
  DEF_VCOND (float, int32_t, COND, SUFFIX##_float)		\
  DEF_VCOND (double, int64_t, COND, SUFFIX##_double)

#define DEF_VCOND_UNSIGNED_ALL(COND, SUFFIX)			\
  DEF_VCOND (uint8_t, uint8_t, COND, SUFFIX)			\
  DEF_VCOND (uint16_t, uint16_t, COND, SUFFIX)			\
  DEF_VCOND (uint32_t, uint32_t, COND, SUFFIX)			\
  DEF_VCOND (uint64_t, uint64_t, COND, SUFFIX)			\
  DEF_VCOND (float, uint32_t, COND, SUFFIX##_float)		\
  DEF_VCOND (double, uint64_t, COND, SUFFIX##_double)

#define DEF_VCOND_ALL(COND, SUFFIX)		\
  DEF_VCOND_SIGNED_ALL (COND, SUFFIX)		\
  DEF_VCOND_UNSIGNED_ALL (COND, SUFFIX)

#define DEF_VCOND_IMM_SIGNED_ALL(COND, IMM, SUFFIX)		\
  DEF_VCOND_IMM (int8_t, int8_t, COND, IMM, SUFFIX)		\
  DEF_VCOND_IMM (int16_t, int16_t, COND, IMM, SUFFIX)		\
  DEF_VCOND_IMM (int32_t, int32_t, COND, IMM, SUFFIX)		\
  DEF_VCOND_IMM (int64_t, int64_t, COND, IMM, SUFFIX)		\
  DEF_VCOND_IMM (float, int32_t, COND, IMM, SUFFIX##_float)	\
  DEF_VCOND_IMM (double, int64_t, COND, IMM, SUFFIX##_double)

#define DEF_VCOND_IMM_UNSIGNED_ALL(COND, IMM, SUFFIX)		\
  DEF_VCOND_IMM (uint8_t, uint8_t, COND, IMM, SUFFIX)		\
  DEF_VCOND_IMM (uint16_t, uint16_t, COND, IMM, SUFFIX)		\
  DEF_VCOND_IMM (uint32_t, uint32_t, COND, IMM, SUFFIX)		\
  DEF_VCOND_IMM (uint64_t, uint64_t, COND, IMM, SUFFIX)		\
  DEF_VCOND_IMM (float, uint32_t, COND, IMM, SUFFIX##_float)	\
  DEF_VCOND_IMM (double, uint64_t, COND, IMM, SUFFIX##_double)

#define DEF_VCOND_IMM_ALL(COND, IMM, SUFFIX)		\
  DEF_VCOND_IMM_SIGNED_ALL (COND, IMM, SUFFIX)		\
  DEF_VCOND_IMM_UNSIGNED_ALL (COND, IMM, SUFFIX)

DEF_VCOND_ALL (>, _gt)
DEF_VCOND_ALL (<, _lt)
DEF_VCOND_ALL (>=, _ge)
DEF_VCOND_ALL (<=, _le)
DEF_VCOND_ALL (==, _eq)
DEF_VCOND_ALL (!=, _ne)

/* == Expect immediates to make it into the encoding == */

DEF_VCOND_IMM_ALL (>, 5, _gt)
DEF_VCOND_IMM_ALL (<, 5, _lt)
DEF_VCOND_IMM_ALL (>=, 5, _ge)
DEF_VCOND_IMM_ALL (<=, 5, _le)
DEF_VCOND_IMM_ALL (==, 5, _eq)
DEF_VCOND_IMM_ALL (!=, 5, _ne)

DEF_VCOND_IMM_SIGNED_ALL (>, 15, _gt2)
DEF_VCOND_IMM_SIGNED_ALL (<, 15, _lt2)
DEF_VCOND_IMM_SIGNED_ALL (>=, 15, _ge2)
DEF_VCOND_IMM_SIGNED_ALL (<=, 15, _le2)
DEF_VCOND_IMM_ALL (==, 15, _eq2)
DEF_VCOND_IMM_ALL (!=, 15, _ne2)

DEF_VCOND_IMM_SIGNED_ALL (>, 16, _gt3)
DEF_VCOND_IMM_SIGNED_ALL (<, 16, _lt3)
DEF_VCOND_IMM_SIGNED_ALL (>=, 16, _ge3)
DEF_VCOND_IMM_SIGNED_ALL (<=, 16, _le3)
DEF_VCOND_IMM_ALL (==, 16, _eq3)
DEF_VCOND_IMM_ALL (!=, 16, _ne3)

DEF_VCOND_IMM_SIGNED_ALL (>, -16, _gt4)
DEF_VCOND_IMM_SIGNED_ALL (<, -16, _lt4)
DEF_VCOND_IMM_SIGNED_ALL (>=, -16, _ge4)
DEF_VCOND_IMM_SIGNED_ALL (<=, -16, _le4)
DEF_VCOND_IMM_ALL (==, -16, _eq4)
DEF_VCOND_IMM_ALL (!=, -16, _ne4)

DEF_VCOND_IMM_SIGNED_ALL (>, -17, _gt5)
DEF_VCOND_IMM_SIGNED_ALL (<, -17, _lt5)
DEF_VCOND_IMM_SIGNED_ALL (>=, -17, _ge5)
DEF_VCOND_IMM_SIGNED_ALL (<=, -17, _le5)
DEF_VCOND_IMM_ALL (==, -17, _eq5)
DEF_VCOND_IMM_ALL (!=, -17, _ne5)

DEF_VCOND_IMM_UNSIGNED_ALL (>, 0, _gt6)
/* Testing if an unsigned value >= 0 or < 0 is pointless as it will get
   folded away by the compiler.  */
DEF_VCOND_IMM_UNSIGNED_ALL (<=, 0, _le6)

DEF_VCOND_IMM_UNSIGNED_ALL (>, 127, _gt7)
DEF_VCOND_IMM_UNSIGNED_ALL (<, 127, _lt7)
DEF_VCOND_IMM_UNSIGNED_ALL (>=, 127, _ge7)
DEF_VCOND_IMM_UNSIGNED_ALL (<=, 127, _le7)

/* == Expect immediates to NOT make it into the encoding, and instead be
      forced into a register.  == */
DEF_VCOND_IMM_UNSIGNED_ALL (>, 128, _gt8)
DEF_VCOND_IMM_UNSIGNED_ALL (<, 128, _lt8)
DEF_VCOND_IMM_UNSIGNED_ALL (>=, 128, _ge8)
DEF_VCOND_IMM_UNSIGNED_ALL (<=, 128, _le8)

/* { dg-final { scan-assembler-times {\tsel\tz[0-9]+.b, p[0-7], z[0-9]+\.b, z[0-9]+\.b\n} 66 } } */
/* { dg-final { scan-assembler-times {\tsel\tz[0-9]+.h, p[0-7], z[0-9]+\.h, z[0-9]+\.h\n} 66 } } */
/* { dg-final { scan-assembler-times {\tsel\tz[0-9]+.s, p[0-7], z[0-9]+\.s, z[0-9]+\.s\n} 132 } } */
/* { dg-final { scan-assembler-times {\tsel\tz[0-9]+.d, p[0-7], z[0-9]+\.d, z[0-9]+\.d\n} 132 } } */

/* There are two signed ordered register comparisons for each of .b and .h,
   one for a variable comparison and one for one of the two out-of-range
   constant comparisons.  The other out-of-ranger constant comparison can
   be adjusted to an in-range value by inverting the handling of equality.

   The same pattern appears twice for each .s and .d, once for integer data
   and once for floating-point data.  */
/* { dg-final { scan-assembler-times {\tcmpgt\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, z[0-9]+\.b\n} 2 } } */
/* { dg-final { scan-assembler-times {\tcmpgt\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, z[0-9]+\.h\n} 2 } } */
/* { dg-final { scan-assembler-times {\tcmpgt\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, z[0-9]+\.s\n} 4 } } */
/* { dg-final { scan-assembler-times {\tcmpgt\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, z[0-9]+\.d\n} 4 } } */

/* { dg-final { scan-assembler-times {\tcmple\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, z[0-9]+\.b\n} 2 } } */
/* { dg-final { scan-assembler-times {\tcmple\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, z[0-9]+\.h\n} 2 } } */
/* { dg-final { scan-assembler-times {\tcmple\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, z[0-9]+\.s\n} 4 } } */
/* { dg-final { scan-assembler-times {\tcmple\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, z[0-9]+\.d\n} 4 } } */

/* { dg-final { scan-assembler-times {\tcmplt\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, z[0-9]+\.b\n} 2 } } */
/* { dg-final { scan-assembler-times {\tcmplt\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, z[0-9]+\.h\n} 2 } } */
/* { dg-final { scan-assembler-times {\tcmplt\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, z[0-9]+\.s\n} 4 } } */
/* { dg-final { scan-assembler-times {\tcmplt\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, z[0-9]+\.d\n} 4 } } */

/* { dg-final { scan-assembler-times {\tcmpge\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, z[0-9]+\.b\n} 2 } } */
/* { dg-final { scan-assembler-times {\tcmpge\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, z[0-9]+\.h\n} 2 } } */
/* { dg-final { scan-assembler-times {\tcmpge\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, z[0-9]+\.s\n} 4 } } */
/* { dg-final { scan-assembler-times {\tcmpge\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, z[0-9]+\.d\n} 4 } } */

/* { dg-final { scan-assembler-times {\tcmphi\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, z[0-9]+\.b\n} 2 } } */
/* { dg-final { scan-assembler-times {\tcmphi\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, z[0-9]+\.h\n} 2 } } */
/* { dg-final { scan-assembler-times {\tcmphi\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, z[0-9]+\.s\n} 4 } } */
/* { dg-final { scan-assembler-times {\tcmphi\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, z[0-9]+\.d\n} 4 } } */

/* { dg-final { scan-assembler-times {\tcmpls\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, z[0-9]+\.b\n} 2 } } */
/* { dg-final { scan-assembler-times {\tcmpls\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, z[0-9]+\.h\n} 2 } } */
/* { dg-final { scan-assembler-times {\tcmpls\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, z[0-9]+\.s\n} 4 } } */
/* { dg-final { scan-assembler-times {\tcmpls\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, z[0-9]+\.d\n} 4 } } */

/* Out-of-range >= is converted to in-range >.  */
/* { dg-final { scan-assembler-times {\tcmphs\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, z[0-9]+\.b\n} 1 } } */
/* { dg-final { scan-assembler-times {\tcmphs\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, z[0-9]+\.h\n} 1 } } */
/* { dg-final { scan-assembler-times {\tcmphs\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, z[0-9]+\.s\n} 2 } } */
/* { dg-final { scan-assembler-times {\tcmphs\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, z[0-9]+\.d\n} 2 } } */

/* Out-of-range < is converted to in-range <=.  */
/* { dg-final { scan-assembler-times {\tcmplo\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, z[0-9]+\.b\n} 1 } } */
/* { dg-final { scan-assembler-times {\tcmplo\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, z[0-9]+\.h\n} 1 } } */
/* { dg-final { scan-assembler-times {\tcmplo\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, z[0-9]+\.s\n} 2 } } */
/* { dg-final { scan-assembler-times {\tcmplo\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, z[0-9]+\.d\n} 2 } } */

/* 6 for .b and .h: {signed, unsigned\n} x {variable, too high, too low\n}.  */
/* 12 for .s and .d: the above 6 repeated for integer and floating-point
   data.  */
/* { dg-final { scan-assembler-times {\tcmpeq\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, z[0-9]+\.b\n} 6 } } */
/* { dg-final { scan-assembler-times {\tcmpeq\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, z[0-9]+\.h\n} 6 } } */
/* { dg-final { scan-assembler-times {\tcmpeq\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, z[0-9]+\.s\n} 12 } } */
/* { dg-final { scan-assembler-times {\tcmpeq\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, z[0-9]+\.d\n} 12 } } */

/* { dg-final { scan-assembler-times {\tcmpne\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, z[0-9]+\.b\n} 6 } } */
/* { dg-final { scan-assembler-times {\tcmpne\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, z[0-9]+\.h\n} 6 } } */
/* { dg-final { scan-assembler-times {\tcmpne\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, z[0-9]+\.s\n} 12 } } */
/* { dg-final { scan-assembler-times {\tcmpne\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, z[0-9]+\.d\n} 12 } } */

/* Also used for >= 16. */
/* { dg-final { scan-assembler-times {\tcmpgt\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, #15\n} 2 } } */
/* { dg-final { scan-assembler-times {\tcmpgt\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, #15\n} 2 } } */
/* { dg-final { scan-assembler-times {\tcmpgt\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, #15\n} 4 } } */
/* { dg-final { scan-assembler-times {\tcmpgt\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, #15\n} 4 } } */

/* gcc converts "a < 15" into "a <= 14".  */
/* { dg-final { scan-assembler-times {\tcmple\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, #14\n} 1 } } */
/* { dg-final { scan-assembler-times {\tcmple\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, #14\n} 1 } } */
/* { dg-final { scan-assembler-times {\tcmple\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, #14\n} 2 } } */
/* { dg-final { scan-assembler-times {\tcmple\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, #14\n} 2 } } */

/* gcc converts "a >= 15" into "a > 14".  */
/* { dg-final { scan-assembler-times {\tcmpgt\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, #14\n} 1 } } */
/* { dg-final { scan-assembler-times {\tcmpgt\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, #14\n} 1 } } */
/* { dg-final { scan-assembler-times {\tcmpgt\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, #14\n} 2 } } */
/* { dg-final { scan-assembler-times {\tcmpgt\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, #14\n} 2 } } */

/* Also used for < 16.  */
/* { dg-final { scan-assembler-times {\tcmple\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, #15\n} 2 } } */
/* { dg-final { scan-assembler-times {\tcmple\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, #15\n} 2 } } */
/* { dg-final { scan-assembler-times {\tcmple\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, #15\n} 4 } } */
/* { dg-final { scan-assembler-times {\tcmple\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, #15\n} 4 } } */

/* Appears once for each signedness.  */
/* { dg-final { scan-assembler-times {\tcmpeq\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, #15\n} 2 } } */
/* { dg-final { scan-assembler-times {\tcmpeq\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, #15\n} 2 } } */
/* { dg-final { scan-assembler-times {\tcmpeq\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, #15\n} 4 } } */
/* { dg-final { scan-assembler-times {\tcmpeq\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, #15\n} 4 } } */

/* { dg-final { scan-assembler-times {\tcmpne\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, #15\n} 2 } } */
/* { dg-final { scan-assembler-times {\tcmpne\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, #15\n} 2 } } */
/* { dg-final { scan-assembler-times {\tcmpne\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, #15\n} 4 } } */
/* { dg-final { scan-assembler-times {\tcmpne\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, #15\n} 4 } } */

/* { dg-final { scan-assembler-times {\tcmpeq\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, #-16\n} 2 } } */
/* { dg-final { scan-assembler-times {\tcmpeq\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, #-16\n} 2 } } */
/* { dg-final { scan-assembler-times {\tcmpeq\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, #-16\n} 4 } } */
/* { dg-final { scan-assembler-times {\tcmpeq\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, #-16\n} 4 } } */

/* { dg-final { scan-assembler-times {\tcmpne\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, #-16\n} 2 } } */
/* { dg-final { scan-assembler-times {\tcmpne\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, #-16\n} 2 } } */
/* { dg-final { scan-assembler-times {\tcmpne\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, #-16\n} 4 } } */
/* { dg-final { scan-assembler-times {\tcmpne\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, #-16\n} 4 } } */

/* gcc converts "a > -16" into "a >= -15".  */
/* { dg-final { scan-assembler-times {\tcmpge\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, #-15\n} 1 } } */
/* { dg-final { scan-assembler-times {\tcmpge\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, #-15\n} 1 } } */
/* { dg-final { scan-assembler-times {\tcmpge\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, #-15\n} 2 } } */
/* { dg-final { scan-assembler-times {\tcmpge\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, #-15\n} 2 } } */

/* Also used for <= -17.  */
/* { dg-final { scan-assembler-times {\tcmplt\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, #-16\n} 2 } } */
/* { dg-final { scan-assembler-times {\tcmplt\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, #-16\n} 2 } } */
/* { dg-final { scan-assembler-times {\tcmplt\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, #-16\n} 4 } } */
/* { dg-final { scan-assembler-times {\tcmplt\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, #-16\n} 4 } } */

/* Also used for > -17.  */
/* { dg-final { scan-assembler-times {\tcmpge\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, #-16\n} 2 } } */
/* { dg-final { scan-assembler-times {\tcmpge\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, #-16\n} 2 } } */
/* { dg-final { scan-assembler-times {\tcmpge\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, #-16\n} 4 } } */
/* { dg-final { scan-assembler-times {\tcmpge\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, #-16\n} 4 } } */

/* gcc converts "a <= -16" into "a < -15".  */
/* { dg-final { scan-assembler-times {\tcmplt\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, #-15\n} 1 } } */
/* { dg-final { scan-assembler-times {\tcmplt\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, #-15\n} 1 } } */
/* { dg-final { scan-assembler-times {\tcmplt\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, #-15\n} 2 } } */
/* { dg-final { scan-assembler-times {\tcmplt\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, #-15\n} 2 } } */

/* gcc converts "a > 0" into "a != 0".  */
/* { dg-final { scan-assembler-times {\tcmpne\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, #0\n} 1 } } */
/* { dg-final { scan-assembler-times {\tcmpne\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, #0\n} 1 } } */
/* { dg-final { scan-assembler-times {\tcmpne\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, #0\n} 2 } } */
/* { dg-final { scan-assembler-times {\tcmpne\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, #0\n} 2 } } */

/* gcc converts "a <= 0" into "a == 0".  */
/* { dg-final { scan-assembler-times {\tcmpeq\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, #0\n} 1 } } */
/* { dg-final { scan-assembler-times {\tcmpeq\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, #0\n} 1 } } */
/* { dg-final { scan-assembler-times {\tcmpeq\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, #0\n} 2 } } */
/* { dg-final { scan-assembler-times {\tcmpeq\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, #0\n} 2 } } */

/* Also used for >= 128.  */
/* { dg-final { scan-assembler-times {\tcmphi\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, #127\n} 2 { xfail *-*-* } } } */
/* { dg-final { scan-assembler-times {\tcmphi\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, #127\n} 2 } } */
/* { dg-final { scan-assembler-times {\tcmphi\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, #127\n} 4 } } */
/* { dg-final { scan-assembler-times {\tcmphi\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, #127\n} 4 } } */

/* gcc converts "a < 127" into "a <= 126".  */
/* { dg-final { scan-assembler-times {\tcmpls\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, #126\n} 1 } } */
/* { dg-final { scan-assembler-times {\tcmpls\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, #126\n} 1 } } */
/* { dg-final { scan-assembler-times {\tcmpls\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, #126\n} 2 } } */
/* { dg-final { scan-assembler-times {\tcmpls\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, #126\n} 2 } } */

/* gcc converts "a >= 127" into "a > 126".  */
/* { dg-final { scan-assembler-times {\tcmphi\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, #126\n} 1 } } */
/* { dg-final { scan-assembler-times {\tcmphi\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, #126\n} 1 } } */
/* { dg-final { scan-assembler-times {\tcmphi\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, #126\n} 2 } } */
/* { dg-final { scan-assembler-times {\tcmphi\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, #126\n} 2 } } */

/* Also used for < 128.  */
/* { dg-final { scan-assembler-times {\tcmpls\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, #127\n} 2 { xfail *-*-* } } } */
/* { dg-final { scan-assembler-times {\tcmpls\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, #127\n} 2 } } */
/* { dg-final { scan-assembler-times {\tcmpls\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, #127\n} 4 } } */
/* { dg-final { scan-assembler-times {\tcmpls\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, #127\n} 4 } } */

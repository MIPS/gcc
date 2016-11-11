/* { dg-do compile { target { ! *-*-* } } } */
/* { dg-options "-std=c++11 -O3 -fno-inline -march=armv8-a+sve" } */

#include <stdint.h>

typedef int8_t v32qi __attribute__((vector_size(32)));
typedef int16_t v16hi __attribute__((vector_size(32)));
typedef int32_t v8si __attribute__((vector_size(32)));
typedef int64_t v4di __attribute__((vector_size(32)));

typedef uint8_t v32qu __attribute__((vector_size(32)));
typedef uint16_t v16hu __attribute__((vector_size(32)));
typedef uint32_t v8su __attribute__((vector_size(32)));
typedef uint64_t v4du __attribute__((vector_size(32)));

#define NUM_ELEMS(TYPE) (sizeof (r_##TYPE) / sizeof (r_##TYPE[0]))

#define DEF_VCOND(TYPE,COND,SUFFIX)				\
TYPE vcond_##TYPE##SUFFIX (TYPE x, TYPE y, TYPE a, TYPE b)	\
{								\
  TYPE r;							\
  r = a COND b ? x : y;						\
  return r;							\
}

#define DEF_VCOND_IMM(TYPE,COND,IMM,SUFFIX)		\
TYPE vcond_imm_##TYPE##SUFFIX (TYPE x, TYPE y, TYPE a)	\
{							\
  TYPE r;						\
  r = a COND IMM ? x : y;				\
  return r;						\
}

#define DEF_VCOND_SIGNED_ALL(COND,SUFFIX)	\
DEF_VCOND (v32qi,COND,SUFFIX)			\
DEF_VCOND (v16hi,COND,SUFFIX)			\
DEF_VCOND (v8si,COND,SUFFIX)			\
DEF_VCOND (v4di,COND,SUFFIX)

#define DEF_VCOND_UNSIGNED_ALL(COND,SUFFIX)	\
DEF_VCOND (v32qu,COND,SUFFIX)			\
DEF_VCOND (v16hu,COND,SUFFIX)			\
DEF_VCOND (v8su,COND,SUFFIX)			\
DEF_VCOND (v4du,COND,SUFFIX)

#define DEF_VCOND_ALL(COND,SUFFIX)		\
DEF_VCOND_SIGNED_ALL (COND,SUFFIX)		\
DEF_VCOND_UNSIGNED_ALL (COND,SUFFIX)

#define DEF_VCOND_IMM_SIGNED_ALL(COND,IMM,SUFFIX)	\
DEF_VCOND_IMM (v32qi,COND,IMM,SUFFIX)			\
DEF_VCOND_IMM (v16hi,COND,IMM,SUFFIX)			\
DEF_VCOND_IMM (v8si,COND,IMM,SUFFIX)			\
DEF_VCOND_IMM (v4di,COND,IMM,SUFFIX)

#define DEF_VCOND_IMM_UNSIGNED_ALL(COND,IMM,SUFFIX)	\
DEF_VCOND_IMM (v32qu,COND,IMM,SUFFIX)			\
DEF_VCOND_IMM (v16hu,COND,IMM,SUFFIX)			\
DEF_VCOND_IMM (v8su,COND,IMM,SUFFIX)			\
DEF_VCOND_IMM (v4du,COND,IMM,SUFFIX)

#define DEF_VCOND_IMM_ALL(COND,IMM,SUFFIX)	\
DEF_VCOND_IMM_SIGNED_ALL (COND,IMM,SUFFIX)	\
DEF_VCOND_IMM_UNSIGNED_ALL (COND,IMM,SUFFIX)

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
DEF_VCOND_IMM_SIGNED_ALL (==, 15, _eq2)
DEF_VCOND_IMM_SIGNED_ALL (!=, 15, _ne2)

DEF_VCOND_IMM_SIGNED_ALL (>, -16, _gt3)
DEF_VCOND_IMM_SIGNED_ALL (<, -16, _lt3)
DEF_VCOND_IMM_SIGNED_ALL (>=, -16, _ge3)
DEF_VCOND_IMM_SIGNED_ALL (<=, -16, _le3)
DEF_VCOND_IMM_SIGNED_ALL (==, -16, _eq3)
DEF_VCOND_IMM_SIGNED_ALL (!=, -16, _ne3)

DEF_VCOND_IMM_UNSIGNED_ALL (>, 0, _gt4)
/* Testing if an unsigned value >= 0 or < 0 is pointless as it will
   get folded away by the compiler.  */
DEF_VCOND_IMM_UNSIGNED_ALL (<=, 0, _le4)

DEF_VCOND_IMM_UNSIGNED_ALL (>, 31, _gt5)
DEF_VCOND_IMM_UNSIGNED_ALL (<, 31, _lt5)
DEF_VCOND_IMM_UNSIGNED_ALL (>=, 31, _ge5)
DEF_VCOND_IMM_UNSIGNED_ALL (<=, 31, _le5)

/* Expect immediates to NOT make it into the encoding, and instead be
   forced into a register.  == */
DEF_VCOND_IMM_ALL (>, 32, _gt6)
DEF_VCOND_IMM_ALL (<, 32, _lt6)
DEF_VCOND_IMM_ALL (>=, 32, _ge6)
DEF_VCOND_IMM_ALL (<=, 32, _le6)
DEF_VCOND_IMM_ALL (==, 32, _eq6)
DEF_VCOND_IMM_ALL (!=, 32, _ne6)

/* { dg-final { scan-assembler {\tsel\tz[0-9]+.b, p[0-7], z[0-9]+\.b, z[0-9]+\.b\n} } } */
/* { dg-final { scan-assembler {\tsel\tz[0-9]+.h, p[0-7], z[0-9]+\.h, z[0-9]+\.h\n} } } */
/* { dg-final { scan-assembler {\tsel\tz[0-9]+.s, p[0-7], z[0-9]+\.s, z[0-9]+\.s\n} } } */
/* { dg-final { scan-assembler {\tsel\tz[0-9]+.d, p[0-7], z[0-9]+\.d, z[0-9]+\.d\n} } } */

/* { dg-final { scan-assembler {\tcmpgt\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, z[0-9]+\.b\n} } } */
/* { dg-final { scan-assembler {\tcmpgt\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, z[0-9]+\.h\n} } } */
/* { dg-final { scan-assembler {\tcmpgt\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, z[0-9]+\.s\n} } } */
/* { dg-final { scan-assembler {\tcmpgt\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, z[0-9]+\.d\n} } } */

/* { dg-final { scan-assembler {\tcmphi\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, z[0-9]+\.b\n} } } */
/* { dg-final { scan-assembler {\tcmphi\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, z[0-9]+\.h\n} } } */
/* { dg-final { scan-assembler {\tcmphi\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, z[0-9]+\.s\n} } } */
/* { dg-final { scan-assembler {\tcmphi\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, z[0-9]+\.d\n} } } */

/* { dg-final { scan-assembler {\tcmphs\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, z[0-9]+\.b\n} } } */
/* { dg-final { scan-assembler {\tcmphs\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, z[0-9]+\.h\n} } } */
/* { dg-final { scan-assembler {\tcmphs\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, z[0-9]+\.s\n} } } */
/* { dg-final { scan-assembler {\tcmphs\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, z[0-9]+\.d\n} } } */

/* { dg-final { scan-assembler {\tcmpge\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, z[0-9]+\.b\n} } } */
/* { dg-final { scan-assembler {\tcmpge\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, z[0-9]+\.h\n} } } */
/* { dg-final { scan-assembler {\tcmpge\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, z[0-9]+\.s\n} } } */
/* { dg-final { scan-assembler {\tcmpge\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, z[0-9]+\.d\n} } } */

/* { dg-final { scan-assembler {\tcmpeq\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, z[0-9]+\.b\n} } } */
/* { dg-final { scan-assembler {\tcmpeq\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, z[0-9]+\.h\n} } } */
/* { dg-final { scan-assembler {\tcmpeq\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, z[0-9]+\.s\n} } } */
/* { dg-final { scan-assembler {\tcmpeq\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, z[0-9]+\.d\n} } } */

/* { dg-final { scan-assembler {\tcmpne\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, z[0-9]+\.b\n} } } */
/* { dg-final { scan-assembler {\tcmpne\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, z[0-9]+\.h\n} } } */
/* { dg-final { scan-assembler {\tcmpne\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, z[0-9]+\.s\n} } } */
/* { dg-final { scan-assembler {\tcmpne\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, z[0-9]+\.d\n} } } */



/* { dg-final { scan-assembler {\tcmpgt\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, #15\n} } } */
/* { dg-final { scan-assembler {\tcmpgt\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, #15\n} } } */
/* { dg-final { scan-assembler {\tcmpgt\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, #15\n} } } */
/* { dg-final { scan-assembler {\tcmpgt\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, #15\n} } } */

/* { dg-final { scan-assembler {\tcmplt\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, #15\n} } } */
/* { dg-final { scan-assembler {\tcmplt\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, #15\n} } } */
/* { dg-final { scan-assembler {\tcmplt\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, #15\n} } } */
/* { dg-final { scan-assembler {\tcmplt\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, #15\n} } } */

/* { dg-final { scan-assembler {\tcmpge\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, #15\n} } } */
/* { dg-final { scan-assembler {\tcmpge\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, #15\n} } } */
/* { dg-final { scan-assembler {\tcmpge\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, #15\n} } } */
/* { dg-final { scan-assembler {\tcmpge\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, #15\n} } } */

/* { dg-final { scan-assembler {\tcmple\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, #15\n} } } */
/* { dg-final { scan-assembler {\tcmple\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, #15\n} } } */
/* { dg-final { scan-assembler {\tcmple\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, #15\n} } } */
/* { dg-final { scan-assembler {\tcmple\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, #15\n} } } */

/* { dg-final { scan-assembler {\tcmpeq\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, #15\n} } } */
/* { dg-final { scan-assembler {\tcmpeq\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, #15\n} } } */
/* { dg-final { scan-assembler {\tcmpeq\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, #15\n} } } */
/* { dg-final { scan-assembler {\tcmpeq\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, #15\n} } } */

/* { dg-final { scan-assembler {\tcmpne\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, #15\n} } } */
/* { dg-final { scan-assembler {\tcmpne\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, #15\n} } } */
/* { dg-final { scan-assembler {\tcmpne\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, #15\n} } } */
/* { dg-final { scan-assembler {\tcmpne\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, #15\n} } } */

/* { dg-final { scan-assembler {\tcmpgt\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, #-16\n} } } */
/* { dg-final { scan-assembler {\tcmpgt\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, #-16\n} } } */
/* { dg-final { scan-assembler {\tcmpgt\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, #-16\n} } } */
/* { dg-final { scan-assembler {\tcmpgt\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, #-16\n} } } */

/* { dg-final { scan-assembler {\tcmplt\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, #-16\n} } } */
/* { dg-final { scan-assembler {\tcmplt\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, #-16\n} } } */
/* { dg-final { scan-assembler {\tcmplt\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, #-16\n} } } */
/* { dg-final { scan-assembler {\tcmplt\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, #-16\n} } } */

/* { dg-final { scan-assembler {\tcmpge\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, #-16\n} } } */
/* { dg-final { scan-assembler {\tcmpge\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, #-16\n} } } */
/* { dg-final { scan-assembler {\tcmpge\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, #-16\n} } } */
/* { dg-final { scan-assembler {\tcmpge\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, #-16\n} } } */

/* { dg-final { scan-assembler {\tcmple\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, #-16\n} } } */
/* { dg-final { scan-assembler {\tcmple\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, #-16\n} } } */
/* { dg-final { scan-assembler {\tcmple\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, #-16\n} } } */
/* { dg-final { scan-assembler {\tcmple\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, #-16\n} } } */

/* { dg-final { scan-assembler {\tcmpeq\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, #-16\n} } } */
/* { dg-final { scan-assembler {\tcmpeq\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, #-16\n} } } */
/* { dg-final { scan-assembler {\tcmpeq\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, #-16\n} } } */
/* { dg-final { scan-assembler {\tcmpeq\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, #-16\n} } } */

/* { dg-final { scan-assembler {\tcmpne\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, #-16\n} } } */
/* { dg-final { scan-assembler {\tcmpne\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, #-16\n} } } */
/* { dg-final { scan-assembler {\tcmpne\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, #-16\n} } } */
/* { dg-final { scan-assembler {\tcmpne\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, #-16\n} } } */



/* { dg-final { scan-assembler {\tcmphi\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, #0\n} } } */
/* { dg-final { scan-assembler {\tcmphi\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, #0\n} } } */
/* { dg-final { scan-assembler {\tcmphi\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, #0\n} } } */
/* { dg-final { scan-assembler {\tcmphi\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, #0\n} } } */

/* { dg-final { scan-assembler {\tcmpls\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, #0\n} } } */
/* { dg-final { scan-assembler {\tcmpls\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, #0\n} } } */
/* { dg-final { scan-assembler {\tcmpls\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, #0\n} } } */
/* { dg-final { scan-assembler {\tcmpls\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, #0\n} } } */


/* { dg-final { scan-assembler {\tcmphi\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, #31\n} } } */
/* { dg-final { scan-assembler {\tcmphi\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, #31\n} } } */
/* { dg-final { scan-assembler {\tcmphi\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, #31\n} } } */
/* { dg-final { scan-assembler {\tcmphi\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, #31\n} } } */

/* { dg-final { scan-assembler {\tcmplo\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, #31\n} } } */
/* { dg-final { scan-assembler {\tcmplo\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, #31\n} } } */
/* { dg-final { scan-assembler {\tcmplo\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, #31\n} } } */
/* { dg-final { scan-assembler {\tcmplo\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, #31\n} } } */

/* { dg-final { scan-assembler {\tcmphs\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, #31\n} } } */
/* { dg-final { scan-assembler {\tcmphs\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, #31\n} } } */
/* { dg-final { scan-assembler {\tcmphs\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, #31\n} } } */
/* { dg-final { scan-assembler {\tcmphs\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, #31\n} } } */

/* { dg-final { scan-assembler {\tcmpls\tp[0-7].b, p[0-7]/z, z[0-9]+\.b, #31\n} } } */
/* { dg-final { scan-assembler {\tcmpls\tp[0-7].h, p[0-7]/z, z[0-9]+\.h, #31\n} } } */
/* { dg-final { scan-assembler {\tcmpls\tp[0-7].s, p[0-7]/z, z[0-9]+\.s, #31\n} } } */
/* { dg-final { scan-assembler {\tcmpls\tp[0-7].d, p[0-7]/z, z[0-9]+\.d, #31\n} } } */

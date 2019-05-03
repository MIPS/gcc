#ifndef TEST_SVE_ACLE_H
#define TEST_SVE_ACLE_H 1

#include <arm_sve.h>

#ifdef CHECK_ASM
#  define DEF_REGISTER(TYPE, NAME) \
     register TYPE NAME asm (#NAME)
#  define BIND_INPUT(TYPE, NAME, CONSTRAINT) \
     DEF_REGISTER (TYPE, NAME); \
     asm volatile ("" : "=" CONSTRAINT (NAME))
#  define BIND_OUTPUT(NAME, CONSTRAINT) \
     asm volatile ("" :: CONSTRAINT (NAME))
#  define DECLARE_RESULT(TYPE, ZN) \
     register TYPE ZN##_res asm (#ZN)
#else
#  define DEF_REGISTER(TYPE, NAME) TYPE NAME
#  define BIND_INPUT(TYPE, NAME, CONSTRAINT) DEF_REGISTER (TYPE, NAME)
#  define BIND_OUTPUT(NAME, CONSTRAINT) (void) NAME
#  define DECLARE_RESULT(TYPE, ZN) TYPE ZN##_res
#endif

#define BIND_INPUT_Z(TYPE, ZN) BIND_INPUT (TYPE, ZN, "w");
#define BIND_INPUT_P(PN) BIND_INPUT (svbool_t, PN, "Upa");
#define BIND_INPUT_X(TYPE, XN) BIND_INPUT (TYPE, XN, "r");
#define BIND_INPUT_D(TYPE, DN) BIND_INPUT (TYPE, DN, "w");

#define BIND_OUTPUT_Z(ZN) BIND_OUTPUT (ZN, "w")
#define BIND_OUTPUT_P(PN) BIND_OUTPUT (PN, "Upa")
#define BIND_OUTPUT_X(XN) BIND_OUTPUT (XN, "r")
#define BIND_OUTPUT_D(DN) BIND_OUTPUT (DN, "w")

#if defined (TEST_OVERLOADS)
#define INVOKE(CODE1, CODE2) CODE2
#elif defined (TEST_FULL)
#define INVOKE(CODE1, CODE2) CODE1
#else
#error "Please define -DTEST_OVERLOADS or -DTEST_FULL"
#endif

#define BIND_INPUT_Z0_Z3(TYPE)			\
  BIND_INPUT_Z (TYPE, z0);			\
  BIND_INPUT_Z (TYPE, z1);			\
  BIND_INPUT_Z (TYPE, z2);			\
  BIND_INPUT_Z (TYPE, z3)

#define BIND_INPUT_Z0_Z4(TYPE)			\
  BIND_INPUT_Z0_Z3 (TYPE);			\
  BIND_INPUT_Z (TYPE, z4)

#define BIND_INPUT_Z16_Z19(TYPE)		\
  BIND_INPUT_Z (TYPE, z16);			\
  BIND_INPUT_Z (TYPE, z17);			\
  BIND_INPUT_Z (TYPE, z18);			\
  BIND_INPUT_Z (TYPE, z19)

#define DECLARE_RESULT_Z16_Z19(TYPE)		\
  DECLARE_RESULT (TYPE, z16);			\
  DECLARE_RESULT (TYPE, z17);			\
  DECLARE_RESULT (TYPE, z18);			\
  DECLARE_RESULT (TYPE, z19)

#define BIND_OUTPUT_Z0_Z3			\
  BIND_OUTPUT_Z (z0);				\
  BIND_OUTPUT_Z (z1);				\
  BIND_OUTPUT_Z (z2);				\
  BIND_OUTPUT_Z (z3)

#define BIND_INPUT_P0_P3			\
  BIND_INPUT_P (p0);				\
  BIND_INPUT_P (p1);				\
  BIND_INPUT_P (p2);				\
  BIND_INPUT_P (p3)

#define BIND_OUTPUT_P0_P3			\
  BIND_OUTPUT_P (p0);				\
  BIND_OUTPUT_P (p1);				\
  BIND_OUTPUT_P (p2);				\
  BIND_OUTPUT_P (p3)

#define BIND_RESULT_Z16_Z19			\
  BIND_OUTPUT_Z (z16_res);			\
  BIND_OUTPUT_Z (z17_res);			\
  BIND_OUTPUT_Z (z18_res);			\
  BIND_OUTPUT_Z (z19_res)

#ifdef __cplusplus
#define PROTO(NAME, RET, ARGS) extern "C" RET NAME ARGS; RET NAME ARGS
#else
#define PROTO(NAME, RET, ARGS) RET NAME ARGS
#endif

#define START(NAME) PROTO (NAME, void, (void))

#define TEST_UNIFORM_Z(NAME, TYPE, CODE1, CODE2)	\
  START (NAME)						\
  {							\
    BIND_INPUT_Z0_Z3 (TYPE);				\
    BIND_INPUT_P (p0);					\
    INVOKE (CODE1, CODE2);				\
    BIND_OUTPUT_Z0_Z3;					\
    BIND_OUTPUT_P (p0);					\
  }

#define TEST_UNIFORM_P(NAME, CODE1, CODE2)		\
  START (NAME)						\
  {							\
    BIND_INPUT_P0_P3;					\
    INVOKE (CODE1, CODE2);				\
    BIND_OUTPUT_P (p0);					\
  }

#define TEST_DUAL_Z(NAME, TYPE1, TYPE2, CODE1, CODE2)	\
  START (NAME)						\
  {							\
    BIND_INPUT_Z0_Z3 (TYPE1);				\
    BIND_INPUT_Z16_Z19 (TYPE2);				\
    DECLARE_RESULT_Z16_Z19 (TYPE1);			\
    BIND_INPUT_P (p0);					\
    INVOKE (CODE1, CODE2);				\
    BIND_OUTPUT_Z0_Z3;					\
    BIND_OUTPUT_P (p0);					\
    BIND_RESULT_Z16_Z19;				\
  }

#define TEST_WIDE_Z(NAME, TYPE, CODE1, CODE2)		\
  TEST_DUAL_Z (NAME, TYPE, svuint64_t, CODE1, CODE2)

#define TEST_UNIFORM_ZS(NAME, ZTYPE, STYPE, CODE1, CODE2)	\
  START (NAME)							\
  {								\
    BIND_INPUT_Z0_Z3 (ZTYPE);					\
    BIND_INPUT_P (p0);						\
    BIND_INPUT_X (STYPE, x0);					\
    BIND_INPUT_D (STYPE, d4);					\
    INVOKE (CODE1, CODE2);					\
    BIND_OUTPUT_Z0_Z3;						\
    BIND_OUTPUT_X (x0);						\
    BIND_OUTPUT_D (d4);						\
    BIND_OUTPUT_P (p0);						\
  }

#define TEST_DUAL_ZS(NAME, ZTYPE1, ZTYPE2, STYPE, CODE1, CODE2)	\
  START (NAME)							\
  {								\
    BIND_INPUT_Z0_Z3 (ZTYPE1);					\
    BIND_INPUT_Z16_Z19 (ZTYPE2);				\
    DECLARE_RESULT_Z16_Z19 (ZTYPE1);				\
    BIND_INPUT_P (p0);						\
    BIND_INPUT_X (STYPE, x0);					\
    BIND_INPUT_D (STYPE, d4);					\
    INVOKE (CODE1, CODE2);					\
    BIND_OUTPUT_Z0_Z3;						\
    BIND_OUTPUT_X (x0);						\
    BIND_OUTPUT_D (d4);						\
    BIND_OUTPUT_P (p0);						\
    BIND_RESULT_Z16_Z19;					\
  }

#define TEST_LOAD(NAME, ZTYPE, STYPE, CODE1, CODE2)	\
  START (NAME)						\
  {							\
    DEF_REGISTER (ZTYPE, z0);				\
    BIND_INPUT_P (p0);					\
    BIND_INPUT_X (const STYPE *, x0);			\
    BIND_INPUT_X (intptr_t, x1);			\
    INVOKE (CODE1, CODE2);				\
    BIND_OUTPUT_Z (z0);					\
  }

#define TEST_LOAD_GATHER_SZ(NAME, RES_TYPE, STYPE, ZTYPE, CODE1, CODE2) \
  START (NAME)							\
  {								\
    DECLARE_RESULT (RES_TYPE, z0);				\
    BIND_INPUT_Z (ZTYPE, z0);					\
    BIND_INPUT_Z (ZTYPE, z1);					\
    BIND_INPUT_P (p0);						\
    BIND_INPUT_X (const STYPE *, x0);				\
    BIND_INPUT_D (const STYPE *, d4);				\
    INVOKE (CODE1, CODE2);					\
    BIND_OUTPUT_Z (z0);						\
  }

#define TEST_LOAD_GATHER_ZS(NAME, RES_TYPE, ZTYPE, CODE1, CODE2) \
  START (NAME)							\
  {								\
    DECLARE_RESULT (RES_TYPE, z0);				\
    BIND_INPUT_Z (ZTYPE, z0);					\
    BIND_INPUT_Z (ZTYPE, z1);					\
    BIND_INPUT_P (p0);						\
    BIND_INPUT_X (int64_t, x0);					\
    BIND_INPUT_D (int64_t, d4);					\
    INVOKE (CODE1, CODE2);					\
    BIND_OUTPUT_Z (z0);						\
  }

#define TEST_STORE(NAME, ZTYPE, STYPE, CODE1, CODE2)	\
  START (NAME)						\
  {							\
    BIND_INPUT_Z (ZTYPE, z0);				\
    BIND_INPUT_P (p0);					\
    BIND_INPUT_X (STYPE *, x0);				\
    BIND_INPUT_X (intptr_t, x1);			\
    INVOKE (CODE1, CODE2);				\
  }

#define TEST_STORE_SCATTER_SZ(NAME, DATA_TYPE, STYPE, ZTYPE, CODE1, CODE2) \
  START (NAME)							\
  {								\
    BIND_INPUT_Z (DATA_TYPE, z0);				\
    BIND_INPUT_Z (ZTYPE, z1);					\
    BIND_INPUT_P (p0);						\
    BIND_INPUT_X (STYPE *, x0);					\
    BIND_INPUT_D (STYPE *, d4);					\
    INVOKE (CODE1, CODE2);					\
  }

#define TEST_STORE_SCATTER_ZS(NAME, DATA_TYPE, ZTYPE, CODE1, CODE2) \
  START (NAME)							\
  {								\
    BIND_INPUT_Z (DATA_TYPE, z0);				\
    BIND_INPUT_Z (ZTYPE, z1);					\
    BIND_INPUT_P (p0);						\
    BIND_INPUT_X (int64_t, x0);					\
    BIND_INPUT_D (int64_t, d4);					\
    INVOKE (CODE1, CODE2);					\
    BIND_OUTPUT_Z (z0);						\
  }

#define TEST_P(NAME, CODE1, CODE2)	\
  START (NAME)				\
  {					\
    BIND_INPUT_P0_P3;			\
    INVOKE (CODE1, CODE2);		\
    BIND_OUTPUT_P0_P3;			\
  }

#define TEST_P_SINGLE(NAME, CODE)	\
  START (NAME)				\
  {					\
    BIND_INPUT_P0_P3;			\
    CODE;				\
    BIND_OUTPUT_P0_P3;			\
  }

#define TEST_PTEST(NAME, TYPE, CODE)	\
  START (NAME)				\
  {					\
    BIND_INPUT_X (TYPE, x0);		\
    BIND_INPUT_X (TYPE, x1);		\
    BIND_INPUT_P0_P3;			\
    INVOKE (CODE, CODE);		\
    BIND_OUTPUT_X (x0);			\
  }

#define TEST_COMPARE_Z(NAME, TYPE, CODE1, CODE2)	\
  START (NAME)						\
  {							\
    BIND_INPUT_Z0_Z3 (TYPE);				\
    BIND_INPUT_P (p0);					\
    BIND_INPUT_P (p1);					\
    INVOKE (CODE1, CODE2);				\
    BIND_OUTPUT_P (p0);					\
  }

#define TEST_COMPARE_ZS(NAME, ZTYPE, STYPE, CODE1, CODE2)	\
  START (NAME)						\
  {							\
    BIND_INPUT_Z0_Z3 (ZTYPE);				\
    BIND_INPUT_P (p0);					\
    BIND_INPUT_P (p1);					\
    BIND_INPUT_X (STYPE, x0);				\
    BIND_INPUT_D (STYPE, d4);				\
    INVOKE (CODE1, CODE2);				\
    BIND_OUTPUT_P (p0);					\
  }

#define TEST_COMPARE_DUAL_Z(NAME, TYPE1, TYPE2, CODE1, CODE2)	\
  START (NAME)						\
  {							\
    BIND_INPUT_Z (TYPE1, z0);				\
    BIND_INPUT_Z (TYPE2, z1);				\
    BIND_INPUT_P (p0);					\
    BIND_INPUT_P (p1);					\
    INVOKE (CODE1, CODE2);				\
    BIND_OUTPUT_P (p0);					\
  }

#define TEST_S(NAME, ZTYPE, STYPE, CODE1)			\
  START (NAME)							\
  {								\
    BIND_INPUT_Z0_Z3 (ZTYPE);					\
    BIND_INPUT_X (STYPE, x0);					\
    BIND_INPUT_X (STYPE, x1);					\
    CODE1;							\
    BIND_OUTPUT_Z0_Z3;						\
    BIND_OUTPUT_X (x0);						\
    BIND_OUTPUT_X (x1);						\
  }

#define TEST_UNDEF(NAME, TYPE, CODE)	\
  START (NAME)				\
  {					\
    DEF_REGISTER (TYPE, z0);		\
    CODE;				\
    BIND_OUTPUT_Z (z0);			\
  }

#define TEST_CREATE(NAME, TTYPE, ZTYPE, CODE1, CODE2)	\
  START (NAME)						\
  {							\
    BIND_INPUT_Z0_Z4 (ZTYPE);				\
    DEF_REGISTER (TTYPE, z16);				\
    INVOKE (CODE1, CODE2);				\
    BIND_OUTPUT_Z (z16);				\
  }

#define TEST_GET(NAME, TTYPE, ZTYPE, CODE1, CODE2)	\
  START (NAME)						\
  {							\
    DEF_REGISTER (ZTYPE, z0);				\
    BIND_INPUT_Z (TTYPE, z16);				\
    DECLARE_RESULT_Z16_Z19 (ZTYPE);			\
    INVOKE (CODE1, CODE2);				\
    BIND_OUTPUT_Z (z0);					\
    BIND_RESULT_Z16_Z19;				\
  }

#define TEST_SET(NAME, TTYPE, ZTYPE, CODE1, CODE2)	\
  START (NAME)						\
  {							\
    DEF_REGISTER (TTYPE, z0);				\
    BIND_INPUT_Z (ZTYPE, z4);				\
    BIND_INPUT_Z (TTYPE, z16);				\
    INVOKE (CODE1, CODE2);				\
    BIND_OUTPUT_Z (z0);					\
    BIND_OUTPUT_Z (z4);					\
    BIND_OUTPUT_Z (z16);				\
  }

#endif

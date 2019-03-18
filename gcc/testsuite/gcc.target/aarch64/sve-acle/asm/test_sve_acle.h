#ifndef TEST_SVE_ACLE_H
#define TEST_SVE_ACLE_H 1

#include <stdint.h>

#pragma GCC aarch64 "arm_sve.h"

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

#define BIND_INPUT_ZS(TYPE)			\
  BIND_INPUT_Z (TYPE, z0);			\
  BIND_INPUT_Z (TYPE, z1);			\
  BIND_INPUT_Z (TYPE, z2);			\
  BIND_INPUT_Z (TYPE, z3);			\
  BIND_INPUT_Z (TYPE, z4)

#define BIND_INPUT_ZS_FROM_Z16(TYPE)		\
  BIND_INPUT_Z (TYPE, z16);			\
  BIND_INPUT_Z (TYPE, z17);			\
  BIND_INPUT_Z (TYPE, z18);			\
  BIND_INPUT_Z (TYPE, z19)

#define DECLARE_RESULT_ZS_FROM_Z16(TYPE)	\
  DECLARE_RESULT (TYPE, z16);			\
  DECLARE_RESULT (TYPE, z17);			\
  DECLARE_RESULT (TYPE, z18);			\
  DECLARE_RESULT (TYPE, z19)

#define BIND_OUTPUT_ZS				\
  BIND_OUTPUT_Z (z0);				\
  BIND_OUTPUT_Z (z1);				\
  BIND_OUTPUT_Z (z2);				\
  BIND_OUTPUT_Z (z3);				\
  BIND_OUTPUT_Z (z4)

#define BIND_INPUT_PS				\
  BIND_INPUT_P (p0);				\
  BIND_INPUT_P (p1);				\
  BIND_INPUT_P (p2);				\
  BIND_INPUT_P (p3);				\
  BIND_INPUT_P (p4)

#define BIND_OUTPUT_PS				\
  BIND_OUTPUT_P (p0);				\
  BIND_OUTPUT_P (p1);				\
  BIND_OUTPUT_P (p2);				\
  BIND_OUTPUT_P (p3);				\
  BIND_OUTPUT_P (p4)

#define BIND_RESULT_ZS_FROM_Z16			\
  BIND_OUTPUT_Z (z16_res);			\
  BIND_OUTPUT_Z (z17_res);			\
  BIND_OUTPUT_Z (z18_res);			\
  BIND_OUTPUT_Z (z19_res)

#ifdef __cplusplus
#define START(NAME) extern "C" void NAME (void); void NAME (void)
#else
#define START(NAME) void NAME (void)
#endif

#define TEST_UNIFORM_Z(NAME, TYPE, CODE1, CODE2)	\
  START (NAME)						\
  {							\
    BIND_INPUT_ZS (TYPE);				\
    BIND_INPUT_P (p0);					\
    INVOKE (CODE1, CODE2);				\
    BIND_OUTPUT_ZS;					\
    BIND_OUTPUT_P (p0);					\
  }

#define TEST_DUAL_Z(NAME, TYPE1, TYPE2, CODE1, CODE2)	\
  START (NAME)						\
  {							\
    BIND_INPUT_ZS (TYPE1);				\
    BIND_INPUT_ZS_FROM_Z16 (TYPE2);			\
    DECLARE_RESULT_ZS_FROM_Z16 (TYPE1);			\
    BIND_INPUT_P (p0);					\
    INVOKE (CODE1, CODE2);				\
    BIND_OUTPUT_ZS;					\
    BIND_OUTPUT_P (p0);					\
    BIND_RESULT_ZS_FROM_Z16;				\
  }

#define TEST_WIDE_Z(NAME, TYPE, CODE1, CODE2)		\
  TEST_DUAL_Z (NAME, TYPE, svuint64_t, CODE1, CODE2)

#define TEST_UNIFORM_ZS(NAME, ZTYPE, STYPE, CODE1, CODE2)	\
  START (NAME)							\
  {								\
    BIND_INPUT_ZS (ZTYPE);					\
    BIND_INPUT_P (p0);						\
    BIND_INPUT_X (STYPE, x0);					\
    BIND_INPUT_D (STYPE, d0);					\
    INVOKE (CODE1, CODE2);					\
    BIND_OUTPUT_ZS;						\
    BIND_OUTPUT_X (x0);						\
    BIND_OUTPUT_D (d0);						\
    BIND_OUTPUT_P (p0);						\
  }

#define TEST_DUAL_ZS(NAME, ZTYPE1, ZTYPE2, STYPE, CODE1, CODE2)	\
  START (NAME)							\
  {								\
    BIND_INPUT_ZS (ZTYPE1);					\
    BIND_INPUT_ZS_FROM_Z16 (ZTYPE2);				\
    DECLARE_RESULT_ZS_FROM_Z16 (ZTYPE1);			\
    BIND_INPUT_P (p0);						\
    BIND_INPUT_X (STYPE, x0);					\
    BIND_INPUT_D (STYPE, d0);					\
    INVOKE (CODE1, CODE2);					\
    BIND_OUTPUT_ZS;						\
    BIND_OUTPUT_X (x0);						\
    BIND_OUTPUT_D (d0);						\
    BIND_OUTPUT_P (p0);						\
    BIND_RESULT_ZS_FROM_Z16;					\
  }

#define TEST_P(NAME, CODE1, CODE2)	\
  START (NAME)				\
  {					\
    BIND_INPUT_PS;			\
    INVOKE (CODE1, CODE2);		\
    BIND_OUTPUT_PS;			\
  }

#define TEST_S(NAME, ZTYPE, STYPE, CODE1)			\
  START (NAME)							\
  {								\
    BIND_INPUT_ZS (ZTYPE);					\
    BIND_INPUT_X (STYPE, x0);					\
    BIND_INPUT_X (STYPE, x1);					\
    CODE1;							\
    BIND_OUTPUT_ZS;						\
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

#define TEST_GET(NAME, TTYPE, ZTYPE, CODE1, CODE2)	\
  START (NAME)						\
  {							\
    DEF_REGISTER (ZTYPE, z0);				\
    BIND_INPUT_Z (TTYPE, z16);				\
    DECLARE_RESULT_ZS_FROM_Z16 (ZTYPE);			\
    INVOKE (CODE1, CODE2);				\
    BIND_OUTPUT_Z (z0);					\
    BIND_RESULT_ZS_FROM_Z16;				\
  }

#endif

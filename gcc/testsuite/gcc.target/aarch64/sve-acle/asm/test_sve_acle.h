#ifndef TEST_SVE_ACLE_H
#define TEST_SVE_ACLE_H 1

#include <stdint.h>

#pragma GCC aarch64 "arm_sve.h"

#ifdef CHECK_ASM
#  define BIND_INPUT_Z(TYPE, ZN) \
     register TYPE ZN asm (#ZN); \
     asm volatile ("" : "=w" (ZN))
#  define BIND_INPUT_P(PN) \
     register svbool_t PN asm (#PN); \
     asm volatile ("" : "=Upa" (PN))
#  define BIND_INPUT_X(TYPE, XN) \
     register TYPE XN asm (#XN); \
     asm volatile ("" : "=r" (XN))
#  define BIND_INPUT_D(TYPE, DN) \
     register TYPE DN asm (#DN); \
     asm volatile ("" : "=w" (DN))
#  define BIND_OUTPUT_Z(ZN) \
     asm volatile ("" :: "w" (ZN))
#  define BIND_OUTPUT_P(PN) \
     asm volatile ("" :: "Upa" (PN))
#  define BIND_OUTPUT_X(XN) \
     asm volatile ("" :: "r" (XN))
#  define BIND_OUTPUT_D(DN) \
     asm volatile ("" :: "w" (DN))
#else
#  define BIND_INPUT_Z(TYPE, ZN) TYPE ZN
#  define BIND_INPUT_P(PN) svbool_t PN
#  define BIND_INPUT_X(TYPE, XN) TYPE XN
#  define BIND_INPUT_D(TYPE, DN) TYPE DN
#  define BIND_OUTPUT_Z(ZN) (void) ZN
#  define BIND_OUTPUT_P(PN) (void) PN
#  define BIND_OUTPUT_X(XN) (void) XN
#  define BIND_OUTPUT_D(DN) (void) DN
#endif

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

#endif

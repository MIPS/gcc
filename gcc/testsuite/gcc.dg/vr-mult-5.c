/* Check that 32-bit vr4120 code can use macc for DI = SI * SI + DI
   operations.  We could use it in 64-bit code too, but it generally
   produces worse code.  */

/* { dg-do compile { target mips*-*-* } } */
/* { dg-options "-march=vr4120 -mabi=32 -O2" } */
#if _MIPS_ARCH_VR4120 && !__mips64

#define CHECK(WIDE, NARROW, NAME)	\
  WIDE NAME (NARROW *x)			\
  {					\
    return ((WIDE) x[0] * x[1]		\
	    + (WIDE) x[2] * x[3]	\
	    + (WIDE) x[4] * x[5]);	\
  }

CHECK (long long, int, f1)
CHECK (unsigned long long, unsigned int, f2)
#else
const char *foo = "f1:macc//f2:maccu";
#endif
/* { dg-final { scan-assembler "f1.*macc.*f2.*maccu" } } */

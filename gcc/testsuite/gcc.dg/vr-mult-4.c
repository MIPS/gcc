/* Like vr-mult-3.c, but check that the result of a macc is not moved
   into a general register using MFLO.  */

/* { dg-do compile { target mips*-*-* } } */
/* { dg-options "-march=vr4120 -O2" } */
#if _MIPS_ARCH_VR4120

#define CHECK(TYP, NAME)	\
  TYP NAME (TYP *x)		\
  {				\
    return (x[0] * x[1]		\
	    + x[2] * x[3]	\
	    + x[4] * x[5]);	\
  }

CHECK (int, f1)
CHECK (unsigned int, f2)
#endif
/* { dg-final { scan-assembler-not "mflo" } } */

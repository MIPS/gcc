/* Check that macc isn't used gratuitously in vr5400 code.  A mul/add
   sequence is better than a mtlo/macc one.  */

/* { dg-do compile { target mips*-*-* } } */
/* { dg-options "-march=vr5400 -O2" } */
#if _MIPS_ARCH_VR5400

#define CHECK(TYP, NAME)		\
  TYP NAME (TYP x, TYP y, TYP z)	\
  {					\
    return (x * y + z);			\
  }

CHECK (int, f1)
CHECK (unsigned int, f2)
#endif
/* { dg-final { scan-assembler-not "macc" } } */

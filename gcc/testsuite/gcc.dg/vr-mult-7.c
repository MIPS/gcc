/* Check that madd and msub are used instead of macc and msac when the
   result is only needed in LO.  */

/* { dg-do compile { target mips*-*-* } } */
/* { dg-options "-march=vr5500 -O2" } */
#if _MIPS_ARCH_VR5500

#define CHECK1(TYP, NAME, OP)		\
  TYP NAME (TYP *dx, TYP a, int n)	\
  {					\
    int result, i;			\
					\
    result = 0;				\
    for (i = 0; i < n; i += 4)		\
      {					\
        result OP a * dx[i];		\
        result OP a * dx[i + 1];	\
        result OP a * dx[i + 2];	\
        result OP a * dx[i + 3];	\
      }					\
    return result;			\
  }

CHECK1 (int, f1, +=)
CHECK1 (int, f2, -=)
CHECK1 (unsigned int, f3, +=)
CHECK1 (unsigned int, f4, -=)

#define CHECK2(TYP, NAME, OP)		\
  TYP NAME (TYP x, TYP y, TYP z)	\
  {					\
    return (x * y OP z);		\
  }

CHECK2 (int, f5, +)
CHECK2 (int, f6, -)
CHECK2 (unsigned int, f7, +)
CHECK2 (unsigned int, f8, -)

#define CHECK3(TYP, NAME, OP)	\
  TYP NAME (TYP *x)		\
  {				\
    return (x[0] * x[1]		\
	    OP x[2] * x[3]	\
	    OP x[4] * x[5]);	\
  }

CHECK3 (int, f9, +)
CHECK3 (int, f10, -)
CHECK3 (unsigned int, f11, +)
CHECK3 (unsigned int, f12, -)

#endif
/* { dg-final { scan-assembler-not "macc\t\\\$0" } } */
/* { dg-final { scan-assembler-not "msac\t\\\$0" } } */

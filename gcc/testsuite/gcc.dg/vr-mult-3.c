/* Like vr-mult-2.c, but for vr4120 code.  */

/* { dg-do compile { target mips*-*-* } } */
/* { dg-options "-march=vr4120 -O2" } */
#if _MIPS_ARCH_VR4120

#define CHECK1(TYP, NAME)		\
  TYP NAME (TYP *dx, TYP a, int n)	\
  {					\
    TYP result;				\
    int i;				\
					\
    result = 0;				\
    for (i = 0; i < n; i += 4)		\
      {					\
        result += a * dx[i];		\
        result += a * dx[i + 1];	\
        result += a * dx[i + 2];	\
        result += a * dx[i + 3];	\
      }					\
    return result;			\
  }

CHECK1 (int, f1)
CHECK1 (unsigned int, f2)

/* We should use mtlo/macc rather than mult/mflo/add.  */
#define CHECK2(TYP, NAME)		\
  TYP NAME (TYP x, TYP y, TYP z)	\
  {					\
    return (x * y + z);			\
  }

CHECK2 (int, f3)
CHECK2 (unsigned int, f4)
#else
const char *foo = "f1:macc//f2:macc//f3:macc//f4:macc";
#endif
/* { dg-final { scan-assembler "f1.*macc.*f2.*macc.*f3.*macc.*f4.*macc" } } */

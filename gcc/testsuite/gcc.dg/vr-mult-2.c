/* Like vr-mult-1.c, but check the macc & msac patterns.  */

/* { dg-do compile { target mips*-*-* } } */
/* { dg-options "-march=vr5400 -O2" } */
#if _MIPS_ARCH_VR5400

#define CHECK(TYP, NAME, OP)		\
  TYP NAME (TYP *dx, TYP a, int n)	\
  {					\
    TYP result;				\
    int i;				\
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

CHECK (int, f1, +=)
CHECK (int, f2, -=)
CHECK (unsigned int, f3, +=)
CHECK (unsigned int, f4, -=)
#else
const char *foo = "f1:macc//f2:msac//f3:macc//f4:msac";
#endif
/* { dg-final { scan-assembler "f1.*macc.*f2.*msac.*f3.*macc.*f4.*msac" } } */

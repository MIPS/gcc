/* { dg-do compile { target { powerpc*-*-* } } } */
/* { dg-skip-if "" { powerpc*-*-darwin* } { "*" } { "" } } */
/* { dg-require-effective-target powerpc_vsx_ok } */
/* { dg-options "-O2 -mcpu=power7" } */

#include <altivec.h>

#define TEST_COPY(NAME, TYPE)						\
void NAME ## _copy_native (vector TYPE *a, vector TYPE *b)		\
{									\
  *a = *b;								\
}									\
									\
void NAME ## _copy_vec (vector TYPE *a, vector TYPE *b)			\
{									\
  vector TYPE x = vec_ld (0, b);					\
  vec_st (x, 0, a);							\
}									\

#define TEST_COPYL(NAME, TYPE)						\
void NAME ## _lvxl (vector TYPE *a, vector TYPE *b)			\
{									\
  vector TYPE x = vec_ldl (0, b);					\
  vec_stl (x, 0, a);							\
}									\

#define TEST_ALIGN(NAME, TYPE)						\
void NAME ## _align (vector unsigned char *a, TYPE *b)			\
{									\
  vector unsigned char x = vec_lvsl (0, b);				\
  vector unsigned char y = vec_lvsr (0, b);				\
  vec_st (x, 0, a);							\
  vec_st (y, 8, a);							\
}

TEST_COPY(uchar,  unsigned char)
TEST_COPY(schar,  signed   char)
TEST_COPY(bchar,  bool     char)
TEST_COPY(ushort, unsigned short)
TEST_COPY(sshort, signed   short)
TEST_COPY(bshort, bool     short)
TEST_COPY(uint,   unsigned int)
TEST_COPY(sint,   signed   int)
TEST_COPY(bint,   bool     int)
TEST_COPY(float,  float)
#ifdef __VSX__
TEST_COPY(ullong, unsigned long long)
TEST_COPY(sllong, signed   long long)
TEST_COPY(bllong, bool     long long)
TEST_COPY(double, double)
#endif	/* __VSX__ */

TEST_COPYL(uchar,  unsigned char)
TEST_COPYL(schar,  signed   char)
TEST_COPYL(bchar,  bool     char)
TEST_COPYL(ushort, unsigned short)
TEST_COPYL(sshort, signed   short)
TEST_COPYL(bshort, bool     short)
TEST_COPYL(uint,   unsigned int)
TEST_COPYL(sint,   signed   int)
TEST_COPYL(bint,   bool     int)
TEST_COPYL(float,  float)
#ifdef __VSX__
TEST_COPYL(ullong, unsigned long long)
TEST_COPYL(sllong, signed   long long)
TEST_COPYL(bllong, bool     long long)
TEST_COPYL(double, double)
#endif	/* __VSX__ */

TEST_ALIGN(uchar,  unsigned char)
TEST_ALIGN(schar,  signed   char)
TEST_ALIGN(ushort, unsigned short)
TEST_ALIGN(sshort, signed   short)
TEST_ALIGN(uint,   unsigned int)
TEST_ALIGN(sint,   signed   int)
TEST_ALIGN(float,  float)
#ifdef __VSX__
TEST_ALIGN(ullong, unsigned long long)
TEST_ALIGN(sllong, signed   long long)
TEST_ALIGN(double, double)
#endif	/* __VSX__ */

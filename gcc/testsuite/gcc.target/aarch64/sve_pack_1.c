/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize -fno-inline -march=armv8-a+sve" } */

#define PACK(TYPED, TYPES, SIGN)				\
void pack_##TYPED##_##TYPES##_##SIGN (SIGN TYPED *d,		\
				      SIGN TYPES *s, int size)	\
{								\
  for (int i = 0; i < size; i++)				\
    d[i] = s[i] + 1;						\
}

PACK (int, long, signed)			\
PACK (short, int, signed)			\
PACK (char, short, signed)			\
PACK (int, long, unsigned)			\
PACK (short, int, unsigned)			\
PACK (char, short, unsigned)

/* { dg-final { scan-assembler-times {\tuzp1\tz[0-9]+\.s, z[0-9]+\.s, z[0-9]+\.s\n} 2 } } */
/* { dg-final { scan-assembler-times {\tuzp1\tz[0-9]+\.h, z[0-9]+\.h, z[0-9]+\.h\n} 2 } } */
/* { dg-final { scan-assembler-times {\tuzp1\tz[0-9]+\.b, z[0-9]+\.b, z[0-9]+\.b\n} 2 } } */

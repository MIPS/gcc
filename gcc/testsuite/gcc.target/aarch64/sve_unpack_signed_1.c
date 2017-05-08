/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize -fno-inline -march=armv8-a+sve" } */

#define UNPACK(TYPED, TYPES, SIGND)					\
void unpack_##TYPED##_##TYPES##_##SIGND (SIGND TYPED *d, signed TYPES *s, \
					 int size)			\
{									\
   for (int i = 0; i < size; i++)					\
     d[i] = s[i] + 1;							\
}

UNPACK (long, int, signed)
UNPACK (int, short, signed)
UNPACK (short, char, signed)
UNPACK (long, int, unsigned)
UNPACK (int, short, unsigned)
UNPACK (short, char, unsigned)

/* { dg-final { scan-assembler-times {\tsunpkhi\tz[0-9]+\.d, z[0-9]+\.s\n} 2 } } */
/* { dg-final { scan-assembler-times {\tsunpkhi\tz[0-9]+\.s, z[0-9]+\.h\n} 2 } } */
/* { dg-final { scan-assembler-times {\tsunpkhi\tz[0-9]+\.h, z[0-9]+\.b\n} 2 } } */

/* { dg-final { scan-assembler-times {\tsunpklo\tz[0-9]+\.d, z[0-9]+\.s\n} 2 } } */
/* { dg-final { scan-assembler-times {\tsunpklo\tz[0-9]+\.s, z[0-9]+\.h\n} 2 } } */
/* { dg-final { scan-assembler-times {\tsunpklo\tz[0-9]+\.h, z[0-9]+\.b\n} 2 } } */

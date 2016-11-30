/* { dg-do assemble } */
/* { dg-options "-O2 -ftree-vectorize -march=armv8-a+sve --save-temps" } */

void gather_loadu64_s16(unsigned long * restrict dst, unsigned long * restrict src,
			short int * restrict indices, short n)
{
  for (short i=0; i<n; i++)
    dst[i] = src[indices[i]];
}

void gather_loadu64_u16(unsigned long * restrict dst, unsigned long * restrict src,
			unsigned short int * restrict indices, short n)
{
  for (short i=0; i<n; i++)
    dst[i] = src[indices[i]];
}

void gather_loadd_s16(double * restrict dst, double * restrict src,
		      short * restrict indices, short n)
{
  for (short i=0; i<n; i++)
    dst[i] = src[indices[i]];
}

void gather_loadd_u16(double * restrict dst, double * restrict src,
		      unsigned short * restrict indices, short n)
{
  for (short i=0; i<n; i++)
    dst[i] = src[indices[i]];
}

void gather_loadu64_s32(unsigned long * restrict dst, unsigned long * restrict src,
			int * restrict indices, int n)
{
  for (int i=0; i<n; i++)
    dst[i] = src[indices[i]];
}

void gather_loadu64_u32(unsigned long * restrict dst, unsigned long * restrict src,
			unsigned int * restrict indices, int n)
{
  for (int i=0; i<n; i++)
    dst[i] = src[indices[i]];
}

void gather_loadd_s32(double * restrict dst, double * restrict src,
		      int * restrict indices, int n)
{
  for (int i=0; i<n; i++)
    dst[i] = src[indices[i]];
}

void gather_loadd_u32(double * restrict dst, double * restrict src,
		      unsigned int * restrict indices, int n)
{
  for (int i=0; i<n; i++)
    dst[i] = src[indices[i]];
}

/* { dg-final { scan-assembler-times {\tpunpklo\tp[0-9]+\.h, p[0-9]+\.b} 16 } } */
/* { dg-final { scan-assembler-times {\tpunpkhi\tp[0-9]+\.h, p[0-9]+\.b} 16 } } */

/* { dg-final { scan-assembler-times {\tsunpklo\tz[0-9]+\.s, z[0-9]+\.h} 2 } } */
/* { dg-final { scan-assembler-times {\tsunpkhi\tz[0-9]+\.s, z[0-9]+\.h} 2 } } */
/* { dg-final { scan-assembler-times {\tsunpklo\tz[0-9]+\.d, z[0-9]+\.s} 6 } } */
/* { dg-final { scan-assembler-times {\tsunpkhi\tz[0-9]+\.d, z[0-9]+\.s} 6 } } */
/* { dg-final { scan-assembler-times {\tuunpklo\tz[0-9]+\.s, z[0-9]+\.h} 2 } } */
/* { dg-final { scan-assembler-times {\tuunpkhi\tz[0-9]+\.s, z[0-9]+\.h} 2 } } */
/* { dg-final { scan-assembler-times {\tuunpklo\tz[0-9]+\.d, z[0-9]+\.s} 6 } } */
/* { dg-final { scan-assembler-times {\tuunpkhi\tz[0-9]+\.d, z[0-9]+\.s} 6 } } */
/* { dg-final { scan-assembler-times {\tld1d\tz[0-9]+.d, p[0-7]/z, \[x[0-9]+, z[0-9]+.d, lsl 3\]} 24 } } */

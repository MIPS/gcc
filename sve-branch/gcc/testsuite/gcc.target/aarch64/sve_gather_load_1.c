/* { dg-do assemble } */
/* { dg-options "-O3 -march=armv8-a+sve --save-temps" } */

void gather_load64(unsigned long * restrict dst, unsigned long * restrict src, unsigned long * restrict indices, int count)
{
  for (int i=0; i<count; i++)
    dst[i] = src[indices[i]];
}

void gather_load32(unsigned int * restrict dst, unsigned int * restrict src, unsigned int * restrict indices, int count)
{
  for (int i=0; i<count; i++)
    dst[i] = src[indices[i]];
}

void gather_load16(unsigned short * restrict dst, unsigned short * restrict src, unsigned short * restrict indices, int count)
{
  for (int i=0; i<count; i++)
    dst[i] = src[indices[i]];
}

void gather_load8(unsigned char * restrict dst, unsigned char * restrict src, unsigned char * restrict indices, int count)
{
  for (int i=0; i<count; i++)
    dst[i] = src[indices[i]];
}

void gather_load64s(signed long * restrict dst, signed long * restrict src, unsigned long * restrict indices, int count)
{
  for (int i=0; i<count; i++)
    dst[i] = src[indices[i]];
}

void gather_load32s(signed int * restrict dst, signed int * restrict src, unsigned int * restrict indices, int count)
{
  for (int i=0; i<count; i++)
    dst[i] = src[indices[i]];
}

void gather_load16s(signed short * restrict dst, signed short * restrict src, unsigned short * restrict indices, int count)
{
  for (int i=0; i<count; i++)
    dst[i] = src[indices[i]];
}

void gather_load8s(signed char * restrict dst, signed char * restrict src, unsigned char * restrict indices, int count)
{
  for (int i=0; i<count; i++)
    dst[i] = src[indices[i]];
}

void gather_load_double(double * restrict dst, double * restrict src, unsigned long * restrict indices, int count)
{
  for (int i=0; i<count; i++)
    dst[i] = src[indices[i]];
}

void gather_load_float(float * restrict dst, float * restrict src, unsigned int * restrict indices, int count)
{
  for (int i=0; i<count; i++)
    dst[i] = src[indices[i]];
}

/* { dg-final { scan-assembler-times "ld1d\\tz\[0-9\]+.d, p\[0-9\]+/z, \\\[x\[0-9\]+, z\[0-9\]+.d, lsl 3\\\]" 3 } } */
/* { dg-final { scan-assembler-not "ld1d\\tz\[0-9\]+.s, p\[0-9\]+/z, \\\[x\[0-9\]+, z\[0-9\]+.s, sxtw 3\\\]" } } */
/* { dg-final { scan-assembler-not "ld1w\\tz\[0-9\]+.d, p\[0-9\]+/z, \\\[x\[0-9\]+, z\[0-9\]+.d, sxtw 2\\\]" } } */
/* { dg-final { scan-assembler-times "ld1w\\tz\[0-9\]+.s, p\[0-9\]+/z, \\\[x\[0-9\]+, z\[0-9\]+.s, uxtw 2\\\]" 3 } } */
/* { dg-final { scan-assembler-not "ld1w\\tz\[0-9\]+.s, p\[0-9\]+/z, \\\[x\[0-9\]+, z\[0-9\]+.s, sxtw 2\\\]" } } */
/* { dg-final { scan-assembler-not "ld1h\\tz\[0-9\]+.d, p\[0-9\]+/z, \\\[x\[0-9\]+, z\[0-9\]+.d, sxtw 1\\\]" } } */
/* { dg-final { scan-assembler-not "ld1h\\tz\[0-9\]+.s, p\[0-9\]+/z, \\\[x\[0-9\]+, z\[0-9\]+.s, sxtw 1\\\]" } } */
/* { dg-final { scan-assembler-not "ld1b\\tz\[0-9\]+.d, p\[0-9\]+/z, \\\[x\[0-9\]+, z\[0-9\]+.d, sxtw\\\ ]" } } */
/* { dg-final { scan-assembler-not "ld1b\\tz\[0-9\]+.s, p\[0-9\]+/z, \\\[x\[0-9\]+, z\[0-9\]+.s, sxtw\\\ ]" } } */

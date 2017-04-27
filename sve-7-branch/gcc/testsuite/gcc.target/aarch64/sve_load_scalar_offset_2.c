/* { dg-do assemble } */
/* { dg-options "-O3 -march=armv8-a+sve -msve-vector-bits=256 --save-temps" } */

typedef long v4di __attribute__((vector_size(32)));
typedef int v8si __attribute__((vector_size(32)));
typedef short v16hi __attribute__((vector_size(32)));
typedef char v32qi __attribute__((vector_size(32)));

void sve_load_64_u_lsl (unsigned long *a)
{
  register unsigned long i asm("x1");
  asm volatile ("" : "=r" (i));
  asm volatile ("" :: "w" (*(v4di *)&a[i]));
}

void sve_load_64_s_lsl (signed long *a)
{
  register long i asm("x1");
  asm volatile ("" : "=r" (i));
  asm volatile ("" :: "w" (*(v4di *)&a[i]));
}

void sve_load_32_u_lsl (unsigned int *a)
{
  register unsigned long i asm("x1");
  asm volatile ("" : "=r" (i));
  asm volatile ("" :: "w" (*(v8si *)&a[i]));
}

void sve_load_32_s_lsl (signed int *a)
{
  register long i asm("x1");
  asm volatile ("" : "=r" (i));
  asm volatile ("" :: "w" (*(v8si *)&a[i]));
}

void sve_load_16_z_lsl (unsigned short *a)
{
  register unsigned long i asm("x1");
  asm volatile ("" : "=r" (i));
  asm volatile ("" :: "w" (*(v16hi *)&a[i]));
}

void sve_load_16_s_lsl (signed short *a)
{
  register long i asm("x1");
  asm volatile ("" : "=r" (i));
  asm volatile ("" :: "w" (*(v16hi *)&a[i]));
}

void sve_load_8_z (unsigned char *a)
{
  register unsigned long i asm("x1");
  asm volatile ("" : "=r" (i));
  asm volatile ("" :: "w" (*(v32qi *)&a[i]));
}

void sve_load_8_s (signed char *a)
{
  register long i asm("x1");
  asm volatile ("" : "=r" (i));
  asm volatile ("" :: "w" (*(v32qi *)&a[i]));
}

/* { dg-final { scan-assembler-times "ld1d\\tz0.d, p\[0-9\]+/z, \\\[x0, x1, lsl 3\\\]" 2 } } */
/* { dg-final { scan-assembler-times "ld1w\\tz0.s, p\[0-9\]+/z, \\\[x0, x1, lsl 2\\\]" 2 } } */
/* { dg-final { scan-assembler-times "ld1h\\tz0.h, p\[0-9\]+/z, \\\[x0, x1, lsl 1\\\]" 2 } } */
/* { dg-final { scan-assembler-times "ld1b\\tz0.b, p\[0-9\]+/z, \\\[x0, x1\\\]" 2 } } */

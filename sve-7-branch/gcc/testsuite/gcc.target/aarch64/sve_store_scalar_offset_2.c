/* { dg-do assemble } */
/* { dg-options "-O3 -march=armv8-a+sve -msve-vector-bits=256 --save-temps" } */

typedef long v4di __attribute__((vector_size(32)));
typedef int v8si __attribute__((vector_size(32)));
typedef short v16hi __attribute__((vector_size(32)));
typedef char v32qi __attribute__((vector_size(32)));

void sve_store_64_z_lsl (unsigned long *a, unsigned long i)
{
  asm volatile ("" : "=w" (*(v4di *)&a[i]));
}

void sve_store_64_s_lsl (signed long *a, signed long i)
{
  asm volatile ("" : "=w" (*(v4di *)&a[i]));
}

void sve_store_32_z_lsl (unsigned int *a, unsigned long i)
{
  asm volatile ("" : "=w" (*(v8si *)&a[i]));
}

void sve_store_32_s_lsl (signed int *a, signed long i)
{
  asm volatile ("" : "=w" (*(v8si *)&a[i]));
}

void sve_store_16_z_lsl (unsigned short *a, unsigned long i)
{
  asm volatile ("" : "=w" (*(v16hi *)&a[i]));
}

void sve_store_16_s_lsl (signed short *a, signed long i)
{
  asm volatile ("" : "=w" (*(v16hi *)&a[i]));
}

/* ??? The other argument order leads to a redundant move.  */
void sve_store_8_z (unsigned long i, unsigned char *a)
{
  asm volatile ("" : "=w" (*(v32qi *)&a[i]));
}

void sve_store_8_s (signed long i, signed char *a)
{
  asm volatile ("" : "=w" (*(v32qi *)&a[i]));
}

/* { dg-final { scan-assembler-times "st1d\\tz0.d, p\[0-9\]+, \\\[x0, x1, lsl 3\\\]" 2 } } */
/* { dg-final { scan-assembler-times "st1w\\tz0.s, p\[0-9\]+, \\\[x0, x1, lsl 2\\\]" 2 } } */
/* { dg-final { scan-assembler-times "st1h\\tz0.h, p\[0-9\]+, \\\[x0, x1, lsl 1\\\]" 2 } } */
/* { dg-final { scan-assembler-times "st1b\\tz0.b, p\[0-9\]+, \\\[x1, x0\\\]" 2 } } */

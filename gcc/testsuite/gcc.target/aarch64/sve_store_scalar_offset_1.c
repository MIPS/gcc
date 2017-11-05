/* { dg-do assemble } */
/* { dg-options "-O3 -march=armv8-a+sve -msve-vector-bits=256 --save-temps" } */

#include <stdint.h>

typedef int64_t v4di __attribute__((vector_size(32)));
typedef int32_t v8si __attribute__((vector_size(32)));
typedef int16_t v16hi __attribute__((vector_size(32)));
typedef int8_t v32qi __attribute__((vector_size(32)));

void sve_store_64_z_lsl (uint64_t *a, unsigned long i)
{
  asm volatile ("" : "=w" (*(v4di *) &a[i]));
}

void sve_store_64_s_lsl (int64_t *a, signed long i)
{
  asm volatile ("" : "=w" (*(v4di *) &a[i]));
}

void sve_store_32_z_lsl (uint32_t *a, unsigned long i)
{
  asm volatile ("" : "=w" (*(v8si *) &a[i]));
}

void sve_store_32_s_lsl (int32_t *a, signed long i)
{
  asm volatile ("" : "=w" (*(v8si *) &a[i]));
}

void sve_store_16_z_lsl (uint16_t *a, unsigned long i)
{
  asm volatile ("" : "=w" (*(v16hi *) &a[i]));
}

void sve_store_16_s_lsl (int16_t *a, signed long i)
{
  asm volatile ("" : "=w" (*(v16hi *) &a[i]));
}

/* ??? The other argument order leads to a redundant move.  */
void sve_store_8_z (unsigned long i, uint8_t *a)
{
  asm volatile ("" : "=w" (*(v32qi *) &a[i]));
}

void sve_store_8_s (signed long i, int8_t *a)
{
  asm volatile ("" : "=w" (*(v32qi *) &a[i]));
}

/* { dg-final { scan-assembler-times {\tst1d\tz0\.d, p[0-7], \[x0, x1, lsl 3\]\n} 2 } } */
/* { dg-final { scan-assembler-times {\tst1w\tz0\.s, p[0-7], \[x0, x1, lsl 2\]\n} 2 } } */
/* { dg-final { scan-assembler-times {\tst1h\tz0\.h, p[0-7], \[x0, x1, lsl 1\]\n} 2 } } */
/* { dg-final { scan-assembler-times {\tst1b\tz0\.b, p[0-7], \[x1, x0\]\n} 2 } } */

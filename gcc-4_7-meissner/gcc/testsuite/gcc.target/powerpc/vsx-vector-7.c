/* { dg-do compile { target { powerpc*-*-* } } } */
/* { dg-skip-if "" { powerpc*-*-darwin* } { "*" } { "" } } */
/* { dg-require-effective-target powerpc_vsx_ok } */
/* { dg-options "-mvsx -Ofast -ftree-vectorize -mno-vector-convert-32bit-to-64bit  -mno-vector-convert-64bit-to-32bit" } */

/* Check whether the compiler does not do vector converts between 32 and 64 bit if desired.  */

#include <stddef.h>

#ifndef SIZE
#define SIZE 1024
#endif

#ifndef ALIGN
#define ALIGN 32
#endif

static double       dbl_array[SIZE] __attribute__((__aligned__(ALIGN)));
static float        flt_array[SIZE] __attribute__((__aligned__(ALIGN)));
static int          int_array[SIZE] __attribute__((__aligned__(ALIGN)));
static unsigned int uns_array[SIZE] __attribute__((__aligned__(ALIGN)));

void
convert_float_to_double (void)
{
  size_t i;

  for (i = 0; i < SIZE; i++)
    dbl_array[i] = (double)flt_array[i];
}

void
convert_int_to_double (void)
{
  size_t i;

  for (i = 0; i < SIZE; i++)
    dbl_array[i] = (double)int_array[i];
}

void
convert_uint_to_double (void)
{
  size_t i;

  for (i = 0; i < SIZE; i++)
    dbl_array[i] = (double)uns_array[i];
}

void
convert_double_to_float (void)
{
  size_t i;

  for (i = 0; i < SIZE; i++)
    flt_array[i] = (float)dbl_array[i];
}

void
convert_double_to_int (void)
{
  size_t i;

  for (i = 0; i < SIZE; i++)
    int_array[i] = (int)dbl_array[i];
}

void
convert_double_to_uint (void)
{
  size_t i;

  for (i = 0; i < SIZE; i++)
    uns_array[i] = (unsigned int)dbl_array[i];
}

/* { dg-final { scan-assembler-not "xxmrghw" } } */
/* { dg-final { scan-assembler-not "xxmrglw" } } */
/* { dg-final { scan-assembler-not "xvcvspdp" } } */
/* { dg-final { scan-assembler-not "xvcvsxwdp" } } */
/* { dg-final { scan-assembler-not "xvcvuxwdp" } } */
/* { dg-final { scan-assembler-not "xvcvdpsp" } } */
/* { dg-final { scan-assembler-not "xvcvdpsxw" } } */
/* { dg-final { scan-assembler-not "xvcvdpuxw" } } */

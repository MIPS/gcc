/* { dg-do compile } */
/* { dg-options "-std=gnu99" } */

/* N1107 Section 4, C995.2.4.2.2a[3], verfiy constants about range of
   DFP and three componets of DFP defined in decfloat.h.  */

/* Make sure we are exporting the right values to decfloat.h. */

extern void abort (void);

int main ()
{
  if (__DEC32_MANT_DIG__ != 7) abort();
  if (__DEC64_MANT_DIG__ != 16) abort();
  if (__DEC128_MANT_DIG__ != 34) abort();

  if (__DEC32_MIN_EXP__ != -95) abort();
  if (__DEC64_MIN_EXP__ != -383) abort();
  if (__DEC128_MIN_EXP__ != -6143) abort();

  if (__DEC32_MAX_EXP__ != 96) abort();
  if (__DEC64_MAX_EXP__ != 384) abort();
  if (__DEC128_MAX_EXP__ != 6144) abort();

  if (__DEC32_MAX__ != 9.999999E96DF) abort();
  if (__DEC64_MAX__ != 9.999999999999999E384DD) abort();
  if (__DEC128_MAX__ != 9.999999999999999999999999999999999E6144DL) abort();

  if (__DEC32_EPSILON__ != 1E-6DF) abort();
  if (__DEC64_EPSILON__ != 1E-15DD) abort();
  if (__DEC128_EPSILON__ != 1E-33DL) abort();
  
  if (__DEC32_MIN__ != 1E-95DF) abort();
  if (__DEC32_MIN__ != 1E-383DD) abort();
  if (__DEC128_MIN__ != 1E-6143DL) abort();

  if (__DEC32_DEN__ != 0.000001E-95DF) abort();
  if (__DEC64_DEN__ != 0.000000000000001E-383DD) abort();
  if (__DEC128_DEN__ != 0.000000000000000000000000000000001E-6143DL) abort();

  return 0;
}

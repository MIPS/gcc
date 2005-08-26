/* { dg-do run } */
/* { dg-options "-O" } */

/* N1107 Section 5.3 Conversions between decimal floating and complex.  */

extern void abort(void);

int main()
{
  _Complex float cf;
  _Complex double cd;
  _Complex long double cld;

  _Decimal32 d32;
  _Decimal64 d64;
  _Decimal128 d128;

  cf = 2.0f *  __extension__ 1i + 3.0f;
  cd = 2.0 * __extension__ 1i + 3.0;
  cld = 2.0ld * __extension__ 1i + 3.0ld;

  /* Convert complex to decimal floating.  */
  d32 = cf;
  d64 = cd;
  d128 = cld;

#if 0
  /* These tests assume that the 3.0 real parts above are represented
     exactly.  */
  if (d32 != 3.0DF)
    abort();
  if (d64 != 3.0DD)
    abort();
  if (d128 != 3.0DL)
    abort();
#endif

  /* Convert decimal floating to complex.  */
  d32 = 2.35DF;
  d64 = 1.23DD;
  d128 = 2.34DL;

  cf = d32;
  cd = d64;
  cld = d128;
 
  /* N1107 5.3 Conversions between decimal floating and complex. 
     When a value of decimal floating type converted to a complex
     type, the real part of the complex result value is undermined
     by the rules of conversions in N1107 5.2 and the imaginary part
     of the complex result value is zero.  */

#if 0
  /* Likewise, these tests assume that the rhs constants can be
     represented exactly.  */
  if (__real__ cf != 2.35f)
    abort ();
  if (__real__ cd != 1.23)
    abort ();
  if (__real__ cld != 2.34)
    abort ();
#endif

  if (__imag__ cf != 0.0f)
    abort ();
  if (__imag__ cd != 0.0)
    abort ();
  if (__imag__ cld != 0.0ld)
    abort ();

  return 0;
}

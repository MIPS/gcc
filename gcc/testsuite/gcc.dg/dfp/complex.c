/* { dg-do run } */
/* { dg-options "-O3" } */

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

  if (d32 != 3.0DF)
    abort ();

  if (d64 != 3.0dd)
    abort ();

  if (d128 != 3.0dl)
    abort ();

  /* Convert decimal floating to complex.  */
  d32 = 2.5DF;
  d64 = 1.5DD;
  d128 = 2.5DL;

  cf = d32;
  cd = d64;
  cld = d128;
 
  /* N1107 5.3 Conversions between decimal floating and complex. 
     When a value of decimal floating type converted to a complex
     type, the real part of the complex result value is undermined
     by the rules of conversions in N1107 5.2 and the imaginary part
     of the complex result value is zero.  */

  if (__real__ cf != 2.5f)
    abort ();

  if (__real__ cd !=1.5)
    abort ();

  if (__real__ cld !=  2.5)
    abort ();

  if (__imag__ cf != 0.0f)
    abort ();
  if (__imag__ cd != 0.0)
    abort ();
  if (__imag__ cld != 0.0ld)
    abort ();

  /*  Verify that the conversions from DFP types to complex is
      determined by the rules of conversion to the real part.  */

  /*  Convert _Decimal64 to _Complex float.  */
  d64 = 0.000488281251dl;
  cf = d64;
  if (__real__ cf != 0.00048828125f)
    abort ();
  /*  Convert _Decimal128 to _Complex double.  */
  d128 = 2.98023223876953125E-8dl;
  cd = d128;
  if (__real__ cd < (2.9802322387695312E-08 - 0.00000000001)
      || __real__ cd  > (2.9802322387695312e-08 + 0.00000000001))
    abort ();

  /*  Verify that conversion from complex to decimal floating types
      results in the value of the real part converted to the result
      type according to the rules of conversion between those types.  */

  /*  Convert _Complex float to _Decimal32.  */
  cf = 2.0f *  __extension__ 1i + 1.00390625f;
  d32 = cf;
  if (d32 != 1.003906DF)
    abort ();

  /*  Convert _Complex double to _Decimal32.  */
  cd = 2.0 *  __extension__ 1i + 1.00390625;
  d32 = cd;
  if (d32 != 1.003906DF)
    abort ();

  /*  Convert _Complex long double to _Decimal32.  */
  cld = 2.0ld *  __extension__ 1i + 1.00390625ld;
  if (d32 != 1.003906DF)
    abort ();

  /*  Convert _Complex double to _Decimal64.  */
  cd = 2.0 *  __extension__ 1i + 10.000030517578125;
  d64 = cd;
  if (d64 != 10.00003051757812)
    abort ();

  /*  Convert _Complex long double to _Decimal64.  */
  cld = 2.0ld *  __extension__ 1i + 10.000030517578125ld;
  d64 = cld;
  if (d64 != 10.00003051757812)
    abort ();

  return 0;
}

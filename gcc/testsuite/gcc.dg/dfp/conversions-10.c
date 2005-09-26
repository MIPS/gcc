/* { dg-do run } */
/* { dg-options "-std=gnu99" } */

/* N1107 5.2 Conversions between decimal floating types and non-decimal
   floating types.  Tests for conversions from decimal floating types to 
   non-decimal floating tyeps.  */

extern void abort (void);

int
main ()
{
  float f;
  long double ll;
  _Bool b;

  _Decimal32 d32;
  _Decimal64 d64;
  _Decimal128 d128;

  /* Verify that conversion of negative 32-bit DFP values of up to six 
     digits to 32-bit standard float produces the exact result.  */
  d32 = -123.456df;
  f = d32;
  if (f != -123.456f)
    abort ();

  /* Verify that conversion of negative 64-bit DFP values of up to six 
     digits to 32-bit standard float produces the exact result.  */  
  d64 = -123.456dd;
  f = d64;
  if (f != -123.456f)
    abort ();

#if 0
  /* This test assumes exact conversion to binary FP.  */

  /* Verify that conversion of negative 64-bit DFP values of up to six
     digits to 64-bit standard float produces the exact result.  */
  d64 = -123.456dd;
  ll = d64;
  if (ll != -123.456l)
    abort ();
#endif

#if 0
  /* This test assumes exact conversion to binary FP.  */

  /* Verify that conversion of negative 128-bit DFP values of up to six
     digits to 64-bit standard float produces the exact result.  */
  d128 = -123.456dl;
  ll = d128;
  if (ll !=  -123.456l)
    abort ();
#endif
  
  /* Verify that division by 100,10 and 1 of a five-digit DFP value 
     produces the exact result.  */
  d32 = 1234.5df;
  f = d32/100;
  if (f != 12.345f)
    abort ();

  f = d32/1;
  if (f != 1234.5f)
    abort ();
  
  d64 = 1234.5678dd;
  f = d64/100;
  if (f != 12.345678f)
    abort ();

  f = d64/1;
  if (f != 1234.5678f)
    abort ();
  
  d128 = 1234.5678dl;
  f = d128/100;
  if (f != 12.345678f)
    abort ();

  f = d128/1;
  if (f != 1234.5678f)
    abort ();

  /* C99 Section 6.7.2 Type specifiers.  Type _Bool is 
     mentioned in this section.  Conversions between 
     _Bool and DFP types.  */
  b = d32;
  if (!b)
    abort ();
  b = d64;
  if (!b)
    abort ();
  b = d128;
  if (!b)
    abort ();

  d32 = 0.0DD;
  d64 = 0.0DL;
  d128 = 0.0DF;
  
  b = d32;
  if (b)
    abort ();
  b = d64;
  if (b)
    abort ();
  b = d128;
  if (b)
    abort ();

  return 0;
}

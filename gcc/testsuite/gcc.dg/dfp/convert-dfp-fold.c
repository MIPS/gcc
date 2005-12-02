/* Make the test compile-only until runtime conversions are supported.  */
/* { dg-do compile } */
/* { dg-options "-std=gnu99 -O3" } */

/* N1107 Section 4 5.2.4.2.2a [2] & [3].  Test various conversions
   involving decimal floating types. */

extern void abort (void);

_Decimal32 d32;
_Decimal64 d64;
_Decimal128 d128;

int main()
{
  /* Conversions between decimal floats.  Similar to conversions-4.c
     but most of the checks below should be optimized away, exercising
     different code-paths in GCC. */
   
  d64 = 1.5df;
  if (d64 != 1.5dd)
    abort();

  d128 = 1.5df;
  if (d128 != 1.5dl)
    abort();

  /* Conversions from _Decimal64. */
  d32 = -1.5dd;
  if (d32 != -1.5df)
    abort();
  
  d128 = -1.5dd;
  if (d128 != -1.5dl)
    abort();

  /* Conversions from _Decimal128. */
  d32 = 2.15dl;
  if (d32 != 2.15df)
    abort();

  d64 = 2.15dl;
  if (d64 != 2.15dd)
    abort();

  /* Test demotion to non-representable decimal floating type, positive 
     and negative respectively. */
  /* Assumes a default rounding mode of 'near'.  The rules are a 
     bit odd in that if off by one digit, it rounds to the maximum
     value, but otherwise to HUGE_VAL. */
  d32 = 9.9999991E96DD;
  if (d32 != __DEC32_MAX__)
    abort();

  d32 = 9.99999912E96DD;
  if (d32 != __builtin_infd32())
    abort();

  /* One digit more than _Decimal32 can handle. */
  d32 = 9.9999991E96DD;
  if (d32 != __DEC32_MAX__)
    abort();
  d32 = 0.0000009E-95DD;
  if (d32 != 0)
    abort();

  /* Two digits more than _Decimal32 can handle. */
  d32 = 9.99999912E96DD;
  if (d32 != __builtin_infd32())
    abort();
  d32 = 0.00000099E-95DD;
  if (d32 != 0)
    abort();

  /* One digit more that _Decimal64 can handle. */
  d64 = 9.9999999999999991E384DL;
  if (d64 != __DEC64_MAX__)
    abort();
  d64 = 0.0000000000000009E-383DL;
  if (d64 != 0)
    abort();

  /* Two digits more than _Decimal64 can handle. */
  d64 = 9.99999999999999912E384DL;
  if (d64 != __builtin_infd64())
    abort();
  d64 = 0.00000000000000099E-383DL;
  if (d64 != 0)
    abort();



  /* One digit more than _Decimal32 can handle. */
  d32 = -9.9999991E96DD;
  if (d32 != -__DEC32_MAX__)
    abort();
  d32 = -0.00000099E-95DD;
  if (d32 != -0.0DF)
    abort();

  /* Two digits more than _Decimal32 can handle. */
  d32 = -9.99999912E96DD;
  if (d32 != -__builtin_infd32())
    abort();  
  d32 = -0.0000009E-95DD;
  if (d32 != -0.0DF)
    abort();

  /* One digit more that _Decimal64 can handle. */
  d64 = -9.9999999999999991E384DL;
  if (d64 != -__DEC64_MAX__)
    abort();
  d64 = -0.0000000000000009E-383DL;
  if (d64 != -0.0DD)
    abort();  

  /* Two digits more than _Decimal64 can handle. */
  d64 = -9.99999999999999912E384DL;
  if (d64 != -__builtin_infd64())
    abort();
  d64 = -0.00000000000000099E-383DL;
  if (d64 != -0.0DD)
    abort();
  return 0;
}

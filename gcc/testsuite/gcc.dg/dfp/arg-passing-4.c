/* { dg-do run } */
/* { dg-options "-std=gnu99 -Wall" } */

/* Test scalar passing and return values involving decimal floating
   point types.  Not discussed in N1107, but still necessary.  */

#include <assert.h>

/* A handful of functions that return their Nth _Decimal32
   argument with mixed types in parameter list.  */

static _Decimal32
arg0_32 (_Decimal32 arg0, int arg1, unsigned int arg2,
       float arg3, double  arg4, long double arg5)
{
  return arg0;
}

static _Decimal32
arg1_32 (int arg0, _Decimal32 arg1, unsigned int arg2,
       float arg3, double arg4, long double arg5)
{
  return arg1;
}

static _Decimal32
arg2_32 (int arg0, unsigned int arg1, _Decimal32 arg2,
       float arg3, double arg4, long double arg5)
{
  return arg2;
}


static _Decimal32
arg3_32 (int arg0, unsigned int arg1, float arg2,
       _Decimal32 arg3, double arg4, long double arg5)
{
  return arg3;
}

static _Decimal32
arg4_32 (int arg0, unsigned int arg1, float arg2,
       double arg3, _Decimal32 arg4, long double arg5)
{
  return arg4;
}

static _Decimal32
arg5_32 (int arg0, unsigned int arg1, float arg2,
       double arg3, long double arg4, _Decimal32 arg5)
{
  return arg5;
}

/* A handful of functions that return their Nth _Decimal64
   argument with mixed types in parameter list.  */

static _Decimal64
arg0_64 (_Decimal64 arg0, int arg1, unsigned int arg2,
         float arg3, double  arg4, long double arg5)
{
  return arg0;
}

static _Decimal64
arg1_64 (int arg0, _Decimal64 arg1, unsigned int arg2,
         float arg3, double arg4, long double arg5)
{
  return arg1;
}

static _Decimal64
arg2_64 (int arg0, unsigned int arg1, _Decimal64 arg2,
         float arg3, double arg4, long double arg5)
{
  return arg2;
}

static _Decimal64
arg3_64 (int arg0, unsigned int arg1, float arg2,
         _Decimal64 arg3, double arg4, long double arg5)
{
  return arg3;
}

static _Decimal64
arg4_64 (int arg0, unsigned int arg1, float arg2,
         float arg3, _Decimal64 arg4, long double arg5)
{
  return arg4;
}

static _Decimal64
arg5_64 (int arg0, unsigned int arg1, float arg2,
         double arg3, long double arg4, _Decimal64 arg5)
{
  return arg5;
}

/* A handful of functions that return their Nth _Decimal128
   argument with mixed types in parameter list.  */

static _Decimal128
arg0_128 (_Decimal128 arg0, int arg1, unsigned int arg2,
         float arg3, double  arg4, long double arg5)
{
  return arg0;
}
static _Decimal32
arg1_128 (int arg0, _Decimal128 arg1, unsigned int arg2,
         float arg3, double arg4, long double arg5)
{
  return arg1;
}

static _Decimal128
arg2_128 (int arg0, unsigned int arg1, _Decimal128 arg2,
         float arg3, double arg4, long double arg5)
{
  return arg2;
}

static _Decimal128
arg3_128 (int arg0, unsigned int arg1, float arg2,
         _Decimal128 arg3, double arg4, long double arg5)
{
  return arg3;
}

static _Decimal128
arg4_128 (int arg0, unsigned int arg1, float arg2,
         float arg3, _Decimal32 arg4, long double arg5)
{
  return arg4;
}

static _Decimal128
arg5_128 (int arg0, unsigned int arg1, float arg2,
         double arg3, long double arg4, _Decimal128 arg5)
{
  return arg5;
}



int main()
{
  /* _Decimal32 variants.  */
  assert (arg0_32 (0.0df, -1, 2, 3.0f, 4.0, 5.0ld) == 0.0df);
  assert (arg1_32 (0, 1.0df, 2, 3.0f, 4.0, 5.0ld) == 1.0df);
  assert (arg2_32 (0, -1, 2.0df, 3.0f, 4.0, 5.0ld) == 2.0df);
  assert (arg3_32 (0, -1, 2.0f, 3.0df, 4.0, 5.0ld) == 3.0df);
  assert (arg4_32 (0, -1, 2.0f, 3.0, 4.0df, 5.0ld) == 4.0df);
  assert (arg5_32 (0, -1, 2.0f, 3.0, 4.0ld, 5.0df) == 5.0df);

  /* _Decimal64 variants.  */
  assert (arg0_64 (0.0dd, -1, 2, 3.0f, 4.0, 5.0ld) == 0.0dd);
  assert (arg1_64 (0, 1.0dd, 2, 3.0f, 4.0, 5.0ld) == 1.0dd);
  assert (arg2_64 (0, -1, 2.0dd, 3.0f, 4.0, 5.0ld) == 2.0dd);
  assert (arg3_64 (0, -1, 2.0f, 3.0dd, 4.0, 5.0ld) == 3.0dd);
  assert (arg4_64 (0, -1, 2.0f, 3.0, 4.0dd, 5.0ld) == 4.0dd);
  assert (arg5_64 (0, -1, 2.0f, 3.0, 4.0ld, 5.0dd) == 5.0dd);

  /* _Decimal128 variants.  */
  assert (arg0_128 (0.0dl, -1, 2, 3.0f, 4.0, 5.0ld) == 0.0dl);
  assert (arg1_128 (0, 1.0dl, 2, 3.0f, 4.0, 5.0ld) == 1.0dl);
  assert (arg2_128 (0, -1, 2.0dl, 3.0f, 4.0, 5.0ld) == 2.0dl);
  assert (arg3_128 (0, -1, 2.0f, 3.0dl, 4.0, 5.0ld) == 3.0dl);
  assert (arg4_128 (0, -1, 2.0f, 3.0, 4.0dl, 5.0ld) == 4.0dl);
  assert (arg5_128 (0, -1, 2.0f, 3.0, 4.0ld, 5.0dl) == 5.0dl);

  return 0;
}

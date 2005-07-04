/* { dg-do run } */
/* { dg-options "-std=gnu99" } */

/* Test passing array elements involving decimal floating point types.
   Not discussed in N1107, but necessary all the same.  */

#include <assert.h>

/* A handful of functions that return the Nth _Decimal32 argument of
   an incoming array.  */

static _Decimal32
arg0_32 (_Decimal32 args[])
{
  return args[0];
}

static _Decimal32
arg1_32 (_Decimal32 args[])
{
  return args[1];
}

static _Decimal32
arg2_32 (_Decimal32 args[])
{
  return args[2];
}

static _Decimal32
arg3_32 (_Decimal32 args[])
{
  return args[3];
}

static _Decimal32
arg4_32 (_Decimal32 args[])
{
  return args[4];
}

static _Decimal32
arg5_32 (_Decimal32 args[])
{
  return args[5];
}
	

/* A handful of functions that return the Nth _Decimal64 argument of
   an incoming array.  */

static _Decimal64
arg0_64 (_Decimal64 args[])
{
  return args[0];
}

static _Decimal64
arg1_64 (_Decimal64 args[])
{
  return args[1];
}

static _Decimal64
arg2_64 (_Decimal64 args[])
{
  return args[2];
}

static _Decimal64
arg3_64 (_Decimal64 args[])
{
  return args[3];
}

static _Decimal64
arg4_64 (_Decimal64 args[])
{
  return args[4];
}

static _Decimal64
arg5_64 (_Decimal64 args[])
{
  return args[5];
}


/* A handful of functions that return the Nth _Decimal128 argument of
   an incoming array.  */

static _Decimal128
arg0_128 (_Decimal128 args[])
{
  return args[0];
}

static _Decimal128
arg1_128 (_Decimal128 args[])
{
  return args[1];
}

static _Decimal128
arg2_128 (_Decimal128 args[])
{
  return args[2];
}

static _Decimal128
arg3_128 (_Decimal128 args[])
{
  return args[3];
}

static _Decimal128
arg4_128 (_Decimal128 args[])
{
  return args[4];
}

static _Decimal128
arg5_128 (_Decimal128 args[])
{
  return args[5];
}


int main()
{
  _Decimal32 d32[] = { 0.0df, 1.0df, 2.0df, 3.0df, 4.0df, 5.0df };
  _Decimal64 d64[] = { 0.0dd, 1.0dd, 2.0dd, 3.0dd, 4.0dd, 5.0dd };
  _Decimal128 d128[] = { 0.0dl, 1.0dl, 2.0dl, 3.0dl, 4.0dl, 5.0dl };

  /* _Decimal32 variants.  */
  assert (arg0_32 (d32) == 0.0df);
  assert (arg1_32 (d32) == 1.0df);
  assert (arg2_32 (d32) == 2.0df);
  assert (arg3_32 (d32) == 3.0df);
  assert (arg4_32 (d32) == 4.0df);
  assert (arg5_32 (d32) == 5.0df);

  /* _Decimal64 variants.  */
  assert (arg0_64 (d64) == 0.0dd);
  assert (arg1_64 (d64) == 1.0dd);
  assert (arg2_64 (d64) == 2.0dd);
  assert (arg3_64 (d64) == 3.0dd);
  assert (arg4_64 (d64) == 4.0dd);
  assert (arg5_64 (d64) == 5.0dd);

  /* _Decimal128 variants.  */
  assert (arg0_128 (d128) == 0.0dl);
  assert (arg1_128 (d128) == 1.0dl);
  assert (arg2_128 (d128) == 2.0dl);
  assert (arg3_128 (d128) == 3.0dl);
  assert (arg4_128 (d128) == 4.0dl);
  assert (arg5_128 (d128) == 5.0dl);

  return 0;
}

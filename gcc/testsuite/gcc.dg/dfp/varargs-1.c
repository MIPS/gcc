/* { dg-do run } */
/* { dg-options "-std=gnu99" } */

/* Test passing varargs of the decimal float types.  Not discussed in
   N1107, but still necessary.  */

#include <assert.h>
#include <stdarg.h>

static _Decimal32
vararg32 (unsigned arg, ...)
{
  int i;
  va_list ap;
  _Decimal32 result;

  va_start (ap, arg);
  for (i = 0; i <= arg; i++)
    result = va_arg (ap, _Decimal32);
  va_end (ap);
  return result;
}

static _Decimal64
vararg64 (unsigned arg, ...)
{
  int i;
  va_list ap;
  _Decimal64 result;

  va_start (ap, arg);
  for (i = 0; i <= arg; i++)
    result = va_arg (ap, _Decimal64);
  va_end (ap);
  return result;
}

static _Decimal128
vararg128 (unsigned arg, ...)
{
  int i;
  va_list ap;
  _Decimal128 result;

  va_start (ap, arg);
  for (i = 0; i <= arg; i++)
    result = va_arg (ap, _Decimal128);
  va_end (ap);
  return result;
}


int main()
{
  /* _Decimal32 variants.  */
  assert (vararg32 (0, 0.0df, 1.0df, 2.0df, 3.0df, 4.0df, 5.0df) == 0.0df);
  assert (vararg32 (1, 0.0df, 1.0df, 2.0df, 3.0df, 4.0df, 5.0df) == 1.0df);
  assert (vararg32 (2, 0.0df, 1.0df, 2.0df, 3.0df, 4.0df, 5.0df) == 2.0df);
  assert (vararg32 (3, 0.0df, 1.0df, 2.0df, 3.0df, 4.0df, 5.0df) == 3.0df);
  assert (vararg32 (4, 0.0df, 1.0df, 2.0df, 3.0df, 4.0df, 5.0df) == 4.0df);
  assert (vararg32 (5, 0.0df, 1.0df, 2.0df, 3.0df, 4.0df, 5.0df) == 5.0df);

  /* _Decimal64 variants.  */
  assert (vararg64 (0, 0.0dd, 1.0dd, 2.0dd, 3.0dd, 4.0dd, 5.0dd) == 0.0dd);
  assert (vararg64 (1, 0.0dd, 1.0dd, 2.0dd, 3.0dd, 4.0dd, 5.0dd) == 1.0dd);
  assert (vararg64 (2, 0.0dd, 1.0dd, 2.0dd, 3.0dd, 4.0dd, 5.0dd) == 2.0dd);
  assert (vararg64 (3, 0.0dd, 1.0dd, 2.0dd, 3.0dd, 4.0dd, 5.0dd) == 3.0dd);
  assert (vararg64 (4, 0.0dd, 1.0dd, 2.0dd, 3.0dd, 4.0dd, 5.0dd) == 4.0dd);
  assert (vararg64 (5, 0.0dd, 1.0dd, 2.0dd, 3.0dd, 4.0dd, 5.0dd) == 5.0dd);

  /* _Decimal128 variants.  */
  assert (vararg128 (0, 0.0dl, 1.0dl, 2.0dl, 3.0dl, 4.0dl, 5.0dl) == 0.0dl);
  assert (vararg128 (1, 0.0dl, 1.0dl, 2.0dl, 3.0dl, 4.0dl, 5.0dl) == 1.0dl);
  assert (vararg128 (2, 0.0dl, 1.0dl, 2.0dl, 3.0dl, 4.0dl, 5.0dl) == 2.0dl);
  assert (vararg128 (3, 0.0dl, 1.0dl, 2.0dl, 3.0dl, 4.0dl, 5.0dl) == 3.0dl);
  assert (vararg128 (4, 0.0dl, 1.0dl, 2.0dl, 3.0dl, 4.0dl, 5.0dl) == 4.0dl);
  assert (vararg128 (5, 0.0dl, 1.0dl, 2.0dl, 3.0dl, 4.0dl, 5.0dl) == 5.0dl);

  return 0;
}

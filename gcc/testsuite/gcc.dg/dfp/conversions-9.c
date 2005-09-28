/* { dg-do run } */
/* { dg-options "-std=gnu99" } */

/* Test various conversions involving decimal floating types. */

extern void abort (void);

_Decimal32 d32;
_Decimal64 d64;
_Decimal128 d128;
signed int si;
signed long long di;
unsigned long long udi;

int main()
{
  /* Constant conversions to signed integers. */
  si = -1.50df;
  if (si != -1)
    abort();

  di = -1.50df;
  if (di != -1)
    abort();

  si = -1.55dd;
  if (si != -1)
    abort();

  di = -1.55dd;
  if (di != -1)
    abort();

  si = -1555.55dl;
  if (si != -1555)
    abort();

  di = -1555.55dl;
  if (di != -1555)
    abort();

  /* Conversions from signed integer constants. */
  d32 = -1555;
  if (d32 != -1555.0df)
    abort();

  d32 = -1555ll;
  if (d32 != -1555.0df)
    abort();

  d64 = -1555;
  if (d64 != -1555.0dd)
    abort();

  d64 = -1555ll;
  if (d64 != -1555.0dd)
    abort();

  d128 = -1555;
  if (d128 != -1555.0dl)
    abort();

  d128 = -1555ll;
  if (d128 != -1555.0dl)
    abort();

  return 0;
}

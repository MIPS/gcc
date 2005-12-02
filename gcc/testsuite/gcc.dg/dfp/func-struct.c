/* { dg-do run } */
/* { dg-options "-std=gnu99" } */

/* Test structure passing and return values involving decimal floating
   point types.  Not discussed in N1107, but still necessary.  */

#include <assert.h>

static struct example
{
  _Decimal128 d128;
  char dummy1;
  _Decimal64 d64;
  char dummy2;
  _Decimal32 d32;
} nums = { 1.0dl, 'a', 2.0dd, 'b', 3.0df };

static _Decimal32
d32_field (struct example s)
{
  return s.d32;
}

static _Decimal64
d64_field (struct example s)
{
  return s.d64;
}

static _Decimal128
d128_field (struct example s)
{
  return s.d128;
}

static char
dummy1_field (struct example s)
{
  return s.dummy1;
}

static char
dummy2_field (struct example s)
{
  return s.dummy2;
}

static _Decimal32
ptr_d32_field (struct example *s)
{
  return s->d32;
}

static _Decimal64
ptr_d64_field (struct example *s)
{
  return s->d64;
}

static _Decimal128
ptr_d128_field (struct example *s)
{
  return s->d128;
}

static char
ptr_dummy1_field (struct example *s)
{
  return s->dummy1;
}

static char
ptr_dummy2_field (struct example *s)
{
  return s->dummy2;
}


int main()
{
  assert (d32_field (nums) == 3.0df);
  assert (d64_field (nums) == 2.0dd);
  assert (d128_field (nums) == 1.0dl);
  assert (dummy1_field (nums) == 'a');
  assert (dummy2_field (nums) == 'b');

  assert (ptr_d32_field (&nums) == 3.0df);
  assert (ptr_d64_field (&nums) == 2.0dd);
  assert (ptr_d128_field (&nums) == 1.0dl);
  assert (ptr_dummy1_field (&nums) == 'a');
  assert (ptr_dummy2_field (&nums) == 'b');

  return 0;
}

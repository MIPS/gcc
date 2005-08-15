/* Runtime comparisons of DFP variables with variables of different types.  */

/* { dg-do run } */
/* { dg-options "-std=gnu99" } */

extern void abort (void);
static int failcnt;

/* Support compiling the test to report individual failures; default is
   to abort as soon as a check fails.  */
#ifdef DBG
#include <stdio.h>
#define FAILURE { printf ("failed at line %d\n", __LINE__); failcnt++; }
#else
#define FAILURE abort ();
#endif

_Decimal32 d32;
_Decimal64 d64;
_Decimal128 d128;

/* Use some typedefs of decimal float types, too.  */
typedef _Decimal32 SDtype;
typedef _Decimal64 DDtype;
typedef _Decimal128 TDtype;

SDtype d32b;
DDtype d64b;
TDtype d128b;
unsigned char uc;
signed char sc;
unsigned short us;
signed short ss;
unsigned int ui;
signed int si;
unsigned long ul;
signed long sl;
unsigned long long ull;
signed long long sll;
float f;
double d;
long double ld;

void
inits (void)
{
  d32 = 1.0df;
  d64 = 3.0dd;
  d128 = 5.0dl;
  d32b = -1.0df;
  d64b = -4.0dd;
  d128b = -6.0dl;
  uc = 1;
  sc = -5;
  us = 3;
  ss = -4;
  ui = 2;
  si = -6;
  ul = 3;
  sl = -2;
  ull = 8;
  sll = -4;
  f = 1.0f;
  d = -3.0;
  ld = 5.0ld;
}

void
compare_ints (void)
{
  /* Compare _Decimal32 variables to variables of integral types.  */
  if ((d32 == uc) != 1) FAILURE
  if ((d32 >= sc) != 1) FAILURE
  if ((d32 == us) != 0) FAILURE
  if ((d32 > ss) != 1) FAILURE
  if ((d32 >= ui) != 0) FAILURE
  if ((d32b != si) != 1) FAILURE
  if ((d32 < ul) != 1) FAILURE
  if ((d32b == sl) != 0) FAILURE
  if ((d32 != ull) != 1) FAILURE
  if ((d32 == sll) != 0) FAILURE

  if ((uc == d32) != 1) FAILURE
  if ((sc <= d32) != 1) FAILURE
  if ((us == d32) != 0) FAILURE
  if ((ss < d32) != 1) FAILURE
  if ((ui <= d32b) != 0) FAILURE
  if ((si != d32) != 1) FAILURE
  if ((ul > d32) != 1) FAILURE
  if ((sl <= d32b) != 1) FAILURE
  if ((ull != d32) != 1) FAILURE
  if ((sll == d32) != 0) FAILURE

  /* Compare _Decimal128 variables to variables of integral types.  */
  if ((d64 == uc) != 0) FAILURE
  if ((d64 >= sc) != 1) FAILURE
  if ((d64 == us) != 1) FAILURE
  if ((d64b <= ss) != 1) FAILURE
  if ((d64 >= ui) != 1) FAILURE
  if ((d64 != si) != 1) FAILURE
  if ((d64 <= ul) != 1) FAILURE
  if ((d64 > sl) != 1) FAILURE
  if ((d64 != ull) != 1) FAILURE
  if ((d64 == sll) != 0) FAILURE

  if ((uc == d64) != 0) FAILURE
  if ((sc <= d64) != 1) FAILURE
  if ((us == d64) != 1) FAILURE
  if ((ss == d64b) != 1) FAILURE
  if ((ui <= d64) != 1) FAILURE
  if ((si != d64) != 1) FAILURE
  if ((ul >= d64) != 1) FAILURE
  if ((sl < d64) != 1) FAILURE
  if ((ull != d64) != 1) FAILURE
  if ((sll == d64) != 0) FAILURE

  /* Compare _Decimal128 variables to variables of integral types.  */
  if ((d128 == uc) != 0) FAILURE
  if ((d128 >= sc) != 1) FAILURE
  if ((d128 == us) != 0) FAILURE
  if ((d128 > ss) != 1) FAILURE
  if ((d128 >= ui) != 1) FAILURE
  if ((d128b != si) != 0) FAILURE
  if ((d128 < ul) != 0) FAILURE
  if ((d128 > sl) != 1) FAILURE
  if ((d128 != ull) != 1) FAILURE
  if ((d128 == sll) != 0) FAILURE
                                                                                
  if ((uc == d128) != 0) FAILURE
  if ((sc <= d128) != 1) FAILURE
  if ((us == d128) != 0) FAILURE
  if ((ss < d128) != 1) FAILURE
  if ((ui <= d128) != 1) FAILURE
  if ((si == d128b) != 1) FAILURE
  if ((ul > d128) != 0) FAILURE
  if ((sl < d128) != 1) FAILURE
  if ((ull != d128) != 1) FAILURE
  if ((sll == d128) != 0) FAILURE
}

void
compare_floats (void)
{
  /* Compare _Decimal32 variables to variables of binary float types.  */
  if ((d32 == f) != 1) FAILURE
  if ((d32 <= d) != 0) FAILURE
  if ((d32 < ld) != 1) FAILURE
  if ((f != d32) != 0) FAILURE
  if ((d < d32) != 1) FAILURE
  if ((ld == d32) != 0) FAILURE

  /* Compare _Decimal64 variables to variables of binary float types.  */
  if ((d64 == f) != 0) FAILURE
  if ((d64b <= d) != 1) FAILURE
  if ((d64 >= ld) != 0) FAILURE
  if ((f != d64) != 1) FAILURE
  if ((d < d64b) != 0) FAILURE
  if ((ld > d64) != 1) FAILURE

  /* Compare _Decimal128 variables to variables of binary float types.  */
  if ((d128 == f) != 0) FAILURE
  if ((d128b <= d) != 1) FAILURE
  if ((d128 == ld) != 1) FAILURE
  if ((f != d128) != 1) FAILURE
  if ((d > d128b) != 1) FAILURE
  if ((ld < d128b) != 0) FAILURE
}

void
compare_dfp (void)
{
  if ((d32 > d64) != 0) FAILURE
  if ((d32 > d128b) != 1) FAILURE

  if ((d64 < d32) != 0) FAILURE
  if ((d64 < d128) != 1) FAILURE

  if ((d128 > d32) != 1) FAILURE
  if ((d128 != d64) != 1) FAILURE
}

int
main ()
{
  inits ();

  compare_ints ();
  compare_floats ();
  compare_dfp ();

  if (failcnt)
    abort ();
  return 0;
}

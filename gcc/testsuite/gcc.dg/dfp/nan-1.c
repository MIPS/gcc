/* { dg-do run } */
/* { dg-options "-std=gnu99" } */

/* A few simple checks on arithmetic operations. */
#include <stdio.h>
extern void abort (void);

int main()
{
  /* Some possibly non-obvious tests, but most logical
     operations on NaN return false, including NaN == NaN. */
  if (__builtin_nandf("") == __builtin_nandf(""))
    abort();

  if (__builtin_nandd("") == __builtin_nandd(""))
    abort();

  if (__builtin_nandl("") == __builtin_nandl(""))
    abort();
  
  if (!(__builtin_nandf("") != __builtin_nandf("")))
    abort();

  if (!(__builtin_nandd("") != __builtin_nandd("")))
    abort();

  if (!(__builtin_nandl("") != __builtin_nandl("")))
    abort();

  if (__builtin_nandf("") > __builtin_nandf(""))
    abort();

  if (__builtin_nandd("") >= __builtin_nandd(""))
    abort();

  if (__builtin_nandl("") <  __builtin_nandl(""))
    abort();

  if (-__builtin_nandl("") <  +__builtin_nandl(""))
    abort();

  /* 0.0/0.0 => NaN, but NaN != NaN.  */
  if (0.0df/0.0dl == __builtin_nandf(""))
    abort();

  /* 0.0/0.0 => NaN, but NaN != NaN.  */
  if ((0.0dd/0.0df) == (0.0dd/0.0df))
    abort();

  if (__builtin_nandf("") <  __builtin_infdf())
    abort();

  if (__builtin_nandf("") >=  __builtin_infdf())
    abort();

  /* Fixme: Add sqrtdf(-x.df) test when sqrt is supported. */

  if (!__builtin_isnandf(__builtin_nandf("")))
    abort();

  if (!__builtin_isnandd(__builtin_nandd("")))
    abort();

  if (!__builtin_isnandl(__builtin_nandl("")))
    abort();

  if (!__builtin_isnandl(8.0df * __builtin_nandl("")))
    abort();

  if (!__builtin_isnandf(8.1dl - __builtin_nandf("")))
    abort();

  if (!__builtin_isnandl(__builtin_nandd("") + __builtin_nandl("")))
    abort();

  return 0;
}

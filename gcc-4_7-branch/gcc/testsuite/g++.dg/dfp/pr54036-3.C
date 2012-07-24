#include <decimal/decimal>
using namespace std;

decimal::decimal128
__attribute__ ((noinline))
my_nan128 (void)
{
  decimal::decimal128 z = 0;
  decimal::decimal128 v = z/z;
  return v;
}

decimal::decimal128
__attribute__ ((noinline))
my_inf128 (void)
{
  decimal::decimal128 o = 1;
  decimal::decimal128 z = 0;
  decimal::decimal128 v = o/z;
  return v;
}

int
main (void)
{
  decimal::decimal128 v;

  v = my_nan128 ();
  if (!__builtin_isnand128 (v.__getval ()))
    __builtin_abort ();
  if (__builtin_signbitd128 (v.__getval ()))
    __builtin_abort ();

  v = -v;

  if (!__builtin_isnand128 (v.__getval ()))
    __builtin_abort ();
  if (!__builtin_signbitd128 (v.__getval ()))
    __builtin_abort ();

  v = my_inf128 ();
  if (!__builtin_isinfd128 (v.__getval ()))
    __builtin_abort ();
  if (__builtin_signbitd128 (v.__getval ()))
    __builtin_abort ();

  v = -v;

  if (!__builtin_isinfd128 (v.__getval ()))
    __builtin_abort ();
  if (!__builtin_signbitd128 (v.__getval ()))
    __builtin_abort ();

  return 0;
}


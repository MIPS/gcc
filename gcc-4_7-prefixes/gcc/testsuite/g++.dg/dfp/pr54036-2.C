#include <decimal/decimal>
using namespace std;

decimal::decimal64
__attribute__ ((noinline))
my_nan64 (void)
{
  decimal::decimal64 z = 0;
  decimal::decimal64 v = z/z;
  return v;
}

decimal::decimal64
__attribute__ ((noinline))
my_inf64 (void)
{
  decimal::decimal64 o = 1;
  decimal::decimal64 z = 0;
  decimal::decimal64 v = o/z;
  return v;
}

int
main (void)
{
  decimal::decimal64 v;

  v = my_nan64 ();
  if (!__builtin_isnand64 (v.__getval ()))
    __builtin_abort ();
  if (__builtin_signbitd64 (v.__getval ()))
    __builtin_abort ();

  v = -v;

  if (!__builtin_isnand64 (v.__getval ()))
    __builtin_abort ();
  if (!__builtin_signbitd64 (v.__getval ()))
    __builtin_abort ();

  v = my_inf64 ();
  if (!__builtin_isinfd64 (v.__getval ()))
    __builtin_abort ();
  if (__builtin_signbitd64 (v.__getval ()))
    __builtin_abort ();

  v = -v;

  if (!__builtin_isinfd64 (v.__getval ()))
    __builtin_abort ();
  if (!__builtin_signbitd64 (v.__getval ()))
    __builtin_abort ();

  return 0;
}


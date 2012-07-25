#include <decimal/decimal>
using namespace std;

decimal::decimal32
__attribute__ ((noinline))
my_nan32 (void)
{
  decimal::decimal32 z = 0;
  decimal::decimal32 v = z/z;
  return v;
}

decimal::decimal32
__attribute__ ((noinline))
my_inf32 (void)
{
  decimal::decimal32 o = 1;
  decimal::decimal32 z = 0;
  decimal::decimal32 v = o/z;
  return v;
}

int
main (void)
{
  decimal::decimal32 v;

  v = my_nan32 ();
  if (!__builtin_isnand32 (v.__getval ()))
    __builtin_abort ();
  if (__builtin_signbitd32 (v.__getval ()))
    __builtin_abort ();

  v = -v;

  if (!__builtin_isnand32 (v.__getval ()))
    __builtin_abort ();
  if (!__builtin_signbitd32 (v.__getval ()))
    __builtin_abort ();

  v = my_inf32 ();
  if (!__builtin_isinfd32 (v.__getval ()))
    __builtin_abort ();
  if (__builtin_signbitd32 (v.__getval ()))
    __builtin_abort ();

  v = -v;

  if (!__builtin_isinfd32 (v.__getval ()))
    __builtin_abort ();
  if (!__builtin_signbitd32 (v.__getval ()))
    __builtin_abort ();

  return 0;
}


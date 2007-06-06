/* { dg-do compile } */
/* { dg-mips-options "-O2 -mips32r2 -mdsp" } */
/* { dg-final { scan-assembler-times "\tdpsq_sa.l.w\t\\\$ac" 2 } } */

_Sat long long _Fract
f1 (_Sat long _Fract x, _Sat long _Fract y, _Sat long long _Fract z)
{
  return z - (_Sat long long _Fract) x * y;
}

_Sat long long _Fract
f2 (_Sat long _Fract x, _Sat long _Fract y, _Sat long long _Fract z)
{
  _Sat long long _Fract t = (_Sat long long _Fract) x * y;
  int temp = 5;
  if (temp == 5)
    z -= t;
  return z;
}

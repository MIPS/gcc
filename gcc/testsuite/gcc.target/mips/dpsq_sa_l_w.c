/* { dg-do compile } */
/* { dg-mips-options "-O2 -mips32r2 -mdsp" } */
/* { dg-final { scan-assembler-times "\tdpsq_sa.l.w\t\\\$ac" 2 } } */

_Sat long _Fract
f1 (_Sat _Fract x, _Sat _Fract y, _Sat long _Fract z)
{
  return z - (_Sat long _Fract) x * y;
}

_Sat long _Fract
f2 (_Sat _Fract x, _Sat _Fract y, _Sat long _Fract z)
{
  _Sat long _Fract t = (_Sat long _Fract) x * y;
  int temp = 5;
  if (temp == 5)
    z -= t;
  return z;
}

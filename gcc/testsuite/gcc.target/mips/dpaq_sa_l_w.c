/* { dg-do compile } */
/* { dg-mips-options "-O2 -mips32r2 -mdsp" } */
/* { dg-final { scan-assembler-times "\tdpaq_sa.l.w\t\\\$ac" 3 } } */

_Sat long _Fract
f1 (_Sat _Fract x, _Sat _Fract y, _Sat long _Fract z)
{
  return (_Sat long _Fract) x * y + z;
}

_Sat long _Fract
f2 (_Sat _Fract x, _Sat _Fract y, _Sat long _Fract z)
{
  return z + (_Sat long _Fract) y * x;
}

_Sat long _Fract
f3 (_Sat _Fract x, _Sat _Fract y, _Sat long _Fract z)
{
  _Sat long _Fract t = (_Sat long _Fract) x * y;
  int temp = 5;
  if (temp == 5)
    z = t + z; /* Need to put z at the end.  GCC does not swap operands to
		  match the ssmadd pattern, because types are saturating.  */
  return z;
}

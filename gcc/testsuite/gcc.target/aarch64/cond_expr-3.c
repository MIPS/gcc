/* { dg-do compile } */
/* { dg-options "-O2" }  */
/* { dg-final { scan-assembler-times "ccmp\t" 2 } } */
/* { dg-final { scan-assembler-times "cset\t" 2 } } */
int
g (int a, int b)
{
  int res;
  if (a && b)
    { res = 73; goto doReturn; }
  res = 72;
  doReturn: return res;
}

int
f (int a, int b)
{
  if (a && b)
    return 73;
  return 72;
}

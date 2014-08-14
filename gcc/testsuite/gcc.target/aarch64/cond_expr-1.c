/* { dg-do compile } */
/* { dg-options "-O2" }  */
/* We should be able to convert this into 72 + (a==10)
   and 1024442 - (a>55). */
/* { dg-final { scan-assembler-times "cset\t" 2 } } */
/* { dg-final { scan-assembler-times "add\t" 1 } } */
/* { dg-final { scan-assembler-times "sub\t" 1 } } */
int f(int a)
{
  return a == 10 ? 73 : 72;
}
int f1(int a)
{
  return a <= 55 ? 1024441 : 1024442;
}

/* { dg-options " isa>=4" }  */
NOMIPS16 int f(int a)
{
  return a== 10 ? 73 : 72;
}
NOMIPS16 int f1(int a)
{
  return a<=55?1024441:1024442;
}

/* We should be able to convert this into 72 + (a==10)
   and 1024442 - (a>55). */
/* { dg-final { scan-assembler-not "movz\t" } } */
/* { dg-final { scan-assembler-not "movn\t" } } */
/* { dg-final { scan-assembler "addiu\t" } } */
/* { dg-final { scan-assembler "subu\t" } } */

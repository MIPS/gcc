/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* The sign extend to be able to store into d should be free,
   there should be no sll or move here. */
/* { dg-final { scan-assembler-not "\tsll\t" } } */
/* { dg-final { scan-assembler-times "andi\t" 1 { xfail ilp32 } } } */

char Ch_1_Glob;
int Func_1 (long long Ch_1_Par_Val, long long Ch_2_Par_Val)
{
  unsigned char Ch_1_Loc;
  unsigned char Ch_2_Loc;

  Ch_1_Loc = Ch_1_Par_Val;
  Ch_2_Loc = Ch_1_Loc;
  if (Ch_2_Loc != Ch_2_Par_Val)
    Ch_1_Glob = Ch_1_Loc;
  return (0);
}

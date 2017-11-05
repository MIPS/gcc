/* { dg-do compile } */
/* { dg-options "-O2 -mpc-relative-literal-loads" } */

__int128
t (void)
{
  return (__int128)1 << 80;
}

/* { dg-final { scan-assembler "\tmov\tx0, 0" } } */
/* { dg-final { scan-assembler "\tmov\tx1, 65536" } } */

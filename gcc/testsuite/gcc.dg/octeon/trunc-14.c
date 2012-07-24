/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* Second operand of shift is always int.  */
/* { dg-final { scan-assembler-not "sll" } } */

int
my_ffsl (unsigned long x)
{
  int i;
  if (x == 0)
    return 0;
  for (i = 0; i < 8 * sizeof (unsigned long); i++)
    if (x & ((unsigned long) 1 << i))
      break;
  return i + 1;
}

/* Check if we expand seq and sne.  */

/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O0" } */
/* { dg-final { scan-assembler-times "seq\t|seqi\t" 4 } } */
/* { dg-final { scan-assembler-times "sne\t|snei\t" 4 } } */

long
f1 (long a, long b)
{
  return a == b;
}

long
f2 (long a)
{
  return a == 511;
}

int
f3 (int a, int b)
{
  return a == b;
}

int
f4 (int a)
{
  return a == 511;
}

long
g1 (long a, long b)
{
  return a != b;
}

long
g2 (long a)
{
  return a != 511;
}

int
g3 (int a, int b)
{
  return a != b;
}

int
g4 (int a)
{
  return a != 511;
}

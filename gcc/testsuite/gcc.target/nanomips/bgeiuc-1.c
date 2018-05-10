/* { dg-options "-m32 isa_rev>=6" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

int
foo (int *x, int y)
{
  int z = *x;
  if (y > z)
    while (y > z)
      z *= 2;
  return z;
}

int
bar (int i, int j, int k)
{
  if (i >= 8 && i <= 15)
    k = 16 + i;
  else if (i >= 4 && i <= 7)
    k = 16 - i;
  else
    k = 16;
  if (j != k)
    return 1;
  j = foo (&i, 7);

    
  return 0;
}

/* { dg-final { scan-assembler "\tbgeiuc\t\\\$a\[0-9\]+,\[4|8\],\[\\\$.\]L" } } */

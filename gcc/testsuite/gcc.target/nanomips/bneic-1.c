/* { dg-options "-m32 isa_rev>=6" } */

extern void abort (void);

void baz (int a, int b, int c)
{
  if (a != 176 || b != 52 || c != 31)
    abort ();
}

/* { dg-final { scan-assembler "\tbneic\t\\\$" } } */

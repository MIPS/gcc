/* { dg-options "-m32 isa_rev>=6" } */

extern void abort (void);

void baz (int a, int b)
{
  if (a != 176 || b != 52)
    abort ();
}

/* { dg-final { scan-assembler "\tbnec\t\\\$" } } */

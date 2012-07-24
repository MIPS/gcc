/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-not "sll" } } */

void abort (void);

int
f (int a, int b, int c, _Bool d, _Bool e, _Bool f, char g)
{
  if (g != 1 || d != 1 || e != 1 || f != 1)
    abort ();
  return a + b + c;
}

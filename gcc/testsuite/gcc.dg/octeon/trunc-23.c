/* { dg-do run { target *-octeon-* } } */
/* { dg-options "-O2" } */

void abort (void);

__attribute__ ((noinline)) void
f (int i)
{
  if (i != 0x12345678)
    abort ();
  asm ("");
}

__attribute__ ((noinline)) void
g (long long a)
{
  f (a);
  asm ("");
}

main ()
{
  g (0x1234567812345678ll);
  return 0;
}

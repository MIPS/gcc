/* { dg-do run { target *-octeon-* } } */
/* { dg-options "-O1" } */

void abort (void);

unsigned
shiftrt (unsigned long long a, unsigned b)
{
  int c = a;
  return c >> b;
}

int
main ()
{
  unsigned a = shiftrt (0x100000000ULL, 1);
  /* The only way MSB can be set if it was set after the 32-bit
     truncatation.  But in that case MSB-1 should be set too.  */
  if ((a >> 31) != ((a >> 30) & 1))
    abort ();

  return 0;
}

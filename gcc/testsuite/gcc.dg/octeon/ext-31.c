/* Minimized version of gcc.c-torture/execute/bitfld-1.c.  */

/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-not "div" } } */

void abort (void);

int
main (int argc, char *argv[])
{
  struct x
  {
    signed int i:7;
    unsigned int u:7;
  }
  bit = {0, 0};
  int i;
  unsigned int unsigned_result = -13U % 61;
  int signed_result = -13 % 61;
  bit.i = -13, i = -13;
  if (i % bit.u != signed_result)
    abort ();
  if (i % (unsigned int) bit.u != unsigned_result)
    abort ();
}

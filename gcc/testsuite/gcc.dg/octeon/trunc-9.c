/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-not "\[d\t\]sll\t|andi" } } */

void abort (void);

void
testc (unsigned char c, int ok)
{
  if ((c >= 1) && (c <= 127))
    {
      if (!ok)
        abort ();
    }
  else if (ok)
    abort ();
}

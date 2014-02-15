/* If we don't expand seq directly combine won't materialize it in
   some cases either. */

/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-not "xor" } } */

unsigned m (unsigned e);

f (unsigned i)
{
  unsigned j = m (i);
  h (j, i != j);
}

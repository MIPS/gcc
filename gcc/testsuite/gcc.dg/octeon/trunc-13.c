/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-not "seh"  } } */
/* { dg-final { scan-assembler-not "sll" } } */

/* The seh should not be required as the argument is already sign extended. */

long
f (short a, short b)
{
  return (long) a / b;
}

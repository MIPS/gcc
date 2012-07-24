/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* A test to make sure anding works for TI mode. */
typedef int TItype __attribute__ ((mode (TI)));
typedef unsigned int UTItype __attribute__ ((mode (TI)));
TItype __floattisf (TItype u)
{
   u &= ~ (((UTItype) 1 << 15) - 1);
  return u;
}

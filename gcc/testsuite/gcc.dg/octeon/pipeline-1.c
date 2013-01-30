/* Check that we use the octeon pipeline description.  */
/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2 -fdump-rtl-sched2 -march=octeon" } */

int f (int a, int b)
{
  return a / b;
}

/* { dg-final { scan-rtl-dump "octeon_mult\\*71" "sched2" } }  */
/* { dg-final { cleanup-rtl-dump "sched2" } }  */

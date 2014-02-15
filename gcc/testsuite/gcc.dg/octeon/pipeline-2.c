/* Check that we use the octeon2 pipeline description.  */
/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2 -fdump-rtl-sched2 -march=octeon2" } */

int f (int a, int b)
{
  return a / b;
}

/* { dg-final { scan-rtl-dump "octeon_mult\\*17" "sched2" } }  */
/* { dg-final { cleanup-rtl-dump "sched2" } }  */

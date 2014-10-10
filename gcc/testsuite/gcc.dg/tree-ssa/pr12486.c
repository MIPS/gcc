/* { dg-do compile } */
/* { dg-options "-O3 -fdump-tree-phiopt3" } */

signed char r;
void compute_wNAF(int neg, int d)
{
 int sign = 1;
 if (neg != 0)
  sign = -1;
 if (d)
    r = sign * d;
}

/* { dg-final { scan-tree-dump-times "'*'" 0 "phiopt3"} } */
/* { dg-final { cleanup-tree-dump "phiopt3" } } */

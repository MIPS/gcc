/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-vrp-details" } */

long long
foo (long long a, signed char b, signed char c)
{
  int bc = b * c;
  return a + (short)bc;
}

/* { dg-final { scan-tree-dump "Folded into" "vrp" { target int32plus } } } */
/* { dg-final { scan-tree-dump "Folding statement: _\[0-9\]\* = \\(long long int\\) bc_\[0-9\]\*;" "vrp" { target int16 } } } */
/* { dg-final { cleanup-tree-dump "vrp" } } */

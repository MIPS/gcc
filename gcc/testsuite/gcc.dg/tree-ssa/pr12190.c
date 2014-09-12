/* { dg-do compile } */
/* { dg-options "-O1 -fdump-tree-phiopt2" } */

int f (int t, int p, int a, int b)
{
   int x = a * (t ? 0 : (p ? -1 : 1)) + b;
   return x;
}

/* { dg-final { scan-tree-dump-times "if " 2 "phiopt2"} } */
/* { dg-final { cleanup-tree-dump "phiopt2" } } */

/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-store_ccp-details" } */

#define N 256

struct
{
  int x;
  int y;
} S[100];

int z[100];

int
foo (void)
{
  int i;
  int x, y;

  S[5].x = 0;
  S[5].y = 0;

  x = S[5].x;
  
  x = x + z[0];
  y = z[0];
  
  return x + y;
}

/* { dg-final { scan-tree-dump-times "Lattice value changed to CONSTANT 0" 3 "store_ccp1" } } */
/* { dg-final { cleanup-tree-dump "store_ccp*" } } */


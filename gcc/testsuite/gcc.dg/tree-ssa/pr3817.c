/* { dg-do compile } */
/* { dg-options "-O1 -fdump-tree-optimized" } */

int f(int a, int b)
{
  return b * (a?1:-1);
}

int f0(int a, int b)
{
  return  (a?b:-b);
}

int f1( int a, int b)
{
   int b1 = ( b * ( a>b? -1 : 1));
   return b1 ;
}

int f10( int a, int b)
{
   int b1 =  ( a>b? -b : b);
   return b1 ;
}

/* { dg-final { scan-tree-dump-times "if " 0 "optimized"} } */
/* { dg-final { cleanup-tree-dump "optimized" } } */

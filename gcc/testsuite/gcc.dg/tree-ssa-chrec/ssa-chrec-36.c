/* { dg-do compile } */ 
/* { dg-options "-O1 -fscalar-evolutions -fdump-tree-scev-stats -fall-data-deps -fdump-tree-ddall" } */

int foo (int);

int main ()
{
  int res;
  int c[100][200];
  int a;
  int x;
  
  for (a = 1; a < 50; a++)
    {
      c[a+1][a] = 2;
      res += c[a][a];
      
      /* This case exercises the subscript coupling detection: the dependence
	 detectors have to determine that there is no dependence between 
	 c[a+1][a] and c[a][a].  */
    }
  
  return res + foo (c[12][13]);
}

/* This also exercises the case when, after a PRE, the loop phi node contains:
   "  #   a_1 = PHI <1(0), T.1_11(1)>;
   ...
   T.1_11 = a_1 + 1;". 
   In fact this creates a cycle:  a -> T.1 -> a.
   The PRE has screwed up the case...
   ...I really have to implement the mixers analyzers.  */

/* { dg-final { diff-tree-dumps "ddall" } } */


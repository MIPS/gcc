/* { dg-do compile } */ 
/* { dg-options "-O1 -fscalar-evolutions -fno-tree-ch -fdump-tree-scev" } */


int bar (void);

int foo ()
{
  int a = -100;
  
  /* This exercises a code with two loop nests.  */
  
  while (a < 0)
    a++;
  
  a -= 77;
  
  while (a < 0)
    a+=3;
}

/* The analyzer sees two loop nests:
   for the first, it determines the evolution:
   a  ->  {-100, +, 1}_1
   
   and for the second, it determines that the first loop ends at 0 and then:
   a  ->  {-77, +, 3}_2
   
   When the constant propagation is postponed, the analyzer detects
   for the second loop the evolution function:
   a  ->  {a_5, +, 3}_2

*/

/* { dg-final { diff-tree-dumps "scev" } } */



/* { dg-do compile } */ 
/* { dg-options "-O1 -fscalar-evolutions -fdump-scalar-evolutions -fall-data-deps -fdump-all-data-deps" } */

void bar (int);

int foo (void)
{
  int a;
  int x;
  int c[100][100];
  
  for (a = 11; a < 50; a++)
    {
      /* Array access functions have to be analyzed.  */
      x = a + 5;
      c[x][1] = c[x+2][3] + c[x-1][2];
    }
  bar (c[1][2]);
}

/* The analyzer has to detect the access functions.
   For the access in "x+2" the analyzer has to dump the following:
   Access function 1: {18, +, 1}_1
*/

/* { dg-final { diff-tree-dumps "scev" } } */
/* { dg-final { diff-tree-dumps "alldd" } } */

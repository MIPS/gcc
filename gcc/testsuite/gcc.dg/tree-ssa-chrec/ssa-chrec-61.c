/* { dg-do compile } */ 
/* { dg-options "-O1 -floop-test -fdump-tree-lptest-details " } */

short A[200];

void bar (int);
void foo ()
{
  int j;

  for (j = 0; j < 32; j++) 
    {
      //A[5*j] = A[7*j+3] + 1;  // x0 = 1, y0 = 2, last_conflict = 4
      //A[7*j+3] = A[5*j] + 1;  // x0 = 2, y0 = 1, last_conflict = 4

      //A[7*j+3] = A[3*j] + 1;  // x0 = 0, y0 = 1, last_conflict = 4
      //A[3*j] = A[7*j+3] + 1;  // x0 = 1, y0 = 0, last_conflict = 4

      //A[3*j+5] = A[5*j+7] + 1; // x0 = 2, y0 = 4, last_conflict = 5
      A[5*j+7] = A[3*j+5] + 1; // x0 = 4, y0 = 2, last_conflict = 5
      
    }

  bar (A[10]);
}


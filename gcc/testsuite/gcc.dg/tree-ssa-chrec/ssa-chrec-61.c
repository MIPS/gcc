/* { dg-do compile } */ 
/* { dg-options "-O1 -floop-test -fdump-tree-lptest-details " } */

void bar (int);

#define N 100
#define NPad 10
#define M 32

void foo ()
{
  short input[2*N];
  int j;
  
  for (j = 0; j < M; j++) 
    {
      input[5*j] += input[7*j+3];
    }
  bar (input[10]);
}


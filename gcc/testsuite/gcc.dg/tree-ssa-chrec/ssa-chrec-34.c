/* { dg-do compile } */ 
/* { dg-options "-O1 -fscalar-evolutions -fdump-scalar-evolutions -fall-data-deps -fdump-all-data-deps" } */

void bar (int);

#define M 16
#define N 8

short foo (short image[][M], short block[][N]){
  int sad, diff = 0;                                
  int i, j;        
  int tmp;

  for (i = 0; i < N; i++) {
    sad = 0;          
    for (j = 0; j < N; j++) {
      tmp = image[i][j] - block[i][j];
      sad += (tmp < 0) ? -tmp : tmp;
    }
    diff += sad; 
  }

  return diff;
}

/* { dg-final { diff-tree-dumps "scev" } } */
/* { dg-final { diff-tree-dumps "alldd" } } */

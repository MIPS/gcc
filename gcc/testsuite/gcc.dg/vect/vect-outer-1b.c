/* { dg-do compile } */

#define N 40
signed short image[N][N];
signed short block[N][N];
signed short out[N];

/* Outer-loop cannot get vectorized because of non-consecutive access.
   Currently doesn't get vectotized because the accesses in the inner-loop
   are misaligend.  */

void
foo (){
  int i,j;
  int diff;

  for (i = 0; i < N; i++) {
    diff = 0;
    for (j = 0; j < N; j+=4) {
      diff += (image[i][j] - block[i][j]);
    }
    out[i]=diff;
  }
}

/* { dg-final { scan-tree-dump-times "OUTER LOOP VECTORIZED" 1 "vect" { xfail *-*-* } } } */
/* FORNOW */
/* { dg-final { scan-tree-dump-times "strided access in outer loop" 1 "vect" { xfail *-*-* } } } */
/* { dg-final { scan-tree-dump-times "inner step doesn't divide the vector-size" 1 "vect" } } */
/* { dg-final { cleanup-tree-dump "vect" } } */

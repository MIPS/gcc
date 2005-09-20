/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -ftree-vectorize -fdump-tree-vect-stats -maltivec -fno-tree-vect-loop-version" { target powerpc*-*-* } } */

#define WSIZE 32768
#define SIZE 1024

foo (unsigned short *head)
{
  unsigned m, n;
  for (n = 0; n < SIZE; n++) {
    m = head[n];
    head[n] = (unsigned short)(m >= WSIZE ? m-WSIZE : 0);
  }
}

/* Fails until eliminate_unnecessary_casts is enabled again */
/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 1 "vect" { xfail *-*-* } } } */
/* { dg-final { cleanup-tree-dump "vect" } } */

/* { dg-do compile { target powerpc*-*-* } } */
#define N 64

void
foo (short * __restrict__ pmetric, 
     short * __restrict__ p1, 
     short * __restrict__ p2,
     short * __restrict__ out1,
     short * __restrict__ out2)
{
  int i;
  short metric, m1, m2;
  short x1, x2;
  short y1, y2;

  for (i = 0; i < N; i++) {
    metric = *pmetric++;

    m1 = *p1 - metric;
    m2 = *p2 + metric;
    if (m1 >= m2) {
       x1 = m1;
       y1 = m1>>1;	
    }
    else {
       x1 = m2;
       y1 = m2>>1;	
    }

    m1 = *p1 + metric;
    m2 = *p2 - metric;
    if (m1 >= m2) {
       x2 = m1;
       y2 = m1>>1;	
    }
    else {
       x2 = m2;
       y2 = m2>>1;	
    }
    *out1 = x1;
    *out2 = y1;
    out1++;
    out2++;
    *out1 = x2;
    *out2 = y2;
    out1++;
    out2++;
    p1++;
    p2++;
  }
}

/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 1 "vect" } } */
/* { dg-final { cleanup-tree-dump "vect" } } */

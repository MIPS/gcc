
#pragma omp declare simd aligned (g : sizeof (*g)) linear (h : 2 * sizeof (g[0]) + sizeof (h)) simdlen (4)
int f16 (long *g, int h);

#pragma omp declare simd aligned (h : sizeof (*h)) linear (g : 2 * sizeof (h[0]) + sizeof (g)) simdlen (4)
int f17 (int g, long *h)
{
  return g + h[0];
}

#pragma omp declare simd aligned (i : sizeof (*i)) linear (j : 2 * sizeof (i[0]) + sizeof (j)) simdlen (4)
int
f18 (j, i)
     long *i;
     int j;
{
  return j + i[0];
}

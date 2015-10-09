/* { dg-do compile } */
/* { dg-options "-fopenmp" } */

void
foo (int *p, int (*q)[10], int r[10], int s[10][10])
{
  int a[10], b[10][10];
  #pragma omp target map (tofrom: p[-1:2])
  ;
  #pragma omp target map (tofrom: q[-1:2][0:10])
  ;
  #pragma omp target map (tofrom: q[-1:2][-2:10]) /* { dg-error "negative low bound in array section in" } */
  ;
  #pragma omp target map (tofrom: r[-1:2])
  ;
  #pragma omp target map (tofrom: s[-1:2][:])
  ;
  #pragma omp target map (tofrom: s[-1:2][-2:10]) /* { dg-error "negative low bound in array section in" } */
  ;
  #pragma omp target map (tofrom: a[-1:2])	 /* { dg-error "negative low bound in array section in" } */
  ;
  #pragma omp target map (tofrom: b[-1:2][0:])	 /* { dg-error "negative low bound in array section in" } */
  ;
  #pragma omp target map (tofrom: b[1:2][-2:10]) /* { dg-error "negative low bound in array section in" } */
  ;
}

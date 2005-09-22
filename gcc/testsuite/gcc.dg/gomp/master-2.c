/* { dg-do compile } */

void f1(void)
{
  #pragma omp master asdf	/* { dg-error "expected new-line" } */
  #pragma omp master
}				/* { dg-error "expected expression" } */

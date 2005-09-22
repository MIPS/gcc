/* { dg-do compile } */

void f1(void)
{
  #pragma omp ordered asdf	/* { dg-error "expected new-line" } */
  #pragma omp ordered
}				/* { dg-error "expected expression" } */

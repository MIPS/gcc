/* { dg-do compile } */

int x;
const int y;
int bar(void);

void f1(void)
{
  register int z;

  #pragma omp atomic
    x %= 2;		/* { dg-error "invalid operator" } */
  #pragma omp atomic
    x = x + 1;		/* { dg-error "invalid operator" } */
  #pragma omp atomic
    x = 1;		/* { dg-error "invalid operator" } */
  #pragma omp atomic
    ++y;		/* { dg-error "read-only variable" } */
  #pragma omp atomic
    y--;		/* { dg-error "read-only variable" } */
  #pragma omp atomic
    y += 1;		/* { dg-error "read-only variable" } */
  #pragma omp atomic
    z += 1;		/* { dg-error "register variable" } */
  #pragma omp atomic
    bar();		/* { dg-error "invalid operator" } */
  #pragma omp atomic
    bar() += 1;		/* { dg-error "invalid lvalue" } */
  {
  #pragma omp atomic
    x
  }			/* { dg-error "invalid operator" } */
  #pragma omp atomic a	/* { dg-error "expected new-line" } */
    ;			/* { dg-error "expected expression" } */
  #pragma omp atomic	/* { dg-error "expected expression" } */
  #pragma omp atomic
}

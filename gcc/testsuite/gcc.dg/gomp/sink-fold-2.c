/* { dg-do compile } */

int i,j, N;

extern void bar();

void
funk ()
{
#pragma omp parallel for ordered(2)
  for (i=0; i < N; i += 3)
    for (j=0; j < N; ++j)
    {
#pragma omp ordered depend(sink:i-8,j-1) /* { dg-warning "ignoring sink clause with offset that is not a multiple" } */
#pragma omp ordered depend(sink:i+3,j-1) /* { dg-error "first offset must be in opposite direction" } */
        bar();
#pragma omp ordered depend(source)
    }
}

/* { dg-additional-options "-Wuninitialized" } */

int
main (void)
{
  int i, j, k;

  #pragma acc parallel loop gang num_gangs(i) /* { dg-warning "is used uninitialized in this function" } */
  for (i = 0; i < 1; i++)
    ;

  #pragma acc parallel loop worker num_workers(j) /* { dg-warning "is used uninitialized in this function" } */
  for (j = 0; j < 1; j++)
    ;

  #pragma acc parallel loop vector vector_length(k) /* { dg-warning "is used uninitialized in this function" } */
  for (k = 0; k < 1; k++)
    ;

  return 0;
}

/* Test the output of -fopt-info-note-omp.  */

/* { dg-additional-options "-fopt-info-note-omp" } */

int
main ()
{
  int x, y, z;

#pragma acc parallel loop seq /* { dg-message "note: Detected parallelism <acc loop seq>" } */
  for (x = 0; x < 10; x++)
    ;

#pragma acc parallel loop gang /* { dg-message "note: Detected parallelism <acc loop gang>" } */
  for (x = 0; x < 10; x++)
    ;

#pragma acc parallel loop worker /* { dg-message "note: Detected parallelism <acc loop worker>" } */
  for (x = 0; x < 10; x++)
    ;

#pragma acc parallel loop vector /* { dg-message "note: Detected parallelism <acc loop vector>" } */
  for (x = 0; x < 10; x++)
    ;

#pragma acc parallel loop gang vector /* { dg-message "note: Detected parallelism <acc loop gang vector>" } */
  for (x = 0; x < 10; x++)
    ;

#pragma acc parallel loop gang worker /* { dg-message "note: Detected parallelism <acc loop gang worker>" } */
  for (x = 0; x < 10; x++)
    ;

#pragma acc parallel loop worker vector /* { dg-message "note: Detected parallelism <acc loop worker vector>" } */
  for (x = 0; x < 10; x++)
    ;

#pragma acc parallel loop gang worker vector /* { dg-message "note: Detected parallelism <acc loop gang worker vector>" } */
  for (x = 0; x < 10; x++)
    ;

#pragma acc parallel loop /* { dg-message "note: Detected parallelism <acc loop gang vector>" } */
  for (x = 0; x < 10; x++)
    ;

#pragma acc parallel loop /* { dg-message "note: Detected parallelism <acc loop gang worker>" } */
  for (x = 0; x < 10; x++)
#pragma acc loop /* { dg-message "note: Detected parallelism <acc loop vector>" } */
    for (y = 0; y < 10; y++)
      ;

#pragma acc parallel loop gang /* { dg-message "note: Detected parallelism <acc loop gang>" } */
  for (x = 0; x < 10; x++)
#pragma acc loop worker /* { dg-message "note: Detected parallelism <acc loop worker>" } */
    for (y = 0; y < 10; y++)
#pragma acc loop vector /* { dg-message "note: Detected parallelism <acc loop vector>" } */
      for (z = 0; z < 10; z++)
	;

  return 0;
}

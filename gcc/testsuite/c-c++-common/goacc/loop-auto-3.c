/* Ensure that the auto clause falls back to seq parallelism when the
   OpenACC loop is not explicitly independent.  */

/* { dg-additional-options "-fopt-info-note-omp" } */

void
test ()
{
  int i, j, k, l, n = 100;
  
#pragma acc parallel loop auto /* { dg-message "Detected parallelism <acc loop seq>" } */
  for (i = 0; i < n; i++)
#pragma acc loop auto independent /* { dg-message "Detected parallelism <acc loop gang>" } */
    for (j = 0; j < n; j++)
#pragma acc loop worker vector /* { dg-message "Detected parallelism <acc loop worker vector>" } */
      for (k = 0; k < n; k++)
	;

#pragma acc parallel loop auto independent /* { dg-message "Detected parallelism <acc loop gang worker>" } */
  for (i = 0; i < n; i++)
#pragma acc loop auto /* { dg-message "Detected parallelism <acc loop seq>" } */
    for (j = 0; j < n; j++)
#pragma acc loop auto /* { dg-message "Detected parallelism <acc loop seq>" } */
      for (k = 0; k < n; k++)
#pragma acc loop auto independent /* { dg-message "Detected parallelism <acc loop vector>" } */
	for (l = 0; l < n; l++)
	  ;

#pragma acc parallel loop gang /* { dg-message "Detected parallelism <acc loop gang>" } */
  for (i = 0; i < n; i++)
#pragma acc loop worker /* { dg-message "Detected parallelism <acc loop worker>" } */
    for (j = 0; j < n; j++)
#pragma acc loop vector /* { dg-message "Detected parallelism <acc loop vector>" } */
      for (k = 0; k < n; k++)
	{
#pragma acc loop auto independent /* { dg-message "Detected parallelism <acc loop seq>" } */
	  /* { dg-warning "insufficient partitioning available to parallelize loop" "" { target *-*-* } .-1 } */
	  for (l = 0; l < n; l++)
	    ;
#pragma acc loop auto /* { dg-message "Detected parallelism <acc loop seq>" } */
	  for (l = 0; l < n; l++)
	    ;
	}

#pragma acc parallel loop /* { dg-message "Detected parallelism <acc loop seq>" } */
  /* { dg-warning "insufficient partitioning available to parallelize loop" "" { target *-*-* } .-1 } */
  for (i = 0; i < n; i++)
    {
#pragma acc loop gang worker /* { dg-message "Detected parallelism <acc loop gang worker>" } */
      for (j = 0; j < n; j++)
#pragma acc loop auto /* { dg-message "Detected parallelism <acc loop seq>" } */
	for (k = 0; k < n; k++)
	  {
#pragma acc loop vector /* { dg-message "Detected parallelism <acc loop vector>" } */
	    for (l = 0; l < n; l++)
	      ;
#pragma acc loop auto independent /* { dg-message "Detected parallelism <acc loop vector>" } */
	    for (l = 0; l < n; l++)
	      ;
	  }
#pragma acc loop worker /* { dg-message "Detected parallelism <acc loop worker>" } */
      for (j = 0; j < n; j++)
#pragma acc loop vector /* { dg-message "Detected parallelism <acc loop vector>" } */
	for (k = 0; k < n; k++)
	  ;
    }

#pragma acc parallel loop /* { dg-message "Detected parallelism <acc loop gang>" } */
  for (i = 0; i < n; i++)
#pragma acc loop /* { dg-message "Detected parallelism <acc loop worker>" } */
    for (j = 0; j < n; j++)
#pragma acc loop /* { dg-message "Detected parallelism <acc loop seq>" } */
      /* { dg-warning "insufficient partitioning available to parallelize loop" "" { target *-*-* } .-1 } */
      for (k = 0; k < n; k++)
#pragma acc loop /* { dg-message "Detected parallelism <acc loop vector>" } */
	  for (l = 0; l < n; l++)
	    ;
}

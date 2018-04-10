/* Verify the default assignment of parallelism.  */

/* { dg-do compile }  */
/* { dg-additional-options "-fopenacc -misa=sm_35 -fopt-info-note-omp" } */

void
gang_independent ()
{
  int i, j;

  #pragma acc parallel
  #pragma acc loop gang /* { dg-message "Detected parallelism <acc loop gang>" } */
  for (i = 0; i < 10; i++)
    #pragma acc loop /* { dg-message "Detected parallelism <acc loop worker vector>" } */
    for (j = 0; j < 10; j++) 
      ;

  #pragma acc parallel vector_length (128)
  #pragma acc loop gang /* { dg-message "Detected parallelism <acc loop gang>" } */
  for (i = 0; i < 10; i++)
    #pragma acc loop /* { dg-message "Detected parallelism <acc loop vector>" } */
    for (j = 0; j < 10; j++) 
      ;
}

void
gang_independent_seq ()
{
  int i, j, k;

  #pragma acc parallel
  #pragma acc loop gang /* { dg-message "Detected parallelism <acc loop gang>" } */
  for (i = 0; i < 10; i++)
    #pragma acc loop /* { dg-message "Detected parallelism <acc loop worker vector>" } */
    for (j = 0; j < 10; j++)
      #pragma acc loop seq /* { dg-message "Detected parallelism <acc loop seq>" } */
      for (k = 0; k < 10; k++)
	;

  #pragma acc parallel vector_length (128)
  #pragma acc loop gang /* { dg-message "Detected parallelism <acc loop gang>" } */
  for (i = 0; i < 10; i++)
    #pragma acc loop /* { dg-message "Detected parallelism <acc loop vector>" } */
    for (j = 0; j < 10; j++)
      #pragma acc loop seq /* { dg-message "Detected parallelism <acc loop seq>" } */
      for (k = 0; k < 10; k++)
	;
}

void
worker ()
{
  int i, j;

  #pragma acc parallel
  #pragma acc loop worker /* { dg-message "Detected parallelism <acc loop worker>" } */
  for (i = 0; i < 10; i++)
    #pragma acc loop /* { dg-message "Detected parallelism <acc loop vector>" } */
    for (j = 0; j < 10; j++) 
      ;

  #pragma acc parallel vector_length (128)
  #pragma acc loop worker /* { dg-message "Detected parallelism <acc loop worker>" } */
  for (i = 0; i < 10; i++)
    #pragma acc loop /* { dg-message "Detected parallelism <acc loop vector>" } */
    for (j = 0; j < 10; j++) 
      ;
}

void
fully_independent ()
{
  int i, j, k;

  #pragma acc parallel
  #pragma acc loop /* { dg-message "Detected parallelism <acc loop gang vector>" } */
  for (i = 0; i < 10; i++)
    ;

  #pragma acc parallel vector_length (128)
  #pragma acc loop /* { dg-message "Detected parallelism <acc loop gang vector>" } */
  for (i = 0; i < 10; i++)
    ;

  #pragma acc parallel
  #pragma acc loop /* { dg-message "Detected parallelism <acc loop gang worker>" } */
  for (i = 0; i < 10; i++)
    #pragma acc loop /* { dg-message "Detected parallelism <acc loop vector>" } */
    for (j = 0; j < 10; j++) 
      ;

  /* FIXME: Should the outer loop only be gang partitioned so that the
     inner loopp can utilize a large vector_length?  */
  #pragma acc parallel vector_length (128)
  #pragma acc loop /* { dg-message "Detected parallelism <acc loop gang worker>" } */
  for (i = 0; i < 10; i++)
    #pragma acc loop /* { dg-message "Detected parallelism <acc loop vector>" } */
    for (j = 0; j < 10; j++) 
      ;

  #pragma acc parallel
  #pragma acc loop /* { dg-message "Detected parallelism <acc loop gang>" } */
  for (i = 0; i < 10; i++)
    #pragma acc loop /* { dg-message "Detected parallelism <acc loop worker>" } */
    for (j = 0; j < 10; j++) 
      #pragma acc loop /* { dg-message "Detected parallelism <acc loop vector>" } */
      for (k = 0; k < 10; k++) 
	;

  /* FIXME: Should the middle loop be seq-partitioned in order to
     respect vector_length = 128 on the innermost loop?  */
  #pragma acc parallel vector_length (128)
  #pragma acc loop /* { dg-message "Detected parallelism <acc loop gang>" } */
  for (i = 0; i < 10; i++)
    #pragma acc loop /* { dg-message "Detected parallelism <acc loop worker>" } */
    for (j = 0; j < 10; j++) 
      #pragma acc loop /* { dg-message "Detected parallelism <acc loop vector>" } */
      for (k = 0; k < 10; k++) 
	;
}

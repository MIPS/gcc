/* Verify the default assignment of parallelism.  */

/* { dg-do compile }  */
/* { dg-additional-options "-fopenacc -fopt-info-optimized-omp" } */
/* { dg-skip-if "cc1: error: option -mgomp is not supported together with -fopenacc" { *-*-* } { "-mgomp" } } */

void
gang_independent ()
{
  int i, j;

  #pragma acc parallel
  #pragma acc loop gang /* { dg-message "note: assigned OpenACC gang loop parallelism" } */
  for (i = 0; i < 10; i++)
    #pragma acc loop /* { dg-message "note: assigned OpenACC worker vector loop parallelism" } */
    for (j = 0; j < 10; j++) 
      ;

  #pragma acc parallel vector_length (128)
  #pragma acc loop gang /* { dg-message "note: assigned OpenACC gang loop parallelism" } */
  for (i = 0; i < 10; i++)
    #pragma acc loop /* { dg-message "note: assigned OpenACC vector loop parallelism" } */
    for (j = 0; j < 10; j++) 
      ;
}

void
gang_independent_seq ()
{
  int i, j, k;

  #pragma acc parallel
  #pragma acc loop gang /* { dg-message "note: assigned OpenACC gang loop parallelism" } */
  for (i = 0; i < 10; i++)
    #pragma acc loop /* { dg-message "note: assigned OpenACC worker vector loop parallelism" } */
    for (j = 0; j < 10; j++)
      #pragma acc loop seq /* { dg-message "note: assigned OpenACC seq loop parallelism" } */
      for (k = 0; k < 10; k++)
	;

  #pragma acc parallel vector_length (128)
  #pragma acc loop gang /* { dg-message "note: assigned OpenACC gang loop parallelism" } */
  for (i = 0; i < 10; i++)
    #pragma acc loop /* { dg-message "note: assigned OpenACC vector loop parallelism" } */
    for (j = 0; j < 10; j++)
      #pragma acc loop seq /* { dg-message "note: assigned OpenACC seq loop parallelism" } */
      for (k = 0; k < 10; k++)
	;
}

void
worker ()
{
  int i, j;

  #pragma acc parallel
  #pragma acc loop worker /* { dg-message "note: assigned OpenACC worker loop parallelism" } */
  for (i = 0; i < 10; i++)
    #pragma acc loop /* { dg-message "note: assigned OpenACC vector loop parallelism" } */
    for (j = 0; j < 10; j++) 
      ;

  #pragma acc parallel vector_length (128)
  #pragma acc loop worker /* { dg-message "note: assigned OpenACC worker loop parallelism" } */
  for (i = 0; i < 10; i++)
    #pragma acc loop /* { dg-message "note: assigned OpenACC vector loop parallelism" } */
    for (j = 0; j < 10; j++) 
      ;
}

void
fully_independent ()
{
  int i, j, k;

  #pragma acc parallel
  #pragma acc loop /* { dg-message "note: assigned OpenACC gang vector loop parallelism" } */
  for (i = 0; i < 10; i++)
    ;

  #pragma acc parallel vector_length (128)
  #pragma acc loop /* { dg-message "note: assigned OpenACC gang vector loop parallelism" } */
  for (i = 0; i < 10; i++)
    ;

  #pragma acc parallel
  #pragma acc loop /* { dg-message "note: assigned OpenACC gang worker loop parallelism" } */
  for (i = 0; i < 10; i++)
    #pragma acc loop /* { dg-message "note: assigned OpenACC vector loop parallelism" } */
    for (j = 0; j < 10; j++) 
      ;

  /* FIXME: Should the outer loop only be gang partitioned so that the
     inner loopp can utilize a large vector_length?  */
  #pragma acc parallel vector_length (128)
  #pragma acc loop /* { dg-message "note: assigned OpenACC gang worker loop parallelism" } */
  for (i = 0; i < 10; i++)
    #pragma acc loop /* { dg-message "note: assigned OpenACC vector loop parallelism" } */
    for (j = 0; j < 10; j++) 
      ;

  #pragma acc parallel
  #pragma acc loop /* { dg-message "note: assigned OpenACC gang loop parallelism" } */
  for (i = 0; i < 10; i++)
    #pragma acc loop /* { dg-message "note: assigned OpenACC worker loop parallelism" } */
    for (j = 0; j < 10; j++) 
      #pragma acc loop /* { dg-message "note: assigned OpenACC vector loop parallelism" } */
      for (k = 0; k < 10; k++) 
	;

  /* FIXME: Should the middle loop be seq-partitioned in order to
     respect vector_length = 128 on the innermost loop?  */
  #pragma acc parallel vector_length (128)
  #pragma acc loop /* { dg-message "note: assigned OpenACC gang loop parallelism" } */
  for (i = 0; i < 10; i++)
    #pragma acc loop /* { dg-message "note: assigned OpenACC worker loop parallelism" } */
    for (j = 0; j < 10; j++) 
      #pragma acc loop /* { dg-message "note: assigned OpenACC vector loop parallelism" } */
      for (k = 0; k < 10; k++) 
	;
}

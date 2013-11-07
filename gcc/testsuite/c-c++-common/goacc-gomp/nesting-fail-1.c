/* TODO: Some of these should either be allowed or fail with a more sensible
   error message.  */
void
f1 (void)
{
  int i;

#pragma omp parallel
  {
#pragma acc parallel	/* { dg-error "may not be nested" } */
    ;
  }

#pragma omp for
  for (i = 0; i < 3; i++)
    {
#pragma acc parallel	/* { dg-error "may not be nested" } */
      ;
    }

#pragma omp sections
  {
#pragma acc parallel	/* { dg-error "may not be nested" } */
    ;
  }

#pragma omp single
  {
#pragma acc parallel	/* { dg-error "may not be nested" } */
    ;
  }

#pragma omp task
  {
#pragma acc parallel	/* { dg-error "may not be nested" } */
    ;
  }

#pragma omp master
  {
#pragma acc parallel	/* { dg-error "may not be nested" } */
    ;
  }

#pragma omp critical
  {
#pragma acc parallel	/* { dg-error "may not be nested" } */
    ;
  }

#pragma omp ordered
  {
#pragma acc parallel	/* { dg-error "may not be nested" } */
    ;
  }
}

/* TODO: Some of these should either be allowed or fail with a more sensible
   error message.  */
void
f2 (void)
{
#pragma acc parallel
  {
#pragma omp parallel	/* { dg-error "may not be nested" } */
    ;
  }

#pragma acc parallel
  {
    int i;
#pragma omp for		/* { dg-error "may not be nested" } */
    for (i = 0; i < 3; i++)
      ;
  }

#pragma acc parallel
  {
#pragma omp sections	/* { dg-error "may not be nested" } */
    {
      ;
    }
  }

#pragma acc parallel
  {
#pragma omp single	/* { dg-error "may not be nested" } */
    ;
  }

#pragma acc parallel
  {
#pragma omp task	/* { dg-error "may not be nested" } */
    ;
  }

#pragma acc parallel
  {
#pragma omp master	/* { dg-error "may not be nested" } */
    ;
  }

#pragma acc parallel
  {
#pragma omp critical	/* { dg-error "may not be nested" } */
    ;
  }

#pragma acc parallel
  {
    int i;
#pragma omp atomic write
    i = 0;		/* { dg-error "may not be nested" } */
  }

#pragma acc parallel
  {
#pragma omp ordered	/* { dg-error "may not be nested" } */
    ;
  }
}

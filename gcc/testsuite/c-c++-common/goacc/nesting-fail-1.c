/* TODO: While the OpenACC specification does allow for certain kinds of
   nesting, we don't support many of these yet.  */
void
f_acc_parallel (void)
{
#pragma acc parallel
  {
#pragma acc parallel	/* { dg-error "may not be nested" } */
    ;
#pragma acc kernels	/* { dg-error "may not be nested" } */
    ;
#pragma acc data	/* { dg-error "may not be nested" } */
    ;
  }
}

/* TODO: While the OpenACC specification does allow for certain kinds of
   nesting, we don't support many of these yet.  */
void
f_acc_kernels (void)
{
#pragma acc kernels
  {
#pragma acc parallel	/* { dg-error "may not be nested" } */
    ;
#pragma acc kernels	/* { dg-error "may not be nested" } */
    ;
#pragma acc data	/* { dg-error "may not be nested" } */
    ;
  }
}

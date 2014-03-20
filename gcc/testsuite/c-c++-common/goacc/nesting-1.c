void
f_acc_data (void)
{
#pragma acc data
  {
#pragma acc parallel
    ;
#pragma acc kernels
    ;
#pragma acc data
    ;
  }
}

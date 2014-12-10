int i;

void
f_acc_data (void)
{
#pragma acc data
  {
#pragma acc update host(i)
#pragma acc enter data copyin(i)
  }
}

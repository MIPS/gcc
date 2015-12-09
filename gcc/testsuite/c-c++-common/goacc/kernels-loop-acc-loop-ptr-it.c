float b[10][15][10];

void
foo (void)
{
  float *i;

#pragma acc kernels
  {
#pragma acc loop
    for (i = &b[0][0][0]; i < &b[0][0][10]; i++)
      ;
  }
}

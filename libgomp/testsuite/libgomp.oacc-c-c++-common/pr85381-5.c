/* { dg-additional-options "-save-temps" } */
/* { dg-skip-if "" { *-*-* } { "*" } { "-DACC_DEVICE_TYPE_nvidia=1 -O2" } } */

#define n 1024

int
main (void)
{
  #pragma acc parallel vector_length(128)
  {
    #pragma acc loop vector
    for (int i = 0; i < n; i++)
      ;

    #pragma acc loop vector
    for (int i = 0; i < n; i++)
      ;
  }

  return 0;
}

/* { dg-final { scan-assembler-not "bar.sync" } } */

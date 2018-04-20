/* { dg-additional-options "-save-temps" } */
/* { dg-skip-if "" { *-*-* } { "*" } { "-DACC_DEVICE_TYPE_nvidia=1 -O2" } } */

int
main (void)
{
  int v1;

  #pragma acc parallel vector_length (128)
  #pragma acc loop vector
  for (v1 = 0; v1 < 20; v1 += 2)
    ;

  return 0;
}

/* { dg-final { scan-assembler-not "bar.sync" } } */

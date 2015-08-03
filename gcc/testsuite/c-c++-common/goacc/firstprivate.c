void
foo (void)
{
  int a, b[100];
#pragma acc parallel firstprivate (a, b)
    ;
#pragma acc parallel firstprivate (b[10:20]) /* { dg-error "expected" } */
    ;
}

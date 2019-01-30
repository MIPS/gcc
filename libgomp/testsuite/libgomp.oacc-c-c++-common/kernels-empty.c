/* { dg-additional-options "-fopenacc-kernels=parloops" } as this is
   specifically testing "parloops" handling.  */
/* Override the compiler's "avoid offloading" decision.
   { dg-additional-options "-foffload-force" } */

int
main (void)
{
#pragma acc kernels
  ;
}

#pragma acc routine
void
foo (void)
{
#pragma acc parallel /* { dg-error "OpenACC region inside of OpenACC routine, nested parallelism not supported yet" } */
  ;
}


/* TODO: While the OpenACC specification does allow for certain kinds of
   nesting, we don't support that yet.  */
void
f1 (void)
{
#pragma acc parallel
  {
#pragma acc parallel	/* { dg-error "may not be nested" } */
    ;
  }
}

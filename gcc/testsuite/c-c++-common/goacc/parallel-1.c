void
foo (void)
{
#pragma acc parallel
  foo ();
}

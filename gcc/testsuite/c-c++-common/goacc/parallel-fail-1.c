void
foo (void)
{
#pragma acc parallel foo	/* { dg-error "expected clause before 'foo'" } */
  foo ();
}

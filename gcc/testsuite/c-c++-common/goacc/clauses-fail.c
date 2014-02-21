void
f (void)
{
#pragma acc parallel one /* { dg-error "expected clause before 'one'" } */
  ;

#pragma acc data two /* { dg-error "expected clause before 'two'" } */
  ;
}

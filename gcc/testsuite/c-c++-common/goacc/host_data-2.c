/* Test invalid use of host_data directive.  */
/* { dg-do compile } */

int v0;
#pragma acc host_data use_device(v0) /* { dg-error "expected" } */

void
f (void)
{
  int v2 = 3;
#pragma acc host_data copy(v2) /* { dg-error "not valid for" } */
  ;
}
/* { dg-bogus "sorry, unimplemented: directive not yet implemented" "host_data" { xfail *-*-* } 11 } */

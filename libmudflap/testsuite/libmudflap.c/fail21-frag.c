#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main ()
{
int foo = 5;
int *bar = & foo;
/* Make an access here to get &foo into the lookup cache.  */
*bar = 5;
__mf_watch (& foo, sizeof(foo));
/* This access should trigger the watch violation.  */
*bar = 10;
return 0;
}
/* { dg-output "mudflap violation 1.*" } */
/* { dg-output "Nearby object 1.*" } */
/* { dg-output "mudflap object.*.main. foo.*" } */
/* { dg-do run { xfail *-*-* } } */

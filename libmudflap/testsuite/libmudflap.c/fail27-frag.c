#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char *foo (unsigned i)
{
  char buffer[10];
  char *k = i ? & buffer[i] : NULL; /* defeat addr-of-local-returned warning */
  return k;
}

int main ()
{
char *f = foo (5);
f[0] = 'b';

return 0;
}
/* { dg-output "mudflap violation 1.*" } */
/* { dg-output "Nearby object.*" } */
/* { dg-output "mudflap object.*buffer.*alloc.*dealloc" } */
/* { dg-do run { xfail *-*-* } } */

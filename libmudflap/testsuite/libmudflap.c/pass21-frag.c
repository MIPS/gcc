#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main ()
{
char *boo, *foo;
boo = (char *) alloca (100);
boo[99] = 'a';
foo = (char *) __builtin_alloca (200);
foo[44] = 'b';
return 0;
}

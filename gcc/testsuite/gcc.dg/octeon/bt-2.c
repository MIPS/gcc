/* { dg-do run { target *-octeon-linux* } } */
/* non-PIC no longer built with -export-dynamic by default so to get the
   symbols into the dynamic symbol table build with -rdynamic.  */
/* { dg-options "-funwind-tables -O2 -rdynamic" } */
/* { dg-output {#0 .*\(bt\+.* #1 .*\(bar+.* #2 .*\(bar+.* #3 .*\(foo+.* #4 .*\(main+} } */

#include <sys/types.h> /* __UCLIBC__ is defined  */
#include <stdio.h>

#ifndef __UCLIBC__
#include <execinfo.h>

void *pcs[10];

__attribute__ ((noinline))
void bt ()
{
  int i;
  char **funcs;
  int size;

  asm ("");
  size = backtrace (pcs, 10);
  funcs = backtrace_symbols (pcs, size);
  for (i = 0; i < size; i++)
    printf ("#%d %s ", i, funcs[i]);
}

int i = 1;

int
main ()
{
  foo (i);
  return 0;
}

__attribute__ ((noinline)) int
foo (int j)
{
  asm ("");
  bar (j);
  return 1;
}

__attribute__ ((noinline)) int
bar (int i)
{
  asm ("");
  if (i)
    bar (i - 1);
  else
    bt ();
  return 1;
}
#else
int main () { return 0; }
#endif /* __UCLIBC__ */

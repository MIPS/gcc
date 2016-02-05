/* { dg-do run { target ia32 } } */
/* { dg-additional-sources pr68637-2.c } */

extern void (*bar[]) (int, int) __attribute__ ((regparm (2)));

void
foo (void)
{
  bar[0] (1, 2);
}

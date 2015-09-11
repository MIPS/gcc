/* { dg-do compile } */

extern void foo (int,int) __attribute__ ((interrupt)); /* { dg-error "Interrupt service routine can't have arguments" } */
extern void bar (int, int);

void foo (int x, int y)
{
  bar (x, y);
}

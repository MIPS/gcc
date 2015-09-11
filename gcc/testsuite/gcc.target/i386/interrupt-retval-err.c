/* { dg-do compile } */

extern int foo (void) __attribute__ ((interrupt)); /* { dg-error "Interrupt service routine can't have non-void return value" } */
extern int bar (int, int);

int foo (void)
{
  int x, y;
  x = y = 1;
  return bar (x, y);
}

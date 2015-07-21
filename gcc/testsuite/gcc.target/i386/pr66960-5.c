/* { dg-do link } */
/* { dg-options "-O" } */

extern void link_error (void);

__attribute__ ((noinline, noclone))
void
foo (void)
{
  void **p = __builtin_stack_top ();
  void *ra = __builtin_return_address (0);
  if (*p != ra)
    link_error ();
}

int
main (void)
{
  foo ();
  return 0;
}

/* { dg-do compile } */
/* { dg-require-effective-target ilp32 } */
/* { dg-options "-mpreferred-stack-boundary=2" } */

void baz (void);
                       
double foo (void)
{
  baz ();
  return;
}

double bar (void)
{
  baz ();
}

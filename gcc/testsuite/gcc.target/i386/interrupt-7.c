/* { dg-do compile } */
/* { dg-options "-O2" } */

extern int error;

extern void fn (void *) __attribute__((interrupt));

void
foo (void)
{
  fn (&error); /* { dg-error "interrupt service routine can't be called directly" } */
}

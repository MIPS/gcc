/* APPLE LOCAL file testsuite nested functions */
/* Test that -Wnested-funcs works.  */
/* { dg-do compile } */
/* { dg-options "-Wnested-funcs" } */
void func (void)
{
  int foo ()
  { /* { dg-warning "nested functions are deprecated on MacOSX" } */
    return 1;
  }
}

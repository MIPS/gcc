/* APPLE LOCAL file testsuite nested functions */
/* This is similar to gcc.dg/Wnested-functions-1.c but tests that we warn by
   default.  */
/* { dg-do compile } */
/* { dg-options "-std=gnu99" } */
void func (void)
{
  int foo ()
  { /* { dg-warning "nested functions are deprecated" } */
    return 1;
  }
}

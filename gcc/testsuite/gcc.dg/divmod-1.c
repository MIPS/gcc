/* { dg-do compile } */
/* { dg-options "-O2" } */
/* expand_twoval_binop_libfunc wasn't calling protect_from_queue.  */
int b;
int
foo (int a)
{
  return  a % (b--);
}

/* { dg-do compile } */
/* { dg-options "-W -Wall" } */

/* Check that subscripting of vectors work with register storage class decls.  */

#define vector __attribute__((vector_size(16) ))


float vf(void)
{
  register vector float a;
  return a[0]; /* { dg-bogus "register" } */
}


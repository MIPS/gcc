/* PR target/47201 */
/* { dg-do compile } */
/* { dg-options "-O -fpic -g" { target fpic } } */

union U
{
  unsigned long m;
  float d;
} u;

int
foo (void)
{
  union U v = {
    (unsigned long)&u
  };
  return u.d == v.d;
}

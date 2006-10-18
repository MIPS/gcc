/* { dg-do compile } */
/* { dg-options "-O2" } */

/* This test is based on a bug that was seen during development.  */

extern void foo (const char *, ...);

void bar (void)
{
  _Decimal128 d128 = -1.0DL;
  foo ("%DDf,%DDf,%DDf,%DDff,%DDf,%DDf\n", d128,d128,d128,d128,d128,d128);
}

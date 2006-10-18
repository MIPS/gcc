/* { dg-do compile } */
/* { dg-options "-O2" } */

/* This test is based on a bug that was seen during development.  */

struct S
{
  int c: 23;
  __attribute__ ((packed)) _Decimal128 d;
} s;

void test (void)
{
  s.d = 233596.515625DL;
}

/* { dg-do compile } */
/* { dg-options "-O2" } */

/* This test is based on a bug that was seen during development.  */

void
foo (_Decimal128 *dst, _Decimal128 src)
{
  *dst = -src;
}


/* { dg-do compile } */
/* { dg-options "-O2" } */

/* This test is based on a bug that was seen during development.  */

void
foo (_Decimal32 *dst, _Decimal32 src1, _Decimal32 src2, int n)
{
  dst[n] = (src2 + 1.0DD) + src1;
}

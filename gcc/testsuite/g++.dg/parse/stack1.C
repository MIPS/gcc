/* PR c/2161: parser stack overflow.  */
/* XFAIL on branch only -- PR 12454 */
/* { dg-do compile { xfail *-*-* } } */
/* { dg-excess-errors "" } */

#define ONE	else if (0) { }
#define TEN	ONE ONE ONE ONE ONE ONE ONE ONE ONE ONE
#define HUN	TEN TEN TEN TEN TEN TEN TEN TEN TEN TEN
#define THOU	HUN HUN HUN HUN HUN HUN HUN HUN HUN HUN

void foo()
{
  if (0) { }
  /* 11,000 else if's.  */
  THOU THOU THOU THOU THOU THOU THOU THOU THOU THOU THOU
}

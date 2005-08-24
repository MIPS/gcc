/* { dg-do compile } */
/* { dg-options "-std=gnu99 -O -Wall" } */

/* C99 Section 6.2.7 Composite Type.  */

_Decimal32 g1();
_Decimal32 g2(_Decimal32 *);

_Decimal32 (*h1)[2];
_Decimal32 (*h2)[3];
_Decimal32 (*h3)[4];

_Decimal32 f1(_Decimal32(*)());
_Decimal32 f1(_Decimal32(*)(_Decimal32*));

_Decimal32 f2(_Decimal32(*)[]);
_Decimal32 f2(_Decimal32(*)[3]);

int main()
{
  _Decimal32 i;
 
  i = f1(g1);
  i = f1(g2);
  
  i = f2(h1); /* { dg-warning "incompatible pointer type" } */
  i = f2(h2);
  i = f2(h3); /* { dg-warning "incompatible pointer type" } */

  return 0;
}

/* Tests for _FloatN / _FloatNx types: test erroneous mixing with DFP.  */
/* { dg-do compile } */
/* { dg-require-effective-target float32 } */
/* { dg-require-effective-target float32x } */
/* { dg-options "" } */

_Decimal32 d32;
_Float32 f32;
_Float32x f32x;
int i;

void
f (void)
{
  (void) (d32 + f32); /* { dg-error "mix operands" } */
  (void) (f32x * d32); /* { dg-error "mix operands" } */
  (void) (i ? d32 : f32); /* { dg-error "mix operands" } */
}

/* { dg-do compile } */
/* { dg-options "-mabs=legacy" } */
/* { dg-skip-if "Removed from mipsr6" { *-img-* } { "*" } { "" } } */

NOMIPS16 float
fabsf_legacy (float f)
{
  return __builtin_fabsf (f);
}

/* { dg-final { scan-assembler-not "\tabs\\.s\t" } } */

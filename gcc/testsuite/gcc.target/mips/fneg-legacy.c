/* { dg-do compile } */
/* { dg-options "-mabs=legacy" } */
/* { dg-skip-if "Removed from mipsr6" { *-img-* } { "*" } { "" } } */

NOMIPS16 double
fneg_legacy (double d)
{
  return -d;
}

/* { dg-final { scan-assembler-not "\tneg\\.d\t" } } */

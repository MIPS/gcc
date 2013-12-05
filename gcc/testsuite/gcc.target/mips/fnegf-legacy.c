/* { dg-do compile } */
/* { dg-options "-mabs=legacy" } */
/* { dg-skip-if "Removed from mipsr6" { *-img-* } { "*" } { "" } } */

NOMIPS16 float
fnegf_legacy (float f)
{
  return -f;
}

/* { dg-final { scan-assembler-not "\tneg\\.s\t" } } */

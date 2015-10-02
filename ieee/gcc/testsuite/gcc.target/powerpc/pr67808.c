/* { dg-do compile { target { powerpc*-*-* } } } */
/* { dg-skip-if "" { powerpc*-*-darwin* } { "*" } { "" } } */
/* { dg-require-effective-target powerpc_vsx_ok } */
/* { dg-options "-O1 -mvsx -mlra" } */

/* PR 67808: LRA ICEs on simple double to long double conversion test case */

void
foo (long double *ldb1, double *db1)
{
  *ldb1 = *db1;
}

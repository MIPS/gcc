/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-phiopt1-details -fdump-tree-tailc-details" } */


int f(int i)
{
  int result;
  result = t(i);
  if (result)
    return result;
  return 0;
}

/* These are xfailed for now as the returns are not merged so the optimizers do not
   see the opportunities.   */

/* We should convert one COND_EXPRs into straightline code.  */
/* { dg-final { scan-tree-dump-times "straightline" 1 "phiopt1"} { xfail *-*-* } } */
/* Also we should have found that the call to t is tail called.   */
/* { dg-final { scan-tree-dump-times "Found tail call" 1 "tailc"} { xfail *-*-* } } */


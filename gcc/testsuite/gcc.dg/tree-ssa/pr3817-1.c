/* { dg-do compile } */
/* { dg-options "-O1 -fdump-tree-phiopt2" } */

int f()
 {
   int secs, negative,hh,mm,ss;
   secs = (negative ? -1 : 1) * ((hh * 60 * 60) + (mm * 60) + ss);
   return secs ;
 }

/* { dg-final { scan-tree-dump-times "if " 1 "phiopt2"} } */
/* { dg-final { cleanup-tree-dump "phiopt2" } } */

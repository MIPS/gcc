/* We used to use c.lt.fmt instead of c.ule.fmt here.  */
/* { dg-options "-mhard-float" } */
/* { dg-skip-if "Removed from mipsr6" { *-img-* } { "*" } { "" } } */
NOMIPS16 int f1 (float x, float y) { return __builtin_isless (x, y); }
NOMIPS16 int f2 (double x, double y) { return __builtin_isless (x, y); }
/* { dg-final { scan-assembler "\tc\\.ule\\.s\t" } } */
/* { dg-final { scan-assembler "\tc\\.ule\\.d\t" } } */

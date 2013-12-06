/* We used to use c.le.fmt instead of c.ult.fmt here.  */
/* { dg-options "-mhard-float" } */
/* { dg-skip-if "Removed from mipsr6" { *-img-* } { "*" } { "" } } */
NOMIPS16 int f1 (float x, float y) { return __builtin_islessequal (x, y); }
NOMIPS16 int f2 (double x, double y) { return __builtin_islessequal (x, y); }
/* { dg-final { scan-assembler "\tc\\.ult\\.s\t" } } */
/* { dg-final { scan-assembler "\tc\\.ult\\.d\t" } } */

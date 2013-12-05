/* { dg-do compile } */
/* { dg-options "-mnan=legacy -EB" } */
/* { dg-skip-if "Removed from mipsr6" { *-img-* } { "*" } { "" } } */

float fs = __builtin_nansf ("");

/* { dg-final { scan-assembler "\t\\.nan\tlegacy\n" } } */
/* { dg-final { scan-assembler "\t\\.word\t2147483647\n" } } */

/* { dg-do compile } */
/* { dg-options "isa>=4" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */
/* { dg-skip-if "Removed from mipsr6" { *-img-* } { "*" } { "" } } */
/* { dg-final { scan-assembler "\tmovz\t" } } */
/* { dg-final { scan-assembler "\tmovn\t" } } */

void ext_long (long);

NOMIPS16 long
sub4 (long i, long j, long k)
{
  ext_long (k ? i : j);
}

NOMIPS16 long
sub5 (long i, long j, int k)
{
  ext_long (!k ? i : j);
}

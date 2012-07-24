/* When combining a sign_extract/zero_extract of a *single bit* we go
   to far and want to create a store-flag of a comparison of the
   extract.  */

/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "exts\t" } } */
/* { dg-final { scan-assembler-not "sra\t|sll\t" } } */

void
invalidate_memory (int i)
{
  remove_from_table ((i << 4) >> 31);
}

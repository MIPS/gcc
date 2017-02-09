/* { dg-options "isa_rev>=2 -mxlp" } */

NOMIPS16 unsigned int
foo (unsigned int x)
{
  return __builtin_bswap32 (x);
}

/* { dg-final { scan-assembler "\twsbh\t" } } */
/* { dg-final { scan-assembler "\tror\t" } } */

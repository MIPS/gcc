/* { dg-options "isa_rev>=6 -mnanomips -mxlp" } */

unsigned int
foo (unsigned int x)
{
  return __builtin_bswap32 (x);
}

/* { dg-final { scan-assembler "\twsbh\t" } } */
/* { dg-final { scan-assembler "\tror\t" } } */

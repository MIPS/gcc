/* { dg-do compile } */
/* { dg-options "-march=32r6 -fpic -m32 -msoft-float -mcmodel=auto" } */

double test (double a, double b)
{
  return a + b;
}

/* { dg-final { scan-assembler "lw\t\\\$\[ast0-9\]+,%got_call\\(__adddf3\\)\\(\\\$gp\\)" } } */
/* { dg-final { scan-assembler "\\\.reloc\t1f,R_NANOMIPS_JALR16,__adddf3" } } */
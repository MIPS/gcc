/* Verify that the m constraint does not create indexed addressing.  */

void
foo (int* a, int b)
{
  __asm__ __volatile__ ("mem %0":"=m"(a[b]));
} 

/* { dg-final { scan-assembler-not "\\\$\[ast0-9\]+\\(\\\$\[ast0-9\]+\\)" } } */

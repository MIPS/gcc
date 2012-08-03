/* { dg-do compile } */
/* { dg-options "-fdump-rtl-fwprop1" } */

void f(unsigned char * p, short s, int c, int *z)
{
  if (c)
    *z = 0;
  *p ^= (unsigned char)s;
}

/* { dg-final { scan-rtl-dump-times "sign_extend:" 0 "fwprop1" { target mips*-*-* } } } */
/* { dg-final { cleanup-rtl-dump "fwprop1" } } */

/* The MIPS ABI all say the arguments are extended to 64bit (n32/n64 and EABI64)
   or to 32bit (o32) so there is no need for an extra sign extend
   and the store is to just the lower 8bits so a sign extend is not
   needed at all.  */
/* { dg-final { scan-assembler-not "exts" } } */
/* { dg-final { scan-assembler-not "seb" } } */

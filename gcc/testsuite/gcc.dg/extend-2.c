/* { dg-do compile { target mips*-*-* } } */
char g(void);
char f(char a, char b, char d)
{
  if (a == b)
    return 0;
  else
    return a;
}

/* The MIPS ABI all say the arguments are extended to 64bit (n32/n64 and EABI64)
   or to 32bit (o32) so there is no need for an extra sign extend.  */
/* { dg-final { scan-assembler-not "exts" } } */
/* { dg-final { scan-assembler-not "seb" } } */

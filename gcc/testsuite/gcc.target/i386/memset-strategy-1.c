/* { dg-do compile } */
/* { dg-options "-O2 -march=atom -mmemset-strategy=libcall:-1:align" } */
/* { dg-final { scan-assembler-times "memset" 3 { target elf }  } } */
/* { dg-final { scan-assembler-times "memset" 2 { target { ! elf } } } } */

char a[2048];
void t (void)
{
  __builtin_memset (a, 1, 2048);
}


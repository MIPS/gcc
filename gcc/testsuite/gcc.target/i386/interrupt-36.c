/* { dg-do compile } */
/* { dg-options "-Os -mno-cld" } */


extern void *a;
extern int b;

__attribute__ ((interrupt))
void
foo (void *frame)
{
  __builtin_memset (a, b, 40);
}

/* { dg-final { scan-assembler "stosb" } } */
/* { dg-final { scan-assembler-times "\tcld" 1 } } */

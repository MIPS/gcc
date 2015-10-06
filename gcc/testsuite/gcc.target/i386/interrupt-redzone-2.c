/* { dg-do compile { target { ! ia32 } } } */
/* { dg-options "-O2 -mred-zone" } */

typedef unsigned int uword_t __attribute__ ((mode (__word__)));

void
__attribute__((interrupt))
fn (uword_t ip, uword_t cs, uword_t eflags, uword_t sp, uword_t ss)
{
  /* Need to adjust stack if more than 128 bytes are used on stack
     with a 128-byte red zone.  */
  char array[113];
  asm ("# %0 "
       :
       : "r" (array));
}

/* { dg-final { scan-assembler-times "(?:sub|add)(?:l|q)\[\\t \]*\\\$\[0-9\]*,\[\\t \]*%\[re\]?sp" 2 } } */

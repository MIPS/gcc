/* { dg-do compile { target { ! ia32 } } } */
/* { dg-options "-O2 -mred-zone" } */

void
__attribute__((interrupt))
fn (void *frame)
{
  /* No need to adjust stack if less than 128 bytes are used on stack
     with a 128-byte red zone.  */
  char array[112];
  asm ("# %0 "
       :
       : "r" (array));
}

/* { dg-final { scan-assembler-not "(sub|add)(l|q)\[\\t \]*\\$\[0-9\]*,\[\\t \]*%\[re\]?sp" } } */

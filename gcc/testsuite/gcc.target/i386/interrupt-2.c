/* { dg-do compile } */
/* { dg-options "-O2 -Wall -g" } */

void
__attribute__((interrupt))
fn (void *frame)
{
}

/* { dg-final { scan-assembler-not "add(l|q)\[\\t \]*\\$\[0-9\]*,\[\\t \]*%\[re\]?sp" } } */
/* { dg-final { scan-assembler "iret" } }*/

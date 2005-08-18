/* APPLE LOCAL file CW asm blocks */
/* { dg-do assemble { target i?86*-*-darwin* } } */
/* { dg-options { "-fasm-blocks" } } */
/* Radar 4214021 */

void foo() {
  asm bar: jmp bar
  __asm nop __asm nop
  __asm nop
  __asm emms
    asm @1: @2: @3 foo: nop asm @1: @2: foo: nop
    asm .word 0
}

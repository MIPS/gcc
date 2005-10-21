/* APPLE LOCAL file CW asm blocks */
/* { dg-do assemble { target i?86*-*-darwin* } } */
/* { dg-options { -fasm-blocks -msse3 } } */
/* Radar 4300186 */

void foo() {
  asm {
    movd mm7, [esi][eax][4]
    movd mm3, [edi][eax][4]
    movd mm1, [edx][eax][4]
  }
}

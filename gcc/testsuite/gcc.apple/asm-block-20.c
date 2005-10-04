/* APPLE LOCAL file CW asm blocks */
/* { dg-do assemble { target i?86*-*-darwin* } } */
/* { dg-options { -fasm-blocks -msse3 } } */
/* Radar 4273624 */

void foo() {
  asm movd xmm3, [ebx*4]
  asm movd xmm3, [8*ebx]
  asm movd xmm3, [ecx+ebx*4]
  asm movd xmm3, [ecx+8*ebx]
  asm movd xmm3, [ebx*4+ecx]
  asm movd xmm3, [8*ebx+ecx]
}

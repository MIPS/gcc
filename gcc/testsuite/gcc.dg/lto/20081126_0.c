/* { dg-do link { target i?86-*-* x86_64-*-* } } */
/* { dg-options "{-flto -shared}" } */

int f(void) {
  register int ri asm("edi");
  return ri;
}

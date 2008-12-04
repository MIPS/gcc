/* { dg-do link { target i?86-*-* x86_64-*-* } } */
/* { dg-options "{-flto-single -shared}" } */

register int ri asm("edi");

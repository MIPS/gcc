/* { dg-do link { target i?86-*-* x86_64-*-* } } */
/* { dg-options "{-w -flto -fPIC -shared}" } */

register int ri asm("edi");

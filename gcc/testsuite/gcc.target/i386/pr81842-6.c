/* { dg-do compile { target *-*-linux* } } */
/* { dg-options "-fcf-protection -mcet -mno-check-ucontext-functions" } */

#include <ucontext.h>

void *p1 = getcontext;
void *p2 = setcontext;
void *p3 = swapcontext;
void *p4 = makecontext;

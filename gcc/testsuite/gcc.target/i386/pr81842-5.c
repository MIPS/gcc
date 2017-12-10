/* { dg-do compile { target *-*-linux* } } */
/* { dg-options "-fcf-protection -mcet" } */

#include <ucontext.h>

void *p1 = getcontext;
void *p2 = setcontext;
void *p3 = swapcontext;
void *p4 = makecontext;
/* { dg-error "'getcontext' cannot be used with -mshstk" "" { target *-*-* } 0 } */
/* { dg-error "'setcontext' cannot be used with -mshstk" "" { target *-*-* } 0 } */
/* { dg-error "'swapcontext' cannot be used with -mshstk" "" { target *-*-* } 0 } */
/* { dg-error "'makecontext' cannot be used with -mshstk" "" { target *-*-* } 0 } */

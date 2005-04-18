/* APPLE LOCAL begin mainline 2005-04-14 */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-maltivec" } */
/* { dg-skip-if "" { "-m64" } { "" } } */

#include <altivec.h>

extern void preansi();

typedef void (*pvecfunc) ();

void foo(pvecfunc pvf) {
   vector int v = (vector int){1, 2, 3, 4};
   preansi (4, 4.0, v); /* { dg-error "AltiVec argument passed to unprototyped function" } */
   (*pvf)  (4, 4.0, v); /* { dg-error "AltiVec argument passed to unprototyped function" } */
}
/* APPLE LOCAL end mainline 2005-04-14 */

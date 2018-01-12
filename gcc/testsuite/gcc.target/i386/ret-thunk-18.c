/* { dg-do compile } */
/* { dg-options "-O2 -mfunction-return=keep -mindirect-branch=keep -mshstk" } */

__attribute__ ((function_return("thunk")))
void
bar (void)
{ /* { dg-error "'-mfunction-return=' and '-mshstk' are not compatible" } */
}

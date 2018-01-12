/* { dg-do compile } */
/* { dg-options "-O2 -mfunction-return=thunk -mindirect-branch=keep -mshstk" } */

void
bar (void)
{ /* { dg-error "'-mfunction-return=' and '-mshstk' are not compatible" } */
}

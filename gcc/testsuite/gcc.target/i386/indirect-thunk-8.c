/* { dg-do compile } */
/* { dg-options "-O2 -mindirect-branch=thunk -mfunction-return=keep -mshstk" } */

void
bar (void)
{ /* { dg-error "'-mindirect-branch=' and '-mshstk' are not compatible" } */
}

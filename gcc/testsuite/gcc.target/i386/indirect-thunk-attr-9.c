/* { dg-do compile } */
/* { dg-options "-O2 -mindirect-branch=keep -mfunction-return=keep -mshstk" } */

__attribute__ ((indirect_branch("thunk")))
void
bar (void)
{ /* { dg-error "'-mindirect-branch=' and '-mshstk' are not compatible" } */
}

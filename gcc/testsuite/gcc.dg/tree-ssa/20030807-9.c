/* { dg-do compile } */
/* { dg-options "-O1 -fdump-tree-ssa" } */

static void
bar ()
{
  const char *label2 = (*"*.L_sfnames_b" == '*') + "*.L_sfnames_b";
  oof (label2);
}

/* There should be no IF conditionals.  */
/* { dg-final { scan-tree-dump-times "if " 0 "ssa"} } */

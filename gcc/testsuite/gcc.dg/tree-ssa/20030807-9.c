/* { dg-do compile } */
/* { dg-options "-O1 -fdump-tree-dom2" } */

static void
bar ()
{
  const char *label2 = (*"*.L_sfnames_b" == '*') + "*.L_sfnames_b";
  oof (label2);
}

/* There should be no IF conditionals.  */
/* { dg-final { scan-tree-dump-times "if " 0 "dom2"} } */

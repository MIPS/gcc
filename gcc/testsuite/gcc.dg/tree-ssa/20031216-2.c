/* { dg-do compile } */ 
/* { dg-options "-O2 -fdump-tree-optimized" } */

union tree_node;
typedef union tree_node *tree;
struct tree_common
{
  int code;
};
struct tree_list
{
  tree purpose;
};
union tree_node
{
  struct tree_common common;
  struct tree_list list;
};
void
simplify_condition (cond_p)
     tree *cond_p;
{
  tree decl;
  tree cond = *cond_p;
  if (cond->common.code != 42)
    abort ();
  decl = cond->list.purpose;
  if (cond->common.code != 42)
    abort ();
  c_simplify_stmt (&decl);
}

/* The second if() should be eliminated.  But only at -O2, with strict
   aliasing enabled.  */
/* { dg-final { scan-tree-dump-times "if " 1 "optimized"} } */

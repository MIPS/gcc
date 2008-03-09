/* { dg-do compile } */
/* { dg-options "-O1 -fdump-tree-dom3" } */

extern void abort (void);
union tree_node;
typedef union tree_node *tree;
struct tree_vec
{
  int length;
  tree a[1];
};
struct tree_type
{
  tree binfo;
};
union tree_node
{
  struct tree_type type;
  struct tree_vec vec;
};
void
record_component_aliases (type)
     tree type;
{
  if (type->type.binfo->vec.length)
    abort ();
  for (; ((
	    {
	    const tree __z = type->type.binfo;
	    if (type->type.binfo->vec.length)
	    abort ();
 type->type.binfo->vec.a[4];}
	  )->vec.length);)
    {
      if (4 >= type->type.binfo->vec.length)
	abort ();
      blah ();
    }
}

/* The call to blah should have been eliminated.  If the call is not
   eliminated, then dominator optimizations failed and it'll be
   impossible to delete other unnecessary code.  */
/* { dg-final { scan-tree-dump-not "blah \\(\\)" "dom3" } } */
  
/* There should be two IF conditionals.  */
/* { dg-final { scan-tree-dump-times "if " 2 "dom3"} } */
                                                                                
/* There should be two loads of a tree node, type.binfo and
   type.binfo->vec.a[4].  */
/* { dg-final { scan-tree-dump-times "MEM <union tree_node" 2 "dom3"} } */

/* There should be two loads of vec.length.  */
/* { dg-final { scan-tree-dump-times "MEM <int " 2 "dom3"} } */

/* { dg-final { cleanup-tree-dump "dom3" } } */

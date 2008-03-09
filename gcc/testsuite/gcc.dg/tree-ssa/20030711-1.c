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
  int i;
  if (4 >= type->type.binfo->vec.length)
    abort ();
  for (; i < ((
                {
                const tree __t = type->type.binfo;
                if (4 >= __t->vec.length)
                abort (); type->type.binfo->vec.a[4];}
              )->vec.length);)
    {
      if (4 >= type->type.binfo->vec.length)
        abort ();
      blah ();
    }
}

/* The call to blah can not be eliminated.  */
/* { dg-final { scan-tree-dump-times "blah \\(\\)" 1 "dom3" } } */
   
/* There should be tree IF conditionals.  */
/* { dg-final { scan-tree-dump-times "if " 3 "dom3"} } */
                                                                                
/* There should be three loads of trees.  */
/* { dg-final { scan-tree-dump-times "MEM <union tree_node" 3 "dom3"} } */
 
/* There should be three loads of vec.length.  */
/* { dg-final { scan-tree-dump-times "MEM <int " 3 "dom3"} } */

/* { dg-final { cleanup-tree-dump "dom3" } } */

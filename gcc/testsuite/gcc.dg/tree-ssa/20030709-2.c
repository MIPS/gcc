/* { dg-do compile } */
/* { dg-options "-O1 -fdump-tree-dom2" } */
  
struct rtx_def;
typedef struct rtx_def *rtx;
union tree_node;
typedef union tree_node *tree;
typedef struct mem_attrs
{
  int  foo;

} mem_attrs;
union rtunion_def
{
  mem_attrs *rtmem;
};
typedef union rtunion_def rtunion;
struct rtx_def
{
  rtunion fld[1];
};
struct tree_decl
{
  rtx rtl;
};
union tree_node
{
  struct tree_decl decl;
};
void *
get_alias_set (t)
     tree t;
{
  long set;
  if (t->decl.rtl)
    return (t->decl.rtl->fld[1].rtmem 
	    ? 0
	    : (((t->decl.rtl ? t->decl.rtl : (make_decl_rtl (t, 0), t->decl.rtl)))->fld[1]).rtmem);
}

/* There should be precisely one load of ->decl.rtl.  If there is
   more than, then the dominator optimizations failed.  */
/* { dg-final { scan-tree-dump-times "->decl\\.rtl" 1 "dom2"} } */
  
/* There should be precisely one load of .rtmem.  If there is
   more than, then the dominator optimizations failed.  */
/* { dg-final { scan-tree-dump-times ".rtmem" 1 "dom2"} } */
  
/* There should be two IF statements.  */
/* { dg-final { scan-tree-dump-times "if " 2 "dom2"} } */


#ifndef TREE_ALIAS_STEEN
#define TREE_ALIAS_STEEN
#include "tree-alias-ecr.h"
#include "tree-alias-type.h"
struct tree_alias_ops
{
  void (*init) (struct tree_alias_ops *);
  void (*cleanup) (struct tree_alias_ops *);
  alias_typevar (*add_var) (struct tree_alias_ops *, tree);
  alias_typevar (*add_var_same) (struct tree_alias_ops *, tree,
			   alias_typevar);
  void (*simple_assign) (struct tree_alias_ops *, alias_typevar,
			 alias_typevar);
  void (*addr_assign) (struct tree_alias_ops *, alias_typevar, alias_typevar);
  void (*ptr_assign) (struct tree_alias_ops *, alias_typevar, alias_typevar);
  void (*op_assign) (struct tree_alias_ops *, alias_typevar, varray_type);
  void (*heap_assign) (struct tree_alias_ops *, alias_typevar);
  void (*assign_ptr) (struct tree_alias_ops *, alias_typevar, alias_typevar);
  void (*function_def) (struct tree_alias_ops *, alias_typevar,
			varray_type, alias_typevar);
  void (*function_call) (struct tree_alias_ops *, alias_typevar,
			 alias_typevar, varray_type);
  void *data;
};
extern struct tree_alias_ops *steen_alias_ops;
extern void create_alias_vars PARAMS ((void));
extern void init_alias_vars PARAMS ((void));
#endif

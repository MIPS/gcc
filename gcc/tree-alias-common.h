#ifndef TREE_ALIAS_COMMON
#define TREE_ALIAS_COMMON
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
  bool (*may_alias) (struct tree_alias_ops *, alias_typevar, alias_typevar);
  void *data;
  int ip:1; /*Interprocedural */
};
extern struct tree_alias_ops *current_alias_ops;
extern void create_alias_vars PARAMS ((void));
extern void delete_alias_vars PARAMS ((void));
extern void init_alias_vars PARAMS ((void));
extern bool ptr_may_alias_var PARAMS ((tree, tree));
enum pta_type
{
	PTA_NONE,
	PTA_STEEN,
	PTA_ANDERSEN
};
extern enum pta_type flag_tree_points_to;
extern const char *alias_get_name PARAMS ((tree));
#endif

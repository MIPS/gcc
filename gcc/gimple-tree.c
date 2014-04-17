#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "gimple-tree.h"
#include "gimple-expr.h"

Gimple::value_ptr boolean_true_node_ptr (&global_trees[TI_BOOLEAN_TRUE]);
Gimple::value_ptr boolean_false_node_ptr (&global_trees[TI_BOOLEAN_FALSE]);
Gimple::integer_cst_ptr integer_zero_node_ptr (&global_trees[TI_INTEGER_ZERO]);
Gimple::type_ptr void_type_node_ptr (&global_trees[TI_VOID_TYPE]);
Gimple::type_ptr boolean_type_node_ptr (&global_trees[TI_BOOLEAN_TYPE]);
Gimple::type_ptr char_type_node_ptr (&integer_types[itk_char]);
Gimple::type_ptr integer_type_node_ptr (&integer_types[itk_int]);

const char *
gimple_tree_printable_name (tree decl, int verbosity)
{
  return gimple_decl_printable_name (decl, verbosity);
}



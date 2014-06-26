#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "gimple-tree.h"
#include "gimple-expr.h"

G::value_ptr boolean_true_node_ptr (&global_trees[TI_BOOLEAN_TRUE]);
G::value_ptr boolean_false_node_ptr (&global_trees[TI_BOOLEAN_FALSE]);
G::integer_cst_ptr integer_zero_node_ptr (&global_trees[TI_INTEGER_ZERO]);
G::type_ptr void_type_node_ptr (&global_trees[TI_VOID_TYPE]);
G::type_ptr boolean_type_node_ptr (&global_trees[TI_BOOLEAN_TYPE]);
G::type_ptr pointer_type_node_ptr (&global_trees[TI_PTR_TYPE]);
G::type_ptr char_type_node_ptr (&integer_types[itk_char]);
G::type_ptr integer_type_node_ptr (&integer_types[itk_int]);

const char *
gimple_tree_printable_name (tree decl, int verbosity)
{
  return gimple_decl_printable_name (decl, verbosity);
}



#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "gimple-tree.h"
#include "gimple-expr.h"

const char *
gimple_tree_printable_name (tree decl, int verbosity)
{
  return gimple_decl_printable_name (decl, verbosity);
}



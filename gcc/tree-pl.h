#include "tree.h"

#define BOUNDED_TYPE_P(type) \
  (TREE_CODE (type) == POINTER_TYPE \
    || TREE_CODE (type) == REFERENCE_TYPE)
#define BOUNDED_P(node) \
  BOUNDED_TYPE_P (TREE_TYPE (node))

extern bool pl_register_var_initializer (tree var);
extern void pl_finish_file (void);
extern tree pl_get_registered_bounds (tree ptr);
extern void pl_split_returned_reg (rtx return_reg, rtx *return_reg_val,
				   rtx *return_reg_bnd);

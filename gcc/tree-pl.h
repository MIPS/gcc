#include "tree.h"

#define BOUNDED_TYPE_P(type) \
  (TREE_CODE (type) == POINTER_TYPE \
    || TREE_CODE (type) == REFERENCE_TYPE)
#define BOUNDED_P(node) \
  BOUNDED_TYPE_P (TREE_TYPE (node))

#define bound_type_node (TARGET_64BIT ? bound64_type_node : bound32_type_node)

extern bool pl_register_var_initializer (tree var);
extern void pl_finish_file (void);
extern tree pl_get_registered_bounds (tree ptr);
extern tree pl_get_arg_bounds (tree arg);
extern void pl_split_returned_reg (rtx return_reg, rtx *return_reg_val,
				   rtx *return_reg_bnd);
extern rtx pl_join_splitted_reg (rtx val, rtx bnd);
extern rtx pl_get_value_with_offs (rtx par, rtx offs);
extern void pl_copy_bounds_for_stack_parm (rtx slot, rtx value, tree type);
extern bool pl_type_has_pointer (tree type);
extern void pl_emit_bounds_store (rtx bounds, rtx value, rtx mem);
extern tree pl_make_bounds_for_struct_addr (tree ptr);
extern tree pl_get_zero_bounds (void);
extern bool pl_variable_size_type (tree type);
extern tree pl_build_make_bounds_call (tree lb, tree size);
extern tree pl_build_bndstx_call (tree addr, tree ptr, tree bounds);
extern void pl_expand_bounds_reset_for_mem (tree mem, tree ptr);

#include "tree.h"

#define BOUNDED_TYPE_P(type) \
  (TREE_CODE (type) == POINTER_TYPE \
    || TREE_CODE (type) == REFERENCE_TYPE)
#define BOUNDED_P(node) \
  BOUNDED_TYPE_P (TREE_TYPE (node))

extern const char *MPX_SIZE_OF_SYMBOL_PREFIX;

extern bool mpx_register_var_initializer (tree var);
extern void mpx_finish_file (void);
extern tree mpx_get_registered_bounds (tree ptr);
extern tree mpx_get_arg_bounds (tree arg);
extern void mpx_split_returned_reg (rtx return_reg, rtx *return_reg_val,
				   rtx *return_reg_bnd);
extern rtx mpx_join_splitted_reg (rtx val, rtx bnd);
extern rtx mpx_get_value_with_offs (rtx par, rtx offs);
extern void mpx_copy_bounds_for_stack_parm (rtx slot, rtx value, tree type);
extern bool mpx_type_has_pointer (tree type);
extern void mpx_emit_bounds_store (rtx bounds, rtx value, rtx mem);
extern tree mpx_make_bounds_for_struct_addr (tree ptr);
extern tree mpx_get_zero_bounds (void);
extern bool mpx_variable_size_type (tree type);
extern tree mpx_build_make_bounds_call (tree lb, tree size);
extern tree mpx_build_bndstx_call (tree addr, tree ptr, tree bounds);
extern void mpx_expand_bounds_reset_for_mem (tree mem, tree ptr);
extern void mpx_put_regs_to_expr_list (rtx par);
extern void mpx_fix_cfg ();

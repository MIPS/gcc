#include "tree.h"

extern bool pl_register_var_initializer (tree var);
extern void pl_finish_file (void);
extern tree pl_get_registered_bounds (tree ptr);
extern void pl_split_returned_reg (rtx return_reg, rtx *return_reg_val,
				   rtx *return_reg_bnd);

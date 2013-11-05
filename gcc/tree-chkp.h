/* Definitions for the ubiquitous 'tree' type for GNU compilers.
   Copyright (C) 1989-2013 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#ifndef GCC_TREE_CHKP_H
#define GCC_TREE_CHKP_H

#include "coretypes.h"
#include "tree.h"

#define DECL_BOUNDS_RTL(NODE) (chkp_get_rtl_bounds (DECL_WRTL_CHECK (NODE)))

#define SET_DECL_BOUNDS_RTL(NODE, VAL) \
  (chkp_set_rtl_bounds (DECL_WRTL_CHECK (NODE), VAL))

#define DECL_BOUNDS(NODE) (chkp_get_bounds (DECL_WRTL_CHECK (NODE)))

#define SET_DECL_BOUNDS(NODE, VAL) \
  (chkp_set_bounds (DECL_WRTL_CHECK (NODE), VAL))

extern rtx chkp_get_rtl_bounds (tree node);
extern void chkp_set_rtl_bounds (tree node, rtx val);
extern void chkp_reset_rtl_bounds ();
extern tree chkp_get_bounds (tree node);
extern void chkp_set_bounds (tree node, tree val);
extern bool chkp_register_var_initializer (tree var);
extern void chkp_finish_file (void);
extern void chkp_split_slot (rtx slot, rtx *slot_val, rtx *slot_bnd);
extern rtx chkp_join_splitted_slot (rtx val, rtx bnd);
extern rtx chkp_get_value_with_offs (rtx par, rtx offs);
extern void chkp_copy_bounds_for_stack_parm (rtx slot, rtx value, tree type);
extern bool chkp_type_has_pointer (tree type);
extern unsigned chkp_type_bounds_count (tree type);
extern void chkp_emit_bounds_store (rtx bounds, rtx value, rtx mem);
extern tree chkp_make_bounds_for_struct_addr (tree ptr);
extern tree chkp_get_zero_bounds_var (void);
extern bool chkp_variable_size_type (tree type);
extern tree chkp_build_make_bounds_call (tree lb, tree size);
extern tree chkp_build_bndstx_call (tree addr, tree ptr, tree bounds);
extern void chkp_expand_bounds_reset_for_mem (tree mem, tree ptr);
extern void chkp_put_regs_to_expr_list (rtx par);
extern vec<bool> chkp_find_bound_slots (tree type);
extern void chkp_build_bndstx (tree addr, tree ptr, tree bounds,
			       gimple_stmt_iterator *gsi);
extern tree chkp_parm_for_arg_bnd_arg (tree arg);
extern gimple chkp_retbnd_call_by_val (tree val);
extern bool chkp_function_instrumented_p (tree fndecl);


#endif /* GCC_TREE_CHKP_H */

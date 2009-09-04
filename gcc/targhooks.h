/* Default target hook functions.
   Copyright (C) 2003, 2004, 2005, 2007, 2008 Free Software Foundation, Inc.

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

#include "multi-target.h"

START_TARGET_SPECIFIC
extern void default_external_libcall (rtx);

extern int default_unspec_may_trap_p (const_rtx, unsigned);
END_TARGET_SPECIFIC

extern enum machine_mode default_cc_modes_compatible (enum machine_mode,
						      enum machine_mode);

extern bool default_return_in_memory (const_tree, const_tree);

extern rtx default_expand_builtin_saveregs (void);
START_TARGET_SPECIFIC
extern void default_setup_incoming_varargs (CUMULATIVE_ARGS *, enum machine_mode, tree, int *, int);
END_TARGET_SPECIFIC
extern rtx default_builtin_setjmp_frame_value (void);
START_TARGET_SPECIFIC
extern bool default_pretend_outgoing_varargs_named (CUMULATIVE_ARGS *);
END_TARGET_SPECIFIC

extern enum machine_mode default_eh_return_filter_mode (void);
START_TARGET_SPECIFIC
extern enum machine_mode default_libgcc_cmp_return_mode (void);
extern enum machine_mode default_libgcc_shift_count_mode (void);
extern enum machine_mode default_unwind_word_mode (void);
extern unsigned HOST_WIDE_INT default_shift_truncation_mask
  (enum machine_mode);
extern unsigned int default_min_divisions_for_recip_mul (enum machine_mode);
END_TARGET_SPECIFIC
extern int default_mode_rep_extended (enum machine_mode, enum machine_mode);

extern tree default_stack_protect_guard (void);
extern tree default_external_stack_protect_fail (void);
START_TARGET_SPECIFIC
extern tree default_hidden_stack_protect_fail (void);
END_TARGET_SPECIFIC

extern enum machine_mode default_mode_for_suffix (char);

extern tree default_cxx_guard_type (void);
START_TARGET_SPECIFIC
extern tree default_cxx_get_cookie_size (tree);

extern bool hook_pass_by_reference_must_pass_in_stack
  (CUMULATIVE_ARGS *, enum machine_mode mode, const_tree, bool);
extern bool hook_callee_copies_named
  (CUMULATIVE_ARGS *ca, enum machine_mode, const_tree, bool);
END_TARGET_SPECIFIC

extern void default_unwind_emit (FILE *, rtx);

START_TARGET_SPECIFIC
extern bool default_scalar_mode_supported_p (enum machine_mode);
extern bool default_decimal_float_supported_p (void);
extern bool default_fixed_point_supported_p (void);
END_TARGET_SPECIFIC

extern const char * default_invalid_within_doloop (const_rtx);

extern tree default_builtin_vectorized_function
  (unsigned /*enum built_in_function */, tree, tree);

//extern tree default_builtin_vectorized_conversion (enum tree_code, tree);
extern tree default_builtin_vectorized_conversion (unsigned, tree);

//extern tree default_builtin_reciprocal (enum built_in_function, bool, bool);
extern tree default_builtin_reciprocal (unsigned, bool, bool);

extern bool default_builtin_vector_alignment_reachable (const_tree, bool);

/* These are here, and not in hooks.[ch], because not all users of
   hooks.h include tm.h, and thus we don't have CUMULATIVE_ARGS.  */

START_TARGET_SPECIFIC
extern bool hook_bool_CUMULATIVE_ARGS_false (CUMULATIVE_ARGS *);
extern bool hook_bool_CUMULATIVE_ARGS_true (CUMULATIVE_ARGS *);

extern bool hook_bool_CUMULATIVE_ARGS_mode_tree_bool_false
  (CUMULATIVE_ARGS *, enum machine_mode, const_tree, bool);
extern bool hook_bool_CUMULATIVE_ARGS_mode_tree_bool_true
  (CUMULATIVE_ARGS *, enum machine_mode, const_tree, bool);
extern int hook_int_CUMULATIVE_ARGS_mode_tree_bool_0
  (CUMULATIVE_ARGS *, enum machine_mode, tree, bool);
END_TARGET_SPECIFIC
extern const char *hook_invalid_arg_for_unprototyped_fn
  (const_tree, const_tree, const_tree);
START_TARGET_SPECIFIC
extern int /*enum reg_class*/ default_branch_target_register_class (void);
extern bool hook_bool_const_rtx_commutative_p (const_rtx, int);
extern rtx default_function_value (const_tree, const_tree, bool);
extern rtx default_internal_arg_pointer (void);
#ifdef IRA_COVER_CLASSES
extern const int /*enum reg_class*/ *default_ira_cover_classes (void);
#endif
extern int /*enum reg_class*/ default_secondary_reload (bool, rtx,
							int /*enum reg_class*/,
						enum machine_mode,
						secondary_reload_info *);
extern bool default_override_options (bool);
extern tree default_vectype_for_scalar_type (tree, FILE *);
extern bool default_task_ok_for_target (struct gcc_target *, enum task_type);
extern bool default_common_data_with_target (struct gcc_target *);
extern enum machine_mode default_get_pmode (void);
END_TARGET_SPECIFIC
extern void hook_void_bitmap (bitmap);
extern bool default_handle_c_option (size_t, const char *, int);
extern int default_reloc_rw_mask (void);
extern tree default_mangle_decl_assembler_name (tree, tree);
extern tree default_emutls_var_fields (tree, tree *);
extern tree default_emutls_var_init (tree, tree, tree);
extern bool default_hard_regno_scratch_ok (unsigned int);
extern bool default_target_option_valid_attribute_p (tree, tree, tree, int);
extern bool default_target_option_pragma_parse (tree, tree);
extern bool default_target_option_can_inline_p (tree, tree);

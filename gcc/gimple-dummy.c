/* FIXME tuples.  HACK.  HACK.  HACK.

   This file defines a collection symbols that have been if0'd out
   during the conversion to GIMPLE tuples.  As the various passes get
   re-enabled, these symbols should be removed.  */

#include "config.h"
#include "system.h"

#define DUMMY_VAR(X)				\
  int X

#define DUMMY_FN(X)				\
  void X (void);				\
  void X (void) { gcc_unreachable (); }

DUMMY_VAR (chrec_dont_know);
DUMMY_VAR (chrec_known);
DUMMY_VAR (chrec_not_analyzed_yet);
DUMMY_VAR (memory_identifier_string);


DUMMY_FN (analyze_scalar_evolution);
DUMMY_FN (canonicalize_induction_variables);
DUMMY_FN (chrec_contains_symbols_defined_in_loop);
DUMMY_FN (compute_builtin_object_size);
DUMMY_FN (compute_data_dependences_for_loop);
DUMMY_FN (delete_tree_ssa);
DUMMY_FN (dump_ddrs);
DUMMY_FN (estimate_move_cost);
DUMMY_FN (estimate_num_insns);
DUMMY_FN (estimate_numbers_of_iterations);
DUMMY_FN (expr_invariant_in_loop_p);
DUMMY_FN (flush_pending_stmts);
DUMMY_FN (fold_stmt);
DUMMY_FN (free_data_refs);
DUMMY_FN (free_dependence_relations);
DUMMY_FN (free_histograms);
DUMMY_FN (free_numbers_of_iterations_estimates);
DUMMY_FN (free_numbers_of_iterations_estimates_loop);
DUMMY_FN (get_loop_exit_condition);
DUMMY_FN (get_type);
DUMMY_FN (gimple_duplicate_stmt_histograms);
DUMMY_FN (init_tree_ssa);
DUMMY_FN (instantiate_parameters);
DUMMY_FN (ipa_add_method);
DUMMY_FN (ipa_callsite_callee);
DUMMY_FN (ipa_callsite_compute_count);
DUMMY_FN (ipa_callsite_compute_param);
DUMMY_FN (ipa_callsite_param);
DUMMY_FN (ipa_callsite_param_count);
DUMMY_FN (ipa_callsite_param_count_set);
DUMMY_FN (ipa_edges_create);
DUMMY_FN (ipa_edges_free);
DUMMY_FN (ipa_free);
DUMMY_FN (ipa_jf_get_info_type);
DUMMY_FN (ipa_method_compute_modify);
DUMMY_FN (ipa_method_compute_tree_map);
DUMMY_FN (ipa_method_formal_compute_count);
DUMMY_FN (ipa_method_formal_count);
DUMMY_FN (ipa_method_formal_count_set);
DUMMY_FN (ipa_method_get_tree);
DUMMY_FN (ipa_method_modify_print);
DUMMY_FN (ipa_method_tree_print);
DUMMY_FN (ipa_methodlist_init);
DUMMY_FN (ipa_methodlist_not_empty);
DUMMY_FN (ipa_node_create);
DUMMY_FN (ipa_nodes_create);
DUMMY_FN (ipa_nodes_free);
DUMMY_FN (ipa_remove_method);
DUMMY_FN (ipa_type_escape_field_does_not_clobber_p);
DUMMY_FN (make_ssa_name);
DUMMY_FN (multiplier_allowed_in_address_p);
DUMMY_FN (multiply_by_cost);
DUMMY_FN (nowrap_type_p);
DUMMY_FN (optimize_inline_calls);
DUMMY_FN (remove_empty_loops);
DUMMY_FN (remove_phi_args);
DUMMY_FN (remove_unused_locals);
DUMMY_FN (reserve_phi_args_for_new_edge);
DUMMY_FN (rewrite_into_loop_closed_ssa);
DUMMY_FN (scev_const_prop);
DUMMY_FN (scev_finalize);
DUMMY_FN (scev_initialize);
DUMMY_FN (scev_probably_wraps_p);
DUMMY_FN (scev_reset);
DUMMY_FN (ssa_name_nonnegative_p);
DUMMY_FN (ssa_name_nonzero_p);
DUMMY_FN (stringop_block_profile);
DUMMY_FN (tree_duplicate_loop_to_header_edge);
DUMMY_FN (tree_function_versioning);
DUMMY_FN (tree_inlinable_function_p);
DUMMY_FN (tree_int_cst_sign_bit);
DUMMY_FN (tree_ssa_iv_optimize);
DUMMY_FN (tree_ssa_lim);
DUMMY_FN (tree_ssa_prefetch_arrays);
DUMMY_FN (tree_ssa_unswitch_loops);
DUMMY_FN (tree_unroll_loops_completely);
DUMMY_FN (tree_versionable_function_p);
DUMMY_FN (update_stmt_operands);
DUMMY_FN (vec_set_verbosity_level);
DUMMY_FN (vect_set_verbosity_level);
DUMMY_FN (vectorize_loops);
DUMMY_FN (verify_loop_closed_ssa);
DUMMY_FN (verify_ssa);
DUMMY_FN (number_of_iterations_exit);
DUMMY_FN (loop_niter_by_eval);
DUMMY_FN (estimated_loop_iterations_int);
DUMMY_FN (remove_phi_node);
DUMMY_FN (create_phi_node);
DUMMY_FN (add_phi_arg);
DUMMY_FN (substitute_in_loop_info);
DUMMY_FN (release_defs);
DUMMY_FN (push_stmt_changes);
DUMMY_FN (pop_stmt_changes);
DUMMY_FN (replace_exp);

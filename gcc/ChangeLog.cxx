2009-08-28  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/i386/i386.c (ix86_expand_args_builtin): Update to take new
	struct tag 'insn_data_d' into account.
	(ix86_expand_special_args_builtin): Likewise.

2009-08-19  J"orn Rennecke  <joern.rennecke@arc.com>

	* recog.h (struct insn_data): Rename to:
	(struct insn_data_d).  Change all users.

2009-05-20  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/rs6000/rs6000.c (legitimate_indirect_address_p):
	Don't define inline.

	* opt-gen.awk: For all variables with an Init clause, set var_seen.
	* config/rs6000/rs6000.c (rs6000_builtin_conversion): Adjust to
	target.h declaration.
	(rs6000_init_hard_regno_mode_ok): Add enum cast(s).
	(rs6000_override_options): Likewise.
	(bdesc_altivec_preds): Add enum casts.
	(altivec_expand_vec_set_builtin): Use EXPAND_NORMAL instead of 0.
	(rs6000_init_dwarf_reg_sizes_extra): Likewise.
	(emit_unlikely_jump, rs6000_emit_allocate_stack): Use alloc_reg_note.
	(rs6000_frame_related, rs6000_emit_prologue): Likewise.
	(output_toc): Use INSERT as last argument to htab_find_slot.
	(output_profile_hook): Use LCT_NORMAL Instead of 0.
	(rs6000_initialize_trampoline): Likewise.

	* config.gcc (spu-*-elf*): Use spu-cxx.o for cxx_target_objs.
	* config/spu/spu-protos.h (struct cpp_reader): Forward declaration.
	(spu_expand_epilogue) Use bool.
	* config/spu/spu-c.c [CC1PLUS]: Include cp/cp-tree.h instead of
	c-tree.h.
	[!CC1PLUS]: Define same_type_p.
	(spu_resolve_overloaded_builtin): Insert enum casts.  Use same_type_p.
	* config/spu/t-spu-elf (spu-c.o): Depend on $(C_TREE_H).
	Use $(CXXX) to build.
	(spu-cxx.o): New rule.
	* config/spu/spu.c (spu_scalar_mode_supported_p): Declare with bool.
	(spu_vector_mode_supported_p, spu_handle_fndecl_attribute): Likewise.
	(spu_handle_vector_attribute, spu_pass_by_reference): Likewise.
	(spu_rtx_costs, spu_function_ok_for_sibcall): Likewise.
	(spu_attribute_table): Declare extern.
	(spu_emit_branch_or_set): Fix type and scope of ior_code.
	(spu_expand_prologue): Use alloc_reg_note.
	(expand_builtin_args): Use EXPAND_NORMAL instead of 0.

2009-05-01  J"orn Rennecke  <joern.rennecke@arc.com>

	* var-tracking.c: Include tm_p.h .

	* targhooks.c (default_builtin_vectorized_function): Make signature
	agree with prototype in targhooks.h.
	(default_builtin_vectorized_conversion): Likewise.
	(default_builtin_reciprocal): Likewise.

	* expr.c (move_by_pieces, store_by_pieces): Append _d to struct tag.
	Changed all users.

	* config/sh/sh.c (sh_attribute_table): Use extern in forward
	declaration.
	(prepare_move_operands): Cast return value of tls_symbolic_operand.
	(expand_cbranchsi4, output_stack_adjust, push): Use alloc_reg_note.
	(pop, sh_expand_prologue): Likewise.
	(from_compare): Cast argument to gen_rtx_fmt_ee.
	(sh_reorg): Use PUT_REG_NOTE_KIND.
	(sh_expand_prologue): Add cast to enum machine_mode.
	(sh_expand_epilogue): Likewise.
	(get_free_reg): Add cast to enum attr_fp_mode.
	(sh_insn_length_adjustment): Compare sh_cpu with enum value of the
	proper type.
	(sh_initialize_trampoline): Use LCT_NORMAL.
	(sh_expand_builtin): Use EXPAND_NORMAL.

	* config/arc/arc.c (arc_attribute_table): Use extern in forward
	declaration.
	(output_shift): Replace initializatin of n with assignment.

2009-04-28  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/sh/sh.h (OVERRIDE_OPTIONS): Use enum processor_type values
	to assign to sh_cpu.

	* config/sh/sh.md (attribute cpu): Add comment about connection to
	enum processor_type in sh.h.

2009-04-27  J"orn Rennecke  <joern.rennecke@arc.com>

	* reorg.c (insn_sets_resource_p): include_delayed_effects is now bool.
	Pass proper type to mark_set_resources.

2009-04-25  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/sh/sh.md (push_fpscr, pop_fpscr): Use alloc_reg_note.
	(movdf_i4+1, reload_outdf__RnFRm+3, reload_outdf__RnFRm+4): Likewise.
	(reload_outdf__RnFRm+5, fpu_switch+1, fpu_switch+2): Likewise.

	* config/sh/predicates.md (trapping_target_operand): Rename and to
	and_expr.

2009-04-24  J"orn Rennecke  <joern.rennecke@arc.com>

	* auto-inc-dec.c (attempt_change): Pass proper type to rtx_cost.
	* regmove.c (try_auto_increment): Use PUT_REG_NOTE_KIND.
	* targhooks.h (default_builtin_vectorized_function): Make prototype
	agree with target.h:struct vectorize:builtin_vectorized_function.
	(default_builtin_vectorized_conversion): Likewise with respect to
	builtin_conversion.
	(default_builtin_reciprocal): Likewise wrt.
	struct gcc_target:builtin_reciprocal
	* config/m32r/m32r.h (INITIALIZE_TRAMPOLINE): Use LCT_NORMAL.
	* config/m32r/m32r.c (df.h): #include.
	(m32r_attribute_table): Mark forward declaration with extern.
	(pop): Use alloc_reg_note.
	(block_move_call): Use LCT_NORMAL.

2009-04-07  J"orn Rennecke  <joern.rennecke@arc.com>

	* cp/pt.c (tsubst_decl): Adjust forward declaration.

2009-03-25  Jerry Quinn  <jlquinn@cerberus.qb5.org>

	* config/i386/i386.c (ix86_function_specific_save): Don't check
	range of enum values.

2009-03-12  Tobias Schlüter  <tobi@gcc.gnu.org>

	* config/i386/x-darwin: Use ALL_CXXFLAGS.

2009-03-11  Tobias Schlüter  <tobi@gcc.gnu.org>

	* config/t-darwin: Use ALL_CXXFLAGS instead of ALL_CFLAGS.
	* config/x-darwin: Same.

2009-03-10  Tobias Schlüter  <tobi@gcc.gnu.org>

	* config/t-darwin: Use CXX as compiler.
	* config/x-darwin: Likewise.
	* config/i386/x-darwin: Same.

2009-03-06  Ian Lance Taylor  <iant@google.com>

	* toplev.h (floor_log2, exact_log2): Don't define as extern for
	C++.

2009-03-04  Ian Lance Taylor  <iant@google.com>

	* config/i386/i386.md (isinf<mode>2): Change slot to enum type.

	* cp-tree.h (finish_call_expr): Correct declaration.

	* rtl.h (SUBREG_PROMOTED_UNSIGNED_P): Add cast.
	* ipa-struct-reorg.c (create_general_new_stmt): Initialize rhs.
	* ira-build.c (copy_info_to_removed_store_destinations):
	Initialize parent_a.
	* varasm.c (const_rtx_hash_1): Don't declare 'shift' as const.
	* config/i386/i386.c (ix86_expand_builtin): Change fcode to enum
	type.

	* tree-nomudflap.c (gt_ggc_r_gt_tree_mudflap_h): Declare as extern
	if C++.

	* vec.c (vec_gc_o_reserve_1): Don't initialize alloc to itself.

	* gcc.c (process_command, main): Use CONST_CAST.
	* collect2.c (main, scan_prog_file): Likewise.

	* bitmap.c (bitmap_clear): Don't declare as inline.
	* tree-ssa-sccvn.c (vn_nary_op_compute_hash): Likewise.

	* cp/cvt.c (convert_to_void): Only warn about ?: operator with
	no effect if neither side has an effect.

	* objc/Make-lang.in (cc1obj-dummy$(exeext)): Link with $(CXX).
	(cc1obj$(exeext)): Likewise.
	* objc/objc-act.c (objc_rewrite_function_call): Change params to
	tree.
	(objc_gimplify_expr): Change return type to int.
	* objc/objc-act.h (objc_gimplify_expr): Update declaration.

	* java/jcf-io.c (find_class): Use CONST_CAST.

	* fortran/Make-lang.in (gfortranspec.o): Compile with $(CXX).
	(fortran/cpp.o): Likewise.
	(gfortran$(exeext)): Link with $(CXX).
	(f951$(exeext)): Likewise.
	* fortran/gfortran.h (enum omp_sched_kind): Move out of struct
	gfc_omp_clauses.
	(enum omp_sharing): Likewise.
	(enum gfc_symbol_type): Move out of struct gfc_gsymbol.
	(enum gfc_array_ref_dimen_type): Move out of strutc
	gfc_array_ref.
	* fortran/cpp.c (struct gfc_cpp_option_data): Give name to
	anonymous struct.
	* fortran/decl.c (build_struct): Initialize first_len.
	(match_attr_spec): Change variable 'd' to int.
	(add_global_entry): Change variable 'type' to gfc_symbol_type.
	* fortran/dump-parse-tree.c (show_namespace): Change variable 'op'
	to int.
	* fortran/interface.c (gfc_check_interfaces): Change loop to use
	int type rather than enum type.
	* fortran/module.c (enum rsym_state, enum wsym_state): Move out of
	struct pointer_info.
	(mio_array_ref, mio_symbol): Add casts to enum type.
	(read_module): Change variable 'i' to int.
	(write_module): Likewise.
	(import_iso_c_binding_module): Add cast to enum type.
	* fortran/parse.c (enum state_order): Move out of st_state
	struct.
	* fortran/resolve.c (resolve_global_procedure): Change variable
	'type' to gfc_symbol_type.
	(check_host_association): Initialize tail.
	* fortran/symbol.c (gfc_get_namespace): Change variable 'in' to
	int.
	(gfc_free_namespace): Change variable 'i' to int.
	* fortran/trans-intrinsic.c (DEFINE_MATH_BUILTIN): Add casts to
	enum type.
	* fortran/trans-io.c (st_parameter_field): Add casts to enum
	type.
	(gfc_build_st_parameter): Change variable 'type' to int.
	(gfc_build_io_library_fndecls): Change variable 'ptype' to int.
	* fortran/trans-types.c (gfc_init_kinds): Change loops to use int
	type rather than enum type.

2009-03-03  Thomas Neumann <tneumann@users.sourceforge.net>

	* cp/parser.c (cp_lexer_get_preprocessor_token): Add missing cast
	to enum.

2009-03-03  Thomas Neumann <tneumann@users.sourceforge.net>

	* tree-into-ssa.c (get_ssa_name_ann): Use NEED_PHI_STATE_UNKNOWN
	instead of 0.

2009-03-03  Thomas Neumann <tneumann@users.sourceforge.net>

	* langhooks.c (add_builtin_function_common): Avoid assigning -1 to
	an enum for C++ compatibility.

2009-02-17  Thomas Neumann <tneumann@users.sourceforge.net>

        * ira.c (setup_prohibited_mode_move_regs): Add missing casts to
        enum machine_mode.

2009-02-10  Ian Lance Taylor  <iant@google.com>

	* df.h (struct df_mw_hardreg): Change flags field to int.
	(struct df_base_ref): Likewise.

	* parser.c (cp_parser_label_for_labeled_statement): Permit
	attributes on a label.
	* semantics.c (finish_label_stmt): Return the new label decl, not
	the new statement.
	* decl2.c (is_late_template_attribute): Return true for "unused"
	on a label.
	* pt.c (tsubst_expr): If the original label decl has attributes,
	apply them to the new label decl.

2009-02-06  Ian Lance Taylor  <iant@google.com>

	* decl2.c (constrain_visibility): Add cast to enum type.

	* parser.c (cp_parser_question_colon_clause): Set skip_evaluation
	if the condition is known to be true or false.
	* typeck.c (cp_build_binary_op): Don't warn about out of range
	shift counts if skip_evaluation is true.

	* timevar.c (timevar_pop_1): Ignore TV_NONE.

2009-02-04  Ian Lance Taylor  <iant@google.com>

	* configure.ac: Use AC_PROG_CXX.
	* Makefile.in (CXX): Set to @CXX@.
	* configure: Rebuild.

	* main.c (main): Use CONST_CAST when adding potentially unsafe
	const.
	* attribs.c (init_attributes): Change slot to void** and use
	CONST_CAST when assigning to it.

	* collect2.c (is_ctor_dtor): Change ret field of struct names to
	symkind.
	* df-scan.c (df_ref_create_structure): Change struct df_ref * in
	declaration to just df_ref.
	(df_def_record_1): Change 0 to VOIDmode.
	(df_get_conditional_uses): Likewise.
	(df_defs_record): Change clobber_flags to int.
	* fixed-value.c (fixed_compare): Add cast to enum type.
	* haifa-sched.c (reemit_notes): Likewise
	(max_issue): Put empty loop ';' on new line.
	* ira.c (setup_class_subset_and_memory_move_costs): Add casts to
	enum type.
	(setup_cover_and_important_classes): Likewise.
	(setup_reg_class_relations): Likewise.
	* real.c (real_arithmetic): Likewise.
	(real_compare): Likewise.
	* rtl.h (NOTE_VAR_LOCATION_STATUS): Likewise.
	* tree-cfg.c (gimple_redirect_edge_and_branch): Change ERROR_MARK
	to GIMPLE_ERROR_MARK.
	* tree-data-ref.c (print_direction_vector): Add cast to enum
	type.
	* tree-ssa-alias.c (set_initial_properties): Change
	pt_anything_mask to int.
	* tree-vectorizer.c (vect_set_verbosity_level): Add casts to enum
	type.
	(supportable_widening_operation): Likewise.
	(new_stmt_vec_info): Change 0 to enum constants.
	(vect_supportable_dr_alignment): Remove useless cast.
	* tree-vect-analyze.c (vect_build_slp_tree): Change 0 to enum
	constants.
	* tree-vect-transform.c (vect_create_epilog_for_reduction):
	Likewise.
	(vectorizable_reduction): Likewise.  Add casts to enum types.
	* var-tracking.c (get_init_value): Change return type and ret_val
	local variable to enum var_init_status.

	* ira-build.c (setup_min_max_conflict_allocno_ids): Use
	LIM_REG_CLASSES instead of -1.

	* config/linux.opt: Put mglibc before muclibc.

	* df-problems.c (df_byte_lr_alloc): Don't assign problem_data to
	itself.
	(df_simulate_one_insn_forwards): Move default case out of inner
	block.

	* gimplify.c (gimplify_omp_for): Use MIN, not bitwise or, to
	gather gimplification status from subroutines.
	(gimplify_expr): Add cast to enum type.

	* dse.c (struct store_info): Change field name from bitmap to
	bmap.  Change all uses.

	* dwarf2.h (enum dwarf_type): Add DW_ATE_lo_user and
	DW_ATE_hi_user as explicit enum values rather than #define
	macros.
	* dwarf2out.c (struct indirect_string_node): Change form to enum
	dwarf_form.
	(INTERNAL_DW_OP_tls_addr): Add cast to enum type.
	(AT_string_form): Change return type to enum dwarf_form.
	(int_loc_descriptor): Add cast to enum type.  Add cast to
	HOST_WIDE_INT for unsigned constant.
	(add_calling_convention_attribute): Add cast to enum type.

	* cp/class.c (resolve_address_of_overloaded_function): Change
	flags to int.
	(instantiate_type): Change flags_in to int.
	* cp/call.c (standard_conversion): Change flags to int.
	(reference_binding): Change lvalue_p to int.
	(build_temp): Change 0 to DK_UNSPECIFIED.
	(convert_like_real): Change lvalue to int.
	* cp/tree.c (lvalue_p_1): Change return type to int. Change
	op1_lvalue_kind and op2_lvalue_kind to int.
	(real_lvalue_p): Change return type to int.
	* cp/cp-gimplify.c (cp_gimplify_expr): Add cast to enum type.
	* cp/cp-tree.h (instantiate_template): Update declaration.
	(real_lvalue_p): Likewise.
	* cp/pt.c (check_instantiated_args): Change complain to int.
	(instantiate_template): Likewise.

	* cp/error.c (next_tcode, next_int): Pass int to va_arg.

2009-02-03  Ian Lance Taylor  <iant@google.com>

	* opts.c (set_debug_level): Don't put an arbitrary int in an
	enum.

	* omega.c (omega_do_elimination): Avoid math on enum types.

	* gcov.c (read_count_file): Avoid goto.

	* fold-const.c (fold_undefer_overflow_warnings): Add cast to enum
	type.
	(combine_comparisons): Change compcode to int.  Add cast.

	* builtins.c (validate_gimple_arglist): Don't pass an enum typ eto
	va_arg.
	* calls.c (emit_library_call_value_1): Likewise.

	* mcf.c (add_fixup_edge): Change type parameter to edge_type.

	* tree.c (attribute_list_contained): Put empty loop ';' on new
	line.
	(build_common_builtin_nodes): Add casts to enum type.

2009-02-01  Tom Tromey  <tromey@redhat.com>

	* c-pch.c (get_ident) <templ>: Don't specify array length.
	(pch_init) <partial_pch>: Likewise.

2009-01-31  Ian Lance Taylor  <iant@google.com>

	* genattrtab.c (write_length_unit_log): If C++, declare const to
	be extern.
	* genchecksum.c (dosum): Likewise.
	* gengtype.c (write_rtx_next, finish_root_table): Likewise.
	(write_roots): Likewise.
	* dummy-checksum.c (executable_checksum): Likewise.
	* gimple.c (gimple_ops_offset_): Likewise.
	(gimplify_assign): Do not declare as inline.

	* reginfo.c (init_move_cost): Add casts to enum type and integer
	types.
	(init_reg_sets_1, init_fake_stack_mems): Likewise.
	(cannot_change_mode_set_regs): Change 'to' to unsigned int.  Add
	casts to enum type.
	(invalid_mode_change_p): Likewise.
	(pass_reginfo_init): Use TV_NONE for tv_id initializer.
	(pass_subregs_of_mode_init): Likewise.
	(pass_subregs_of_mode_finish): Likewise.

	* reload.c (push_secondary_reload): Add cast for
	secondary_reload_info icode field.
	(secondary_reload_class): Likewise.
	(find_valid_class): Likewise.
	(alternative_allows_const_pool_ref): Put empty loop ';' on new
	line.
	* reload1.c (emit_input_reload_insns): Add cast for
	secondary_reload_info icode field.

	* cse.c (insert): Put empty loop ';' on new line.
	* emit-rtl.c (push_to_sequence): Likewise.
	* matrix-reorg.c (add_allocation_site): Likewise.
	* postreload-gcse.c (eliminate_partially_redundant_load):
	Likewise.
	* sched-rgn.c (rgn_add_block): Likewise.
	(rgn_fix_recovery_cfg): Likewise.
	* tree-loop-distribution.c (rdg_flag_similar_memory_accesses):
	Likewise.

	* profile.c (compute_branch_probabilities): Remove useless
	references to variable on right side of comma operator.

	* parser.c (cp_parser_expression_stack_entry): Change prec field
	to enum cp_parser_prec.
	(cp_parser_decl_specifier_seq): Change flags to int.
	(cp_parser_type_specifier): Likewise.
	(cp_parser_simple_type_specifier): Likewise.
	(cp_parser_type_specifier_seq): Likewise.
	(cp_parser_direct_declarator): Don't jump into variable scope.

	* cp/semantics.c (finish_omp_clauses): Change c_kind to enum
	omp_clause_code.

	* tree-complex.c (complex_lattice_t): Change to int type.  Leave
	enum type unnamed.
	(expand_complex_libcall): Add casts to enum type.

	* hooks.h (hook_int_void_no_regs): Don't declare.
	* hooks.c (hook_int_void_no_regs): Don't define.

	* target.h (struct sched): Correct needs_block_p prototype.
	(struct gcc_target): Change return type of
	branch_target_register_class to enum reg_class.
	* target-def.h (TARGET_BRANCH_TARGET_REGISTER_CLASS): Change to
	default_branch_target_register_class.
	* targhooks.h (default_branch_target_register_class): Declare.
	* targhooks.c (default_branch_target_register_class): Define.

	* fwprop.c (update_df): Change 0 to VOIDmode.
	* sel-sched-ir.c (hash_with_unspec_callback): Likewise.
	* combine.c (record_value_for_reg): Likewise.
	(record_dead_and_set_regs): Likewise.
	* optabs.c (expand_widen_pattern_expr): Likewise.
	(expand_vec_shift_expr): Don't cast insn_code field of
	optab_handler to int.
	(emit_cmp_and_jump_insn_1): Likewise.

	* Makefile.in (xgcc$(exeext)): Link with $(CXX).
	(cpp$(exeext)): Likewise.
	(cc1-dummy$(exeext), cc1$(exeext)): Likewise.
	(collect2$(exeext)): Likewise.
	(protoize$(exeext), unprotoize$(exeext)): Likewise.
	(gcov$(exeext), gcov-dump$(exeext)): Likewise.
	(collect2.o, c-opts.o, c-cppbuiltin.o): Compile with $(CXX).
	(c-pch.o, gcc.o, gccspec.o, gcc-options.o): Likewise.
	(version.o, prefix.o, toplev.o, intl.o): Likewise.
	($(out_object_file)): Likewise.
	(libbackend.o): Likewise.
	(cppdefault.o): Likewise.
	(protoize.o, unprotoize.o): Likewise.
	* cp/Make-lang.in (g++spec.o): Likewise.
	(g++$(exeext)): Link with $(CXX).
	(cc1plus-dummy$(exeext), cc1plus$(exeext)): Likewise.
	* config/x-linux (host-linux.o): Compile with $(CXX).

	* Makefile.in (omp.low.o): Depend upon gt-omp-low.h.
	(ipa-reference.o): Depend upon gt-ipa-reference.h.
	(ipa-cp.o): Depend upon $(FIBHEAP_H) and $(PARAMS_H).
	* cp/Make-lang.in (cp/class.o): Depend upon gt-cp-class.h.
	(cp/semantics.o): Depend upon gt-cp-semantics.h.

	* config/i386/i386.c (ix86_function_specific_restore): Add casts
	to enum types.
	(bdesc_multi_arg): Change 0 to UNKNOWN where appropriate.  Add
	casts to enum type where needed.
	* config/i386/i386-c.c (ix86_pragma_target_parse): Add casts to
	enum types.

	* config/i386/x-i386 (driver-i386.o): Build with $(CXX).
	* config/i386/t-i386 (i386-c.o): Likewise.

2008-10-06  Tom Tromey  <tromey@redhat.com>

	* errors.h (progname): Wrap in 'extern "C"'.

2008-10-05  Tom Tromey  <tromey@redhat.com>

	* bitmap.c (bitmap_obstack_alloc_stat): Remove extra cast.
	(bitmap_obstack_free): Likewise.

2008-10-05  Tom Tromey  <tromey@redhat.com>

	* dominance.c (iterate_fix_dominators): Cast argument to
	BITMAP_FREE.
	* bitmap.c (bitmap_obstack_alloc_stat): Add cast.
	(bitmap_obstack_free): Likewise.

2008-10-05  Tom Tromey  <tromey@redhat.com>

	* fold-const.c (fold_unary): Rename 'and' to 'and_expr'.

2008-10-05  Tom Tromey  <tromey@redhat.com>

	* sel-sched.c (move_op_hooks, fur_hooks): Mark forward
	declarations 'extern'.
	* regstat.c (regstat_n_sets_and_refs): Remove duplicate.
	* haifa-sched.c (sched_scan_info): Remove duplicate.

2008-10-05  Tom Tromey  <tromey@redhat.com>

	* cgraph.h (struct inline_summary): Move to top level.
	(cgraph_local_info): Update.
	* target.h (struct asm_out): Move to top level.
	(struct asm_int_op): Likewise.
	(struct sched): Likewise.
	(struct vectorize): Likewise.
	(struct calls): Likewise.
	(struct c): Likewise.
	(struct cxx): Likewise.
	(struct emutls): Likewise.
	(struct target_option_hooks): Likewise.
	(struct gcc_target): Update.
	* matrix-reorg.c (struct free_info): Move to top level.
	(struct matrix_info): Update.
	* tree-eh.c (struct goto_queue_node): Move to top level.
	(struct leh_tf_state): Update.
	* sched-int.h (struct deps_reg): Move to top level.
	(enum post_call_value): Likewise.  Give name.
	(struct deps): Update.
	* cse.c (struct branch_path): Move to top level.
	(struct cse_basic_block_data): Update.

2008-10-05  Tom Tromey  <tromey@redhat.com>

	* sdbout.c (sdb_debug_hooks): Initialize.
	* ggc-page.c (ggc_pch_write_object): Initialize emptyBytes.

2008-09-19  Tom Tromey  <tromey@redhat.com>

	* gimple.h (gimple_cond_code): Cast result to tree_code.

2008-09-19  Tom Tromey  <tromey@redhat.com>

	* tree-flow.h (struct ptr_info_def) <escape_mask>: Now unsigned
	int.
	(struct var_ann_d) <escape_mask>: Likewise.

2008-09-19  Tom Tromey  <tromey@redhat.com>

	* tree.c (tree_range_check_failed): Use 'int' to iterate.
	(omp_clause_range_check_failed): Likewise.
	(build_common_builtin_nodes): Likewise.
	* sel-sched.c (init_hard_regs_data): Use 'int' to iterate.
	* regclass.c (cannot_change_mode_set_regs): Use 'int' to iterate.
	(invalid_mode_change_p): Likewise.
	* passes.c (finish_optimization_passes): Use 'int' to iterate.
	* ira.c (setup_class_subset_and_memory_move_costs): Use 'int' to
	iterate.
	(setup_cover_and_important_classes): Likewise.
	(setup_class_translate): Likewise.
	(setup_reg_class_nregs): Likewise.
	(ira_init_once): Likewise.
	(free_register_move_costs): Likewise.
	* gimple.c (gimple_range_check_failed): Use 'int' to iterate.

2008-09-18  Tom Tromey  <tromey@redhat.com>

	* cp/parser.c (cp_parser_check_decl_spec): Use 'int' to iterate.
	* cp/class.c (layout_class_type): Use 'int' to iterate.
	* cp/decl.c (finish_enum): Use 'int' to iterate.

2008-09-17  Tom Tromey  <tromey@redhat.com>

	* ipa-reference.c (pass_ipa_reference): Fix struct tag.
	* ipa-pure-const.c (pass_ipa_pure_const): Fix struct tag.
	* ipa-cp.c (pass_ipa_cp): Fix struct tag.
	* except.c (add_call_site): Fix struct tag.
	* tree-pass.h (pass_ipa_cp, pass_ipa_reference,
	pass_ipa_pure_const): Fix struct tag.

2008-09-17  Tom Tromey  <tromey@redhat.com>

	* cfgrtl.c (pass_free_cfg): Use TV_NONE.
	* tree-vectorizer.c (pass_ipa_increase_alignment): Use TV_NONE.
	* tree-vect-generic.c (pass_lower_vector): Use TV_NONE.
	(pass_lower_vector_ssa): Likewise.
	* tree-tailcall.c (pass_tail_recursion): Use TV_NONE.
	(pass_tail_calls): Likewise.
	* tree-stdarg.c (pass_stdarg): Use TV_NONE.
	* tree-ssanames.c (pass_release_ssa_names): Use TV_NONE.
	* tree-ssa-math-opts.c (pass_cse_reciprocals): Use TV_NONE.
	(pass_cse_sincos): Likewise.
	(pass_convert_to_rsqrt): Likewise.
	* tree-ssa-dse.c (pass_simple_dse): Use TV_NONE.
	* tree-ssa-ccp.c (pass_fold_builtins): Use TV_NONE.
	* tree-ssa.c (pass_early_warn_uninitialized): Use TV_NONE.
	(pass_late_warn_uninitialized): Likewise.
	(pass_update_address_taken): Likewise.
	* tree-ssa-alias.c (pass_reset_cc_flags): Use TV_NONE.
	(pass_build_alias): Likewise.
	* tree-optimize.c (pass_all_optimizations): Use TV_NONE.
	(pass_early_local_passes): Likewise.
	(pass_all_early_optimizations): Likewise.
	(pass_cleanup_cfg): Likewise.
	(pass_cleanup_cfg_post_optimizing): Likewise.
	(pass_free_datastructures): Likewise.
	(pass_free_cfg_annotations): Likewise.
	(pass_init_datastructures): Likewise.
	* tree-object-size.c (pass_object_sizes): Use TV_NONE.
	* tree-nrv.c (pass_return_slot): Use TV_NONE.
	* tree-nomudflap.c (pass_mudflap_1): Use TV_NONE.
	(pass_mudflap_2): Likewise.
	* tree-mudflap.c (pass_mudflap_1): Use TV_NONE.
	(pass_mudflap_2): Likewise.
	* tree-into-ssa.c (pass_build_ssa): Use TV_NONE.
	* tree-if-conv.c (pass_if_conversion): Use TV_NONE.
	* tree-complex.c (pass_lower_complex): Use TV_NONE.
	(pass_lower_complex_O0): Likewise.
	* tree-cfg.c (pass_remove_useless_stmts): Use TV_NONE.
	(pass_warn_function_return): Likewise.
	(pass_warn_function_noreturn): Likewise.
	* stack-ptr-mod.c (pass_stack_ptr_mod): Use TV_NONE.
	* regclass.c (pass_regclass_init): Use TV_NONE.
	(pass_subregs_of_mode_init): Likewise.
	(pass_subregs_of_mode_finish): Likewise.
	* recog.c (pass_split_all_insns): Use TV_NONE.
	(pass_split_after_reload): Likewise.
	(pass_split_before_regstack): Likewise.
	(pass_split_before_sched2): Likewise.
	(pass_split_for_shorten_branches): Likewise.
	* omp-low.c (pass_expand_omp): Use TV_NONE.
	(pass_lower_omp): Likewise.
	* matrix-reorg.c (pass_ipa_matrix_reorg): Use TV_NONE.
	* jump.c (pass_cleanup_barriers): Use TV_NONE.
	* ira.c (pass_ira): Use TV_NONE.
	* integrate.c (pass_initial_value_sets): Use TV_NONE.
	* init-regs.c (pass_initialize_regs): Use TV_NONE.
	* gimple-low.c (pass_lower_cf): Use TV_NONE.
	(pass_mark_used_blocks): Likewise.
	* function.c (pass_instantiate_virtual_regs): Use TV_NONE.
	(pass_init_function): Likewise.
	(pass_leaf_regs): Likewise.
	(pass_match_asm_constraints): Likewise.
	* final.c (pass_compute_alignments): Use TV_NONE.
	* except.c (pass_set_nothrow_function_flags): Use TV_NONE.
	(pass_convert_to_eh_region_ranges): Likewise.
	* emit-rtl.c (pass_unshare_all_rtl): Use TV_NONE.
	* combine-stack-adj.c (pass_stack_adjustments): Use TV_NONE.
	* cgraphbuild.c (pass_build_cgraph_edges): Use TV_NONE.
	(pass_rebuild_cgraph_edges): Likewise.
	* cfglayout.c (pass_into_cfg_layout_mode): Use TV_NONE.
	(pass_outof_cfg_layout_mode): Likewise.
	* bt-load.c (pass_branch_target_load_optimize1): Use TV_NONE.
	(pass_branch_target_load_optimize2): Likewise.
	* passes.c (pass_postreload): Use TV_NONE.
	(execute_one_ipa_transform_pass): Unconditionally push and pop
	timevar.
	(execute_one_pass): Likewise.
	* df-core.c (pass_df_initialize_opt): Use TV_NONE.
	(pass_df_finish): Likewise.
	(pass_df_initialize_no_opt): Likewise.
	* timevar.c (timevar_print): Ignore TV_NONE.
	(timevar_push_1): Likewise.
	* timevar.def (TV_NONE): New timevar.

2008-09-10  Ian Lance Taylor  <iant@google.com>

	* machmode.h (GET_MODE_CLASS): Add cast to enum type.
	(GET_CLASS_NARROWEST_MODE): Likewise.
	* tree.h (PREDICT_EXPR_OUTCOME): Add cast to enum type.
	(SET_PREDICT_EXPR_OUTCOME): Define.
	* rtl.h: Update declaration.
	* calls.c (store_one_arg): Change 0 to enum constant.
	* combine.c (try_combine): Use alloc_reg_note.
	(recog_for_combine, move_deaths, distribute_notes): Likewise.
	* combine-stack-adj.c (adjust_frame_related_expr): Use
	add_reg_note.
	* cse.c (hash_rtx_cb): Change 0 to enum constant.
	* dbgcnt.c (dbg_cnt_set_limit_by_name): Add cast to enum type.
	* dbxout.c (dbxout_symbol): Change 0 to enum constant.
	(dbxout_parms): Likewise.
	* dce.c (run_fast_df_dce): Change old_flags to int.
	* df.h: Include "timevar.h".  Update declarations.
	(enum df_ref_flags): Define DF_REF_NONE.
	(struct df_problem): Change tv_id to timevar_id_t.
	(struct df): Change changeable_flags to int.
	* df-core.c (df_set_flags): Change return type and
	changeable_flags and old_flags to int.
	(df_clear_flags): Likewise.
	* df-problems.c (df_rd_bb_local_compute): Change 0 to enum
	constant.
	(df_chain_create_bb): Likewise.
	(df_chain_add_problem): Change flags to unsigned int.
	* df-scan.c (df_ref_create): Change ref_flags to int.
	(df_notes_rescan): Change 0 to enum constant.
	(df_ref_create_structure): change ref_flags to in.
	(df_ref_record, df_def_record_1): Likewise.
	(df_defs_record, df_uses_record): Likewise.
	(df_get_call_refs): Likewise.  Change 0 to enum constant.
	(df_insn_refs_collect): Change 0 to enum constant.
	(df_bb_refs_collect): Likewise.
	(df_entry_block_defs_collect): Likewise.
	(df_exit_block_uses_collect): Likewise.
	* double-int.c (double_int_divmod): Add cast to enum type.
	* dse.c (replace_inc_dec): Correct parameters to gen_int_mode.
	* dwarf2out.c (new_reg_loc_descr): Add cast to enum type.
	(based_loc_descr): Likewise.
	(loc_descriptor_from_tree_1): Change first_op and second_top to
	enum dwarf_location_atom.
	* expmed.c (init_expmed): Change 0 to enum constant.
	* expr.c (init_expr_target): Change 0 to enum constant.
	(expand_expr_real_1): Likewise.
	* fixed-value.h (struct fixed_value): Change mode to enum
	machine_mode.
	* genautomata.c (insert_automaton_decl): Change integer constant
	to enum constant.
	(insert_insn_decl, insert_decl, insert_state): Likewise.
	(automata_list_finish): Likewise.
	* genrecog.c (process_define_predicate): Add cast to enum type.
	* gensupport.c (init_predicate_table): Add cast to enum type.
	* gimple.c (gimple_build_return): Change 0 to enum constant.
	(gimple_build_call_1, gimple_build_label): Likewise.
	(gimple_build_goto, gimple_build_asm_1): Likewise.
	(gimple_build_switch_1, gimple_build_cdt): Likewise.
	* gimple.h: Update declaration.
	(GIMPLE_CHECK): Change 0 to enum constant.
	* gimple-low.c (lower_builtin_setjmp): Change TSI_SAME_STMT to
	GSI_SAME_STMT.
	* gimplify.c (gimplify_compound_lval): Change fallback to int.
	(gimplify_cond_expr, gimplify_expr): Likewise.
	* haifa-sched.c (sched_create_recovery_edges): Use add_reg_note.
	* ipa-prop.c (update_jump_functions_after_inlining): Change
	IPA_BOTTOM to IPA_UNKNOWN.
	* ira.c (setup_class_subset_and_memory_move_costs): Add cast to
	enum type.
	(setup_reg_class_intersect_union): Likewise.
	(setup_prohibited_class_mode_regs): Likewise.
	(setup_prohibited_mode_move_regs): Likewise.
	* ira-costs.c (record_reg_classes): Likewise.
	* lists.c (alloc_EXPR_LIST): Add cast to enum type.
	* omp-low.c (expand_omp_for): Add cast to enum type.
	* optabs.c (debug_optab_libfuncs): Add cast to enum type.
	* opts.c (enable_warning_as_error): Change kind to diagnostic_t.
	* postreload.c (reload_cse_simplify_operands): Likewise.
	* predict.c (combine_predictions_for_insn): Add cast to enum
	type.
	(combine_predictions_for_bb): Likewise.
	(estimate_bb_frequencies): Check profile_status, not
	function_frequency.
	(build_predict_expr): Use SET_PREDICT_EXPR_OUTCOME.
	* real.c (real_arithmetic): Add cast to enum type.
	* regclass.c (init_move_cost, init_reg_sets_1): Add cast to enum
	type.
	(init_fake_stack_mems, record_reg_classes): Likewise.
	* regmove.c (regclass_compatible_p): Change class0 and class1 to
	enum reg_class.
	(try_auto_increment): Use PUT_REG_NOTE_KIND rather than PUT_MODE.
	* reload.c (find_valid_class): Add cast to enum type.
	(push_reload): Change 0 to enum constant.
	(find_reloads): Add cast to enum type.
	(make_memloc): Change 0 to enum constant.
	* reload1.c (reload): Change 0 to enum constant.
	(eliminate_regs_1): Use alloc_reg_note.  Change 0 to enum
	constant.
	(elimination_effects): Change 0 to enum constant.
	(eliminate_regs_in_insn, delete_output_reload): Likewise.
	(emit_input_reload_insns): Add cast to enum type.
	* rtlanal.c (alloc_reg_note): New function.
	(add_reg_note): Call it.
	* tree-dump.c (get_dump_file_info): Change phase to int.  Add cast
	to avoid warning.
	(get_dump_file_name, dump_begin): Change phase to int.
	(dump_enabled_p, dump_initialized_p): Likewise.
	(dump_flag_name, dump_end, dump_function): Likewise.
	* tree-dump.h (dump_function): Update declaration.
	* tree-pass.h: Include "timevar.h".  Update several function
	declarations.
	(struct opt_pass): Change tv_id to timevar_id_t.
	* tree-vect-patterns.c (vect_pattern_recog_1): Change vec_mode to
	enum machine_mode.
	* varasm.c (assemble_integer): Change mclass to enum mode_class.
	* config/i386/i386.md (cpu attr): Add cast to enum type.
	(truncdfsf2): Change slot to enum ix86_stack_slot.
	(truncxf<mode>2, floatunssi<mode>2): Likewise.
	* config/i386/i386-c.c (ix86_pragma_target_parse): Add cast to
	enum type.
	* config/i386/i386.c (ix86_expand_prologue): Add add_reg_note.
	(ix86_split_fp_branch, predict_jump): Likewise.
	(ix86_expand_multi_arg_builtin): Change sub_code to enum
	rtx_code.
	(ix86_builtin_vectorized_function): Add cast to enum type.
	* cp/call.c (build_new_function_call): Change complain to int.
	(build_object_call, build_conditional_expr): Likewise.
	(build_new_op, convert_like_real, build_over_call): Likewise.
	(build_special_member_call, build_new_method_call): Likewise.
	(perform_implicit_conversion): Likewise.
	(perform_direct_initialization_if_possible): Likewise.
	* cp/class.c (instantiate_type): Change complain to int.
	* cp/cp-lang.c (objcp_tsubst_coy_and_build): Change complain to
	int.
	* cp/cvt.c (convert_to_void): Change complain to int.
	* cp/decl.c (make_typename_type): Change complain to int.
	(make_unbound_class_template): Likewise.
	* cp/init.c (build_aggr_init): Change complain to int.
	(expand_default_init, expand_aggr_init_1, build_new_1): Likewise.
	(build_new, build_vec_init): Likewise.
	* cp/parser.c (cp_parser_omp_var_list_no_open): Change integer
	constant to enum constant.
	(cp_parser_omp_flush, cp_parser_omp_threadprivate): Likewise.
	* cp/pt.c (reduce_template_parm_level): Change complain to int.
	(coerce_template_template_parm): Likewise.
	(coerce_template_template_parms): Likewise.
	(convert_template_argument): Likewise.
	(coerce_template_parameter_pack): Likewise.
	(coerce_template_parms, lookup_template_class): Likewise.
	(apply_late_template_attributes): Likewise.
	(tsubst_template_arg, tsubst_pack_expansion): Likewise.
	(tsubst_tempalte_args, tsubst_tempalte_parms): Likewise.
	(tsubst_aggr_type, tsubst_decl, tsubst_arg_types): Likewise.
	(tsubst_function_type, tsubst_exception_specification): Likewise.
	(tsubst, tsubst_baselink, tsubst_qualified_id): Likewise.
	(tsubst_copy, tsubst_omp_clauses): Likewise.
	(tsubst_copy_asm_operand, tsubst_omp_for_iterator): Likewise.
	(tsubst_expr, tsubst_non_call_postfix_expression): Likewise.
	(tsubst_copy_and_build, check_instantiated_args): Likewise.
	(do_type_instantiation, invalid_nontype_parm_type_p): Likewise.
	(process_template_parm): Change integer constant to enum constant.
	(unify_pack_expansion): Add cast to enum type.
	* cp/rtti.c (build_dynamic_cast_1): Change complain to int.
	(build_dynamic_cast): Likewise.
	* cp/search.c (lookup_base): Change access to int.
	* cp/semantics.c (finish_call_expr): Change complain to int.
	* cp/tree.c (cp_build_qualified_type_real): Change complain to
	int.
	* cp/typeck.c (composite_pointer_type_r): Change complain to int.
	(composite_pointer_type, invalid_nonstatic_memfn_p): Likewise.
	(build_class_member_access_expr): Likewise.
	(finish_class_member_access_expr): Likewise.
	(build_x_indirect_ref, cp_build_indirect_ref): Likewise.
	(cp_build_function_call, convert_arguments): Likewise.
	(build_x_binary_op, cp_build_binary_op): Likewise.
	(build_x_unary_op, cp_build_unary_op): Likewise.
	(build_x_conditional_expr): Likewise.
	(build_x_compound_expr, cp_build_compound_expr): Likewise.
	(build_static_cast_1, build_static_cast): Likewise.
	(build_reinterpret_cast_1, build_reinterpret_cast): Likewise.
	(build_const_cast, cp_build_c_cast): Likewise.
	(cp_build_modify_expr, build_x_modify_expr): Likewise.
	(convert_for_assignment, convert_for_initialization): Likewise.
	(lvalue_or_else): Likewise.
	* cp/typeck2.c (build_functional_cast): Change complain to int.
	* cp/cp-tree.h: Update declarations.
	* cp/cp-objcp-common.h: Update declaration.
	* fortran/decl.c (gfc_mod_pointee_as): Change return type to
	match.
	* fortran/gfortran.h: Update declaration.
	* fortran/module.c (import_iso_c_binding_module): Use new
	iso_c_binding_symbol local.  Add cast to enum type.
	* fortran/trans-intrinsic.c (gfc_conv_intrinsic_minmax): Change op
	to enum tree_code.
	(gfc_conv_intrinsic_anyall, gfc_conv_intrinsic_arith): Likewise.
	(gfc_conv_intrinsic_minmaxloc): Likewise.
	(gfc_conv_intrinsic_minmaxval): Likewise.
	(gfc_conv_intrinsic_bitop): Likewise.
	(gfc_conv_intrinsic_singlebitop): Likewise.
	(gfc_conv_intrinsic_strcmp): Likewise.
	* Makefile.in: Change tree-pass.h to $(TREE_PASS_H) globally.
	(DF_H): Add $(TIMEVAR_H).
	(TREE_PASS_H): New variable.

2008-07-05  Tom Tromey  <tromey@redhat.com>

	* basic-block.h (enum profile_status): Move to top level.
	(struct control_flow_graph): Update.
	* combine.c (enum undo_kinds): Move to top level.
	(struct undo): Update.
	* tree-inline.h (enum copy_body_cge_which): Move to top level.
	(copy_body_data): Update.
	* ggc-page.c (struct ggc_pch_ondisk): Move to top level.
	(struct ggc_pch_data): Update.
	* except.c (enum eh_region_type): Move to top level.
	(struct eh_region_d): Update.
	* regmove.c (enum match_use_kinds): Move to top level.
	(struct match): Update.
	* cgraphunit.c (enum cgraph_order_kinds): Move to top level.
	(struct cgraph_order_sort): Update.

2008-07-05  Tom Tromey  <tromey@redhat.com>

	* optabs.c (optab_table, convert_optab_table): Check
	HAVE_DESIGNATED_INITIALIZERS.
	(init_optabs): Likewise.
	* system.h (HAVE_DESIGNATED_INITIALIZERS): Define to 0 for C++.

2008-07-03  Tom Tromey  <tromey@redhat.com>

	* stringpool.c (alloc_node): Update.

2008-07-03  Tom Tromey  <tromey@redhat.com>

	* tree-eh.c (struct leh_state): Update.
	(struct leh_tf_state): Likewise.
	(lower_catch): Likewise.
	(lower_eh_filter): Likewise.
	(lower_cleanup): Likewise.
	(make_eh_edge): Likewise.
	(mark_eh_edge): Likewise.
	* function.h (call_site_record): Update.
	(ipa_opt_pass): Likewise.
	* alias.c (struct alias_set_entry_d): Rename from
	alias_set_entry.
	(record_alias_subset): Update.
	* except.c (struct eh_region_d): Rename from eh_region.
	(struct call_site_record_d): Rename from call_site_record.
	Update all users.
	* except.h (struct eh_region_d): Rename from eh_region.
	Update all users.
	* tree-predcom.c (struct dref_d): Rename from dref.
	Update all users.
	* tree-cfg.c (update_eh_label): Update.
	* passes.c (add_ipa_transform_pass): Update.
	(execute_ipa_summary_passes): Update.
	(execute_one_ipa_transform_pass): Update.
	(execute_ipa_summary_passes): Update.
	(execute_ipa_pass_list): Update.
	* optabs.c (optab_table): Update.
	(convert_optab_table): Likewise.
	(sign_expand_binop): Likewise.
	* optabs.h (struct optab_d): Rename from optab.
	(optab): Update.
	(struct convert_optab_d): Rename from convert_optab.
	(convert_optab): Update.
	(optab_table, convert_optab_table): Likewise.
	* tree-pass.h (enum opt_pass_type): Declare at top-level.
	(struct ipa_ops_pass_d): Rename from struct ipa_ops_pass.
	(pass_ipa_inline): Update.
	* omega.c (verify_omega_pb): Update.
	(omega_eliminate_redundant, omega_eliminate_red,
	parallel_splinter, omega_alloc_problem): Likewise.
	* omega.h (struct eqn_d): Rename from eqn.
	(struct omega_pb_d): Rename from omega_pb.
	(omega_alloc_eqns): Update.
	* ipa-inline.c (pass_ipa_inline): Update.

2008-07-01  Ian Lance Taylor  <iant@google.com>

	* machmode.h (GET_MODE_INNER): Cast to enum machine_mode.
	(GET_MODE_WIDER_MODE): Likewise.
	(GET_MODE_2XWIDER_MODE): Likewise.

	* builtins.c (expand_builtin_profile_func): Rename local variable
	this to this_func.
	(validate_arglist): Pass int rather than enum to va_arg.

2008-06-30  Ian Lance Taylor  <iant@google.com>

	* vec.h (DEF_VEC_FUNC_P) [iterate]: Add cast for constant 0.

2008-06-21  Tom Tromey  <tromey@redhat.com>

	* system.h (CONST_CAST2): Define for C++.

2008-06-19  Ian Lance Taylor  <iant@google.com>

	* tree.h (enum tree_code): Include all-tree.def, not tree.def.
	* tree.c (tree_code_type): New global array.
	(tree_code_length, tree_code_name): Likewise.
	* Makefile.in (TREE_H): Add all-tree.def, c-common.def, and
	$(lang_tree_files).
	(all-tree.def, s-alltree): New targets.
	(gencheck.h, s-gencheck): Remove.
	(tree.o): Depend upon all-tree.def.
	(build/gencheck.o): Remove gencheck.h dependency.
	(mostlyclean): Don't remove gencheck.h.
	* c-common.h (enum c_tree_code): Remove.
	* c-lang.c (tree_code_type): Remove.
	(tree_code_length, tree_code_name): Remove.
	* gencheck.c (tree_codes): Include all-tree.def, rather than
	tree.def, c-common.def, and gencheck.h.  Undefined DEFTREECODE
	after it is used.
	* tree-browser.c (tb_tree_codes): Include all-tree.def, rather
	than tree.def.
	* cp/cp-tree.h (enum cplus_tree_code): Remove.
	(operator_name_info): Size to LAST_AND_UNUSED_TREE_CODE.
	(assignment_operator_name_info): Likewise.
	* cp/cp-lang.c (tree_code_type): Remove.
	(tree_code_length, tree_code_name): Remove.
	* cp/lex.c (operator_name_info): Size to
	LAST_AND_UNUSED_TREE_CODE.
	(assignment_operator_name_info): Likewise.
	* cp/decl.c (grok_op_properties): Change LAST_CPLUS_TREE_CODE to
	LAST_AND_UNUSED_TREE_CODE.
	* cp/mangle.c (write_expression): Likewise.
	* cp/Make-lang.in (CXX_TREE_H): Remove cp/cp-tree.def.
	* fortran/f95-lang.c (tree_code_type): Remove.
	(tree_code_length, tree_code_name): Remove.
	* java/java-tree.h (enum java_tree_code): Remove.
	* java/lang.c (tree_code_type): Remove.
	(tree_code_length, tree_code_name): Remove.
	* java/Make-lang.in (JAVA_TREE_H): Remove java/java-tree.def.
	* objc/objc-act.h (enum objc_tree_code): Remove.
	* objc/objc-lang.c (tree_code_type): Remove.
	(tree_code_length, tree_code_name): Remove.
	* objcp/objcp-lang.c (tree_code_type): Remove.
	(tree_code_length, tree_code_name): Remove.
	* ada/ada-tre.h (enum gnat_tree_code): Remove.
	* ada/Make-lang.in (ADA_TREE_H): Remove ada/ada-tre.def.
	* ada/misc.c (tree_code_type): Remove.
	(tree_code_length, tree_code_name): Remove.

	* c-lex.c (narrowest_unsigned_type): Change itk to int.
	(narrowest_signed_type): Likewise.
	* c-typeck.c (c_common_type): Change local variable mclass to enum
	mode_class, twice.
	(parser_build_binary_op): Compare the TREE_CODE_CLASS with
	tcc_comparison, not the tree code itself.
	* c-common.c (def_fn_type): Pass int, not an enum, to va_arg.
	(c_expand_expr): Cast modifier to enum expand_modifier.
	* c-common.h (C_RID_CODE): Add casts.
	(C_SET_RID_CODE): Define.
	* c-parser.c (c_parse_init): Use C_SET_RID_CODE.
	(c_lex_one_token): Add cast to avoid warning.
	(c_parser_objc_type_name): Rename local typename to type_name.
	(check_no_duplicate_clause): Change code parameter to enum
	omp_clause_code.
	(c_parser_omp_var_list_parens): Change kind parameter to enum
	omp_clause_code.
	(c_parser_omp_flush): Pass OMP_CLAUSE_ERROR, not 0, to
	c_parser_omp_list_var_parens.
	(c_parser_omp_threadprivate): Likewise.
	* c-format.c (NO_FMT): Define.
	(printf_length_specs): Use NO_FMT.
	(asm_fprintf_length_specs): Likewise.
	(gcc_diag_length_specs): Likewise.
	(scanf_length_specs): Likewise.
	(strfmon_length_specs): Likewise.
	(gcc_gfc_length_specs): Likewise.
	(printf_flag_specs): Change 0 to STD_C89.
	(asm_fprintf_flag_specs): Likewise.
	(gcc_diag_flag_specs): Likewise.
	(gcc_cxxdiag_flag_specs): Likewise.
	(scanf_flag_specs): Likewise.
	(strftime_flag_specs): Likewise.
	(strfmon_flag_specs): Likewise.
	(print_char_table): Likewise.
	(asm_fprintf_char_table): Likewise.
	(gcc_diag_char_table): Likewise.
	(gcc_tdiag_char_table): Likewise.
	(gcc_cdiag_char_table): Likewise.
	(gcc_cxxdiag_char_table): Likewise.
	(gcc_gfc_char_table): Likewise.
	(scan_char_table): Likewise.
	(time_char_table): Likewis.
	(monetary_char_table): Likewise.
	* c-format.h (BADLEN): Likewise.

	* toplev.h (progname): Declare as extern "C" when compiling with
	C++.

2008-06-18  Ian Lance Taylor  <iant@google.com>

	* configure.ac: Split c_loose_warn out from loose_warn, and
	c_strict_warn from strict_warn.  Set and substitute
	warn_cxxflags.
	* Makefile.in (C_LOOSE_WARN, C_STRICT_WARN): New variables.
	(GCC_WARN_CFLAGS): Add $(C_LOOSE_WARN) and $(C_STRICT_WARN).
	(GCC_WARN_CXXFLAGS, WARN_CXXFLAGS): New variables.
	(GCC_CFLAGS): Add $(C_LOOSE_WARN).
	(ALL_CXXFLAGS): New variable.
	(.c.o): Compile with $(CXX) rather than $(CC).
	* configure: Rebuild.

Local Variables:
mode: change-log
change-log-default-name: "ChangeLog.cxx"
End:

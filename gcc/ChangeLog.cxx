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

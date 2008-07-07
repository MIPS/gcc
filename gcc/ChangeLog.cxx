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

/* Manipulation of formal and actual parameters of functions and function
   calls.
   Copyright (C) 2017-2019 Free Software Foundation, Inc.

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

#ifndef IPA_PARAM_MANIPULATION_H
#define IPA_PARAM_MANIPULATION_H

/* Indices into ipa_param_prefixes to identify a human-readable prefix for newly
   synthesized parameters.  Keep in sync with the array.  */
#define IPA_PARAM_PREFIX_SYNTH  0
#define IPA_PARAM_PREFIX_ISRA   1
#define IPA_PARAM_PREFIX_SIMD   2
#define IPA_PARAM_PREFIX_MASK   3

/* We do not support manipulating functions with more than
   1<<IPA_PARAM_MAX_INDEX_BITS parameters.  */
#define IPA_PARAM_MAX_INDEX_BITS 16

/* Operation to be performed for the parameter in ipa_parm_adjustment
   below.  */

enum ipa_parm_op {
  /* Do not use or you will trigger an assert.  */
  IPA_PARAM_OP_UNDEFINED,

  /* This new parameter is an unmodified parameter at index base_index. */
  IPA_PARAM_OP_COPY,

  /* This describes a brand new parameter.  If it somehow relates to any
     original parameters, the user needs to manage the transition itself.  */
  IPA_PARAM_OP_NEW,

    /* Split parameter as indicated by fields base_index, offset and type.  */
  IPA_PARAM_OP_SPLIT
};

/* Structure that describes one parameter of a function after transformation.
   Omitted parameters will be removed.  */

struct GTY(()) ipa_adjusted_param
{
  /* Type of the new parameter.  Required for all operations except
     IPA_PARM_OP_COPY when the original type will be preserved.  */
  tree type;

  /* Alias reference type to be used in MEM_REFs when adjusting caller
     arguments.  Required for IPA_PARM_OP_SPLIT operation.  */
  tree alias_ptr_type;

  /* Offset into the original parameter (for the cases when the new parameter
     is a component of an original one).  Required for IPA_PARM_OP_SPLIT
     operation.  */
  unsigned unit_offset;

  /* Zero based index of the original parameter this one is based on.  Required
     for IPA_PARAM_OP_COPY and IPA_PARAM_OP_SPLIT, users of IPA_PARAM_OP_NEW
     only need to specify it if they use replacement lookup provided by
     ipa_param_body_adjustments.  */
  unsigned base_index : IPA_PARAM_MAX_INDEX_BITS;

  /* Zero based index of the parameter this one is based on in the previous
     clone.  If there is no previous clone, it must be equal to base_index.  */
  unsigned prev_clone_index : IPA_PARAM_MAX_INDEX_BITS;

  /* Specify the operation, if any, to be performed on the parameter.  */
  enum ipa_parm_op op : 2;

  /* If set, this structure describes a parameter copied over from a previous
     IPA clone, any transformations are thus not to be re-done.  */
  unsigned prev_clone_adjustment : 1;

  /* Index into ipa_param_prefixes specifying a prefix to be used with
     DECL_NAMEs of newly synthesized parameters.  */
  unsigned param_prefix_index : 2;

  /* Storage order of the original parameter (for the cases when the new
     parameter is a component of an original one).  */
  unsigned reverse : 1;

  /* A bit free for the user.  */
  unsigned user_flag : 1;
};

void ipa_dump_adjusted_parameters (FILE *f,
				   vec<ipa_adjusted_param, va_gc> *adj_params);

/* Structure to remember the split performed on a node so that edge redirection
   (i.e. splitting arguments of call statements) know how split formal
   parameters of the caller are represented.  */

struct GTY(()) ipa_param_performed_split
{
  /* The dummy VAR_DECL that was created instead of the split parameter that
     sits in the call in the meantime between clone materialization and call
     redirection.  All entries in a vector of performed splits that correspond
     to the same dumy decl must be grouped together.  */
  tree dummy_decl;
  /* Offset into the original parameter.  */
  unsigned unit_offset;
};

/* Class used to record planned modifications to parameters of a function and
   also to perform necessary modifications at the caller side at the gimple
   level.  Used to describe all cgraph node clones that have their parameters
   changed, therefore the class should only have a small memory footprint.  */

class GTY(()) ipa_param_adjustments
{
public:
  /* Constructor from NEW_PARAMS showing how new parameters should look like
      plus copying any pre-existing actual arguments starting from argument
      with index ALWAYS_COPY_START (if non-negative, negative means do not copy
      anything beyond what is described in NEW_PARAMS), and SKIP_RETURN, which
      indicates that the function should return void after transformation.  */

  ipa_param_adjustments (vec<ipa_adjusted_param, va_gc> *new_params,
			 int always_copy_start, bool skip_return)
    : m_adj_params (new_params), m_always_copy_start (always_copy_start),
    m_skip_return (skip_return)
    {}

  gcall *modify_call (gcall *stmt,
		      vec<ipa_param_performed_split, va_gc> *performed_splits,
		      tree callee_decl, bool update_references);
  bool first_param_intact_p ();
  tree build_new_function_type (tree old_type, bool type_is_original_p);
  tree adjust_decl (tree orig_decl);
  void get_surviving_params (vec<bool> *surviving_params);
  void get_updated_indices (vec<int> *new_indices);

  void dump (FILE *f);
  void debug ();

  /* How the known part of arguments should look like.  */
  vec<ipa_adjusted_param, va_gc> *m_adj_params;

  /* If non-negative, copy any arguments starting at this offset without any
     modifications so that functions with variable number of arguments can be
     modified. This number should be equal to the number of original forma
     parameters.  */
  int m_always_copy_start;
  /* If true, make the function not return any value.  */
  bool m_skip_return;

private:
  ipa_param_adjustments () {}

  void init (vec<tree> *cur_params);
  int get_max_base_index ();
  bool method2func_p (tree orig_type);
};

/* Structure used to map expressions accessing split or replaced parameters to
   new PARM_DECLs.  */

struct ipa_param_body_replacement
{
  /* The old decl of the original parameter.   */
  tree base;
  /* The new decl it should be replaced with.  */
  tree repl;
  /* When modifying clones during IPA clone materialization, this is a dummy
     decl used to mark calls in which we need to apply transitive splitting,
     these dummy delcls are inserted as arguments to such calls and then
     followed by all the replacements with offset info stored in
     ipa_param_performed_split.

     Users of ipa_param_body_adjustments that modify standalone functions
     outside of IPA clone materialization can use this field for their internal
     purposes.  */
  tree dummy;
  /* The offset within BASE that REPL represents.  */
  unsigned unit_offset;
};

struct ipa_replace_map;

/* Class used when actually performing adjustments to formal parameters of a
   function to map accesses that need to be replaced to replacements.  The
   class attempts to work in two very different sets of circumstances: as a
   part of tree-inine.c's tree_function_versioning machinery to clone functions
   (when M_ID is not NULL) and in s standalone fashion, modifying an existing
   function in place (when M_ID is NULL).  While a lot of stuff handled in a
   unified way in both modes, there are many aspects of the processs that
   requires distinct paths.  */

class ipa_param_body_adjustments
{
public:
  ipa_param_body_adjustments (ipa_param_adjustments *adjustments,
			      tree fndecl, tree old_fndecl,
			      struct copy_body_data *id, tree *vars,
			      vec<ipa_replace_map *, va_gc> *tree_map);
  ipa_param_body_adjustments (ipa_param_adjustments *adjustments,
			      tree fndecl);
  ipa_param_body_adjustments (vec<ipa_adjusted_param, va_gc> *adj_params,
			      tree fndecl);

  bool perform_cfun_body_modifications ();

  void modify_formal_parameters ();
  void register_replacement (ipa_adjusted_param *apm, tree replacement,
			     tree dummy = NULL_TREE);
  tree lookup_replacement (tree base, unsigned unit_offset);
  ipa_param_body_replacement *get_expr_replacement (tree expr,
						    bool ignore_default_def);
  tree get_replacement_ssa_base (tree old_decl);
  bool modify_gimple_stmt (gimple **stmt, gimple_seq *extra_stmts);
  tree get_new_param_chain ();

  /* Pointers to data structures defining how the function should be
     modified.  */
  vec<ipa_adjusted_param, va_gc> *m_adj_params;
  ipa_param_adjustments *m_adjustments;

  /* Vector of old parameter declarations that must have their debug bind
     statements re-mapped and debug decls created.  */

  auto_vec<tree, 16> m_reset_debug_decls;

  /* Set to true if there are any IPA_PARAM_OP_SPLIT adjustments among stored
     adjustments.  */
  bool m_split_modifications_p;
private:
  void common_initialization (tree old_fndecl, tree *vars,
			      vec<ipa_replace_map *, va_gc> *tree_map);
  unsigned get_base_index (ipa_adjusted_param *apm);
  ipa_param_body_replacement *lookup_replacement_1 (tree base,
						    unsigned unit_offset);
  tree replace_removed_params_ssa_names (tree old_name, gimple *stmt);
  bool modify_expression (tree *expr_p, bool convert);
  bool modify_assignment (gimple *stmt, gimple_seq *extra_stmts);
  bool modify_call_stmt (gcall **stmt_p);
  bool modify_cfun_body ();
  void reset_debug_stmts ();


  /* Declaration of the function that is being transformed.  */

  tree m_fndecl;

  /* If non-NULL, the tree-inline master data structure guiding materialization
     of the current clone.  */
  struct copy_body_data *m_id;

  /* Vector of old parameter declarations (before changing them).  */

  auto_vec<tree, 16> m_oparms;

  /* Vector of parameter declarations the function will have after
     transformation.  */

  auto_vec<tree, 16> m_new_decls;

  /* If the function type has non-NULL TYPE_ARG_TYPES, this is the vector of
     these types after transformation, otherwise an empty one.  */

  auto_vec<tree, 16> m_new_types;

  /* Vector of structures telling how to replace old parameters in in the
     function body.  TODO: Even though there usually be only few, but should we
     use a hash?  */

  auto_vec<ipa_param_body_replacement, 16> m_replacements;

  /* Vector for remapping SSA_BASES from old parameter declarations that are
     being removed as a part of the transformation.  Before a new VAR_DECL is
     created, it holds the old PARM_DECL, once the variable is built it is
     stored here.  */

  auto_vec<tree> m_removed_decls;

  /* Hash to quickly lookup the item in m_removed_decls given the old decl.  */

  hash_map<tree, unsigned> m_removed_map;

  /* True iff the transformed function is a class method that is about to loose
     its this pointer and must be converted to a normal function.  */

  bool m_method2func;
};

void ipa_fill_vector_with_formal_parms (vec<tree> *args, tree fndecl);
void ipa_fill_vector_with_formal_parm_types (vec<tree> *types, tree fntype);

#endif	/* IPA_PARAM_MANIPULATION_H */

// Tree generation.

// Copyright (C) 2004, 2005 Free Software Foundation, Inc.
//
// This file is part of GCC.
//
// GCC is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// GCC is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GCC; see the file COPYING.  If not, write to the Free
// Software Foundation, 59 Temple Place - Suite 330, Boston, MA
// 02111-1307, USA.

#ifndef GCC_JAVA_TREE_HH
#define GCC_JAVA_TREE_HH

class tree_generator : public visitor
{
  typedef std::map< const model_stmt *,
		    std::pair<tree, tree>
                  > target_map_type;

  // The underlying compiler we're talking to.
  tree_builtins *gcc_builtins;

  // The wrapper for this method's declaring class.
  aot_class *class_wrapper;

  // This maps statements onto 'break' and 'continue' targets.  The
  // first element of the pair is the 'continue' target, the second
  // element is the 'break' target.
  target_map_type target_map;

  // Current result from walking sub-part of the model.
  tree current;

  // Tree that holds the 'this' expression.
  tree this_tree;

  // The method we are handling.
  model_method *method;

  // Tree representing the current method decl.
  tree method_tree;

  // The current block to which we're adding.
  tree current_block;

  // A counter for use when generating temporary names.
  int temp_counter;


  // When compiling bytecode, this holds an array of label decls.
  tree *labels;

  // The stack top.
  int stack_top;

  // Type-tracking for the stack.
  tree *stack_types;

  // Trees representing local variables and stack slots.
  tree *stack_slots;
  tree *local_slots;


  // This saves and restores a tree.
  class save_tree
  {
    tree &location;
    tree save;

  public:

    save_tree (tree &loc, tree new_value)
      : location (loc),
	save (loc)
    {
      location = new_value;
    }

    ~save_tree ()
    {
      location = save;
    }

    tree get ()
    {
      return save;
    }
  };

  void emit_type_assertion (model_type *, model_type *);
  tree transform_list (const std::list<ref_stmt> &);
  void annotate (tree, model_element *);
  tree wrap_synchronized (tree, tree);
  tree wrap_label (tree, model_element *);
  tree add_var (const ref_variable_decl &);
  tree add_temporary (tree, tree);
  tree build_class_ref (model_type *, model_element *);
  tree build_class_ref (const std::string &);
  tree handle_instanceof (tree, tree);
  void handle_inc_dec (model_element *, tree_code, const ref_expression &);
  void handle_op_assignment (model_element *, tree_code,
			     const ref_expression &, const ref_expression &);
  void handle_invocation (const model_method *, tree,
			  const std::list<ref_expression> &,
			  bool = false);
  tree arith_shift (model_element *, tree_code, const ref_expression &,
		    const ref_expression &, bool);
  void binary_operator (model_element *, tree_code, const ref_expression &,
			const ref_expression &, tree = NULL_TREE);
  tree build_jni_stub ();
  tree handle_string_literal (const std::string &);
  tree build_int (jint, tree = NULL_TREE);
  tree build_long (jlong);
  tree handle_float (jfloat);
  tree handle_double (jdouble);
  tree build_ref_from_constant_pool (tree, int);
  tree build_new_array (model_type *, tree, model_element *);
  tree build_new_object_array (model_type *, tree);
  model_type *find_model_class (const std::string &);
  tree build_divide (tree, tree, tree);
  tree build_mod (tree, tree, tree);
  tree build_array_reference (tree, tree, tree, bool = true);
  tree build_exception_object_ref (tree);
  tree build_label ();
  tree build_arguments (const std::list<ref_expression> &, tree &);

  void stringbuffer_append (model_expression *, tree &, model_class *,
			    tree = NULL_TREE);
  void handle_string_plus (model_plus *, const ref_expression &,
			   const ref_expression &, tree &, model_class *);
  model_method *find_method (const char *, model_class *,
			     model_type *, model_type *,
			     model_element *);
  tree create_stringbuffer (model_class **, model_element *);
  tree finish_stringbuffer (model_class *, tree, model_element *);

  // This class also includes code to transform bytecode to trees.
  // This is kept in a separate file, lower.cc, because it makes the
  // code easier to follow.  However, it is convenient for other
  // reasons to have that code be part of this class.  So, we declare
  // the methods here.
  tree int_condition (tree_code, const uint8 *, int &);
  tree full_condition (tree_code, const uint8 *, int &);
  tree full_condition (tree_code, tree, tree, const uint8 *, int &);
  tree binop (tree_code, tree);
  tree compare (tree type, tree_code gt_opt = GT_EXPR);
  tree handle_return (tree, tree = NULL_TREE);
  tree cast (tree, tree);
  tree handle_convert (model_type *, tree);
  tree find_label (int);
  tree push (tree);
  tree pop (tree);
  tree find_slot (int, tree);
  tree load (int, tree);
  tree store (int, tree);
  tree find_generic_slot (int, tree, tree *);
  tree array_store (model_type *, tree);
  tree array_access (model_type *, tree);
  tree find_class (const std::string &);
  tree handle_ldc (constant_pool *, uint16);
  bool type_wide_p (tree) const;
  model_field *find_field (const std::string &name,
			   model_class *klass, model_type *type,
			   model_element *request);

public:

  tree_generator (tree_builtins *, aot_class *);

  virtual ~tree_generator ();

  tree generate (model_method *);

  void visit_method (model_method *,
		     const std::list<ref_variable_decl> &,
		     const ref_block &);

  void visit_assert (model_assert *, const ref_expression &,
		     const ref_expression &);

  void visit_block (model_block *,
		    const std::list<ref_stmt> &);

  // Note that the implementation of this function is in its own file,
  // lower.cc.
  void visit_bytecode_block (model_bytecode_block *, int, int, int,
			     const uint8 *);

  void visit_break (model_break *, const ref_stmt &);

  void visit_catch (model_catch *, const ref_variable_decl &,
		    const ref_block &);

  void visit_continue (model_continue *, const ref_stmt &);

  void visit_class_decl_stmt (model_class_decl_stmt *,
			      const ref_class &);

  void visit_do (model_do *, const ref_expression &,
		 const ref_stmt &);

  void visit_empty (model_empty *);

  void visit_expression_stmt (model_expression_stmt *,
			      const ref_expression &);

  void visit_for_enhanced (model_for_enhanced *,
			   const ref_stmt &, const ref_expression &,
			   const ref_variable_decl &);

  void visit_for (model_for *, const ref_stmt &,
		  const ref_expression &, const ref_stmt &,
		  const ref_stmt &);

  void visit_if (model_if *, const ref_expression &,
		 const ref_stmt &, const ref_stmt &);

  void visit_label (model_label *, const ref_stmt &);

  void visit_return (model_return *, const ref_expression &);

  void visit_switch (model_switch *,
		     const ref_expression &,
		     const std::list<ref_switch_block> &);

  void visit_switch_block (model_switch_block *,
			   const std::list<ref_stmt> &);

  void visit_synchronized (model_synchronized *,
			   const ref_expression &,
			   const ref_stmt &);

  void visit_throw (model_throw *, const ref_expression &);

  void visit_try (model_try *, const ref_block &,
		  const std::list<ref_catch> &, const ref_block &);

  void visit_variable_stmt (model_variable_stmt *,
			    const std::list<ref_variable_decl> &);

  void visit_while (model_while *, const ref_expression &,
		    const ref_stmt &);

  void visit_array_initializer (model_array_initializer *,
				const ref_forwarding_type &,
				const std::list<ref_expression> &);

  void visit_array_ref (model_array_ref *,
			const ref_expression &,
			const ref_expression &);

  void visit_assignment (model_assignment *,
			 const ref_expression &,
			 const ref_expression &);

  void visit_op_assignment (model_minus_equal *,
			    const ref_expression &,
			    const ref_expression &);

  void visit_op_assignment (model_mult_equal *,
			    const ref_expression &,
			    const ref_expression &);

  void visit_op_assignment (model_div_equal *,
			    const ref_expression &,
			    const ref_expression &);

  void visit_op_assignment (model_and_equal *,
			    const ref_expression &,
			    const ref_expression &);

  void visit_op_assignment (model_or_equal *,
			    const ref_expression &,
			    const ref_expression &);

  void visit_op_assignment (model_plus_equal *,
			    const ref_expression &,
			    const ref_expression &);

  void visit_op_assignment (model_xor_equal *,
			    const ref_expression &,
			    const ref_expression &);

  void visit_op_assignment (model_mod_equal *,
			    const ref_expression &,
			    const ref_expression &);

  void visit_op_assignment (model_ls_equal *,
			    const ref_expression &,
			    const ref_expression &);

  void visit_op_assignment (model_rs_equal *,
			    const ref_expression &,
			    const ref_expression &);

  void visit_op_assignment (model_urs_equal *,
			    const ref_expression &,
			    const ref_expression &);

  void visit_arith_binary (model_minus *,
			   const ref_expression &,
			   const ref_expression &);

  void visit_arith_binary (model_mult *,
			   const ref_expression &,
			   const ref_expression &);

  void visit_arith_binary (model_div *,
			   const ref_expression &,
			   const ref_expression &);

  void visit_arith_binary (model_mod *,
			   const ref_expression &,
			   const ref_expression &);

  void visit_arith_binary (model_and *,
			   const ref_expression &,
			   const ref_expression &);

  void visit_arith_binary (model_or *,
			   const ref_expression &,
			   const ref_expression &);

  void visit_arith_binary (model_xor *,
			   const ref_expression &,
			   const ref_expression &);

  void visit_arith_binary (model_plus *,
			   const ref_expression &,
			   const ref_expression &);

  void visit_arith_shift (model_left_shift *,
			  const ref_expression &,
			  const ref_expression &);

  void visit_arith_shift (model_right_shift *,
			  const ref_expression &,
			  const ref_expression &);

  void visit_arith_shift (model_unsigned_right_shift *,
			  const ref_expression &,
			  const ref_expression &);

  void visit_cast (model_cast *, const ref_forwarding_type &,
		   const ref_expression &);

  void visit_class_ref (model_class_ref *,
			const ref_forwarding_type &);

  void visit_comparison (model_equal *,
			 const ref_expression &,
			 const ref_expression &);

  void visit_comparison (model_notequal *,
			 const ref_expression &,
			 const ref_expression &);

  void visit_comparison (model_lessthan *,
			 const ref_expression &,
			 const ref_expression &);

  void visit_comparison (model_greaterthan *,
			 const ref_expression &,
			 const ref_expression &);

  void visit_comparison (model_lessthanequal *,
			 const ref_expression &,
			 const ref_expression &);

  void visit_comparison (model_greaterthanequal *,
			 const ref_expression &,
			 const ref_expression &);

  void visit_conditional (model_conditional *,
			  const ref_expression &,
			  const ref_expression &,
			  const ref_expression &);

  void visit_field_ref (model_field_ref *,
			const ref_expression &,
			model_field *);

  void visit_field_initializer (model_field_initializer *,
				model_field *);

  void visit_field (model_field *)
  {
    // Nothing.
  }

  void visit_instanceof (model_instanceof *,
			 const ref_expression &,
			 const ref_forwarding_type &);

  void visit_logical_binary (model_lor *,
			     const ref_expression &,
			     const ref_expression &);

  void visit_logical_binary (model_land *,
			     const ref_expression &,
			     const ref_expression &);

  void visit_simple_literal (model_literal_base *,
			     const jboolean &val);

  void visit_simple_literal (model_literal_base *,
			     const jbyte &val);

  void visit_simple_literal (model_literal_base *,
			     const jchar &val);

  void visit_simple_literal (model_literal_base *,
			     const jshort &val);

  void visit_simple_literal (model_literal_base *,
			     const jint &val);

  void visit_simple_literal (model_literal_base *,
			     const jlong &val);

  void visit_simple_literal (model_literal_base *,
			     const jfloat &val);

  void visit_simple_literal (model_literal_base *,
			     const jdouble &val);

  void visit_string_literal (model_string_literal *,
			     const std::string &val);

  void visit_method_invocation (model_method_invocation *,
				model_method *,
				const ref_expression &,
				const std::list<ref_expression> &);

  void visit_type_qualified_invocation (model_type_qualified_invocation *,
					const model_method *,
					const std::list<ref_expression> &,
					bool);

  void visit_super_invocation (model_super_invocation *,
			       const model_method *,
			       const std::list<ref_expression> &,
			       const ref_expression &);

  void visit_this_invocation (model_this_invocation *,
			      const model_method *,
			      const std::list<ref_expression> &);

  void visit_new (model_new *, const model_method *,
		  const ref_forwarding_type &,
		  const std::list<ref_expression> &);

  void visit_new_array (model_new_array *,
			const ref_forwarding_type &,
			const std::list<ref_expression> &,
			const ref_expression &);

  void visit_null_literal (model_null_literal *);

  void visit_prefix_simple (model_prefix_plus *,
			    const ref_expression &);

  void visit_prefix_simple (model_prefix_minus *,
			    const ref_expression &);

  void visit_prefix_simple (model_bitwise_not *,
			    const ref_expression &);

  void visit_prefix_simple (model_logical_not *,
			    const ref_expression &);

  void visit_prefix_side_effect (model_prefix_plusplus *,
				 const ref_expression &);

  void visit_prefix_side_effect (model_prefix_minusminus *,
				 const ref_expression &);

  void visit_postfix_side_effect (model_postfix_plusplus *,
				  const ref_expression &);

  void visit_postfix_side_effect (model_postfix_minusminus *,
				  const ref_expression &);

  void visit_this (model_this *);

  void visit_simple_variable_ref (model_simple_variable_ref *,
				  const model_variable_decl *);

  void visit_forwarding_type (model_forwarding_type *,
                              model_type *)
  {
    // Nothing.
  }

  void visit_forwarding_resolved (model_forwarding_resolved *,
                                  model_type *)
  {
    // Nothing.
  }

  void visit_forwarding_owned (model_forwarding_owned *,
                               const ref_type &)
  {
    // Nothing.
  }

  void visit_forwarding_simple (model_forwarding_simple *,
                                const std::list<std::string> &)
  {
    // Nothing.
  }

  void
  visit_forwarding_array (model_forwarding_array *,
                          const owner<model_forwarding_type> &)
  {
    // Nothing.
  }

  void
  visit_forwarding_element (model_forwarding_element *,
                            const owner<model_forwarding_type> &)
  {
    // Nothing.
  }

  void visit_forwarding_full (model_forwarding_full *,
                              const std::string &)
  {
    // Nothing.
  }

  void
  visit_forwarding_inner (model_forwarding_inner *,
                          const std::list<std::string> &,
                          const owner<model_forwarding_type> &)
  {
    // Nothing.
  }

  void
  visit_forwarding_parameterized (model_forwarding_parameterized *,
                                  const owner<model_forwarding_type> &,
                                  const std::list< owner<model_forwarding_type> > &)
  {
    // Nothing.
  }

  void visit_variable_decl (model_variable_decl *,
                            const std::string &,
                            const ref_forwarding_type &,
                            const ref_expression &,
                            bool,
                            bool)
  {
    // Nothing.
  }

  void visit_parameter_decl (model_variable_decl *,
                             const std::string &,
                             const ref_forwarding_type &,
                             const ref_expression &,
                             bool,
                             bool)
  {
    // Nothing.
  }

  void visit_catch_decl (model_variable_decl *,
                         const std::string &,
                         const ref_forwarding_type &,
                         const ref_expression &,
                         bool,
                         bool)
  {
    // Nothing.
  }

  void visit_package (model_package *, const std::list<std::string> &)
  {
    // Nothing.
  }

  void visit_primitive (model_primitive_base *, const char *)
  {
    // Nothing.
  }

  void visit_type (model_type *, const std::string &)
  {
    // Nothing.
  }

  void visit_identifier (model_identifier *, const std::string &)
  {
    // Nothing.
  }
  
  void visit_annotation_member (model_annotation_member *,
                                const ref_forwarding_type &)
  {
    // Nothing.
  }

  void visit_annotation_value (model_annotation_value *,
                               const std::string &, const ref_expression &)
  {
    // Nothing.
  }

  void visit_import_single (model_import_single *,
                            const std::list<std::string> &, model_class *)
  {
    // Nothing.
  }

  void visit_import_on_demand (model_import_on_demand *,
                               const std::list<std::string> &, Iname *, bool)
  {
    // Nothing.
  }

  void visit_static_import_single (model_static_import_single *,
				   const std::list<std::string> &,
				   model_class *, const std::string &)
  {
    // Nothing.
  }

  void visit_static_import_on_demand (model_static_import_on_demand *,
				      const std::list<std::string> &,
				      model_class *)
  {
    // Nothing.
  }
  
  void visit_unit_source (model_unit_source *, model_package *,
			  const std::list<ref_class> &,
			  const std::string &,
			  bool, const std::list<ref_import> &)
  {
    // Nothing.
  }
  
  void visit_unit_class (model_unit_class *, model_package *,
			 const std::list<ref_class> &,
			 const std::string &, bool)
  {
    // Nothing.
  }
  
  void visit_unit_fake (model_unit_fake *, model_package *,
			const std::list<ref_class> &,
			const std::string &, bool)
  {
    // Nothing.
  }
};

#endif // GCC_JAVA_TREE_HH

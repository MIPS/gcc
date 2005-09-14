// Bytecode generation.

// Copyright (C) 2004, 2005 Free Software Foundation, Inc.
//
// This file is part of GCC.
//
// gcjx is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// gcjx is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with gcjx; see the file COPYING.LIB.  If
// not, write to the Free Software Foundation, Inc.,
// 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

#ifndef GCJX_BYTECODE_GENERATE_HH
#define GCJX_BYTECODE_GENERATE_HH

// This generates bytecode for a single method.
class bytecode_generator : public visitor
{
  // Ways to handle expression results.
  enum expression_result
  {
    IGNORE,
    ON_STACK,
    LEFT_HAND_SIDE,
    CONDITIONAL,
    STRING
  };

  // Different kinds of left-hand-sides for expressions.
  enum left_hand_side_type
  {
    ARRAY,
    STATIC_FIELD,
    INSTANCE_FIELD,
    VARIABLE,
    // This is used when we need a trampoline method.
    METHOD_CALL
  };

  // This is used to represent exception handlers.
  struct handler
  {
    // Start of region covered by exception handler.
    bytecode_block *start;
    // End of covered region.
    bytecode_block *end;
    // Where the exception is delivered.
    bytecode_block *target;
    // The exception's type, or NULL for a 'finally' clause.
    model_type *type;
  };

  // Information pertaining to a 'finally' clause.
  struct finally_handler
  {
    // The 'try' or 'synchronized' statement.
    const model_stmt *statement;
    // Corresponding block.  If this is NULL and variable != -1, this
    // finally handler represents the finally part of a 'synchronized'
    // block.  This can be NULL for an ordinary try statement if it
    // has no finally clause; in this case variable will be -1.
    // If BLOCK is NULL and VARIABLE is -2, then this is a dummy
    // handler representing some labeled block in the tree.
    model_block *block;
    // Variable used by the 'try'.
    int variable;

    finally_handler (const model_stmt *s, model_block *b, int v = -1)
      : statement (s),
	block (b),
	variable (v)
    {
    }

    finally_handler (const model_stmt *s)
      : statement (s),
	block (NULL),
	variable (-2)
    {
    }
  };

  // Our method.
  model_method *method;

  // Constant pool we're generating.
  outgoing_constant_pool *cpool;

  typedef std::map< const model_stmt *,
		    std::pair<bytecode_block *, bytecode_block *>
                  > target_map_type;

  // This maps statements onto 'break' and 'continue' targets.  The
  // first element of the pair is the 'continue' target, the second
  // element is the 'break' target.
  target_map_type target_map;

  // Local variables.
  locals vars;

  // The index of the variable representing "this".
  int this_index;

  // The very first block.  We must keep an explicit reference on this
  // to avoid collecting it.
  bytecode_block *first_block;

  // The current block.
  bytecode_block *current_block;

  // The current "true" and "false" branch targets.  These are used
  // when generating code for a boolean expression.
  bytecode_block *current_true_target;
  bytecode_block *current_false_target;

  // How the expression we're generating should be handled.
  expression_result expr_target;

  // This type is a convenience to save some typing.
  typedef std::deque<finally_handler> fstackt;

  // The stack of pending "finally" clauses.
  fstackt finally_stack;

  // Information for keeping track of the stack.
  int stack_depth;
  int max_stack;

  // Length of the bytecode we're generating.
  int bytecode_length;

  // A structure of this type is used to hold information about the
  // left hand side of an assignment expression.
  struct left_hand_side_info
  {
    // The type of this left-hand-side.
    left_hand_side_type kind;
    // If this assignment refers to some index, this is that value.
    // For instance it could be a constant pool index for a field.
    int index;
    model_type *type;
    model_field *field;
  };

  // Used when processing the left hand side of an assignment.
  left_hand_side_info lhs_info;

  // When processing String '+', this is used to pass type information
  // about nested '+' expressions.
  model_type *string_plus_type;

  // All the bytecode blocks we have allocated.
  std::list<bytecode_block *> allocated_blocks;

  // All the exception handlers.
  std::list<handler> handlers;

  // This holds the number of line number notes we are going to emit.
  int line_count;

  // Attributes attached to the code.
  bytecode_attribute_list attributes;

  /// This class is used to handle the LineNumberTable attribute.
  class line_table_attribute : public bytecode_attribute
  {
    bytecode_generator *gen;

  public:

    line_table_attribute (outgoing_constant_pool *p, bytecode_generator *g)
      : bytecode_attribute (p, "LineNumberTable"),
	gen (g)
    {
    }

    void emit (bytecode_stream &writer)
    {
      bytecode_attribute::emit (writer);
      gen->write_line_table (&writer);
    }

    int size ()
    {
      return 2 + 4 * gen->line_count;
    }
  };

  /// This class is used to handle the LocalVariableTable attribute.
  class local_variable_table_attribute : public bytecode_attribute
  {
    locals *vars;

    // True if we're emitting the type table, not the ordinary table.
    bool use_types;

    // Number of bytes we're going to emit.
    int bytes;

  public:

    local_variable_table_attribute (outgoing_constant_pool *p, locals *v,
				    bool types)
      : bytecode_attribute (p, (types ? "LocalVariableTypeTable"
				: "LocalVariableTable")),
	vars (v),
	use_types (types)
    {
      bytes = vars->emit (pool, NULL, use_types);
      // We shouldn't make an empty table.
      assert (bytes > 0);
    }

    void emit (bytecode_stream &writer)
    {
      bytecode_attribute::emit (writer);
      vars->emit (pool, &writer, use_types);
    }

    int size ()
    {
      return bytes;
    }
  };

  void write_line_table (bytecode_stream *);

  int adjust_for_type (model_type *) const;
  int adjust_for_type_full (model_type *) const;

  void increase_stack (model_type *);
  void reduce_stack (model_type *);

  void emit_saved_cleanup (const finally_handler &);
  void call_cleanups (const model_stmt *);

  void emit (jbyte);
  void emit2 (int);
  void emit4 (int);
  void emit_new_array (model_type *);
  void add_exception_handler (model_type *, bytecode_block *,
			      bytecode_block *,
			      bytecode_block *);
  void emit_relocation (relocation_kind, bytecode_block *);
  void non_normal_completion (bool);
  void emit_load (model_type *, int);
  void emit_store (model_type *, int);
  void emit_ldc (int);
  void emit_ldc2 (int);
  void emit_pop (model_type *);
  void define (bytecode_block *);
  model_field *find_field (const std::string &, model_class *, model_type *,
                           model_element *);
  model_method *find_method (const char *, model_class *, model_type *,
			     model_type *, model_element *);

  void full_comparison (relocation_kind,
			const ref_expression &,
			const ref_expression &,
			bytecode_block *,
			bytecode_block *);
  void handle_invocation (java_opcode,
			  model_class *,
			  const model_method *,
			  const std::list<ref_expression> &,
			  bool = false, bool = false);
  void handle_logical_binary (const ref_expression &,
			      const ref_expression &,
			      bool);
  void emit_null_pointer_check (model_expression *);

  void handle_block_statements (const std::list<ref_stmt> &);

  void binary_arith_operator (java_opcode,
			      const ref_expression &,
			      const ref_expression &);

  void arith_shift (java_opcode,
		    const ref_expression &,
		    const ref_expression &);

  void emit_cast (model_type *, model_type *);
  void emit_cast_maybe_boxing (model_element *, model_type *, model_type *);

  void handle_comparison (java_opcode,
			  const ref_expression &,
			  const ref_expression &);
  void handle_inc_dec (const ref_expression &, bool, int);

  void handle_op_assignment (java_opcode,
			     const ref_expression &,
			     const ref_expression &);
  void dereference_left_hand_side (left_hand_side_info *);
  void duplicate_lhs_value (left_hand_side_info *, model_type *);
  void emit_lhs_store (left_hand_side_info *, model_type *);

  model_class *get_stringbuffer_class ();
  void create_stringbuffer (model_type *, model_element *);
  void close_stringbuffer (model_type *, model_element *);

  void handle_valueof (model_type *, model_element *);

  // Create a new bytecode block.
  bytecode_block *new_bytecode_block ();
  void note_line (model_element *);

  int count_exception_handlers ();

  // Used to push a new expression target.
  class push_expr_target
  {
    expression_result save;
    expression_result &loc;

  public:
    push_expr_target (bytecode_generator *gen,
		      expression_result n_et)
      : save (gen->expr_target),
	loc (gen->expr_target)
    {
      gen->expr_target = n_et;
    }

    ~push_expr_target ()
    {
      loc = save;
    }
  };

  // Push branch targets.
  class branch_targets : public push_expr_target
  {
    bytecode_block *&true_target;
    bytecode_block *&false_target;

    bytecode_block *true_save;
    bytecode_block *false_save;

  public:

    branch_targets (bytecode_generator *gen,
		    bytecode_block *tr,
		    bytecode_block *fr)
      : push_expr_target (gen, CONDITIONAL),
	true_target (gen->current_true_target),
	false_target (gen->current_false_target),
	true_save (gen->current_true_target),
	false_save (gen->current_false_target)
    {
      true_target = tr;
      false_target = fr;
    }

    ~branch_targets ()
    {
      true_target = true_save;
      false_target = false_save;
    }
  };

  // For access to new_bytecode_block.
  friend class bytecode_block;

public:

  bytecode_generator (model_method *, outgoing_constant_pool *);

  virtual ~bytecode_generator ();

  /// The primary interface to this class.  This generates all code
  /// for the method.
  void generate ();

  /// Write the bytecode for this method.
  void write (bytecode_stream *);

  /// Compute the overall length used by the Code attribute.
  int bytecode_size ()
  {
    int count = count_exception_handlers ();
    return 8 + bytecode_length + 2 + 8 * count + attributes.size ();
  }

  /// Used to keep track of lifetimes of local variables.
  bytecode_block *get_current () const
  {
    return current_block;
  }

  /// Return the number of bytes in the bytecode.
  int get_byte_count () const
  {
    return bytecode_length;
  }


  void visit_method (model_method *,
		     const std::list<ref_variable_decl> &,
		     const ref_block &);

  void visit_assert (model_assert *, const ref_expression &,
		     const ref_expression &);

  void visit_block (model_block *,
		    const std::list<ref_stmt> &);

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

  void visit_method_invocation (model_method_invocation *,
				const model_method *,
				const ref_expression &,
				const std::list<ref_expression> &);

  void visit_type_qualified_invocation (model_type_qualified_invocation *,
					const model_method *,
					const std::list<ref_expression> &,
					bool);

  void visit_super_invocation (model_super_invocation *,
			       const model_method *,
			       const std::list<ref_expression> &);

  void visit_this_invocation (model_this_invocation *,
			      const model_method *,
			      const std::list<ref_expression> &);

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

  void visit_op_assignment (model_plus_equal *,
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
			const model_field *);

  void visit_field_initializer (model_field_initializer *,
				model_field *);

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
};

#endif // GCJX_BYTECODE_GENERATE_HH

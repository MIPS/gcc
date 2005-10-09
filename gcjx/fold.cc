// Constant folding.

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

#include "typedefs.hh"

class fold_visitor : public visitor
{
public:

  void visit_method (model_method *,
		     const std::list<ref_variable_decl> &,
		     const ref_block &)
  {
    abort ();
  }

  void visit_assert (model_assert *, const ref_expression &,
		     const ref_expression &)
  {
    abort ();
  }

  void visit_block (model_block *,
		    const std::list<ref_stmt> &)
  {
    abort ();
  }

  void visit_bytecode_block (model_bytecode_block *, int, int, int,
			     const uint8 *)
  {
    abort ();
  }

  void visit_break (model_break *, const ref_stmt &)
  {
    abort ();
  }

  void visit_catch (model_catch *, const ref_variable_decl &,
		    const ref_block &)
  {
    abort ();
  }

  void visit_continue (model_continue *, const ref_stmt &)
  {
    abort ();
  }

  void visit_class_decl_stmt (model_class_decl_stmt *,
			      const ref_class &)
  {
    abort ();
  }

  void visit_do (model_do *, const ref_expression &,
		 const ref_stmt &)
  {
    abort ();
  }

  void visit_empty (model_empty *)
  {
    abort ();
  }

  void visit_expression_stmt (model_expression_stmt *,
			      const ref_expression &)
  {
    abort ();
  }

  void visit_for_enhanced (model_for_enhanced *,
			   const ref_stmt &, const ref_expression &,
			   const ref_variable_decl &)
  {
    abort ();
  }

  void visit_for (model_for *, const ref_stmt &,
		  const ref_expression &, const ref_stmt &,
		  const ref_stmt &)
  {
    abort ();
  }

  void visit_if (model_if *, const ref_expression &,
		 const ref_stmt &, const ref_stmt &)
  {
    abort ();
  }

  void visit_label (model_label *, const ref_stmt &)
  {
    abort ();
  }

  void visit_return (model_return *, const ref_expression &)
  {
    abort ();
  }

  void visit_switch (model_switch *,
		     const ref_expression &,
		     const std::list<ref_switch_block> &)
  {
    abort ();
  }

  void visit_switch_block (model_switch_block *,
			   const std::list<ref_stmt> &)
  {
    abort ();
  }

  void visit_synchronized (model_synchronized *,
			   const ref_expression &,
			   const ref_stmt &)
  {
    abort ();
  }

  void visit_throw (model_throw *, const ref_expression &)
  {
    abort ();
  }

  void visit_try (model_try *, const ref_block &,
		  const std::list<ref_catch> &, const ref_block &)
  {
    abort ();
  }

  void visit_variable_stmt (model_variable_stmt *,
			    const std::list<ref_variable_decl> &)
  {
    abort ();
  }

  void visit_while (model_while *, const ref_expression &,
		    const ref_stmt &)
  {
    abort ();
  }

  void visit_array_initializer (model_array_initializer *init,
				const ref_forwarding_type &,
				const std::list<ref_expression> &exprs)
  {
    init->set_initializers (fold (exprs));
  }

  void visit_array_ref (model_array_ref *arrayref,
			const ref_expression &array,
			const ref_expression &index)
  {
    array->visit (this);
    arrayref->set_index (fold (index));
  }

  void visit_assignment (model_assignment *assignment,
			 const ref_expression &lhs,
			 const ref_expression &rhs)
  {
    lhs->visit (this);
    assignment->set_rhs (fold (rhs));
  }

  void visit_op_assignment (model_minus_equal *assignment,
			    const ref_expression &lhs,
			    const ref_expression &rhs)
  {
    lhs->visit (this);
    assignment->set_rhs (fold (rhs));
  }

  void visit_op_assignment (model_mult_equal *assignment,
			    const ref_expression &lhs,
			    const ref_expression &rhs)
  {
    lhs->visit (this);
    assignment->set_rhs (fold (rhs));
  }

  void visit_op_assignment (model_div_equal *assignment,
			    const ref_expression &lhs,
			    const ref_expression &rhs)
  {
    lhs->visit (this);
    assignment->set_rhs (fold (rhs));
  }

  void visit_op_assignment (model_and_equal *assignment,
			    const ref_expression &lhs,
			    const ref_expression &rhs)
  {
    lhs->visit (this);
    assignment->set_rhs (fold (rhs));
  }

  void visit_op_assignment (model_or_equal *assignment,
			    const ref_expression &lhs,
			    const ref_expression &rhs)
  {
    lhs->visit (this);
    assignment->set_rhs (fold (rhs));
  }

  void visit_op_assignment (model_plus_equal *assignment,
			    const ref_expression &lhs,
			    const ref_expression &rhs)
  {
    lhs->visit (this);
    assignment->set_rhs (fold (rhs));
  }

  void visit_op_assignment (model_xor_equal *assignment,
			    const ref_expression &lhs,
			    const ref_expression &rhs)
  {
    lhs->visit (this);
    assignment->set_rhs (fold (rhs));
  }

  void visit_op_assignment (model_mod_equal *assignment,
			    const ref_expression &lhs,
			    const ref_expression &rhs)
  {
    lhs->visit (this);
    assignment->set_rhs (fold (rhs));
  }

  void visit_op_assignment (model_ls_equal *assignment,
			    const ref_expression &lhs,
			    const ref_expression &rhs)
  {
    lhs->visit (this);
    assignment->set_rhs (fold (rhs));
  }

  void visit_op_assignment (model_rs_equal *assignment,
			    const ref_expression &lhs,
			    const ref_expression &rhs)
  {
    lhs->visit (this);
    assignment->set_rhs (fold (rhs));
  }

  void visit_op_assignment (model_urs_equal *assignment,
			    const ref_expression &lhs,
			    const ref_expression &rhs)
  {
    lhs->visit (this);
    assignment->set_rhs (fold (rhs));
  }

  void visit_arith_binary (model_minus *op,
			   const ref_expression &lhs,
			   const ref_expression &rhs)
  {
    op->set_lhs (fold (lhs));
    op->set_rhs (fold (rhs));
  }

  void visit_arith_binary (model_mult *op,
			   const ref_expression &lhs,
			   const ref_expression &rhs)
  {
    op->set_lhs (fold (lhs));
    op->set_rhs (fold (rhs));
  }

  void visit_arith_binary (model_div *op,
			   const ref_expression &lhs,
			   const ref_expression &rhs)
  {
    op->set_lhs (fold (lhs));
    op->set_rhs (fold (rhs));
  }

  void visit_arith_binary (model_mod *op,
			   const ref_expression &lhs,
			   const ref_expression &rhs)
  {
    op->set_lhs (fold (lhs));
    op->set_rhs (fold (rhs));
  }

  void visit_arith_binary (model_and *op,
			   const ref_expression &lhs,
			   const ref_expression &rhs)
  {
    op->set_lhs (fold (lhs));
    op->set_rhs (fold (rhs));
  }

  void visit_arith_binary (model_or *op,
			   const ref_expression &lhs,
			   const ref_expression &rhs)
  {
    op->set_lhs (fold (lhs));
    op->set_rhs (fold (rhs));
  }

  void visit_arith_binary (model_xor *op,
			   const ref_expression &lhs,
			   const ref_expression &rhs)
  {
    op->set_lhs (fold (lhs));
    op->set_rhs (fold (rhs));
  }

  void visit_arith_binary (model_plus *op,
			   const ref_expression &lhs,
			   const ref_expression &rhs)
  {
    op->set_lhs (fold (lhs));
    op->set_rhs (fold (rhs));
  }

  void visit_arith_shift (model_left_shift *op,
			  const ref_expression &lhs,
			  const ref_expression &rhs)
  {
    op->set_lhs (fold (lhs));
    op->set_rhs (fold (rhs));
  }

  void visit_arith_shift (model_right_shift *op,
			  const ref_expression &lhs,
			  const ref_expression &rhs)
  {
    op->set_lhs (fold (lhs));
    op->set_rhs (fold (rhs));
  }

  void visit_arith_shift (model_unsigned_right_shift *op,
			  const ref_expression &lhs,
			  const ref_expression &rhs)
  {
    op->set_lhs (fold (lhs));
    op->set_rhs (fold (rhs));
  }

  void visit_cast (model_cast *, const ref_forwarding_type &,
		   const ref_expression &expr)
  {
    expr->visit (this);
  }

  void visit_class_ref (model_class_ref *,
			const ref_forwarding_type &)
  {
  }

  void visit_comparison (model_equal *op,
			 const ref_expression &lhs,
			 const ref_expression &rhs)
  {
    op->set_lhs (fold (lhs));
    op->set_rhs (fold (rhs));
  }

  void visit_comparison (model_notequal *op,
			 const ref_expression &lhs,
			 const ref_expression &rhs)
  {
    op->set_lhs (fold (lhs));
    op->set_rhs (fold (rhs));
  }

  void visit_comparison (model_lessthan *op,
			 const ref_expression &lhs,
			 const ref_expression &rhs)
  {
    op->set_lhs (fold (lhs));
    op->set_rhs (fold (rhs));
  }

  void visit_comparison (model_greaterthan *op,
			 const ref_expression &lhs,
			 const ref_expression &rhs)
  {
    op->set_lhs (fold (lhs));
    op->set_rhs (fold (rhs));
  }

  void visit_comparison (model_lessthanequal *op,
			 const ref_expression &lhs,
			 const ref_expression &rhs)
  {
    op->set_lhs (fold (lhs));
    op->set_rhs (fold (rhs));
  }

  void visit_comparison (model_greaterthanequal *op,
			 const ref_expression &lhs,
			 const ref_expression &rhs)
  {
    op->set_lhs (fold (lhs));
    op->set_rhs (fold (rhs));
  }

  void visit_conditional (model_conditional *op,
			  const ref_expression &cond,
			  const ref_expression &true_expr,
			  const ref_expression &false_expr)
  {
    op->set_condition (fold (cond));
    op->set_true (fold (true_expr));
    op->set_false (fold (false_expr));
  }

  void visit_field_ref (model_field_ref *,
			const ref_expression &expr,
			model_field *)
  {
    if (expr)
      expr->visit (this);
  }

  void visit_field_initializer (model_field_initializer *,
				model_field *)
  {
    // This is a statement, so we should never reach it.
    abort ();
  }

  void visit_field (model_field *)
  {
    // Nothing.
  }

  void visit_instanceof (model_instanceof *,
			 const ref_expression &expr,
			 const ref_forwarding_type &)
  {
    expr->visit (this);
  }

  void visit_logical_binary (model_lor *op,
			     const ref_expression &lhs,
			     const ref_expression &rhs)
  {
    op->set_lhs (fold (lhs));
    op->set_rhs (fold (rhs));
  }

  void visit_logical_binary (model_land *op,
			     const ref_expression &lhs,
			     const ref_expression &rhs)
  {
    op->set_lhs (fold (lhs));
    op->set_rhs (fold (rhs));
  }

  void visit_simple_literal (model_literal_base *,
			     const jboolean &)
  {
  }

  void visit_simple_literal (model_literal_base *,
			     const jbyte &)
  {
  }

  void visit_simple_literal (model_literal_base *,
			     const jchar &)
  {
  }

  void visit_simple_literal (model_literal_base *,
			     const jshort &)
  {
  }

  void visit_simple_literal (model_literal_base *,
			     const jint &)
  {
  }

  void visit_simple_literal (model_literal_base *,
			     const jlong &)
  {
  }

  void visit_simple_literal (model_literal_base *,
			     const jfloat &)
  {
  }

  void visit_simple_literal (model_literal_base *,
			     const jdouble &)
  {
  }

  void visit_string_literal (model_string_literal *,
			     const std::string &)
  {
  }

  void visit_method_invocation (model_method_invocation *invocation,
				model_method *,
				const ref_expression &expr,
				const std::list<ref_expression> &arguments)
  {
    if (expr)
      expr->visit (this);
    invocation->set_arguments (fold (arguments));
  }

  void
  visit_type_qualified_invocation (model_type_qualified_invocation *invocation,
				   const model_method *,
				   const std::list<ref_expression> &arguments,
				   bool)
  {
    invocation->set_arguments (fold (arguments));
  }

  void visit_super_invocation (model_super_invocation *invocation,
			       const model_method *,
			       const std::list<ref_expression> &arguments)
  {
    invocation->set_arguments (fold (arguments));
  }

  void visit_this_invocation (model_this_invocation *invocation,
			      const model_method *,
			      const std::list<ref_expression> &arguments)
  {
    invocation->set_arguments (fold (arguments));
  }

  void visit_new (model_new *invocation,
		  const model_method *,
		  const ref_forwarding_type &,
		  const std::list<ref_expression> &arguments)
  {
    invocation->set_arguments (fold (arguments));
  }

  void visit_new_array (model_new_array *invocation,
			const ref_forwarding_type &,
			const std::list<ref_expression> &arguments,
			const ref_expression &initializer)
  {
    invocation->set_size_expressions (fold (arguments));
    if (initializer)
      initializer->visit (this);
  }

  void visit_null_literal (model_null_literal *)
  {
  }

  void visit_prefix_simple (model_prefix_plus *op,
			    const ref_expression &expr)
  {
    op->set_expression (fold (expr));
  }

  void visit_prefix_simple (model_prefix_minus *op,
			    const ref_expression &expr)
  {
    op->set_expression (fold (expr));
  }

  void visit_prefix_simple (model_bitwise_not *op,
			    const ref_expression &expr)
  {
    op->set_expression (fold (expr));
  }

  void visit_prefix_simple (model_logical_not *op,
			    const ref_expression &expr)
  {
    op->set_expression (fold (expr));
  }

  void visit_prefix_side_effect (model_prefix_plusplus *op,
				 const ref_expression &expr)
  {
    op->set_expression (fold (expr));
  }

  void visit_prefix_side_effect (model_prefix_minusminus *op,
				 const ref_expression &expr)
  {
    op->set_expression (fold (expr));
  }

  void visit_postfix_side_effect (model_postfix_plusplus *op,
				  const ref_expression &expr)
  {
    op->set_expression (fold (expr));
  }

  void visit_postfix_side_effect (model_postfix_minusminus *op,
				  const ref_expression &expr)
  {
    op->set_expression (fold (expr));
  }

  void visit_this (model_this *)
  {
  }

  void visit_simple_variable_ref (model_simple_variable_ref *,
				  const model_variable_decl *)
  {
  }

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

  void visit_element (model_element *)
  {
    // Nothing.
  }

  ref_expression fold (const ref_expression &expr)
  {
    if (expr->constant_p ())
      {
	// If we already have a literal, just return it.
	model_literal_base *lit
	  = dynamic_cast<model_literal_base *> (expr.get ());
	if (lit)
	  return expr;
	return create_literal (expr);
      }

    expr->visit (this);
    return expr;
  }

  std::list<ref_expression> fold (const std::list<ref_expression> &exprs)
  {
    std::list<ref_expression> new_exprs;
    for (std::list<ref_expression>::const_iterator i = exprs.begin ();
	 i != exprs.end ();
	 ++i)
      new_exprs.push_back (fold (*i));
    return new_exprs;
  }
};

void
fold (ref_expression &expr)
{
  fold_visitor v;
  expr = v.fold (expr);
}

void
fold (std::list<ref_expression> &exprs)
{
  fold_visitor v;
  exprs = v.fold (exprs);
}

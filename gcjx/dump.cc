// Print the model for debugging.

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

#include <cctype>

class dumper : public visitor
{
  // The output stream to use.
  std::ostream &out;

  // Current indentation level.
  int indentation;

  // The current class.
  model_class *klass;

public:

  dumper (std::ostream &o)
    : out (o),
      indentation (0),
      klass (NULL)
  {
  }

  void more_indentation ()
  {
    indentation += 2;
  }

  void less_indentation ()
  {
    indentation -= 2;
  }

  void indent (model_element *stmt)
  {
    // Special case: indentation == -1 means don't print anything.
    if (indentation == -1)
      return;

    // FIXME: should use real ostream formatter here.
    if (stmt)
      {
	char buf[10];
	sprintf (buf, "%5d", int (stmt->get_location ().get_line ()));
	out << "[" << buf << "] ";
      }
    else
      out << "[     ] ";
    for (int i = 0; i < indentation; ++i)
      out << " ";
  }

  void print (model_variable_decl *var)
  {
    out << var->type ()->get_pretty_name () << " " << var->get_name ();
  }

  void visit_method (model_method *meth,
		     const std::list<ref_variable_decl> &args,
		     const ref_block &body)
  {
    klass = meth->get_declaring_class ();

    out << "Method: " << meth->get_return_type ()->get_pretty_name ()
	<< " " << meth->get_declaring_class ()->get_pretty_name ()
	<< "." << meth->get_name ()
	<< " (";
    bool first = true;
    for (std::list<ref_variable_decl>::const_iterator i = args.begin ();
	 i != args.end ();
	 ++i)
      {
	if (first)
	  first = false;
	else
	  out << ", ";
	print ((*i).get ());
      }
    out << ")" << std::endl;

    if (body)
      body->visit (this);
    out << std::endl << std::endl;
  }

  void visit_assert (model_assert *stmt, const ref_expression &expr,
		     const ref_expression &result)
  {
    indent (stmt);
    out << "assert ";
    expr->visit (this);
    if (result)
      {
	out << " : ";
	result->visit (this);
      }
    out << ";" << std::endl;
  }

  void visit_block (model_block *stmt,
		    const std::list<ref_stmt> &stmts)
  {
    indent (stmt);
    out << "{" << std::endl;
    more_indentation ();
    ::visit (this, stmts);
    less_indentation ();
    indent (NULL);
    out << "}" << std::endl;
  }

  void visit_bytecode_block (model_bytecode_block *stmt,
			     int, int, int, const uint8 *)
  {
    indent (stmt);
    out << "<< bytecode >>" << std::endl;
  }

  void handle_target (model_stmt *stmt)
  {
    if (dynamic_cast<model_label *> (stmt) != NULL)
      {
	model_label *label = assert_cast<model_label *> (stmt);
	out << " " << label->get_name ();
      }
  }

  void visit_break (model_break *stmt, const ref_stmt &)
  {
    indent (stmt);
    out << "break";
    handle_target (stmt->get_target ());
    out << ";" << std::endl;
  }

  void visit_catch (model_catch *stmt, const ref_variable_decl &var,
		    const ref_block &body)
  {
    indent (stmt);
    out << "catch (";
    print (var.get ());
    out << ")" << std::endl;
    body->visit (this);
  }

  void visit_continue (model_continue *stmt, const ref_stmt &)
  {
    indent (stmt);
    out << "continue";
    handle_target (stmt->get_target ());
    out << ";" << std::endl;
  }

  void visit_class_decl_stmt (model_class_decl_stmt *,
			      const ref_class &klass)
  {
  }

  void visit_do (model_do *dostmt, const ref_expression &expr,
		 const ref_stmt &stmt)
  {
    indent (dostmt);
    out << "do" << std::endl;
    more_indentation ();
    stmt->visit (this);
    less_indentation ();
    indent (expr.get ());
    out << "while (";
    expr->visit (this);
    out << ");" << std::endl;
  }

  void visit_empty (model_empty *stmt)
  {
    indent (stmt);
    out << ";" << std::endl;
  }

  void visit_expression_stmt (model_expression_stmt *stmt,
			      const ref_expression &expr)
  {
    indent (stmt);
    expr->visit (this);
    out << ";" << std::endl;
  }

  void visit_for_enhanced (model_for_enhanced *stmt,
			   const ref_stmt &body, const ref_expression &expr,
			   const ref_variable_decl &var)
  {
    indent (stmt);
    out << "for (";
    print (var.get ());
    out << " : ";
    expr->visit (this);
    out << ")" << std::endl;
    more_indentation ();
    body->visit (this);
    less_indentation ();
  }

  void visit_for (model_for *stmt, const ref_stmt &init,
		  const ref_expression &cond, const ref_stmt &update,
		  const ref_stmt &body)
  {
    indent (stmt);
    out << "for (";
    if (init)
      {
	int save = indentation;
	indentation = -1;
	init->visit (this);
	indentation = save;
      }
    out << "; ";
    if (cond)
      cond->visit (this);
    out << "; ";
    if (update)
      {
	int save = indentation;
	indentation = -1;
	update->visit (this);
	indentation = save;
      }
    out << ")" << std::endl;
    more_indentation ();
    body->visit (this);
    less_indentation ();
  }

  void visit_if (model_if *stmt, const ref_expression &cond,
		 const ref_stmt &yes, const ref_stmt &no)
  {
    indent (stmt);
    out << "if (";
    cond->visit (this);
    out << ")" << std::endl;
    more_indentation ();
    yes->visit (this);
    less_indentation ();
    if (no)
      {
	indent (no.get ());
	out << "else" << std::endl;
	more_indentation ();
	no->visit (this);
	less_indentation ();
      }
  }

  void visit_label (model_label *label, const ref_stmt &body)
  {
    indent (label);
    out << label->get_name () << ":" << std::endl;
    more_indentation ();
    body->visit (this);
    less_indentation ();
  }

  void visit_return (model_return *stmt, const ref_expression &expr)
  {
    indent (stmt);
    out << "return";
    if (expr)
      {
	out << " ";
	expr->visit (this);
      }
    out << std::endl;
  }

  void visit_switch (model_switch *stmt,
		     const ref_expression &expr,
		     const std::list<ref_switch_block> &blocks)
  {
    indent (stmt);
    out << "switch (";
    expr->visit (this);
    out << ")" << std::endl;
    more_indentation ();
    for (std::list<ref_switch_block>::const_iterator i = blocks.begin ();
	 i != blocks.end ();
	 ++i)
      (*i)->visit (this);
    less_indentation ();
  }

  void visit_switch_block (model_switch_block *block,
			   const std::list<ref_stmt> &stmts)
  {
    std::list<ref_expression> labels = block->get_labels ();
    for (std::list<ref_expression>::const_iterator i = labels.begin ();
	 i != labels.end ();
	 ++i)
      {
	indent ((*i).get ());
	out << "case ";
	(*i)->visit (this);
	out << ":" << std::endl;
      }

    more_indentation ();
    ::visit (this, stmts);
    less_indentation ();
  }

  void visit_synchronized (model_synchronized *stmt,
			   const ref_expression &expr,
			   const ref_stmt &body)
  {
    indent (stmt);
    out << "synchronized (";
    expr->visit (this);
    out << ")" << std::endl;
    more_indentation ();
    body->visit (this);
    less_indentation ();
  }

  void visit_throw (model_throw *stmt, const ref_expression &expr)
  {
    indent (stmt);
    out << "throw ";
    expr->visit (this);
    out << ";" << std::endl;
  }

  void visit_try (model_try *stmt, const ref_block &body,
		  const std::list<ref_catch> &catchers,
		  const ref_block &finally)
  {
    indent (stmt);
    out << "try" << std::endl;
    body->visit (this);
    for (std::list<ref_catch>::const_iterator i = catchers.begin ();
	 i != catchers.end ();
	 ++i)
      (*i)->visit (this);
    if (finally)
      {
	out << "finally" << std::endl;
	finally->visit (this);
      }
  }

  void visit_variable_stmt (model_variable_stmt *,
			    const std::list<ref_variable_decl> &vars)
  {
    for (std::list<ref_variable_decl>::const_iterator i = vars.begin ();
	 i != vars.end ();
	 ++i)
      {
	indent ((*i).get ());
	print ((*i).get ());
	if ((*i)->has_initializer_p ())
	  {
	    out << " = ";
	    (*i)->get_initializer ()->visit (this);
	  }
	out << ";" << std::endl;
      }
  }

  void visit_while (model_while *stmt, const ref_expression &expr,
		    const ref_stmt &body)
  {
    indent (stmt);
    out << "while (";
    expr->visit (this);
    out << ")" << std::endl;
    more_indentation ();
    body->visit (this);
    less_indentation ();
  }

  void visit_array_initializer (model_array_initializer *,
				const ref_forwarding_type &,
				const std::list<ref_expression> &exprs)
  {
    out << "{ ";
    bool first = true;
    for (std::list<ref_expression>::const_iterator i = exprs.begin ();
	 i != exprs.end ();
	 ++i)
      {
	if (first)
	  first = false;
	else
	  out << ", ";
	(*i)->visit (this);
      }
    out << " }";
  }

  void visit_array_ref (model_array_ref *,
			const ref_expression &array,
			const ref_expression &index)
  {
    array->visit (this);
    out << "[";
    index->visit (this);
    out << "]";
  }

  void handle_binary (const char *op,
		      const ref_expression &lhs,
		      const ref_expression &rhs)
  {
    lhs->visit (this);
    out << " " << op << " ";
    rhs->visit (this);
  }

  void visit_assignment (model_assignment *,
			 const ref_expression &lhs,
			 const ref_expression &rhs)
  {
    handle_binary ("=", lhs, rhs);
  }

  void visit_op_assignment (model_minus_equal *,
			    const ref_expression &lhs,
			    const ref_expression &rhs)
  {
    handle_binary ("-=", lhs, rhs);
  }

  void visit_op_assignment (model_mult_equal *,
			    const ref_expression &lhs,
			    const ref_expression &rhs)
  {
    handle_binary ("*=", lhs, rhs);
  }

  void visit_op_assignment (model_div_equal *,
			    const ref_expression &lhs,
			    const ref_expression &rhs)
  {
    handle_binary ("/=", lhs, rhs);
  }

  void visit_op_assignment (model_and_equal *,
			    const ref_expression &lhs,
			    const ref_expression &rhs)
  {
    handle_binary ("&=", lhs, rhs);
  }

  void visit_op_assignment (model_or_equal *,
			    const ref_expression &lhs,
			    const ref_expression &rhs)
  {
    handle_binary ("|=", lhs, rhs);
  }

  void visit_op_assignment (model_plus_equal *,
			    const ref_expression &lhs,
			    const ref_expression &rhs)
  {
    handle_binary ("+=", lhs, rhs);
  }

  void visit_op_assignment (model_xor_equal *,
			    const ref_expression &lhs,
			    const ref_expression &rhs)
  {
    handle_binary ("^=", lhs, rhs);
  }

  void visit_op_assignment (model_mod_equal *,
			    const ref_expression &lhs,
			    const ref_expression &rhs)
  {
    handle_binary ("%=", lhs, rhs);
  }

  void visit_op_assignment (model_ls_equal *,
			    const ref_expression &lhs,
			    const ref_expression &rhs)
  {
    handle_binary ("<<=", lhs, rhs);
  }

  void visit_op_assignment (model_rs_equal *,
			    const ref_expression &lhs,
			    const ref_expression &rhs)
  {
    handle_binary (">>=", lhs, rhs);
  }

  void visit_op_assignment (model_urs_equal *,
			    const ref_expression &lhs,
			    const ref_expression &rhs)
  {
    handle_binary (">>>=", lhs, rhs);
  }

  void visit_arith_binary (model_minus *,
			   const ref_expression &lhs,
			   const ref_expression &rhs)
  {
    handle_binary ("-", lhs, rhs);
  }

  void visit_arith_binary (model_mult *,
			   const ref_expression &lhs,
			   const ref_expression &rhs)
  {
    handle_binary ("*", lhs, rhs);
  }

  void visit_arith_binary (model_div *,
			   const ref_expression &lhs,
			   const ref_expression &rhs)
  {
    handle_binary ("/", lhs, rhs);
  }

  void visit_arith_binary (model_mod *,
			   const ref_expression &lhs,
			   const ref_expression &rhs)
  {
    handle_binary ("%", lhs, rhs);
  }

  void visit_arith_binary (model_and *,
			   const ref_expression &lhs,
			   const ref_expression &rhs)
  {
    handle_binary ("&", lhs, rhs);
  }

  void visit_arith_binary (model_or *,
			   const ref_expression &lhs,
			   const ref_expression &rhs)
  {
    handle_binary ("|", lhs, rhs);
  }

  void visit_arith_binary (model_xor *,
			   const ref_expression &lhs,
			   const ref_expression &rhs)
  {
    handle_binary ("^", lhs, rhs);
  }

  void visit_arith_binary (model_plus *,
			   const ref_expression &lhs,
			   const ref_expression &rhs)
  {
    handle_binary ("+", lhs, rhs);
  }

  void visit_arith_shift (model_left_shift *,
			  const ref_expression &lhs,
			  const ref_expression &rhs)
  {
    handle_binary ("<<", lhs, rhs);
  }

  void visit_arith_shift (model_right_shift *,
			  const ref_expression &lhs,
			  const ref_expression &rhs)
  {
    handle_binary (">>", lhs, rhs);
  }

  void visit_arith_shift (model_unsigned_right_shift *,
			  const ref_expression &lhs,
			  const ref_expression &rhs)
  {
    handle_binary (">>>", lhs, rhs);
  }

  void visit_cast (model_cast *, const ref_forwarding_type &type,
		   const ref_expression &expr)
  {
    out << "(" << type->type ()->get_pretty_name () << ") ";
    expr->visit (this);
  }

  void visit_class_ref (model_class_ref *,
			const ref_forwarding_type &type)
  {
    out << type->type ()->get_pretty_name () << ".class";
  }

  void visit_comparison (model_equal *,
			 const ref_expression &lhs,
			 const ref_expression &rhs)
  {
    handle_binary ("==", lhs, rhs);
  }

  void visit_comparison (model_notequal *,
			 const ref_expression &lhs,
			 const ref_expression &rhs)
  {
    handle_binary ("!=", lhs, rhs);
  }

  void visit_comparison (model_lessthan *,
			 const ref_expression &lhs,
			 const ref_expression &rhs)
  {
    handle_binary ("<", lhs, rhs);
  }

  void visit_comparison (model_greaterthan *,
			 const ref_expression &lhs,
			 const ref_expression &rhs)
  {
    handle_binary (">", lhs, rhs);
  }

  void visit_comparison (model_lessthanequal *,
			 const ref_expression &lhs,
			 const ref_expression &rhs)
  {
    handle_binary ("<=", lhs, rhs);
  }

  void visit_comparison (model_greaterthanequal *,
			 const ref_expression &lhs,
			 const ref_expression &rhs)
  {
    handle_binary (">=", lhs, rhs);
  }

  void visit_conditional (model_conditional *,
			  const ref_expression &cond,
			  const ref_expression &lhs,
			  const ref_expression &rhs)
  {
    cond->visit (this);
    out << " ? ";
    lhs->visit (this);
    out << " : ";
    rhs->visit (this);
  }

  void visit_field_ref (model_field_ref *,
			const ref_expression &expr,
			model_field *field)
  {
    if (expr)
      {
	expr->visit (this);
	out << ".";
      }
    else
      out << field->get_declaring_class ()->get_pretty_name () << ".";
    out << field->get_name ();
  }

  void visit_field_initializer (model_field_initializer *stmt,
				model_field *field)
  {
    indent (stmt);
    out << field->get_name ();
    if (field->has_initializer_p ())
      {
	out << " = ";
	field->get_initializer ()->visit (this);
      }
    out << ";" << std::endl;
  }

  void visit_field (model_field *)
  {
    // Nothing.
  }

  void visit_instanceof (model_instanceof *,
			 const ref_expression &expr,
			 const ref_forwarding_type &type)
  {
    expr->visit (this);
    out << " instanceof " << type->type ()->get_pretty_name ();;
  }

  void visit_logical_binary (model_lor *,
			     const ref_expression &lhs,
			     const ref_expression &rhs)
  {
    handle_binary ("||", lhs, rhs);
  }

  void visit_logical_binary (model_land *,
			     const ref_expression &lhs,
			     const ref_expression &rhs)
  {
    handle_binary ("&&", lhs, rhs);
  }

  void visit_simple_literal (model_literal_base *,
			     const jboolean &val)
  {
    out << (val ? "true" : "false");
  }

  void visit_simple_literal (model_literal_base *,
			     const jbyte &val)
  {
    out << val;
  }

  void visit_simple_literal (model_literal_base *,
			     const jchar &val)
  {
    // FIXME: pretty print.
    out << val;
  }

  void visit_simple_literal (model_literal_base *,
			     const jshort &val)
  {
    out << val;
  }

  void visit_simple_literal (model_literal_base *,
			     const jint &val)
  {
    out << val;
  }

  void visit_simple_literal (model_literal_base *,
			     const jlong &val)
  {
    out << val;
  }

  void visit_simple_literal (model_literal_base *,
			     const jfloat &val)
  {
    out << val;
  }

  void visit_simple_literal (model_literal_base *,
			     const jdouble &val)
  {
    out << val;
  }

  void visit_string_literal (model_string_literal *,
			     const std::string &val)
  {
    // FIXME: quote contents.
    out << "\"" << val << "\"";
  }

  void handle_args (const std::list<ref_expression> &args)
  {
    out << " (";
    bool first = true;
    for (std::list<ref_expression>::const_iterator i = args.begin ();
	 i != args.end ();
	 ++i)
      {
	if (first)
	  first = false;
	else
	  out << ", ";
	(*i)->visit (this);
      }
    out << ")";
  }

  void visit_method_invocation (model_method_invocation *,
				model_method *meth,
				const ref_expression &expr,
				const std::list<ref_expression> &args)
  {
    if (expr)
      {
	expr->visit (this);
	out << ".";
      }
    else
      out << meth->get_declaring_class ()->get_pretty_name () << ".";
    out << meth->get_name ();
    handle_args (args);
  }

  void
  visit_type_qualified_invocation (model_type_qualified_invocation *,
                                   const model_method *meth,
                                   const std::list<ref_expression> &args,
                                   bool super)
  {
    out << meth->get_declaring_class ()->get_pretty_name () << ".";
    if (super)
      out << "super.";
    out << meth->get_name ();
    handle_args (args);
  }

  void visit_super_invocation (model_super_invocation *,
			       const model_method *,
			       const std::list<ref_expression> &args,
			       const ref_expression &finit)
  {
    out << "super";
    handle_args (args);
    if (finit)
      finit->visit (this);
  }

  void visit_this_invocation (model_this_invocation *,
			      const model_method *,
			      const std::list<ref_expression> &args)
  {
    out << "this";
    handle_args (args);
  }

  void visit_new (model_new *, const model_method *,
		  const ref_forwarding_type &type,
		  const std::list<ref_expression> &args)
  {
    out << "new " << type->type ()->get_pretty_name ();
    handle_args (args);
  }

  void visit_new_array (model_new_array *,
			const ref_forwarding_type &type,
			const std::list<ref_expression> &exprs,
			const ref_expression &init)
  {
    out << "new " << type->type ()->get_pretty_name ();
    for (std::list<ref_expression>::const_iterator i = exprs.begin ();
	 i != exprs.end ();
	 ++i)
      {
	out << "[";
	(*i)->visit (this);
	out << "]";
      }
    if (init)
      {
	out << " ";
	init->visit (this);
      }
  }

  void visit_null_literal (model_null_literal *)
  {
    out << "null";
  }

  void visit_prefix_simple (model_prefix_plus *,
			    const ref_expression &expr)
  {
    out << "+";
    expr->visit (this);
  }

  void visit_prefix_simple (model_prefix_minus *,
			    const ref_expression &expr)
  {
    out << "-";
    expr->visit (this);
  }

  void visit_prefix_simple (model_bitwise_not *,
			    const ref_expression &expr)
  {
    out << "~";
    expr->visit (this);
  }

  void visit_prefix_simple (model_logical_not *,
			    const ref_expression &expr)
  {
    out << "!";
    expr->visit (this);
  }

  void visit_prefix_side_effect (model_prefix_plusplus *,
				 const ref_expression &expr)
  {
    out << "++";
    expr->visit (this);
  }

  void visit_prefix_side_effect (model_prefix_minusminus *,
				 const ref_expression &expr)
  {
    out << "--";
    expr->visit (this);
  }

  void visit_postfix_side_effect (model_postfix_plusplus *,
				  const ref_expression &expr)
  {
    expr->visit (this);
    out << "++";
  }

  void visit_postfix_side_effect (model_postfix_minusminus *,
				  const ref_expression &expr)
  {
    expr->visit (this);
    out << "--";
  }

  void visit_this (model_this *expr)
  {
    if (expr->type () != klass)
      out << expr->type ()->get_pretty_name () << ".";
    out << "this";
  }

  void visit_simple_variable_ref (model_simple_variable_ref *,
				  const model_variable_decl *var)
  {
    out << var->get_name ();
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

  void visit_abstract_method (model_abstract_method *am,
                              const std::list<ref_variable_decl> &params,
			      const ref_block &body, model_method *)
  {
    visit_method (am, params, body);
  }

  void visit_annotation_type (model_annotation_type *at,
                              const std::string &descr,
                              const std::string &name)
  {
    visit_class (at, descr, name);
  }
  
  void visit_array_type (model_array_type *at, const std::string &descr,
                         const std::string &name, model_type *)
  {
    visit_class (at, descr, name);
  }

  void visit_class (model_class *c, const std::string &descr,
                    const std::string &)
  {
    visit_type (c, descr);
  }

  void visit_class_instance (model_class_instance *ci,
                             const std::string &descr,
                             const std::string &name, model_class *)
  {
    visit_class (ci, descr, name);
  }
  
  void visit_constructor (model_constructor *c,
                          const std::list<ref_variable_decl> &params,
                          const ref_block &body)
  {
    visit_method (c, params, body);
  }

  void visit_enum (model_enum *e, const std::string &descr,
                   const std::string &name,
                   const std::list<ref_enum_constant> &)
  {
    visit_class (e, descr, name);
  }

  void visit_enum_constant (model_enum_constant *ec,
                            const std::string &descr,
                            const std::string &name,
                            const std::list<ref_expression> &)
  {
    visit_class (ec, descr, name);
  }

  void visit_fp_primitive (model_primitive_base *, char, jfloat)
  {
    // Nothing.
  }

  void visit_fp_primitive (model_primitive_base *, char, jdouble)
  {
    // Nothing.
  }

  void visit_int_primitive (model_primitive_base *, char,
                            long long, long long, jbyte)
  {
    // Nothing.
  }

  void visit_int_primitive (model_primitive_base *, char,
                            long long, long long, jchar)
  {
    // Nothing.
  }

  void visit_int_primitive (model_primitive_base *, char,
                            long long, long long, jshort)
  {
    // Nothing.
  }

  void visit_int_primitive (model_primitive_base *, char,
                            long long, long long, jint)
  {
    // Nothing.
  }

  void visit_int_primitive (model_primitive_base *, char,
                            long long, long long, jlong)
  {
    // Nothing.
  }

  void visit_primitive_boolean (model_primitive_boolean *)
  {
    // Nothing.
  }

  void visit_initializer_block (model_initializer_block *ib,
                                const std::list<ref_stmt> &stmts, bool)
  {
    visit_block (ib, stmts);
  }

  void visit_new_primary (model_new_primary *np, const model_method *meth,
			  const ref_forwarding_type &klass,
			  const std::list<ref_expression> &args,
			  const std::string &,
			  const std::list<ref_forwarding_type> &)
  {
    visit_new (np, meth, klass, args);
  }

  void visit_null_type (model_null_type *nt, const std::string &descr)
  {
    visit_type (nt, descr);
  }

  void visit_phony_block (model_phony_block *pb,
                          const std::list<ref_stmt> &stmts)
  {
    visit_block (pb, stmts);
  }

  void visit_primordial_package (model_primordial_package *pp,
                                 const std::list<std::string> &name)
  {
    visit_package (pp, name);
  }

  void visit_unnamed_package (model_unnamed_package *up,
                              const std::list<std::string> &name)
  {
    visit_package (up, name);
  }

  void visit_synthetic_this (model_synthetic_this *st)
  {
    visit_this (st);
  }

  void visit_this_outer (model_this_outer *to)
  {
    visit_this (to);
  }

  void
  visit_type_variable (model_type_variable *tv,
                       const std::string &descr, const std::string &name,
                       const std::list<ref_forwarding_type> &)
  {
    visit_class (tv, descr, name);
  }

  void visit_void_type (model_void_type *vt, const std::string &descr)
  {
    visit_type (vt, descr);
  }

  void visit_wildcard (model_wildcard *w, const std::string &descr,
                       const std::string &name, bool,
                       const ref_forwarding_type &)
  {
    visit_class (w, descr, name);
  }

  void visit_javadoc (model_javadoc *, bool)
  {
    // Nothing.
  }

  void
  visit_generic_invocation (model_type_qualified_invocation *tqi,
                            const model_method *meth,
                            const std::list<ref_expression> &args,
			    bool is_super,
                            const std::list<ref_forwarding_type> &)
  {
    visit_type_qualified_invocation (tqi, meth, args, is_super);
  }

  void
  visit_generic_invocation (model_super_invocation *si,
                            const model_method *meth,
                            const std::list<ref_expression> &args,
                            const ref_expression &finit,
                            const std::list<ref_forwarding_type> &)
  {
    visit_super_invocation (si, meth, args, finit);
  }

  void
  visit_generic_invocation (model_this_invocation *ti,
                            const model_method *meth,
                            const std::list<ref_expression> &args,
                            const std::list<ref_forwarding_type> &)
  {
    visit_this_invocation (ti, meth, args);
  }

  void
  visit_generic_invocation (model_new *n, const model_method *meth,
                            const ref_forwarding_type &klass,
                            const std::list<ref_expression> &args,
                            const std::list<ref_forwarding_type> &)
  {
    visit_new (n, meth, klass, args);
  }

  void
  visit_generic_invocation (model_new_primary *np, const model_method *meth,
                            const ref_forwarding_type &klass,
                            const std::list<ref_expression> &args,
                            const std::string &simple_name,
                            const std::list<ref_forwarding_type> &type_params,
                            const std::list<ref_forwarding_type> &)
  {
    visit_new_primary (np, meth, klass, args, simple_name, type_params);
  }

  void
  visit_annotation (model_annotation *, const ref_forwarding_type &,
                    const std::list<ref_annotation_value> &)
  {
    abort ();
  }

  void
  visit_annotation_initializer (model_annotation_initializer *,
                                const ref_forwarding_type &,
                                const std::list<ref_expression> &)
  {
    abort ();
  }

  void visit_memberref_enum (model_memberref_enum *,
                             const ref_forwarding_type &,
                             const std::string &)
  {
    abort ();
  }
};

/// A simple pretty-printer for the model elements.
class pretty_printer : public visitor
{
protected:

  /// The output stream to use.
  std::ostream &out;

  /// The current indentation level.
  int indentation;

  /// Begin printing an element.  This includes indenting the output,
  /// printing the address of the element, etc.
  /// @param e a pointer to the element to be printed.
  /// @param name the name for the type of the element.
  virtual void begin_element (model_element *e, const std::string &name)
  {
    for (int i = 0; i < indentation; i++)
      out << " ";

    out << "(" << name << " " << e;

    if (e != NULL)
      {
        location l = e->get_location ();
        out << " " << l.get_line () << ":" << l.get_column ();
      }
  }

  /// End printing an element.
  virtual void end_element ()
  {
    out << ")";
    if (indentation == 0)
      out << std::endl;
  }

  /// Helper method to print out an element on the next level in the tree.
  /// @param e a pointer to the element.
  virtual void descend (model_element *e)
  {
    if (e != NULL)
      {
        indentation += 2;
        out << std::endl;

        // FIXME: Keep track of nodes already visited so that we do not
        // inadvertently cause an infinite recursion and blow the stack.
        e->visit (this);

        indentation -= 2;
      }
  }

  /// Helper method to print out a list of elements on the next level in the
  /// tree.
  /// @param l the list of elements.
  template<typename T>
  void descend (const std::list<T> &l)
  {
    typename std::list<T>::const_iterator i = l.begin ();
    if (i != l.end ())
      {
        indentation += 2;
        out << std::endl;
        if (*i)
          (*i)->visit (this);
        indentation -= 2;
      }
  }

  /// Helper method to print out a dot-separated name with multiple
  /// components.
  /// @param name the components comprising the name.
  virtual void print_multi_name (const std::list<std::string> &name)
  {
    std::list<std::string>::const_iterator i = name.begin ();

    if (i != name.end ())
      out << " ";

    bool first = true;
    while (i != name.end ())
      {
        if (!first)
          out << ".";
        else
          first = false;

        out << (*i);
        ++i;
      }
  }

  /// Helper function to print out the modifiers for an element.
  /// @param mods the modifiers for the element.
  virtual void print_modifiers (modifier_t mods)
  {
    if ((mods & ACC_PUBLIC) != 0)
      out << " PUBLIC";
    else if ((mods & ACC_PRIVATE) != 0)
      out << " PRIVATE";
    else if ((mods & ACC_PROTECTED) != 0)
      out << " PROTECTED";
    else
      out << " PKG_PRIVATE";

    if ((mods & ACC_STATIC) != 0)
      out << " STATIC";

    if ((mods & ACC_FINAL) != 0)
      out << " FINAL";

    if ((mods & ACC_SYNCHRONIZED) != 0)
      out << " SYNCHRONIZED";

    if ((mods & ACC_SUPER) != 0)
      out << " SUPER";

    if ((mods & ACC_VOLATILE) != 0)
      out << " VOLATILE";

    if ((mods & ACC_BRIDGE) != 0)
      out << " BRIDGE";

    if ((mods & ACC_TRANSIENT) != 0)
      out << " TRANSIENT";

    if ((mods & ACC_VARARGS) != 0)
      out << " VARARGS";

    if ((mods & ACC_NATIVE) != 0)
      out << " NATIVE";

    if ((mods & ACC_INTERFACE) != 0)
      out << " INTERFACE";

    if ((mods & ACC_ABSTRACT) != 0)
      out << " ABSTRACT";

    if ((mods & ACC_STRICTFP) != 0)
      out << " STRICTFP";

    if ((mods & ACC_SYNTHETIC) != 0)
      out << " SYNTHETIC";

    if ((mods & ACC_ANNOTATION) != 0)
      out << " ANNOTATION";

    if ((mods & ACC_ENUM) != 0)
      out << " ENUM";
  }

public:

  pretty_printer (std::ostream &o)
    : out (o),
      indentation (0)
  {
  }

  void visit_method (model_method *meth,
                     const std::list<ref_variable_decl> &args,
                     const ref_block &body)
  {
    begin_element (meth, "method");

    out << " " << meth->get_name ();

    descend (meth->get_return_type ());
    descend (args);
    if (body)
      descend (body.get ());

    end_element ();
  }

  void visit_assert (model_assert *a, const ref_expression &eval_expr,
                     const ref_expression &res_expr)
  {
    begin_element (a, "assert");
    if (eval_expr)
      descend (eval_expr.get ());
    if (res_expr)
      descend (res_expr.get ());
    end_element ();
  }

  void visit_block (model_block *block, const std::list<ref_stmt> &stmts)
  {
    begin_element (block, "block");
    descend (stmts);
    end_element ();
  }

  void visit_bytecode_block (model_bytecode_block *bc_block,
                             int max_stack, int max_locals, int length,
                             const uint8 *bytes)
  {
    begin_element (bc_block, "bytecode_block");
    out << " MAX_STACK=" << max_stack;
    out << " MAX_LOCALS=" << max_locals;
    out << " LENGTH=" << length;
    end_element ();
  }

  void visit_break (model_break *brk, const ref_stmt &target)
  {
    begin_element (brk, "break");
    
    // We can not descend to the target statement, as it would already
    // have been printed and this could cause an infinite loop.
    // For now, just print out the address of the target statement.
    if (target)
      out << " TARGET_STMT=" << target.get ();

    end_element ();
  }

  void visit_catch (model_catch *catch_stmt,
                    const ref_variable_decl &var_decl,
                    const ref_block &catch_block)
  {
    begin_element (catch_stmt, "catch");
    if (var_decl)
      descend (var_decl.get ());
    if (catch_block)
      descend (catch_block.get ());
    end_element ();
  }

  void visit_continue (model_continue *cnt, const ref_stmt &target)
  {
    begin_element (cnt, "continue");
    
    // We can not descend to the target loop, as it would already
    // have been printed and this could cause an infinite loop.
    // For now, just print out the address of the target loop.
    if (target)
      out << " TARGET_LOOP=" << target.get ();

    end_element ();
  }

  void visit_class_decl_stmt (model_class_decl_stmt *stmt,
                              const ref_class &decl)
  {
    begin_element (stmt, "class_decl");
    if (decl)
      descend (decl.get ());
    end_element ();
  }

  void visit_do (model_do *do_stmt, const ref_expression &expr,
                 const ref_stmt &stmt)
  {
    begin_element (do_stmt, "do");
    if (expr)
      descend (expr.get ());
    if (stmt)
      descend (stmt.get ());
    end_element ();
  }

  void visit_empty (model_empty *e)
  {
    begin_element (e, "empty");
    end_element ();
  }

  void visit_expression_stmt (model_expression_stmt *stmt,
                              const ref_expression &expr)
  {
    begin_element (stmt, "expr_stmt");
    if (expr)
      descend (expr.get ());
    end_element ();
  }

  void visit_for_enhanced (model_for_enhanced *for_enh,
                           const ref_stmt &body,
                           const ref_expression &expr,
                           const ref_variable_decl &var)
  {
    begin_element (for_enh, "for_enhanced");

    model_type *elt_type = for_enh->get_element_type ();
    if (elt_type != NULL)
      out << " ELT_TYPE=" << elt_type->get_pretty_name ();

    if (var)
      descend (var.get ());
    if (expr)
      descend (expr.get ());
    if (body)
      descend (body.get ());

    end_element ();
  }

  void visit_for (model_for *for_stmt, const ref_stmt &initializer,
                  const ref_expression &cond_expr, const ref_stmt &update,
                  const ref_stmt &body)
  {
    begin_element (for_stmt, "for");
    if (initializer)
      descend (initializer.get ());
    if (cond_expr)
      descend (cond_expr.get ());
    if (update)
      descend (update.get ());
    if (body)
      descend (body.get ());
    end_element ();
  }

  void visit_if (model_if *if_stmt, const ref_expression &cond_expr,
                 const ref_stmt &true_branch, const ref_stmt &false_branch)
  {
    begin_element (if_stmt, "if");
    if (cond_expr)
      descend (cond_expr.get ());
    if (true_branch)
      descend (true_branch.get ());
    if (false_branch)
      descend (false_branch.get ());
    end_element ();
  }

  void visit_label (model_label *lbl, const ref_stmt &stmt)
  {
    begin_element (lbl, "label");
    out << " " << lbl->get_name ();
    if (stmt)
      descend (stmt.get ());
    end_element ();
  }

  void visit_return (model_return *ret, const ref_expression &ret_expr)
  {
    begin_element (ret, "return");
    if (ret_expr)
      descend (ret_expr.get ());
    end_element ();
  }

  void visit_switch (model_switch *sw, const ref_expression &expr,
                     const std::list<ref_switch_block> &sw_blocks)
  {
    begin_element (sw, "switch");
    if (expr)
      descend (expr.get ());
    descend (sw_blocks);
    end_element ();
  }

  void visit_switch_block (model_switch_block *sw_block,
                           const std::list<ref_stmt> &stmts)
  {
    begin_element (sw_block, "switch_block");

    if (sw_block->can_complete_normally ())
      out << " COMPLETES_NORMALLY";

    std::list<ref_expression> labels = sw_block->get_labels ();
    descend (labels);

    descend (stmts);
    end_element ();
  }

  void visit_synchronized (model_synchronized *synch,
                           const ref_expression &expr,
                           const ref_stmt &stmt)
  {
    begin_element (synch, "synchronized");
    if (expr)
      descend (expr.get ());
    if (stmt)
      descend (stmt.get ());
    end_element ();
  }

  void visit_throw (model_throw *th, const ref_expression &expr)
  {
    begin_element (th, "throw");
    if (expr)
      descend (expr.get ());
    end_element ();
  }

  void visit_try (model_try *tr, const ref_block &try_block,
                  const std::list<ref_catch> &catchers,
                  const ref_block &finally_block)
  {
    begin_element (tr, "try");
    if (try_block)
      descend (try_block.get ());
    descend (catchers);
    if (finally_block)
      descend (finally_block.get ());
    end_element ();
  }

  void visit_variable_stmt (model_variable_stmt *var_stmt,
                            const std::list<ref_variable_decl> &vars)
  {
    begin_element (var_stmt, "var_stmt");
    descend (vars);
    end_element ();
  }

  void visit_while (model_while *wh, const ref_expression &expr,
                    const ref_stmt &body)
  {
    begin_element (wh, "while");
    if (expr)
      descend (expr.get ());
    if (body)
      descend (body.get ());
    end_element ();
  }

  void visit_array_initializer (model_array_initializer *arr_init,
                                const ref_forwarding_type &elt_type,
                                const std::list<ref_expression> &initializers)
  {
    begin_element (arr_init, "array_init");
    if (elt_type)
      descend (elt_type.get ());
    descend (initializers);
    end_element ();
  }

  void visit_array_ref (model_array_ref *arr_ref,
                        const ref_expression &array,
                        const ref_expression &index)
  {
    begin_element (arr_ref, "array_ref");
    if (array)
      descend (array.get ());
    if (index)
      descend (index.get ());
    end_element ();
  }

  void visit_assignment (model_assignment *assgn,
                         const ref_expression &lhs,
                         const ref_expression &rhs)
  {
    begin_element (assgn, "assign");
    if (lhs)
      descend (lhs.get ());
    if (rhs)
      descend (rhs.get ());
    end_element ();
  }

  void visit_op_assignment (model_minus_equal *assgn,
                            const ref_expression &lhs,
                            const ref_expression &rhs)
  {
    begin_element (assgn, "minus_equal");
    if (lhs)
      descend (lhs.get ());
    if (rhs)
      descend (rhs.get ());
    end_element ();
  }

  void visit_op_assignment (model_mult_equal *assgn,
                            const ref_expression &lhs,
                            const ref_expression &rhs)
  {
    begin_element (assgn, "mult_equal");
    if (lhs)
      descend (lhs.get ());
    if (rhs)
      descend (rhs.get ());
    end_element ();
  }

  void visit_op_assignment (model_div_equal *assgn,
                            const ref_expression &lhs,
                            const ref_expression &rhs)
  {
    begin_element (assgn, "div_equal");
    if (lhs)
      descend (lhs.get ());
    if (rhs)
      descend (rhs.get ());
    end_element ();
  }

  void visit_op_assignment (model_and_equal *assgn,
                            const ref_expression &lhs,
                            const ref_expression &rhs)
  {
    begin_element (assgn, "and_equal");
    if (lhs)
      descend (lhs.get ());
    if (rhs)
      descend (rhs.get ());
    end_element ();
  }

  void visit_op_assignment (model_or_equal *assgn,
                            const ref_expression &lhs,
                            const ref_expression &rhs)
  {
    begin_element (assgn, "or_equal");
    if (lhs)
      descend (lhs.get ());
    if (rhs)
      descend (rhs.get ());
    end_element ();
  }

  void visit_op_assignment (model_plus_equal *assgn,
                            const ref_expression &lhs,
                            const ref_expression &rhs)
  {
    begin_element (assgn, "plus_equal");
    if (lhs)
      descend (lhs.get ());
    if (rhs)
      descend (rhs.get ());
    end_element ();
  }

  void visit_op_assignment (model_xor_equal *assgn,
                            const ref_expression &lhs,
                            const ref_expression &rhs)
  {
    begin_element (assgn, "xor_equal");
    if (lhs)
      descend (lhs.get ());
    if (rhs)
      descend (rhs.get ());
    end_element ();
  }

  void visit_op_assignment (model_mod_equal *assgn,
                            const ref_expression &lhs,
                            const ref_expression &rhs)
  {
    begin_element (assgn, "mod_equal");
    if (lhs)
      descend (lhs.get ());
    if (rhs)
      descend (rhs.get ());
    end_element ();
  }

  void visit_op_assignment (model_ls_equal *assgn,
                            const ref_expression &lhs,
                            const ref_expression &rhs)
  {
    begin_element (assgn, "lshift_equal");
    if (lhs)
      descend (lhs.get ());
    if (rhs)
      descend (rhs.get ());
    end_element ();
  }

  void visit_op_assignment (model_rs_equal *assgn,
                            const ref_expression &lhs,
                            const ref_expression &rhs)
  {
    begin_element (assgn, "rshift_equal");
    if (lhs)
      descend (lhs.get ());
    if (rhs)
      descend (rhs.get ());
    end_element ();
  }

  void visit_op_assignment (model_urs_equal *assgn,
                            const ref_expression &lhs,
                            const ref_expression &rhs)
  {
    begin_element (assgn, "urshift_equal");
    if (lhs)
      descend (lhs.get ());
    if (rhs)
      descend (rhs.get ());
    end_element ();
  }

  void visit_arith_binary (model_minus *op,
                           const ref_expression &lhs,
                           const ref_expression &rhs)
  {
    begin_element (op, "minus");
    if (lhs)
      descend (lhs.get ());
    if (rhs)
      descend (rhs.get ());
    end_element ();
  }

  void visit_arith_binary (model_mult *op,
                           const ref_expression &lhs,
                           const ref_expression &rhs)
  {
    begin_element (op, "mult");
    if (lhs)
      descend (lhs.get ());
    if (rhs)
      descend (rhs.get ());
    end_element ();
  }

  void visit_arith_binary (model_div *op,
                           const ref_expression &lhs,
                           const ref_expression &rhs)
  {
    begin_element (op, "div");
    if (lhs)
      descend (lhs.get ());
    if (rhs)
      descend (rhs.get ());
    end_element ();
  }

  void visit_arith_binary (model_mod *op,
                           const ref_expression &lhs,
                           const ref_expression &rhs)
  {
    begin_element (op, "mod");
    if (lhs)
      descend (lhs.get ());
    if (rhs)
      descend (rhs.get ());
    end_element ();
  }

  void visit_arith_binary (model_and *op,
                           const ref_expression &lhs,
                           const ref_expression &rhs)
  {
    begin_element (op, "and");
    if (lhs)
      descend (lhs.get ());
    if (rhs)
      descend (rhs.get ());
    end_element ();
  }

  void visit_arith_binary (model_or *op,
                           const ref_expression &lhs,
                           const ref_expression &rhs)
  {
    begin_element (op, "or");
    if (lhs)
      descend (lhs.get ());
    if (rhs)
      descend (rhs.get ());
    end_element ();
  }

  void visit_arith_binary (model_xor *op,
                           const ref_expression &lhs,
                           const ref_expression &rhs)
  {
    begin_element (op, "xor");
    if (lhs)
      descend (lhs.get ());
    if (rhs)
      descend (rhs.get ());
    end_element ();
  }

  void visit_arith_binary (model_plus *op,
                           const ref_expression &lhs,
                           const ref_expression &rhs)
  {
    begin_element (op, "plus");
    if (lhs)
      descend (lhs.get ());
    if (rhs)
      descend (rhs.get ());
    end_element ();
  }

  void visit_arith_shift (model_left_shift *op,
                          const ref_expression &lhs,
                          const ref_expression &rhs)
  {
    begin_element (op, "lshift");
    if (lhs)
      descend (lhs.get ());
    if (rhs)
      descend (rhs.get ());
    end_element ();
  }

  void visit_arith_shift (model_right_shift *op,
                          const ref_expression &lhs,
                          const ref_expression &rhs)
  {
    begin_element (op, "rshift");
    if (lhs)
      descend (lhs.get ());
    if (rhs)
      descend (rhs.get ());
    end_element ();
  }

  void visit_arith_shift (model_unsigned_right_shift *op,
                          const ref_expression &lhs,
                          const ref_expression &rhs)
  {
    begin_element (op, "urshift");
    if (lhs)
      descend (lhs.get ());
    if (rhs)
      descend (rhs.get ());
    end_element ();
  }

  void visit_cast (model_cast *cast,
                   const ref_forwarding_type &target_type,
                   const ref_expression &expr)
  {
    begin_element (cast, "cast");
    if (target_type)
      descend (target_type.get ());
    if (expr)
      descend (expr.get ());
    end_element ();
  }

  void visit_class_ref (model_class_ref *cref,
                        const ref_forwarding_type &ctype)
  {
    begin_element (cref, "class_ref");
    if (ctype)
      descend (ctype.get ());
    end_element ();
  }

  void visit_comparison (model_equal *comp,
                         const ref_expression &lhs,
                         const ref_expression &rhs)
  {
    begin_element (comp, "equal");
    if (lhs)
      descend (lhs.get ());
    if (rhs)
      descend (rhs.get ());
    end_element ();
  }

  void visit_comparison (model_notequal *comp,
                         const ref_expression &lhs,
                         const ref_expression &rhs)
  {
    begin_element (comp, "not_equal");
    if (lhs)
      descend (lhs.get ());
    if (rhs)
      descend (rhs.get ());
    end_element ();
  }

  void visit_comparison (model_lessthan *comp,
                         const ref_expression &lhs,
                         const ref_expression &rhs)
  {
    begin_element (comp, "less_than");
    if (lhs)
      descend (lhs.get ());
    if (rhs)
      descend (rhs.get ());
    end_element ();
  }

  void visit_comparison (model_greaterthan *comp,
                         const ref_expression &lhs,
                         const ref_expression &rhs)
  {
    begin_element (comp, "greater_than");
    if (lhs)
      descend (lhs.get ());
    if (rhs)
      descend (rhs.get ());
    end_element ();
  }

  void visit_comparison (model_lessthanequal *comp,
                         const ref_expression &lhs,
                         const ref_expression &rhs)
  {
    begin_element (comp, "less_than_equal");
    if (lhs)
      descend (lhs.get ());
    if (rhs)
      descend (rhs.get ());
    end_element ();
  }

  void visit_comparison (model_greaterthanequal *comp,
                         const ref_expression &lhs,
                         const ref_expression &rhs)
  {
    begin_element (comp, "greater_than_equal");
    if (lhs)
      descend (lhs.get ());
    if (rhs)
      descend (rhs.get ());
    end_element ();
  }

  void visit_conditional (model_conditional *cond,
                          const ref_expression &cond_expr,
                          const ref_expression &true_branch,
                          const ref_expression &false_branch)
  {
    begin_element (cond, "conditional");
    if (cond_expr)
      descend (cond_expr.get ());
    if (true_branch)
      descend (true_branch.get ());
    if (false_branch)
      descend (false_branch.get ());
    end_element ();
  }

  void visit_field_ref (model_field_ref *f,
                        const ref_expression &expr,
                        model_field *field)
  {
    begin_element (f, "field_ref");

    if (f->qualified_p ())
      out << " QUALIFIED";

    descend (f->get_qualifying_class ());
    if (expr)
      descend (expr.get ());
    descend (field);
     
    end_element ();
  }

  void visit_field_initializer (model_field_initializer *f_init,
                                model_field *f)
  {
    begin_element (f_init, "field_init");
    descend (f);
    end_element ();
  }

  void visit_field (model_field *field)
  {
    begin_element (field, "field");
    
    modifier_t mods = field->get_modifiers ();
    print_modifiers (mods);

    out << " " << field->get_name ();

    end_element ();
  }

  void visit_instanceof (model_instanceof *inst_of,
                         const ref_expression &expr,
                         const ref_forwarding_type &check_type)
  {
    begin_element (inst_of, "instanceof");
    if (expr)
      descend (expr.get ());
    if (check_type)
      descend (check_type.get ());
    end_element ();
  }

  void visit_logical_binary (model_lor *op,
                             const ref_expression &lhs,
                             const ref_expression &rhs)
  {
    begin_element (op, "logical_or");
    if (lhs)
      descend (lhs.get ());
    if (rhs)
      descend (rhs.get ());
    end_element ();
  }

  void visit_logical_binary (model_land *op,
                             const ref_expression &lhs,
                             const ref_expression &rhs)
  {
    begin_element (op, "logical_and");
    if (lhs)
      descend (lhs.get ());
    if (rhs)
      descend (rhs.get ());
    end_element ();
  }

  void visit_simple_literal (model_literal_base *lit, const jboolean &val)
  {
    begin_element (lit, "jboolean");
    out << (val ? " TRUE" : " FALSE");
    end_element ();
  }

  void visit_simple_literal (model_literal_base *lit, const jbyte &val)
  {
    begin_element (lit, "jbyte");
    out << " " << val;
    end_element ();
  }

  void visit_simple_literal (model_literal_base *lit, const jchar &val)
  {
    begin_element (lit, "jchar");
    if (::isgraph (val))
      out << " \'" << (char) val << "\'";
    else
      {
        switch (val)
          {
          case ' ':
            out << " \' \'";
            break;
          case '\t':
            out << " \'\\t\'";
            break;
          case '\n':
            out << " \'\\n\'";
            break;
          case '\r':
            out << " \'\\r\'";
            break;
          default:
            // FIXME: Show Unicode escape sequence.
            out << " " << std::hex << std::showbase << val;
            break;
          }
      }
    end_element ();
  }

  void visit_simple_literal (model_literal_base *lit, const jshort &val)
  {
    begin_element (lit, "jshort");
    out << " " << val;
    end_element ();
  }

  void visit_simple_literal (model_literal_base *lit, const jint &val)
  {
    begin_element (lit, "jint");
    out << " " << val;
    end_element ();
  }

  void visit_simple_literal (model_literal_base *lit, const jlong &val)
  {
    begin_element (lit, "jlong");
    out << " " << val;
    end_element ();
  }

  void visit_simple_literal (model_literal_base *lit, const jfloat &val)
  {
    begin_element (lit, "jfloat");
    out << " " << val;
    end_element ();
  }

  void visit_simple_literal (model_literal_base *lit, const jdouble &val)
  {
    begin_element (lit, "jdouble");
    out << " " << val;
    end_element ();
  }

  void visit_string_literal (model_string_literal *s, const std::string &val)
  {
    begin_element (s, "string");
    
    // FIXME: Need to escape certain characters.
    out << " \"" << val << "\"";

    end_element ();
  }

  void visit_method_invocation (model_method_invocation *meth_inv,
                                model_method *meth,
                                const ref_expression &expr,
                                const std::list<ref_expression> &args)
  {
    begin_element (meth_inv, "method_invocation");
    descend (meth);
    if (expr)
      descend (expr.get ());
    descend (args);
    end_element ();
  }

  void
  visit_type_qualified_invocation (model_type_qualified_invocation *tqi,
				   const model_method *meth,
				   const std::list<ref_expression> &args,
				   bool is_super)
  {
    begin_element (tqi, "type_qual_invocation");
    if (is_super)
      out << " SUPER";
    if (meth != NULL)
      out << " " << meth->get_name ();
    else
      out << " NULL";
    descend (args);
    end_element ();
  }

  void visit_super_invocation (model_super_invocation *si,
                               const model_method *meth,
                               const std::list<ref_expression> &args,
			       const ref_expression &finit)
  {
    begin_element (si, "super_invocation");
    if (meth != NULL)
      out << " " << meth->get_name ();
    else
      out << " NULL";
    descend (args);
    end_element ();
    if (finit)
      finit->visit (this);
  }

  void visit_this_invocation (model_this_invocation *ti,
                              const model_method *meth,
                              const std::list<ref_expression> &args)
  {
    begin_element (ti, "this_invocation");
    if (meth != NULL)
      out << " " << meth->get_name ();
    else
      out << " NULL";
    descend (args);
    end_element ();
  }

  void visit_new (model_new *n, const model_method *meth,
                  const ref_forwarding_type &klass,
                  const std::list<ref_expression> &args)
  {
    begin_element (n, "new");
    if (klass)
      descend (klass.get ());
    if (meth != NULL)
      out << " " << meth->get_name ();
    else
      out << " NULL";
    descend (args);
    end_element ();
  }

  void visit_new_array (model_new_array *na,
                        const ref_forwarding_type &klass,
                        const std::list<ref_expression> &sizes,
                        const ref_expression &initializer)
  {
    begin_element (na, "new_array");
    if (klass)
      descend (klass.get ());
    descend (sizes);
    if (initializer)
      descend (initializer.get ());
    end_element ();
  }

  void visit_null_literal (model_null_literal *n)
  {
    begin_element (n, "null");
    end_element ();
  }

  void visit_prefix_simple (model_prefix_plus *p, const ref_expression &expr)
  {
    begin_element (p, "prefix_plus");
    if (expr)
      descend (expr.get ());
    end_element ();
  }

  void visit_prefix_simple (model_prefix_minus *p, const ref_expression &expr)
  {
    begin_element (p, "prefix_minus");
    if (expr)
      descend (expr.get ());
    end_element ();
  }

  void visit_prefix_simple (model_bitwise_not *p, const ref_expression &expr)
  {
    begin_element (p, "bitwise_not");
    if (expr)
      descend (expr.get ());
    end_element ();
  }

  void visit_prefix_simple (model_logical_not *p, const ref_expression &expr)
  {
    begin_element (p, "logical_not");
    if (expr)
      descend (expr.get ());
    end_element ();
  }

  void visit_prefix_side_effect (model_prefix_plusplus *p,
                                 const ref_expression &expr)
  {
    begin_element (p, "prefix_plusplus");
    if (expr)
      descend (expr.get ());
    end_element ();
  }

  void visit_prefix_side_effect (model_prefix_minusminus *p,
                                 const ref_expression &expr)
  {
    begin_element (p, "prefix_minusminus");
    if (expr)
      descend (expr.get ());
    end_element ();
  }

  void visit_postfix_side_effect (model_postfix_plusplus *p,
                                  const ref_expression &expr)
  {
    begin_element (p, "postfix_plusplus");
    if (expr)
      descend (expr.get ());
    end_element ();
  }

  void visit_postfix_side_effect (model_postfix_minusminus *p,
                                  const ref_expression &expr)
  {
    begin_element (p, "postfix_minusminus");
    if (expr)
      descend (expr.get ());
    end_element ();
  }

  void visit_this (model_this *t)
  {
    begin_element (t, "this");
    end_element ();
  }

  void visit_simple_variable_ref (model_simple_variable_ref *ref,
                                  const model_variable_decl *var)
  {
    begin_element (ref, "variable_ref");
    if (var != NULL)
      out << " " << var->get_name ();
    else
      out << " NULL";
    end_element ();
  }

  void visit_forwarding_type (model_forwarding_type *fwd_type,
                              model_type *res_type)
  {
    begin_element (fwd_type, "fwd_type");
    descend (res_type);
    end_element ();
  }

  void visit_forwarding_resolved (model_forwarding_resolved *fwd_resolved,
                                  model_type *res_type)
  {
    begin_element (fwd_resolved, "fwd_type_resolved");
    descend (res_type);
    end_element ();
  }

  void visit_forwarding_owned (model_forwarding_owned *fwd_owned,
                               const ref_type &own_type)
  {
    begin_element (fwd_owned, "fwd_type_owned");
    if (own_type)
      descend (own_type.get ());
    end_element ();
  }

  void visit_forwarding_simple (model_forwarding_simple *fwd_simple,
                                const std::list<std::string> &name)
  {
    begin_element (fwd_simple, "fwd_type_simple");
    print_multi_name (name);
    end_element ();
  }

  void
  visit_forwarding_array (model_forwarding_array *fwd_array,
                          const owner<model_forwarding_type> &t)
  {
    begin_element (fwd_array, "fwd_type_array");
    if (t)
      descend (t.get ());
    end_element ();
  }

  void
  visit_forwarding_element (model_forwarding_element *fwd_element,
                            const owner<model_forwarding_type> &t)
  {
    begin_element (fwd_element, "fwd_type_element");
    if (t)
      descend (t.get ());
    end_element ();
  }

  void visit_forwarding_full (model_forwarding_full *fwd_full,
                              const std::string &name)
  {
    begin_element (fwd_full, "fwd_type_full");
    out << " " << name;
    end_element ();
  }

  void
  visit_forwarding_inner (model_forwarding_inner *fwd_inner,
                          const std::list<std::string> &name,
                          const owner<model_forwarding_type> &parent)
  {
    begin_element (fwd_inner, "fwd_type_inner");
    print_multi_name (name);
    if (parent)
      descend (parent.get ());
    end_element ();
  }

  void
  visit_forwarding_parameterized (model_forwarding_parameterized *fwd_param,
                                  const owner<model_forwarding_type> &base,
                                  const std::list< owner<model_forwarding_type> > &params)
  {
    begin_element (fwd_param, "fwd_type_param");
    if (base)
      descend (base.get ());
    descend (params);
    end_element ();
  }

  void visit_variable_decl (model_variable_decl *var,
                            const std::string &name,
                            const ref_forwarding_type &decltype,
                            const ref_expression &initializer,
                            bool is_final,
                            bool is_used)
  {
    begin_element (var, "var_decl");

    out << " " << name;

    if (is_final)
      out << " FINAL";
    if (!is_used)
      out << " UNUSED";

    if (decltype)
      descend (decltype.get ());
    if (initializer)
      descend (initializer.get ());

    end_element ();
  }

  void visit_parameter_decl (model_variable_decl *param,
                             const std::string &name,
                             const ref_forwarding_type &decltype,
                             const ref_expression &initializer,
                             bool is_final,
                             bool is_used)
  {
    begin_element (param, "param_decl");

    out << " " << name;

    if (is_final)
      out << " FINAL";
    if (!is_used)
      out << " UNUSED";

    if (decltype)
      descend (decltype.get ());
    if (initializer)
      descend (initializer.get ());

    end_element ();
  }

  void visit_catch_decl (model_variable_decl *decl, const std::string &name,
                         const ref_forwarding_type &decltype,
                         const ref_expression &initializer,
                         bool is_final, bool is_used)
  {
    begin_element (decl, "catch_decl");

    out << " " << name;
    if (is_final)
      out << " FINAL";
    if (!is_used)
      out << " UNUSED";

    if (decltype)
      descend (decltype.get ());
    if (initializer)
      descend (initializer.get ());

    end_element ();
  }

  void visit_package (model_package *p, const std::list<std::string> &name)
  {
    begin_element (p, "package");
    print_multi_name (name);
    end_element ();
  }

  void visit_type (model_type *t, const std::string &descriptor)
  {
    begin_element (t, "type");
    out << " " << t->get_pretty_name ();
    end_element ();
  }

  void visit_identifier (model_identifier *i, const std::string &ident)
  {
    begin_element (i, "identifier");
    out << " " << ident;
    end_element ();
  }
  
  void visit_annotation_member (model_annotation_member *,
                                const ref_forwarding_type &)
  {
    // TODO.
  }

  void visit_annotation_value (model_annotation_value *,
                               const std::string &, const ref_expression &)
  {
    // TODO.
  }

  void visit_import_single (model_import_single *,
                            const std::list<std::string> &, model_class *)
  {
    // TODO.
  }

  void visit_import_on_demand (model_import_on_demand *,
                               const std::list<std::string> &, Iname *, bool)
  {
    // TODO.
  }

  void visit_static_import_single (model_static_import_single *,
				   const std::list<std::string> &,
				   model_class *, const std::string &)
  {
    // TODO.
  }

  void visit_static_import_on_demand (model_static_import_on_demand *,
				      const std::list<std::string> &,
				      model_class *)
  {
    // TODO.
  }
  
  void visit_unit_source (model_unit_source *, model_package *,
			  const std::list<ref_class> &,
			  const std::string &,
			  bool, const std::list<ref_import> &)
  {
    // TODO.
  }
  
  void visit_unit_class (model_unit_class *, model_package *,
			 const std::list<ref_class> &,
			 const std::string &, bool)
  {
    // TODO.
  }
  
  void visit_unit_fake (model_unit_fake *, model_package *,
			const std::list<ref_class> &,
			const std::string &, bool)
  {
    // TODO.
  }

  void visit_abstract_method (model_abstract_method *am,
                              const std::list<ref_variable_decl> &params,
			      const ref_block &body, model_method *)
  {
    // TODO.
  }

  void visit_annotation_type (model_annotation_type *at,
                              const std::string &descr,
                              const std::string &name)
  {
    // TODO.
  }
  
  void visit_array_type (model_array_type *at, const std::string &descr,
                         const std::string &name, model_type *)
  {
    // TODO.
  }

  void visit_class (model_class *c, const std::string &descr,
                    const std::string &)
  {
    // TODO.
  }

  void visit_class_instance (model_class_instance *ci,
                             const std::string &descr,
                             const std::string &name, model_class *)
  {
    // TODO.
  }
  
  void visit_constructor (model_constructor *c,
                          const std::list<ref_variable_decl> &params,
                          const ref_block &body)
  {
    // TODO.
  }

  void visit_enum (model_enum *e, const std::string &descr,
                   const std::string &name,
                   const std::list<ref_enum_constant> &)
  {
    // TODO.
  }

  void visit_enum_constant (model_enum_constant *ec,
                            const std::string &descr,
                            const std::string &name,
                            const std::list<ref_expression> &)
  {
    // TODO.
  }

  void visit_fp_primitive (model_primitive_base *, char, jfloat)
  {
    // TODO.
  }

  void visit_fp_primitive (model_primitive_base *, char, jdouble)
  {
    // TODO.
  }

  void visit_int_primitive (model_primitive_base *, char,
                            long long, long long, jbyte)
  {
    // TODO.
  }

  void visit_int_primitive (model_primitive_base *, char,
                            long long, long long, jchar)
  {
    // TODO.
  }

  void visit_int_primitive (model_primitive_base *, char,
                            long long, long long, jshort)
  {
    // TODO.
  }

  void visit_int_primitive (model_primitive_base *, char,
                            long long, long long, jint)
  {
    // TODO.
  }

  void visit_int_primitive (model_primitive_base *, char,
                            long long, long long, jlong)
  {
    // TODO.
  }

  void visit_primitive_boolean (model_primitive_boolean *)
  {
    // TODO.
  }

  void visit_initializer_block (model_initializer_block *ib,
                                const std::list<ref_stmt> &stmts, bool)
  {
    // TODO.
  }

  void visit_new_primary (model_new_primary *np, const model_method *meth,
			  const ref_forwarding_type &klass,
			  const std::list<ref_expression> &args,
			  const std::string &,
			  const std::list<ref_forwarding_type> &)
  {
    // TODO.
  }

  void visit_null_type (model_null_type *nt, const std::string &descr)
  {
    // TODO.
  }

  void visit_phony_block (model_phony_block *pb,
                          const std::list<ref_stmt> &stmts)
  {
    // TODO.
  }

  void visit_primordial_package (model_primordial_package *pp,
                                 const std::list<std::string> &name)
  {
    // TODO.
  }

  void visit_unnamed_package (model_unnamed_package *up,
                              const std::list<std::string> &name)
  {
    // TODO.
  }

  void visit_synthetic_this (model_synthetic_this *st)
  {
    // TODO.
  }

  void visit_this_outer (model_this_outer *to)
  {
    // TODO.
  }

  void
  visit_type_variable (model_type_variable *tv,
                       const std::string &descr, const std::string &name,
                       const std::list<ref_forwarding_type> &)
  {
    // TODO.
  }

  void visit_void_type (model_void_type *vt, const std::string &descr)
  {
    // TODO.
  }

  void visit_wildcard (model_wildcard *w, const std::string &descr,
                       const std::string &name, bool,
                       const ref_forwarding_type &)
  {
    // TODO.
  }

  void visit_javadoc (model_javadoc *, bool)
  {
    // TODO.
  }

  void
  visit_generic_invocation (model_type_qualified_invocation *tqi,
                            const model_method *meth,
                            const std::list<ref_expression> &args,
			    bool is_super,
                            const std::list<ref_forwarding_type> &)
  {
    // TODO.
  }

  void
  visit_generic_invocation (model_super_invocation *si,
                            const model_method *meth,
                            const std::list<ref_expression> &args,
                            const ref_expression &finit,
                            const std::list<ref_forwarding_type> &)
  {
    // TODO.
  }

  void
  visit_generic_invocation (model_this_invocation *ti,
                            const model_method *meth,
                            const std::list<ref_expression> &args,
                            const std::list<ref_forwarding_type> &)
  {
    // TODO.
  }

  void
  visit_generic_invocation (model_new *n, const model_method *meth,
                            const ref_forwarding_type &klass,
                            const std::list<ref_expression> &args,
                            const std::list<ref_forwarding_type> &)
  {
    // TODO.
  }

  void
  visit_generic_invocation (model_new_primary *np, const model_method *meth,
                            const ref_forwarding_type &klass,
                            const std::list<ref_expression> &args,
                            const std::string &simple_name,
                            const std::list<ref_forwarding_type> &type_params,
                            const std::list<ref_forwarding_type> &)
  {
    // TODO.
  }

  void
  visit_annotation (model_annotation *, const ref_forwarding_type &,
                    const std::list<ref_annotation_value> &)
  {
    // TODO.
  }

  void
  visit_annotation_initializer (model_annotation_initializer *,
                                const ref_forwarding_type &,
                                const std::list<ref_expression> &)
  {
    // TODO.
  }

  void visit_memberref_enum (model_memberref_enum *,
                             const ref_forwarding_type &,
                             const std::string &)
  {
    // TODO.
  }
};

void
dump_method (model_method *m)
{
  dumper d (std::cout);
  m->visit (&d);
}

void
dump_tree (model_element *e)
{
  if (e != NULL)
    {
      pretty_printer p (std::cout);
      e->visit (&p);
    }
  else
    std::cout << "(NULL)" << std::endl;
}

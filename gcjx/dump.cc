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
			const model_field *field)
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
				const model_method *meth,
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
			       const std::list<ref_expression> &args)
  {
    out << "super";
    handle_args (args);
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
                              const model_type *)
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
};

void
dump_method (model_method *m)
{
  dumper d (std::cout);
  m->visit (&d);
}

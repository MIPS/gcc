// The parser.

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

#ifndef GCJX_SOURCE_PARSE_HH
#define GCJX_SOURCE_PARSE_HH

#include "source/tstream.hh"

// Helper typedef.
class parse;
typedef ref_expression (parse::*expression_parser) ();

class parse
{
  static const int METHOD_CONSTRUCTOR = 1;
  static const int METHOD_VOID = 2;

  // Source of tokens.
  token_stream *our_token_stream;

  // Enclosing classes.
  std::deque<model_class *> class_stack;

  // Scope for 'break' and 'continue'.
  std::deque<Ilabel *> label_stack;

  // This compilation unit.
  model_unit_source *unit;

  // The package for this compilation unit.
  model_package *pkg;

  // Get the current enclosing class.  When parsing a class body, this
  // is the same as the class we are parsing.
  model_class *enclosing_class ()
  {
    return class_stack.empty () ? NULL : class_stack.back ();
  }

  // Find the appropriate target for a break or continue statement.
  Ilabel *find_lexically_enclosing_loop (model_element *, token_value,
					 const std::string &);

  // Fetch the next token.
  token &get ()
  {
    return our_token_stream->get_token ();
  }

  // Peek ahead a token.
  token &peek ()
  {
    return our_token_stream->peek_token ();
  }

  // Peek ahead two tokens.
  token &peek1 ()
  {
    return our_token_stream->peek_token1 ();
  }

  // Require that the next token be of a certain type.  If not, throw
  // a parse exception.
  token &require (token_value val, const char *message = NULL);

  // Require that the next token will be a certain type.  If not,
  // abort.
  token &assume (token_value);

  // Classify tokens.
  static bool parse::basic_type_p (token_value);
  static bool parse::assignment_op_p (token_value);

  // Locate the lexically enclosing class with the given name.
  model_class *find_lexically_enclosing_class (const std::string &);

  // Return the primitive type corresponding to a token value.
  model_type *get_primitive_type (token_value);

  // Helper to set containing class.
  void set_containing_class (const ref_class &);

  // Various rules of the grammar.  We don't comment these
  // individually as they come more or less straight from the JLS.
  // Oddities are documented in the implementation.
  std::string identifier ();
  std::list<std::string> qualified_identifier ();
  std::list< std::list<std::string> > qualified_identifier_list ();
  ref_expression identifier_suffix (const std::list<std::string> &);
  std::list<ref_expression> arguments ();
  ref_expression variable_initializer (const ref_forwarding_type &);
  ref_expression array_initializer (const ref_forwarding_type &);
  ref_expression new_something (bool = false);
  ref_expression type_dot_class ();
  ref_expression class_name_dot_this ();
  ref_expression super_access ();
  ref_expression primary ();
  ref_expression postfix_expression ();
  ref_expression unary_expression_not_plus_minus ();
  ref_expression unary_expression ();
  ref_expression multiplicative_expression ();
  ref_expression additive_expression ();
  ref_expression shift_expression ();
  ref_expression relational_expression ();
  ref_expression equality_expression ();
  ref_expression and_expression ();
  ref_expression exclusive_or_expression ();
  ref_expression inclusive_or_expression ();
  ref_expression conditional_and_expression ();
  ref_expression conditional_or_expression ();
  ref_expression conditional_expression ();
  ref_expression left_hand_side ();
  ref_expression assignment ();
  ref_expression assignment_expression ();
  ref_expression statement_expression ();
  ref_expression expression ();
  ref_forwarding_type brackets_opt (ref_forwarding_type);
  ref_forwarding_type primitive_type ();
  ref_forwarding_type type_name (int &);
  ref_forwarding_type counted_type (int &);
  ref_forwarding_type type ();
  std::list<ref_forwarding_type> type_list ();
  ref_variable_decl variable_declarator_id (ref_forwarding_type &,
					    bool = false, bool = false);
  ref_variable_decl formal_parameter (bool, bool *, bool = false,
				      bool = false);
  std::list<ref_variable_decl> formal_parameter_list (bool, bool *);
  ref_assert assert_statement ();
  ref_stmt more_statement_expressions ();
  ref_stmt for_init ();
  ref_stmt for_update ();
  ref_for for_statement ();
  ref_while while_statement ();
  ref_synchronized synchronized_statement ();
  ref_empty empty_statement ();
  ref_label labeled_statement ();
  ref_do do_statement ();
  ref_try try_statement ();
  ref_if if_then_statement ();
  void switch_block_statement_groups (const ref_switch &);
  ref_switch switch_statement ();
  ref_stmt expression_statement ();
  ref_stmt statement ();
  ref_variable_decl variable_declarator (ref_forwarding_type);
  std::list<ref_variable_decl> variable_declarators (ref_forwarding_type);
  ref_stmt local_variable_declaration ();
  ref_stmt local_variable_declaration_statement ();
  std::list<ref_stmt> block_statements ();
  ref_block block (const ref_block &);
  ref_modifier_list modifiers_opt ();
  ref_forwarding_type result_type ();
  ref_stmt explicit_constructor_invocation ();
  ref_block constructor_body ();
  ref_block method_body ();
  void type_bound_opt (const ref_type_variable &, int &);
  ref_type_variable type_parameter (int &);
  std::list<ref_type_variable> type_parameters ();
  ref_annotation annotation ();
  std::list<ref_annotation> annotations ();
  ref_method any_method_declarator (const std::list<ref_annotation> &,
				    const ref_modifier_list &,
				    int,
				    ref_forwarding_type,
				    bool);
  void field_declarator (const ref_class &,
			 const ref_forwarding_type &,
			 const std::list<ref_annotation> &,
			 const ref_modifier_list &,
			 bool);
  void member_decl (const ref_class &,
		    const std::list<ref_annotation> &,
		    const ref_modifier_list &,
		    bool,
		    bool);
  ref_method void_method (const ref_class &,
			  const std::list<ref_annotation> &,
			  const ref_modifier_list &,
			  bool);
  void class_body_block (const std::list<ref_annotation> &,
			 const ref_modifier_list &);
  void class_body_declaration (const ref_class &, bool);
  void class_body (const ref_class &, bool = false);
  ref_class interface_declaration (const ref_modifier_list &);
  ref_class class_declaration (const ref_modifier_list &);
  void enum_body (ref_enum);
  ref_enum enum_declaration (const ref_modifier_list &);
  ref_class class_or_interface_declaration ();
  ref_stmt class_or_interface_declaration_statement ();
  ref_class type_declaration ();
  ref_import import_declaration ();
  ref_expression member_value ();
  ref_annotation_type annotation_type_declaration (const ref_modifier_list &);
  void annotation_type_member (const ref_class &,
			       const ref_modifier_list &,
			       const ref_forwarding_type &);
  void final_or_attributes (bool &, std::list<ref_annotation> &);
  std::list<ref_forwarding_type> actual_type_parameters ();

  template<token_value ttype, typename M> ref_stmt loop_statement ();
  template<token_value ttype, typename M> ref_stmt control_statement ();

  template<expression_parser, model_binary_base *MATCH (const token &)>
  friend class recursion_machine;

  template<typename T>
  friend class state_machine;

public:

  parse (token_stream *ts)
    : our_token_stream (ts),
      unit (NULL),
      pkg (NULL)
  {
  }

  ~parse ()
  {
  }

  // Compile a file and return a compilation unit.  This may throw
  // exceptions of various sorts.
  ref_unit compilation_unit ();
};

#endif // GCJX_SOURCE_PARSE_HH

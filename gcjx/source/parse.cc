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

#include "typedefs.hh"
#include "source/parse.hh"
#include "source/machine.hh"

// A few notes on the implementation:
//
// This is a pretty simplistic recursive-descent parser.  It hasn't
// been tuned at all.  Functions in this parser correspond fairly
// closely to the grammar from JLS 2nd Ed.  In some cases this has
// been changed around for implementation convenience (such places are
// commented) or for post-JLS additions like "assert".
//
// The bulk of the code in this file should either be directly
// concerned with parsing, or it should be constructing model objects
// to represent the program.  Very little semantic analysis should be
// done here; that is generally left to a later phase.  In some
// situations we do a little analysis here, where it is easy and makes
// things simpler later on.
//
// This parser does more work than it needs to.  In particular it
// back-tracks, when in fact it doesn't need to.  This is purely
// programming convenience.

// Basically no work has been done on error recovery or error
// messages.  This needs a serious look.  FIXME.



inline bool
parse::basic_type_p (token_value val)
{
  return (val == TOKEN_BYTE
	  || val == TOKEN_SHORT
	  || val == TOKEN_CHAR
	  || val == TOKEN_INT
	  || val == TOKEN_LONG
	  || val == TOKEN_FLOAT
	  || val == TOKEN_DOUBLE
	  || val == TOKEN_BOOLEAN);
}

inline bool
parse::assignment_op_p (token_value val)
{
  return (val == TOKEN_ASSIGN
	  || val == TOKEN_BITWISE_AND_ASSIGN
	  || val == TOKEN_BITWISE_OR_ASSIGN
	  || val == TOKEN_BITWISE_XOR_ASSIGN
	  || val == TOKEN_DIVIDE_ASSIGN
	  || val == TOKEN_LEFT_SHIFT_ASSIGN
	  || val == TOKEN_MINUS_ASSIGN
	  || val == TOKEN_MOD_ASSIGN
	  || val == TOKEN_MULTIPLY_ASSIGN
	  || val == TOKEN_PLUS_ASSIGN
	  || val == TOKEN_RIGHT_SHIFT_ASSIGN
	  || val == TOKEN_UNSIGNED_RIGHT_SHIFT_ASSIGN);
}



inline token &
parse::require (token_value val, const char *message)
{
  token &t = get ();
  if (t != val)
    {
      if (message)
	throw syntax_error (t, message);
      else
	throw syntax_error (t, "%1 expected") % lexer::token_to_string (val);
    }
  return t;
}

inline token &
parse::assume (token_value val)
{
  token &t = get ();
  assert (t == val);
  return t;
}



model_type *
parse::get_primitive_type (token_value val)
{
  switch (val)
    {
    case TOKEN_BYTE:
      return primitive_byte_type;
    case TOKEN_SHORT:
      return primitive_short_type;
    case TOKEN_CHAR:
      return primitive_char_type;
    case TOKEN_INT:
      return primitive_int_type;
    case TOKEN_LONG:
      return primitive_long_type;
    case TOKEN_FLOAT:
      return primitive_float_type;
    case TOKEN_DOUBLE:
      return primitive_double_type;
    case TOKEN_BOOLEAN:
      return primitive_boolean_type;

      // We have a default case to let us avoid compiler warnings, and
      // also "just in case".
    default:
      abort ();
    }
}

model_class *
parse::find_lexically_enclosing_class (const std::string &name)
{
  for (std::deque<model_class *>::reverse_iterator i = class_stack.rbegin ();
       i != class_stack.rend ();
       ++i)
    {
      if ((*i)->get_name () == name)
	return *i;
    }
  return NULL;
}

Ilabel *
parse::find_lexically_enclosing_loop (model_element *request,
				      token_value kind,
				      const std::string &name)
{
  for (std::deque<Ilabel *>::reverse_iterator i = label_stack.rbegin ();
       i != label_stack.rend ();
       ++i)
    {
      // A NULL label on the stack indicates a class boundary.
      if (*i == NULL)
	break;

      if ((*i)->match_label_p (kind, name))
	return *i;
    }

  if (kind == TOKEN_BREAK)
    std::cerr << request->error ("break with no enclosing do, "
				 "while, for, or switch statement");
  else
    {
      assert (kind == TOKEN_CONTINUE);
      std::cerr << request->error ("continue with no enclosing do, "
				   "while, or for statement");
    }

  return NULL;
}



std::string
parse::identifier ()
{
  token &t = require (TOKEN_IDENTIFIER, "identifier expected");
  return ((ref_identifier) t)->get_identifier ();
}

// qualified-identifier:
//   identifier { "." identifier }
std::list<std::string>
parse::qualified_identifier ()
{
  std::list<std::string> result;
  while (true)
    {
      result.push_back (identifier ());
      if (peek () != TOKEN_DOT || peek1 () != TOKEN_IDENTIFIER)
	break;
      assume (TOKEN_DOT);
    }
  return result;
}

// qualified-identifier-list:
//   qualified-identifier { "," qualified-identifier }
std::list< std::list<std::string> >
parse::qualified_identifier_list ()
{
  std::list< std::list<std::string> > result;
  while (true)
    {
      result.push_back (qualified_identifier ());
      if (peek () != TOKEN_COMMA)
	break;
      assume (TOKEN_COMMA);
    }
  return result;
}

// arguments:
//   "(" [ expression { , expression } ] ")"
std::list<ref_expression>
parse::arguments ()
{
  require (TOKEN_OPEN_PAREN);
  std::list<ref_expression> result;
  if (peek () != TOKEN_CLOSE_PAREN)
    {
      while (true)
	{
	  result.push_back (expression ());
	  if (peek () != TOKEN_COMMA)
	    break;
	  assume (TOKEN_COMMA);
	}
    }
  require (TOKEN_CLOSE_PAREN);

  return result;
}

// variable-initializer:
//   array-initializer
//   | expression
ref_expression
parse::variable_initializer (const ref_forwarding_type &var_type)
{
  if (peek () == TOKEN_OPEN_BRACE)
    return array_initializer (var_type);
  // FIXME: better error message here.
  return expression ();
}

// array-initializer
//   "{" [ variable-initializer { "," variable-initializer } [ "," ] "}"
ref_expression
parse::array_initializer (const ref_forwarding_type &array_type)
{
  location w = require (TOKEN_OPEN_BRACE);

  std::list<ref_expression> result;
  ref_forwarding_type element_type = new model_forwarding_element (array_type);
  if (peek () != TOKEN_CLOSE_BRACE)
    {
      // It is possible to have a sole ',' in the initializer.  We
      // handle this specially.
      if (peek () == TOKEN_COMMA && peek1 () == TOKEN_CLOSE_BRACE)
	assume (TOKEN_COMMA);
      else
	{
	  while (true)
	    {
	      result.push_back (variable_initializer (element_type));
	      if (peek () != TOKEN_COMMA)
		break;
	      assume (TOKEN_COMMA);
	      // A trailing comma is ok, and is ignored.
	      if (peek () == TOKEN_CLOSE_BRACE)
		break;
	    }
	}
    }

  require (TOKEN_CLOSE_BRACE);
  return ref_expression (new model_array_initializer (w, element_type,
						      result));
}

// You won't find this one in the JLS.
// new-something:
//   "new" [ actual-type-parameters ] class-or-interface-type
//     "(" [ argument-list ] ")" [ class-body ]
//   | array-creation-expression
//
// array-creation-expression:
//   "new" primitive-type dim-exprs [ dims ]
//   "new" type-name dim-exprs [ dims ]
//   "new" primitive-type dim-exprs dims array-initializer
//   "new" type-name dims array-initializer
//
// dim-exprs:
//   { "[" expression "]" }
//
// dims:
//   { "[" "]" }
ref_expression
parse::new_something (bool primary_dot_new)
{
  ref_forwarding_type ctype;
  std::string simple_name;
  std::list<ref_forwarding_type> simple_type_params;

  // FIXME: should be location of 'new'.
  location where = peek ();

  // We might see type parameters for a generic invocation.
  std::list<ref_forwarding_type> actual_params;
  bool require_class = false;
  if (peek () == TOKEN_LESS_THAN)
    {
      actual_params = actual_type_parameters ();
      require_class = true;
    }

  if (primary_dot_new)
    {
      simple_name = identifier ();
      // We can also have type parameters after the simple name.
      if (peek () == TOKEN_LESS_THAN)
	simple_type_params = actual_type_parameters ();
    }
  else
    {
      int lt_depth = 0;
      ctype = type_name (lt_depth);
      assert (lt_depth == 0);
    }

  ref_expression expr;

  // Note that the syntax 'primary.new Name() { }' is valid, even
  // though one of the grammars rejects it.  In this case Name is a
  // type name, not a simple name.  We don't really handle this here,
  // since in this case we must search the primary's class for the
  // member type named Name; instead we handle this when resolving the
  // 'new'.
  token &t = peek ();
  if (t == TOKEN_OPEN_PAREN || primary_dot_new || require_class)
    {
      std::list<ref_expression> args (arguments ());
      ref_class anon;
      if (peek () == TOKEN_OPEN_BRACE)
	{
	  // Anonymous class.
	  anon = new model_class (where);
	  anon->set_anonymous ();
	  anon->set_superclass (ctype);
	  // Note that we don't use the set_containing_class helper
	  // function, as that does error checks which aren't needed
	  // for anonymous classes.
	  anon->set_declaring_class (enclosing_class ());
	  anon->set_compilation_unit (unit);
	  anon->implicit_modifier (ACC_FINAL);
	  class_body (anon);
	  ctype = new model_forwarding_resolved (anon->get_location (), anon);
	}
      ref_new nexpr;

      if (primary_dot_new)
	{
	  ref_new_primary pnew;
	  if (actual_params.empty ())
	    pnew = new model_new_primary (where);
	  else
	    pnew = new model_generic_new_primary (where, actual_params);
	  pnew->set_simple_name (simple_name);
	  pnew->set_simple_type_parameters (simple_type_params);
	  nexpr = pnew;
	}
      else
	{
	  if (actual_params.empty ())
	    nexpr = new model_new (where);
	  else
	    nexpr = new model_generic_new (where, actual_params);
	  nexpr->set_class (ctype);
	}
      nexpr->set_arguments (args);
      if (anon)
	nexpr->set_anonymous (anon);
      expr = nexpr;
    }
  else if (t == TOKEN_OPEN_BRACKET)
    {
      assert (actual_params.empty ());
      ref_new_array newa (new model_new_array (where));
      while (peek () == TOKEN_OPEN_BRACKET && peek1 () != TOKEN_CLOSE_BRACKET)
	{
	  assume (TOKEN_OPEN_BRACKET);
	  ref_expression e (expression ());
	  newa->add (e);
	  require (TOKEN_CLOSE_BRACKET);
	}
      ctype = brackets_opt (ctype);
      newa->set_element_type (ctype);

      if (peek () == TOKEN_OPEN_BRACE)
	newa->set_initializer (array_initializer (ctype));

      expr = newa;
    }
  else
    throw syntax_error (t, "%<(%> or %<[%> expected");

  return expr;
}

// This is just the obvious thing.  We put it in a separate function
// so it will work well with a machine.
ref_expression
parse::type_dot_class ()
{
  ref_forwarding_type the_type (type ());
  require (TOKEN_DOT);
  token &t = require (TOKEN_CLASS);
  return ref_expression (new model_class_ref (t, the_type));
}

// This is just the obvious thing.  We put it in a separate function
// so it will work well with a machine.
ref_expression
parse::class_name_dot_this ()
{
  std::string name = identifier ();
  require (TOKEN_DOT);
  token &t = require (TOKEN_THIS);
  std::list<std::string> fake_qual;
  fake_qual.push_back (name);
  ref_forwarding_type fwdt (new model_forwarding_simple (t, fake_qual));
  return ref_expression (new model_this_outer (t, fwdt));
}

// Access via "super": either field or method.
ref_expression
parse::super_access ()
{
  ref_forwarding_type enclosing;
  if (peek () == TOKEN_IDENTIFIER)
    {
      enclosing = type ();
      require (TOKEN_DOT);
    }

  location where = require (TOKEN_SUPER);
  require (TOKEN_DOT);
  // We might see type parameters here.
  std::list<ref_forwarding_type> type_params;
  bool require_invocation = false;
  if (peek () == TOKEN_LESS_THAN)
    {
      type_params = actual_type_parameters ();
      require_invocation = true;
    }
  std::string ident = identifier ();

  if (require_invocation || peek () == TOKEN_OPEN_PAREN)
    {
      std::list<ref_expression> args (arguments ());
      model_type_qualified_invocation *super;
      if (type_params.empty ())
	super = new model_type_qualified_invocation (where);
      else
	super = new model_generic_type_qualified_invocation (where,
							     type_params);
      super->set_method (ident);
      super->set_arguments (args);
      super->set_class (enclosing);
      super->set_super_call ();
      return super;
    }
  else
    {
      ref_field_ref super (new model_field_ref (where));
      super->set_super ();
      super->set_field (ident);
      super->set_enclosing_class (enclosing);
      return super;
    }

  abort ();
}

// This has a few changes from the JLS to make the implementation
// simpler.  Also it corrects a buglet in the main part of the text.
// It is valid to write 'new type[] { init } [0]'.  Some sub-parts of
// this are farmed out to a couple helper functions.
//
// primary:
//   literal
//   "void" "." "class"
//   "this"
//   "(" expression ")"
//   new-something
//   type "." "class"
//   class-name "." "this"
//   primary "." "new" identifier "(" [ argument-list ] ")" [ class-body ]
//   field-access
//   method-invocation
//   array-access
//
// field-access:
//   primary "." identifier
//   "super" "." identifier
//   class-name "." "super" "." identifier
//
// method-invocation:
//   method-name "(" arguments ")"
//   primary "." [ actual-type-parameters ] identifier "(" arguments ")"
//   "super" "." [ actual-type-parameters ] identifier "(" arguments ")"
//   class-name "." "super" "." [ actual-type-parameters ]
//     identifier "(" arguments ")"
//
// array-access:
//   expression-name "[" expression "]"
//   primary-no-new-array "[" expression "]"
//
// literal:
//   integer-literal
//   | floating-point-literal
//   | boolean-literal
//   | character-literal
//   | string-literal
//   | null-literal
ref_expression
parse::primary ()
{
  ref_expression result;
  token t;			// FIXME

  // Introduce a new scope so the marker lives just as long as we may
  // need it.
  {
    marker mark (our_token_stream);
    t = get ();
    // First do all the easy ones.
    switch (t.get_type ())
      {
      case TOKEN_OPEN_PAREN:
	result = expression ();
	require (TOKEN_CLOSE_PAREN);
	break;

      case TOKEN_THIS:
	{
	  ref_forwarding_type fwdt
	    = new model_forwarding_resolved (t, enclosing_class ());
	  result = new model_this (t, fwdt);
	}
	break;

      case TOKEN_BYTE:
      case TOKEN_SHORT:
      case TOKEN_CHAR:
      case TOKEN_INT:
      case TOKEN_LONG:
      case TOKEN_FLOAT:
      case TOKEN_DOUBLE:
      case TOKEN_BOOLEAN:
	{
	  ref_forwarding_type basic
	    = new model_forwarding_resolved (t,
					     get_primitive_type (t.get_type ()));
	  basic = brackets_opt (basic);
	  require (TOKEN_DOT);
	  require (TOKEN_CLASS);
	  result = new model_class_ref (t, basic);
	}
	break;

      case TOKEN_VOID:
	require (TOKEN_DOT);
	require (TOKEN_CLASS);
	result = new model_class_ref (t, primitive_void_type);
	break;

      case TOKEN_TRUE:
      case TOKEN_FALSE:
	result = new model_boolean_literal (t, t == TOKEN_TRUE);
	break;

      case TOKEN_NULL:
	result = new model_null_literal (t);
	break;

      case TOKEN_DECIMAL_INT_LIT:
	// Check that we haven't seen the most negative value.
	{
	  ref_int_literal ilit = t;
	  // Just keep going on error here.
	  long long v = jint (ilit->value ());
	  if (v == MIN_INT)
	    std::cerr << syntax_error (t, "%<int%> literal only "
				       "valid as operand of unary %<-%>");
	  result = ilit;
	  break;
	}

      case TOKEN_DECIMAL_LONG_LIT:
	// Check that we haven't seen the most negative value.
	{
	  ref_long_literal llit = t;
	  // Just keep going on error here.
	  long long v = jlong (llit->value ());
	  if (v == (long long) MIN_LONG)
	    std::cerr << syntax_error (t, "%<long%> literal only "
				       "valid as operand of unary %<-%>");
	  result = llit;
	  break;
	}

      case TOKEN_CHAR_LIT:
      case TOKEN_INT_LIT:
      case TOKEN_LONG_LIT:
      case TOKEN_FLOAT_LIT:
      case TOKEN_DOUBLE_LIT:
      case TOKEN_STRING_LIT:
	// For literals, the model object is stored in the token itself.
	result = t;
	break;

      case TOKEN_NEW:
	result = new_something ();
	break;

      default:
	{
	  mark.backtrack ();
	  state_machine<ref_expression> machine (this);
	  result
	    = machine.run (&parse::type_dot_class,
			   &parse::class_name_dot_this,
			   &parse::super_access,
			   (state_machine<ref_expression>::parse_function) NULL);
	}
	break;
      }

    if (! result)
      {
	// What's left are variable or field references, or method
	// invocations.  We use a single kind of object to represent
	// all of these, with resolution later.  We do this because a
	// qualified-identifier could have an arbitrary number of
	// field references in it.
	mark.backtrack ();

	std::list<std::string> qual (qualified_identifier ());
	std::list<ref_forwarding_type> type_params;
	if (peek () == TOKEN_DOT && peek1 () == TOKEN_LESS_THAN)
	  {
	    assume (TOKEN_DOT);
	    // Generic invocation with explicit type parameters.
	    type_params = actual_type_parameters ();
	    // This is a goofy hack: fetch the method name and push it
	    // on the identifier list.  FIXME: really we ought to make
	    // the model a little closer to the source here.
	    qual.push_back (identifier ());
	  }

	ref_memberref_forward fwd (new model_memberref_forward (t, qual));
	if (! type_params.empty () || peek () == TOKEN_OPEN_PAREN)
	  {
	    fwd->set_arguments (arguments ());
	    fwd->set_type_parameters (type_params);
	  }
	result = fwd;
      }
  }

  // Now handle trailing field references, new invocations, array
  // accesses, and method calls.
  bool keep_going = true;
  while (keep_going)
    {
      t = peek ();
      switch (t.get_type ())
	{
	case TOKEN_DOT:
	  {
	    // We might see "primary . super", in which case we just
	    // return the primary and let the caller worry about the
	    // rest.
	    if (peek1 () == TOKEN_SUPER)
	      {
		keep_going = false;
		break;
	      }

	    assume (TOKEN_DOT);
	    if (peek () == TOKEN_NEW)
	      {
		// primary "." "new" ...
		assume (TOKEN_NEW);
		ref_new newexpr = ref_new (new_something (true));
		newexpr->set_expression (result);
		result = newexpr;
	      }
	    else
	      {
		if (peek () == TOKEN_LESS_THAN)
		  {
		    // Generic invocation with explicit type
		    // parameters.
		    std::list<ref_forwarding_type> type_params
		      = actual_type_parameters ();
		    std::string name = identifier ();
		    ref_method_invocation inv
		      = new model_generic_method_invocation (t, type_params);
		    inv->set_method (name);
		    inv->set_expression (result);
		    inv->set_arguments (arguments ());
		    result = inv;
		  }
		else
		  {
		    std::string name = identifier ();
		    if (peek () == TOKEN_OPEN_PAREN)
		      {
			ref_method_invocation inv
			  = new model_method_invocation (t);
			inv->set_method (name);
			inv->set_expression (result);
			inv->set_arguments (arguments ());
			result = inv;
		      }
		    else
		      {
			// Field reference.
			ref_field_ref fref (new model_field_ref (t));
			fref->set_field (name);
			fref->set_expression (result);
			result = fref;
		      }
		  }
	      }
	  }
	  break;

	case TOKEN_OPEN_BRACKET:
	  {
	    assume (TOKEN_OPEN_BRACKET);
	    ref_array_ref ae (new model_array_ref (t));
	    ae->set_array (result);
	    ae->set_index (expression ());
	    require (TOKEN_CLOSE_BRACKET);
	    result = ae;
	  }
	  break;

	default:
	  keep_going = false;
	  break;
	}
    }

  return result;
}

// postfix-expression:
//   primary
//   | expression-name
//   | post-increment-expression
//   | post-decrement-expression
// post-increment-expression:
//   postfix-expression "++"
// post-decrement-expression:
//   postfix-expression "--"
ref_expression
parse::postfix_expression ()
{
  // We can omit 'expression-name', since that is covered by primary
  // in the 'identifier' part of the rule.
  ref_expression result (primary ());
  while (true)
    {
      token &t = peek ();
      if (t == TOKEN_INCREMENT)
	{
	  assume (TOKEN_INCREMENT);
	  result = new model_postfix_plusplus (t, result);
	}
      else if (t == TOKEN_DECREMENT)
	{
	  assume (TOKEN_DECREMENT);
	  result = new model_postfix_minusminus (t, result);
	}
      else
	break;
    }
  return result;
}

// unary-expression-not-plus-minus:
//   postfix-expression
//   | "~" unary-expression
//   | "!" unary-expression
//   | cast-expression
// cast-expression:
//   "(" primitive-type ")" unary-expression
//   "(" reference-type ")" unary-expression-not-plus-minus
ref_expression
parse::unary_expression_not_plus_minus ()
{
  ref_unary op;
  token &t = peek ();
  switch (t.get_type ())
    {
    case TOKEN_BITWISE_NOT:
      assume (TOKEN_BITWISE_NOT);
      op = new model_bitwise_not (t);
      op->set_expression (unary_expression ());
      break;
    case TOKEN_LOGICAL_NOT:
      assume (TOKEN_LOGICAL_NOT);
      op = new model_logical_not (t);
      op->set_expression (unary_expression ());
      break;
    case TOKEN_OPEN_PAREN:
      // First try a cast-expression.  If that fails, try
      // postfix-expression.
      {
	marker mark (our_token_stream);
	try
	  {
	    assume (TOKEN_OPEN_PAREN);
	    ref_forwarding_type dest (type ());
	    require (TOKEN_CLOSE_PAREN);
	    op = new model_cast (t, dest);

	    model_forwarding_resolved *fwr
	      = dynamic_cast<model_forwarding_resolved *> (dest.get ());
	    if (fwr && fwr->type ()->primitive_p ())
	      op->set_expression (unary_expression ());
	    else
	      op->set_expression (unary_expression_not_plus_minus ());
	    break;
	  }
	catch (syntax_error &)
	  {
	    mark.backtrack ();
	  }
	// Fall through.
      }
    default:
      return postfix_expression ();
    }
  return op;
}

// unary-expression:
//   pre-increment-expression
//   | pre-decrement-expression
//   | "+" unary-expression
//   | "-" unary-expression
//   | unary-expression-not-plus-minus
// pre-increment-expression:
//   "++" unary-expression
// pre-decrement-expression:
//   "--" unary-expression
ref_expression
parse::unary_expression ()
{
  ref_unary op;
  token &t = peek ();
  switch (t.get_type ())
    {
    case TOKEN_INCREMENT:
      op = new model_prefix_plusplus (t);
      break;
    case TOKEN_DECREMENT:
      op = new model_prefix_minusminus (t);
      break;
    case TOKEN_PLUS:
      op = new model_prefix_plus (t);
      break;
    case TOKEN_MINUS:
      op = new model_prefix_minus (t);
      break;
    default:
      return unary_expression_not_plus_minus ();
    }

  // It is simplest to just recurse.
  assume (t.get_type ());
  if (t == TOKEN_MINUS)
    {
      token &t2 = peek ();
      // This special case circumvents the value checking in the
      // ordinary case, allowing the largest negative value through.
      if (t2 == TOKEN_DECIMAL_INT_LIT || t2 == TOKEN_DECIMAL_LONG_LIT)
	{
	  assume (t2.get_type ());
	  op->set_expression (ref_expression (t2));
	  return op;
	}
    }
  op->set_expression (unary_expression ());
  return op;
}

// multiplicative-expression:
//   unary-expression
//   | multiplicative-expression "*" unary-expression
//   | multiplicative-expression "/" unary-expression
//   | multiplicative-expression "%" unary-expression
ref_expression
parse::multiplicative_expression ()
{
  recursion_machine<
    &parse::unary_expression,
    token_matcher<
      TOKEN_MULTIPLY, model_mult,
      TOKEN_DIVIDE, model_div,
      TOKEN_MOD, model_mod>
    > machine (this);
  return machine.run ();
}

// additive-expression:
//   multiplicative-expression
//   | additive-expression "+" multiplicative-expression
//   | additive-expression "-" multiplicative-expression
ref_expression
parse::additive_expression ()
{
  recursion_machine<
    &parse::multiplicative_expression,
    token_matcher<TOKEN_PLUS, model_plus, TOKEN_MINUS, model_minus>
    > machine (this);
  return machine.run ();
}

// shift-expression:
//   additive-expression
//   | shift-expression "<<" additive-expression
//   | shift-expression ">>" additive-expression
//   | shift-expression ">>>" additive-expression
ref_expression
parse::shift_expression ()
{
  recursion_machine<
    &parse::additive_expression,
    token_matcher<
      TOKEN_LEFT_SHIFT, model_left_shift,
      TOKEN_RIGHT_SHIFT, model_right_shift,
      TOKEN_UNSIGNED_RIGHT_SHIFT, model_unsigned_right_shift>
    > machine (this);
  return machine.run ();
}

// relational-expression:
//   shift-expression
//   | relational-expression "<" shift-expression
//   | relational-expression "<=" shift-expression
//   | relational-expression ">" shift-expression
//   | relational-expression ">=" shift-expression
//   | relational-expression "instanceof" type
ref_expression
parse::relational_expression ()
{
  ref_expression shift (shift_expression ());
  while (true)
    {
      ref_binary op;
      token &t = peek ();
      switch (t.get_type ())
	{
	case TOKEN_LESS_THAN:
	  {
	    assume (TOKEN_LESS_THAN);
	    op = new model_lessthan (t);
	    op->set_lhs (shift);
	    op->set_rhs (shift_expression ());
	    shift = op;
	  }
	  break;

	case TOKEN_LESS_EQUALS:
	  {
	    assume (TOKEN_LESS_EQUALS);
	    op = new model_lessthanequal (t);
	    op->set_lhs (shift);
	    op->set_rhs (shift_expression ());
	    shift = op;
	  }
	  break;

	case TOKEN_GREATER_THAN:
	  {
	    assume (TOKEN_GREATER_THAN);
	    op = new model_greaterthan (t);
	    op->set_lhs (shift);
	    op->set_rhs (shift_expression ());
	    shift = op;
	  }
	  break;

	case TOKEN_GREATER_EQUALS:
	  {
	    assume (TOKEN_GREATER_EQUALS);
	    op = new model_greaterthanequal (t);
	    op->set_lhs (shift);
	    op->set_rhs (shift_expression ());
	    shift = op;
	  }
	  break;

	case TOKEN_INSTANCEOF:
	  {
	    assume (TOKEN_INSTANCEOF);
	    ref_forwarding_type inst (type ());
	    shift = new model_instanceof (t, shift, inst);
	  }
	  break;

	default:
	  return shift;
	}
    }
}

// equality-expression:
//   relational-expression
//   | equality-expression "==" relational-expression
//   | equality-expression "!=" relational-expression
ref_expression
parse::equality_expression ()
{
  recursion_machine<
    &parse::relational_expression,
    token_matcher<TOKEN_EQUALS, model_equal, TOKEN_NOT_EQUALS, model_notequal>
    > machine (this);
  return machine.run ();
}

// and-expression:
//   equality-expression
//   | and-expression "&" equality-expression
ref_expression
parse::and_expression ()
{
  recursion_machine<
    &parse::equality_expression,
    token_matcher<TOKEN_BITWISE_AND, model_and>
    > machine (this);
  return machine.run ();
}

// exclusive-or-expression:
//   and-expression
//   | exclusive-or-expression "^" and-expression
ref_expression
parse::exclusive_or_expression ()
{
  recursion_machine<
    &parse::and_expression,
    token_matcher<TOKEN_BITWISE_XOR, model_xor>
    > machine (this);
  return machine.run ();
}

// inclusive-or-expression:
//   exclusive-or-expression
//   | inclusive-or-expression "|" exclusive-or-expression
ref_expression
parse::inclusive_or_expression ()
{
  recursion_machine<
    &parse::exclusive_or_expression,
    token_matcher<TOKEN_BITWISE_OR, model_or>
    > machine (this);
  return machine.run ();
}

// conditional-and-expression:
//   inclusive-or-expression
//   | conditional-and-expression "&&" inclusive-or-expression
ref_expression
parse::conditional_and_expression ()
{
  recursion_machine<
    &parse::inclusive_or_expression,
    token_matcher<TOKEN_LOGICAL_AND, model_land>
    > machine (this);
  return machine.run ();
}

// conditional-or-expression:
//   conditional-and-expression
//   | conditional-or-expression "||" conditional-and-expression
ref_expression
parse::conditional_or_expression ()
{
  recursion_machine<
    &parse::conditional_and_expression,
    token_matcher<TOKEN_LOGICAL_OR, model_lor>
    > machine (this);
  return machine.run ();
}

// conditional-expression:
//   conditional-or-expression
//   | conditional-or-expression "?" expression ":" conditional-expression
ref_expression
parse::conditional_expression ()
{
  ref_expression expr = conditional_or_expression ();
  if (peek () == TOKEN_QUERY)
    {
      location w = assume (TOKEN_QUERY);
      ref_expression true_branch = expression ();
      require (TOKEN_COLON);
      ref_expression false_branch = conditional_expression ();
      expr = new model_conditional (w, expr, true_branch, false_branch);
    }
  return expr;
}

// left-hand-side:
//   name | field-access | array-access
// However, we just use 'primary' as we don't have separate rules for
// the above.  Then we detect invalid assignments during semantic
// analysis.
ref_expression
parse::left_hand_side ()
{
  return primary ();
}

// assignment:
//   left-hand-side assignment-operator assignment-expression
ref_expression
parse::assignment ()
{
  ref_expression lhs (left_hand_side ());
  token &t = get ();
  if (! assignment_op_p (t.get_type ()))
    throw syntax_error (t, "assignment operator expected");
  ref_expression rhs (assignment_expression ());
  ref_assignment op ((ref_assignment) t);
  op->set_lhs (lhs);
  op->set_rhs (rhs);
  return op;
}

// assignment-expression:
//   conditional-expression
//   | assignment
ref_expression
parse::assignment_expression ()
{
  state_machine<ref_expression> machine (this);
  // Note that order matters here.
  ref_expression result
    = machine.run (&parse::assignment,
		   &parse::conditional_expression,
		   (state_machine<ref_expression>::parse_function) NULL);
  if (! result)
    throw syntax_error (peek (), "assignment or conditional "
			"expression expected");
  return result;
}

// statement-expression:
//   assignment
//   | pre-increment-expression
//   | pre-decrement-expression
//   | post-increment-expression
//   | post-decrement-expression
//   | method-invocation
//   | class-instance-creation-expression
ref_expression
parse::statement_expression ()
{
  // It is simplest to just allow any expression, and then check the
  // semantics later.
  ref_expression result = expression ();

  // We used to do this during semantic analysis, but with generics we
  // have to do it now.  Otherwise we incorrectly parse something like
  // "Enumeration<Object> name;"
  model_expression *expr = result.get ();
  if (! dynamic_cast<model_assignment *> (expr)
      && ! dynamic_cast<model_prefix_plusplus *> (expr)
      && ! dynamic_cast<model_prefix_minusminus *> (expr)
      && ! dynamic_cast<model_postfix_plusplus *> (expr)
      && ! dynamic_cast<model_postfix_minusminus *> (expr)
      && ! dynamic_cast<model_new *> (expr)
      && ! dynamic_cast<model_invocation_base *> (expr))
    {
      model_memberref_forward *base
	= dynamic_cast<model_memberref_forward *> (expr);
      if (! base || ! base->call_p ())
	throw syntax_error (expr->get_location (),
			    "expression not valid as a statement");
    }

  return result;
}

// constant-expression:
//   expression
// expression: 
//   assignment-expression
ref_expression
parse::expression ()
{
  return assignment_expression ();
}



// brackets-opt:
//   { "[" "]" }
ref_forwarding_type
parse::brackets_opt (ref_forwarding_type t)
{
  while (peek () == TOKEN_OPEN_BRACKET)
    {
      assume (TOKEN_OPEN_BRACKET);
      require (TOKEN_CLOSE_BRACKET);
      if (t)
	t = t->array ();
    }
  return t;
}

// primitive-type:
//   boolean | byte | short | char | int | long | float | double
ref_forwarding_type
parse::primitive_type ()
{
  token &t = get ();
  if (! basic_type_p (t.get_type ()))
    throw syntax_error (t, "primitive type expected");
  ref_forwarding_type pt
    = new model_forwarding_resolved (t, get_primitive_type (t.get_type ()));
  return pt;
}

// type-name:
//   ( basic-type | reference-type )
ref_forwarding_type
parse::type_name (int &lt_depth)
{
  token t = peek ();		// FIXME
  if (basic_type_p (t.get_type ()))
    {
      get ();
      // Note that other parts of the parser know that a primitive
      // type will be returned as a model_forwarding_resolved.
      return
	new model_forwarding_resolved (t,
				       get_primitive_type (t.get_type ()));
    }

  // RESULT stores our ultimate result, but it might also store some
  // intermediate result if we see parameterizations.
  ref_forwarding_type result;
  std::list<std::string> ids;
  location here = t;

  while (true)
    {
      ids.push_back (identifier ());
      token t = peek ();	// FIXME

      // '<' means we're starting the parameters of a generic type.
      if (t == TOKEN_LESS_THAN)
	{
	  assume (TOKEN_LESS_THAN);
	  int saved_depth = ++lt_depth;

	  ref_forwarding_type inner
	    = new model_forwarding_inner (here, ids, result);
	  ref_forwarding_parameterized param
	    = new model_forwarding_parameterized (here, inner);
	  ids.clear ();

	  // Fetch each type parameter.
	  while (true)
	    {
	      if (peek () == TOKEN_QUERY)
		{
		  // We have a wildcard parameter.
		  t = require (TOKEN_QUERY);
		  ref_wildcard w = new model_wildcard (t);
		  t = peek ();
		  if (t == TOKEN_SUPER || t == TOKEN_EXTENDS)
		    {
		      assume (t.get_type ());
		      w->set_bound (counted_type (lt_depth));
		      if (t == TOKEN_SUPER)
			w->set_super ();
		    }
		  // The forwarder owns the type.
		  // FIXME this seems a bit wrong, but it isn't clear
		  // where else we could put it.
		  param->add (new model_forwarding_owned (w->get_location (),
							  w));
		}
	      else
		{
		  // Plain old type parameter.
		  param->add (counted_type (lt_depth));
		}

	      // If we saw '>>' or '>>>', we exit the loop.
	      if (saved_depth != lt_depth)
		break;
	      // If we saw a ',' then we continue gathering
	      // parameters.
	      t = peek ();
	      if (t != TOKEN_COMMA)
		break;
	      assume (TOKEN_COMMA);
	    }

	  result = param;

	  // Perhaps the recursive call saw '>>' or '>>>'.  In this
	  // case we must return.
	  if (saved_depth != lt_depth)
	    break;

	  if (t == TOKEN_RIGHT_SHIFT && lt_depth > 1)
	    {
	      assume (TOKEN_RIGHT_SHIFT);
	      lt_depth -= 2;
	      break;
	    }
	  else if (t == TOKEN_UNSIGNED_RIGHT_SHIFT && lt_depth > 2)
	    {
	      assume (TOKEN_UNSIGNED_RIGHT_SHIFT);
	      lt_depth -= 3;
	      break;
	    }

	  require (TOKEN_GREATER_THAN);
	  --lt_depth;

	  // Keep looping.
	  t = peek ();
	}

      if (t != TOKEN_DOT || peek1 () != TOKEN_IDENTIFIER)
	break;
      assume (TOKEN_DOT);
    }

  if (! ids.empty ())
    {
      if (result)
	result = new model_forwarding_inner (here, ids, result);
      else
	result = new model_forwarding_simple (here, ids);
    }

  assert (result);
  return result;
}

ref_forwarding_type
parse::counted_type (int &lt_depth)
{
  int saved_depth = lt_depth;
  ref_forwarding_type result (type_name (lt_depth));
  // Don't look for [] if we just closed more than one <>.
  if (saved_depth == lt_depth)
    result = brackets_opt (result);
  return result;
}

// type:
//   type-name brackets-opt
ref_forwarding_type 
parse::type ()
{
  int depth = 0;
  ref_forwarding_type result = counted_type (depth);
  assert (depth == 0);
  return result;
}

// type-list:
//   type { "," type }
std::list<ref_forwarding_type>
parse::type_list ()
{
  std::list<ref_forwarding_type> result;
  while (true)
    {
      result.push_back (type ());
      if (peek () != TOKEN_COMMA)
	break;
      assume (TOKEN_COMMA);
    }
  return result;
}

// This isn't mentioned as such in the JLS.  Instead it is similar to
// non-wildcard type parameters, but we do the wildcard checking
// during semantic analysis.
//
// actual-type-parameters:
//  "<" type { "," type } ">"
std::list<ref_forwarding_type>
parse::actual_type_parameters ()
{
  std::list<ref_forwarding_type> result;
  int depth = 1;
  require (TOKEN_LESS_THAN);
  while (true)
    {
      result.push_back (counted_type (depth));
      if (depth == 0)
	break;
      if (peek () != TOKEN_COMMA)
	break;
      assume (TOKEN_COMMA);
    }

  if (depth != 0)
    {
      // FIXME: not sure if this assert is correct.
      assert (depth == 1);
      require (TOKEN_GREATER_THAN);
    }

  return result;
}

// variable-declarator-id:
//   identifier brackets-opt
// This accepts a type parameter because trailing brackets will cause
// us to construct a new type; we will update the argument in this
// case.
ref_variable_decl
parse::variable_declarator_id (ref_forwarding_type &current_type,
			       bool is_parameter, bool is_catch)
{
  location where = peek ();
  std::string name (identifier ());
  current_type = brackets_opt (current_type);
  ref_variable_decl result;
  if (is_catch)
    result = new model_catch_decl (where, name, current_type,
				   enclosing_class ());
  else if (is_parameter)
    result = new model_parameter_decl (where, name, current_type,
				       enclosing_class ());
  else
    result = new model_variable_decl (where, name, current_type,
				      enclosing_class ());
  return result;
}

// This is a convenience subroutine used to handle "final" and
// attributes for variable and formal parameter declarations.
void
parse::final_or_attributes (bool &is_final, std::list<ref_annotation> &annos)
{
  while (true)
    {
      token &t = peek ();
      if (t == TOKEN_FINAL)
	{
	  // FIXME: if (is_final) throw ...
	  assume (TOKEN_FINAL);
	  is_final = true;
	}
      else if (t == TOKEN_AT)
	annos.push_back (annotation ());
      else
	break;
    }
}

// formal-parameter:
//   [ "final" ] type variable-declarator-id
ref_variable_decl
parse::formal_parameter (bool dots_ok, bool *dots_result, bool is_parameter,
			 bool is_catch)
{
  bool final = false;
  std::list<ref_annotation> annos;
  final_or_attributes (final, annos);

  ref_forwarding_type the_type = type ();

  if (dots_ok && peek () == TOKEN_DOTDOTDOT)
    {
      assume (TOKEN_DOTDOTDOT);
      *dots_result = true;
      the_type = the_type->array ();
    }

  ref_variable_decl result (variable_declarator_id (the_type, is_parameter,
						    is_catch));
  if (final)
    result->set_final ();
  result->set_annotations (annos);
  return result;
}

// formal-parameters:
//   "(" [ formal-parameter { "," formal-parameter } ] ")"
std::list<ref_variable_decl>
parse::formal_parameter_list (bool dots_ok, bool *dots_result)
{
  require (TOKEN_OPEN_PAREN);

  std::list<ref_variable_decl> result;

  *dots_result = false;
  if (peek () != TOKEN_CLOSE_PAREN)
    {
      while (true)
	{
	  result.push_back (formal_parameter (dots_ok, dots_result, true));
	  if (peek () != TOKEN_COMMA)
	    break;
	  if (*dots_result)
	    throw syntax_error (peek (),
				"%<...%> valid only in last parameter "
				"of parameter list");
	  assume (TOKEN_COMMA);
	}
    }

  require (TOKEN_CLOSE_PAREN);
  return result;
}



// assert-statement:
//   "assert" expression [ ":" expression ] ";"
ref_assert 
parse::assert_statement ()
{
  token &t = assume (TOKEN_ASSERT);
  ref_assert result (new model_assert (t));
  result->set_expression (expression ());

  if (peek () == TOKEN_COLON)
    {
      assume (TOKEN_COLON);
      result->set_message (expression ());
    }

  require (TOKEN_SEMI);

  // This is a somewhat ugly hack.  We need to add some synthetic
  // members to the class if an 'assert' is seen.  However, if we wait
  // until resolution, then there is not really a good time to do it.
  // So, it is simplest to note the presence of an assert during
  // parsing.
  enclosing_class ()->add_assert_members ();

  return result;
}

// continue-statement:
//   "continue" { identifier } ";"
// break-statement:
//   "break" { identifier } ";"
template<token_value ttype, typename M>
ref_stmt
parse::loop_statement ()
{
  location where = assume (ttype);
  owner<M> result (new M (where));
  ref_stmt loc;
  std::string id;
  if (peek () == TOKEN_IDENTIFIER)
    id = identifier ();
  require (TOKEN_SEMI);

  Ilabel *dest = find_lexically_enclosing_loop (result.get (),
						ttype, id);
  result->set_target (dest);
  return result;
}

// return-statement:
//   "return" { expression } ";"
// throw-statement:
//   "throw" expression ";"
template<token_value ttype, typename M>
ref_stmt
parse::control_statement ()
{
  location where = assume (ttype);
  owner<M> result (new M (where));
  if (ttype == TOKEN_THROW || peek () != TOKEN_SEMI)
    result->set_expression (expression ());
  require (TOKEN_SEMI);
  return result;
}

// This isn't exactly the specification for this method, as it was
// more useful to parse the initial statement-expression here as well.
//
// more-statement-expressions:
//   { "," statement-expression }
ref_stmt
parse::more_statement_expressions ()
{
  std::list<ref_stmt> all;
  location block_w = peek ();
  location loop_w = block_w;
  while (true)
    {
      ref_stmt stmt (new model_expression_stmt (loop_w, expression ()));
      all.push_back (stmt);
      if (peek () != TOKEN_COMMA)
	break;
      loop_w = assume (TOKEN_COMMA);
    }
  return ref_stmt (new model_block (block_w, all));
}

// for-init:
//   statement-expression more-statement-expressions
//   | [ "final" ] type variable-declarators
ref_stmt
parse::for_init ()
{
  state_machine<ref_stmt> machine (this);
  ref_stmt result
    = machine.run (&parse::local_variable_declaration,
		   &parse::more_statement_expressions,
		   (state_machine<ref_expression>::parse_function) NULL);
  if (! result)
    throw syntax_error (peek (), "variable declaration or statement "
			"expression expected");
  return result;
}

// for-update:
//   statement-expression more-statement-expressions
ref_stmt
parse::for_update ()
{
  // This is just a wrapper due to the way we've implemented
  // more_statement_expressions.
  return more_statement_expressions ();
}

// for-statement:
//   "for" "(" for-init-opt ";" expression-opt ";" for-update-opt ")"
//   + statement
// for-statement-no-short-if:
//   "for" "(" for-init-opt ";" expression-opt ";" for-update-opt ")"
//   + statement-no-short-if
// enhanced-for-statement:
//   "for" "(" type identifier ":" expression ")" statement
// enhanced-for-statement-no-short-if:
//   "for" "(" type identifier ":" expression ")" statement-no-short-if
ref_for 
parse::for_statement ()
{
  location where = assume (TOKEN_FOR);
  require (TOKEN_OPEN_PAREN);

  ref_for result;

  marker mark (our_token_stream);
  try
    {
      model_for *mod = new model_for (where);
      result = mod;

      if (peek () != TOKEN_SEMI)
	mod->set_initializer (for_init ());
      require (TOKEN_SEMI);

      if (peek () != TOKEN_SEMI)
	mod->set_expression (expression ());
      require (TOKEN_SEMI);

      if (peek () != TOKEN_CLOSE_PAREN)
	mod->set_update (for_update ());
    }
  catch (syntax_error &s)
    {
      // Just rethrow if we can't continue parsing.  That will give
      // the best error message.  Note that there is a weird situation
      // here where we might see 'for (; ...error)', and then try to
      // re-parse it as a new-style 'for' even though that is a waste
      // of time.  We could add a flag for this case if desired.
      if (! global->get_compiler ()->feature_enhanced_for ())
	throw s;
      mark.backtrack ();
      ref_variable_decl vardecl (formal_parameter (false, NULL));
      require (TOKEN_COLON);
      ref_expression expr = expression ();
      result = new model_for_enhanced (where, vardecl, expr);
    }

  require (TOKEN_CLOSE_PAREN);

  // The scope has to be in place before parsing the body.
  stack_temporary<Ilabel *> push_scope (label_stack, result.get ());
  result->set_body (statement ());

  return result;
}

// while-statement:
//   "while" "(" expression ")" statement
// while-statement-no-short-if:
//   "while" "(" expression ")" statement-no-short-if
ref_while 
parse::while_statement ()
{
  location where = assume (TOKEN_WHILE);
  require (TOKEN_OPEN_PAREN);

  ref_while result (new model_while (where));
  stack_temporary<Ilabel *> push_scope (label_stack, result.get ());
  result->set_expression (expression ());

  require (TOKEN_CLOSE_PAREN);

  result->set_body (statement ());
  return result;
}

// synchronized-statement:
//   "synchronized" "(" expression ")" statement
ref_synchronized 
parse::synchronized_statement ()
{
  location where = assume (TOKEN_SYNCHRONIZED);
  require (TOKEN_OPEN_PAREN);

  ref_synchronized result (new model_synchronized (where));
  result->set_expression (expression ());

  require (TOKEN_CLOSE_PAREN);

  ref_block b (new model_block (peek ()));
  result->set_body (block (b));
  return result;
}

// empty-statement:
//   ";"
ref_empty 
parse::empty_statement ()
{
  location w = assume (TOKEN_SEMI);
  return new model_empty (w);
}

// labeled-statement:
//   identifier ":" statement
// labeled-statement-no-short-if:
//   identifier ":" statement-no-short-if
ref_label 
parse::labeled_statement ()
{
  ref_label label (new model_label (peek ()));
  label->set_name (identifier ());

  // Look for clashes in this scope.
  for (std::deque<Ilabel *>::reverse_iterator i = label_stack.rbegin ();
       i != label_stack.rend ();
       ++i)
    {
      // Might see a class boundary.
      if (*i == NULL)
	break;

      if ((*i)->match_label_p (TOKEN_BREAK, label->get_name ()))
	{
	  std::cerr << label->error ("duplicate label named %1")
	    % label->get_name ();
	  model_element *elt = assert_cast<model_element *> (*i);
	  std::cerr << elt->error ("original declaration is here");
	  break;
	}
    }

  // Set up a new scope for the enclosed statement.  Do this after
  // setting the name to allow easier error checking.
  stack_temporary<Ilabel *> push_scope (label_stack, label.get ());

  require (TOKEN_COLON);
  label->set_statement (statement ());
  return label;
}

// do-statement:
//   "do" statement "while" "(" expression ")" ";"
ref_do 
parse::do_statement ()
{
  location where = require (TOKEN_DO);
  ref_do result (new model_do (where));
  stack_temporary<Ilabel *> push_scope (label_stack, result.get ());

  result->set_statement (statement ());
  require (TOKEN_WHILE);
  require (TOKEN_OPEN_PAREN);
  result->set_expression (expression ());
  require (TOKEN_CLOSE_PAREN);
  require (TOKEN_SEMI);

  return result;
}



// try-statement:
//   "try" block catches
//   "try" block catches-opt finally
// catches:
//   { catch-clause }
// catch-clause:
//   "catch" "(" formal-parameter ")" block
// finally:
//   "finally" block
ref_try 
parse::try_statement ()
{
  token &t = assume (TOKEN_TRY);
  ref_try result (new model_try (t));
  result->set_block (block (new model_block (t)));  // FIXME location

  std::list<ref_catch> catchers;
  while (peek () == TOKEN_CATCH)
    {
      location w = assume (TOKEN_CATCH);
      require (TOKEN_OPEN_PAREN);
      ref_catch c (new model_catch (w));
      c->set_formal_parameter (formal_parameter (false, NULL, false, true));
      w = require (TOKEN_CLOSE_PAREN);
      c->set_block (block (new model_block (w)));  // FIXME location
      catchers.push_back (c);
    }
  result->set_catchers (catchers);

  if (peek () == TOKEN_FINALLY)
    {
      location w = require (TOKEN_FINALLY);
      result->set_finally (block (new model_block (w)));  // FIXME location
    }
  else if (catchers.empty ())
    throw syntax_error (peek (), "expected %<catch%> or %<finally%>");

  return result;
}



// if-then-statement:
//   "if" "(" expression ")" statement
// if-then-else-statement:
//   "if" "(" expression ")" statement-no-short-if "else" statement
// if-then-else-statement-no-short-if:
//   "if" "(" expression ")" statement-no-short-if "else" statement-no-short-if
ref_if
parse::if_then_statement ()
{
  location where = assume (TOKEN_IF);
  ref_if result (new model_if (where));

  require (TOKEN_OPEN_PAREN);
  result->set_expression (expression ());
  require (TOKEN_CLOSE_PAREN);

  result->set_true_branch (statement ());

  token &t = peek ();
  // The simple rule is, the inner-most "if" gets the "else".
  // We don't need to handle the no-short-if case specially.
  if (t == TOKEN_ELSE)
    {
      assume (TOKEN_ELSE);
      result->set_false_branch (statement ());
    }

  return result;
}



// switch-block-statement-groups:
//   { switch-block-statement-group }
// switch-block-statement-group:
//   switch-label block-statements
// switch-label:
//   "case" constant-expression ":"
//   | "default" ":"
void
parse::switch_block_statement_groups (const ref_switch &result_switch)
{
  // FIXME
  for (token t = peek (); t == TOKEN_CASE || t == TOKEN_DEFAULT; t = peek ())
    {
      ref_switch_block swb (new model_switch_block (t));
      for (; t == TOKEN_CASE || t == TOKEN_DEFAULT; t = peek ())
	{
	  get ();
	  if (t == TOKEN_CASE)
	    {
	      ref_expression expr (expression ());
	      require (TOKEN_COLON);
	      swb->add (expr);
	    }
	  else
	    {
	      assert (t == TOKEN_DEFAULT);
	      require (TOKEN_COLON);
	      result_switch->set_default (swb.get ());
	    }
	}
      swb->set_statements (block_statements ());
      result_switch->add (swb);
    }
}

// switch-statement:
//   "switch" "(" expression ")" "{" switch-block-statement-groups "}"
ref_switch 
parse::switch_statement ()
{
  location where = assume (TOKEN_SWITCH);
  require (TOKEN_OPEN_PAREN);

  ref_switch result (new model_switch (where));
  result->set_expression (expression ());

  require (TOKEN_CLOSE_PAREN);
  require (TOKEN_OPEN_BRACE);
  stack_temporary<Ilabel *> push_scope (label_stack, result.get ());
  switch_block_statement_groups (result);
  require (TOKEN_CLOSE_BRACE);

  return result;
}

// expression-statement:
//   statement-expression ";"
ref_stmt
parse::expression_statement ()
{
  location where = peek ();
  ref_expression exp = statement_expression ();
  require (TOKEN_SEMI);
  return ref_expression_stmt (new model_expression_stmt (where, exp));
}

// statement:
//   statement-without-trailing-substatement
//   | labeled-statement
//   | if-then-statement
//   | if-then-else-statement
//   | while-statement
//   | for-statement
//
// statement-no-short-if:
//   statement-without-trailing-substatement
//   | labeled-statement-no-short-if
//   | if-then-else-statement-no-short-if
//   | while-statement-no-short-if
//   | for-statement-no-short-if
//
// statement-without-trailing-substatement:
//   block
//   | empty-statement
//   | expression-statement
//   | switch-statement
//   | do-statement
//   | break-statement
//   | continue-statement
//   | return-statement
//   | synchronized-statement
//   | throw-statement
//   | try-statement
//   | assert-statement
ref_stmt 
parse::statement ()
{
  token &t = peek ();

  switch (t.get_type ())
    {
    case TOKEN_OPEN_BRACE:
      return block (new model_block (t));

    case TOKEN_SEMI:
      return empty_statement ();

    case TOKEN_SWITCH:
      return switch_statement ();

    case TOKEN_DO:
      return do_statement ();

    case TOKEN_BREAK:
      return loop_statement<TOKEN_BREAK, model_break> ();

    case TOKEN_CONTINUE:
      return loop_statement<TOKEN_CONTINUE, model_continue> ();

    case TOKEN_RETURN:
      return control_statement<TOKEN_RETURN, model_return> ();

    case TOKEN_SYNCHRONIZED:
      return synchronized_statement ();

    case TOKEN_THROW:
      return control_statement<TOKEN_THROW, model_throw> ();

    case TOKEN_TRY:
      return try_statement ();

    case TOKEN_ASSERT:
      return assert_statement ();

    case TOKEN_IF:
      // Also handles if-then-else.
      return if_then_statement ();

    case TOKEN_WHILE:
      return while_statement ();

    case TOKEN_FOR:
      return for_statement ();

    case TOKEN_IDENTIFIER:
      {
	if (peek1 () == TOKEN_COLON)
	  return labeled_statement ();
	break;
      }

      // Warning avoidance.
    default:
      break;
    }

  // Anything else must be an expression-statement.
  return expression_statement ();
}



// variable-declarator:
//   variable-declarator-id [ "=" variable-initializer ]
ref_variable_decl
parse::variable_declarator (ref_forwarding_type base_type)
{
  // Note that this might change BASE_TYPE.
  ref_variable_decl result (variable_declarator_id (base_type));
  if (peek () == TOKEN_ASSIGN)
    {
      assume (TOKEN_ASSIGN);
      result->set_initializer (variable_initializer (base_type));
    }
  return result;
}

// variable-declarators:
//   variable-declarator { "," variable-declarator }
std::list<ref_variable_decl>
parse::variable_declarators (ref_forwarding_type base_type)
{
  std::list<ref_variable_decl> result;
  while (true)
    {
      result.push_back (variable_declarator (base_type));
      if (peek () != TOKEN_COMMA)
	break;
      assume (TOKEN_COMMA);
    }
  return result;
}

// local-variable-declaration:
//   [ "final" ] type variable-declarators
ref_stmt
parse::local_variable_declaration ()
{
  bool is_final = false;
  std::list<ref_annotation> annos;
  final_or_attributes (is_final, annos);

  ref_forwarding_type base_type = type ();
  ref_variable_stmt result (new model_variable_stmt (peek ()));
  result->set_variables (variable_declarators (base_type));

  if (is_final)
    result->set_final ();
  result->set_annotations (annos);

  return result;
}

// local-variable-declaration-statement:
//   local-variable-declaration ";"
// local-variable-declaration:
//   [ "final" ] type variable-declarators
ref_stmt
parse::local_variable_declaration_statement ()
{
  ref_stmt result = local_variable_declaration ();
  require (TOKEN_SEMI);
  return result;
}

// block-statements:
//   { local-variable-declaration-statement | class-or-interface-declaration
//     | statement }
std::list<ref_stmt>
parse::block_statements ()
{
  std::list<ref_stmt> result;

  // This function is also used when parsing a switch block.
  // FIXME
  for (token t = peek ();
       t != TOKEN_CLOSE_BRACE && t != TOKEN_CASE && t != TOKEN_DEFAULT;
       t = peek ())
    {
      state_machine<ref_stmt> machine (this);
      ref_stmt next
	= machine.run (&parse::statement,
		       &parse::local_variable_declaration_statement,
		       &parse::class_or_interface_declaration_statement,
		       (state_machine<ref_expression>::parse_function) NULL);
      if (! next)
	throw syntax_error (t, "expected statement, local variable "
			    "declaration, or class declaration");
      result.push_back (next);
    }

  return result;
}

// block:
//   "{" block-statements "}"
ref_block 
parse::block (const ref_block &b)
{
  location where = require (TOKEN_OPEN_BRACE);

  b->set_statements (block_statements ());

  require (TOKEN_CLOSE_BRACE);

  return b;
}

// modifiers-opt:
//   { modifier }
ref_modifier_list
parse::modifiers_opt ()
{
  ref_modifier_list mods (new model_modifier_list ());

  for (token t = peek ();	// FIXME
       model_modifier_list::modifier_token_p (t);
       t = peek ())
    {
      assume (t.get_type ());
      mods->add (t);
    }

  return mods;
}

// explicit-constructor-invocation:
//   [ actual-type-parameters ] "this" "(" [ argument-list ] ")" ";"
//   | [ actual-type-parameters ] "super" "(" [ argument-list ] ")" ";"
//   | primary "." [ actual-type-parameters ] "super"
//       "(" [ argument-list ] ")" ";"
ref_stmt
parse::explicit_constructor_invocation ()
{
  ref_invocation_base result_expr;

  std::list<ref_forwarding_type> type_params;
  if (peek () == TOKEN_LESS_THAN)
    type_params = actual_type_parameters ();

  token &t = peek ();
  bool need_default = false;

  switch (t.get_type ())
    {
    case TOKEN_THIS:
      {
	if (peek1 () != TOKEN_OPEN_PAREN)
	  need_default = true;
	else
	  {
	    assume (TOKEN_THIS);
	    model_this_invocation *th;
	    if (type_params.empty ())
	      th = new model_this_invocation (t);
	    else
	      th = new model_generic_this_invocation (t, type_params);
	    th->set_enclosing_class (enclosing_class ());
	    result_expr = th;
	  }
      }
      break;

    case TOKEN_SUPER:
      {
	if (peek1 () != TOKEN_OPEN_PAREN)
	  need_default = true;
	else
	  {
	    assume (TOKEN_SUPER);
	    ref_super_invocation super;
	    if (type_params.empty ())
	      super = new model_super_invocation (t);
	    else
	      super = new model_generic_super_invocation (t, type_params);
	    super->set_enclosing_class (enclosing_class ());
	    result_expr = super;
	  }
      }
      break;

    default:
      {
	if (! type_params.empty ())
	  throw syntax_error (t, "%<this%> or %<super%> expected");
	marker mark (our_token_stream);
	try
	  {
	    ref_expression prim (primary ());
	    require (TOKEN_DOT);
	    if (peek () == TOKEN_LESS_THAN)
	      type_params = actual_type_parameters ();
	    require (TOKEN_SUPER);
	    ref_super_invocation super;
	    if (type_params.empty ())
	      super = new model_super_invocation (t);
	    else
	      super = new model_generic_super_invocation (t, type_params);
	    super->set_expression (prim);
	    result_expr = super;
	  }
	catch (syntax_error &)
	  {
	    // We can't tell if there is a real syntax error or if we
	    // just need to supply a super() call.  So, we supply one
	    // and, if there really was an error, it will be found
	    // later.
	    mark.backtrack ();

	    if (enclosing_class ()->get_pretty_name () == "java.lang.Object")
	      return NULL;

	    need_default = true;
	  }
      }
      break;
    }

  if (need_default)
    {
      // Can't have type parameters without an actual call.
      if (! type_params.empty ())
	throw syntax_error (t, "%<this%> or %<super%> expected");

      ref_super_invocation super (new model_super_invocation (t));
      super->set_enclosing_class (enclosing_class ());
      super->set_synthetic ();
      result_expr = super;
    }
  else
    result_expr->set_arguments (arguments ());
  return ref_stmt (new model_expression_stmt (t, result_expr));
}

// constructor-body:
//   [ explicit-constructor-invocation ]
//   + [ block-statements ]
//
ref_block
parse::constructor_body ()
{
  location where = require (TOKEN_OPEN_BRACE);

  ref_stmt constructor = explicit_constructor_invocation ();
  std::list<ref_stmt> statements (block_statements ());
  require (TOKEN_CLOSE_BRACE);

  if (constructor)
    statements.push_front (constructor);
  return ref_block (new model_block (where, statements));
}

// method-body:
//   block | ";"
ref_block
parse::method_body ()
{
  ref_block b;
  if (peek () == TOKEN_SEMI)
    assume (TOKEN_SEMI);
  else
    b = block (new model_block (peek ()));
  return b;
}

// type-bound-opt:
//   "extends" class-or-interface-type additional-bound-list-opt
void
parse::type_bound_opt (const ref_type_variable &typevar,
		       int &lt_depth)
{
  if (peek () == TOKEN_EXTENDS)
    {
      assume (TOKEN_EXTENDS);
      // FIXME: eliminate primitive types at parse-time?
      int save = lt_depth;
      typevar->add_bound (counted_type (lt_depth));
      if (lt_depth == save)
	{
	  while (peek () == TOKEN_BITWISE_AND)
	    {
	      assume (TOKEN_BITWISE_AND);
	      typevar->add_bound (counted_type (lt_depth));
	      if (lt_depth != save)
		break;
	    }
	}
    }
}

// type-parameter:
//   identifier type-bound-opt
ref_type_variable
parse::type_parameter (int &lt_depth)
{
  ref_type_variable result (new model_type_variable (peek ()));
  result->set_name (identifier ());
  result->set_compilation_unit (unit);
  type_bound_opt (result, lt_depth);
  return result;
}

// type-parameters:
//   "<" type-parameter-list ">"
std::list<ref_type_variable>
parse::type_parameters ()
{
  assume (TOKEN_LESS_THAN);
  std::list<ref_type_variable> result;

  int lt_depth = 1;
  do
    {
      result.push_back (type_parameter (lt_depth));
      assert (lt_depth <= 1);
      if (! lt_depth)
	break;
      token &t = peek ();
      if (t != TOKEN_COMMA)
	break;
      assume (TOKEN_COMMA);
    }
  while (true);

  if (lt_depth)
    require (TOKEN_GREATER_THAN);
  return result;
}

// member-value:
//   conditional-expression
//   | annotation
//   | member-value-array-initializer
ref_expression
parse::member_value ()
{
  token &t = peek ();
  if (t == TOKEN_OPEN_BRACE)
    {
      assume (TOKEN_OPEN_BRACE);
      // Here we duplicate some code from array initializer parsing.
      // We could parameterize it instead.
      std::list<ref_expression> result;
      if (peek () != TOKEN_CLOSE_BRACE)
	{
	  // It is possible to have a sole ',' in the initializer.  We
	  // handle this specially.
	  if (peek () == TOKEN_COMMA && peek1 () == TOKEN_CLOSE_BRACE)
	    assume (TOKEN_COMMA);
	  else
	    {
	      while (true)
		{
		  result.push_back (member_value ());
		  if (peek () != TOKEN_COMMA)
		    break;
		  assume (TOKEN_COMMA);
		  // A trailing comma is ok, and is ignored.
		  if (peek () == TOKEN_CLOSE_BRACE)
		    break;
		}
	    }
	}
      require (TOKEN_CLOSE_BRACE);
      return new model_annotation_initializer (t, result);
    }
  else if (t == TOKEN_AT)
    return annotation ();
  return conditional_expression ();
}

// annotation:
// ...
ref_annotation
parse::annotation ()
{
  location w = assume (TOKEN_AT);
  ref_forwarding_type name (type ());
  std::list<ref_annotation_value> values;
  if (peek () == TOKEN_OPEN_PAREN)
    {
      assume (TOKEN_OPEN_PAREN);
      
      token t = peek ();	// FIXME
      if (t == TOKEN_CLOSE_PAREN)
	{
	  // Marker annotation, do nothing.
	}
      else if (t != TOKEN_IDENTIFIER || peek1 () != TOKEN_ASSIGN)
	{
	  // Single member annotation.
	  ref_expression expr = member_value ();
	  values.push_back (new model_annotation_value (t, "value", expr));
	}
      else
	{
	  while (true)
	    {
	      std::string name = identifier ();
	      require (TOKEN_ASSIGN);
	      ref_expression expr = member_value ();
	      values.push_back (new model_annotation_value (t, name, expr));
	      if (peek () != TOKEN_COMMA)
		break;
	      assume (TOKEN_COMMA);
	      t = peek ();
	    }
	}

      require (TOKEN_CLOSE_PAREN);
    }
  ref_annotation ann = new model_annotation (w, name, values);
  return ann;
}

// annotations:
//   [ annotation ]...
std::list<ref_annotation>
parse::annotations ()
{
  if (! global->get_compiler ()->feature_annotations ())
    throw syntax_error (peek (), "%<@%> seen but annotations not supported");

  std::list<ref_annotation> result;
  while (peek () == TOKEN_AT)
    result.push_back (annotation ());
  return result;
}

// This is not really "annotation-type-member".
// Instead it just parses the method-like syntax:
//   identifier "(" ")" [ "default" member-value ]
// The type and modifiers are passed in.
void
parse::annotation_type_member (const ref_class &klass,
			       const ref_modifier_list &mods,
			       const ref_forwarding_type &member_type)
{
  location where = peek ();
  std::string name = identifier ();
  require (TOKEN_OPEN_PAREN);
  require (TOKEN_CLOSE_PAREN);
  ref_expression def_value;
  if (peek () == TOKEN_DEFAULT)
    {
      assume (TOKEN_DEFAULT);
      def_value = member_value ();
    }
  require (TOKEN_SEMI);

  ref_annotation_member result (new model_annotation_member (where, name,
							     def_value,
							     member_type));
  result->set_modifiers (mods);

  ref_annotation_type annt
    = assert_cast<model_annotation_type *> (klass.get ());
  annt->add_annotation_member (result);
}

// This can parse any sort of method or constructor.
ref_method 
parse::any_method_declarator (const std::list<ref_annotation> &annos,
			      const ref_modifier_list &mods,
			      int flags,
			      ref_forwarding_type member_type,
			      bool deprecated)
{
  std::string id;
  bool is_interface = enclosing_class ()->interface_p ();
  ref_method result;
  if (! is_interface && (flags & METHOD_CONSTRUCTOR) != 0)
    {
      id = "<init>";
      result = new model_constructor (peek (), enclosing_class ());
    }
  else
    {
      id = identifier ();
      result = new model_method (peek (), enclosing_class ());
    }

  result->set_return_type (member_type);
  result->set_annotations (annos);
  result->set_deprecated (deprecated);
  result->set_name (id);

  bool dots_found = false;
  result->set_parameters
    (formal_parameter_list (global->get_compiler ()->feature_varargs (),
			    &dots_found));
  if (dots_found)
    result->set_varargs ();
  result->set_modifiers (mods);

  if ((flags & (METHOD_CONSTRUCTOR | METHOD_VOID)) == 0)
    {
      ref_forwarding_type save = member_type;
      location save_loc = peek ();
      member_type = brackets_opt (member_type);
      if (member_type != save)
	{
	  result->set_return_type (member_type);
	  if (global->get_compiler ()->warn_deprecated ())
	    std::cerr << format (new format_repr (format_type (global->get_compiler ()->warn_deprecated ()),
						  save_loc,
						  "deprecated use of brackets "
						  "after formal parameters in "
						  "method declarator"));
	}
    }
  else if (peek () == TOKEN_OPEN_BRACKET)
    throw syntax_error (peek (),
			"use of brackets after formal parameters in "
			"method declarator");

  if (peek () == TOKEN_THROWS)
    {
      assume (TOKEN_THROWS);
      result->set_throws (type_list ());
    }

  token &t = peek ();
  if (t == TOKEN_OPEN_BRACE)
    {
      if (is_interface)
	throw syntax_error (t, "%<;%> expected");
      else if ((flags & METHOD_CONSTRUCTOR) != 0)
	result->set_body (constructor_body ());
      else
	result->set_body (method_body ());
      result->set_method_end (peek ());
    }
  else if (t == TOKEN_SEMI)
    assume (TOKEN_SEMI);
  else
    throw syntax_error (t, "method body or %<;%> expected");

  return result;
}

// field-declarator:
//   modifier-opts type identifier variable-declarator-rest
void
parse::field_declarator (const ref_class &klass,
			 const ref_forwarding_type &member_type,
			 const std::list<ref_annotation> &annos,
			 const ref_modifier_list &mods,
			 bool deprecated)
{
  // We can have several fields from a single declarator.
  std::list<ref_variable_decl> vars = variable_declarators (member_type);
  require (TOKEN_SEMI);

  bool checked = false;
  for (std::list<ref_variable_decl>::const_iterator i = vars.begin ();
       i != vars.end ();
       ++i)
    {
      // We do this checking here since this way we get a single error
      // for a set of field declarations.
      if (! checked)
	{
	  model_field::check_modifiers ((*i).get (), mods);
	  checked = true;
	}

      ref_field result (new model_field (*i));
      result->set_annotations (annos);
      result->set_modifiers (mods);
      result->set_deprecated (deprecated);
      klass->add (result);
    }
}

ref_method
parse::void_method (const ref_class &klass,
		    const std::list<ref_annotation> &annos,
		    const ref_modifier_list &mods,
		    bool deprecated)
{
  token &t = require (TOKEN_VOID);
  ref_forwarding_type voidtype
    = new model_forwarding_resolved (t, primitive_void_type);
  ref_method decl = any_method_declarator (annos, mods, METHOD_VOID,
					   voidtype, deprecated);
  klass->add (decl);
  return decl;
}

// member-decl:
//   method-or-field-decl
//   | "void" identifier void-method-declarator
//   | identifier constructor-declarator-rest
//   | class-or-interface-declaration
// method-or-field-decl:
//   type identifier method-or-field-rest
// This has also been modified to handle annotation type members.
void
parse::member_decl (const ref_class &klass,
		    const std::list<ref_annotation> &annos,
		    const ref_modifier_list &mods,
		    bool deprecated,
		    bool parse_annotation)
{
  token &t = peek ();
  // You can't have a void method in an annotation; we throw an
  // exception for this later.
  if (! parse_annotation && t == TOKEN_VOID)
    {
      void_method (klass, annos, mods, deprecated);
      return;
    }
  else if (t == TOKEN_CLASS)
    {
      ref_class member = class_declaration (mods);
      member->set_deprecated (deprecated);
      member->set_annotations (annos);
      klass->add (member);
      return;
    }
  else if (t == TOKEN_INTERFACE)
    {
      ref_class member = interface_declaration (mods);
      member->set_deprecated (deprecated);
      member->set_annotations (annos);
      klass->add (member);
      return;
    }
  else if (t == TOKEN_ENUM)
    {
      ref_class member = enum_declaration (mods);
      member->set_deprecated (deprecated);
      member->set_annotations (annos);
      klass->add (member);
      return;
    }
  else if (t == TOKEN_AT)
    {
      ref_class member = annotation_type_declaration (mods);
      member->set_deprecated (deprecated);
      member->set_annotations (annos);
      klass->add (member);
      return;
    }

  // FIXME?  perhaps we should parse type parameters and then give an
  // error message instead if !feature_generics.
  std::list<ref_type_variable> type_parms;
  bool must_be_method = false;
  if (! parse_annotation
      && global->get_compiler ()->feature_generics ()
      && t == TOKEN_LESS_THAN)
    {
      type_parms = type_parameters ();
      // We have to handle void methods specially here, since 'void'
      // is not a "type".
      if (peek () == TOKEN_VOID)
	{
	  ref_method m = void_method (klass, annos, mods, deprecated);
	  m->set_type_parameters (type_parms);
	  return;
	}
      must_be_method = true;
    }

  int method_flags = 0;
  ref_forwarding_type member_type;

  token &pt = peek ();
  if (! parse_annotation
      && pt == TOKEN_IDENTIFIER
      && ((ref_identifier) pt)->get_identifier () == klass->get_name ()
      && peek1 () == TOKEN_OPEN_PAREN)
    {
      // Found a constructor.
      assume (TOKEN_IDENTIFIER);
      method_flags |= METHOD_CONSTRUCTOR;
      must_be_method = true;
      member_type = new model_forwarding_resolved (t, primitive_void_type);
    }
  else
    member_type = type ();

  // A method will be a single identifier followed by an open paren.
  if (must_be_method || peek1 () == TOKEN_OPEN_PAREN)
    {
      if (parse_annotation)
	{
	  assert (! must_be_method);
	  // We just ignore javadoc here.  Perhaps we should give a
	  // warning if it had @deprecated, since that is just
	  // ignored.
	  annotation_type_member (klass, mods, member_type);
	}
      else
	{
	  ref_method meth (any_method_declarator (annos, mods, method_flags,
						  member_type, deprecated));
	  if (! type_parms.empty ())
	    meth->set_type_parameters (type_parms);
	  klass->add (meth);
	}
    }
  else
    field_declarator (klass, member_type, annos, mods, deprecated);
}

// Helper function for class_body_declaration.  This handles static
// blocks and instance initializer blocks.
void
parse::class_body_block (const std::list<ref_annotation> &annos,
			 const ref_modifier_list &mods)
{
  bool is_static = ((mods->value () & ACC_STATIC) != 0);

  ref_block b = block (new model_initializer_block (peek (), ! is_static));

  if (is_static)
    {
      mods->check (b.get (), ACC_STATIC,
		   "modifier %1 is invalid for static initializer");
      enclosing_class ()->add_static_initializer (b);
    }
  else
    {
      mods->check (b.get (), 0,
		   "modifier %1 is invalid for instance initializer");
      enclosing_class ()->add_instance_initializer (b);
    }
}

// class-body-declaration:
//   ";"
//   | [ "static" ] block
//   | modifiers-opt member-decl
void
parse::class_body_declaration (const ref_class &result,
			       bool parse_annotation)
{
  token &t = peek ();
  if (t == TOKEN_SEMI)
    {
      assume (TOKEN_SEMI);
      // FIXME: emit a warning about how this is dumb.
      return;
    }

  // '@' could represent the start of annotations, or it could be
  // '@interface'.
  std::list<ref_annotation> annos;
  if (peek () == TOKEN_AT && peek1 () != TOKEN_INTERFACE)
    annos = annotations ();

  // Javadoc is valid before the modifiers of a member.  Before a
  // static block we can just ignore the javadoc as it is an ordinary
  // comment.
  bool deprecated = our_token_stream->get_javadoc ();

  // Always read modifiers at this point, since "static" is a valid
  // modifier.  Anything incorrect we diagnose later.
  ref_modifier_list mods = modifiers_opt ();

  token t2 = peek ();
  if (! parse_annotation && t2 == TOKEN_OPEN_BRACE)
    class_body_block (annos, mods);
  else
    member_decl (result, annos, mods, deprecated, parse_annotation);
}

// class-body:
//   "{" { class-body-declaration } "}"
// This also handles annotation type bodies.
void
parse::class_body (const ref_class &result, bool parse_annotation)
{
  // We push a new empty label to indicate to inner methods that there
  // is a class boundary on the label stack.  This lets us avoid
  // errors about shadowing when they are not warranted.
  stack_temporary<Ilabel *> push_scope (label_stack, NULL);

  stack_temporary<model_class *> pusher (class_stack, result.get ());
  require (TOKEN_OPEN_BRACE);
  while (peek () != TOKEN_CLOSE_BRACE)
    class_body_declaration (result, parse_annotation);
  require (TOKEN_CLOSE_BRACE);
}

// Helper method to set the containing class and possibly emit an
// error if required.
void
parse::set_containing_class (const ref_class &klass)
{
  std::string name = klass->get_name ();
  // Ensure name is set before this is called.
  assert (name != "");

  klass->set_declaring_class (enclosing_class ());
  for (model_class *iter = enclosing_class ();
       iter;
       iter = iter->get_lexically_enclosing_class ())
    {
      if (name == iter->get_name ())
	// FIXME: location of enclosing class.
	std::cerr << klass->error ("class has the same simple name "
				   "as enclosing class");
    }
}

// interface-declaration
//   "interface" identifier [ type-parameters ]
//   + ["extends" type-list] interface-body
ref_class
parse::interface_declaration (const ref_modifier_list &mods)
{
  location where = require (TOKEN_INTERFACE);
  ref_class result (new model_class (where));
  result->set_interface ();

  result->set_compilation_unit (unit);
  result->set_name (identifier ());
  set_containing_class (result);
  result->set_modifiers (mods);

  token &t = peek ();

  if (global->get_compiler ()->feature_generics () && t == TOKEN_LESS_THAN)
    {
      result->set_type_parameters (type_parameters ());
      t = peek ();
    }

  if (t == TOKEN_EXTENDS)
    {
      assume (TOKEN_EXTENDS);
      result->set_implements (type_list ());
    }

  // It is simplest for us to use class_body and then catch some
  // errors during semantic analysis.
  class_body (result);
  return result;
}

// class-declaration:
//   "class" identifier [ type-parameters ]
//   + ["extends" type] ["implements" type-list]
//   + class-body
ref_class 
parse::class_declaration (const ref_modifier_list &mods)
{
  location where = assume (TOKEN_CLASS);
  ref_class result (new model_class (where));

  result->set_compilation_unit (unit);
  result->set_name (identifier ());
  set_containing_class (result);
  result->set_modifiers (mods);

  token &t = peek ();

  if (global->get_compiler ()->feature_generics () && t == TOKEN_LESS_THAN)
    {
      result->set_type_parameters (type_parameters ());
      t = peek ();
    }

  if (peek () == TOKEN_EXTENDS)
    {
      assume (TOKEN_EXTENDS);
      result->set_superclass (type ());
    }

  if (peek () == TOKEN_IMPLEMENTS)
    {
      assume (TOKEN_IMPLEMENTS);
      result->set_implements (type_list ());
    }

  class_body (result);
  return result;
}

// enum-body:
//   "{" enum-constants-opt enum-body-declarations-opt "}"
// enum-constants:
//   enum-constant { "," enum-constant }
// enum-constant:
//   identifier [ arguments ] [ class-body ]
// enum-body-declarations:
//   ";" [ class-body-declarations ]
void
parse::enum_body (ref_enum result)
{
  require (TOKEN_OPEN_BRACE);

  if (peek () != TOKEN_SEMI && peek () != TOKEN_CLOSE_BRACE)
    {
      while (true)
	{
	  ref_enum_constant c (new model_enum_constant (peek ()));
	  c->set_compilation_unit (unit);
	  c->set_name (identifier ());
	  if (peek () == TOKEN_OPEN_PAREN)
	    c->set_arguments (arguments ());
	  if (peek () == TOKEN_OPEN_BRACE)
	    {
	      c->set_has_body ();
	      class_body (c);
	    }
	  result->add_enum (c);

	  if (peek () != TOKEN_COMMA)
	    break;
	  assume (TOKEN_COMMA);
	  // We might have a trailing comma.
	  if (peek () == TOKEN_SEMI || peek () == TOKEN_CLOSE_BRACE)
	    break;
	}
    }

  if (peek () == TOKEN_SEMI)
    {
      assume (TOKEN_SEMI);

      // We push a new empty label to indicate to inner methods that
      // there is a class boundary on the label stack.  This lets us
      // avoid errors about shadowing when they are not warranted.
      stack_temporary<Ilabel *> push_scope (label_stack, NULL);

      stack_temporary<model_class *> pusher (class_stack, result.get ());

      while (peek () != TOKEN_CLOSE_BRACE)
	class_body_declaration (result, false);
    }

  require (TOKEN_CLOSE_BRACE);
}

// enum-declaration:
//   "enum" identifier [ "implements" type-list ] enum-body
ref_enum
parse::enum_declaration (const ref_modifier_list &mods)
{
  location where = assume (TOKEN_ENUM);
  ref_enum result (new model_enum (where));
  result->set_compilation_unit (unit);
  result->set_name (identifier ());
  set_containing_class (result);
  result->set_modifiers (mods);

  if (peek () == TOKEN_IMPLEMENTS)
    {
      assume (TOKEN_IMPLEMENTS);
      result->set_implements (type_list ());
    }
  enum_body (result);
  return result;
}

// annotation-type-declaration:
//   "@" "interface" identifier annotation-type-body
ref_annotation_type
parse::annotation_type_declaration (const ref_modifier_list &mods)
{
  assume (TOKEN_AT);
  location where = require (TOKEN_INTERFACE);

  ref_annotation_type result (new model_annotation_type (where));
  result->set_compilation_unit (unit);
  result->set_name (identifier ());
  set_containing_class (result);
  result->set_modifiers (mods);
  class_body (result, true);
  return result;
}

// Like class-or-interface-declaration, but wraps the result in a
// statement.
ref_stmt
parse::class_or_interface_declaration_statement ()
{
  location where = peek ();
  ref_class decl (class_or_interface_declaration ());
  decl->set_local ();
  return ref_stmt (new model_class_decl_stmt (where, decl));
}

// class-or-interface-declaration:
//   modifers-opt ( class-declaration | interface-declaration )
// We augment this with javadoc, enum, and annotation type declaration
// handling.
ref_class
parse::class_or_interface_declaration ()
{
  ref_class result;

  // Javadoc is valid before the modifiers of a class.
  bool deprecated = our_token_stream->get_javadoc ();

  // '@' could represent the start of annotations, or it could be
  // '@interface'.
  std::list<ref_annotation> annos;
  if (peek () == TOKEN_AT && peek1 () != TOKEN_INTERFACE)
    annos = annotations ();

  ref_modifier_list mods = modifiers_opt ();
  token &t = peek ();
  if (t == TOKEN_CLASS)
    result = class_declaration (mods);
  else if (t == TOKEN_INTERFACE)
    result = interface_declaration (mods);
  else if (t == TOKEN_ENUM)
    result = enum_declaration (mods);
  else if (t == TOKEN_AT)
    result = annotation_type_declaration (mods);
  else
    throw syntax_error (t, "%<class%>, %<interface%>, or %<enum%> "
			"declaration expected");

  result->set_deprecated (deprecated);
  result->set_annotations (annos);

  return result;
}

// type-declaration:
//   class-or-interface-declaration
//   | ";"
// However, the empty statement is deprecated.
ref_class
parse::type_declaration ()
{
  token &t = peek ();
  if (t == TOKEN_SEMI)
    {
      assume (TOKEN_SEMI);
      if (global->get_compiler ()->warn_deprecated ())
	// FIXME is this really deprecated?  Or just bad style?
	std::cerr << format (new format_repr (format_type (global->get_compiler ()->warn_deprecated ()),
					      t, "empty declaration is deprecated"));
      return NULL;
    }
  else
    return class_or_interface_declaration ();
}

// import-declaration:
//   "import" [ "static" ] identifier { "." identifier } [ "." "*" ] ";"
ref_import 
parse::import_declaration ()
{
  ref_import import;
  location where = require (TOKEN_IMPORT);
  bool on_demand = false;
  bool is_static = false;

  if (peek () == TOKEN_STATIC)
    {
      assume (TOKEN_STATIC);
      is_static = true;
      if (! global->get_compiler ()->feature_static_import ())
	throw syntax_error (where, "static import feature disabled "
			    "in this compilation");
    }

  std::list<std::string> id (qualified_identifier ());
  if (peek () == TOKEN_DOT)
    {
      assume (TOKEN_DOT);
      require (TOKEN_MULTIPLY);
      on_demand = true;
    }

  require (TOKEN_SEMI);

  if (on_demand)
    {
      if (is_static)
	import = new model_static_import_on_demand (where, id);
      else
	import = new model_import_on_demand (where, id);
    }
  else
    {
      if (is_static)
	import = new model_static_import_single (where, id);
      else
	import = new model_import_single (where, id);
    }

  return import;
}

// compilation-unit:
//    [ package ] { import-declaration } { type-declaration }
// package:
//   "package" qualified-identifier ";"
ref_unit 
parse::compilation_unit ()
{
  unit = new model_unit_source (peek ());

  // Create a fake 'import java.lang.*'.  We do this here so that
  // model_unit can also be used to represent .class files.
  std::list<std::string> javalang;
  javalang.push_back ("java");
  javalang.push_back ("lang");
  ref_import imp (new model_import_on_demand (LOCATION_UNKNOWN, javalang,
					      true));
  unit->add (imp);

  std::list<ref_annotation> annos;
  if (peek () == TOKEN_AT)
    annos = annotations ();

  token t = peek ();		// FIXME
  if (t == TOKEN_PACKAGE)
    {
      assume (TOKEN_PACKAGE);
      // FIXME: we make a bogus new element just to hold the location
      // as an error target.
      ref_empty loc (new model_empty (t));
      std::list<std::string> name (qualified_identifier ());
      require (TOKEN_SEMI);
      // It is simple enough to create the package now, and it means
      // we don't have to create a special package-forwarder class.
      pkg = classify_package_name (loc.get (), name);
      pkg->set_annotations (annos);
      annos.clear ();
      t = peek ();
    }
  else
    pkg = global->get_compiler ()->get_unnamed_package ();
  unit->set_package (pkg);

  while (t == TOKEN_IMPORT)
    {
      // FIXME: if ! (annos.empty ()) error
      unit->add (import_declaration ());
      t = peek ();
    }

  while (t != TOKEN_EOF)
    {
      ref_class type = type_declaration ();
      if (type)
	{
	  if (! annos.empty ())
	    {
	      type->set_annotations (annos);
	      annos.clear ();
	    }
	  unit->add (type);
	}
      t = peek ();
    }

  return ref_unit (unit);
}

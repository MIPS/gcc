// Unary operators.

// Copyright (C) 2004 Free Software Foundation, Inc.
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

void
model_unary::ensure_variable (const ref_expression &expr)
{
  model_expression *base = expr.get ();
  // I'm sure there is a better way.
  // FIXME: we should just have a new method on expr.
  // FIXME: merge with model_assignment::check_lhs.
  if (dynamic_cast<model_memberref_forward *> (base))
    {
      model_memberref_forward *mem
	= dynamic_cast<model_memberref_forward *> (base);
      base = mem->get_real ();
    }

  // Note that currently this method is only used by side-effect
  // operators, so we can do 'final' checks here.
  if (dynamic_cast<model_simple_variable_ref *> (base))
    {
      model_simple_variable_ref *svf
	= dynamic_cast<model_simple_variable_ref *> (base);
      if (svf->get_variable ()->final_p ())
	throw expr->error ("argument to operator %1 cannot be "
			   "a %<final%> variable")
	  % get_name ();
      return;
    }
  else if (dynamic_cast<model_field_ref *> (base))
    {
      model_field_ref *fr = dynamic_cast<model_field_ref *> (base);
      if (fr->get_field ()->final_p ())
	throw expr->error ("argument to operator %1 cannot be "
			   "a %<final%> field")
	  % get_name ();
      return;
    }
  else if (! dynamic_cast<model_array_ref *> (base))
    throw expr->error ("argument to operator %1 must be variable reference")
      % get_name ();
}

void
model_unary::resolve (resolution_scope *scope)
{
  expr->resolve (scope);
  model_type *t = unary_numeric_promotion (expr);
  if (! t)
    throw expr->error ("argument to operator %1 not of numeric type")
      % get_name ();
  set_type (t);
}



template<unary_function OP, predicate P, char NAME>
std::string
model_prefix_simple<OP, P, NAME>::get_name () const
{
  char n[2];
  n[0] = NAME;
  n[1] = '\0';
  return std::string (n);
}

template<unary_function OP, predicate P, char NAME>
void
model_prefix_simple<OP, P, NAME>::visit (visitor *v)
{
  v->visit_prefix_simple (this, expr);
}

template<unary_function OP, predicate P, char NAME>
void
model_prefix_simple<OP, P, NAME>::resolve (resolution_scope *scope)
{
  model_unary::resolve (scope);
  model_primitive_base *bt = assert_cast<model_primitive_base *> (type ());
  if (! (bt->*P) ())
    throw error ("invalid type for operator %1") % NAME;
}

// This overrides an otherwise incorrect definition.
template<>
void
model_prefix_simple<&model_primitive_base::lnot,
		    &model_type::boolean_p,
		    '!'>::visit (visitor *)
{
  // This is overridden by a subclass and should never be called.
  abort ();
}



template<char NAME>
std::string
model_prefix_side_effect<NAME>::get_name () const
{
  char op[3];
  op[0] = NAME;
  op[1] = NAME;
  op[2] = '\0';
  return std::string (op);
}

template<char NAME>
void
model_prefix_side_effect<NAME>::resolve (resolution_scope *scope)
{
  expr->set_left_hand_side (true);
  expr->resolve (scope);
  ensure_variable (expr);

  if (! expr->type ()->numeric_p ())
    {
      throw error ("argument to prefix %1 must have numeric type")
	% get_name ();
    }
  set_type (expr->type ());
}

template<char NAME>
void
model_prefix_side_effect<NAME>::visit (visitor *v)
{
  v->visit_prefix_side_effect (this, expr);
}



template<char NAME>
std::string
model_postfix<NAME>::get_name () const
{
  char op[3];
  op[0] = NAME;
  op[1] = NAME;
  op[2] = '\0';
  return std::string (op);
}

template<char NAME>
void
model_postfix<NAME>::resolve (resolution_scope *scope)
{
  expr->set_left_hand_side (true);
  expr->resolve (scope);
  ensure_variable (expr);

  if (! expr->type ()->numeric_p ())
    {
      throw error ("argument to postfix %1 have numeric type")
	% get_name ();
    }
  set_type (expr->type ());
}

template<char NAME>
void
model_postfix<NAME>::visit (visitor *v)
{
  v->visit_postfix_side_effect (this, expr);
}



void
model_logical_not::resolve (resolution_scope *scope)
{
  expr->resolve (scope);
  if (! boolean_conversion (expr))
    throw expr->error ("operand of %<!%> not convertible to boolean type");
  set_type (primitive_boolean_type);
}

void
model_logical_not::visit (visitor *v)
{
  v->visit_prefix_simple (this, expr);
}



// Instantiations.

template class model_prefix_simple<&model_primitive_base::plus,
				   &model_type::numeric_p,
				   '+'>;
template class model_prefix_simple<&model_primitive_base::minus,
				   &model_type::numeric_p,
				   '-'>;
template class model_prefix_simple<&model_primitive_base::bnot,
				   &model_type::integral_p,
				   '~'>;
template class model_prefix_simple<&model_primitive_base::lnot,
				   &model_type::boolean_p,
				   '!'>;
template class model_prefix_side_effect<'+'>;
template class model_prefix_side_effect<'-'>;
template class model_postfix<'+'>;
template class model_postfix<'-'>;

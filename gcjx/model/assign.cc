// Assignments.

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

void
model_assignment::check_lhs (const char *name)
{
  model_expression *base = lhs.get ();
  // FIXME: there must be a better way.  We have similar snippets in
  // a few places, maybe we should have a method on model_expression
  // that unwraps automatically.  This starts to get into bad
  // gcc-like territory, where bugs are fixed by adding unwraps here
  // and there... :-(
  // FIXME: merge with model_unary:ensure_variable
  if (dynamic_cast<model_memberref_forward *> (base))
    {
      model_memberref_forward *mem
	= dynamic_cast<model_memberref_forward *> (base);
      base = mem->get_real ();
    }

  model_field_ref *fr = dynamic_cast<model_field_ref *> (base);
  if (fr != NULL)
    {
      model_field *f = fr->get_field ();
      if (f->get_declaring_class ()->array_p ())
	{
	  assert (f->get_name () == "length");
	  throw lhs->error ("can't assign to %<length%> field of an array");
	}

      if (f->final_p () && f->has_initializer_p ())
	std::cerr << lhs->error ("can't assign to an "
				 "initialized %<final%> field");
    }
  else if (dynamic_cast<model_array_ref *> (base))
    {
      // Nothing special.
    }
  else if (dynamic_cast<model_simple_variable_ref *> (base))
    {
      model_simple_variable_ref *svr
	= assert_cast<model_simple_variable_ref *> (base);
      model_variable_decl *var = svr->get_variable ();
      if (var->final_p () && var->consider_initialized_p ())
	std::cerr << lhs->error ("can't assign to an "
				 "initialized %<final%> variable");
    }
  else
    throw lhs->error ("argument to operator %<%u1=%> must be a "
		      "variable reference")
      % name;
}

void
model_assignment::resolve (resolution_scope *scope)
{
  lhs->set_left_hand_side (false);
  lhs->resolve (scope);

  rhs->use_assignment_conversion (lhs->type ());
  rhs->resolve (scope);

  check_lhs ("");

  model_type *result = assignment_conversion (lhs->type (), rhs);
  if (! result)
    throw rhs->error ("type %1 cannot be converted to type %2 in assignment")
      % rhs->type () % lhs->type ();
  set_type (result);

  // FIXME: this fails as these will be memberrefs.
  // we need something generic here, there are dynamic casts like
  // this elsewhere...
  model_simple_variable_ref *lhsref
    = dynamic_cast<model_simple_variable_ref *> (lhs.get ());
  model_simple_variable_ref *rhsref
    = dynamic_cast<model_simple_variable_ref *> (rhs.get ());
  if (lhsref
      && rhsref
      && lhsref->get_variable () == rhsref->get_variable ()
      && scope->warn_assignment_no_effect ())
    std::cerr << warn (global->get_compiler ()->warn_assignment_no_effect (),
		       "assignment to variable %1 has no effect")
      % lhs;

  // FIXME also warn for things like 'this.field = field'.
}

void
model_assignment::visit (visitor *v)
{
  v->visit_assignment (this, lhs, rhs);
}



template<typename T>
void
model_op_assignment<T>::extra_check ()
{
  // Nothing.
}

template<>
void
model_op_assignment<model_plus>::extra_check ()
{
  model_class *strtype = global->get_compiler ()->java_lang_String ();
  if (lhs->type () != strtype && rhs->type () == strtype)
    throw error ("left hand side of string %<+=%> must have "
		 "type %<java.lang.String%>");
}

template<typename T>
void
model_op_assignment<T>::resolve (resolution_scope *scope)
{
  lhs->set_left_hand_side (true);

  // Create a temporary binary operation and use it to handle
  // resolution.
  T temp (get_location ());
  temp.set_lhs (lhs);
  temp.set_rhs (rhs);
  temp.resolve (scope);

  if (! casting_conversion (lhs->type (), temp.type ()))
    throw error ("expression of type %1 cannot be converted to type %2 "
		 "in assignment")
      % temp.type () % lhs->type ();
  extra_check ();

  // Resolution might have introduced some casts, which we want to
  // keep.  We only need the cast on the RHS; we don't want to
  // introduce a cast on the LHS and the information from the RHS is
  // enough for the back ends to reconstruct the conversion.
  rhs = temp.get_rhs ();
  check_lhs (T::get_name ());
  set_type (lhs->type ());
}

template<typename T>
void
model_op_assignment<T>::visit (visitor *v)
{
  v->visit_op_assignment (this, lhs, rhs);
}



// Instantiations.
template class model_op_assignment<model_plus>;
template class model_op_assignment<model_minus>;
template class model_op_assignment<model_mult>;
template class model_op_assignment<model_div>;
template class model_op_assignment<model_and>;
template class model_op_assignment<model_or>;
template class model_op_assignment<model_xor>;
template class model_op_assignment<model_mod>;
template class model_op_assignment<model_left_shift>;
template class model_op_assignment<model_right_shift>;
template class model_op_assignment<model_unsigned_right_shift>;

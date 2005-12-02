// switch statement.

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
model_switch_block::visit (visitor *v)
{
  v->visit_switch_block (this, statements);
}

void
model_switch_block::resolve (resolution_scope *scope,
			     model_type *switch_type,
			     std::map<jint, model_expression *> &seen,
			     bool is_enum)
{
  model_primitive_base *bt
    = assert_cast<model_primitive_base *> (primitive_int_type);

  for (std::list<ref_expression>::iterator i = labels.begin ();
       i != labels.end ();
       ++i)
    {
      (*i)->resolve (scope);
      if (assignment_conversion (switch_type, (*i).get ()) == NULL)
	throw error ("case expression not assignable to switch type");

      jint val;
      if (is_enum)
	{
	  // Note that we don't fold the expression in this branch.
	  // The reason for this is that we need to only accept class
	  // references, not other things.

	  model_enum_constant *enumc = unwrap_enum_constant ((*i).get ());
	  if (enumc == NULL)
	    throw error ("case expression not enum constant");
	  val = enumc->get_ordinal ();
	}
      else
	{
	  if (! (*i)->constant_p ())
	    throw error ("case expression must be constant");
	  fold (*i);

	  jvalue conv = bt->convert ((*i)->type (), (*i)->value ());
	  val = conv;
	}

      if (seen.find (val) == seen.end ())
	seen.insert (std::make_pair (val, (*i).get ()));
      else
	{
	  std::cerr << error ("duplicate case value");
	  std::cerr
	    << seen.find (val)->second->error ("original instance is here");
	}
    }

  ::resolve (scope, statements);
}



void
model_switch::visit (visitor *v)
{
  v->visit_switch (this, expression, blocks);
}

void
model_switch::resolve (resolution_scope *scope)
{
  model_block_scope vars (scope);
  resolution_scope::push_iscope var_holder (scope, &vars);

  expression->resolve (scope);
  fold (expression);

  model_type *expr_type = expression->type ();
  if (expr_type != primitive_char_type
      && expr_type != primitive_byte_type
      && expr_type != primitive_short_type
      && expr_type != primitive_int_type
      && ! expr_type->enum_p ())
    throw expression->error ("invalid %<switch%> expression type %1"
			     ", must be %<char%>, %<byte%>, %<short%>"
			     ", %<int%>, or %<enum%>")
      % expr_type;

  bool is_enum = expr_type->enum_p ();
  std::map<jint, model_expression *> seen;
  std::list<ref_switch_block>::const_iterator i;
  for (i = blocks.begin (); i != blocks.end (); ++i)
    (*i)->resolve (scope, expression->type (), seen, is_enum);

  if (default_block == NULL)
    {
      if (scope->warn_switch_default ())
	std::cerr << warn (global->get_compiler ()->warn_switch_default (),
			   "%<switch%> without %<default%> case");

      if (scope->warn_switch () && expr_type->enum_p ())
	{
	  model_enum *etype = assert_cast<model_enum *> (expr_type);
	  if (seen.size () != etype->constants_count ())
	    {
	      // FIXME: ought to list the missing values.
	      std::cerr << warn (global->get_compiler ()->warn_switch (),
				 "%<enum%> constant missing from %<switch%>");
	    }
	}
    }
}

void
model_switch::compute_normal_completion (normal_completion_state &state)
{
  normal_completion_pusher push (state, this);

  // First, handle reachability of the switch blocks.
  bool last = true;
  warning_state warn_flag
    = global->get_compiler ()->warn_switch_fall_through ();
  for (std::list<ref_switch_block>::const_iterator i = blocks.begin ();
       i != blocks.end ();
       ++i)
    {
      (*i)->compute_normal_completion (state);
      last = (*i)->can_complete_normally ();

      // FIXME: SuppressWarnings support here... put into normal
      // completion state?
      if (warn_flag && last)
	std::cerr << (*i)->warn (warn_flag,
				 "%<switch%> block without %<break%>");
    }

  // The switch completes normally if:
  // * The final 'case' complete normally, or
  // * There is no 'default' case, or
  // * One of the cases has a 'break' with the switch as a target.
  normal_completion = last || default_block == NULL || break_found;
}

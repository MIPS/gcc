// Conversion and promotion rules.

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
maybe_cast_wrap (model_type *dest, ref_expression &expr)
{
  if (dest && dest != expr->type ())
    {
      ref_forwarding_type dtype
	= new model_forwarding_resolved (LOCATION_UNKNOWN, dest);
      model_cast *c = new model_cast (expr->get_location (), dtype, expr);
      c->already_resolved ();
      expr = c;
    }
}

model_type *
binary_numeric_promotion (model_type *lhs_type, model_type *rhs_type)
{
  if (! lhs_type->primitive_p ())
    {
      lhs_type = unboxing_conversion (lhs_type);
      if (! lhs_type)
	return NULL;
    }
  if (! rhs_type->primitive_p ())
    {
      rhs_type = unboxing_conversion (rhs_type);
      if (! rhs_type)
	return NULL;
    }

  if (! lhs_type->numeric_p () || ! rhs_type->numeric_p ())
    return NULL;

  if (lhs_type == primitive_double_type || rhs_type == primitive_double_type)
    return primitive_double_type;
  if (lhs_type == primitive_float_type || rhs_type == primitive_float_type)
    return primitive_float_type;
  if (lhs_type == primitive_long_type || rhs_type == primitive_long_type)
    return primitive_long_type;
  return primitive_int_type;
}

model_type *
binary_numeric_promotion (ref_expression &lhs, ref_expression &rhs)
{
  model_type *result = binary_numeric_promotion (lhs->type (), rhs->type ());
  if (result)
    {
      maybe_cast_wrap (result, lhs);
      maybe_cast_wrap (result, rhs);
    }
  return result;
}

model_type *
unary_numeric_promotion (model_type *op_type)
{
  if (! op_type->primitive_p ())
    {
      op_type = unboxing_conversion (op_type);
      if (! op_type)
	return NULL;
    }
  if (! op_type->numeric_p ())
    return NULL;
  if (op_type == primitive_byte_type || op_type == primitive_short_type
      || op_type == primitive_char_type)
    return primitive_int_type;
  return op_type;
}

model_type *
unary_numeric_promotion (ref_expression &expr)
{
  model_type *result = unary_numeric_promotion (expr->type ());
  if (result)
    maybe_cast_wrap (result, expr);
  return result;
}

model_type *
narrowing_primitive_conversion (model_type *to, model_type *from)
{
  if (! to->primitive_p () || ! from->primitive_p ()
      || to == primitive_boolean_type || from == primitive_boolean_type)
    return NULL;

  // We don't handle identity conversion as it is not narrowing.
  if (to == from)
    return NULL;

  if (to == primitive_double_type)
    return NULL;
  if (from == primitive_double_type)
    return to;

  if (to == primitive_float_type)
    return NULL;
  if (from == primitive_float_type)
    return to;

  if (to == primitive_long_type)
    return NULL;
  if (from == primitive_long_type)
    return to;

  if (to == primitive_int_type)
    return NULL;
  if (from == primitive_int_type)
    return to;

  if (to == primitive_short_type && from == primitive_byte_type)
    return NULL;
  // This is short/char to byte, byte/short to char, or char to short.
  return to;
}

model_type *
widening_primitive_conversion (model_type *to, model_type *from)
{
  if (! to->primitive_p () || ! from->primitive_p ())
    return NULL;
  if (to == from)
    return to;
  if (to == primitive_boolean_type || from == primitive_boolean_type)
    return NULL;

  if (to == primitive_double_type)
    return to;
  // Can't convert from double after this.
  if (from == primitive_double_type)
    return NULL;

  if (to == primitive_float_type)
    return to;
  if (from == primitive_float_type)
    return NULL;

  if (to == primitive_long_type)
    return to;
  if (from == primitive_long_type)
    return NULL;

  if (to == primitive_int_type)
    return to;
  if (from == primitive_int_type)
    return NULL;

  if (to == primitive_char_type || to == primitive_byte_type
      || from == primitive_char_type)
    return NULL;

  assert (to == primitive_short_type);
  assert (from == primitive_byte_type);
  return to;
}

bool
widening_reference_conversion (model_type *to, model_type *from)
{
  assert ((to->reference_p () || to == null_type)
	  && (from->reference_p () || from == null_type));

  // The null type can be assigned to any class type.
  if (from == null_type)
    return true;
  // No class type can be assigned to the null type.
  if (to == null_type)
    return false;

  // Arrays are compatible if their component types are.
  while (to->array_p () && from->array_p ())
    {
      to = to->element_type ();
      from = from->element_type ();
    }

  // A non-array can't be assigned to an array.
  if (to->array_p ())
    return false;

  // Primitive arrays must have identical component types.  We've
  // already stripped the array parts above.
  if (to->primitive_p () || from->primitive_p ())
    return to == from;

  // Anything can be assigned to Object.  Also, identity conversion.
  // FIXME: not sure checking the erasure is right here -- a type
  // variable may have multiple bounds.
  if (to->erasure () == global->get_compiler ()->java_lang_Object ()
      || to == from)
    return true;

  model_class *tok = assert_cast<model_class *> (to);
  model_class *fromk = assert_cast<model_class *> (from);

  // FIXME not at all clear we should do this here.  But where else?
  // this particular need came from method invocation conversion, but
  // there are other instances...
  tok->resolve_classes ();
  fromk->resolve_classes ();

  model_class *iter = fromk;
  bool iface = to->interface_p ();
  bool result = false;
  while (iter)
    {
      if (tok->erasure () == iter->erasure ())
	{
	  // At this point we have an assignment between two possible
	  // parameterizations with the same base.  Now check capture
	  // conversion.

	  // FIXME: this is probably wrong... check the spec.
	  if (tok->type_variable_p ())
	    result = true;
	  else if (! tok->parameterized_p ())
	    {
	      // This is an assignment to the raw type.
	      result = true;
	    }
	  else if (! iter->parameterized_p ())
	    {
	      // This is either two identical types or assignment of
	      // the raw type to a wildcard type.
	      // FIXME: do capture conversion here.
	      // FIXME: result = tok == iter. ... ?
	      result = true;
	    }
	  else
	    {
	      // FIXME: this is wrong.
	    }
	  // If we ended up here, we're done.
	  break;
	}

      if (iface)
	{
	  std::list<ref_forwarding_type> ifaces (iter->get_interfaces ());
	  for (std::list<ref_forwarding_type>::const_iterator i
		 = ifaces.begin ();
	       i != ifaces.end ();
	       ++i)
	    {
	      if (widening_reference_conversion (to, (*i)->type ()))
		return true;
	    }
	}
      iter = iter->get_superclass ();
    }

  // TO is an ancestor of FROM.
  return result;
}

model_type *
widening_conversion (model_type *to, model_type *from)
{
  if (to == primitive_void_type || from == primitive_void_type)
    return NULL;
  if (to->primitive_p () || from->primitive_p ())
    return widening_primitive_conversion (to, from);
  return widening_reference_conversion (to, from) ? to : NULL;
}

// This implements assignment conversion, both in the cases where an
// expression is known and when the expression is not; in the latter
// case RHS is NULL and we skip the special constant expression
// handling.
static model_type *
assignment_conversion (model_type *lhs_type, model_type *rhs_type,
		       model_expression *rhs)
{
  // Identity conversion.
  if (lhs_type == rhs_type)
    return lhs_type;

  // Narrowing conversion is ok in a specific situation: if the right
  // hand side is a constant expression whose value is representable
  // in the left hand side's type.  We might box the result.  We skip
  // this step if we don't have an expression.
  if (rhs != NULL)
    {
      model_type *unboxed_lhs = unboxing_conversion (lhs_type);
      if (unboxed_lhs == NULL)
	unboxed_lhs = lhs_type;
      if ((unboxed_lhs == primitive_byte_type
	   || unboxed_lhs == primitive_short_type
	   || unboxed_lhs == primitive_char_type)
	  && (rhs_type == primitive_byte_type
	      || rhs_type == primitive_short_type
	      || rhs_type == primitive_char_type
	      || rhs_type == primitive_int_type)
	  && rhs->constant_p ()
	  && representable_p (unboxed_lhs, rhs))
	return unboxed_lhs;
    }

  model_type *r = widening_conversion (lhs_type, rhs_type);
  if (r == NULL)
    {
      // We might have a boxing conversion followed by a widening
      // reference conversion, or an unboxing conversion followed by a
      // widening primitive conversion.  Just collapse them here.
      r = boxing_conversion (rhs_type);
      if (r == NULL)
	r = unboxing_conversion (rhs_type);
	    
      if (r != NULL)
	r = widening_conversion (lhs_type, r);
    }

  // FIXME: ought to apply unchecked conversion here.

  assert (r == NULL || r == lhs_type);
  return r;
}

model_type *
assignment_conversion (model_type *lhs, model_type *rhs)
{
  return assignment_conversion (lhs, rhs, NULL);
}

bool
representable_p (model_type *lhs_type, model_expression *rhs)
{
  model_primitive_base *lhs_base
    = assert_cast<model_primitive_base *> (lhs_type);
  model_primitive_base *int_base
    = assert_cast<model_primitive_base *> (primitive_int_type);
  return lhs_base->representable_p (int_base->convert (rhs->type (),
						       rhs->value ()));
}

model_type *
assignment_conversion (model_type *lhs_type, model_expression *rhs)
{
  return assignment_conversion (lhs_type, rhs->type (), rhs);
}

model_type *
assignment_conversion (model_type *lhs, ref_expression &rhs)
{
  model_type *result = assignment_conversion (lhs, rhs.get ());
  maybe_cast_wrap (result, rhs);
  return result;
}

model_type *
method_invocation_conversion (model_type *formal, model_type *actual,
			      method_phase phase)
{
  // Identity conversion.
  if (formal == actual)
    return formal;

  model_type *r = widening_conversion (formal, actual);
  if (r == NULL && phase != PHASE_1)
    {
      // Try a boxing or unboxing conversion followed by a widening
      // conversion.  Note that varargs are handled elsewhere.
      r = boxing_conversion (actual);
      if (r == NULL)
	r = unboxing_conversion (actual);
      if (r != NULL)
	r = widening_conversion (formal, r);
    }

  // FIXME: should do unchecked conversion here.

  return r;
}

void
method_invocation_conversion (model_type *formal, ref_expression &actual)
{
  model_type *result = method_invocation_conversion (formal, actual->type (),
						     PHASE_3);
  assert (result);
  maybe_cast_wrap (result, actual);
}

model_class *
box_primitive_type (model_type *in)
{
  if (in == primitive_boolean_type)
    return global->get_compiler ()->java_lang_Boolean ();
  if (in == primitive_byte_type)
    return global->get_compiler ()->java_lang_Byte ();
  if (in == primitive_char_type)
    return global->get_compiler ()->java_lang_Character ();
  if (in == primitive_double_type)
    return global->get_compiler ()->java_lang_Double ();
  if (in == primitive_float_type)
    return global->get_compiler ()->java_lang_Float ();
  if (in == primitive_int_type)
    return global->get_compiler ()->java_lang_Integer ();
  if (in == primitive_long_type)
    return global->get_compiler ()->java_lang_Long ();
  if (in == primitive_short_type)
    return global->get_compiler ()->java_lang_Short ();
  if (in == primitive_void_type)
    return global->get_compiler ()->java_lang_Void ();
  assert (0 && "unknown primitive type");
}

model_type *
boxing_conversion (model_type *in)
{
  if (! global->get_compiler ()->feature_boxing_conversion ()
      || ! in->primitive_p ())
    return NULL;
  return box_primitive_type (in);
}

model_type *
unboxing_conversion (model_type *in)
{
  if (! global->get_compiler ()->feature_boxing_conversion ())
    return NULL;

  if (in == global->get_compiler ()->java_lang_Boolean ())
    return primitive_boolean_type;
  if (in == global->get_compiler ()->java_lang_Byte ())
    return primitive_byte_type;
  if (in == global->get_compiler ()->java_lang_Short ())
    return primitive_short_type;
  if (in == global->get_compiler ()->java_lang_Character ())
    return primitive_char_type;
  if (in == global->get_compiler ()->java_lang_Integer ())
    return primitive_int_type;
  if (in == global->get_compiler ()->java_lang_Long ())
    return primitive_long_type;
  if (in == global->get_compiler ()->java_lang_Float ())
    return primitive_float_type;
  if (in == global->get_compiler ()->java_lang_Double ())
    return primitive_double_type;
  return NULL;
}

bool
boolean_conversion (model_type *in)
{
  return (in == primitive_boolean_type
	  || (global->get_compiler ()->feature_boxing_conversion ()
	      && in == global->get_compiler ()->java_lang_Boolean ()));
}

bool
boolean_conversion (ref_expression &expr)
{
  bool result = boolean_conversion (expr->type ());
  if (result)
    maybe_cast_wrap (primitive_boolean_type, expr);
  return result;
}

// FIXME: should pass in an element for error messages, since it
// might be an expression.
bool
casting_conversion (model_type *to, model_type *from)
{
  // fixme throw an error instead
  if (from->primitive_p () != to->primitive_p ()
      || from == primitive_void_type
      || to == primitive_void_type)
    return false;

  // Identity conversion.
  if (to == from)
    return true;

  if (narrowing_primitive_conversion (to, from)
      || widening_primitive_conversion (to, from))
    return true;

  // Boxing or unboxing conversion.
  if (to->primitive_p () && unboxing_conversion (from) == to)
    return true;
  if (from->primitive_p () && boxing_conversion (to) == from)
    return true;

  // After this point we only handle reference types.
  if (from->primitive_p ())
    return false;

  if (from == null_type)
    return true;

  from = from->erasure ();
  to = to->erasure ();

  model_type *obj = global->get_compiler ()->java_lang_Object ();

  if (from->array_p ())
    {
      if (to->array_p ())
	{
	  model_type *eltfrom = from->element_type ();
	  model_type *eltto = to->element_type ();
	  if (eltfrom->primitive_p () || eltto->primitive_p ())
	    {
	      // fixme better/more complete errors
	      if (eltfrom != eltto)
		return false;
	    }
	  else
	    // fixme could easily loop instead
	    return casting_conversion (eltto, eltfrom);
	}
      else if (to->interface_p ())
	{
	  if (! to->assignable_from_p (from))
	    return false;
	}
      else
	{
	  if (to != obj)
	    return false;
	}
    }
  else if (from->interface_p ())
    {
      if (to->array_p ())
	{
	  if (! from->assignable_from_p (to))
	    return false;
	}
      else if (to->interface_p ())
	{
	  // This is ok, unless the two interfaces have methods with
	  // the same name and parameters with conflicting return
	  // types.

	  // fixme: error message in this code.

	  model_class *fromc = assert_cast<model_class *> (from);
	  model_class *toc = assert_cast<model_class *> (to);

	  if (fromc->has_any_conflicting_interface_method_p (toc))
	    return false;
	}
      else if (to->final_p ())
	{
	  if (! from->assignable_from_p (to))
	    return false;
	}
    }
  else
    {
      // FROM is an ordinary class.
      if (to->array_p ())
	{
	  if (from != obj)
	    return false;
	}
      else if (to->interface_p ())
	{
	  if (from->final_p () && ! to->assignable_from_p (from))
	    {
	      return false;
	      // final class doesn't implement interface: error
	    }
	}
      else
	{
	  // TO is an ordinary class.
	  if (from != to
	      && ! from->assignable_from_p (to)
	      && ! to->assignable_from_p (from))
	    {
	      // Unrelated classes, hence error.
	      return false;
	    }
	}
    }

  return true;
}

bool
constant_type_p (model_type *t)
{
  // It requires more work to make this a method on model_type, but it
  // could be done.
  return (t->primitive_p ()
	  || t == global->get_compiler ()->java_lang_String ());
}

bool
convertible_to_numeric_p (model_type *t)
{
  if (t->numeric_p ())
    return true;
  t = unboxing_conversion (t);
  return t != NULL && t->numeric_p ();
}

bool
annotation_commensurate_p (model_type *type, model_expression *expr)
{
  if (type->array_p ())
    {
      model_type *elt = type->element_type ();
      model_annotation_initializer *ainit
	= dynamic_cast<model_annotation_initializer *> (expr);
      if (ainit != NULL)
	{
	  // Set the actual type of the initializer.
	  ainit->set_actual_type (type);

	  std::list<ref_expression> inits = ainit->get_initializers ();
	  for (std::list<ref_expression>::const_iterator i = inits.begin ();
	       i != inits.end ();
	       ++i)
	    {
	      if (! annotation_commensurate_p (elt, (*i).get ()))
		return false;
	    }
	  return true;
	}
      else
	return annotation_commensurate_p (elt, expr);
    }
  else
    {
      if (assignment_conversion (type, expr) == NULL)
	return false;

      if (type->primitive_p ()
	  || type == global->get_compiler ()->java_lang_String ())
	return expr->constant_p ();
      if (type->erasure () == global->get_compiler ()->java_lang_Class ())
	return dynamic_cast<model_class_ref *> (expr) != NULL;
      if (type->enum_p ())
	return dynamic_cast<model_enum_constant *> (expr);
    }

  return false;
}

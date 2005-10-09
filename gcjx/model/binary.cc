// Binary operators.

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

const char *
get_operator_name (operator_name n)
{
  switch (n)
    {
    case OP_PLUS:
      return "+";
    case OP_MINUS:
      return "-";
    case OP_MULTIPLY:
      return "*";
    case OP_DIVIDE:
      return "/";
    case OP_MOD:
      return "%";
    case OP_AND:
      return "&";
    case OP_OR:
      return "|";
    case OP_XOR:
      return "^";
    case OP_LEFT_SHIFT:
      return "<<";
    case OP_RIGHT_SHIFT:
      return ">>";
    case OP_UNSIGNED_RIGHT_SHIFT:
      return ">>>";
    case OP_EQUAL:
      return "==";
    case OP_NOT_EQUAL:
      return "!=";
    case OP_LESS_THAN:
      return "<";
    case OP_GREATER_THAN:
      return ">";
    case OP_LESS_EQUAL:
      return "<=";
    case OP_GREATER_EQUAL:
      return ">=";

    default:
      abort ();
    }
}



template<binary_function OP, operator_name NAME>
bool
model_arith_binary<OP, NAME>::compute_constant_p ()
{
  return model_binary_base::compute_constant_p ();
}

// Special rules for division operators.
template<>
bool
model_arith_binary<&model_primitive_base::divide,
		   OP_DIVIDE>::compute_constant_p ()
{
  if (! model_binary_base::compute_constant_p ())
    return false;
  if (! type ()->integral_p ())
    return true;

  model_primitive_base *bt
    = assert_cast<model_primitive_base *> (primitive_long_type);
  jlong val = bt->convert (rhs->type (), rhs->value ());
  return val != 0;
}

template<>
bool
model_arith_binary<&model_primitive_base::mod, OP_MOD>::compute_constant_p ()
{
  if (! model_binary_base::compute_constant_p ())
    return false;
  if (! type ()->integral_p ())
    return true;

  model_primitive_base *bt
    = assert_cast<model_primitive_base *> (primitive_long_type);
  jlong val = bt->convert (rhs->type (), rhs->value ());
  return val != 0;
}

template<binary_function OP, operator_name NAME>
void
model_arith_binary<OP, NAME>::resolve (resolution_scope *scope)
{
  model_binary_base::resolve (scope);
  model_type *result = binary_numeric_promotion (lhs, rhs);
  if (! result)
    // FIXME This is sort of a lame error -- we could explain which
    // argument is the problem.
    throw error ("operands to %1 aren't compatible via binary "
		 "numeric promotion")
      % get_name ();
  set_type (result);
}

template<binary_function OP, operator_name NAME>
jvalue
model_arith_binary<OP, NAME>::value ()
{
  model_primitive_base *bt = assert_cast<model_primitive_base *> (type ());
  jvalue left = bt->convert (lhs->type (), lhs->value ());
  jvalue right = bt->convert (rhs->type (), rhs->value ());
  return (bt->*OP) (this, left, right);
}

template<binary_function OP, operator_name NAME>
void
model_arith_binary<OP, NAME>::visit (visitor *v)
{
  v->visit_arith_binary (this, lhs, rhs);
}



template<binary_function OP, operator_name NAME>
void
model_bitwise_binary<OP, NAME>::resolve (resolution_scope *scope)
{
  model_binary_base::resolve (scope);

  if (boolean_conversion (lhs->type ()) || boolean_conversion (rhs->type ()))
    {
      if (! boolean_conversion (lhs) || ! boolean_conversion (rhs))
	throw error ("operands to %1 have incompatible type")
	  % get_name ();

      // This warning doesn't make sense for '^', since the point is
      // to look for typos.
      if (scope->warn_bitwise_boolean ()
	  && OP != &model_primitive_base::bxor)
	std::cerr << warn (global->get_compiler ()->warn_bitwise_boolean (),
			   "use of operator %1 with boolean operands")
	  % get_name ();
      set_type (primitive_boolean_type);
    }
  else
    {
      model_type *result = binary_numeric_promotion (lhs, rhs);
      if (! result)
	throw error ("operands to %1 aren't compatible via binary "
		     "numeric promotion")
	  % get_name ();
      if (! lhs->type ()->integral_p ())
	throw lhs->error ("operand to %1 must have integral type")
	  % get_name ();
      else if (! rhs->type ()->integral_p ())
	throw rhs->error ("operand to %1 must have integral type")
	  % get_name ();
      set_type (result);
    }
}

template<binary_function OP, operator_name NAME>
jvalue
model_bitwise_binary<OP, NAME>::value ()
{
  model_primitive_base *bt = assert_cast<model_primitive_base *> (type ());
  jvalue left = bt->convert (lhs->type (), lhs->value ());
  jvalue right = bt->convert (rhs->type (), rhs->value ());
  return (bt->*OP) (this, left, right);
}

template<binary_function OP, operator_name NAME>
void
model_bitwise_binary<OP, NAME>::visit (visitor *v)
{
  // FIXME: misnomer.
  v->visit_arith_binary (this, lhs, rhs);
}




void
model_plus::resolve (resolution_scope *scope)
{
  model_binary_base::resolve (scope);

  model_type *t;
  model_type *strtype
    = global->get_compiler ()->java_lang_String ();
  if (lhs->type () == strtype || rhs->type () == strtype)
    {
      if (lhs->type () == primitive_void_type)
	throw lhs->error ("operand to %1 can't have %<void%> type")
	  % get_name ();
      if (rhs->type () == primitive_void_type)
	throw rhs->error ("operand to %1 can't have %<void%> type")
	  % get_name ();

      t = strtype;
      if (scope->warn_string_plus_char_array ())
	{
	  model_type *chararray = primitive_char_type->array ();
	  if (lhs->type () == chararray)
	    std::cerr << lhs->warn (global->get_compiler ()->warn_string_plus_char_array (),
				    "expression of type %<char[]%> used in "
				    "String concatenation");
	  if (rhs->type () == chararray)
	    std::cerr << rhs->warn (global->get_compiler ()->warn_string_plus_char_array (),
				    "expression of type %<char[]%> used in "
				    "String concatenation");
	}
    }
  else
    {
      // FIXME: it would be better if binary_numeric_promotion told us
      // which operand was at fault.
      t = binary_numeric_promotion (lhs, rhs);
      if (t == NULL)
	throw error ("operand to %1 not convertible to numeric type")
	  % get_name ();
    }
  set_type (t);
}

std::string
model_plus::string_value ()
{
  model_type *strtype = global->get_compiler ()->java_lang_String ();
  assert (type_cache == strtype);

  std::string lstr;
  std::string rstr;

  model_type *ltype = lhs->type ();
  if (ltype == strtype)
    lstr = lhs->string_value ();
  else
    lstr = ltype->to_string (lhs->value ());

  model_type *rtype = rhs->type ();
  if (rtype == strtype)
    rstr = rhs->string_value ();
  else
    rstr = rtype->to_string (rhs->value ());

  return lstr + rstr;
}

jvalue
model_plus::value ()
{
  assert (type_cache != global->get_compiler ()->java_lang_String ());

  jvalue left = lhs->value ();
  model_primitive_base *ltype
    = assert_cast<model_primitive_base *> (lhs->type ());
  jvalue right = rhs->value ();
  model_primitive_base *rtype
    = assert_cast<model_primitive_base *> (rhs->type ());

  model_primitive_base *bt = assert_cast<model_primitive_base *> (type ());
  left = bt->convert (ltype, left);
  right = bt->convert (rtype, right);
  return bt->plus (this, left, right);
}

void
model_plus::visit (visitor *v)
{
  v->visit_arith_binary (this, lhs, rhs);
}



template<binary_function OP, operator_name NAME>
jvalue
model_arith_shift<OP, NAME>::value ()
{
  // Note that this isn't perfectly "java-like", in that we are in
  // effect doing binary promotion on the values.  This doesn't
  // matter, though.
  model_primitive_base *bt = assert_cast<model_primitive_base *> (type ());
  jvalue left = bt->convert (lhs->type (), lhs->value ());
  jvalue right = bt->convert (rhs->type (), rhs->value ());
  return (bt->*OP) (this, left, right);
}

template<binary_function OP, operator_name NAME>
void
model_arith_shift<OP, NAME>::resolve (resolution_scope *scope)
{
  model_binary_base::resolve (scope);
  model_type *nt = unary_numeric_promotion (rhs);
  if (nt == NULL)
    throw rhs->error ("couldn't apply unary numeric promotion "
		      "to expression of type %1")
      % rhs->type ();
  else if (! nt->integral_p ())
    throw rhs->error ("operand to %1 must have integral type")
      % get_name ();

  nt = unary_numeric_promotion (lhs);
  if (nt == NULL)
    throw lhs->error ("couldn't apply unary numeric promotion "
		      "to expression of type %1")
      % lhs->type ();
  else if (! nt->integral_p ())
    throw lhs->error ("operand to %1 must have integral type")
      % get_name ();
  set_type (nt);

  model_type *lhs_type = lhs->type ();
  assert (lhs_type == primitive_int_type || lhs_type == primitive_long_type);
  if (rhs->constant_p ())
    {
      model_primitive_base *plong
	= assert_cast<model_primitive_base *> (primitive_long_type);
      jlong val = plong->convert (rhs->type (), rhs->value ());
      int width = (lhs_type == primitive_int_type ? 32 : 64);
      if (val < 0)
	{
	  if (scope->warn_negative_shift ())
	    std::cerr
	      << rhs->warn (global->get_compiler ()->warn_negative_shift (),
			    "negative shift count");
	}
      else if (val == 0)
	{
	  if (scope->warn_zero_shift ())
	    std::cerr
	      << rhs->warn (global->get_compiler ()->warn_zero_shift (),
			    "shift count is zero");
	}
      else if (val >= width)
	{
	  if (scope->warn_large_shift ())
	    std::cerr
	      << rhs->warn (global->get_compiler ()->warn_large_shift (),
			    "shift count of %1 is wider than type "
			    "width of %2")
	      // FIXME: this is a bit inefficient, should have a
	      // separate operator% for jlong.
	      % primitive_format (val) % primitive_format (width);
	}
    }
}

template<binary_function OP, operator_name NAME>
void
model_arith_shift<OP, NAME>::visit (visitor *v)
{
  v->visit_arith_shift (this, lhs, rhs);
}



template<comparator OP, operator_name NAME>
jvalue
model_equality_comparison<OP, NAME>::value ()
{
  if (promoted_type == global->get_compiler ()->java_lang_String ())
    {
      jint comp = lhs->string_value () == rhs->string_value ();
      model_primitive_base *bt
	= assert_cast<model_primitive_base *> (primitive_int_type);
      // We set things up so that we can use the ordinary comparison
      // operator to yield the correct result.  If OP is '==' and the
      // two strings are equal, then both LEFT and RIGHT will be 1 and
      // we'll get the right result -- and likewise for the other 3
      // cases.
      jvalue left = jint (1);
      jvalue right = comp;
      return (bt->*OP) (left, right);
    }
  else
    {
      model_primitive_base *pt
	= assert_cast<model_primitive_base *> (promoted_type);
      jvalue left = pt->convert (lhs->type (), lhs->value ());
      jvalue right = pt->convert (rhs->type (), rhs->value ());
      return (pt->*OP) (left, right);
    }
}

template<comparator OP, operator_name NAME>
void
model_equality_comparison<OP, NAME>::visit (visitor *v)
{
  v->visit_comparison (this, lhs, rhs);
}

template<comparator OP, operator_name NAME>
void
model_equality_comparison<OP, NAME>::resolve (resolution_scope *scope)
{
  model_binary_base::resolve (scope);
  model_type *ltype = lhs->type ();
  model_type *rtype = rhs->type ();

  // If one is numeric and the other is convertible to a numeric type,
  // then we do numeric comparison.  Note that if one is numeric and
  // the other is not convertible, we do not do a boxing conversion,
  // but instead give an error.
  if ((ltype->numeric_p () && convertible_to_numeric_p (rtype))
      || (rtype->numeric_p () && convertible_to_numeric_p (ltype)))
    {
      promoted_type = binary_numeric_promotion (lhs, rhs);
      assert (promoted_type);
    }
  // Likewise, if both operands are Boolean, we do not unbox.
  else if ((ltype == primitive_boolean_type && boolean_conversion (rtype))
	   || (rtype == primitive_boolean_type && boolean_conversion (ltype)))
    {
      // Explicitly unwrap if needed.
      maybe_cast_wrap (primitive_boolean_type, lhs);
      maybe_cast_wrap (primitive_boolean_type, rhs);
      promoted_type = primitive_boolean_type;
    }
  else if ((ltype != null_type && ! ltype->reference_p ())
	   || (rtype != null_type && ! rtype->reference_p ()))
    throw error ("incompatible types %1 and %2 to operator %3")
      % ltype % rtype % get_name ();
  else
    {
      // Both reference types.
      if (casting_conversion (ltype, rtype))
	promoted_type = ltype;
      else if (casting_conversion (rtype, ltype))
	promoted_type = rtype;
      else
	throw error ("types %1 and %2 are not compatible "
		     "via casting conversion")
	  % rtype % ltype;
    }

  set_type (primitive_boolean_type);
}



template<comparator OP, operator_name NAME>
jvalue
model_numeric_comparison<OP, NAME>::value ()
{
  model_primitive_base *pt
    = assert_cast<model_primitive_base *> (promoted_type);
  jvalue left = pt->convert (lhs->type (), lhs->value ());
  jvalue right = pt->convert (rhs->type (), rhs->value ());
  return (pt->*OP) (left, right);
}

template<comparator OP, operator_name NAME>
void
model_numeric_comparison<OP, NAME>::visit (visitor *v)
{
  v->visit_comparison (this, lhs, rhs);
}

template<comparator OP, operator_name NAME>
void
model_numeric_comparison<OP, NAME>::resolve (resolution_scope *scope)
{
  model_binary_base::resolve (scope);

  // FIXME: can't tell which argument was the problem.
  // Could give more precise error message.
  promoted_type = binary_numeric_promotion (lhs, rhs);

  if (promoted_type == NULL)
    throw error ("operand to %1 not convertible to numeric type")
      % get_name ();

  set_type (primitive_boolean_type);
}



void
model_logical_binary::resolve (resolution_scope *scope)
{
  model_binary_base::resolve (scope);
  if (! boolean_conversion (lhs))
    throw lhs->error ("boolean type required as operand to %1")
      % get_name ();
  if (! boolean_conversion (rhs))
    throw rhs->error ("boolean type required as operand to %1")
      % get_name ();
  set_type (primitive_boolean_type);
}

void
model_lor::visit (visitor *v)
{
  v->visit_logical_binary (this, lhs, rhs);
}

void
model_land::visit (visitor *v)
{
  v->visit_logical_binary (this, lhs, rhs);
}



// Instantiations.

template class model_arith_shift<&model_primitive_base::left_shift,
				 OP_LEFT_SHIFT>;
template class model_arith_shift<&model_primitive_base::right_shift,
				 OP_RIGHT_SHIFT>;
template class model_arith_shift<&model_primitive_base::unsigned_right_shift,
				 OP_UNSIGNED_RIGHT_SHIFT>;

template class model_arith_binary<&model_primitive_base::subtract,
				  OP_MINUS>;
template class model_arith_binary<&model_primitive_base::multiply,
				  OP_MULTIPLY>;
template class model_arith_binary<&model_primitive_base::divide,
				  OP_DIVIDE>;
template class model_arith_binary<&model_primitive_base::mod,
				  OP_MOD>;
template class model_bitwise_binary<&model_primitive_base::band, OP_AND>;
template class model_bitwise_binary<&model_primitive_base::bor, OP_OR>;
template class model_bitwise_binary<&model_primitive_base::bxor, OP_XOR>;

template class model_equality_comparison<&model_primitive_base::equal,
					 OP_EQUAL>;
template class model_equality_comparison<&model_primitive_base::not_equal,
					 OP_NOT_EQUAL>;
template class model_numeric_comparison<&model_primitive_base::less_than,
					OP_LESS_THAN>;
template class model_numeric_comparison<&model_primitive_base::greater_than,
					OP_GREATER_THAN>;
template class model_numeric_comparison<&model_primitive_base::less_than_equal,
					OP_LESS_EQUAL>;
template class model_numeric_comparison<&model_primitive_base::greater_than_equal,
					OP_GREATER_EQUAL>;

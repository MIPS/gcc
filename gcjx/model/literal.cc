// Literals.

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

template<typename T, model_type **RT>
void
model_simple_lit<T, RT>::visit (visitor *v)
{
  v->visit_simple_literal (this, val);
}



void
model_string_literal::resolve (resolution_scope *)
{
  set_type (global->get_compiler ()->java_lang_String ());
}

void
model_string_literal::visit (visitor *v)
{
  v->visit_string_literal (this, val);
}



ref_expression
create_literal (const ref_expression &expr)
{
  // FIXME: there is probably a better way to write this, involving
  // templates or methods of primitive types.
  model_type *t = expr->type ();
  ref_expression result;
  if (t == primitive_boolean_type)
    result = new model_boolean_literal (expr->get_location (),
					jboolean (expr->value ()));
  else if (t == primitive_byte_type)
    result = new model_byte_literal (expr->get_location (),
				     jbyte (expr->value ()));
  else if (t == primitive_char_type)
    result = new model_char_literal (expr->get_location (),
				     jchar (expr->value ()));
  else if (t == primitive_short_type)
    result = new model_short_literal (expr->get_location (),
				      jshort (expr->value ()));
  else if (t == primitive_int_type)
    result = new model_int_literal (expr->get_location (),
				    jint (expr->value ()));
  else if (t == primitive_long_type)
    result = new model_long_literal (expr->get_location (),
				     jlong (expr->value ()));
  else if (t == primitive_float_type)
    result = new model_float_literal (expr->get_location (),
				      jfloat (expr->value ()));
  else if (t == primitive_double_type)
    result = new model_double_literal (expr->get_location (),
				       jdouble (expr->value ()));
  else
    {
      // String value.
      assert (t == global->get_compiler ()->java_lang_String ());
      result = new model_string_literal (expr->get_location (),
					 expr->string_value ());
    }

  // This will resolve any literal.
  result->resolve (NULL);

  return result;
}



// Instantiations.

template class model_simple_lit<jchar, &primitive_char_type>;
template class model_simple_lit<jbyte, &primitive_byte_type>;
template class model_simple_lit<jshort, &primitive_short_type>;
template class model_simple_lit<jint, &primitive_int_type>;
template class model_simple_lit<jlong, &primitive_long_type>;
template class model_simple_lit<jfloat, &primitive_float_type>;
template class model_simple_lit<jdouble, &primitive_double_type>;
template class model_simple_lit<jboolean, &primitive_boolean_type>;

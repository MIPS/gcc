// Wrap a pointer and provide ownership.

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

#include "bytecode/block.hh"

template<typename T>
void
owner<T>::ref () const
{
  if (value)
    ++value->refc;
}

template<typename T>
void
owner<T>::deref ()
{
  if (value != NULL)
    {
      --value->refc;
      if (value->refc == 0)
	{
	  delete value;
	  value = NULL;
	}
    }
}

template<typename T>
owner<T> &
owner<T>::operator= (T *nv)
{
  if (nv != NULL)
    ++nv->refc;
  deref ();
  value = nv;
  return *this;
}



// Instantiations.

template class owner<relocation>;

template class owner<format_repr>;

template class owner<model_package>;
template class owner<model_unit>;
template class owner<model_annotation>;
template class owner<model_class>;
template class owner<model_class_instance>;
template class owner<model_unnamed_package>;
template class owner<model_expression>;
template class owner<model_forwarding_type>;
template class owner<model_forwarding_parameterized>;
template class owner<model_method>;
template class owner<model_field>;
template class owner<model_type_variable>;
template class owner<model_block>;
template class owner<model_variable_decl>;
template class owner<model_array_type>;
template class owner<model_element>;
template class owner<model_identifier>;
template class owner<model_assignment>;
template class owner<model_double_literal>;
template class owner<model_int_literal>;
template class owner<model_float_literal>;
template class owner<model_long_literal>;
template class owner<model_string_literal>;
template class owner<model_char_literal>;
template class owner<model_javadoc>;
template class owner<model_invocation_base>;
template class owner<model_unary>;
template class owner<model_binary_base>;
template class owner<model_for_base>;
template class owner<model_import>;
template class owner<model_stmt>;
template class owner<model_catch>;
template class owner<model_switch_block>;
template class owner<model_enum>;
template class owner<model_enum_constant>;
template class owner<model_super_invocation>;
template class owner<model_variable_stmt>;
template class owner<model_modifier_list>;
template class owner<model_throw>;
template class owner<model_return>;
template class owner<model_continue>;
template class owner<model_break>;
template class owner<model_expression_stmt>;
template class owner<model_switch>;
template class owner<model_if>;
template class owner<model_try>;
template class owner<model_do>;
template class owner<model_label>;
template class owner<model_empty>;
template class owner<model_synchronized>;
template class owner<model_while>;
template class owner<model_assert>;
template class owner<model_array_ref>;
template class owner<model_field_ref>;
template class owner<model_method_invocation>;
template class owner<model_memberref_forward>;
template class owner<model_new_array>;
template class owner<model_new>;
template class owner<model_new_primary>;
template class owner<model_type>;
template class owner<model_simple_variable_ref>;
template class owner<model_wildcard>;
template class owner<model_annotation_value>;
template class owner<model_annotation_member>;
template class owner<model_annotation_type>;
template class owner<model_bytecode_block>;

// Primitive types.

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

bool
model_primitive_base::assignable_from_p (model_type *other)
{
  if (! other->primitive_p ())
    return false;
  return assignment_conversion (this, other) != NULL;
}

void
model_primitive_base::visit (visitor *v)
{
  v->visit_primitive (this, pretty_name);
}

template<typename T, char sig_char, long long MIN, long long MAX>
jvalue
model_int_primitive<T,sig_char,MIN,MAX>::divide (model_element *request,
						 const jvalue &l,
						 const jvalue &r)
{
  assert ((T) r != 0);
  if ((T) l == MIN && (T) r == -1)
    return l;
  return (T) l / (T) r;
}



// Instantiations.

template class model_int_primitive<jbyte, 'B', -128, 127>;
template class model_int_primitive<jchar, 'C', 0, 65536>;
template class model_int_primitive<jshort, 'S', -32768, 32767>;
template class model_int_primitive<jint, 'I', -0x80000000LL, 0x7fffffffLL>;
template class model_int_primitive<jlong, 'J', -0x8000000000000000LL, 0x7fffffffffffffffLL>;

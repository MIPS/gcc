// Initialize various things.

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

model_type *primitive_boolean_type;
model_type *primitive_byte_type;
model_type *primitive_char_type;
model_type *primitive_short_type;
model_type *primitive_int_type;
model_type *primitive_long_type;
model_type *primitive_float_type;
model_type *primitive_double_type;
model_void_type *primitive_void_type;

model_null_type *null_type;

global_state *global;

void
initialize_primitive_types ()
{
  primitive_boolean_type = new model_primitive_boolean ();

  primitive_byte_type
    = new model_int_primitive<jbyte, 'B', -128, 127> ("byte");
  primitive_char_type
    = new model_int_primitive<jchar, 'C', 0, 65536> ("char");
  primitive_short_type
    = new model_int_primitive<jshort, 'S', -32768, 32767> ("short");
  primitive_int_type
    = new model_int_primitive<jint, 'I', MIN_INT, MAX_INT> ("int");
  primitive_long_type
    = new model_int_primitive<jlong, 'J', MIN_LONG, MAX_LONG> ("long");

  primitive_float_type = new model_fp_primitive<jfloat, 'F'> ("float");
  primitive_double_type = new model_fp_primitive<jdouble, 'D'> ("double");

  primitive_void_type = new model_void_type ();
  null_type = new model_null_type ();

  global = new global_state ();
}

// Constant pool.

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

std::string
constant_pool::get_utf8 (uint16 index)
{
  if (! valid_p (index) || tag (index) != CONSTANT_Utf8)
    throw class_file_error (where, "expected CONSTANT_Utf8 tag at index %1")
      % int (index);
  return utf_data[index];
}

std::string
constant_pool::get_string (uint16 index)
{
  if (! valid_p (index) || tag (index) != CONSTANT_String)
    throw class_file_error (where, "expected CONSTANT_String tag at index %1")
      % int (index);
  return get_utf8 (value (index));
}

std::string
constant_pool::get_class (uint16 index)
{
  if (! valid_p (index) || tag (index) != CONSTANT_Class)
    throw class_file_error (where, "expected CONSTANT_Class tag at index %1")
      % int (index);
  return get_utf8 (value (index));
}

void
constant_pool::get_name_and_type (uint16 index,
				  std::string &name,
				  std::string &type)
{
  if (! valid_p (index) || tag (index) != CONSTANT_NameAndType)
    throw class_file_error (where,
			    "expected CONSTANT_NameAndType tag at index %1")
      % int (index);
  name = get_utf8 (value (index) & 0xffff);
  type = get_utf8 ((value (index) >> 16) & 0xffff);
}

void
constant_pool::get_fieldref (uint16 index, std::string &class_name,
			     std::string &field_name,
			     std::string &descriptor)
{
  if (! valid_p (index) || tag (index) != CONSTANT_Fieldref)
    throw class_file_error (where, "expected CONSTANT_Fieldref tag at index %1")
      % int (index);
  class_name = get_class (value (index) & 0xffff);
  get_name_and_type ((value (index) >> 16) & 0xffff, field_name, descriptor);
}

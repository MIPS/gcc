// The void type.

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

#ifndef GCJX_MODEL_VOID_HH
#define GCJX_MODEL_VOID_HH

/// This represents the 'void' type.  There is only a single instance
/// of the void type in a program -- primitive_void_type is one of the
/// few globals in gcjx.  Note that we don't override erasure().  It
/// does make sense to ask for the erasure of the void type.
class model_void_type : public model_type
{
public:

  model_void_type ()
    : model_type (LOCATION_UNKNOWN)
  {
    descriptor = "V";
  }

  // It doesn't make sense to try to construct an array here.
  model_array_type *array ()
  {
    abort ();
  }

  bool assignable_from_p (model_type *other)
  {
    return false;
  }

  bool primitive_p () const
  {
    return false;
  }

  std::string to_string (const jvalue &) const
  {
    abort ();
  }

  std::string get_pretty_name () const
  {
    return std::string ("void");
  }

  void visit (visitor *);
};

extern model_void_type *primitive_void_type;

#endif // GCJX_MODEL_VOID_HH

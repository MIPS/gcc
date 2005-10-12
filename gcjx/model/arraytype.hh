// Represent an array type.

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

#ifndef GCJX_MODEL_ARRAYTYPE_HH
#define GCJX_MODEL_ARRAYTYPE_HH

class model_array_type : public model_class
{
  // The element type.
  model_type *element;

public:

  model_array_type (model_type *elt);

  model_type *element_type ()
  {
    return element;
  }

  model_type *erasure ();

  bool array_p () const
  {
    return true;
  }

  std::string get_pretty_name () const;

  std::string get_signature ()
  {
    return "[" + element->get_signature ();
  }

  void check_instantiation (model_element *)
  {
    // This should not be called, since the parser should not create
    // an ordinary 'new' when the argument type is an array.
    abort ();
  }

  void visit (visitor *);
};

#endif // GCJX_MODEL_ARRAYTYPE_HH

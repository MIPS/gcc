// Array references.

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

#ifndef GCJX_MODEL_ARRAYREF_HH
#define GCJX_MODEL_ARRAYREF_HH

class model_array_ref : public model_expression
{
  // The array expression.
  ref_expression array;

  // The index expression.
  ref_expression index;

  bool compute_constant_p ()
  {
    return false;
  }

public:

  model_array_ref (const location &w)
    : model_expression (w)
  {
  }

  void set_array (const ref_expression &a)
  {
    array = a;
  }

  void set_index (const ref_expression &e)
  {
    index = e;
  }

  model_expression *get_array () const
  {
    return array.get ();
  }

  model_expression *get_index () const
  {
    return index.get ();
  }

  void resolve (resolution_scope *);

  void visit (visitor *);

  void set_left_hand_side (bool)
  {
    // Nothing.
  }
};

#endif // GCJX_MODEL_ARRAYREF_HH

// Represent an annotation type member declaration.

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

#ifndef GCJX_MODEL_ANNOMEMBER_HH
#define GCJX_MODEL_ANNOMEMBER_HH

class model_annotation_member : public model_annotation_value,
				public IModifiable
{
  // The type of this member.
  ref_forwarding_type type;

  bool type_ok_p ();

protected:

  void massage_modifiers (const ref_modifier_list &);

public:

  model_annotation_member (const location &w, const std::string &n,
			   const ref_expression &ex,
			   const ref_forwarding_type &t)
    : model_annotation_value (w, n, ex),
      type (t)
  {
  }

  model_type *get_type ()
  {
    return type->type ();
  }

  /// This accessor is used when constructing the equivalent method.
  ref_forwarding_type get_forwarding_type ()
  {
    return type;
  }

  void resolve (resolution_scope *);

  void visit (visitor *);
};

#endif // GCJX_MODEL_ANNOMEMBER_HH

// A reference to a ".class" "constant"

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

#ifndef GCJX_MODEL_CLASSREF_HH
#define GCJX_MODEL_CLASSREF_HH

/// This represents a class literal expression.
class model_class_ref : public model_expression
{
  // The class to which we refer.
  ref_forwarding_type klass;

  bool compute_constant_p ()
  {
    return false;
  }

public:

  model_class_ref (const location &w, const ref_forwarding_type &k)
    : model_expression (w),
      klass (k)
  {
  }

  model_class_ref (const location &w, model_type *k)
    : model_expression (w),
      klass (new model_forwarding_resolved (w, k))
  {
  }

  model_class_ref (const location &w, const std::list<std::string> &idlist)
    : model_expression (w),
      klass (new model_forwarding_simple (w, idlist))
  {
  }

  model_type *get_class () const
  {
    return klass->type ();
  }

  void resolve (resolution_scope *);

  void visit (visitor *);
};

#endif // GCJX_MODEL_CLASSREF_HH

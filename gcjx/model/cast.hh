// Represent a cast operation.

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

#ifndef GCJX_MODEL_CAST_HH
#define GCJX_MODEL_CAST_HH

class model_cast : public model_unary
{
  // Type to cast to.
  ref_forwarding_type target;

  bool compute_constant_p ();

  std::string get_name () const
  {
    abort ();
  }

public:

  model_cast (const location &w, const ref_forwarding_type &t)
    : model_unary (w),
      target (t)
  {
  }

  model_cast (const location &w,
	      const ref_forwarding_type &t,
	      const ref_expression &e)
    : model_unary (w, e),
      target (t)
  {
  }

  jvalue value ();

  std::string string_value ();

  void resolve (resolution_scope *scope);

  // This is used when a new cast is created by a conversion.  It
  // indicates that we should set our type without trying to resolve
  // our children.
  void already_resolved ();

  void visit (visitor *);
};

#endif // GCJX_MODEL_CAST_HH

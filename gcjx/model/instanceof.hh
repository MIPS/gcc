// Represent the instanceof operator.

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

#ifndef GCJX_MODEL_INSTANCEOF_HH
#define GCJX_MODEL_INSTANCEOF_HH

/// This represents the 'instanceof' expression.  It holds an
/// expression and a type against which to compare.
class model_instanceof : public model_expression
{
  // The expression.
  ref_expression expr;

  // The type to check against.
  ref_forwarding_type type;

  bool compute_constant_p ()
  {
    return false;
  }

public:

  model_instanceof (const location &w,
		    const ref_expression &e,
		    const ref_forwarding_type &t)
    : model_expression (w),
      expr (e),
      type (t)
  {
  }

  void resolve (resolution_scope *);

  void visit (visitor *);
};

#endif // GCJX_MODEL_INSTANCEOF_HH

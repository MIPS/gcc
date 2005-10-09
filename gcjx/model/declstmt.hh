// Class or interface declaration statement.

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

#ifndef GCJX_MODEL_DECLSTMT_HH
#define GCJX_MODEL_DECLSTMT_HH

/// This represents a class declaration statement, that is, the
/// declaration of a class local to a method.
class model_class_decl_stmt : public model_stmt
{
  // The class being declared.
  // FIXME do we own this?
  ref_class decl;

public:

  model_class_decl_stmt (const location &w, const ref_class &k)
    : model_stmt (w),
      decl (k)
  {
  }

  void resolve (resolution_scope *);

  void compute_normal_completion (normal_completion_state &)
  {
    normal_completion = true;
  }

  void visit (visitor *v);
};

#endif // GCJX_MODEL_DECLSTMT_HH

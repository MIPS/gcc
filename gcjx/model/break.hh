// Represent the break statement.

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

#ifndef GCJX_MODEL_BREAK_HH
#define GCJX_MODEL_BREAK_HH

class model_break : public model_stmt
{
  // The statement we break out of.
  Ilabel *target;

  // The actual target statement of the break.
  model_stmt *actual;

public:

  model_break (const location &w)
    : model_stmt (w),
      target (NULL),
      actual (NULL)
  {
  }

  void resolve (resolution_scope *);

  void set_target (Ilabel *t)
  {
    target = t;
  }

  model_stmt *get_target () const
  {
    return assert_cast<model_stmt *> (target);
  }

  void compute_normal_completion (normal_completion_state &);

  void visit (visitor *v);
};

#endif // GCJX_MODEL_BREAK_HH

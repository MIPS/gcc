// Represent a field initializer.

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

#ifndef GCJX_MODEL_FIELDINIT_HH
#define GCJX_MODEL_FIELDINIT_HH

/// Field initializations are handled a bit strangely.  They are
/// attached to the field declarations, but they also have code
/// generated for them in special synthetic methods.  When adding a
/// field to a class, we create new field initializer statements.
/// This lets us properly order field initializers with (static or
/// instance) initializer blocks.  Also, we may create a
/// model_field_initializer even if the field in question does not
/// have an initializer.  In this case the "initializer" functions
/// merely as a placeholder; this makes "use before declaration"
/// checking simpler.
class model_field_initializer : public model_stmt
{
  // The field we're initializing.
  model_field *field;

public:

  model_field_initializer (const location &w, model_field *f)
    : model_stmt (w),
      field (f)
  {
    assert (field);
  }

  model_field *get_field () const
  {
    return field;
  }

  void resolve (resolution_scope *);

  void compute_normal_completion (normal_completion_state &)
  {
    normal_completion = true;
  }

  void visit (visitor *);
};

#endif // GCJX_MODEL_FIELDINIT_HH

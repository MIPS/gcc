// Represent a catch clause.

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

#ifndef GCJX_MODEL_CATCH_HH
#define GCJX_MODEL_CATCH_HH

// We treat a catch block as a variable declaration with an embedded
// block.
class model_catch : public model_variable_stmt
{
  // The included block.
  ref_block block;

public:

  model_catch (const location &w)
    : model_variable_stmt (w)
  {
  }

  void set_formal_parameter (const ref_variable_decl &d)
  {
    assert (vars.empty ());
    vars.push_back (d);
  }

  model_type *get_parameter_type () const
  {
    assert (vars.size () == 1);
    return vars.front ()->type ();
  }

  void set_block (const ref_block &b)
  {
    block = b;
  }

  void resolve (resolution_scope *);

  void compute_normal_completion (normal_completion_state &);

  void visit (visitor *v);
};

#endif // GCJX_MODEL_CATCH_HH

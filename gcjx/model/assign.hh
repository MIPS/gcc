// Represent an assignment operator.

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

#ifndef GCJX_MODEL_ASSIGN_HH
#define GCJX_MODEL_ASSIGN_HH

/// This class represents an assignment operation.  It is also the
/// base class for other kinds of assignment operations.
class model_assignment : public model_binary_base
{
  bool compute_constant_p ()
  {
    return false;
  }

protected:

  // A helper method to check that the LHS has a valid form.
  void check_lhs (const char *);

public:

  model_assignment (const location &w)
    : model_binary_base (w)
  {
  }

  void resolve (resolution_scope *);

  void visit (visitor *);
};

// This handles all the different 'op=' compound assignment operators.
// Since 'a op= b' is the same as 'a = (lhs_type) (a op b)', we just
// reuse logic from the binary operators.  The type argument here is
// the class of the corresponding binary operator.
template<typename T>
class model_op_assignment : public model_assignment
{
  void extra_check ();

public:

  model_op_assignment (const location &w)
    : model_assignment (w)
  {
  }

  void resolve (resolution_scope *);
  void visit (visitor *);
};

typedef model_op_assignment<model_plus> model_plus_equal;
typedef model_op_assignment<model_minus> model_minus_equal;
typedef model_op_assignment<model_mult> model_mult_equal;
typedef model_op_assignment<model_div> model_div_equal;
typedef model_op_assignment<model_and> model_and_equal;
typedef model_op_assignment<model_or> model_or_equal;
typedef model_op_assignment<model_xor> model_xor_equal;
typedef model_op_assignment<model_mod> model_mod_equal;
typedef model_op_assignment<model_left_shift> model_ls_equal;
typedef model_op_assignment<model_right_shift> model_rs_equal;
typedef model_op_assignment<model_unsigned_right_shift> model_urs_equal;

#endif // GCJX_MODEL_ASSIGN_HH

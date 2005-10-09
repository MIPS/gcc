// Represent the conditional operator.

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

#ifndef GCJX_MODEL_COND_HH
#define GCJX_MODEL_COND_HH

/// This represents a conditional expression.
class model_conditional : public model_expression
{
  // The condition.
  ref_expression condition;

  // True branch.
  ref_expression true_branch;

  // False branch.
  ref_expression false_branch;

  bool compute_constant_p ()
  {
    return (condition->constant_p ()
	    && true_branch->constant_p ()
	    && false_branch->constant_p ());
  }

public:

  model_conditional (const location &w,
		     const ref_expression &c,
		     const ref_expression &t,
		     const ref_expression &f)
    : model_expression (w),
      condition (c),
      true_branch (t),
      false_branch (f)
  {
  }

  void set_condition (const ref_expression &c)
  {
    condition = c;
  }

  void set_true (const ref_expression &t)
  {
    true_branch = t;
  }

  void set_false (const ref_expression &f)
  {
    false_branch = f;
  }

  void resolve (resolution_scope *scope);

  void visit (visitor *);

  jvalue value ();

  std::string string_value ();
};

#endif // GCJX_MODEL_COND_HH

// Represent the continue statement.

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

#ifndef GCJX_MODEL_CONTINUE_HH
#define GCJX_MODEL_CONTINUE_HH

class model_continue : public model_stmt
{
  // The loop we continue.
  Ilabel *target;

  // The statement representing the actual continue target.
  model_loop *actual;

public:

  model_continue (const location &w)
    : model_stmt (w),
      target (NULL),
      actual (NULL)
  {
  }

  void resolve (resolution_scope *)
  {
    if (! target)
      {
	// We already gave an error at parse time.
      }
    else
      {
	actual = target->get_continue_target ();
	if (! actual)
	  {
	    // fixme: name of the target?
	    std::cerr << error ("target of %<continue%> statement "
				"is not a loop");
	    model_stmt *real = assert_cast<model_stmt *> (target);
	    std::cerr << real->error ("target is here");
	  }
      }
  }

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

#endif // GCJX_MODEL_CONTINUE_HH

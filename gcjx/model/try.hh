// Represent the try statement.

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

#ifndef GCJX_MODEL_TRY_HH
#define GCJX_MODEL_TRY_HH

/// This represents a try/finally statement.  It holds the try block,
/// a possibly-empty list of catch clauses, and an optional finally
/// block.
class model_try : public model_stmt, public ICatcher, public IScope
{
  // The block to attempt.
  ref_block block;

  // Each catch clause.  This list can be empty.
  std::list<ref_catch> catchers;

  // finally block.  This is null if there is no 'finally' clause.
  ref_block finally;

  // Flag determining where a thrown exception should be recorded.
  bool flag;

  // Possibly-thrown checked exceptions found in the block.
  std::set<model_type *> checked_exceptions;

  // Possibly-thrown checked exceptions found in the catch blocks.
  std::set<model_type *> catch_block_exceptions;

  // Our enclosing catcher.  This will never be NULL during
  // resolution, as the enclosing method is always a catcher.
  ICatcher *enclosing_catcher;

  // This is true if we have a 'finally' block and it cannot complete
  // normally.
  watch<bool> finally_completion;


  // Helper method that propagates exceptions during resolution.
  void propagate_exceptions ();

public:

  model_try (const location &w)
    : model_stmt (w),
      flag (false),
      enclosing_catcher (NULL)
  {
  }

  void set_block (const ref_block &b)
  {
    block = b;
  }

  void set_finally (const ref_block &b)
  {
    finally = b;
  }

  void set_catchers (const std::list<ref_catch> &c)
  {
    catchers = c;
  }

  void resolve (resolution_scope *);

  void note_throw_type (model_type *exc_type)
  {
    if (flag)
      catch_block_exceptions.insert (exc_type);
    else
      checked_exceptions.insert (exc_type);
  }

  void compute_normal_completion (normal_completion_state &);

  bool finally_can_complete_normally () const
  {
    return finally ? finally->can_complete_normally () : true;
  }

  void visit (visitor *v);
};

#endif // GCJX_MODEL_TRY_HH

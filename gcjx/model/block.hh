// Represent a block.

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

#ifndef GCJX_MODEL_BLOCK_HH
#define GCJX_MODEL_BLOCK_HH

/// This represents a block of statements.
class model_block : public model_stmt
{
protected:

  // The statements in the block.
  std::list<ref_stmt> statements;

public:

  model_block (const location &w, const std::list<ref_stmt> &s)
    : model_stmt (w),
      statements (s)
  {
  }

  model_block (const location &w)
    : model_stmt (w)
  {
  }

  /// Set the statements in the block.
  void set_statements (const std::list<ref_stmt> &s)
  {
    statements = s;
  }

  /// Get the list of statements in the block.
  std::list<ref_stmt> get_statements () const
  {
    return statements;
  }

  void resolve (resolution_scope *);

  void compute_normal_completion (normal_completion_state &);

  void visit (visitor *v);
};

#endif // GCJX_MODEL_BLOCK_HH

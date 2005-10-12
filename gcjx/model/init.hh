// Represent an initializer block.

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

#ifndef GCJX_MODEL_INIT_HH
#define GCJX_MODEL_INIT_HH

class model_initializer_block : public model_block
{
  // This class does not inherit from ICatcher.  If it did, we would
  // want to propagate unchecked exceptions upward when processing an
  // anonymous class.  It seems a bit simpler to just put the logic in
  // model_method.

  // True if this is an instance initializer, false otherwise.
  const bool instance;

public:

  model_initializer_block (const location &w, const std::list<ref_stmt> &s,
			   bool inst)
    : model_block (w, s),
      instance (inst)
  {
  }

  model_initializer_block (const location &w, bool inst)
    : model_block (w),
      instance (inst)
  {
  }

  void resolve (resolution_scope *);

  void compute_normal_completion (normal_completion_state &);

  void visit (visitor *);
};

#endif // GCJX_MODEL_INIT_HH

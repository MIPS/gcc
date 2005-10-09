// Represent a block scope.

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

#ifndef GCJX_MODEL_BLOCKSCOPE_HH
#define GCJX_MODEL_BLOCKSCOPE_HH

/// This represents a variable binding contour.  An instance is
/// created during resolution and will be attached to the resolution
/// scope.  Other statements during resolution can add variables to
/// the bindings or look up names.  A model_block_scope object is
/// transient and is destroyed when its corresponding block has
/// finished resolving.
/// FIXME: This is not properly part of the model, it should have a
/// different name.
class model_block_scope : public IScope
{
  // The scope we're attached to.
  resolution_scope *scope;

  // Maps for each type.
  std::map<std::string, model_class *> class_map;
  std::map<std::string, model_variable_decl *> variable_map;

public:

  model_block_scope (resolution_scope *s)
    : scope (s)
  {
  }

  ~model_block_scope ();

  void look_up_name (const std::string &name,
		     std::set<model_class *> &result,
		     IContext *request,
		     model_class *qualifier);

   void look_up_name (const std::string &name,
		      std::set<model_variable_decl *> &result,
		      IContext *request,
		      model_class *qualifier);

  void add_binding (model_variable_decl *);

  void add_binding (model_class *);
};

/// This is a special kind of IScope that only handles whether a
/// context is static.
class model_static_context_scope : public IScope
{
  bool flag;

public:

  model_static_context_scope (bool f)
    : flag (f)
  {
  }

  static_result is_static_scope () const
  {
    return flag ? STATIC_CONTEXT : NOT_STATIC_CONTEXT;
  }
};

#endif // GCJX_MODEL_BLOCKSCOPE_HH

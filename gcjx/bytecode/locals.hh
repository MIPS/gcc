// Local variable handling for bytecode generation.

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

#ifndef GCJX_BYTECODE_LOCALS_HH
#define GCJX_BYTECODE_LOCALS_HH

class locals
{
  // This maps a local variable to its index.
  std::map<model_variable_decl *, int> indexes;

  // This maps a local variable to its defining scope.
  std::map<model_variable_decl *, const model_stmt *> scope_map;

  // This is used as a quick way to deterine whether a slot is in use.
  std::deque<bool> used;

  // Statement representing current scope.
  std::deque<const model_stmt *> scope;

  // Number of entries ever allocated in 'used'.
  int max;

public:

  locals ()
    : max (0)
  {
    scope.push_back (NULL);
  }

  ~locals ()
  {
  }

  // Indicate the current scope.
  void push_scope (const model_stmt *);

  // Request a new slot for a given variable.  NULL is an acceptable
  // argument, it is used for synthetic variables.  These must be
  // managed more explicitly.
  int request (model_variable_decl *);

  // Find the index of an existing local.
  int get_index (model_variable_decl *);

  // Indicate that the lifetime of the variable is over.
  void remove (model_variable_decl *);

  // Likewise, for synthetic variables only.
  void remove (int);

  // Return maximum number of locals.
  int get_max () const
  {
    return max;
  }

  /// Remove all variables whose enclosing statement is as specified.
  /// This also pops the scope associated with this statement.
  /// @param stmt the enclosing statement
  void remove (const model_stmt *);
};


class temporary_local
{
  // The variable, or NULL for synthetic.
  model_variable_decl *var;

  // The slot.
  int n;

  // The locals.
  locals &vars;

public:

  temporary_local (locals &vs, model_variable_decl *v)
    : var (v),
      vars (vs)
  {
    n = vars.request (v);
  }

  ~temporary_local ()
  {
    if (var)
      vars.remove (var);
    else
      vars.remove (n);
  }

  operator int () const
  {
    return n;
  }
};

#endif // GCJX_BYTECODE_LOCALS_HH

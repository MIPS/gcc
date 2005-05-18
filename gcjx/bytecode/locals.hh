// Local variable handling for bytecode generation.

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

#ifndef GCJX_BYTECODE_LOCALS_HH
#define GCJX_BYTECODE_LOCALS_HH

class bytecode_block;
class bytecode_generator;
class bytecode_stream;
class outgoing_constant_pool;

/// This class is used to keep track of the local variable slots while
/// generating code for a given method.
class locals
{
  // This represents a single local variable and is used when
  // generating debug information.
  struct debug_info
  {
    // The variable.
    model_variable_decl *variable;
    // The starting block.
    bytecode_block *start;
    // The ending block.
    bytecode_block *end;
    // The defining scope.
    const model_stmt *scope;
    // The index.
    int index;
  };

  // The corresponding bytecode generator.
  bytecode_generator *gen;

  // This is used as a quick way to deterine whether a slot is in use.
  std::deque<bool> used;

  // Statement representing current scope.
  std::deque<const model_stmt *> scope;

  // Number of entries ever allocated in 'used'.
  int max;

  // Keep track of a variable while it is live.
  std::map<model_variable_decl *, debug_info> vars;

  // Keep track of a variable after it is live.  A given variable may
  // appear several times here, due to how we emit 'finally' clauses.
  std::list<debug_info> var_descriptions;

  // Number of variables that survived updating.
  int valid;

  int request (bool);

public:

  locals (bytecode_generator *g)
    : gen (g),
      max (0),
      valid (-1)
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

  // Request a new slot for a wide variable.  The variable is assumed
  // to be synthetic and must be explicitly managed.  That is, removal
  // must be handled via the special remove() method for synthetic
  // variables.
  int request ();

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

  /// Used when generating debug information.  Updates the ranges of
  /// variables and returns 'true' if any exist to be written out.
  bool update ();

  /// Used when generating debug information.  Write the actual
  /// information.  If the writer is NULL, just enter information into
  /// the constant pool and return the number of bytes that will be
  /// emitted.  The final argument is true if we are emitting a
  /// LocalVariableTypeTable, false if we are emitting a
  /// LocalVariableTable.
  int emit (outgoing_constant_pool *, bytecode_stream *, bool);

  /// Return true if any local variable has a parameterized type (and
  /// thus a LocalVariableTypeTable is required).
  bool any_parameterized_p ();
};

/// This is an class for allocating a temporary local variable.  Usage
/// is RAII; when the temporary_local is destroyed, the corresponding
/// local variable slot is reclaimed.
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

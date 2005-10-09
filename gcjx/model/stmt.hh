// Represent a statement.

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

#ifndef GCJX_MODEL_STMT_HH
#define GCJX_MODEL_STMT_HH

class model_stmt;

/// This is used to track some state when computing normal completion.
class normal_completion_state
{
  // This holds each statement as it is pushed.
  std::list<model_stmt *> statements;

public:

  normal_completion_state ()
  {
  }

  ~normal_completion_state ()
  {
  }

  /// Push a statement into the state.
  void push (model_stmt *s)
  {
    assert (s != NULL);
    statements.push_back (s);
  }

  /// Pop the last statement from the state.
  void pop ()
  {
    statements.pop_back ();
  }

  /// Return true if a jump from the current location to the indicated
  /// statement will occur "as planned".  For instance, if we are in a
  /// 'try' clause which has a 'finally' clause which cannot complete
  /// normally, then a 'break' out of this try will not occur as
  /// planned.
  bool can_goto_p (model_stmt *) const;
};

/// This is an RAII-based class for pushing and popping to a
/// normal_completion_state.
class normal_completion_pusher
{
  normal_completion_state &state;

public:

  normal_completion_pusher (normal_completion_state &s, model_stmt *stmt)
    : state (s)
  {
    state.push (stmt);
  }

  ~normal_completion_pusher ()
  {
    state.pop ();
  }
};

/// A helper function that calls compute_normal_completion for a list
/// of statements.
bool compute_normal_completion (normal_completion_state &,
				std::list< owner<model_stmt> > &);

/// This is the base class for all statements.  It doesn't hold much
/// state, but declares an interface to which all statements must
/// conform.
class model_stmt : public model_element
{
protected:

  /// This caches the results of can_complete_normally.  This must be
  /// set by subclasses.
  watch<bool> normal_completion;

  friend bool compute_normal_completion (normal_completion_state &,
					 std::list< owner<model_stmt> > &);

  model_stmt (const location &w)
    : model_element (w)
  {
  }

public:

  /// This is called for a statement which is the target of a "break"
  /// statement.  It is used to indicate that the particular label
  /// that is the target of the "break" has been referenced.  The
  /// default implementation simply calls abort(); this is acceptable
  /// because it represents a programming error, as the compiler
  /// should never let an invalid statement be resolved as the target
  /// of a "break" or "continue".
  virtual void break_reached ()
  {
    abort ();
  }

  /// This is called for an unreachable statement.
  void unreachable ()
  {
    std::cerr << error ("statement is unreachable");
  }

  /// Resolve all as-yet unresolved things in this statement.  For
  /// instance, this will compute expression types and determine the
  /// meaning of names.
  /// @param scope  the resolution scope used for name lookups
  virtual void resolve (resolution_scope *) = 0;

  /// Check whether or not this statement can complete normally.  Note
  /// that "can complete normally" is precisely defined; see the JLS
  /// for details.
  /// @return true if this statement can complete normally
  bool can_complete_normally ()
  {
    return normal_completion;
  }

  /// This computes whether or not this statement can complete
  /// normally.  This must be called before can_complete_normally is
  /// called.  Implementations of this method must set the
  /// 'normal_completion' field.
  /// @param state  state which must be kept updated during computation
  virtual void compute_normal_completion (normal_completion_state &) = 0;

  /// Find the "continue" target corresponding to this object.
  /// @return a pointer to the statement, or NULL if this isn't a
  /// loop statement.
  virtual model_loop *get_continue_target ()
  {
    return NULL;
  }

  /// Find the "break" target corresponding to this object.
  /// @return a pointer to the statement, or NULL if this isn't a
  /// loop statement.
  virtual model_stmt *get_break_target ()
  {
    return NULL;
  }

  /// Call the appropriate method of the visitor with information
  /// about the tree.
  /// @param v  the visitor to call
  virtual void visit (visitor *) = 0;
};

#endif // GCJX_MODEL_STMT_HH

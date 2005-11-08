// Represent an expression.

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

#ifndef GCJX_MODEL_EXPR_HH
#define GCJX_MODEL_EXPR_HH

class model_type;

/// This is the base class for all expressions.  It keeps track of the
/// cached type of the expression, and declares an interface to which
/// all expressions must conform.
class model_expression : public model_element
{
protected:

  /// The type of the expression.  Initially this is NULL.  It is
  /// computed and cached during resolution.  It is wrapped in a watch
  /// template to catch programming errors where it is used before it
  /// is set.
  watch<model_type *> type_cache;

  /// This flag is true if this expression is constant, false
  /// otherwise.
  watch<bool> constant_cache;

  /// This should only, and must, be called by resolve().  It is used
  /// to set type_cache.  (FIXME remove this and just set directly?)
  void set_type (model_type *);

  /// This is called to compute whether or not this expression is
  /// constant.
  virtual bool compute_constant_p () = 0;

  model_expression (const location &w)
    : model_element (w)
  {
  }

public:

  /// Determine whether or not this is a constant expression.  Note
  /// that "constant expression" is precisely defined; see the JLS for
  /// details.
  /// @return true if this is a constant expression
  bool constant_p ()
  {
    if (! constant_cache.set_p ())
      constant_cache = compute_constant_p ();
    return constant_cache;
  }

  /// Resolve the expression, computing types and determining the
  /// meaning of names.  Any implementation of this method is required
  /// to call set_type().  (fixme: document error handling approach)
  /// @param scope  the resolution scope used during name lookup
  virtual void resolve (resolution_scope *) = 0;

  /// Compute the value of this expression.  This can only be called
  /// for constant expressions of primitive type.  In other situations
  /// this method is likely to abort.
  /// @return the value of the expression
  virtual jvalue value ();

  /// Compute the string value of this expression.  This can only be
  /// called for constant expression of String type.  In other
  /// situations this method is likely to abort.  The implementation
  /// works for most cases.
  /// @return a string representing the value of the expression
  virtual std::string string_value ();

  /// Call the appropriate method of the visitor with information
  /// about the tree.
  /// @param v  the visitor to call
  virtual void visit (visitor *) = 0;

  /// Return the type of this expression.  It is only valid to call
  /// this after resolution; if called before it will abort.
  /// @return object representing the type of this expression
  model_type *type () const
  {
    return type_cache;
  }

  /// This is called by an assignment operator to indicate that this
  /// object is the left hand side of an assignment.  If COMPOUND is
  /// true, then this is a compound assignment and not a simple
  /// assignment.  Operators like "++" count as compound assignments
  /// for purposes of this method.  This must be called before the
  /// expression in question is resolved.
  virtual void set_left_hand_side (bool compound)
  {
    // Don't abort here -- at the point when this is called, we won't
    // have had a chance to emit an error message.
  }

  /// This is called before resolution on an expression which will
  /// undergo assignment conversion.  The argument is the type of the
  /// left hand side of the assignment.  (This hook exists for type
  /// inference of method calls to work correctly.)
  virtual void use_assignment_conversion (model_type *)
  {
    // Do nothing by default.
  }
};

const format &operator% (const format &, const model_expression *);

#endif // GCJX_MODEL_EXPR_HH

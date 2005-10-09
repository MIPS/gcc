// Represent an element.

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

#ifndef GCJX_MODEL_ELEMENT_HH
#define GCJX_MODEL_ELEMENT_HH

#include <stdarg.h>
#include <stdio.h>

/// This is the base class for all model objects.  Its only attribute
/// is a location.  It understands how to issue error and warning
/// messages.  (FIXME this should be refactored, see TODO.)
///
/// This class also holds the reference count for the owner<> template
/// class.  The general naming rule is that for a model class named
/// "model_foo" we can have a typedef for the corresponding owner
/// class named "ref_foo".  Not all model classes have corresponding
/// typedefs; these are made on an as-needed basis only.
class model_element
{
  /// Reference count.
  /// @see owner.
  int refc;

  /// Location of element.
  location where;

  // For reference count access.
  template<typename T> friend class owner;

protected:

  /// Constructor used only for cloning.
  model_element (model_element *other)
    : refc (0),
      where (other->where)
  {
  }

  /// Constructor.
  /// @param w Location of this object in the source.
  model_element (const location &w)
    : refc (0),
      where (w)
  {
  }

public:

  virtual ~model_element ()
  {
  }

  /// Return a new formatting object at this location.
  semantic_error error (const char *fmt) const
  {
    return semantic_error (where, fmt);
  }

  /// Return a new formatting object at this location.
  format warn (const char *fmt) const
  {
    return new format_repr (format_warning, where, fmt);
  }

  /// Return a new formatting object at this location.
  format warn (warning_state val, const char *fmt) const
  {
    assert (val != OFF);
    // Values are chosen such that the cast here will work.
    return new format_repr (format_type (val), where, fmt);
  }

  /// @return The location of this object.
  location get_location () const
  {
    return where;
  }

  /// Call the appropriate method of the visitor with information
  /// about the tree.
  /// @param v  the visitor to call
  virtual void visit (visitor *v);
};

#endif // GCJX_MODEL_ELEMENT_HH

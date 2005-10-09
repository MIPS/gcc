// Represent a type, primitive or not.

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

#ifndef GCJX_MODEL_TYPE_HH
#define GCJX_MODEL_TYPE_HH

class model_array_type;
class model_type;

/// This is the base class of all types in the model.  Subclasses
/// include primitive types, the void type, the null type, and
/// reference types.
class model_type : public model_element
{
protected:

  // The type descriptor, in the class file sense.
  std::string descriptor;

  // The array type for this type.  For allocation purposes it is
  // considered to be owned by the element type.
  owner<model_array_type> array_type;

  /// Compute the descriptor if needed.  Subclasses are expected to
  /// override this if they cannot compute their descriptor when
  /// constructed.
  virtual void compute_descriptor ()
  {
  }

  model_type (const location &w)
    : model_element (w)
  {
  }

public:

  std::string get_descriptor ()
  {
    if (descriptor.empty ())
      compute_descriptor ();
    return descriptor;
  }

  /// Return the signature (in the 1.5 sense) of this type.
  virtual std::string get_signature ()
  {
    // Default implementation, suitable for primitive types, is the
    // same as the descriptor.
    return get_descriptor ();
  }

  // This is virtual just so 'void' can error-check.
  virtual model_array_type *array ();

  virtual model_type *erasure ();

  // Return the element type.  It is a programming error if this is
  // not an array type.
  virtual model_type *element_type ()
  {
    abort ();
  }

  // Return true if this type is an array.
  virtual bool array_p () const
  {
    return false;
  }

  // Return true if this is an interface type.
  virtual bool interface_p () const
  {
    return false;
  }

  // Return true if this is a reference type.
  virtual bool reference_p () const
  {
    return false;
  }

  // Return true if this type is final.
  virtual bool final_p () const
  {
    return false;
  }

  // Return true if this has numeric type.
  virtual bool numeric_p () const
  {
    return false;
  }

  // True if this is an integral type.
  virtual bool integral_p () const
  {
    return false;
  }

  // True if this is the boolean type.
  virtual bool boolean_p () const
  {
    return false;
  }

  // True if this is an annotation type.
  virtual bool annotation_p () const
  {
    return false;
  }

  // True if this is a checked exception type.
  bool checked_exception_p ();

  // True if this is an enum type.
  virtual bool enum_p () const
  {
    return false;
  }

  // Return true if OTHER is assignable to this type.
  virtual bool assignable_from_p (model_type *other) = 0;

  // Return true if this is a primitive type.
  // Note that this will return 'false' for the void type.
  // That type can be checked for by comparison to primitive_void_type.
  virtual bool primitive_p () const = 0;

  // Convert a value to a string.  This is only implemented properly
  // by primitive types.  FIXME: let's move stuff around...?
  virtual std::string to_string (const jvalue &) const = 0;

  // Return a "pretty" name for this type.  This is a name that the
  // user is likely to recognize as a type name.
  virtual std::string get_pretty_name () const = 0;

  void visit (visitor *);
};

const format &operator% (const format &fmt, const model_type *t);

#endif // GCJX_MODEL_TYPE_HH

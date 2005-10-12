// A type variable.

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

#ifndef GCJX_MODEL_TYPEVAR_HH
#define GCJX_MODEL_TYPEVAR_HH

/// This represents a type variable.  It is a subclass of model_class
/// to simplify the overall implementation -- a type variable is
/// treated as a special kind of class, whose superclass and
/// interfaces are the bounds.
class model_type_variable : public model_class
{
  // The bounds before resolution.
  std::list<ref_forwarding_type> bounds;

  void resolve_hook (resolution_scope *);

  void compute_descriptor ()
  {
    // We override this method to avoid crashes if our superclass'
    // method is called.  There's no sensible value here however.    
  }

public:

  model_type_variable (const location &w)
    : model_class (w)
  {
    // Mark as abstract in case we only extend an interface.
    // FIXME: this is a bogus side effect of our chosen implementation
    // technique...
    set_modifiers (ACC_PUBLIC | ACC_ABSTRACT);
  }

  model_type_variable (const location &w,
		       const std::list<ref_forwarding_type> &bl)
    : model_class (w),
      bounds (bl)
  {
  }

  bool type_variable_p () const
  {
    return true;
  }

  void add_bound (const ref_forwarding_type &bound)
  {
    bounds.push_back (bound);
  }

  model_type *erasure ();

  void validate (model_element *, model_type *);

  bool assignable_from_p (model_type *);

  model_class *apply_type_map (model_element *, const model_type_map &);

  void check_instantiation (model_element *);

  void resolve (resolution_scope *);

  std::string get_pretty_name () const
  {
    return name;
  }

  std::string get_signature ();

  /// This returns the signature as used by the class signature (as
  /// opposed to the type signature).
  std::string get_formal_signature ();

  void visit (visitor *);
};

#endif // GCJX_MODEL_TYPEVAR_HH

// "modifiable" interface.

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

#ifndef GCJX_MODEL_IMODIFIABLE_HH
#define GCJX_MODEL_IMODIFIABLE_HH

/// This interface is implemented by any class which has modifiers.
/// It handles all the work associated with modifiers and provides
/// some helpful accessor methods to look at various attributes of the
/// modified object.
class IModifiable
{
protected:

  /// The modifier flags.  See the ACC_* defines in modifier.hh.
  modifier_t modifiers;

  /// True if this object was read from a .class file.  This affects
  /// whether or not implicit modifiers emit warnings.
  bool from_class;

  /// The constructor is protected, as you must extend this class for
  /// it to be meaningful.
  IModifiable ()
    : modifiers (0),
      from_class (false)
  {
  }

  /// This constructor is used only for cloning.
  IModifiable (IModifiable *other)
    : modifiers (other->modifiers),
      from_class (other->from_class)
  {
  }

  /// Subclasses must implement this.  It takes the modifiers and
  /// should perform checking and update them as appropriate.  The
  /// 'modifiers' field is set before this method is called.
  virtual void massage_modifiers (const ref_modifier_list &) = 0;

public:

  virtual ~IModifiable ()
  {
  }

  /// Set the modifiers for this object.
  virtual void set_modifiers (const ref_modifier_list &);

  /// Set the modifiers on this method.  This overload should only be
  /// used when reading a .class file or when creating a synthetic
  /// class; in those situations no error checking needs to be done.
  virtual void set_modifiers (modifier_t val)
  {
    modifiers = val;
  }

  /// Indicate that this object was either read from a .class file, or
  /// is synthetic.  This affects whether implicit modifiers generate
  /// warning -- objects from .class files and synthetic objects are
  /// assumed to be created with the correct flags.
  void set_from_class ()
  {
    from_class = true;
  }

  bool from_class_p () const
  {
    return from_class;
  }

  /// Note that this can only be called after set_modifiers.  FIXME:
  // should either lift this restriction or check for it.
  void set_synthetic ()
  {
    modifiers |= ACC_SYNTHETIC;
  }

  bool synthetic_p () const
  {
    return (modifiers & ACC_SYNTHETIC) != 0;
  }

  /// Return the modifiers for this object.
  modifier_t get_modifiers () const
  {
    return modifiers;
  }

  /// Returns true if this object is static.
  bool static_p () const
  {
    return (modifiers & ACC_STATIC) != 0;
  }

  /// Returns true if this object is final.
  virtual bool final_p () const
  {
    return (modifiers & ACC_FINAL) != 0;
  }

  /// Returns true if this object is abstract.
  bool abstract_p () const
  {
    return (modifiers & ACC_ABSTRACT) != 0;
  }

  /// Returns true if this object is 'native'.
  bool native_p () const
  {
    return (modifiers & ACC_NATIVE) != 0;
  }

  /// Indicate that this object is implicitly 'public' due to its
  /// context.
  void implicitly_public ();

  /// Set some modifiers on this object based on its context.  Only
  /// one modifier can be passed to this method; if you need to set
  /// multiple modifiers, use multiple calls.  Also, protection bits
  /// like ACC_PUBLIC cannot be set with this method.
  virtual void implicit_modifier (modifier_t);
};

#endif // GCJX_MODEL_IMODIFIABLE_HH

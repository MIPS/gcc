// Represent an enum class.

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

#ifndef GCJX_MODEL_ENUM_HH
#define GCJX_MODEL_ENUM_HH

/// This represents an 'enum' declaration.  An enum is a kind of
/// class that has special constant fields.
class model_enum : public model_class
{
protected:

  // Ordinal of the next enum constant to be added.
  int ordinal;

  // All the enum constants that have been added.
  std::list<ref_enum_constant> constants;

  // Add enum-specific arguments to the constructor.  See the Enum
  // constructor; these are passed up to it by the super() call in the
  // constructor.
  void add_enum_arguments (model_constructor *);

  void resolve_classes_hook (resolution_scope *);
  void resolve_member_hook (resolution_scope *);
  void massage_modifiers (const ref_modifier_list &mods);
  ref_method add_implicit_constructor ();

public:

  model_enum (const location &);

  void add_enum (const ref_enum_constant &);

  bool enum_p () const
  {
    return true;
  }

  void check_instantiation (model_element *);

  /// Return number of enum constants in this class.
  unsigned int constants_count () const
  {
    return constants.size ();
  }

  void visit (visitor *);
};

#endif // GCJX_MODEL_ENUM_HH

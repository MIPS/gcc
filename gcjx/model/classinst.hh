// A parameterized class instance.

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

#ifndef GCJX_MODEL_CLASSINST_HH
#define GCJX_MODEL_CLASSINST_HH

/// This represents an instance of a parameterized class.  Each
/// instance is characterized by its type map and its parent class.
class model_class_instance : public model_class
{
  /// The parent class.
  model_class *parent;

  /// The type map.
  model_type_map type_map;

protected:

  void resolve_hook (resolution_scope *);
  void resolve_member_hook (resolution_scope *);
  std::string get_signature_map_fragment ();

public:

  model_class_instance (const location &w, model_class *p,
			const model_type_map &tm)
    : model_class (w),
      parent (p),
      type_map (tm)
  {
  }

  bool parameterized_p () const
  {
    return true;
  }

  model_class *get_parent () const
  {
    return parent;
  }

  const model_type_map &get_type_map () const
  {
    return type_map;
  }

  // Retrieve the type map in "argument form" -- in the same order as
  // the parameters given when instantiating this class.
  void get_type_map (std::list<model_class *> &result);

  model_class *apply_type_map (model_element *, const model_type_map &);

  // FIXME: could have covariant return here..
  model_type *erasure ()
  {
    return parent;
  }

  /// Return true if the argument can be converted to this type via
  /// capture conversion.  This differs from can_capture_p in that
  /// this method iterates over the parameters.
  bool capture_conversion_p (model_class_instance *);

  void visit (visitor *);
};

#endif // GCJX_MODEL_CLASSINST_HH

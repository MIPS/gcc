// Raw types.

// Copyright (C) 2005 Free Software Foundation, Inc.
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

#ifndef GCJX_MODEL_RAW_HH
#define GCJX_MODEL_RAW_HH

/// This represents a raw class.  Note that a raw class is only
/// meaningful when the parent class is parameterized, and this is
/// enforced via assertions.
class model_raw_class : public model_class
{
  /// The parent class.
  model_class *parent;

protected:

  void resolve_member_hook (resolution_scope *);
  void ensure_classes_inherited (resolution_scope *);

public:

  model_raw_class (const location &, model_class *);

  model_class *get_parent () const
  {
    return parent;
  }

  model_class *apply_type_map (model_element *, const model_type &)
  {
    // Applying a type map to the raw type does nothing, hence we
    // simply return 'this'.
    return this;
  }

  bool raw_p () const
  {
    return true;
  }

  model_type *erasure ()
  {
    return this;
  }

  std::string get_pretty_name () const;

  // FIXME
  // void visit (visitor *);
};

#endif // GCJX_MODEL_RAW_HH

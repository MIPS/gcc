// Represent an annotatable object.

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

#ifndef GCJX_MODEL_IANNOTATABLE_HH
#define GCJX_MODEL_IANNOTATABLE_HH

/// This is used to mark a given object as accepting annotations.
class IAnnotatable
{
  /// The type of annotations supported by this object.
  /// FIXME: we don't need to keep this as data, we could have a
  /// virtual method to get the answer.  It is only used once.
  annotation_kind kind;

  /// Annotations applied to this object.
  std::list<ref_annotation> annotations;

  /// This keeps the warning flags associated with this object.
  bool warnings_set;
  warning_bitset warning_bits;

protected:

  /// Return the annotation with the indicated type, or NULL if none
  /// found.
  model_annotation *find_annotation (model_type *);

  virtual annotation_kind get_annotation_kind () const = 0;

  /// Resolve the envelope of the annotations.
  void resolve_annotation_classes (resolution_scope *);

  /// Subclasses should call this when they are resolved in order to
  /// resolve their annotations as well.
  void resolve_annotations (resolution_scope *);

  /// Return true if there are any annotations.
  bool has_annotations_p () const
  {
    return ! annotations.empty ();
  }

  IAnnotatable ()
    : warnings_set (false),
      warning_bits (false)
  {
  }

  // This constructor is used for cloning.
  IAnnotatable (IAnnotatable *other)
    : annotations (other->annotations),
      warnings_set (other->warnings_set),
      warning_bits (other->warning_bits)
  {
  }

public:

  virtual ~IAnnotatable ()
  {
  }

  /// Set annotations.
  void set_annotations (const std::list<ref_annotation> &annos)
  {
    annotations = annos;
  }

  /// Retrieve annotations.
  std::list<ref_annotation> get_annotations () const
  {
    return annotations;
  }

  /// Returns current warning suppression bits.
  const warning_bitset &get_warning_flags ();
};

#endif // GCJX_MODEL_IANNOTATABLE_HH

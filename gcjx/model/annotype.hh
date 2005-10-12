// Represent an annotation type.

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

#ifndef GCJX_MODEL_ANNOTYPE_HH
#define GCJX_MODEL_ANNOTYPE_HH

/// This represents an '@interface' -- an annotation type declaration.
class model_annotation_type : public model_class
{
  typedef std::map<std::string, ref_annotation_member> member_type;

  // The annotation members.
  member_type anno_members;

  // Values of the valid element types for this annotation type, "or"d
  // together.
  watch<int> valid_elements;

  // Retention policy for this annotation type.
  watch<annotation_retention> policy;


  void resolve_hook (resolution_scope *);

  annotation_kind get_annotation_kind () const
  {
    return ANNOTATE_ANNOTATION;
  }

public:

  model_annotation_type (const location &w);

  void add_annotation_member (const ref_annotation_member &);

  bool annotation_p () const
  {
    return true;
  }

  /// Return true if this annotation is allowable for the indicated
  /// program element.
  bool element_compatible_p (annotation_kind);

  /// Return the retention policy for this annotation type.
  annotation_retention get_retention_policy ();

  /// Check annotation arguments against our values for completeness.
  /// Throw exception, using request as the location, on error.
  void check_completeness (const std::list<ref_annotation_value> &args,
			   model_element *request);

  void visit (visitor *);
};

#endif // GCJX_MODEL_ANNOTYPE_HH

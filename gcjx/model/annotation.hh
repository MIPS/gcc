// Represent an annotation.

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

#ifndef GCJX_MODEL_ANNOTATION_HH
#define GCJX_MODEL_ANNOTATION_HH

/// Program objects that can be annotated.
enum annotation_kind
{
  ANNOTATE_LOCAL = 1,
  ANNOTATE_PARAMETER = 2,
  ANNOTATE_FIELD = 4,
  ANNOTATE_METHOD = 8,
  ANNOTATE_TYPE = 16,
  ANNOTATE_ANNOTATION = 32,
  ANNOTATE_CONSTRUCTOR = 64,
  ANNOTATE_PACKAGE = 128
};

/// How long a given annotation should be held.
enum annotation_retention
{
  RETAIN_SOURCE,
  RETAIN_CLASS,
  RETAIN_RUNTIME
};

/// This represents a single annotation, like '@Name(first = "Maude")'.
/// It is an expression for type convenience elsewhere in the compiler.
class model_annotation : public model_expression
{
  // Type of this annotation.
  ref_forwarding_type name;

  // Argument list.
  std::list<ref_annotation_value> args;

protected:

  bool compute_constant_p ()
  {
    // It shouldn't matter.
    return false;
  }

public:

  model_annotation (const location &w,
		    const ref_forwarding_type &n,
		    const std::list<ref_annotation_value> &as)
    : model_expression (w),
      name (n),
      args (as)
  {
  }

  void resolve (resolution_scope *);

  void visit (visitor *);

  model_annotation_value *find_annotation (const char *);

  std::list<ref_annotation_value> get_arguments () const
  {
    return args;
  }
};

#endif // GCJX_MODEL_ANNOTATION_HH

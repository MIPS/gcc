// Represent an annotatable object.

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

#include "typedefs.hh"

model_annotation *
IAnnotatable::find_annotation (model_type *type)
{
  for (std::list<ref_annotation>::const_iterator i = annotations.begin ();
       i != annotations.end ();
       ++i)
    {
      if (type == (*i)->type ())
	return (*i).get ();
    }
  return NULL;
}

void
IAnnotatable::resolve_annotations (resolution_scope *scope)
{
  ::resolve (scope, annotations);

  // This is a little tricky: if there are no annotations, skip the
  // rest so that we don't try to read annotation-related classes if
  // we're compiling against a pre-1.5 library.
  if (! has_annotations_p ())
    return;

  annotation_kind kind = get_annotation_kind ();
  model_type *dep = global->get_compiler ()->java_lang_Deprecated ();
  for (std::list<ref_annotation>::const_iterator i = annotations.begin ();
       i != annotations.end ();
       ++i)
    {
      model_annotation_type *annot
      	= assert_cast<model_annotation_type *> ((*i)->type ());

      // If the annotation type is itself annotated with Target, make
      // sure the target is compatible with our `kind'.
      if (! annot->element_compatible_p (kind))
	{
	  model_element *elt = assert_cast<model_element *> (this);
	  // FIXME: use correct name of element.
	  std::cerr << elt->error ("annotation type %1 not compatible with "
				   "program element")
	    % annot;
	}

      // Handle @Deprecated.
      if (annot == dep && dynamic_cast<IDeprecatable *> (this))
	{
	  IDeprecatable *dep_this = dynamic_cast<IDeprecatable *> (this);
	  dep_this->set_deprecated (true);
	}
    }
}

const warning_bitset &
IAnnotatable::get_warning_flags ()
{
  if (! warnings_set)
    {
      // This is a little tricky: if there are no annotations, skip
      // the rest so that we don't try to read annotation-related
      // classes if we're compiling against a pre-1.5 library.
      if (! has_annotations_p ())
	{
	  warnings_set = true;
	  return warning_bits;
	}

      model_annotation *anno
	= find_annotation (global->get_compiler ()->java_lang_SuppressWarnings ());

      if (anno)
	{
	  // FIXME: some of the asserts here are not really correct.
	  model_annotation_value *val = anno->find_annotation ("value");
	  assert (val);
	  model_expression *expr = val->get_value ();
	  std::list<ref_expression> exprs;
	  // FIXME: inefficient, use a helper function instead.
	  if (dynamic_cast<model_string_literal *> (expr))
	    exprs.push_back (expr);
	  else
	    exprs = assert_cast<model_annotation_initializer *> (expr)->get_initializers ();

	  for (std::list<ref_expression>::const_iterator i = exprs.begin ();
	       i != exprs.end ();
	       ++i)
	    {
	      model_expression *e = (*i).get ();
	      std::string value = e->string_value ();
	      warning_flag flag = warning_scope::find_warning (value.c_str ());
	      if (flag != WARN_NONE)
		warning_bits.set (flag, true);
	    }
	}

      // We don't allow deprecation warnings in a deprecated context;
      // this is part of the specification of the @Deprecated
      // annotation, and it seems to make sense to apply that
      // uniformly.
      if (find_annotation (global->get_compiler ()->java_lang_Deprecated ()))
	warning_bits.set (WARN_DEPRECATED, true);

      warnings_set = true;
    }

  return warning_bits;
}

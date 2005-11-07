// Represent a constructor.

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

#ifndef GCJX_MODEL_CONSTRUCTOR_HH
#define GCJX_MODEL_CONSTRUCTOR_HH

class model_constructor : public model_method
{
  // A list of all the outer variables we must capture.  This is only
  // used for local and anonymous classes.
  std::list<model_variable_decl *> captured_variables;

  // If not NULL, the 'this$0' argument to this constructor.
  ref_variable_decl this0;

  // If this method is a constructor, and it calls this(...), this
  // points to other constructor that is invoked.  It is NULL if:
  // * This constructor has not yet been resolved, or
  // * This constructor does not call this().
  model_constructor *other_this;

  annotation_kind get_annotation_kind () const
  {
    return ANNOTATE_CONSTRUCTOR;
  }

protected:

  void massage_modifiers (const ref_modifier_list &);

  // This constructor is used only when applying a type map.
  model_constructor (model_constructor *, const model_type_map &,
		     model_class *);

  // This constructor is used only when creating the erasure.
  model_constructor (model_constructor *, model_class *);

public:

  model_constructor (const location &w, model_class *decl)
    : model_method (w, decl),
      other_this (NULL)
  {
  }

  // This constructor is used only when creating an accessor.
  model_constructor (model_constructor *);

  bool constructor_p () const
  {
    return true;
  }

  void resolve_classes (resolution_scope *);

  void resolve (resolution_scope *);

  void add_parameter (const ref_variable_decl &);

  /// This is called when resolving this(...) to indicate the other
  /// constructor that is invoked by this constructor.  This will
  /// throw an error if it detects a cycle.  The model_element
  /// argument is used for error reporting only.
  void set_other_this (model_constructor *, model_element *);

  model_variable_decl *get_this0_parameter () const
  {
    return this0.get ();
  }

  // FIXME: could use covariant return here.
  model_method *apply_type_map (const model_type_map &, model_class *);

  model_method *erasure (model_class *);

  /// Prepend some statements to the constructor body.
  void prepend_statements (resolution_scope *, std::list<ref_stmt> &,
			   bool = true);

  void visit (visitor *);
};

#endif // GCJX_MODEL_CONSTRUCTOR_HH

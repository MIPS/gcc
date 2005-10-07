// Represent a variable.

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

#ifndef GCJX_MODEL_VARIABLE_HH
#define GCJX_MODEL_VARIABLE_HH

// This is used for fields, catch clauses, and fields.  Note that this
// extends IMember, even though an ordinary variable is not a member.
// We use this field for the obvious thing for model_fields, and for
// variables it is used when an anonymous or local class refers to a
// final variable.
class model_variable_decl : public model_element, public IMember,
			    public IAnnotatable
{
protected:

  // Name of the variable.
  std::string name;

  // Type of the variable.
  ref_forwarding_type decltype;

  // Expression representing the initializer, or NULL if none.
  ref_expression initializer;

  // True if this is final.
  bool final;

  // True if this variable was referenced.  For an ordinary variable,
  // we only set this if the variable was used as a value.
  bool used;

  annotation_kind get_annotation_kind () const
  {
    return ANNOTATE_LOCAL;
  }

  // This is used to clone this object.
  virtual model_variable_decl *clone (const location &w,
				      const std::string &s,
				      const ref_forwarding_type &t,
				      model_class *k)
  {
    return new model_variable_decl (w, s, t, k);
  }

public:

  model_variable_decl (const location &w,
		       const std::string &n,
		       const ref_forwarding_type &t,
		       model_class *k)
    : model_element (w),
      name (n),
      decltype (t),
      final (false),
      used (false)
  {
    set_declaring_class (k);
  }

  virtual model_type *type () const
  {
    return decltype->type ();
  }

  void set_declared_type (const ref_forwarding_type &t)
  {
    decltype = t;
  }

  ref_forwarding_type get_declared_type () const
  {
    return decltype;
  }

  void set_final ()
  {
    final = true;
  }

  void set_initializer (const ref_expression &exp)
  {
    initializer = exp;
  }

  ref_expression get_initializer () const
  {
    return initializer;
  }

  bool has_initializer_p () const
  {
    return bool (initializer);
  }

  /// Returns true if this variable can be considered as
  /// "initialized".  The base implementation returns true if and only
  /// if there is an initializer.
  virtual bool consider_initialized_p () const
  {
    return has_initializer_p ();
  }

  // This is virtual for use by fields...
  virtual std::string get_name () const
  {
    return name;
  }

  virtual void resolve (resolution_scope *scope);
  virtual void resolve_classes (resolution_scope *scope);

  virtual bool constant_p ();

  virtual bool final_p () const
  {
    return final;
  }

  jvalue value ()
  {
    if (! initializer)
      abort ();
    return initializer->value ();
  }

  std::string string_value ()
  {
    if (! initializer)
      abort ();
    return initializer->string_value ();
  }

  virtual model_variable_decl *apply_type_map (const model_type_map &,
					       model_class *);

  std::string get_signature ();

  void set_used ()
  {
    used = true;
  }

  virtual void check_referenced (resolution_scope *);

  void visit (visitor *);
};

/// This is a variable that happens to be a method parameter.
class model_parameter_decl : public model_variable_decl
{
protected:

  annotation_kind get_annotation_kind () const
  {
    return ANNOTATE_PARAMETER;
  }

  model_variable_decl *clone (const location &w,
			      const std::string &s,
			      const ref_forwarding_type &t,
			      model_class *k)
  {
    return new model_parameter_decl (w, s, t, k);
  }

public:

  model_parameter_decl (const location &w,
			const std::string &n,
			const ref_forwarding_type &t,
			model_class *k)
    : model_variable_decl (w, n, t, k)
  {
  }

  bool consider_initialized_p () const
  {
    return true;
  }

  void check_referenced (resolution_scope *);

  void visit (visitor *);
};

/// This is a variable that happens to be a 'catch' parameter.
class model_catch_decl : public model_variable_decl
{
protected:

  annotation_kind get_annotation_kind () const
  {
    return ANNOTATE_PARAMETER;
  }

  model_variable_decl *clone (const location &w,
			      const std::string &s,
			      const ref_forwarding_type &t,
			      model_class *k)
  {
    return new model_catch_decl (w, s, t, k);
  }

public:

  model_catch_decl (const location &w,
			const std::string &n,
			const ref_forwarding_type &t,
			model_class *k)
    : model_variable_decl (w, n, t, k)
  {
  }

  bool consider_initialized_p () const
  {
    return true;
  }

  void check_referenced (resolution_scope *)
  {
    // It doesn't seem sensible to warn about 'catch' variables which
    // are not used.  Perhaps we should and just make the user use
    // SuppressWarnings?
  }

  void visit (visitor *);
};

#endif // GCJX_MODEL_VARIABLE_HH

// Represent a field.

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

#ifndef GCJX_MODEL_FIELD_HH
#define GCJX_MODEL_FIELD_HH

class model_field_base
{
public:

  virtual ~model_field_base ()
  {
  }

  virtual std::string get_name () const = 0;

  virtual bool ambiguous_p () const = 0;
};

class model_field : public model_field_base, public model_variable_decl,
		    public IDeprecatable, public IModifiable
{
  // Indicates our current resolution state.
  int state;

  void massage_modifiers (const ref_modifier_list &)
  {
    // Nothing.
  }

  annotation_kind get_annotation_kind () const
  {
    return ANNOTATE_FIELD;
  }

  void check_serialization_fields ();
  void require_resolution ();

public:

  model_field (const ref_variable_decl &);

  model_field (const location &,
	       const std::string &,
	       const ref_forwarding_type &,
	       model_class *);

  bool constant_p ();

  /// This is like constant_p, except that it does not examine whether
  /// the field is static.  We're required to inline field values in
  /// some cases, even if the field does not fit the definition of a
  /// compile-time constant.
  bool inlineable_p ();

  // This is static, and not a method, because we only want to give
  // one error per set of field declarations.
  static void check_modifiers (model_element *request,
			       const ref_modifier_list &mods)
  {
    modifier_t m = mods->value ();

    // Not all modifiers are valid for a field.
    mods->check (request, (ACC_PUBLIC | ACC_PROTECTED | ACC_PRIVATE
			   | ACC_STATIC | ACC_FINAL | ACC_TRANSIENT
			   | ACC_VOLATILE),
		 "invalid modifier %1 for field declaration");
    // Fields have access modifiers, so check validity of these.
    mods->access_check (request, "field declared %1 also declared %2");

    if ((m & ACC_FINAL) != 0)
      mods->check (request, ~ACC_VOLATILE, "%<final%> field also declared %1");
  }

  void set_modifiers (modifier_t mods)
  {
    IModifiable::set_modifiers (mods);
    if ((mods & ACC_FINAL) != 0)
      set_final ();
  }

  void set_modifiers (const ref_modifier_list &mods)
  {
    IModifiable::set_modifiers (mods);
    if ((modifiers & ACC_FINAL) != 0)
      set_final ();
  }

  void implicit_modifier (modifier_t what)
  {
    IModifiable::implicit_modifier (what);
    if ((what & ACC_FINAL) != 0)
      set_final ();
  }

  void resolve (resolution_scope *);
  void resolve_classes (resolution_scope *);

  model_type *type () const;

  bool ambiguous_p () const
  {
    return false;
  }

  // This is needed because we inherit two versions.
  bool final_p () const
  {
    return IModifiable::final_p ();
  }

  std::string get_name () const
  {
    return model_variable_decl::get_name ();
  }

  /// If this field is deprecated, and deprecation warnings are
  /// enabled, issue a warning message.  The argument is the element
  /// requesting the check; any warning is issued against this
  /// element.
  void check_deprecated (model_element *);

  /// Check whether this method was referenced and emit a warning if
  /// not.
  void check_referenced (resolution_scope *);

  model_variable_decl *apply_type_map (const model_type_map &, model_class *);

  void visit (visitor *);
};

class model_ambiguous_field : public model_field_base
{
  // Name of the field.
  std::string name;

  // All the fields to which this name might refer.
  std::list<model_field_base *> fields;

public:

  model_ambiguous_field (model_field *f)
    : name (f->get_name ())
  {
    add (f);
  }

  void add (model_field_base *f)
  {
    fields.push_back (f);
  }

  std::string get_name () const
  {
    return name;
  }

  bool ambiguous_p () const
  {
    return true;
  }
};

inline const format &operator% (const format &fmt, model_field *f)
{
  return fmt % f->get_name ();
}

#endif // GCJX_MODEL_FIELD_HH

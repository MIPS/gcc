// Various built-in functions.

// Copyright (C) 2004, 2005 Free Software Foundation, Inc.
//
// This file is part of GCC.
//
// GCC is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// GCC is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GCC; see the file COPYING.  If not, write to the Free
// Software Foundation, 59 Temple Place - Suite 330, Boston, MA
// 02111-1307, USA.

// This include must come first.
#include "java/glue.hh"

tree_builtins::tree_builtins ()
  : aot_class_factory ()
{
}

tree_builtins::~tree_builtins ()
{
}



gcj_abi *
tree_builtins::find_abi (model_type *)
{
  // FIXME: implement
  return &old_abi;
}

tree
tree_builtins::find_atable_slot (model_field *)
{
  abort ();
}

tree
tree_builtins::find_otable_slot (model_field *)
{
  abort ();
}

tree
tree_builtins::check_reference (tree ref)
{
  if (flag_check_references)
    {
      ref = save_expr (ref);
      tree npe = builtin_Jv_ThrowNullPointerException;
      ref = build3 (COND_EXPR, TREE_TYPE (ref),
		    build2 (EQ_EXPR, type_jboolean,
			    ref, null_pointer_node),
		    build3 (CALL_EXPR, void_type_node,
			    npe, NULL_TREE, NULL_TREE),
		    ref);
    }
  return ref;
}

#if 0
// FIXME
tree
tree_builtins::check_array_store ()
{
  if (! flag_store_check)
    return array;
  // fixme checks on a
}
#endif

tree
tree_builtins::map_identifier (const std::string &str)
{
  // Let gcc intern identifiers for us.
  return get_identifier (str.c_str ());
}

void
tree_builtins::add (tree context, model_method *meth)
{
  // Create a function or method type.
  tree mtype = make_node (meth->static_p () ? FUNCTION_TYPE : METHOD_TYPE);
  if (! meth->static_p ())
    TYPE_METHOD_BASETYPE (mtype) = map_type (meth->get_declaring_class ());
  TREE_TYPE (mtype) = map_type (meth->get_return_type ());

  // Convert argument types.
  std::list<ref_variable_decl> args = meth->get_parameters ();
  tree argt = NULL_TREE;
  if (! meth->static_p ())
    argt = tree_cons (NULL_TREE, TYPE_METHOD_BASETYPE (mtype), argt);
  for (std::list<ref_variable_decl>::const_iterator i = args.begin ();
       i != args.end ();
       ++i)
    argt = tree_cons (NULL_TREE, map_type ((*i)->type ()), argt);
  TYPE_ARG_TYPES (mtype) = chainon (nreverse (argt), void_list_node);

  layout_type (mtype);

  // Now create the function decl.
  tree result = build_decl (FUNCTION_DECL,
			    map_identifier (meth->get_name ()),
			    mtype);
  TREE_PUBLIC (result) = 1;
  DECL_CONTEXT (result) = context;
  SET_DECL_ASSEMBLER_NAME (result,
			   get_identifier (get_mangled_form (meth).c_str ()));

  // Convert the actual parameters.  We do this later because we want
  // to set the context when creating the parameter.
  tree formals = NULL_TREE;
  if (! meth->static_p ())
    {
      tree this_decl = build_decl (PARM_DECL, get_identifier ("this"),
				   TYPE_METHOD_BASETYPE (mtype));
      DECL_CONTEXT (this_decl) = result;
      TREE_CHAIN (this_decl) = formals;
      formals = this_decl;
    }
  for (std::list<ref_variable_decl>::const_iterator i = args.begin ();
       i != args.end ();
       ++i)
    {
      tree next_formal = map_parameter (result, (*i).get ());
      TREE_CHAIN (next_formal) = formals;
      formals = next_formal;
    }
  DECL_ARGUMENTS (result) = nreverse (formals);

  // Create the declaration for the result.
  {
    tree res_loc = build_decl (RESULT_DECL, NULL_TREE, TREE_TYPE (mtype));
    DECL_ARTIFICIAL (res_loc) = 1;
    DECL_IGNORED_P (res_loc) = 1;
    DECL_CONTEXT (res_loc) = result;
    DECL_RESULT (result) = res_loc;
  }

  TREE_CHAIN (result) = TYPE_METHODS (context);
  TYPE_METHODS (context) = result;

  // fixme DECL_SOURCE_FILE and DECL_SOURCE_LINE

  // FIXME: better check here... eg if we have a sealed jar we could
  // inline anything.  also perhaps have a flag for package-private.
  if ((meth->get_modifiers () & ACC_PRIVATE) != 0
      || meth->static_p ()
      || meth->final_p ())
    DECL_INLINE (result) = 1;
  if (meth->native_p ())
    DECL_EXTERNAL (result) = 1;	// FIXME wrong..?

  methodmap[meth] = result;
}

void
tree_builtins::add (tree context, model_field *field)
{
  // We don't allow a non-static field addition to Object or Class.
  // These classes are special and have their non-static fields
  // defined inside the compiler.
  if ((context == type_object || context == type_class)
      && ! field->static_p ())
    throw field->error ("can't add field to class %1 for native compilation")
      % field->get_declaring_class ();

  tree result = build_decl (field->static_p () ? VAR_DECL : FIELD_DECL,
			    map_identifier (field->get_name ()),
			    map_type (field->type ()));
  DECL_CONTEXT (result) = context;
  DECL_EXTERNAL (result) = 1;
  TREE_PUBLIC (result) = 1;
  SET_DECL_ASSEMBLER_NAME (result,
			   get_identifier (get_mangled_form (field).c_str ()));

  TREE_CHAIN (result) = TYPE_FIELDS (context);
  TYPE_FIELDS (context) = result;

  fieldmap[field] = result;
}

void
tree_builtins::initialize_type_map ()
{
  assert (typemap.empty ());

  // Set up all primitive types.
  typemap[primitive_byte_type] = type_jbyte;
  typemap[primitive_short_type] = type_jshort;
  typemap[primitive_int_type] = type_jint;
  typemap[primitive_long_type] = type_jlong;
  typemap[primitive_float_type] = type_jfloat;
  typemap[primitive_double_type] = type_jdouble;
  typemap[primitive_boolean_type] = type_jboolean;
  typemap[primitive_char_type] = type_jchar;
  typemap[primitive_void_type] = void_type_node;
  typemap[null_type] = ptr_type_node;
}

tree
tree_builtins::map_type (model_type *type)
{
  if (typemap.find (type) == typemap.end ())
    {
      assert (type->reference_p ());
      model_class *klass = assert_cast<model_class *> (type);

      // We have to specially handle Object and Class, as we know some
      // details about their layout.  The actual RECORD_TYPE tree for
      // these classes is made elsewhere; we simply reuse it here.
      // Also, we do not allow non-static fields to be found in either
      // of these classes.  Static fields are ok as we need to set the
      // serialVersionUID for Class, and it is best to keep that in
      // the library.
      bool special = false;
      tree record;
      if (type == global->get_compiler ()->java_lang_Object ())
	{
	  record = type_object;
	  special = true;
	}
      else if (type == global->get_compiler ()->java_lang_Class ())
	{
	  record = type_class;
	  special = true;
	}
      else
	record = make_node (RECORD_TYPE);
      TYPE_BINFO (record) = make_tree_binfo (0);

      // FIXME: pushdecl() ?
      tree decl = build_decl (TYPE_DECL, map_identifier (klass->get_name ()),
			      record);
      // FIXME: this isn't right... but we use it elsewhere.
      TYPE_STUB_DECL (record) = decl;

      typemap[klass] = build_pointer_type (record);
    }
  return typemap[type];
}

tree
tree_builtins::map_param_or_var (tree_code type, tree context,
				 model_variable_decl *var)
{
  if (varmap.find (var) == varmap.end ())
    {
      tree result = build_decl (type,
				map_identifier (var->get_name ()),
				map_type (var->type ()));
      DECL_CONTEXT (result) = context;
      varmap[var] = result;
    }
  return varmap[var];
}

tree
tree_builtins::map_parameter (tree context, model_variable_decl *var)
{
  return map_param_or_var (PARM_DECL, context, var);
}

tree
tree_builtins::map_variable (tree context, model_variable_decl *var)
{
  return map_param_or_var (VAR_DECL, context, var);
}

tree
tree_builtins::map_field (model_field *field)
{
  assert (fieldmap.find (field) != fieldmap.end ());
  return fieldmap[field];
}

tree
tree_builtins::map_field_ref (tree obj, model_field *field)
{
  gcj_abi *abi = find_abi (field->get_declaring_class ());

  tree result
    = abi->build_field_reference (this, field->static_p () ? NULL_TREE : obj,
				  field);

  if (obj != NULL_TREE && field->static_p ())
    {
      // We're evaluating OBJ for side effects only.
      result = build2 (COMPOUND_EXPR, TREE_TYPE (result),
		       obj, result);
    }

  return result;
}

tree
tree_builtins::map_field_ref (tree, const std::string &, const std::string &,
			      const std::string &)
{
  // FIXME
  abort ();
}

tree
tree_builtins::map_method (model_method *meth)
{
  assert (methodmap.find (meth) != methodmap.end ());
  return methodmap[meth];
}

tree
tree_builtins::map_method_call (tree obj, tree args, model_method *meth,
				bool is_super)
{
  gcj_abi *abi = find_abi (meth->get_declaring_class ());
  tree result = abi->build_method_call (this,
					meth->static_p () ? NULL_TREE : obj,
					args, meth, is_super);
  // FIXME: set this in ABI?
  TREE_SIDE_EFFECTS (result) = 1;

  if (obj != NULL_TREE && meth->static_p ())
    {
      // We're evaluating OBJ for side effects only.
      result = build2 (COMPOUND_EXPR, TREE_TYPE (result),
		       obj, result);
      TREE_SIDE_EFFECTS (result) = 1;
    }

  return result;
}

tree
tree_builtins::map_new (model_class *klass, tree constructor, tree arguments)
{
  gcj_abi *abi = find_abi (klass);
  return abi->build_new (this, map_type (klass), constructor, arguments);
}

void
tree_builtins::lay_out_vtable (model_class *klass)
{
  AllMethodsIterator end_all_methods = klass->end_all_methods ();
  std::list<ref_method> super_methods;
  tree super_vtable = NULL_TREE;
  model_class *super = klass->get_superclass ();
  if (super != NULL)
    {
      for (AllMethodsIterator it = klass->begin_all_methods ();
           it != end_all_methods;
           ++it)
        {
          super_methods.push_back (*it);
        }
      tree super_ptr_type = map_type (super);
      super_vtable = BINFO_VTABLE (TYPE_BINFO (TREE_TYPE (super_ptr_type)));
    }

  int last_max = super_vtable ? TREE_VEC_LENGTH (super_vtable) : 0;
  for (AllMethodsIterator i = klass->begin_all_methods ();
       i != end_all_methods;
       ++i)
    {
      if ((*i)->static_p ())
	continue;

      int index = -1;
      model_class *decl = (*i)->get_declaring_class ();
      if (decl->interface_p ())
	{
	  assert (super != NULL);
	  // If this interface is implemented by some superclass, then
	  // the method is already in the vtable.
	  if (decl->assignable_from_p (super))
	    continue;
	}
      else if (decl != klass)
	{
	  // Declared in a concrete superclass, so it is already in
	  // the vtable.
	  continue;
	}
      else
	{
	  // Method is declared here.  It still might overload some
	  // method from the superclass.
	  for (std::list<ref_method>::const_iterator si
		 = super_methods.begin ();
	       si != super_methods.end ();
	       ++si)
	    {
	      if ((*i)->hides_or_overrides_p ((*si).get (), klass))
		{
		  // Re-use the previous slot.
		  index
		    = tree_low_cst (DECL_VINDEX (map_method ((*si).get ())),
				    0);
		  break;
		}
	    }
	}

      // We've found a method that isn't declared in some superclass,
      // so assign it a vtable index.
      if (index == -1)
	index = last_max++;
      tree m = map_method ((*i).get ());
      // FIXME: if M is an interface method then we're screwed here!
      DECL_VINDEX (m) = size_int_kind (index, SIZETYPE);
    }

  tree vtable = make_tree_vec (last_max);

  if (super_vtable != NULL_TREE)
    {
      for (int i = 0; i < TREE_VEC_LENGTH (super_vtable); ++i)
	TREE_VEC_ELT (vtable, i) = TREE_VEC_ELT (super_vtable, i);
    }

  for (AllMethodsIterator i = klass->begin_all_methods ();
       i != end_all_methods;
       ++i)
    {
      tree m = map_method ((*i).get ());
      TREE_VEC_ELT (vtable, tree_low_cst (DECL_VINDEX (m), 0)) = m;
    }

  tree klass_ptr_type = map_type (klass);
  BINFO_VTABLE (TYPE_BINFO (TREE_TYPE (klass_ptr_type))) = vtable;
}

tree
tree_builtins::lay_out_class (model_class *klass)
{
  //   if (class laid out)
  //     return fixme;
  tree klass_tree = map_type (klass);
  tree klass_record = TREE_TYPE (klass_tree);

  if (klass->get_superclass () != NULL)
    lay_out_class (klass->get_superclass ());

  // Ensure all non-static methods have been added.
  std::list<ref_method> methods = klass->get_methods ();
  for (std::list<ref_method>::const_iterator i = methods.begin ();
       i != methods.end ();
       ++i)
    add (klass_record, (*i).get ());
  // Fix the ordering.
  TYPE_METHODS (klass_record) = nreverse (TYPE_METHODS (klass_record));

  // Ensure all non-static fields have been added.
  std::list<ref_field> fields = klass->get_fields ();
  for (std::list<ref_field>::const_iterator i = fields.begin ();
       i != fields.end ();
       ++i)
    add (klass_record, (*i).get ());

  // For arrays, add a "data" member.  This field isn't visible to
  // java, but is needed for code generation.
  if (klass->array_p ())
    {
      tree data = build_decl (FIELD_DECL, get_identifier ("data"),
			      map_type (klass->element_type ()));
      DECL_CONTEXT (data) = klass_record;
      TREE_PUBLIC (data) = 1;
      DECL_ARTIFICIAL (data) = 1;

      TREE_CHAIN (data) = TYPE_FIELDS (klass_record);
      TYPE_FIELDS (klass_record) = data;

      // Also update the "length" field.
      tree length_field = find_decl (klass_record, "length");
      TREE_READONLY (length_field) = 1;
    }

  // Fix the ordering.
  TYPE_FIELDS (klass_record) = nreverse (TYPE_FIELDS (klass_record));

  lay_out_vtable (klass);

  layout_type (klass_tree);
  return klass_tree;
}

tree
tree_builtins::find_decl (tree type, const char *name)
{
  // This may only be called for local fields.
  tree tname = get_identifier (name);
  for (tree field = TYPE_FIELDS (type); field; field = TREE_CHAIN (field))
    {
      if (DECL_NAME (field) == tname)
	return field;
    }
  abort ();
}

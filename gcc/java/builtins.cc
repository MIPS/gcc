// Various built-in functions.

// Copyright (C) 2004, 2005, 2006 Free Software Foundation, Inc.
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
#include "aot/mangle.hh"
#include "java/util.h"

tree_builtins::tree_builtins ()
  : aot_class_factory (),
    symbol_count (0)
{
}

tree_builtins::~tree_builtins ()
{
}



gcj_abi *
tree_builtins::find_abi ()
{
  if (flag_indirect_dispatch)
    return &new_abi;
  return &old_abi;
}

tree
tree_builtins::check_reference (tree ref, bool override)
{
  if (flag_check_references || override)
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
  // Convert argument types.
  std::list<ref_variable_decl> args = meth->get_parameters ();
  tree argt = NULL_TREE;
  for (std::list<ref_variable_decl>::const_iterator i = args.begin ();
       i != args.end ();
       ++i)
    argt = tree_cons (NULL_TREE, map_type ((*i)->type ()), argt);
  argt = chainon (nreverse (argt), void_list_node);

  // Create a function or method type.
  tree ret_type = map_type (meth->get_return_type ());
  tree mtype;
  tree klass_ptr = map_type (meth->get_declaring_class ());
  if (meth->static_p ())
    mtype = build_function_type (ret_type, argt);
  else
    mtype = build_method_type_directly (TREE_TYPE (klass_ptr), ret_type, argt);

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
				   klass_ptr);
      DECL_CONTEXT (this_decl) = result;
      DECL_ARG_TYPE (this_decl) = TREE_TYPE (this_decl);
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

  // We mark all methods as external, but then we clear this bit when
  // we actually compile the method in question.
  DECL_EXTERNAL (result) = 1;

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
  TREE_PUBLIC (result) = 1;
  if (field->static_p ())
    SET_DECL_ASSEMBLER_NAME (result,
			     get_identifier (get_mangled_form (field).c_str ()));

  // Only chain instance fields here.
  if (! field->static_p ())
    {
      TREE_CHAIN (result) = TYPE_FIELDS (context);
      TYPE_FIELDS (context) = result;
    }
  else
    {
      TREE_STATIC (result) = 1;
      // We clear this later if we compile the field's declaring
      // class.
      DECL_EXTERNAL (result) = 1;
      pushdecl (result);
    }

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
      if (! klass->array_p ())
	TYPE_NAME (record)
	  = map_identifier (klass->get_fully_qualified_name ());

      // FIXME: make a NAMESPACE_DECL and use it as the DECL_CONTEXT.

      // FIXME: should we use the class's name or its fully qualified
      // name?  For the moment we use the latter.
      tree decl = build_decl (TYPE_DECL, TYPE_NAME (record), record);
      // FIXME: this isn't right... but we use it elsewhere.
      TYPE_STUB_DECL (record) = decl;

      typemap[klass] = build_pointer_type (record);

      pushdecl (typemap[klass]);
    }
  return typemap[type];
}

tree
tree_builtins::map_class_object (model_class *klass)
{
  if (classobj_map.find (klass) == classobj_map.end ())
    {
      tree decl = build_decl (VAR_DECL, NULL_TREE, type_class);
      TREE_PUBLIC (decl) = 1;
      TREE_STATIC (decl) = 1;
      DECL_ARTIFICIAL (decl) = 1;
      DECL_IGNORED_P (decl) = 1;
      SET_DECL_ASSEMBLER_NAME (decl,
			       get_identifier (get_class_object_name (klass).c_str ()));
      DECL_NAME (decl) = DECL_ASSEMBLER_NAME (decl);

      classobj_map[klass] = decl;
      pushdecl (decl);
    }
  return classobj_map[klass];
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
      if (type == PARM_DECL)
	{
	  // Promote argument types here.  I think this assumes that
	  // 'jint' and the ABI 'int' are the same... FIXME?
	  tree argtype = TREE_TYPE (result);
	  if (targetm.calls.promote_prototypes (argtype)
	      && TYPE_PRECISION (argtype) < TYPE_PRECISION (type_jint)
	      && INTEGRAL_TYPE_P (argtype))
	    argtype = type_jint;
	  DECL_ARG_TYPE (result) = argtype;
	}
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
tree_builtins::map_field_ref (aot_class *wrapper, tree obj, model_field *field)
{
  gcj_abi *abi = find_abi ();

  tree result
    = abi->build_field_reference (this, wrapper,
				  field->static_p () ? NULL_TREE : obj,
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
tree_builtins::map_field_ref (aot_class *wrapper, tree obj,
			      const std::string &classname,
			      const std::string &fieldname,
			      const std::string &descriptor)
{
  gcj_abi *abi = find_abi ();
  return abi->build_field_reference (this, wrapper, obj, classname,
				     fieldname, descriptor);
}

tree
tree_builtins::map_method (model_method *meth)
{
  // An abstract method can itself be derived from an abstract method.
  // We strip them all off to get the method as actually declared.
  while (dynamic_cast<model_abstract_method *> (meth))
    {
      model_abstract_method *mam = assert_cast<model_abstract_method *> (meth);
      meth = mam->get_original ();
    }

  assert (methodmap.find (meth) != methodmap.end ());
  return methodmap[meth];
}

tree
tree_builtins::map_method_call (aot_class *wrapper,
				tree obj, tree args, model_method *meth,
				bool is_super)
{
  gcj_abi *abi = find_abi ();
  tree result = abi->build_method_call (this, wrapper,
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
tree_builtins::map_method_call (aot_class *wrapper, tree obj, tree args,
				const std::string &classname,
				const std::string &meth_name,
				const std::string &meth_descriptor)
{
  gcj_abi *abi = find_abi ();
  return abi->build_method_call (this, wrapper, obj, args,
				 classname, meth_name, meth_descriptor);
}

tree
tree_builtins::map_new (aot_class *current, model_class *klass,
			model_method *constructor, tree arguments)
{
  gcj_abi *abi = find_abi ();
  return abi->build_new (this, current, klass, constructor, arguments);
}

tree
tree_builtins::build_utf8const_type (int len)
{
  if (utf8typemap.find (len) == utf8typemap.end ())
    {
      tree field = NULL_TREE;
      tree new_type = make_node (RECORD_TYPE);
      push_field (new_type, field, "hash", type_jushort);
      push_field (new_type, field, "length", type_jushort);
      push_field (new_type, field, "data",
		  build_array_type (char_type_node,
				    build_index_type (build_int_cst (type_jint,
								     len))));
      layout_type (new_type);
      utf8typemap[len] = new_type;
    }
  return utf8typemap[len];
}

int
tree_builtins::hash_utf8 (const char *s, int len)
{
  const unsigned char *ptr = (const unsigned char *) s;
  const unsigned char *limit = ptr + len;
  int hash = 0;
  while (ptr < limit)
    {
      int ch = UTF8_GET (ptr, limit);
      /* Updated specification from
	 http://www.javasoft.com/docs/books/jls/clarify.html. */
      hash = (31 * hash) + ch;
    }
  return hash;
}

tree
tree_builtins::map_utf8const (const std::string &value)
{
  if (utf8map.find (value) == utf8map.end ())
    {
      // Note: add 1 to length here for trailing \0.
      tree type = build_utf8const_type (value.length () + 1);
      int hash = hash_utf8 (value.c_str (), value.length ());

      record_creator utf (type);
      utf.set_field ("hash", build_int_cst (type_jushort, hash & 0xffff));
      utf.set_field ("length", build_int_cst (type_jushort, value.length ()));

      tree str = build_string (value.length (), value.c_str ());
      tree strtype = TREE_TYPE (find_decl (type, "data"));
      TREE_TYPE (str) = strtype;
      TREE_CONSTANT (str) = 1;
      TREE_INVARIANT (str) = 1;
      TREE_READONLY (str) = 1;
      TREE_STATIC (str) = 1;
      utf.set_field ("data", str);

      tree init = utf.finish_record ();
      TREE_CONSTANT (init) = 1;
      TREE_INVARIANT (init) = 1;
      TREE_READONLY (init) = 1;

      char buf[20];
      sprintf (buf, "_Utf%d", utf8map.size ());

      tree decl = build_decl (VAR_DECL, get_identifier (buf), type);
      TREE_STATIC (decl) = 1;
      DECL_ARTIFICIAL (decl) = 1;
      DECL_IGNORED_P (decl) = 1;
      TREE_READONLY (decl) = 1;
      DECL_INITIAL (decl) = init;

      if (HAVE_GAS_SHF_MERGE)
	{
	  int decl_size;
	  // Ensure decl_size is a multiple of utf8const_type's alignment.
	  decl_size = ((value.length () + 5 + TYPE_ALIGN_UNIT (type) - 1)
		       & ~(TYPE_ALIGN_UNIT (type) - 1));
	  if (flag_merge_constants && decl_size < 256)
	    {
	      char buf[32];
	      int flags = (SECTION_OVERRIDE
			   | SECTION_MERGE | (SECTION_ENTSIZE & decl_size));
	      sprintf (buf, ".rodata.jutf8.%d", decl_size);
	      named_section_flags (buf, flags);
	      DECL_SECTION_NAME (decl) = build_string (strlen (buf), buf);
	    }
	}

      layout_decl (decl, 0);
      rest_of_decl_compilation (decl, 1, 0);
      make_decl_rtl (decl);

      utf8map[value] = build_address_of (decl);
      pushdecl (utf8map[value]);
    }
  return utf8map[value];
}

tree
tree_builtins::get_vtable_decl (model_class *klass)
{
  if (vtable_map.find (klass) == vtable_map.end ())
    {
      tree decl = build_decl (VAR_DECL, NULL_TREE, type_dtable);
      TREE_PUBLIC (decl) = 1;
      TREE_STATIC (decl) = 1;
      DECL_ARTIFICIAL (decl) = 1;
      DECL_IGNORED_P (decl) = 1;
      mangler m (klass, true);
      SET_DECL_ASSEMBLER_NAME (decl,
			       get_identifier (m.get ().c_str ()));
      DECL_NAME (decl) = DECL_ASSEMBLER_NAME (decl);

      vtable_map[klass] = decl;
    }
  return vtable_map[klass];
}

tree
tree_builtins::get_constant_pool_decl (model_class *klass)
{
  if (cpool_map.find (klass) == cpool_map.end ())
    {
      tree type = build_array_type (ptr_type_node,
				    build_index_type (integer_zero_node));
      tree decl = build_decl (VAR_DECL, washed_name ("_cpool_", klass), type);
      TREE_STATIC (decl) = 1;
      DECL_ARTIFICIAL (decl) = 1;
      DECL_IGNORED_P (decl) = 1;

      cpool_map[klass] = decl;
    }
  return cpool_map[klass];
}

tree
tree_builtins::get_atable_decl (model_class *klass)
{
  if (atable_map.find (klass) == atable_map.end ())
    {
      tree decl = build_decl (VAR_DECL, washed_name ("_atable_", klass),
			      type_atable);
      TREE_STATIC (decl) = 1;
      DECL_ARTIFICIAL (decl) = 1;
      DECL_IGNORED_P (decl) = 1;
      atable_map[klass] = decl;
      pushdecl (decl);
    }
  return atable_map[klass];
}

tree
tree_builtins::get_otable_decl (model_class *klass)
{
  if (otable_map.find (klass) == otable_map.end ())
    {
      tree decl = build_decl (VAR_DECL, washed_name ("_otable_", klass),
			      type_otable);
      TREE_STATIC (decl) = 1;
      DECL_ARTIFICIAL (decl) = 1;
      DECL_IGNORED_P (decl) = 1;
      otable_map[klass] = decl;
      pushdecl (decl);
    }
  return otable_map[klass];
}

tree
tree_builtins::get_itable_decl (model_class *klass)
{
  if (itable_map.find (klass) == itable_map.end ())
    {
      // Note: uses same type as atable.
      tree decl = build_decl (VAR_DECL, washed_name ("_itable_", klass),
			      type_atable);
      TREE_STATIC (decl) = 1;
      DECL_ARTIFICIAL (decl) = 1;
      DECL_IGNORED_P (decl) = 1;
      itable_map[klass] = decl;
      pushdecl (decl);
    }
  return itable_map[klass];
}

tree
tree_builtins::map_catch_class (model_class *current, model_class *caught)
{
  catch_map_type::iterator it = catch_map.find (current);
  std::map<model_class *, tree> *inner_map;
  if (it == catch_map.end ())
    {
      std::map<model_class *, tree> new_map;
      catch_map[current] = new_map;
      inner_map = &new_map;
    }
  else
    inner_map = &((*it).second);

  std::map<model_class *, tree>::iterator inner_it
    = (*inner_map).find (caught);
  if (inner_it != (*inner_map).end ())
    return (*inner_it).second;

  tree decl = build_decl (VAR_DECL, get_symbol (), type_class_ptr);
  TREE_STATIC (decl) = 1;
  DECL_ARTIFICIAL (decl) = 1;
  DECL_IGNORED_P (decl) = 1;

  // get_catch_initializer will always return the correct DECL_INITIAL
  // -- but for the BC ABI do we need more processing as well?
  gcj_abi *abi = find_abi ();
  DECL_INITIAL (decl) = abi->get_catch_initializer (this, caught);

  rest_of_decl_compilation (decl, 1, 0);
  pushdecl (decl);

  (*inner_map)[caught] = decl;
  return decl;
}

std::map<model_class *, tree> *
tree_builtins::get_catch_map (model_class *current)
{
  catch_map_type::iterator it = catch_map.find (current);
  if (it == catch_map.end ())
    return NULL;
  return &(*it).second;
}

// FIXME: this whole method should probably migrate into the ABI or
// into classobj.cc.  There's no need, I think, for it to be a generic
// part of the builtins.
void
tree_builtins::lay_out_vtable (model_class *mklass)
{
  aot_class *klass = get_class (mklass);
  const std::vector<model_method *> &vtable (klass->get_vtable ());

  // Create a new tree vector to represent the vtable, and fill it in.
  // Note that we have two empty slots at the beginning; this is kept
  // in sync with aot_class.  FIXME: define a constant.
  // FIXME: should move into aotclass.
  tree vtable_tree = make_tree_vec (2 + vtable.size ());
  int index = 2;
  for (std::vector<model_method *>::const_iterator i = vtable.begin ();
       i != vtable.end ();
       ++i)
    {
      tree meth;
      // FIXME: consider using _Jv_ThrowNoSuchMethodError.
      if ((*i)->abstract_p ())
	meth = null_pointer_node;
      else
	meth = build_address_of (map_method (*i));
      TREE_VEC_ELT (vtable_tree, index) = meth;
      ++index;
    }

  gcj_abi *abi = find_abi ();
  TREE_VEC_ELT (vtable_tree, 0) = abi->build_direct_class_reference (this,
								     klass,
								     mklass);
  TREE_VEC_ELT (vtable_tree, 1) = null_pointer_node; // FIXME: GC descriptor

  tree klass_ptr_type = map_type (klass->get ());
  BINFO_VTABLE (TYPE_BINFO (TREE_TYPE (klass_ptr_type))) = vtable_tree;
}

tree
tree_builtins::lay_out_class (model_class *klass)
{
  tree klass_tree = map_type (klass);
  if (TYPE_LANG_FLAG_0 (klass_tree))
    return klass_tree;
  TYPE_LANG_FLAG_0 (klass_tree) = 1;

  tree klass_record = TREE_TYPE (klass_tree);

  // The class itself must be resolved for layout to work.
  klass->resolve_members ();

  // Lay out superclasses and interfaces.
  tree super_record = NULL_TREE;
  if (klass->get_superclass () != NULL)
    super_record = TREE_TYPE (lay_out_class (klass->get_superclass ()));

  std::list<ref_forwarding_type> &ifaces (klass->get_interfaces ());
  for (std::list<ref_forwarding_type>::const_iterator i = ifaces.begin ();
       i != ifaces.end ();
       ++i)
    {
      model_class *ik = assert_cast<model_class *> ((*i)->type ());
      lay_out_class (ik);
    }

  // Ensure all non-static methods have been added.
  std::list<ref_method> methods = klass->get_methods ();
  for (std::list<ref_method>::const_iterator i = methods.begin ();
       i != methods.end ();
       ++i)
    add (klass_record, (*i).get ());
  // Fix the ordering.
  TYPE_METHODS (klass_record) = nreverse (TYPE_METHODS (klass_record));

  // FIXME: This is sort of bogus, because we could easily create
  // Class and Object here.  Meanwhile, these two classes have their
  // fields added elsewhere.  Skipping any class that already has
  // fields should suffice.
  if (TYPE_FIELDS (klass_record) == NULL_TREE)
    {
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
	  tree elt_type = map_type (klass->element_type ());
	  tree data = build_decl (FIELD_DECL, get_identifier ("data"),
				  build_array_type (elt_type,
						    build_index_type (integer_zero_node)));
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

      // Link to the superclass.
      if (super_record != NULL_TREE)
	{
	  tree base = build_decl (FIELD_DECL, NULL_TREE, super_record);
	  DECL_IGNORED_P (base) = 1;
	  TREE_CHAIN (base) = TYPE_FIELDS (klass_record);
	  TYPE_FIELDS (klass_record) = base;
	  DECL_SIZE (base) = TYPE_SIZE (super_record);
	  DECL_SIZE_UNIT (base) = TYPE_SIZE_UNIT (super_record);
	}
    }

  // Laying out an array's vtable here is incorrect, since we don't
  // plan to write out the vtable.  FIXME: really we should only lay
  // out a vtable when we write the C++ ABI class itself...?
  if (! klass->array_p ())
    lay_out_vtable (klass);

  layout_type (klass_record);
  return klass_tree;
}

tree
tree_builtins::find_decl (tree type, const char *name)
{
  // This may only be called for local fields.
  tree tname = get_identifier (name);
  tree result = NULL_TREE;
  for (tree field = TYPE_FIELDS (type); field; field = TREE_CHAIN (field))
    {
      if (DECL_NAME (field) == tname)
	{
	  result = field;
	  break;
	}
    }
  assert (result != NULL_TREE);
  return result;
}

std::string
tree_builtins::get_class_object_name (model_class *klass)
{
  mangler m (klass, "class$");
  return m.get ();
}

tree
tree_builtins::get_symbol ()
{
  char buf[50];
  sprintf (buf, "_temp_%d", symbol_count);
  ++symbol_count;
  return get_identifier (buf);
}

tree
tree_builtins::washed_name (const std::string &prefix, model_class *klass)
{
  std::string result = klass->get_fully_qualified_name ();
  replace_all (result, '.', '_');
  return get_identifier ((prefix + result).c_str ());
}

tree
tree_builtins::build_class_init (model_class *klass)
{
  aot_class *wrapper = get_class (klass);
  gcj_abi *abi = find_abi ();
  tree ref = abi->build_class_reference (this, wrapper, klass);

  tree result
    = build3 (CALL_EXPR, void_type_node,
	      builtin_Jv_InitClass,
	      build_tree_list (NULL_TREE,
			       // FIXME: why do we declare this
			       // function with the wrong argument
			       // type?
			       build1 (NOP_EXPR, ptr_type_node, ref)),
	      NULL_TREE);
  return result;
}

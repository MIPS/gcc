// Create a Class instance.

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

#include "java/glue.hh"

record_creator::record_creator (tree record_type)
{
  constructor = build_constructor (record_type, NULL_TREE);
  field_iterator = TYPE_FIELDS (record_type);
}

record_creator::~record_creator ()
{
  assert (field_iterator == NULL_TREE);
  assert (constructor == NULL_TREE);
}

void
record_creator::set_field (const char *name, tree value)
{
  assert (! strcmp (IDENTIFIER_POINTER (DECL_NAME (field_iterator)), name));
  CONSTRUCTOR_ELTS (constructor) = tree_cons (field_iterator, value,
					      CONSTRUCTOR_ELTS (constructor));
  field_iterator = TREE_CHAIN (field_iterator);
}

tree
record_creator::finish_record ()
{
  CONSTRUCTOR_ELTS (constructor) = nreverse (CONSTRUCTOR_ELTS (constructor));
  tree result = constructor;
  constructor = NULL_TREE;
  return result;
}



tree
class_object_creator::make_decl (tree type, tree value)
{
  tree decl = build_decl (VAR_DECL, NULL_TREE, type);
  DECL_INITIAL (decl) = build_constructor (type, value);
  TREE_STATIC (decl) = 1;
  DECL_ARTIFICIAL (decl) = 1;
  DECL_IGNORED_P (decl) = 1;
  rest_of_decl_compilation (decl, 1, 0);

  return build1 (ADDR_EXPR, build_pointer_type (type), decl);
}

tree
class_object_creator::create_one_field_record (model_field *field)
{
  record_creator inst (type_field);

  tree fdecl = builtins->map_field (field);

  inst.set_field ("name", builtins->map_utf8const (field->get_name ()));
  inst.set_field ("type", builtins->map_type (field->type ()));
  inst.set_field ("accflags", build_int_cst (type_jint,
					     field->get_modifiers ()));
  inst.set_field ("bsize", TYPE_SIZE_UNIT (TREE_TYPE (fdecl)));

  tree value;
  tree ufield = TYPE_FIELDS (type_field_info_union);
  if (field->static_p ())
    {
      ufield = TREE_CHAIN (ufield);
      value = build_address_of (fdecl);
    }
  else
    value = byte_position (fdecl);

  tree info = build_constructor (type_field_info_union,
				 build_tree_list (ufield, value));
  inst.set_field ("info", info);

  return inst.finish_record ();
}

tree
class_object_creator::create_field_array (model_class *real_class,
					  int &num_fields,
					  int &num_static_fields)
{
  std::list<ref_field> fields = real_class->get_fields ();

  num_fields = 0;
  num_static_fields = 0;
  tree field_array = NULL_TREE;
  for (std::list<ref_field>::const_iterator i = fields.begin ();
       i != fields.end ();
       ++i)
    {
      tree elt = create_one_field_record ((*i).get ());
      field_array = tree_cons (NULL_TREE, elt, field_array);
      if ((*i)->static_p ())
	++num_static_fields;
      else
	++num_fields;
    }

  if (field_array == NULL_TREE)
    return null_pointer_node;
  field_array = nreverse (field_array);

  return make_decl (type_field_array, field_array);
}

tree
class_object_creator::create_method_array (model_class *real_class, int &)
{
  return null_pointer_node;	// FIXME
}

void
class_object_creator::create_index_table (const std::vector<model_element *> &table,
					  tree &result_table,
					  tree &result_syms)
{
  for (std::vector<model_element *>::const_iterator i = table.begin ();
       i != table.end ();
       ++i)
    {
      // We can be looking at a field or a method.
      // FIXME: it would be nice if we could use IMember; it would
      // need name and descriptor accessors.
      std::string class_desc, name, descriptor;
      if (dynamic_cast<model_field *> (*i))
	{
	  model_field *field = assert_cast<model_field *> (*i);
	  class_desc = field->get_declaring_class ()->get_descriptor ();
	  name = field->get_name ();
	  descriptor = field->type ()->get_descriptor ();
	}
      else
	{
	  model_method *method = assert_cast<model_method *> (*i);
	  class_desc = method->get_declaring_class ()->get_descriptor ();
	  name = method->get_name ();
	  descriptor = method->get_descriptor ();
	}

      // FIXME: enter new utf8consts.
      
    }

  // FIXME create the tables and update the arguments
}

void
class_object_creator::handle_interfaces (model_class *real_class,
					 tree &interfaces,
					 tree &iface_len)
{
  std::list<ref_forwarding_type> ifaces (real_class->get_interfaces ());
  int len = 0;

  if (ifaces.empty ())
    interfaces = null_pointer_node;
  else
    {
      tree result = NULL_TREE;
      for (std::list<ref_forwarding_type>::const_iterator i = ifaces.begin ();
	   i != ifaces.end ();
	   ++i)
	{
	  ++len;
	  gcj_abi *abi = builtins->find_abi ((*i)->type ());
	  tree one_iface
	    = abi->build_class_reference (builtins,
					  builtins->map_type ((*i)->type ()));
	  result = tree_cons (NULL_TREE, one_iface, result);
	}
      result = nreverse (result);

      // Make a new type which is an array of 'jclass' of the
      // appropriate length.
      tree type_index = build_index_type (build_int_cst (sizetype, len - 1));
      tree type_interface_array = build_array_type (type_class_ptr,
						    type_index);

      interfaces = make_decl (type_interface_array, result);
    }

  iface_len = build_int_cst (type_jshort, len);

}

void
class_object_creator::create_class_instance (tree class_tree)
{
  assert (TREE_CODE (class_tree) == RECORD_TYPE);

  // FIXME: handle fields in Object.

  model_class *real_class = klass->get ();
  gcj_abi *abi = builtins->find_abi (real_class);
  record_creator inst (type_class);

  inst.set_field ("next", null_pointer_node);
  inst.set_field ("name",
		  builtins->map_utf8const (real_class->get_fully_qualified_name ()));

  int mods = real_class->get_modifiers ();
  // Inner classes have modifiers like top-level classes, where the
  // only valid values are public and package-private.
  if ((mods & ACC_ACCESS) != 0 && (mods & ACC_ACCESS) != ACC_PUBLIC)
    mods &= ~ACC_ACCESS;
  inst.set_field ("accflags", build_int_cst (type_jushort, mods));

  model_class *super = real_class->get_superclass ();
  if (real_class->interface_p ())
    super = global->get_compiler ()->java_lang_Object ();
  inst.set_field ("superclass",
		  super ? builtins->map_type (real_class->get_superclass ())
		  : null_pointer_node);

  inst.set_field ("constants", null_pointer_node);  // FIXME

  int method_len;
  tree methods = create_method_array (real_class, method_len);
  inst.set_field ("methods", methods);
  inst.set_field ("method_count", build_int_cst (type_jshort, method_len));
  inst.set_field ("vtable_method_count",
		  build_int_cst (type_jshort,
				 TREE_VEC_LENGTH (BINFO_VTABLE (TYPE_BINFO (class_tree)))));

  int num_fields, num_static_fields;
  tree field_array = create_field_array (real_class, num_fields,
					 num_static_fields);

  inst.set_field ("fields", field_array);
  inst.set_field ("size_in_bytes", abi->get_size_in_bytes (class_tree));
  inst.set_field ("field_count", build_int_cst (type_jshort, num_fields));
  inst.set_field ("static_field_count",
		  build_int_cst (type_jshort, num_static_fields));

  // FIXME abi->get_vtable (blah));
  inst.set_field ("vtable", null_pointer_node);

  tree table, syms;
  create_index_table (klass->get_otable (), table, syms);
  inst.set_field ("otable", table);
  inst.set_field ("otable_syms", syms);

  create_index_table (klass->get_atable (), table, syms);
  inst.set_field ("atable", table);
  inst.set_field ("atable_syms", syms);

  create_index_table (klass->get_itable (), table, syms);
  inst.set_field ("itable", table);
  inst.set_field ("itable_syms", syms);

  inst.set_field ("catch_classes", null_pointer_node);  // FIXME

  tree interfaces, interface_count;
  handle_interfaces (real_class, interfaces, interface_count);
  inst.set_field ("interfaces", interfaces);
  inst.set_field ("loader", null_pointer_node);
  inst.set_field ("interface_count", interface_count);

  inst.set_field ("state", build_int_cst (type_jbyte,
					  abi->get_class_state ()));
  inst.set_field ("thread", null_pointer_node);
  inst.set_field ("depth", integer_zero_node);
  inst.set_field ("ancestors", null_pointer_node);
  inst.set_field ("idt", null_pointer_node);
  inst.set_field ("arrayclass", null_pointer_node);
  inst.set_field ("protectionDomain", null_pointer_node);
  inst.set_field ("assertion_table", null_pointer_node);  // FIXME
  inst.set_field ("signers", null_pointer_node);
  inst.set_field ("chain", null_pointer_node);
  inst.set_field ("aux_info", null_pointer_node);
  inst.set_field ("engine", null_pointer_node);

  tree init = inst.finish_record ();

  result = make_decl (type_class, init);
}

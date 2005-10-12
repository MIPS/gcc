// ABI interface.

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
#include "java/hooks.hh"
#include "java/builtins.hh"

gcj_abi::gcj_abi ()
{
}

gcj_abi::~gcj_abi ()
{
}



tree
cxx_abi::build_method_call (tree_builtins *builtins,
			    aot_class *,
			    tree obj, tree args, model_method *meth,
			    bool is_super)
{
  builtins->lay_out_class (meth->get_declaring_class ());

  assert (! meth->static_initializer_p ());
  tree meth_tree = builtins->map_method (meth);

  // An abstract method can itself be derived from an abstract method.
  // We strip them all off to get the method as actually declared.  We
  // have to do this here because the abstract method might wrap a
  // 'final' method from Object, and we use a different invocation
  // mode for those.
  while (dynamic_cast<model_abstract_method *> (meth))
    {
      model_abstract_method *mam = assert_cast<model_abstract_method *> (meth);
      meth = mam->get_original ();
    }

  tree func;
  if (meth->static_p ())
    {
      assert (obj == NULL_TREE);
      func = build_address_of (meth_tree);
    }
  else if (meth->get_declaring_class ()->interface_p ())
    {
      assert (obj != NULL_TREE);

      // FIXME: use _Jv_LookupInterfaceMethodIdx.

      obj = save_expr (obj);
      args = tree_cons (NULL_TREE, obj, args);

      obj = builtins->check_reference (obj);
      tree dtable = build1 (INDIRECT_REF, type_object,
			    build1 (NOP_EXPR, type_object_ptr, obj));
      dtable = build3 (COMPONENT_REF, type_dtable_ptr,
		       dtable,
		       builtins->find_decl (type_object, "vtable"),
		       NULL_TREE);

      tree obj_class = build3 (COMPONENT_REF, type_class_ptr,
			       build1 (INDIRECT_REF, type_dtable, dtable),
			       builtins->find_decl (type_dtable, "class"),
			       NULL_TREE);

      tree name_tree = builtins->map_utf8const (meth->get_name ());
      tree desc_tree = builtins->map_utf8const (tree_builtins::get_descriptor (meth));

      tree lookup_args
	= tree_cons (NULL_TREE, obj_class,
		     tree_cons (NULL_TREE, name_tree,
				build_tree_list (NULL_TREE, desc_tree)));

      func = build3 (CALL_EXPR, ptr_type_node,
		     builtin_Jv_LookupInterfaceMethod,
		     lookup_args, NULL_TREE);
      func = build1 (NOP_EXPR, build_pointer_type (TREE_TYPE (meth_tree)),
		     func);
    }
  else if (is_super || meth->final_p () || meth->constructor_p ()
	   || (meth->get_modifiers () & ACC_PRIVATE) != 0)
    {
      assert (obj != NULL_TREE);

      // A final method, a constructor, or a super method should be
      // called directly.  A method in a final class is implicitly
      // final, and will be caught by this same condition.  Private
      // methods are also caught this way.
      func = build_address_of (meth_tree);

      // In some cases we must generate an explicit null check.  We
      // leave it to the optimizers to deduce that 'this != null' and
      // remove checks in this case.  We force a real check because in
      // the case of a final method, a SEGV will not be generated.
      // FIXME: we may need a check anyway, depending on platform.
      if (! meth->constructor_p ()
	  && (meth->get_modifiers () & ACC_PRIVATE) == 0)
	obj = builtins->check_reference (obj, true);

      args = tree_cons (NULL_TREE, obj, args);
    }
  else
    {
      // Virtual dispatch.

      assert (obj != NULL_TREE);
      obj = save_expr (obj);

      args = tree_cons (NULL_TREE, obj, args);

      aot_class *aotk = builtins->get_class (meth->get_declaring_class ());

      tree index = build_int_cst (sizetype, aotk->find_in_vtable (meth));
      index = size_binop (MULT_EXPR, index,
			  TYPE_SIZE_UNIT (type_nativecode_ptr_ptr));
      if (TARGET_VTABLE_USES_DESCRIPTORS)
	index = size_binop (MULT_EXPR, index,
			    size_int (TARGET_VTABLE_USES_DESCRIPTORS));

      // Dereference the object to find the table.  Check for a null
      // reference if needed.
      obj = builtins->check_reference (obj);

      // Find the vtable by looking for the 'vtable' field.
      tree dtable = build1 (INDIRECT_REF, type_object,
			    build1 (NOP_EXPR, type_object_ptr, obj));
      dtable = build3 (COMPONENT_REF, type_dtable_ptr,
		       dtable,
		       builtins->find_decl (type_object, "vtable"),
		       NULL_TREE);

      func = build2 (PLUS_EXPR, type_nativecode_ptr_ptr, dtable,
		     convert (type_nativecode_ptr_ptr, index));
      if (! TARGET_VTABLE_USES_DESCRIPTORS)
	func = build1 (INDIRECT_REF, type_nativecode_ptr, func);
      // Cast back to the correct type, not just 'void *'.
      func = build1 (NOP_EXPR, build_pointer_type (TREE_TYPE (meth_tree)),
		     func);
    }

  // METH_TREE is a method decl, so we need one TREE_TYPE to get the
  // method's type and one to get the method's return type.
  tree call = build3 (CALL_EXPR, TREE_TYPE (TREE_TYPE (meth_tree)),
		      func, args, NULL_TREE);
  TREE_SIDE_EFFECTS (call) = 1;

  // call = check_for_builtin (method, call);

  return call;
}

tree
cxx_abi::build_method_call (tree_builtins *builtins,
			    aot_class *current,
			    tree obj, tree args,
			    const std::string &class_name,
			    const std::string &method_name,
			    const std::string &descriptor)
{
  abort ();			// FIXME
}

tree
cxx_abi::build_field_reference (tree_builtins *builtins,
				aot_class *,
				tree obj, model_field *field)
{
  builtins->lay_out_class (field->get_declaring_class ());
  tree result = builtins->map_field (field);
  if (field->static_p ())
    {
      assert (obj == NULL_TREE);

      result
	= build2 (COMPOUND_EXPR, TREE_TYPE (result),
		  builtins->build_class_init (field->get_declaring_class ()),
		  result);
    }
  else
    {
      assert (obj != NULL_TREE);
      result = build3 (COMPONENT_REF, TREE_TYPE (result),
		       build1 (INDIRECT_REF,
			       TREE_TYPE (TREE_TYPE (obj)),
			       builtins->check_reference (obj)),
		       result,
		       NULL_TREE);
    }
  return result;
}

tree
cxx_abi::build_field_reference (tree_builtins *builtins,
				aot_class *current,
				tree obj,
				const std::string &classname,
				const std::string &fieldname,
				const std::string &descriptor)
{
  abort ();			// FIXME
}

tree
cxx_abi::build_class_reference (tree_builtins *builtins,
				aot_class *current,
				const std::string &classname)
{
  // Turn the class name into a reference to a real class in the
  // model.
  model_class *klass = current->get ();
  // We know that any call to this method necessarily comes from a
  // .class file.
  model_unit_class *unit
    = assert_cast<model_unit_class *> (klass->get_compilation_unit ());
  model_type *type = unit->find_class_from_descriptor (NULL, klass, classname);
  return build_class_reference (builtins, current, type);
}

tree
cxx_abi::build_class_reference (tree_builtins *builtins,
				aot_class *,
				model_type *klass)
{
  assert (! klass->primitive_p () && klass != primitive_void_type);
  tree class_obj
    = builtins->map_class_object (assert_cast<model_class *> (klass));
  return build1 (ADDR_EXPR, type_class_ptr, class_obj);
}

tree
cxx_abi::build_new (tree_builtins *builtins, aot_class *current,
		    model_class *klass, model_method *constructor,
		    tree arguments)
{
  tree allocator = builtin_Jv_AllocObject;  // FIXME: finalizer
  tree klass_tree = builtins->map_type (klass);
  // Allocate the object.
  tree call = build3 (CALL_EXPR, type_object_ptr, allocator,
		      build_tree_list (NULL_TREE,
				       build_class_reference (builtins,
							      current, klass)),
		      NULL_TREE);
  TREE_SIDE_EFFECTS (call) = 1;

  call = build1 (NOP_EXPR, klass_tree, call);
  TREE_SIDE_EFFECTS (call) = 1;

  tree mem = save_expr (call);

  // Call the constructor.
  tree real_call = build_method_call (builtins, current, mem, arguments,
				      constructor, false);

  // Yield the new object
  tree result = build2 (COMPOUND_EXPR, klass_tree, real_call, mem);
  TREE_SIDE_EFFECTS (result) = 1;

  return result;
}

tree
cxx_abi::get_vtable (tree_builtins *builtins, model_class *klass)
{
  // An interface doesn't have a vtable.
  if (klass->interface_p ())
    return null_pointer_node;
  return build_address_of (builtins->get_vtable_decl (klass));
}

tree
cxx_abi::get_vtable_index (aot_class *klass, model_method *method)
{
  assert (method->get_declaring_class () == klass->get ());
  if (method->static_p () || method->constructor_p ()
      || (method->get_modifiers () & ACC_PRIVATE) != 0)
    return integer_minus_one_node;
  // A special case for methods that are never virtual.
  if (method->final_p ()
      && (method->get_declaring_class ()
	  == global->get_compiler ()->java_lang_Object ()))
    return integer_minus_one_node;
  return build_int_cst (type_jint, klass->find_in_vtable (method));
}

tree
cxx_abi::get_catch_initializer (tree_builtins *builtins,
				model_class *klass)
{
  // We know a NULL argument here is always ok.
  return build_class_reference (builtins, NULL, klass);
}



tree
bc_abi::build_method_call (tree_builtins *builtins, aot_class *current,
			   tree obj, tree args,
			   model_method *meth, bool /*is_super*/)
{
  builtins->lay_out_class (meth->get_declaring_class ());
  tree meth_tree = builtins->map_method (meth);
  tree meth_ptr_type = build_pointer_type (TREE_TYPE (meth_tree));

  tree func;

  // FIXME: the tests here must be kept in sync with aotclass.  We
  // should encapsulate this info in a single place.
  if (meth->static_p ())
    {
      assert (obj == NULL_TREE);

      int slot = current->register_indirect_call (meth);
      tree atable = builtins->get_atable_decl (current->get ());

      tree atable_ref = build4 (ARRAY_REF, ptr_type_node, atable,
				build_int_cst (type_jint, slot),
				NULL_TREE, NULL_TREE);
      func = build1 (INDIRECT_REF, meth_ptr_type,
		     convert (build_pointer_type (meth_ptr_type),
			      atable_ref));
    }
  else if (meth->get_declaring_class ()->interface_p ())
    {
      assert (obj != NULL_TREE);

      obj = save_expr (obj);
      args = tree_cons (NULL_TREE, obj, args);

      int slot = 2 * current->register_interface_call (meth);
      tree itable = builtins->get_itable_decl (current->get ());

      tree interface = convert (type_class_ptr,
				build4 (ARRAY_REF, ptr_type_node, itable,
					build_int_cst (type_jint, slot - 1),
					NULL_TREE, NULL_TREE));
      tree idx = convert (type_jint,
			  build4 (ARRAY_REF, ptr_type_node, itable,
				  build_int_cst (type_jint, slot),
				  NULL_TREE, NULL_TREE));

      obj = builtins->check_reference (obj);
      tree dtable = build1 (INDIRECT_REF, type_object,
			    build1 (NOP_EXPR, type_object_ptr, obj));
      dtable = build3 (COMPONENT_REF, type_dtable_ptr,
		       dtable,
		       builtins->find_decl (type_object, "vtable"),
		       NULL_TREE);

      tree obj_class = build3 (COMPONENT_REF, type_class_ptr,
			       build1 (INDIRECT_REF, type_dtable, dtable),
			       builtins->find_decl (type_dtable, "class"),
			       NULL_TREE);

      tree lookup = tree_cons (NULL_TREE, obj_class,
			       tree_cons (NULL_TREE, interface,
					  build_tree_list (NULL_TREE, idx)));
      func = build3 (CALL_EXPR, ptr_type_node,
		     builtin_Jv_LookupInterfaceMethodIdx,
		     lookup, NULL_TREE);
      func = convert (meth_ptr_type, func);
    }
  // Note that, unlike the C++ ABI, we call final methods via the
  // vtable.  This is because a final method can be made non-final and
  // the call must still work properly.  Note however that we don't do
  // this for final methods in Object, due to an old gcj peculiarity.
  // We suspect that these methods will not be made non-final anyway.
  else if (meth->constructor_p ()
	   || (meth->get_modifiers () & ACC_PRIVATE) != 0
	   || (meth->final_p ()
	       && meth->get_declaring_class () == global->get_compiler ()->java_lang_Object ()))
    {
      assert (obj != NULL_TREE);

      int slot = current->register_indirect_call (meth);
      tree atable = builtins->get_atable_decl (current->get ());

      // For final methods we have to do an explicit check.
      // FIXME: we may need a check anyway, depending on platform.
      if (! meth->constructor_p ()
	  && (meth->get_modifiers () & ACC_PRIVATE) == 0)
	obj = builtins->check_reference (obj, true);

      args = tree_cons (NULL_TREE, obj, args);

      tree atable_ref = build4 (ARRAY_REF, ptr_type_node, atable,
				build_int_cst (type_jint, slot),
				NULL_TREE, NULL_TREE);
      func = build1 (INDIRECT_REF, meth_ptr_type,
		     convert (build_pointer_type (meth_ptr_type),
			      atable_ref));
    }
  else
    {
      // FIXME: 'super' invocations need special work.

      // Virtual dispatch.
      assert (obj != NULL_TREE);

      obj = save_expr (obj);
      args = tree_cons (NULL_TREE, obj, args);

      int slot = current->register_indirect_call (meth);

      tree otable = builtins->get_otable_decl (current->get ());
      tree index = fold (convert (sizetype,
				  build4 (ARRAY_REF, type_jint, otable,
					  build_int_cst (type_jint, slot),
					  NULL_TREE, NULL_TREE)));

      // FIXME: we could do this at link time and have the otable hold
      // a pure byte offset.
      index = size_binop (MULT_EXPR, index,
			  TYPE_SIZE_UNIT (type_nativecode_ptr_ptr));
      if (TARGET_VTABLE_USES_DESCRIPTORS)
	index = size_binop (MULT_EXPR, index,
			    size_int (TARGET_VTABLE_USES_DESCRIPTORS));

      // Dereference the object to find the table.  Check for a null
      // reference if needed.
      obj = builtins->check_reference (obj);

      // Find the vtable by looking for the 'vtable' field.
      tree dtable = build1 (INDIRECT_REF, type_object,
			    build1 (NOP_EXPR, type_object_ptr, obj));
      dtable = build3 (COMPONENT_REF, type_dtable_ptr,
		       dtable,
		       builtins->find_decl (type_object, "vtable"),
		       NULL_TREE);

      func = build2 (PLUS_EXPR, type_nativecode_ptr_ptr, dtable,
		     convert (type_nativecode_ptr_ptr, index));
      if (! TARGET_VTABLE_USES_DESCRIPTORS)
	func = build1 (INDIRECT_REF, type_nativecode_ptr, func);
      // Cast back to the correct type, not just 'void *'.
      func = build1 (NOP_EXPR, meth_ptr_type, func);
    }

  // METH_TREE is a method decl, so we need one TREE_TYPE to get the
  // method's type and one to get the method's return type.
  tree call = build3 (CALL_EXPR, TREE_TYPE (TREE_TYPE (meth_tree)),
		      func, args, NULL_TREE);
  TREE_SIDE_EFFECTS (call) = 1;

  // call = check_for_builtin (method, call);

  return call;
}

tree
bc_abi::build_method_call (tree_builtins *builtins,
			   aot_class *current,
			   tree obj, tree args,
			   const std::string &class_name,
			   const std::string &method_name,
			   const std::string &descriptor)
{
  abort ();			// FIXME
}

tree
bc_abi::build_field_reference (tree_builtins *builtins, aot_class *current,
			       tree obj, model_field *field)
{
  tree result;
  builtins->lay_out_class (field->get_declaring_class ());
  int slot = current->register_field_reference (field);
  tree field_type = builtins->map_type (field->type ());
  if (field->static_p ())
    {
      assert (obj == NULL_TREE);

      tree atable = builtins->get_atable_decl (current->get ());
      tree atable_ref = build4 (ARRAY_REF, ptr_type_node, atable,
				build_int_cst (type_jint, slot),
				NULL_TREE, NULL_TREE);

      result = build1 (INDIRECT_REF,
		       // Note we don't need ARRAY_REF, we
		       // just generate a direct reference.
		       field_type,
		       convert (build_pointer_type (field_type),
				atable_ref));
    }
  else
    {
      assert (obj != NULL_TREE);

      tree otable = builtins->get_otable_decl (current->get ());
      tree otable_ref = build4 (ARRAY_REF, type_jint, otable,
				build_int_cst (type_jint, slot),
				NULL_TREE, NULL_TREE);

      obj = builtins->check_reference (obj);
      // Generate *(TYPE *) ((char *) OBJ + OFFSET)
      result = build1 (INDIRECT_REF, field_type,
		       convert (build_pointer_type (field_type),
				build2 (PLUS_EXPR, ptr_type_node,
					convert (ptr_type_node, obj),
					otable_ref)));
    }
  return result;
}

tree
bc_abi::build_field_reference (tree_builtins *builtins,
			       aot_class *current,
			       tree obj,
			       const std::string &classname,
			       const std::string &fieldname,
			       const std::string &descriptor)
{
  abort ();			// FIXME
}

tree
bc_abi::build_class_reference (tree_builtins *builtins, aot_class *current,
			       const std::string &classname)
{
  // FIXME: handle primitive classes
  int index = current->add_class (classname);
  tree cpool = builtins->get_constant_pool_decl (current->get ());
  return convert (type_class_ptr, build4 (ARRAY_REF, ptr_type_node,
					  cpool,
					  build_int_cst (type_jint, index),
					  NULL_TREE, NULL_TREE));
}

tree
bc_abi::build_class_reference (tree_builtins *builtins, aot_class *current,
			       model_type *klass)
{
  assert (! klass->primitive_p () && klass != primitive_void_type);
  int index = current->add (assert_cast<model_class *> (klass));
  tree cpool = builtins->get_constant_pool_decl (current->get ());
  return convert (type_class_ptr, build4 (ARRAY_REF, ptr_type_node,
					  cpool,
					  build_int_cst (type_jint, index),
					  NULL_TREE, NULL_TREE));
}

tree
bc_abi::build_direct_class_reference (tree_builtins *, aot_class *current,
				      model_type *klass)
{
  assert (! klass->primitive_p () && klass != primitive_void_type);
  int index = current->add (assert_cast<model_class *> (klass));
  return build_int_cst (type_jint, index);
}

tree
bc_abi::build_new (tree_builtins *builtins, aot_class *current,
		   model_class *klass, model_method *constructor,
		   tree arguments)
{
  tree allocator = builtin_Jv_AllocObject;
  tree klass_tree = build_class_reference (builtins, current, klass);
  tree klass_type = builtins->map_type (klass);

  // Allocate the object.
  tree n = build3 (CALL_EXPR, TREE_TYPE (TREE_TYPE (allocator)), allocator,
		   build_tree_list (NULL_TREE,
				    build_class_reference (builtins, current,
							   klass)),
		   NULL_TREE);
  TREE_SIDE_EFFECTS (n) = 1;

  n = build1 (NOP_EXPR, klass_type, n);
  TREE_SIDE_EFFECTS (n) = 1;

  tree mem = save_expr (n);

  // Call the constructor.
  n = build_method_call (builtins, current, mem, arguments, constructor,
			 false);
  TREE_SIDE_EFFECTS (n) = 1;

  // Yield the new object.
  n = build2 (COMPOUND_EXPR, klass_type, n, mem);
  TREE_SIDE_EFFECTS (n) = 1;

  return n;
}

tree
bc_abi::get_catch_initializer (tree_builtins *builtins,
			       model_class *klass)
{
  return builtins->map_utf8const (klass->get_fully_qualified_name ());
}

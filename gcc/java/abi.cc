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

  tree func;
  if (meth->static_p ())
    {
      assert (obj == NULL_TREE);
      func = build_address_of (meth_tree);
    }
  else if (meth->get_declaring_class ()->interface_p ())
    {
      // FIXME: interface call.
    }
  else if (is_super || meth->final_p () || meth->constructor_p ())
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
      if (! meth->constructor_p ())
	obj = builtins->check_reference (obj, true);

      args = tree_cons (NULL_TREE, obj, args);
    }
  else
    {
      // Virtual dispatch.

      assert (obj != NULL_TREE);
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
  // FIXME: handle primitive type.
  tree class_obj
    = builtins->map_class_object (assert_cast<model_class *> (klass));
  return build1 (ADDR_EXPR, type_class_ptr, class_obj);
}

tree
cxx_abi::build_new (tree_builtins *builtins, aot_class *current,
		    tree klass, tree constructor, tree arguments)
{
  tree allocator = builtin_Jv_AllocObject;  // FIXME: finalizer
  // Allocate the object.
  tree n = build3 (CALL_EXPR, klass, allocator,
		   build_tree_list (NULL_TREE,
				    build_class_reference (builtins, current,
							   // FIXME
							   NULL)),
		   NULL_TREE);
  TREE_SIDE_EFFECTS (n) = 1;
  // Call the constructor.
  n = build3 (CALL_EXPR, klass, constructor,
	      tree_cons (NULL_TREE, n, arguments),
	      NULL_TREE);
  TREE_SIDE_EFFECTS (n) = 1;

  return n;
}

tree
cxx_abi::get_vtable (tree_builtins *builtins, model_class *klass)
{
  return build_address_of (builtins->get_vtable_decl (klass));
}

tree
cxx_abi::get_vtable_index (aot_class *klass, model_method *method)
{
  assert (method->get_declaring_class () == klass->get ());
  if (method->static_p () || method->constructor_p ())
    return integer_minus_one_node;
  return build_int_cst (type_jint, klass->find_in_vtable (method));
}



tree
bc_abi::build_method_call (tree_builtins *, aot_class *current, tree obj,
			   tree args, model_method *meth, bool is_super)
{
  return NULL_TREE;
}

tree
bc_abi::build_field_reference (tree_builtins *builtins,
			       aot_class *current,
			       tree obj, model_field *field)
{
  tree result;
  int slot = current->register_field_reference (field);
  if (field->static_p ())
    {
      assert (obj == NULL_TREE);
      // FIXME: find the class' atable and then build a reference to
      // the appropriate part of it.
      tree atable_ref = NULL_TREE;
      result = build1 (INDIRECT_REF,
		       // Note we don't need ARRAY_REF, we
		       // just generate a direct reference.
		       builtins->map_type (field->type ()),
		       // FIXME find_atable_slot must cast to the
		       // correct type!
		       atable_ref);
    }
  else
    {
      assert (obj != NULL_TREE);
      // FIXME: find the class' otable and then build a reference to
      // the appropriate part of it.
      tree otable_ref = NULL_TREE;
      // FIXME  cast OBJ to pointer to field type -- this works
      // due to structure layout rules ... ?
      result = build4 (ARRAY_REF, builtins->map_type (field->type ()),
		       obj, otable_ref, NULL_TREE, NULL_TREE);
    }
  return result;
}

tree
bc_abi::build_class_reference (tree_builtins *builtins,
			       aot_class *current,
			       const std::string &classname)
{
  // FIXME: handle primitive classes
  int index = current->add_class (classname);
  tree cpool = builtins->get_constant_pool_decl (current->get ());
  return build4 (ARRAY_REF, type_class_ptr,
		 cpool, build_int_cst (type_jint, index),
		 NULL_TREE, NULL_TREE);
}

tree
bc_abi::build_class_reference (tree_builtins *builtins,
			       aot_class *current,
			       model_type *klass)
{
  // FIXME: handle primitive classes
  int index = current->add (assert_cast<model_class *> (klass));
  tree cpool = builtins->get_constant_pool_decl (current->get ());
  return build4 (ARRAY_REF, type_class_ptr,
		 cpool, build_int_cst (type_jint, index),
		 NULL_TREE, NULL_TREE);
}

tree
bc_abi::build_new (tree_builtins *builtins, aot_class *current,
		   tree klass, tree constructor, tree arguments)
{
  abort ();
}

// ABI interface.

// Copyright (C) 2004 Free Software Foundation, Inc.
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

#include "tree/glue.hh"
#include "tree/hooks.hh"
#include "tree/builtins.hh"

gcj_abi::gcj_abi ()
{
}

gcj_abi::~gcj_abi ()
{
}



tree
cxx_abi::build_method_call (tree_builtins *builtins, tree obj,
			    tree args, model_method *meth,
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
  else if (is_super || meth->final_p () || meth->instance_initializer_p ())
    {
      // A final method, a constructor, or a super method should be
      // called directly.  A method in a final class is implicitly
      // final, and will be caught by this same condition.
      func = build_address_of (meth_tree);

      assert (obj != NULL_TREE);
      args = tree_cons (NULL_TREE, obj, args);
    }
  else
    {
      // Virtual dispatch.

      assert (obj != NULL_TREE);
      args = tree_cons (NULL_TREE, obj, args);

      tree index = DECL_VINDEX (meth_tree);
      index = size_binop (MULT_EXPR, index,
			  TYPE_SIZE_UNIT (type_nativecode_ptr_ptr));
      if (TARGET_VTABLE_USES_DESCRIPTORS)
	index = size_binop (MULT_EXPR, index,
			    size_int (TARGET_VTABLE_USES_DESCRIPTORS));

      tree dtable; // fixme
      func = build2 (PLUS_EXPR, type_nativecode_ptr_ptr, dtable,
		     convert (type_nativecode_ptr_ptr, index));
      if (TARGET_VTABLE_USES_DESCRIPTORS)
	func = build1 (NOP_EXPR, type_nativecode_ptr, func);
      else
	func = build1 (INDIRECT_REF, type_nativecode_ptr, func);
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
cxx_abi::build_field_reference (tree_builtins *builtins, tree obj,
				model_field *field)
{
  builtins->lay_out_class (field->get_declaring_class ());
  tree result = builtins->map_field (field);
  if (field->static_p ())
    {
      assert (obj == NULL_TREE);
    }
  else
    {
      assert (obj != NULL_TREE);
      result = build3 (COMPONENT_REF, TREE_TYPE (result),
		       build1 (INDIRECT_REF,
			       TREE_TYPE (TREE_TYPE (result)),
			       builtins->check_reference (result)),
		       result,
		       NULL_TREE);
    }
  return result;
}

tree
cxx_abi::build_class_reference (tree_builtins *builtins, tree klass)
{
  assert (TREE_CODE (klass) == POINTER_TYPE);
  tree decl = TYPE_STUB_DECL (TREE_TYPE (klass));
  return build1 (ADDR_EXPR, type_class_ptr, decl);
}

tree
cxx_abi::build_new (tree_builtins *builtins, tree klass, tree constructor,
		    tree arguments)
{
  tree allocator = builtin_Jv_AllocObject;  // FIXME: finalizer
  // Allocate the object.
  tree n = build3 (CALL_EXPR, klass, allocator,
		   build_tree_list (NULL_TREE,
				    build_class_reference (builtins, klass)),
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
bc_abi::build_method_call (tree_builtins *, tree obj,
			   tree args, model_method *meth, bool is_super)
{
  return NULL_TREE;
}

tree
bc_abi::build_field_reference (tree_builtins *builtins, tree obj,
			       model_field *field)
{
  tree result;
  if (field->static_p ())
    {
      assert (obj == NULL_TREE);
      tree atable_ref = builtins->find_atable_slot (field);
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
      tree otable_ref = builtins->find_otable_slot (field);
      // FIXME find_otable_slot must cast to the correct type.
      // FIXME  cast OBJ to pointer to field type -- this works
      // due to structure layout rules ... ?
      result = build4 (ARRAY_REF, builtins->map_type (field->type ()),
		       obj, otable_ref, NULL_TREE, NULL_TREE);
    }
  return result;
}

tree
bc_abi::build_class_reference (tree_builtins *, tree klass)
{
  return NULL_TREE;
}

tree
bc_abi::build_new (tree_builtins *builtins, tree klass, tree constructor,
		   tree arguments)
{
  return NULL_TREE;
}

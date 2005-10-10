// Tree generation.

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

#include "bytecode/cpool.hh"
#include "java/tree.hh"

/// This is a code generator that generates GCC 'GENERIC' trees.
/// This is a language-independent tree representation.  Currently
/// the documentation is here:
///   http://gcc.gnu.org/ml/gcc/2002-08/msg01397.html

tree_generator::tree_generator (tree_builtins *builtins, aot_class *k)
  : gcc_builtins (builtins),
    class_wrapper (k),
    current (NULL_TREE),
    this_tree (NULL_TREE),
    method (NULL),
    method_tree (NULL_TREE),
    current_block (NULL_TREE),
    temp_counter (0),
    labels (NULL),
    stack_top (0),
    stack_types (NULL),
    stack_slots (NULL),
    local_slots (NULL)
{
}

tree_generator::~tree_generator ()
{
  if (labels)
    delete[] labels;
  if (stack_types)
    delete[] stack_types;
  if (stack_slots)
    delete[] stack_slots;
  if (local_slots)
    delete[] local_slots;
}

tree
tree_generator::generate (model_method *m)
{
  m->visit (this);
  return method_tree;
}



void
tree_generator::annotate (tree val, model_element *element)
{
  location where = element->get_location ();
  if (where.get_line () >= 0)
    annotate_with_file_line (val, where.get_file (), where.get_line ());
}

tree
tree_generator::add_var (const ref_variable_decl &vardecl)
{
  tree var = gcc_builtins->map_variable (method_tree, vardecl.get ());
  assert (TREE_CHAIN (var) == NULL_TREE);
  TREE_CHAIN (var) = BLOCK_VARS (current_block);
  BLOCK_VARS (current_block) = var;
  return var;
}

tree
tree_generator::add_temporary (tree name, tree type)
{
  tree var = build_decl (VAR_DECL, name, type);
  DECL_CONTEXT (var) = method_tree;
  TREE_CHAIN (var) = BLOCK_VARS (current_block);
  BLOCK_VARS (current_block) = var;
  return var;
}

void
tree_generator::emit_type_assertion (model_type *to_type,
				     model_type *from_type)
{
}

tree
tree_generator::wrap_label (tree label_decl, model_element *request)
{
  tree result = build1 (LABEL_EXPR, void_type_node, label_decl);
  annotate (result, request);
  return result;
}

tree
tree_generator::build_label ()
{
  tree result = build0 (LABEL_DECL, NULL_TREE);
  DECL_CONTEXT (result) = current_block;
  DECL_ARTIFICIAL (result) = 1;
  return result;
}



void
tree_generator::visit_method (model_method *meth,
			      const std::list<ref_variable_decl> &params,
			      const ref_block &block)
{
  method = meth;
  gcc_builtins->lay_out_class (meth->get_declaring_class ());
  method_tree = gcc_builtins->map_method (meth);

  if (block || flag_jni)
    DECL_EXTERNAL (method_tree) = 0;

  if (block)
    {
      assert (this_tree == NULL_TREE);
      if (! meth->static_p ())
	this_tree = DECL_ARGUMENTS (method_tree);

      current_block = make_node (BLOCK);
      BLOCK_SUPERCONTEXT (current_block) = method_tree;
      DECL_INITIAL (method_tree) = current_block;

      tree statements = alloc_stmt_list ();
      tree_stmt_iterator out = tsi_start (statements);

      if (meth->static_p () && ! meth->static_initializer_p ())
	{
	  tree init
	    = gcc_builtins->build_class_init (meth->get_declaring_class ());
	  tsi_link_after (&out, init, TSI_CONTINUE_LINKING);
	}

      block->visit (this);
      tsi_link_after (&out, current, TSI_CONTINUE_LINKING);

      // Emit an explicit 'return' for GCC's sake.
      if (meth->get_return_type () == primitive_void_type
	  && block->can_complete_normally ())
	tsi_link_after (&out, build1 (RETURN_EXPR, void_type_node, NULL_TREE),
			TSI_CONTINUE_LINKING);

      current = statements;

      // Handle synchronized methods.  This isn't done for JNI
      // methods, since such synchronization is handled by the VM.
      if ((meth->get_modifiers () & ACC_SYNCHRONIZED) != 0)
	{
	  tree k;
	  if (meth->static_p ())
	    // FIXME: this is kind of wrong for new ABI code.
	    k = build_class_ref (meth->get_declaring_class (), meth);
	  else
	    k = this_tree;
	  current = wrap_synchronized (k, current);
	}

      current = build3 (BIND_EXPR, void_type_node,
			BLOCK_VARS (current_block),
			current, current_block);
    }
  else
    {
      assert (meth->native_p ());
      if (flag_jni)
	current = build_jni_stub ();
      else
	{
	  // For CNI (old ABI) just generate an external reference.
	}
    }

  DECL_SAVED_TREE (method_tree) = current;
}

// Build a stub to call a JNI method.  This code comes almost verbatim
// from the old gcj.
tree
tree_generator::build_jni_stub ()
{
  current_block = make_node (BLOCK);
  BLOCK_SUPERCONTEXT (current_block) = method_tree;
  DECL_INITIAL (method_tree) = current_block;

  DECL_ARTIFICIAL (method_tree) = 1;

  tree env_var = build_decl (VAR_DECL, get_identifier ("env"), ptr_type_node);
  DECL_CONTEXT (env_var) = method_tree;

  tree res_var = NULL_TREE;
  if (TREE_TYPE (TREE_TYPE (method_tree)) != void_type_node)
    {
      res_var = build_decl (VAR_DECL, get_identifier ("res"),
			    TREE_TYPE (TREE_TYPE (method_tree)));
      DECL_CONTEXT (res_var) = method_tree;
      TREE_CHAIN (env_var) = res_var;
    }

  tree meth_var = build_decl (VAR_DECL, get_identifier ("meth"),
			      ptr_type_node);
  TREE_STATIC (meth_var) = 1;
  TREE_PUBLIC (meth_var) = 0;
  DECL_EXTERNAL (meth_var) = 0;
  DECL_CONTEXT (meth_var) = method_tree;
  DECL_ARTIFICIAL (meth_var) = 1;
  DECL_INITIAL (meth_var) = null_pointer_node;
  TREE_USED (meth_var) = 1;
  chainon (env_var, meth_var);

  tree method_args = DECL_ARGUMENTS (method_tree);
  tree block = build_block (env_var, NULL_TREE, NULL_TREE,
			    method_args, NULL_TREE);
  TREE_SIDE_EFFECTS (block) = 1;
  TREE_TYPE (block) = TREE_TYPE (TREE_TYPE (method_tree));

  // Compute the local 'env' by calling _Jv_GetJNIEnvNewFrame.
  gcj_abi *abi = gcc_builtins->find_abi ();
  tree klass
    = abi->build_direct_class_reference (gcc_builtins, class_wrapper,
					 class_wrapper->get ());
  tree body = build2 (MODIFY_EXPR, ptr_type_node, env_var,
		      build3 (CALL_EXPR, ptr_type_node,
			      builtin_Jv_GetJNIEnvNewFrame,
			      build_tree_list (NULL_TREE, klass),
			      NULL_TREE));
  TREE_SIDE_EFFECTS (body) = 1;

  // All the arguments to this method become arguments to the
  // underlying JNI function.  If we had to wrap object arguments in a
  // special way, we would do that here.
  tree args = NULL_TREE;
  tree tem;
  int args_size = 0;
  for (tem = method_args; tem != NULL_TREE; tem = TREE_CHAIN (tem))
    {
      int arg_bits = TREE_INT_CST_LOW (TYPE_SIZE_UNIT (TREE_TYPE (tem)));
#ifdef PARM_BOUNDARY
      arg_bits = (((arg_bits + PARM_BOUNDARY - 1) / PARM_BOUNDARY)
                  * PARM_BOUNDARY);
#endif
      args_size += (arg_bits / BITS_PER_UNIT);

      args = tree_cons (NULL_TREE, tem, args);
    }
  args = nreverse (args);
  tree arg_types = TYPE_ARG_TYPES (TREE_TYPE (method_tree));

  // For a static method the second argument is the class.  For a
  // non-static method the second argument is 'this'; that is already
  // available in the argument list.
  if (method->static_p ())
    {
      args_size += int_size_in_bytes (TREE_TYPE (klass));
      args = tree_cons (NULL_TREE, klass, args);
      arg_types = tree_cons (NULL_TREE, type_object, arg_types);
    }

  // The JNIEnv structure is the first argument to the JNI function.
  args_size += int_size_in_bytes (TREE_TYPE (env_var));
  args = tree_cons (NULL_TREE, env_var, args);
  arg_types = tree_cons (NULL_TREE, ptr_type_node, arg_types);

  // We call _Jv_LookupJNIMethod to find the actual underlying
  // function pointer.  _Jv_LookupJNIMethod will throw the appropriate
  // exception if this function is not found at runtime.
  tem = build_tree_list (NULL_TREE, build_int_cst (NULL_TREE, args_size));
  tree method_sig
    = build_ref_from_constant_pool (type_utf8const_ptr,
				    class_wrapper->add_utf (method->get_descriptor ()));
  tree lookup_arg = tree_cons (NULL_TREE, method_sig, tem);

  tem = build_ref_from_constant_pool (type_utf8const_ptr,
				      class_wrapper->add_utf (method->get_name ()));
  lookup_arg
    = tree_cons (NULL_TREE, klass,
		 tree_cons (NULL_TREE, tem, lookup_arg));

  tem = build_function_type (TREE_TYPE (TREE_TYPE (method_tree)), arg_types);

#ifdef MODIFY_JNI_METHOD_CALL
  tem = MODIFY_JNI_METHOD_CALL (tem);
#endif

  tree jni_func_type = build_pointer_type (tem);

  tree jnifunc = build3 (COND_EXPR, ptr_type_node,
			 meth_var, meth_var,
			 build2 (MODIFY_EXPR, ptr_type_node, meth_var,
				 build3 (CALL_EXPR, ptr_type_node,
					 builtin_Jv_LookupJNIMethod,
					 lookup_arg, NULL_TREE)));

  // Now we make the actual JNI call via the resulting function
  // pointer.
  tree call = build3 (CALL_EXPR, TREE_TYPE (TREE_TYPE (method_tree)),
		      build1 (NOP_EXPR, jni_func_type, jnifunc),
		      args, NULL_TREE);

  // If the JNI call returned a result, capture it here.  If we had to
  // unwrap JNI object results, we would do that here.
  if (res_var != NULL_TREE)
    call = build2 (MODIFY_EXPR, TREE_TYPE (TREE_TYPE (method_tree)),
		   res_var, call);

  TREE_SIDE_EFFECTS (call) = 1;

  body = build2 (COMPOUND_EXPR, void_type_node, body, call);
  TREE_SIDE_EFFECTS (body) = 1;

  // Now free the environment we allocated.
  call = build3 (CALL_EXPR, ptr_type_node,
		 builtin_Jv_JNI_PopSystemFrame,
		 build_tree_list (NULL_TREE, env_var),
		 NULL_TREE);
  TREE_SIDE_EFFECTS (call) = 1;
  body = build2 (COMPOUND_EXPR, void_type_node, body, call);
  TREE_SIDE_EFFECTS (body) = 1;

  // Finally, do the return.
  tree res_type = void_type_node;
  if (res_var != NULL_TREE)
    {
      tree drt;
      assert (DECL_RESULT (method_tree));
      // Make sure we copy the result variable to the actual result.
      // We use the type of the DECL_RESULT because it might be
      // different from the return type of the function: it might be
      // promoted.
      drt = TREE_TYPE (DECL_RESULT (method_tree));
      if (drt != TREE_TYPE (res_var))
	res_var = build1 (CONVERT_EXPR, drt, res_var);
      res_var = build2 (MODIFY_EXPR, drt, DECL_RESULT (method_tree), res_var);
      TREE_SIDE_EFFECTS (res_var) = 1;
    }

  body = build2 (COMPOUND_EXPR, void_type_node, body,
		 build1 (RETURN_EXPR, res_type, res_var));
  TREE_SIDE_EFFECTS (body) = 1;

  tree bind = build3 (BIND_EXPR, void_type_node, BLOCK_VARS (block), 
		      body, block);
  return bind;
}



void
tree_generator::visit_assert (model_assert *element,
			      const ref_expression &first,
			      const ref_expression &second)
{
  if (! global->get_compiler ()->target_assert ())
    {
      current = build_empty_stmt ();
      return;
    }

  // Add assertion-related members.
  ref_field disabled = method->get_declaring_class ()->add_assert_members ();
  gcc_builtins->lay_out_class (disabled->get_declaring_class ());
  tree disabled_tree = gcc_builtins->map_field_ref (class_wrapper,
						    NULL_TREE,
						    disabled.get ());

  first->visit (this);
  tree first_tree = current;

  tree args = NULL_TREE;
  model_type *arg_type = NULL;
  if (second)
    {
      arg_type = second->type ();
      second->visit (this);
      args = build_tree_list (NULL_TREE, current);
    }

  model_class *errclass = global->get_compiler ()->java_lang_AssertionError ();
  gcc_builtins->lay_out_class (errclass);

  model_method *init = find_method ("<init>", errclass, arg_type,
				    primitive_void_type, element);

  tree new_tree = gcc_builtins->map_new (class_wrapper, errclass, init, args);

  // Generate:
  //   if (! $assertionsDisabled && ! FIRST) throw new AssertionError (SECOND)
  tree throw_node = build3 (CALL_EXPR, void_type_node,
			    builtin_Jv_Throw,
			    build_tree_list (NULL_TREE, new_tree),
			    NULL_TREE);
  current = build3 (COND_EXPR, void_type_node,
		    build2 (TRUTH_ANDIF_EXPR,
			    type_jboolean,
			    build1 (TRUTH_NOT_EXPR,
				    type_jboolean,
				    disabled_tree),
			    build1 (TRUTH_NOT_EXPR,
				    type_jboolean,
				    first_tree)),
		    throw_node, NULL_TREE);
  annotate (current, element);
}

tree
tree_generator::transform_list (const std::list<ref_stmt> &vals)
{
  tree result = alloc_stmt_list ();
  tree_stmt_iterator out = tsi_start (result);

  for (std::list<ref_stmt>::const_iterator i = vals.begin ();
       i != vals.end ();
       ++i)
    {
      current = NULL_TREE;
      (*i)->visit (this);
      if (current != NULL_TREE)
	tsi_link_after (&out, current, TSI_CONTINUE_LINKING);
    }
  return result;
}

void
tree_generator::visit_block (model_block *block,
			     const std::list<ref_stmt> &statements)
{
  // Create a new block for the body.
  save_tree saver (current_block, make_node (BLOCK));
  BLOCK_SUPERCONTEXT (current_block) = saver.get ();

  tree body = transform_list (statements);

  current = build3 (BIND_EXPR, void_type_node,
		    BLOCK_VARS (current_block),
		    body, current_block);
  annotate (current, block);
}

void
tree_generator::visit_break (model_break *brk,
			     const ref_stmt &target)
{
  model_stmt *real_target = target.get ();
  if (! real_target)
    real_target = brk->get_target ();
  // Find the target statement.
  target_map_type::iterator iter = target_map.find (real_target);
  assert (iter != target_map.end ());

  current = build1 (GOTO_EXPR, void_type_node,
		    (*iter).second.second);
  TREE_SIDE_EFFECTS (current) = 1;
  annotate (current, brk);
}

void
tree_generator::visit_catch (model_catch *stmt,
			     const ref_variable_decl &vardecl,
			     const ref_block &body)
{
  // Create a new block for the body.
  save_tree saver (current_block, make_node (BLOCK));
  BLOCK_SUPERCONTEXT (current_block) = saver.get ();

  // Make a new variable and link it in.
  tree var = add_var (vardecl);

  tree body_tree = alloc_stmt_list ();
  tree_stmt_iterator out = tsi_start (body_tree);

  // Create an assignment to the variable and put it in the statement
  // list.
  tree vtype = gcc_builtins->map_type (vardecl->type ());
  // FIXME: type?
  tree assign = build2 (MODIFY_EXPR, ptr_type_node,
			var, build_exception_object_ref (vtype));

  tsi_link_after (&out, assign, TSI_CONTINUE_LINKING);

  // Generate code for the body and link it in.
  body->visit (this);
  tsi_link_after (&out, current, TSI_CONTINUE_LINKING);

  tree bind = build3 (BIND_EXPR, void_type_node,
		      BLOCK_VARS (current_block),
		      body_tree, current_block);

  // It would be nice to emit this as a pointer into the constant
  // pool, but GCC apparently crashes when this is compiled with
  // -fPIC.
  model_class *vklass = assert_cast<model_class *> (vardecl->type ());
  tree decl = gcc_builtins->map_catch_class (class_wrapper->get (), vklass);

  current = build2 (CATCH_EXPR, void_type_node, build_address_of (decl), bind);
  annotate (current, stmt);
}

void
tree_generator::visit_continue (model_continue *element,
				const ref_stmt &target)
{
  // Find the target statement.
  target_map_type::iterator iter = target_map.find (target.get ());
  assert (iter != target_map.end ());
  current = build1 (GOTO_EXPR, void_type_node,
		    (*iter).second.first);
  TREE_SIDE_EFFECTS (current) = 1;
  annotate (current, element);
}

void
tree_generator::visit_class_decl_stmt (model_class_decl_stmt *,
				       const ref_class &)
{
  // Nothing to do.
}

void
tree_generator::visit_do (model_do *dstmt,
			  const ref_expression &expr,
			  const ref_stmt &body)
{
  // Some labels which we'll use later.
  tree test = build_label ();
  tree done = build_label ();
  target_map[dstmt] = std::make_pair (test, done);

  // Generate code for the expression and the body.
  expr->visit (this);
  tree expr_tree = current;
  body->visit (this);
  tree stmt_tree = current;

  tree body_tree = alloc_stmt_list ();
  tree_stmt_iterator out = tsi_start (body_tree);

  // Set up the loop body and arrange to exit once the condition has
  // changed.
  tsi_link_after (&out, stmt_tree, TSI_CONTINUE_LINKING);
  tsi_link_after (&out, wrap_label (test, expr.get ()), TSI_CONTINUE_LINKING);
  tsi_link_after (&out, build1 (EXIT_EXPR,
				void_type_node,
				fold (build1 (TRUTH_NOT_EXPR, type_jboolean,
					      expr_tree))),
		  TSI_CONTINUE_LINKING);

  // Wrap the body in a loop.
  body_tree = build1 (LOOP_EXPR, void_type_node, body_tree);
  annotate (current, dstmt);

  // Now make an outer statement list with the remaining label.
  tree outer = alloc_stmt_list ();
  out = tsi_start (outer);
  tsi_link_after (&out, body_tree, TSI_CONTINUE_LINKING);
  // FIXME: location should be end of do statement, not beginning.
  tsi_link_after (&out, wrap_label (done, dstmt), TSI_CONTINUE_LINKING);
  current = outer;
}

void
tree_generator::visit_empty (model_empty *)
{
  current = build_empty_stmt ();
}

void
tree_generator::visit_expression_stmt (model_expression_stmt *,
				       const ref_expression &expr)
{
  expr->visit (this);
}

void
tree_generator::visit_for_enhanced (model_for_enhanced *fstmt,
				    const ref_stmt &body,
				    const ref_expression &container,
				    const ref_variable_decl &var)
{
  tree update_tree = build_label ();
  tree done_tree = build_label ();
  target_map[fstmt] = std::make_pair (update_tree, done_tree);

  // Push a new block around the loop.
  save_tree saver (current_block, make_node (BLOCK));
  BLOCK_SUPERCONTEXT (current_block) = saver.get ();

  tree body_tree = alloc_stmt_list ();
  tree_stmt_iterator out = tsi_start (body_tree);

  // Generate code to find the container.
  container->visit (this);
  tree container_tree = save_expr (current);
  tree container_ptr_type = TREE_TYPE (current);

  model_type *ctype = container->type ();
  gcc_builtins->lay_out_class (assert_cast<model_class *> (ctype));

  // The user's variable.
  tree var_decl = add_var (var);

  tree exit_expr, update_expr, assign_rhs;

  if (container->type ()->array_p ())
    {
      // Make some temporaries: one to hold the index and one to hold
      // the length of the array.
      tree index_tree = add_temporary (gcc_builtins->get_symbol (), type_jint);
      tree len_tree = add_temporary (gcc_builtins->get_symbol (), type_jint);

      tree container_type = TREE_TYPE (container_ptr_type);
      tree lenfield = gcc_builtins->find_decl (container_type, "length");
      tree datafield = gcc_builtins->find_decl (container_type, "data");

      // Compute and save the length of the array.
      tree get_len
	= build3 (COMPONENT_REF, type_jint,
		  build1 (INDIRECT_REF, container_type,
			  // FIXME: there is probably a more optimal
			  // approach here.
			  gcc_builtins->check_reference (container_tree)),
		  lenfield, NULL_TREE);
      get_len = build2 (MODIFY_EXPR, type_jint, len_tree, get_len);
      TREE_SIDE_EFFECTS (get_len) = 1;
      tsi_link_after (&out, get_len, TSI_CONTINUE_LINKING);

      // Initialize the index.
      tree init = build2 (MODIFY_EXPR, type_jint, index_tree,
			  fold (convert (type_jint, integer_zero_node)));
      TREE_SIDE_EFFECTS (init) = 1;
      tsi_link_after (&out, init, TSI_CONTINUE_LINKING);

      // Code for the exit expression.
      exit_expr = build2 (EQ_EXPR, type_jboolean, index_tree, len_tree);

      // Code for the update expression.
      update_expr = build2 (PREINCREMENT_EXPR, type_jint, index_tree,
			    fold (convert (type_jint, integer_one_node)));
      TREE_SIDE_EFFECTS (update_expr) = 1;

      // Code to compute the new user variable value.
      assign_rhs = build4 (ARRAY_REF, TREE_TYPE (TREE_TYPE (datafield)),
			   build3 (COMPONENT_REF, TREE_TYPE (datafield),
				   build1 (INDIRECT_REF, container_type,
					   container_tree),
				   datafield, NULL_TREE),
			   index_tree, NULL_TREE, NULL_TREE);
    }
  else
    {
      // Use an iterator.

      model_class *iterator_type
	= global->get_compiler ()->java_util_Iterator ();
      model_class *object_type
	= global->get_compiler ()->java_lang_Object ();
      model_class *iterable_type
	= global->get_compiler ()->java_lang_Iterable ();

      model_method *iter_meth = find_method ("iterator", iterable_type, NULL,
					     iterator_type, fstmt);
      model_method *has_next_meth = find_method ("hasNext", iterator_type,
						 NULL, primitive_boolean_type,
						 fstmt);
      model_method *next_meth = find_method ("next", iterator_type, NULL,
					     object_type, fstmt);

      // Introduce a new variable that holds the iterator.
      tree iter_tree = add_temporary (gcc_builtins->get_symbol (),
				      gcc_builtins->map_type (iterator_type));

      // Initialize the iterator and link it in.
      tree init = build2 (MODIFY_EXPR, TREE_TYPE (iter_tree),
			  iter_tree,
			  gcc_builtins->map_method_call (class_wrapper,
							 container_tree,
							 NULL_TREE, iter_meth,
							 false));
      TREE_SIDE_EFFECTS (init) = 1;
      tsi_link_after (&out, init, TSI_CONTINUE_LINKING);

      // Compute the exit expression.
      exit_expr = build1 (TRUTH_NOT_EXPR, type_jboolean,
			  gcc_builtins->map_method_call (class_wrapper,
							 iter_tree, NULL_TREE,
							 has_next_meth,
							 false));
      TREE_SIDE_EFFECTS (exit_expr) = 1;

      // Code for the update expression.
      assign_rhs = gcc_builtins->map_method_call (class_wrapper,
						  iter_tree, NULL_TREE,
						  next_meth, false);
      TREE_SIDE_EFFECTS (assign_rhs) = 1;

      // We don't need a different update expression.
      update_expr = build_empty_stmt ();
    }

  // We've already linked in the initialization code.  Now start the
  // loop body.
  tree loop_tree = alloc_stmt_list ();
  tree_stmt_iterator loop_out = tsi_start (loop_tree);

  // Link in the exit expression.
  tsi_link_after (&loop_out, build1 (EXIT_EXPR, void_type_node, exit_expr),
		  TSI_CONTINUE_LINKING);
  // Link in the user variable assignment.
  tree user_assign = build2 (MODIFY_EXPR, TREE_TYPE (var_decl),
			     var_decl,
			     convert (TREE_TYPE (var_decl), assign_rhs));
  TREE_SIDE_EFFECTS (user_assign) = 1;
  tsi_link_after (&loop_out, user_assign, TSI_CONTINUE_LINKING);

  // Compute and link in the body of the loop.
  body->visit (this);
  tsi_link_after (&loop_out, current, TSI_CONTINUE_LINKING);

  // Link in the update label and the update expression.
  tsi_link_after (&loop_out, wrap_label (update_tree, fstmt),
		  TSI_CONTINUE_LINKING);
  tsi_link_after (&loop_out, update_expr, TSI_CONTINUE_LINKING);

  // Wrap up the loop body and link it into the outer statement list.
  tree loop_contents = build1 (LOOP_EXPR, void_type_node, loop_tree);
  TREE_SIDE_EFFECTS (loop_contents) = 1;
  annotate (loop_contents, fstmt);
  tsi_link_after (&out, loop_contents, TSI_CONTINUE_LINKING);

  tsi_link_after (&out, wrap_label (done_tree, fstmt), TSI_CONTINUE_LINKING);

  current = build3 (BIND_EXPR, void_type_node, BLOCK_VARS (current_block),
		    body_tree, current_block);
}

void
tree_generator::visit_for (model_for *fstmt,
			   const ref_stmt &init,
			   const ref_expression &cond,
			   const ref_stmt &update,
			   const ref_stmt &body)
{
  tree update_tree = build_label ();
  tree done_tree = build_label ();
  target_map[fstmt] = std::make_pair (update_tree, done_tree);

  save_tree saver (current_block, make_node (BLOCK));
  BLOCK_SUPERCONTEXT (current_block) = saver.get ();

  tree result = alloc_stmt_list ();
  tree_stmt_iterator result_out = tsi_start (result);

  if (init)
    {
      init->visit (this);
      tsi_link_after (&result_out, current, TSI_CONTINUE_LINKING);
    }

  // The body of the loop, including the condition and the update.
  tree body_tree = alloc_stmt_list ();
  tree_stmt_iterator body_out = tsi_start (body_tree);
  if (cond)
    {
      cond->visit (this);
      tsi_link_after (&body_out, build1 (EXIT_EXPR, void_type_node,
					 fold (build1 (TRUTH_NOT_EXPR,
						       type_jboolean,
						       current))),
		      TSI_CONTINUE_LINKING);
    }
  body->visit (this);
  tsi_link_after (&body_out, current, TSI_CONTINUE_LINKING);

  tsi_link_after (&body_out, wrap_label (update_tree, fstmt),
		  TSI_CONTINUE_LINKING);
  if (update)
    {
      update->visit (this);
      tsi_link_after (&body_out, current, TSI_CONTINUE_LINKING);
    }

  // Now wrap the body in a loop and link it into the outer statement
  // list.
  body_tree = build1 (LOOP_EXPR, void_type_node, body_tree);
  annotate (body_tree, fstmt);
  tsi_link_after (&result_out, body_tree, TSI_CONTINUE_LINKING);

  tsi_link_after (&result_out, wrap_label (done_tree, fstmt),
		  TSI_CONTINUE_LINKING);

  current = build3 (BIND_EXPR, void_type_node, BLOCK_VARS (current_block),
		    result, current_block);
}

void
tree_generator::visit_if (model_if *element,
			  const ref_expression &cond,
			  const ref_stmt &true_branch,
			  const ref_stmt &false_branch)
{
  cond->visit (this);
  tree cond_tree = current;
  true_branch->visit (this);
  tree true_tree = current;
  tree false_tree;
  if (false_branch)
    {
      false_branch->visit (this);
      false_tree = current;
    }
  else
    false_tree = build_empty_stmt ();

  current = build3 (COND_EXPR, void_type_node, cond_tree,
		    true_tree, false_tree);
  TREE_SIDE_EFFECTS (current) = (TREE_SIDE_EFFECTS (cond_tree)
				 || TREE_SIDE_EFFECTS (true_tree)
				 || TREE_SIDE_EFFECTS (false_tree));
  annotate (current, element);
}

void
tree_generator::visit_label (model_label *label, const ref_stmt &stmt)
{
  // We might not know the target of a break statement at semantic
  // analysis time, so we compute it here.
  if (label->get_break_target () == NULL)
    {
      // Note that we don't use build_label() here since this is not
      // an artificial label.
      tree brk = build0 (LABEL_DECL, NULL_TREE);
      DECL_CONTEXT (brk) = current_block;
      target_map[label] = std::make_pair (NULL_TREE, brk);

      tree body_tree = alloc_stmt_list ();
      tree_stmt_iterator out = tsi_start (body_tree);

      stmt->visit (this);
      tsi_link_after (&out, current, TSI_CONTINUE_LINKING);
      tsi_link_after (&out, wrap_label (brk, label), TSI_CONTINUE_LINKING);

      current = body_tree;
    }
  else
    stmt->visit (this);
}

void
tree_generator::visit_return (model_return *rtn,
			      const ref_expression &expr)
{
  if (expr)
    {
      expr->visit (this);
      current = build2 (MODIFY_EXPR, TREE_TYPE (DECL_RESULT (method_tree)),
			DECL_RESULT (method_tree),
			convert (TREE_TYPE (DECL_RESULT (method_tree)),
				 current));
      TREE_SIDE_EFFECTS (current) = 1;
    }
  else
    current = NULL_TREE;
  current = build1 (RETURN_EXPR, void_type_node, current);
  TREE_SIDE_EFFECTS (current) = 1;
  annotate (current, rtn);
}

void
tree_generator::visit_switch (model_switch *swstmt,
			      const ref_expression &expr,
			      const std::list<ref_switch_block> &blocks)
{
  expr->visit (this);
  tree expr_tree = current;

  tree done = build_label ();
  target_map[swstmt] = std::make_pair (NULL_TREE, done);

  tree body_tree = alloc_stmt_list ();
  tree_stmt_iterator out = tsi_start (body_tree);

  model_switch_block *def = swstmt->get_default ();
  for (std::list<ref_switch_block>::const_iterator i = blocks.begin ();
       i != blocks.end ();
       ++i)
    {
      if ((*i).get () == def)
	{
	  tree label = build_label ();

	  tree case_label = build3 (CASE_LABEL_EXPR, NULL_TREE, NULL_TREE,
				    NULL_TREE, label);
	  annotate (case_label, (*i).get ());
	  tsi_link_after (&out, case_label, TSI_CONTINUE_LINKING);
	}
      (*i)->visit (this);
      tsi_link_after (&out, current, TSI_CONTINUE_LINKING);
    }

  current = build3 (SWITCH_EXPR, TREE_TYPE (expr_tree), expr_tree,
		    body_tree, NULL_TREE);
  annotate (current, swstmt);

  body_tree = alloc_stmt_list ();
  out = tsi_start (body_tree);
  tsi_link_after (&out, current, TSI_CONTINUE_LINKING);
  tsi_link_after (&out, wrap_label (done, swstmt), TSI_CONTINUE_LINKING);
  current = body_tree;
}

void
tree_generator::visit_switch_block (model_switch_block *swblock,
				    const std::list<ref_stmt> &statements)
{
  // FIXME: redeclare primitive types so we can do this
  // without a cast.
  model_primitive_base *intb
    = assert_cast<model_primitive_base *> (primitive_int_type);

  tree body_tree = alloc_stmt_list ();
  tree_stmt_iterator out = tsi_start (body_tree);

  std::list<ref_expression> labels = swblock->get_labels ();
  for (std::list<ref_expression>::const_iterator i = labels.begin ();
       i != labels.end ();
       ++i)
    {
      tree label = build_label ();

      jint value = jint (intb->convert ((*i)->type (), (*i)->value ()));
      tree new_label = build3 (CASE_LABEL_EXPR, NULL_TREE, build_int (value),
			       NULL_TREE, label);
      annotate (new_label, swblock);
      tsi_link_after (&out, new_label, TSI_CONTINUE_LINKING);
    }

  tree stmt_tree = transform_list (statements);
  tsi_link_after (&out, stmt_tree, TSI_CONTINUE_LINKING);

  current = body_tree;
}

tree
tree_generator::wrap_synchronized (tree expr, tree body)
{
  // Make sure we only evaluate the expression once.
  expr = save_expr (expr);

  // Emit a call to enter the monitor.
  tree enter = build3 (CALL_EXPR, void_type_node,
		       builtin_Jv_MonitorEnter,
		       build_tree_list (NULL_TREE, expr),
		       NULL_TREE);
  TREE_SIDE_EFFECTS (enter) = 1;

  // Build a call to leave the monitor, we use it shortly.
  tree exit_tree = build3 (CALL_EXPR, void_type_node,
			   builtin_Jv_MonitorExit,
			   build_tree_list (NULL_TREE, expr),
			   NULL_TREE);
  TREE_SIDE_EFFECTS (exit_tree) = 1;

  tree body_tree = alloc_stmt_list ();
  tree_stmt_iterator out = tsi_start (body_tree);
  tsi_link_after (&out, enter, TSI_CONTINUE_LINKING);

  tsi_link_after (&out, body, TSI_CONTINUE_LINKING);

  return build2 (TRY_FINALLY_EXPR, NULL_TREE, body_tree, exit_tree);
}

void
tree_generator::visit_synchronized (model_synchronized *sync,
				    const ref_expression &expr,
				    const ref_stmt &body)
{
  expr->visit (this);
  tree expr_tree = current;
  body->visit (this);
  tree body_tree = current;

  current = wrap_synchronized (expr_tree, body_tree);
  annotate (current, sync);
}

void
tree_generator::visit_throw (model_throw *element,
			     const ref_expression &expr)
{
  expr->visit (this);
  tree expr_tree = current;
  current = build3 (CALL_EXPR, void_type_node,
		    builtin_Jv_Throw,
		    build_tree_list (NULL_TREE, expr_tree),
		    NULL_TREE);
  TREE_SIDE_EFFECTS (current) = 1;
  annotate (current, element);
}

void
tree_generator::visit_try (model_try *trystmt,
			   const ref_block &body,
			   const std::list<ref_catch> &catchers,
			   const ref_block &finally)
{
  // First generate trees for the body.
  body->visit (this);
  tree body_tree = current;

  assert (finally || ! catchers.empty ());

  // Now generate trees for the catch clauses, but only if there are
  // any.
  tree result;

  if (catchers.empty ())
    result = body_tree;
  else
    {
      tree catch_tree = alloc_stmt_list ();
      tree_stmt_iterator out = tsi_start (catch_tree);
      for (std::list<ref_catch>::const_iterator i = catchers.begin ();
	   i != catchers.end ();
	   ++i)
	{
	  (*i)->visit (this);
	  // It is fine to simply link in CURRENT here, since we know
	  // that each catcher will just generate a CATCH_EXPR.
	  tsi_link_after (&out, current, TSI_CONTINUE_LINKING);
	}

      // Generate the internal try-catch.
      result = build2 (TRY_CATCH_EXPR, NULL_TREE, body_tree, catch_tree);
    }

  // Generate code for 'finally' if needed.
  if (finally)
    {
      finally->visit (this);
      tree finally_tree = current;
      // In this case we need an outer try-finally, which encloses all
      // the catch clauses.
      result = build2 (TRY_FINALLY_EXPR, NULL_TREE, result, finally_tree);
    }

  current = result;
  annotate (current, trystmt);
}

void
tree_generator::visit_variable_stmt (model_variable_stmt *,
				     const std::list<ref_variable_decl> &decls)
{
  // The result here is a statement list that initializes all the
  // variables, as needed.  The statement list might be empty.
  tree result = alloc_stmt_list ();
  tree_stmt_iterator out = tsi_start (result);

  for (std::list<ref_variable_decl>::const_iterator i = decls.begin ();
       i != decls.end ();
       ++i)
    {
      tree decl = add_var (*i);
      if ((*i)->has_initializer_p ())
	{
	  ref_expression init = (*i)->get_initializer ();
	  init->visit (this);

	  // Apparently setting DECL_INITIAL is not enough for locals
	  // -- instead we must emit explicit initializations.
	  // DECL_INITIAL (decl) = current;  -- does not work.
	  tree modify = build2 (MODIFY_EXPR, TREE_TYPE (decl),
				decl, current);
	  TREE_SIDE_EFFECTS (modify) = 1;
	  tsi_link_after (&out, modify, TSI_CONTINUE_LINKING);
	}
    }

  current = result;
}

void
tree_generator::visit_while (model_while *wstmt,
			     const ref_expression &cond,
			     const ref_stmt &body)
{
  tree again = build_label ();
  tree done = build_label ();
  target_map[wstmt] = std::make_pair (again, done);

  // Create the body of the loop: first the 'again' label, then the
  // exit expression, then the actual body.
  tree body_tree = alloc_stmt_list ();
  tree_stmt_iterator out = tsi_start (body_tree);
  tsi_link_after (&out, wrap_label (again, wstmt), TSI_CONTINUE_LINKING);

  cond->visit (this);
  tsi_link_after (&out, build1 (EXIT_EXPR, void_type_node,
				fold (build1 (TRUTH_NOT_EXPR, type_jboolean,
					      current))),
		  TSI_CONTINUE_LINKING);

  body->visit (this);
  tsi_link_after (&out, current, TSI_CONTINUE_LINKING);

  // Now wrap the body in a loop, and add the "done" label.
  body_tree = build1 (LOOP_EXPR, void_type_node, body_tree);
  annotate (body_tree, wstmt);

  current = alloc_stmt_list ();
  out = tsi_start (current);
  tsi_link_after (&out, body_tree, TSI_CONTINUE_LINKING);
  tsi_link_after (&out, wrap_label (done, wstmt), TSI_CONTINUE_LINKING);
}



void
tree_generator::visit_array_initializer (model_array_initializer *initx,
					 const ref_forwarding_type &elt_type,
					 const std::list<ref_expression> &exprs)
{
  // FIXME: constant array initialization optimization... ?

  tree ind_tree = build_int_cst (type_jint, exprs.size ());
  if (elt_type->type ()->primitive_p ())
    current = build_new_array (elt_type->type (), ind_tree, initx);
  else
    current = build_new_object_array (elt_type->type (), ind_tree);

  // Ensure array class is laid out.
  gcc_builtins->lay_out_class (elt_type->type ()->array ());

  // At this point, 'current' is the 'new' expression for the array.
  tree new_expr = save_expr (current);

  // Yield 'new_expr'.
  tree result = new_expr;

  tree elt_tree = gcc_builtins->map_type (elt_type->type ());

  // Build in reverse order so that the result ends up on the right
  // hand side of the last compound expression.
  int index = exprs.size () - 1;
  for (std::list<ref_expression>::const_reverse_iterator i = exprs.rbegin ();
       i != exprs.rend ();
       ++i)
    {
      (*i)->visit (this);
      tree value = current;

      tree idx_tree = build_int_cst (type_jint, index);
      tree assign
	= build2 (MODIFY_EXPR, elt_tree,
		  build_array_reference (new_expr, idx_tree,
					 elt_tree, false),
		  value);
      TREE_SIDE_EFFECTS (assign) = 1;
      result = build2 (COMPOUND_EXPR, TREE_TYPE (result), assign, result);
      --index;
    }

  current = result;
  annotate (current, initx);
}

void
tree_generator::visit_array_ref (model_array_ref *aref,
				 const ref_expression &array,
				 const ref_expression &index)
{
  array->visit (this);
  tree array_tree = current;
  index->visit (this);
  tree index_tree = current;
  tree component_type
    = gcc_builtins->map_type (array->type ()->element_type ());
  // We need the array type laid out before we can use it.
  // FIXME: this should be handled more generically.
  gcc_builtins->lay_out_class (assert_cast<model_class *> (array->type ()));

  current = build_array_reference (array_tree, index_tree, component_type);
  annotate (current, aref);
}

void
tree_generator::binary_operator (model_element *element,
				 tree_code code,
				 const ref_expression &lhs,
				 const ref_expression &rhs,
				 tree result_type)
{
  lhs->visit (this);
  tree lhs_tree = current;
  rhs->visit (this);
  tree rhs_tree = current;
  if (result_type == NULL_TREE)
    result_type = gcc_builtins->map_type (lhs->type ());
  current = build2 (code, result_type, lhs_tree, rhs_tree);
  TREE_SIDE_EFFECTS (current) = (TREE_SIDE_EFFECTS (lhs_tree)
				 || TREE_SIDE_EFFECTS (rhs_tree));
  annotate (current, element);
}

void
tree_generator::visit_arith_binary (model_minus *elt,
				    const ref_expression &lhs,
				    const ref_expression &rhs)
{
  binary_operator (elt, MINUS_EXPR, lhs, rhs);
}

void
tree_generator::visit_arith_binary (model_mult *elt,
				    const ref_expression &lhs,
				    const ref_expression &rhs)
{
  binary_operator (elt, MULT_EXPR, lhs, rhs);
}

void
tree_generator::visit_arith_binary (model_div *op,
				    const ref_expression &lhs,
				    const ref_expression &rhs)
{
  lhs->visit (this);
  tree lhs_tree = current;
  rhs->visit (this);
  tree rhs_tree = current;

  current = build_divide (gcc_builtins->map_type (op->type ()),
			  lhs_tree, rhs_tree);
  TREE_SIDE_EFFECTS (current) = (TREE_SIDE_EFFECTS (lhs_tree)
				 | TREE_SIDE_EFFECTS (rhs_tree));
  annotate (current, op);
}

void
tree_generator::visit_arith_binary (model_mod *op,
				    const ref_expression &lhs,
				    const ref_expression &rhs)
{
  lhs->visit (this);
  tree lhs_tree = current;
  rhs->visit (this);
  tree rhs_tree = current;

  current = build_mod (gcc_builtins->map_type (op->type ()),
		       lhs_tree, rhs_tree);
  TREE_SIDE_EFFECTS (current) = (TREE_SIDE_EFFECTS (lhs_tree)
				 | TREE_SIDE_EFFECTS (rhs_tree));
  annotate (current, op);
}

void
tree_generator::visit_arith_binary (model_and *elt,
				    const ref_expression &lhs,
				    const ref_expression &rhs)
{
  binary_operator (elt, BIT_AND_EXPR, lhs, rhs);
}

void
tree_generator::visit_arith_binary (model_or *elt,
				    const ref_expression &lhs,
				    const ref_expression &rhs)
{
  binary_operator (elt, BIT_IOR_EXPR, lhs, rhs);
}

void
tree_generator::visit_arith_binary (model_xor *elt,
				    const ref_expression &lhs,
				    const ref_expression &rhs)
{
  binary_operator (elt, BIT_XOR_EXPR, lhs, rhs);
}

// FIXME: this is copied from bytecode_generator.
model_method *
tree_generator::find_method (const char *mname, model_class *klass,
			     model_type *argtype, model_type *result_type,
			     model_element *request)
{
  std::set<model_method *> methods;
  klass->find_members (mname, methods, method->get_declaring_class (), klass);
  model_method *result = NULL;
  for (std::set<model_method *>::const_iterator i = methods.begin ();
       i != methods.end ();
       ++i)
    {
      model_method *meth = *i;
      std::list<ref_variable_decl> params = meth->get_parameters ();
      int len = params.size ();
      if (! argtype && len == 0)
	{
	  result = meth;
	  break;
	}
      if (! argtype || len != 1)
	continue;
      ref_variable_decl var = params.front ();
      if (var->type () == argtype)
	{
	  result = meth;
	  break;
	}
    }

  if (! result)
    {
      throw request->error ("couldn't find method %1 with argument of type "
			    "%2 in class %3 -- perhaps you have the wrong "
			    "class library?")
	% mname % (argtype ? argtype : primitive_void_type) % klass;
    }

  if (result->get_return_type () != result_type)
    {
      throw request->error ("method %1 doesn't have expected return type"
			    " of %2")
	% result % result_type;
    }
  return result;
}

void
tree_generator::stringbuffer_append (model_expression *expr,
				     tree &buffer_tree,
				     model_class *sb_class,
				     tree expr_override)
{
  if (! expr->type ()->primitive_p () && dynamic_cast<model_plus *> (expr))
    {
      assert (! expr_override);
      // We have another String '+'.  So recurse, using the same
      // StringBuffer.  Note that it is simpler to handle this
      // recursion explicitly here than it is to do more bookkeeping
      // so we can reuse visitor.
      model_plus *plus = assert_cast<model_plus *> (expr);
      handle_string_plus (plus, plus->get_lhs (), plus->get_rhs (),
			  buffer_tree, sb_class);
    }
  else
    {
      // Generate code for the expression.
      tree expr_tree;
      if (expr_override)
	expr_tree = expr_override;
      else
	{
	  expr->visit (this);
	  expr_tree = current;
	}

      // Maybe promote the expression -- StringBuffer doesn't have
      // every possible overload.
      model_type *expr_type = expr->type ();
      if (expr_type == primitive_byte_type
	  || expr_type == primitive_short_type)
	{
	  expr_type = primitive_int_type;
	  expr_tree = convert (type_jint, expr_tree);
	}
      else if (! expr_type->primitive_p ()
	       && expr_type != global->get_compiler ()->java_lang_String ())
	{
	  expr_type = global->get_compiler ()->java_lang_Object ();
	  expr_tree = convert (type_object_ptr, expr_tree);
	}

      tree args = build_tree_list (NULL_TREE, expr_tree);

      model_method *append = find_method ("append", sb_class, expr_type,
					  sb_class, expr);
      tree ap_tree = gcc_builtins->map_method_call (class_wrapper,
						    buffer_tree, args,
						    append, false);
      buffer_tree = save_expr (ap_tree);
    }
}

void
tree_generator::handle_string_plus (model_plus *model,
				    const ref_expression &lhs,
				    const ref_expression &rhs,
				    tree &buffer_tree,
				    model_class *sb_class)
{
  stringbuffer_append (lhs.get (), buffer_tree, sb_class);
  stringbuffer_append (rhs.get (), buffer_tree, sb_class);
}

tree
tree_generator::create_stringbuffer (model_class **sb_class_r,
				     model_element *model)
{
  // Our StringBuffer is unsynchronized, but unlike StringBuilder does
  // not allocate any garbage.
  model_class *sb_class
    = global->get_compiler ()->gnu_gcj_runtime_StringBuffer ();
  gcc_builtins->lay_out_class (sb_class);

  // Create the StringBuffer.
  // FIXME: could optimize ""+foo if we wanted ...
  // FIXME: could call a different constructor if the LHS is a String.
  model_method *init = find_method ("<init>", sb_class, NULL,
				    primitive_void_type, model);

  tree buffer_tree = gcc_builtins->map_new (class_wrapper, sb_class,
					    init, NULL_TREE);
  buffer_tree = save_expr (buffer_tree);

  *sb_class_r = sb_class;
  return buffer_tree;
}

tree
tree_generator::finish_stringbuffer (model_class *sb_class,
				     tree buffer_tree,
				     model_element *model)
{
  // At this point we have a big expression to create a StringBuffer
  // and append all the contents.  So now we just convert it into a
  // String.
  model_method *tostring
    = find_method ("toString", sb_class, NULL,
		   global->get_compiler ()->java_lang_String (),
		   model);
  tree result = gcc_builtins->map_method_call (class_wrapper, buffer_tree,
					       NULL_TREE, tostring, false);
  TREE_SIDE_EFFECTS (result) = 1;
  return result;
}

void
tree_generator::visit_arith_binary (model_plus *model,
				    const ref_expression &lhs,
				    const ref_expression &rhs)
{
  if (model->type ()->primitive_p ())
    {
      binary_operator (model, PLUS_EXPR, lhs, rhs);
      return;
    }

  // String '+'.
  model_class *sb_class;
  tree buffer_tree = create_stringbuffer (&sb_class, model);
  handle_string_plus (model, lhs, rhs, buffer_tree, sb_class);
  current = finish_stringbuffer (sb_class, buffer_tree, model);
  TREE_SIDE_EFFECTS (current) = 1;
  annotate (current, model);
}

tree
tree_generator::arith_shift (model_element *element,
			     tree_code kind,
			     const ref_expression &lhs,
			     const ref_expression &rhs,
			     bool is_unsigned)
{
  lhs->visit (this);
  tree lhs_tree = current;
  tree saved_lhs_type = TREE_TYPE (lhs_tree);
  rhs->visit (this);
  tree rhs_tree = current;

  // Convert right hand side to 'int' if required.
  if (rhs->type () == primitive_long_type)
    {
      tree conv = build1 (CONVERT_EXPR, type_jint, rhs_tree);
      TREE_SIDE_EFFECTS (conv) = TREE_SIDE_EFFECTS (rhs_tree);
      rhs_tree = conv;
    }

  if (is_unsigned)
    {
      model_type *lt = lhs->type ();
      tree utype;
      if (lt == primitive_int_type)
	utype = type_juint;
      else
	{
	  assert (lt == primitive_long_type);
	  utype = type_julong;
	}
      tree conv = build1 (CONVERT_EXPR, utype, lhs_tree);
      TREE_SIDE_EFFECTS (conv) = TREE_SIDE_EFFECTS (lhs_tree);
      lhs_tree = conv;
    }

  // Mask off relevant bits of shift count.
  model_type *t = lhs->type ();
  int bits = t == primitive_long_type ? 0x3f : 0x1f;
  tree folded = fold (build2 (BIT_AND_EXPR, type_jint,
			      rhs_tree, build_int (bits)));
  TREE_SIDE_EFFECTS (folded) = TREE_SIDE_EFFECTS (rhs_tree);
  rhs_tree = folded;

  tree result = build2 (kind, TREE_TYPE (lhs_tree), lhs_tree, rhs_tree);
  TREE_SIDE_EFFECTS (result) = (TREE_SIDE_EFFECTS (lhs_tree)
				| TREE_SIDE_EFFECTS (rhs_tree));
  annotate (result, element);

  if (is_unsigned)
    {
      tree conv = build1 (CONVERT_EXPR, saved_lhs_type, result);
      TREE_SIDE_EFFECTS (conv) = TREE_SIDE_EFFECTS (result);
      result = conv;
    }

  return result;
}

void
tree_generator::visit_arith_shift (model_left_shift *elt,
				   const ref_expression &lhs,
				   const ref_expression &rhs)
{
  current = arith_shift (elt, LSHIFT_EXPR, lhs, rhs, false);
}

void
tree_generator::visit_arith_shift (model_right_shift *elt,
				   const ref_expression &lhs,
				   const ref_expression &rhs)
{
  current = arith_shift (elt, RSHIFT_EXPR, lhs, rhs, false);
}

void
tree_generator::visit_arith_shift (model_unsigned_right_shift *elt,
				   const ref_expression &lhs,
				   const ref_expression &rhs)
{
  current = arith_shift (elt, RSHIFT_EXPR, lhs, rhs, true);
}

void
tree_generator::visit_assignment (model_assignment *elt,
				  const ref_expression &lhs,
				  const ref_expression &rhs)
{
  lhs->visit (this);
  tree lhs_tree = current;
  rhs->visit (this);
  tree rhs_tree = current;
  // FIXME: if LHS is array, may need assign. check.
  current = build2 (MODIFY_EXPR, TREE_TYPE (lhs_tree),
		    lhs_tree, convert (TREE_TYPE (lhs_tree), rhs_tree));
  TREE_SIDE_EFFECTS (current) = 1;
  annotate (current, elt);
}

void
tree_generator::handle_op_assignment (model_element *element,
				      tree_code op,
				      const ref_expression &lhs,
				      const ref_expression &rhs)
{
  lhs->visit (this);
  tree lhs_tree = save_expr (current);
  rhs->visit (this);
  tree rhs_tree = current;

  bool is_shift = (op == LSHIFT_EXPR || op == RSHIFT_EXPR);

  // Note that we convert the LHS to the type of the RHS, because the
  // RHS might have been promoted.  However, we don't do this for
  // shift operators, as they don't undergo binary numeric promotion.
  // FIXME: probably the model should be more clear on this point.
  tree rhs_type = TREE_TYPE (rhs_tree);
  tree lhs_dup_tree = lhs_tree;
  if (! is_shift)
    lhs_dup_tree = fold (convert (rhs_type, lhs_dup_tree));

  // The 'mod' operation requires a special case as it may expand to a
  // builtin or a function call.
  tree operation;
  if (op == TRUNC_MOD_EXPR)
    operation = build_mod (rhs_type, lhs_dup_tree, rhs_tree);
  else
    operation = build2 (op, rhs_type, lhs_dup_tree, rhs_tree);
  TREE_SIDE_EFFECTS (operation) = (TREE_SIDE_EFFECTS (lhs_tree)
				   | TREE_SIDE_EFFECTS (rhs_tree));

  current = build2 (MODIFY_EXPR, TREE_TYPE (lhs_tree), lhs_tree,
		    fold (convert (TREE_TYPE (lhs_tree), operation)));
  TREE_SIDE_EFFECTS (current) = 1;
  annotate (current, element);

  model_type *lhs_mtype = lhs->type ();
  model_type *rhs_mtype = rhs->type ();
  if (! lhs_mtype->primitive_p () && lhs_mtype != null_type
      && ! rhs_mtype->primitive_p () && rhs_mtype != null_type)
    emit_type_assertion (lhs_mtype, rhs_mtype);
}

void
tree_generator::visit_op_assignment (model_minus_equal *elt,
				     const ref_expression &lhs,
				     const ref_expression &rhs)
{
  handle_op_assignment (elt, MINUS_EXPR, lhs, rhs);
}

void
tree_generator::visit_op_assignment (model_mult_equal *elt,
				     const ref_expression &lhs,
				     const ref_expression &rhs)
{
  handle_op_assignment (elt, MULT_EXPR, lhs, rhs);
}

void
tree_generator::visit_op_assignment (model_div_equal *op,
				     const ref_expression &lhs,
				     const ref_expression &rhs)
{
  lhs->visit (this);
  tree lhs_tree = save_expr (current);
  rhs->visit (this);
  tree rhs_tree = current;

  tree div_type = gcc_builtins->map_type (op->type ());

  current = build2 (MODIFY_EXPR, gcc_builtins->map_type (lhs->type ()),
		    lhs_tree,
		    build_divide (div_type, lhs_tree,
				  rhs_tree));
  TREE_SIDE_EFFECTS (current) = 1;
  annotate (current, op);
}

void
tree_generator::visit_op_assignment (model_and_equal *elt,
				     const ref_expression &lhs,
				     const ref_expression &rhs)
{
  handle_op_assignment (elt, BIT_AND_EXPR, lhs, rhs);
}

void
tree_generator::visit_op_assignment (model_or_equal *elt,
				     const ref_expression &lhs,
				     const ref_expression &rhs)
{
  handle_op_assignment (elt, BIT_IOR_EXPR, lhs, rhs);
}

void
tree_generator::visit_op_assignment (model_xor_equal *elt,
				     const ref_expression &lhs,
				     const ref_expression &rhs)
{
  handle_op_assignment (elt, BIT_XOR_EXPR, lhs, rhs);
}

void
tree_generator::visit_op_assignment (model_mod_equal *op,
				     const ref_expression &lhs,
				     const ref_expression &rhs)
{
  handle_op_assignment (op, TRUNC_MOD_EXPR, lhs, rhs);
}

void
tree_generator::visit_op_assignment (model_ls_equal *op,
				     const ref_expression &lhs,
				     const ref_expression &rhs)
{
  handle_op_assignment (op, LSHIFT_EXPR, lhs, rhs);
}

void
tree_generator::visit_op_assignment (model_rs_equal *op,
				     const ref_expression &lhs,
				     const ref_expression &rhs)
{
  handle_op_assignment (op, RSHIFT_EXPR, lhs, rhs);
}

void
tree_generator::visit_op_assignment (model_urs_equal *op,
				     const ref_expression &lhs,
				     const ref_expression &rhs)
{
  lhs->visit (this);
  tree lhs_tree = save_expr (current);

  tree utype;
  if (lhs->type () == primitive_int_type)
    utype = type_juint;
  else
    {
      assert (lhs->type () == primitive_long_type);
      utype = type_julong;
    }

  tree ulhs_tree = build1 (NOP_EXPR, utype, lhs_tree);

  rhs->visit (this);
  tree rhs_tree = current;

  current = build2 (MODIFY_EXPR, gcc_builtins->map_type (lhs->type ()),
		    lhs_tree,
		    build2 (RSHIFT_EXPR,
			    gcc_builtins->map_type (lhs->type ()),
			    ulhs_tree, rhs_tree));
  TREE_SIDE_EFFECTS (current) = 1;
  annotate (current, op);
}

void
tree_generator::visit_op_assignment (model_plus_equal *elt,
				     const ref_expression &lhs,
				     const ref_expression &rhs)
{
  if (elt->type ()->primitive_p ())
    {
      handle_op_assignment (elt, PLUS_EXPR, lhs, rhs);
      return;
    }

  // String '+='.
  model_class *sb_class;
  tree buffer_tree = create_stringbuffer (&sb_class, elt);

  // Wrap the LHS in a SAVE_EXPR so we only evaluate it once.
  lhs->visit (this);
  tree lhs_tree = save_expr (current);

  // Add the LHS and RHS to the StringBuffer.
  stringbuffer_append (lhs.get (), buffer_tree, sb_class, lhs_tree);
  stringbuffer_append (rhs.get (), buffer_tree, sb_class);

  tree result = finish_stringbuffer (sb_class, buffer_tree, elt);

  // Note that the LHS might not have String type.  So, we make sure
  // to cast everything to the actual type.
  current = build2 (MODIFY_EXPR, TREE_TYPE (lhs_tree), lhs_tree,
		    convert (TREE_TYPE (lhs_tree), result));
  TREE_SIDE_EFFECTS (current) = 1;
  annotate (current, elt);
}

void
tree_generator::visit_cast (model_cast *elt,
			    const ref_forwarding_type &dest,
			    const ref_expression &expr)
{
  expr->visit (this);
  tree expr_tree = current;

  model_type *dest_type = dest->type ();
  if (dest_type->primitive_p () != expr->type ()->primitive_p ())
    {
      if (dest_type->primitive_p ())
	{
	  // Unboxing conversion.  Call <type>Value() on the wrapper
	  // object, e.g. for Integer we call intValue().  Using
	  // get_pretty_name here is a bit of an abuse.  Note that
	  // Character doesn't have all the methods from Number, so we
	  // need a special case here.
	  // FIXME: for the C++ ABI we could reference fields directly
	  // in some situations.
	  model_type *tmp_dest_type = dest_type;
	  if (expr->type () == global->get_compiler ()->java_lang_Character ())
	    tmp_dest_type = primitive_char_type;
	  std::string method_name = (tmp_dest_type->get_pretty_name ()
				     + "Value");
	  model_method *call
	    = find_method (method_name.c_str (),
			   assert_cast<model_class *> (expr->type ()),
			   NULL, tmp_dest_type, elt);
	  current = gcc_builtins->map_method_call (class_wrapper, expr_tree,
						   NULL_TREE, call, false);
	  if (tmp_dest_type != dest_type)
	    {
	      assert (tmp_dest_type == primitive_char_type);
	      current = convert (gcc_builtins->map_type (dest_type), current);
	    }
	}
      else
	{
	  // Boxing conversion.  We call the static factory method
	  // valueOf(), which handles the caching required by boxing
	  // conversion.
	  model_class *dest_class = assert_cast<model_class *> (dest_type);
	  model_method *call = find_method ("valueOf", dest_class,
					    expr->type (), dest_class,
					    elt);
	  tree args = build_tree_list (NULL_TREE, expr_tree);
	  current = gcc_builtins->map_method_call (class_wrapper, NULL_TREE,
						   args, call, false);
	  TREE_SIDE_EFFECTS (current) = 1;
	}
    }
  else if (dest_type->primitive_p ())
    {
      // We can't use fold_convert() here since, apparently, it can't
      // convert a float to an int.
      current = convert (gcc_builtins->map_type (dest_type), expr_tree);
      TREE_SIDE_EFFECTS (current) = TREE_SIDE_EFFECTS (expr_tree);
    }
  else
    {
      // Reference types.  We only need to emit a cast check if the
      // types are known to be incompatible.
      if (! dest_type->assignable_from_p (expr->type ()))
	{
	  current = build3 (CALL_EXPR, gcc_builtins->map_type (dest_type),
			    builtin_Jv_CheckCast,
			    tree_cons (NULL_TREE,
				       build_class_ref (dest_type, elt),
				       build_tree_list (NULL_TREE, expr_tree)),
			    NULL_TREE);
	  TREE_SIDE_EFFECTS (current) = 1;
	}
      else
	{
	  emit_type_assertion (dest_type, expr->type ());
	  current = build1 (NOP_EXPR, gcc_builtins->map_type (dest_type),
			    expr_tree);
	  TREE_SIDE_EFFECTS (current) = TREE_SIDE_EFFECTS (expr_tree);
	}
    }
  // FIXME: might have a constant or DECL here.
  // annotate (current, elt);
}

void
tree_generator::visit_class_ref (model_class_ref *ref,
				 const ref_forwarding_type &req)
{
  current = build_class_ref (req->type (), ref);
  annotate (current, ref);
}

void
tree_generator::visit_comparison (model_equal *elt,
				  const ref_expression &lhs,
				  const ref_expression &rhs)
{
  binary_operator (elt, EQ_EXPR, lhs, rhs, type_jboolean);
}

void
tree_generator::visit_comparison (model_notequal *elt,
				  const ref_expression &lhs,
				  const ref_expression &rhs)
{
  binary_operator (elt, NE_EXPR, lhs, rhs, type_jboolean);
}

void
tree_generator::visit_comparison (model_lessthan *elt,
				  const ref_expression &lhs,
				  const ref_expression &rhs)
{
  binary_operator (elt, LT_EXPR, lhs, rhs, type_jboolean);
}

void
tree_generator::visit_comparison (model_greaterthan *elt,
				  const ref_expression &lhs,
				  const ref_expression &rhs)
{
  binary_operator (elt, GT_EXPR, lhs, rhs, type_jboolean);
}

void
tree_generator::visit_comparison (model_lessthanequal *elt,
				  const ref_expression &lhs,
				  const ref_expression &rhs)
{
  binary_operator (elt, LE_EXPR, lhs, rhs, type_jboolean);
}

void
tree_generator::visit_comparison (model_greaterthanequal *elt,
				  const ref_expression &lhs,
				  const ref_expression &rhs)
{
  binary_operator (elt, GE_EXPR, lhs, rhs, type_jboolean);
}

void
tree_generator::visit_conditional (model_conditional *m_cond,
				   const ref_expression &cond,
				   const ref_expression &true_branch,
				   const ref_expression &false_branch)
{
  cond->visit (this);
  tree cond_tree = current;
  true_branch->visit (this);
  tree true_tree = current;
  false_branch->visit (this);
  tree false_tree = current;

  current = build3 (COND_EXPR, gcc_builtins->map_type (m_cond->type ()),
		    cond_tree, true_tree, false_tree);
  TREE_SIDE_EFFECTS (current) = (TREE_SIDE_EFFECTS (cond_tree)
				 || TREE_SIDE_EFFECTS (true_tree)
				 || TREE_SIDE_EFFECTS (false_tree));
  annotate (current, m_cond);
}

void
tree_generator::visit_field_ref (model_field_ref *elt,
				 const ref_expression &expr,
				 model_field *field)
{
  bool should_inline = field->inlineable_p ();

  // Note that we don't need any special handling for 'array.length'
  // -- the generic code here works fine.
  tree expr_tree = NULL_TREE;
  if (expr)
    {
      expr->visit (this);
      expr_tree = current;

      // A very obscure case: if we have 'foo.bar', and bar is
      // inlineable, we must inline it but we must also null-check
      // 'foo'.
      if (should_inline && ! field->static_p ())
	expr_tree = gcc_builtins->check_reference (expr_tree, true);
    }

  if (should_inline)
    {
      model_expression *init = field->get_initializer ().get ();
      // This assertion should be true due to constant folding when
      // resolving the field.
      assert (dynamic_cast<model_literal_base *> (init));
      init->visit (this);

      if (expr_tree != NULL_TREE)
	current = build2 (COMPOUND_EXPR, TREE_TYPE (current),
			  expr_tree, current);

      // FIXME: should annotate() here if possible.
    }
  else
    {
      // FIXME: Note that map_field_ref does not handle the case of a
      // non-static reference to a static field.

#if 0
      // FIXME: where should this go?  [ In the ABI ]
      if (expr->type () != field->get_declaring_class ())
	// FIXME: is this right?
	emit_type_assertion (field->get_declaring_class (), expr->type ());
#endif

      gcc_builtins->lay_out_class (field->get_declaring_class ());
      current = gcc_builtins->map_field_ref (class_wrapper, expr_tree, field);
      annotate (current, elt);
    }
}

void
tree_generator::visit_field_initializer (model_field_initializer *elt,
					 model_field *field)
{
  if ((field->static_p () && field->constant_p ())
      || ! field->has_initializer_p ())
    {
      // Ignore constant fields.
      return;
    }

  // FIXME: perhaps ABI should lay out class.
  gcc_builtins->lay_out_class (field->get_declaring_class ());
  tree field_tree = gcc_builtins->map_field_ref (class_wrapper, this_tree,
						 field);

  field->get_initializer ()->visit (this);
  current = build2 (MODIFY_EXPR, TREE_TYPE (field_tree), field_tree, current);
  annotate (current, elt);
}

tree
tree_generator::handle_instanceof (tree expr_tree, tree class_tree)
{
  tree call = build3 (CALL_EXPR, type_jboolean,
		      builtin_Jv_IsInstanceOf,
		      tree_cons (NULL_TREE, expr_tree,
				 build_tree_list (NULL_TREE, class_tree)),
		      NULL_TREE);
  TREE_SIDE_EFFECTS (call) = 1;

  // It is better to have an explicit null check here, as this enables
  // VRP-based optimizations.
  tree result = build2 (TRUTH_AND_EXPR, type_jboolean,
			build2 (NE_EXPR, type_jboolean,
				expr_tree, null_pointer_node),
			call);
  TREE_SIDE_EFFECTS (result) = 1;

  return result;
}

void
tree_generator::visit_instanceof (model_instanceof *stmt,
				  const ref_expression &expr,
				  const ref_forwarding_type &klass)
{
  expr->visit (this);
  tree expr_tree = save_expr (current);
  tree class_tree = build_class_ref (klass->type (), stmt);
  current = handle_instanceof (expr_tree, class_tree);
  annotate (current, stmt);
}

void
tree_generator::visit_logical_binary (model_lor *elt,
				      const ref_expression &lhs,
				      const ref_expression &rhs)
{
  binary_operator (elt, TRUTH_ORIF_EXPR, lhs, rhs);
}

void
tree_generator::visit_logical_binary (model_land *elt,
				      const ref_expression &lhs,
				      const ref_expression &rhs)
{
  binary_operator (elt, TRUTH_ANDIF_EXPR, lhs, rhs);
}

void
tree_generator::visit_simple_literal (model_literal_base *,
				      const jboolean &val)
{
  current = val ? boolean_true_node : boolean_false_node;
}

void
tree_generator::visit_simple_literal (model_literal_base *, const jbyte &val)
{
  current = build_int (val, type_jbyte);
}

void
tree_generator::visit_simple_literal (model_literal_base *, const jchar &val)
{
  current = build_int (val, type_jchar);
}

void
tree_generator::visit_simple_literal (model_literal_base *, const jshort &val)
{
  current = build_int (val, type_jshort);
}

void
tree_generator::visit_simple_literal (model_literal_base *, const jint &val)
{
  current = build_int (val);
}

void
tree_generator::visit_simple_literal (model_literal_base *, const jlong &val)
{
  current = build_long (val);
}

tree
tree_generator::handle_float (jfloat val)
{
  jint w = float_to_word (val);
  long wl = w;
  REAL_VALUE_TYPE f;
  real_from_target_fmt (&f, &wl, &ieee_single_format);
  return build_real (type_jfloat, f);
}

void
tree_generator::visit_simple_literal (model_literal_base *, const jfloat &val)
{
  current = handle_float (val);
}

tree
tree_generator::handle_double (jdouble val)
{
  jint w[2];
  double_to_words (w, val);
  long wl[2];
  wl[0] = w[0];
  wl[1] = w[1];
  REAL_VALUE_TYPE d;
  real_from_target_fmt (&d, wl, &ieee_double_format);
  return build_real (type_jdouble, d);
}

void
tree_generator::visit_simple_literal (model_literal_base *, const jdouble &val)
{
  current = handle_double (val);
}

tree
tree_generator::handle_string_literal (const std::string &val)
{
  int location = class_wrapper->add (val);
  tree type
    = gcc_builtins->map_type (global->get_compiler ()->java_lang_String ());
  return build_ref_from_constant_pool (type, location);
}

void
tree_generator::visit_string_literal (model_string_literal *,
				      const std::string &val)
{
  current = handle_string_literal (val);
}

void
tree_generator::handle_invocation (const model_method *meth,
				   tree this_expr_tree,
				   const std::list<ref_expression> &args,
				   bool is_super)
{
  tree arg_tree = NULL_TREE;
  for (std::list<ref_expression>::const_iterator i = args.begin ();
       i != args.end ();
       ++i)
    {
      (*i)->visit (this);
      arg_tree = tree_cons (NULL_TREE, current, arg_tree);
    }
  arg_tree = nreverse (arg_tree);

  // Handle the reference-checking part of a non-static reference to a
  // static method here.  The rest is handled by map_method_call.
#if 0
  // FIXME: wrong!  we must evaluate for side effects, not nullity.
  if (this_expr_tree != NULL_TREE
      && ! meth->static_p ()
      && this_expr_tree != this_tree)
    this_expr_tree = gcc_builtins->check_reference (this_expr_tree);
#endif

  current = gcc_builtins->map_method_call (class_wrapper,
					   this_expr_tree, arg_tree,
					   const_cast<model_method *> (meth),
					   is_super);
}

void
tree_generator::visit_method_invocation (model_method_invocation *elt,
					 model_method *meth,
					 const ref_expression &this_expr,
					 const std::list<ref_expression> &args)
{
  tree expr_tree = NULL_TREE;
  tree side_effect = NULL_TREE;
  if (this_expr)
    {
      this_expr->visit (this);
      if (! meth->static_p ())
	expr_tree = current;
      else
	side_effect = current;
    }
  handle_invocation (meth, expr_tree, args);
  annotate (current, elt);

  // Handle the case where we evaluate the expression for side effects
  // but discard its value.
  if (side_effect != NULL_TREE)
    {
      current = build2 (COMPOUND_EXPR, TREE_TYPE (current),
			side_effect, current);
      TREE_SIDE_EFFECTS (current) = 1;
    }
}

void
tree_generator::visit_type_qualified_invocation
    (model_type_qualified_invocation *elt,
     const model_method *meth,
     const std::list<ref_expression> &args,
     bool super)
{
  handle_invocation (meth, meth->static_p () ? NULL_TREE : this_tree, args,
		     super);
  annotate (current, elt);
}

void
tree_generator::visit_super_invocation (model_super_invocation *elt,
					const model_method *meth,
					const std::list<ref_expression> &args)
{
  handle_invocation (meth, this_tree, args, true);
  annotate (current, elt);
}

void
tree_generator::visit_this_invocation (model_this_invocation *elt,
				       const model_method *meth,
				       const std::list<ref_expression> &args)
{
  handle_invocation (meth, this_tree, args);
  annotate (current, elt);
}

void
tree_generator::visit_new (model_new *elt,
			   const model_method *constructor,
			   const ref_forwarding_type &klass,
			   const std::list<ref_expression> &args)
{
  tree arg_tree = NULL_TREE;
  for (std::list<ref_expression>::const_iterator i = args.begin ();
       i != args.end ();
       ++i)
    {
      (*i)->visit (this);
      arg_tree = tree_cons (NULL_TREE, current, arg_tree);
    }
  arg_tree = nreverse (arg_tree);

  // FIXME: layout should be done by the ABI, not us.
  model_class *klassp = assert_cast<model_class *> (klass->type ());
  gcc_builtins->lay_out_class (klassp);
  current
    = gcc_builtins->map_new (class_wrapper, klassp,
			     const_cast<model_method *>(constructor),
			     arg_tree);
  annotate (current, elt);
}

void
tree_generator::visit_new_array (model_new_array *new_elt,
				 const ref_forwarding_type &elt_type,
				 const std::list<ref_expression> &indices,
				 const ref_expression &init)
{
  if (indices.size () == 1)
    {
      ref_expression ind = indices.front ();
      ind->visit (this);

      tree ind_tree = current;
      if (elt_type->type ()->primitive_p ())
	current = build_new_array (elt_type->type (), ind_tree, new_elt);
      else
	current = build_new_object_array (elt_type->type (), ind_tree);
      annotate (current, new_elt);
    }
  else if (indices.size () != 0)
    {
      tree args = NULL_TREE;
      for (std::list<ref_expression>::const_iterator i = indices.begin ();
	   i != indices.end ();
	   ++i)
	{
	  (*i)->visit (this);
	  args = tree_cons (NULL_TREE, current, args);
	}

      tree array_type_tree = gcc_builtins->map_type (new_elt->type ());
      current = build3 (CALL_EXPR, array_type_tree,
			builtin_Jv_NewMultiArray,
			nreverse (args), NULL_TREE);
      annotate (current, new_elt);
    }
  else
    {
      assert (init);
      init->visit (this);
    }
}

void
tree_generator::visit_null_literal (model_null_literal *)
{
  current = null_pointer_node;
}

void
tree_generator::handle_inc_dec (model_element *element, tree_code op,
				const ref_expression &expr)
{
  expr->visit (this);
  tree expr_tree = current;
  tree expr_type = gcc_builtins->map_type (expr->type ());
  current = build2 (op, expr_type, expr_tree,
		    fold (convert (expr_type, integer_one_node)));
  TREE_SIDE_EFFECTS (current) = 1;
  annotate (current, element);
}

void
tree_generator::visit_prefix_side_effect (model_prefix_plusplus *elt,
					  const ref_expression &expr)
{
  handle_inc_dec (elt, PREINCREMENT_EXPR, expr);
}

void
tree_generator::visit_prefix_side_effect (model_prefix_minusminus *elt,
					  const ref_expression &expr)
{
  handle_inc_dec (elt, PREDECREMENT_EXPR, expr);
}

void
tree_generator::visit_prefix_simple (model_prefix_plus *,
				     const ref_expression &expr)
{
  // Nothing special to do here.
  expr->visit (this);
}

void
tree_generator::visit_prefix_simple (model_prefix_minus *elt,
				     const ref_expression &expr)
{
  expr->visit (this);
  tree hold = current;
  current = build1 (NEGATE_EXPR, gcc_builtins->map_type (expr->type ()), hold);
  TREE_SIDE_EFFECTS (current) = TREE_SIDE_EFFECTS (hold);
  annotate (current, elt);
}

void
tree_generator::visit_prefix_simple (model_bitwise_not *elt,
				     const ref_expression &expr)
{
  expr->visit (this);
  tree hold = current;
  current = build1 (BIT_NOT_EXPR, gcc_builtins->map_type (expr->type ()),
		    hold);
  TREE_SIDE_EFFECTS (current) = TREE_SIDE_EFFECTS (hold);
  annotate (current, elt);
}

void
tree_generator::visit_prefix_simple (model_logical_not *elt,
				     const ref_expression &expr)
{
  expr->visit (this);
  tree hold = current;
  current = build1 (TRUTH_NOT_EXPR, type_jboolean, hold);
  TREE_SIDE_EFFECTS (current) = TREE_SIDE_EFFECTS (hold);
  annotate (current, elt);
}

void
tree_generator::visit_postfix_side_effect (model_postfix_plusplus *elt,
					   const ref_expression &expr)
{
  handle_inc_dec (elt, POSTINCREMENT_EXPR, expr);
}

void
tree_generator::visit_postfix_side_effect (model_postfix_minusminus *elt,
					   const ref_expression &expr)
{
  handle_inc_dec (elt, POSTDECREMENT_EXPR, expr);
}

void
tree_generator::visit_this (model_this *)
{
  assert (this_tree != NULL_TREE);
  current = this_tree;
}

void
tree_generator::visit_simple_variable_ref (model_simple_variable_ref *elt,
					   const model_variable_decl *decl)
{
  current
    = gcc_builtins->map_variable (method_tree,
				  const_cast<model_variable_decl *> (decl));
  // We don't have to set TREE_SIDE_EFFECTS here, as a variable cannot
  // be volatile and this would be handled by map_variable anyway.
}

tree
tree_generator::build_int (jint val, tree override)
{
  return build_int_cst (override ? override : type_jint, val);
}

tree
tree_generator::build_long (jlong val)
{
  // FIXME: what if HOST_WIDE_INT is smaller than jlong?
  return build_int_cst (type_jlong, val);
}

tree
tree_generator::build_new_array (model_type *elt_type, tree size,
				 model_element *request)
{
  assert (elt_type->primitive_p ());
  model_type *array_type = elt_type->array ();

  tree array_type_tree = gcc_builtins->map_type (array_type);

  tree insn = build3 (CALL_EXPR, ptr_type_node,
		      builtin_Jv_NewPrimArray,
		      tree_cons (NULL_TREE,
				 build_class_ref (elt_type, request),
				 tree_cons (NULL_TREE, size, NULL_TREE)),
		      NULL_TREE);
  TREE_SIDE_EFFECTS (insn) = 1;

  // Now cast to correct type.
  insn = build1 (NOP_EXPR, array_type_tree, insn);
  TREE_SIDE_EFFECTS (insn) = 1;

  return insn;
}

tree
tree_generator::build_new_object_array (model_type *elt_type, tree size)
{
  model_class *array_type = assert_cast<model_class *> (elt_type->array ());

  tree elt_type_tree = build_class_ref (array_type, elt_type);
  tree array_type_tree = gcc_builtins->lay_out_class (array_type);

  tree args = tree_cons (NULL_TREE, size,
			 tree_cons (NULL_TREE, elt_type_tree,
				    tree_cons (NULL_TREE, null_pointer_node,
					       NULL_TREE)));

  tree insn = build3 (CALL_EXPR, array_type_tree, builtin_Jv_NewObjectArray,
		      args, NULL_TREE);
  TREE_SIDE_EFFECTS (insn) = 1;

  return insn;
}

tree
tree_generator::build_ref_from_constant_pool (tree type, int index)
{
  tree cpool = gcc_builtins->get_constant_pool_decl (class_wrapper->get ());
  tree result = build4 (ARRAY_REF, ptr_type_node,
			cpool, build_int_cst (type_jint, index),
			NULL_TREE, NULL_TREE);
  TREE_CONSTANT (result) = 1;
  return build1 (NOP_EXPR, type, result);
}

// This comes directly from gcj.
tree
tree_generator::build_exception_object_ref (tree type)
{
  // Java only passes object via pointer and doesn't require
  // adjusting.  The java object is immediately before the generic
  // exception header.
  tree obj = build0 (EXC_PTR_EXPR, build_pointer_type (type));
  obj = build2 (MINUS_EXPR, TREE_TYPE (obj), obj,
		TYPE_SIZE_UNIT (TREE_TYPE (obj)));
  obj = build1 (INDIRECT_REF, type, obj);
  return obj;
}

// FIXME: this is (sort of) duplicated in bytecode_generator.
model_field *
tree_generator::find_field (const std::string &name,
			    model_class *klass, model_type *type,
			    model_element *request)
{
  std::set<model_field *> result;
  klass->find_members (name, result, klass, NULL);
  if (result.size () == 1)
    {
      model_field *field = *(result.begin ());
      if (field->type () == type)
	return field;
    }
  throw request->error ("couldn't find field %1 of type %2 in class %3")
    % name % type % klass;
}

tree
tree_generator::build_class_ref (model_type *t, model_element *request)
{
  tree result;

  assert (t != null_type);
  if (t->primitive_p () || t == primitive_void_type)
    {
      model_class *wrapper = box_primitive_type (t);
      // Maybe for BC we could still emit a direct reference to the
      // primitive class?
      model_field *field
	= find_field ("TYPE", wrapper,
		      global->get_compiler ()->java_lang_Class (),
		      request);
      result = gcc_builtins->map_field_ref (class_wrapper, NULL_TREE,
					    field);
    }
  else if (t->array_p ())
    {
      // We can't refer to array types directly, so we emit a
      // reference via the constant pool.
      model_class *klass = assert_cast<model_class *> (t);
      result = build_ref_from_constant_pool (type_class_ptr,
					     class_wrapper->add (klass));
    }
  else
    {
      gcj_abi *abi = gcc_builtins->find_abi ();
      result = abi->build_class_reference (gcc_builtins, class_wrapper,
					   assert_cast<model_class *> (t));
    }
  return result;
}

tree
tree_generator::build_class_ref (const std::string &classname)
{
  gcj_abi *abi = gcc_builtins->find_abi ();
  return abi->build_class_reference (gcc_builtins, class_wrapper, classname);
}

tree
tree_generator::build_divide (tree result_type, tree lhs, tree rhs)
{
  enum tree_code opcode;
  if (result_type == type_jfloat || result_type == type_jdouble)
    opcode = RDIV_EXPR;
  else
    {
      if (flag_use_divide_subroutine)
	{
	  tree func;
	  if (result_type == type_jlong)
	    func = builtin_Jv_divJ;
	  else
	    func = builtin_Jv_divI;
	  return build3 (CALL_EXPR, result_type, func,
			 tree_cons (NULL_TREE, lhs,
				    build_tree_list (NULL_TREE, rhs)),
			 NULL_TREE);
	}
      opcode = TRUNC_DIV_EXPR;
    }

  return build2 (opcode, result_type, lhs, rhs);
}

tree
tree_generator::build_mod (tree result_type, tree lhs, tree rhs)
{
  tree func;
  if (result_type == type_jfloat)
    func = build_address_of (built_in_decls[BUILT_IN_FMODF]);
  else if (result_type == type_jdouble)
    func = build_address_of (built_in_decls[BUILT_IN_FMOD]);
  else
    {
      if (! flag_use_divide_subroutine)
	return build2 (TRUNC_MOD_EXPR, result_type, lhs, rhs);

      if (result_type == type_jlong)
	func = builtin_Jv_remJ;
      else
	func = builtin_Jv_remI;
    }

  return build3 (CALL_EXPR, result_type, func,
		 tree_cons (NULL_TREE, lhs,
			    build_tree_list (NULL_TREE, rhs)),
		 NULL_TREE);
}

tree
tree_generator::build_array_reference (tree array, tree index,
				       tree result_type,
				       bool use_checks)
{
  tree array_type = TREE_TYPE (TREE_TYPE (array));
  // Note that the 'data' field is a back-end invention; it does not
  // exist in the model, so we can't look for it there.
  tree datafield = gcc_builtins->find_decl (array_type, "data");

  array = save_expr (array);
  index = save_expr (index);

  tree result
    = build4 (ARRAY_REF, result_type,
	      build3 (COMPONENT_REF, TREE_TYPE (datafield),
		      build1 (INDIRECT_REF, array_type,
			      gcc_builtins->check_reference (array)),
		      datafield, NULL_TREE),
	      index,
	      NULL_TREE, NULL_TREE);
  TREE_SIDE_EFFECTS (result) = (TREE_SIDE_EFFECTS (array)
				|| TREE_SIDE_EFFECTS (index));

  if (use_checks && flag_bounds_check)
    {
      tree field = gcc_builtins->find_decl (array_type, "length");

      // First: if ((unsigned) index >= (unsigned) length) throw
      tree length = build3 (COMPONENT_REF, type_jint,
			    // Note we don't use check_reference here,
			    // as we it would be redundant.
			    build1 (INDIRECT_REF, array_type, array),
			    field, NULL_TREE);
      tree check = build3 (COND_EXPR, void_type_node,
			   build2 (GE_EXPR, type_jboolean,
				   build1 (NOP_EXPR, type_juint, index),
				   build1 (NOP_EXPR, type_juint, length)),
			   build3 (CALL_EXPR, void_type_node,
				   builtin_Jv_ThrowBadArrayIndex,
				   NULL_TREE, NULL_TREE),
			   build_empty_stmt ());
      result = build2 (COMPOUND_EXPR, result_type, check, result);
      TREE_SIDE_EFFECTS (result) = (TREE_SIDE_EFFECTS (array)
				    || TREE_SIDE_EFFECTS (index));
    }

  return result;
}

// Tree generation.

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

// This include must come first.
#include "tree/glue.hh"

#include "bytecode/cpool.hh"
#include "tree/tree.hh"

/// This is a code generator that generates GCC 'GENERIC' trees.
/// This is a language-independent tree representation.  Currently
/// the documentation is here:
///   http://gcc.gnu.org/ml/gcc/2002-08/msg01397.html

tree_generator::tree_generator (tree_builtins *builtins)
  : gcc_builtins (builtins),
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

void
tree_generator::emit_type_assertion (model_type *to_type,
				     model_type *from_type)
{
}



void
tree_generator::visit_method (model_method *meth,
			      const std::list<ref_variable_decl> &params,
			      const ref_block &block)
{
  method = meth;
  gcc_builtins->lay_out_class (meth->get_declaring_class ());
  method_tree = gcc_builtins->map_method (meth);

  if (block)
    {
      assert (this_tree == NULL_TREE);
      if (! meth->static_p ())
	this_tree = DECL_ARGUMENTS (method_tree);

      current_block = make_node (BLOCK);
      BLOCK_SUPERCONTEXT (current_block) = method_tree;
      DECL_INITIAL (method_tree) = current_block;

      block->visit (this);
      // Handle synchronized methods.  This isn't done for JNI
      // methods, since such synchronization is handled by the VM.
      if ((meth->get_modifiers () & ACC_SYNCHRONIZED) != 0)
	{
	  tree k;
	  if (meth->static_p ())
	    k = build_class_ref (meth->get_declaring_class ());
	  else
	    k = this_tree;
	  current = wrap_synchronized (k, current);
	}

      current = build3 (BIND_EXPR,
			gcc_builtins->map_type (meth->get_return_type ()),
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
	  DECL_EXTERNAL (method_tree) = 1;
	}
    }

  DECL_SAVED_TREE (method_tree) = current;
}

// Build a stub to call a JNI method.  This code comes almost verbatim
// from the old gcj.
tree
tree_generator::build_jni_stub ()
{
  DECL_ARTIFICIAL (method_tree) = 1;
  DECL_EXTERNAL (method_tree) = 0;

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
  tree klass = gcc_builtins->map_type (method->get_declaring_class ());
  tree body = build2 (MODIFY_EXPR, ptr_type_node, env_var,
		      build3 (CALL_EXPR, ptr_type_node,
			      build_address_of (builtin_Jv_GetJNIEnvNewFrame),
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
  tree method_sig = get_identifier (method->get_descriptor ().c_str ());
  method_sig
    = build_ref_from_constant_pool (alloc_name_constant (CONSTANT_Utf8,
							 method_sig));
  // FIXME: set TREE_TYPE on method_sig.
  // FIXME: this should probably be done by build_ref_from_constant_pool?
  TREE_CONSTANT (method_sig) = 1;
  tree lookup_arg = tree_cons (NULL_TREE, method_sig, tem);

  tem = build_ref_from_constant_pool (alloc_name_constant (CONSTANT_Utf8,
							   DECL_NAME (method_tree)));
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
					 build_address_of
					 (builtin_Jv_LookupJNIMethod),
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
		 build_address_of (builtin_Jv_JNI_PopSystemFrame),
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
    return;

  // Add assertion-related members.
  ref_field disabled = method->get_declaring_class ()->add_assert_members ();
  gcc_builtins->lay_out_class (disabled->get_declaring_class ());
  tree disabled_tree = gcc_builtins->map_field (disabled.get ());

  first->visit (this);
  tree first_tree = current;
  second->visit (this);
  tree second_tree = current;

  // Generate:
  //   if (! $assertionsDisabled && ! FIRST) throw new AssertionError (SECOND)
  tree throw_node = build3 (CALL_EXPR, void_type_node,
			    builtin_Jv_Throw,
			    // fixme new ...
			    tree_cons (NULL_TREE, second_tree, NULL_TREE),
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
      (*i)->visit (this);
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
			var, gcc_builtins->build_exception_object_ref (vtype));

  tsi_link_after (&out, assign, TSI_CONTINUE_LINKING);

  // Generate code for the body and link it in.
  body->visit (this);
  tsi_link_after (&out, current, TSI_CONTINUE_LINKING);

  tree bind = build3 (BIND_EXPR, void_type_node,
		      BLOCK_VARS (current_block),
		      body_tree, current_block);

  current = build2 (CATCH_EXPR, void_type_node,
		    // FIXME: must prepare the catch type properly...
		    vtype, bind);
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
  tree test = build0 (LABEL_DECL, NULL_TREE);
  tree done = build0 (LABEL_DECL, NULL_TREE);
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
  tsi_link_after (&out, build1 (LABEL_EXPR, void_type_node, test),
		  TSI_CONTINUE_LINKING);
  tsi_link_after (&out, stmt_tree, TSI_CONTINUE_LINKING);
  tsi_link_after (&out, build1 (EXIT_EXPR,
				void_type_node,
				build1 (TRUTH_NOT_EXPR,
					type_jboolean,
					expr_tree)),
		  TSI_CONTINUE_LINKING);

  // Wrap the body in a loop.
  body_tree = build1 (LOOP_EXPR, void_type_node, body_tree);
  annotate (current, dstmt);

  // Now make an outer statement list with the remaining label.
  tree outer = alloc_stmt_list ();
  out = tsi_start (outer);
  tsi_link_after (&out, body_tree, TSI_CONTINUE_LINKING);
  tsi_link_after (&out, build1 (LABEL_EXPR, void_type_node, done),
		  TSI_CONTINUE_LINKING);
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
  tree update_tree = build_decl (LABEL_DECL, NULL_TREE, NULL_TREE);
  tree done_tree = build_decl (LABEL_DECL, NULL_TREE, NULL_TREE);
  target_map[fstmt] = std::make_pair (update_tree, done_tree);

  tree body_tree = alloc_stmt_list ();
  tree_stmt_iterator out = tsi_start (body_tree);

  if (container->type ()->array_p ())
    {
    }
  else
    {
      // Use an iterator.
    }

  tsi_link_after (&out, done_tree, TSI_CONTINUE_LINKING);
  current = body_tree;
}

void
tree_generator::visit_for (model_for *fstmt,
			   const ref_stmt &init,
			   const ref_expression &cond,
			   const ref_stmt &body,
			   const ref_stmt &update)
{
  tree update_tree = build_decl (LABEL_DECL, NULL_TREE, NULL_TREE);
  tree done_tree = build_decl (LABEL_DECL, NULL_TREE, NULL_TREE);
  target_map[fstmt] = std::make_pair (update_tree, done_tree);

  save_tree saver (current_block, make_node (BLOCK));
  BLOCK_SUPERCONTEXT (current_block) = saver.get ();

  tree body_tree = alloc_stmt_list ();
  tree_stmt_iterator out = tsi_start (body_tree);

  if (init)
    {
      init->visit (this);
      tsi_link_after (&out, current, TSI_CONTINUE_LINKING);
    }
  if (cond)
    {
      cond->visit (this);
      tsi_link_after (&out, build1 (EXIT_EXPR, void_type_node,
				    build1 (TRUTH_NOT_EXPR,
					    type_jboolean,
					    current)),
		      TSI_CONTINUE_LINKING);
    }
  body->visit (this);
  tsi_link_after (&out, current, TSI_CONTINUE_LINKING);

  tsi_link_after (&out, update_tree, TSI_CONTINUE_LINKING);
  if (update)
    {
      update->visit (this);
      tsi_link_after (&out, current, TSI_CONTINUE_LINKING);
    }

  tsi_link_after (&out, done_tree, TSI_CONTINUE_LINKING);

  current = build3 (BIND_EXPR, void_type_node,
		    BLOCK_VARS (current_block),
		    body_tree, current_block);
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
  false_branch->visit (this);
  tree false_tree = current;

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
      tree brk = build0 (LABEL_DECL, NULL_TREE);
      target_map[label] = std::make_pair (NULL_TREE, brk);

      tree body_tree = alloc_stmt_list ();
      tree_stmt_iterator out = tsi_start (body_tree);

      stmt->visit (this);
      tsi_link_after (&out, current, TSI_CONTINUE_LINKING);
      tsi_link_after (&out, build1 (LABEL_EXPR, void_type_node,
				    brk),
		      TSI_CONTINUE_LINKING);

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
      current = build2 (MODIFY_EXPR, gcc_builtins->map_type (expr->type ()),
			DECL_RESULT (method_tree), current);
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

  tree body_tree = alloc_stmt_list ();
  tree_stmt_iterator out = tsi_start (body_tree);

  model_switch_block *def = swstmt->get_default ();
  for (std::list<ref_switch_block>::const_iterator i = blocks.begin ();
       i != blocks.end ();
       ++i)
    {
      if ((*i).get () == def)
	tsi_link_after (&out, build3 (CASE_LABEL_EXPR, NULL_TREE,
				      NULL_TREE, NULL_TREE, NULL_TREE),
			TSI_CONTINUE_LINKING);
      (*i)->visit (this);
      tsi_link_after (&out, current, TSI_CONTINUE_LINKING);
    }

  current = build3 (SWITCH_EXPR, NULL_TREE, expr_tree, body_tree, NULL_TREE);
  annotate (current, swstmt);
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

  tree label = build0 (LABEL_DECL, NULL_TREE);

  std::list<ref_expression> labels = swblock->get_labels ();
  for (std::list<ref_expression>::const_iterator i = labels.begin ();
       i != labels.end ();
       ++i)
    {
      jint value = jint (intb->convert ((*i)->type (), (*i)->value ()));
      tree new_label = build3 (CASE_LABEL_EXPR,
			       void_type_node,
			       build_int (value), NULL_TREE, label);
      tsi_link_after (&out, new_label, TSI_CONTINUE_LINKING);
    }

  tree stmt_tree = transform_list (statements);
  tsi_link_after (&out, stmt_tree, TSI_CONTINUE_LINKING);

  current = body_tree;
}

tree
tree_generator::wrap_synchronized (tree expr, tree body)
{
  // Generate a temporary variable to hold the expression's value.
  char buf[20];
  sprintf (buf, "$synctemp%d", temp_counter);
  ++temp_counter;

  tree expr_decl = build_decl (VAR_DECL, get_identifier (buf),
			       TREE_TYPE (expr));
  DECL_INITIAL (expr_decl) = current;
  TREE_CHAIN (expr_decl) = BLOCK_VARS (current_block);
  BLOCK_VARS (current_block) = expr_decl;

  // Emit a call to enter the monitor.
  tree enter = build3 (CALL_EXPR, void_type_node,
		       builtin_Jv_MonitorEnter,
		       build_tree_list (NULL_TREE, expr_decl),
		       NULL_TREE);
  TREE_SIDE_EFFECTS (enter) = 1;

  // Build a call to leave the monitor, we use it shortly.
  tree exit_tree = build3 (CALL_EXPR, void_type_node,
			   builtin_Jv_MonitorExit,
			   build_tree_list (NULL_TREE, expr_decl),
			   NULL_TREE);
  TREE_SIDE_EFFECTS (exit_tree) = 1;

  tree body_tree = alloc_stmt_list ();
  tree_stmt_iterator out = tsi_start (body_tree);
  tsi_link_after (&out, enter, TSI_CONTINUE_LINKING);

  tsi_link_after (&out, body, TSI_CONTINUE_LINKING);

  return build2 (TRY_FINALLY_EXPR, void_type_node,
		 body_tree, exit_tree);
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

  // Now generate trees for the catch clauses.
  tree catch_tree = alloc_stmt_list ();
  tree_stmt_iterator out = tsi_start (catch_tree);
  for (std::list<ref_catch>::const_iterator i = catchers.begin ();
       i != catchers.end ();
       ++i)
    {
      (*i)->visit (this);
      // It is fine to simply link in CURRENT here, since we know that
      // each catcher will just generate a CATCH_EXPR.
      tsi_link_after (&out, current, TSI_CONTINUE_LINKING);
    }

  // Generate an internal try-catch.
  tree result = build2 (TRY_CATCH_EXPR, NULL_TREE, body_tree, catch_tree);

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
}

void
tree_generator::visit_variable_stmt (model_variable_stmt *,
				     const std::list<ref_variable_decl> &decls)
{
  for (std::list<ref_variable_decl>::const_iterator i = decls.begin ();
       i != decls.end ();
       ++i)
    {
      tree decl = add_var (*i);
      if ((*i)->has_initializer_p ())
	{
	  ref_expression init = (*i)->get_initializer ();
	  init->visit (this);
	  DECL_INITIAL (decl) = current;
	}
    }
}

void
tree_generator::visit_while (model_while *wstmt,
			     const ref_expression &cond,
			     const ref_stmt &body)
{
  tree again = build0 (LABEL_DECL, NULL_TREE);
  tree done = build0 (LABEL_DECL, NULL_TREE);
  target_map[wstmt] = std::make_pair (again, done);

  // Create the body of the loop: first the 'again' label, then the
  // exit expression, then the actual body.
  tree body_tree = alloc_stmt_list ();
  tree_stmt_iterator out = tsi_start (body_tree);
  tsi_link_after (&out, build1 (LABEL_EXPR, void_type_node, again),
		  TSI_CONTINUE_LINKING);

  cond->visit (this);
  tsi_link_after (&out, build1 (EXIT_EXPR, void_type_node,
				build1 (TRUTH_NOT_EXPR,
					type_jboolean,
					current)),
		  TSI_CONTINUE_LINKING);

  body->visit (this);
  tsi_link_after (&out, current, TSI_CONTINUE_LINKING);

  // Now wrap the body in a loop, and add the "done" label.
  body_tree = build1 (LOOP_EXPR, void_type_node, body_tree);

  current = alloc_stmt_list ();
  out = tsi_start (current);
  tsi_link_after (&out, body_tree, TSI_CONTINUE_LINKING);
  tsi_link_after (&out, build1 (LABEL_EXPR, void_type_node, done),
		  TSI_CONTINUE_LINKING);

  annotate (current, wstmt);
}



void
tree_generator::visit_array_initializer (model_array_initializer *,
					 const ref_forwarding_type &elt_type,
					 const std::list<ref_expression> &exprs)
{
  // FIXME: constant array initialization optimization... ?

  // At this point, 'current' is the 'new' expression for the array.
  tree new_expr = save_expr (current);

  tree result = alloc_stmt_list ();
  tree_stmt_iterator out = tsi_start (result);

  tree elt_tree = gcc_builtins->map_type (elt_type->type ());

  int index = 0;
  for (std::list<ref_expression>::const_iterator i = exprs.begin ();
       i != exprs.end ();
       ++i)
    {
      (*i)->visit (this);
      tree value = current;

      tree idx_tree = build_int_cst (type_jint, index);
      tree assign
	= build2 (MODIFY_EXPR, elt_tree,
		  gcc_builtins->build_array_reference (new_expr, idx_tree,
						       elt_tree, false),
		  value);
      TREE_SIDE_EFFECTS (assign) = 1;
      tsi_link_after (&out, assign, TSI_CONTINUE_LINKING);
      ++index;
    }

  // Yield 'new_expr'.
  tsi_link_after (&out, new_expr, TSI_CONTINUE_LINKING);
  current = result;
}

void
tree_generator::visit_array_ref (model_array_ref *,
				 const ref_expression &array,
				 const ref_expression &index)
{
  array->visit (this);
  tree array_tree = current;
  index->visit (this);
  tree index_tree = current;
  tree component_type
    = gcc_builtins->map_type (array->type ()->element_type ());

  current = gcc_builtins->build_array_reference (array_tree, index_tree,
						 component_type);
}

void
tree_generator::binary_operator (tree_code code,
				 const ref_expression &lhs,
				 const ref_expression &rhs)
{
  lhs->visit (this);
  tree lhs_tree = current;
  rhs->visit (this);
  tree rhs_tree = current;
  current = build2 (code, gcc_builtins->map_type (lhs->type ()),
		    lhs_tree, rhs_tree);
  TREE_SIDE_EFFECTS (current) = (TREE_SIDE_EFFECTS (lhs_tree)
				 || TREE_SIDE_EFFECTS (rhs_tree));
}

void
tree_generator::visit_arith_binary (model_minus *,
				    const ref_expression &lhs,
				    const ref_expression &rhs)
{
  binary_operator (MINUS_EXPR, lhs, rhs);
}

void
tree_generator::visit_arith_binary (model_mult *,
				    const ref_expression &lhs,
				    const ref_expression &rhs)
{
  binary_operator (MULT_EXPR, lhs, rhs);
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

  current = gcc_builtins->build_divide (gcc_builtins->map_type (op->type ()),
					lhs_tree, rhs_tree);
  TREE_SIDE_EFFECTS (current) = (TREE_SIDE_EFFECTS (lhs_tree)
				 | TREE_SIDE_EFFECTS (rhs_tree));
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

  current = gcc_builtins->build_mod (gcc_builtins->map_type (op->type ()),
				     lhs_tree, rhs_tree);
  TREE_SIDE_EFFECTS (current) = (TREE_SIDE_EFFECTS (lhs_tree)
				 | TREE_SIDE_EFFECTS (rhs_tree));
}

void
tree_generator::visit_arith_binary (model_and *,
				    const ref_expression &lhs,
				    const ref_expression &rhs)
{
  binary_operator (BIT_AND_EXPR, lhs, rhs);
}

void
tree_generator::visit_arith_binary (model_or *,
				    const ref_expression &lhs,
				    const ref_expression &rhs)
{
  binary_operator (BIT_IOR_EXPR, lhs, rhs);
}

void
tree_generator::visit_arith_binary (model_xor *,
				    const ref_expression &lhs,
				    const ref_expression &rhs)
{
  binary_operator (BIT_XOR_EXPR, lhs, rhs);
}

void
tree_generator::visit_arith_binary (model_plus *model,
				    const ref_expression &lhs,
				    const ref_expression &rhs)
{
  binary_operator (PLUS_EXPR, lhs, rhs);
}

tree
tree_generator::arith_shift (tree_code kind,
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
  if (is_unsigned)
    {
      tree conv = build1 (CONVERT_EXPR, saved_lhs_type, result);
      TREE_SIDE_EFFECTS (conv) = TREE_SIDE_EFFECTS (result);
      result = conv;
    }

  return result;
}

void
tree_generator::visit_arith_shift (model_left_shift *,
				   const ref_expression &lhs,
				   const ref_expression &rhs)
{
  current = arith_shift (LSHIFT_EXPR, lhs, rhs, false);
}

void
tree_generator::visit_arith_shift (model_right_shift *,
				   const ref_expression &lhs,
				   const ref_expression &rhs)
{
  current = arith_shift (RSHIFT_EXPR, lhs, rhs, false);
}

void
tree_generator::visit_arith_shift (model_unsigned_right_shift *,
				   const ref_expression &lhs,
				   const ref_expression &rhs)
{
  current = arith_shift (RSHIFT_EXPR, lhs, rhs, true);
}

void
tree_generator::visit_assignment (model_assignment *,
				  const ref_expression &lhs,
				  const ref_expression &rhs)
{
  lhs->visit (this);
  tree lhs_tree = current;
  rhs->visit (this);
  tree rhs_tree = current;
  // FIXME: if LHS is array, may need assign. check.
  current = build2 (MODIFY_EXPR, gcc_builtins->map_type (lhs->type ()),
		    lhs_tree, rhs_tree);
  TREE_SIDE_EFFECTS (current) = 1;
}

void
tree_generator::handle_op_assignment (tree_code op,
				      const ref_expression &lhs,
				      const ref_expression &rhs)
{
  lhs->visit (this);
  tree lhs_tree = save_expr (current);
  rhs->visit (this);
  tree rhs_tree = current;
  current = build2 (MODIFY_EXPR, gcc_builtins->map_type (lhs->type ()),
		    lhs_tree,
		    build2 (op, gcc_builtins->map_type (lhs->type ()),
			    lhs_tree, rhs_tree));
  TREE_SIDE_EFFECTS (current) = 1;

  model_type *lhs_type = lhs->type ();
  model_type *rhs_type = rhs->type ();
  if (! lhs_type->primitive_p () && lhs_type != null_type
      && ! rhs_type->primitive_p () && rhs_type != null_type)
    emit_type_assertion (lhs_type, rhs_type);
}

void
tree_generator::visit_op_assignment (model_minus_equal *,
				     const ref_expression &lhs,
				     const ref_expression &rhs)
{
  handle_op_assignment (MINUS_EXPR, lhs, rhs);
}

void
tree_generator::visit_op_assignment (model_mult_equal *,
				     const ref_expression &lhs,
				     const ref_expression &rhs)
{
  handle_op_assignment (MULT_EXPR, lhs, rhs);
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
		    gcc_builtins->build_divide (div_type, lhs_tree,
						rhs_tree));
  TREE_SIDE_EFFECTS (current) = 1;
}

void
tree_generator::visit_op_assignment (model_and_equal *,
				     const ref_expression &lhs,
				     const ref_expression &rhs)
{
  handle_op_assignment (BIT_AND_EXPR, lhs, rhs);
}

void
tree_generator::visit_op_assignment (model_or_equal *,
				     const ref_expression &lhs,
				     const ref_expression &rhs)
{
  handle_op_assignment (BIT_IOR_EXPR, lhs, rhs);
}

void
tree_generator::visit_op_assignment (model_xor_equal *,
				     const ref_expression &lhs,
				     const ref_expression &rhs)
{
  handle_op_assignment (BIT_XOR_EXPR, lhs, rhs);
}

void
tree_generator::visit_op_assignment (model_mod_equal *op,
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
		    gcc_builtins->build_mod (div_type, lhs_tree,
					     rhs_tree));
  TREE_SIDE_EFFECTS (current) = 1;
}

void
tree_generator::visit_op_assignment (model_ls_equal *,
				     const ref_expression &lhs,
				     const ref_expression &rhs)
{
  handle_op_assignment (LSHIFT_EXPR, lhs, rhs);
}

void
tree_generator::visit_op_assignment (model_rs_equal *,
				     const ref_expression &lhs,
				     const ref_expression &rhs)
{
  handle_op_assignment (RSHIFT_EXPR, lhs, rhs);
}

void
tree_generator::visit_op_assignment (model_urs_equal *,
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
}

void
tree_generator::visit_op_assignment (model_plus_equal *,
				     const ref_expression &lhs,
				     const ref_expression &rhs)
{
  handle_op_assignment (PLUS_EXPR, lhs, rhs);
}

void
tree_generator::visit_cast (model_cast *,
			    const ref_forwarding_type &dest,
			    const ref_expression &expr)
{
  expr->visit (this);
  tree expr_tree = current;

  if (dest->type ()->primitive_p ())
    {
      current = build1 (CONVERT_EXPR, gcc_builtins->map_type (dest->type ()),
			expr_tree);
      TREE_SIDE_EFFECTS (current) = TREE_SIDE_EFFECTS (expr_tree);
    }
  else
    {
      // Reference types.  We only need to emit a cast check if the
      // types are known to be incompatible.
      if (dest->type ()->assignable_from_p (expr->type ()))
	{
	  current = build3 (CALL_EXPR, gcc_builtins->map_type (dest->type ()),
			    builtin_Jv_CheckCast,
			    build_tree_list (NULL_TREE, expr_tree),
			    NULL_TREE);
	  TREE_SIDE_EFFECTS (current) = 1;
	}
      else
	{
	  emit_type_assertion (dest->type (), expr->type ());
	  current = build1 (NOP_EXPR, gcc_builtins->map_type (dest->type ()),
			    expr_tree);
	  TREE_SIDE_EFFECTS (current) = TREE_SIDE_EFFECTS (expr_tree);
	}
    }
}

tree
tree_generator::build_class_ref (tree klass)
{
  return gcc_builtins->build_class_ref (klass);
}

tree
tree_generator::build_class_ref (model_type *t)
{
  return build_class_ref (gcc_builtins->map_type (t));
}

void
tree_generator::visit_class_ref (model_class_ref *,
				 const ref_forwarding_type &req)
{
  current = build_class_ref (req->type ());
}

void
tree_generator::visit_comparison (model_equal *,
				  const ref_expression &lhs,
				  const ref_expression &rhs)
{
  binary_operator (EQ_EXPR, lhs, rhs);
}

void
tree_generator::visit_comparison (model_notequal *,
				  const ref_expression &lhs,
				  const ref_expression &rhs)
{
  binary_operator (NE_EXPR, lhs, rhs);
}

void
tree_generator::visit_comparison (model_lessthan *,
				  const ref_expression &lhs,
				  const ref_expression &rhs)
{
  binary_operator (LT_EXPR, lhs, rhs);
}

void
tree_generator::visit_comparison (model_greaterthan *,
				  const ref_expression &lhs,
				  const ref_expression &rhs)
{
  binary_operator (GT_EXPR, lhs, rhs);
}

void
tree_generator::visit_comparison (model_lessthanequal *,
				  const ref_expression &lhs,
				  const ref_expression &rhs)
{
  binary_operator (LE_EXPR, lhs, rhs);
}

void
tree_generator::visit_comparison (model_greaterthanequal *,
				  const ref_expression &lhs,
				  const ref_expression &rhs)
{
  binary_operator (GE_EXPR, lhs, rhs);
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
}

void
tree_generator::visit_field_ref (model_field_ref *,
				 const ref_expression &expr,
				 const model_field *field)
{
  // Note that we don't need any special handling for 'array.length'
  // -- the generic code here works fine.
  tree expr_tree = NULL_TREE;
  if (expr)
    {
      expr->visit (this);
      expr_tree = current;
    }

  // FIXME: where should this go?
  if (expr->type () != field->get_declaring_class ())
    // FIXME: is this right?
    emit_type_assertion (field->get_declaring_class (), expr->type ());

  // FIXME: should we handle inlining constant fields here or in the
  // ABI?

  // Note that map_field_ref handles the case of a non-static
  // reference to a static field.
  gcc_builtins->lay_out_class (field->get_declaring_class ());
  current = gcc_builtins->map_field_ref (expr_tree,
					 const_cast<model_field *> (field));
}

void
tree_generator::visit_field_initializer (model_field_initializer *,
					 model_field *field)
{
  if ((field->static_p () && field->constant_p ())
      || ! field->has_initializer_p ())
    {
      // Ignore constant fields.
      return;
    }

  abort ();			// FIXME
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
tree_generator::visit_instanceof (model_instanceof *,
				  const ref_expression &expr,
				  const ref_forwarding_type &klass)
{
  expr->visit (this);
  tree expr_tree = save_expr (current);
  tree class_tree = build_class_ref (klass->type ());
  current = handle_instanceof (expr_tree, class_tree);
}

void
tree_generator::visit_logical_binary (model_lor *,
				      const ref_expression &lhs,
				      const ref_expression &rhs)
{
  binary_operator (TRUTH_ORIF_EXPR, lhs, rhs);
}

void
tree_generator::visit_logical_binary (model_land *,
				      const ref_expression &lhs,
				      const ref_expression &rhs)
{
  binary_operator (TRUTH_ANDIF_EXPR, lhs, rhs);
}

void
tree_generator::visit_simple_literal (model_literal_base *,
				      const jboolean &val)
{
  current = val ? boolean_true_node : boolean_false_node;
}

void
tree_generator::visit_simple_literal (model_literal_base *,
				      const jbyte &val)
{
  current = build_int (val, type_jbyte);
}

void
tree_generator::visit_simple_literal (model_literal_base *,
				      const jchar &val)
{
  current = build_int (val, type_jchar);
}

void
tree_generator::visit_simple_literal (model_literal_base *,
				      const jshort &val)
{
  current = build_int (val, type_jshort);
}

void
tree_generator::visit_simple_literal (model_literal_base *,
				      const jint &val)
{
  current = build_int (val);
}

void
tree_generator::visit_simple_literal (model_literal_base *,
				      const jlong &val)
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
tree_generator::visit_simple_literal (model_literal_base *,
				      const jfloat &val)
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
tree_generator::visit_simple_literal (model_literal_base *,
				      const jdouble &val)
{
  current = handle_double (val);
}

tree
tree_generator::handle_string_literal (const std::string &val)
{
  tree node = get_identifier (val.c_str ());
  int location = alloc_name_constant (CONSTANT_String, node);
  node = build_ref_from_constant_pool (location);
  TREE_TYPE (node)
    = gcc_builtins->map_type (global->get_compiler ()->java_lang_String ());
  TREE_CONSTANT (node) = 1;
  return node;
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
  if (this_expr_tree != NULL_TREE
      && ! meth->static_p ()
      && this_expr_tree != this_tree)
    this_expr_tree = gcc_builtins->check_reference (this_expr_tree);

  current = gcc_builtins->map_method_call (this_expr_tree, arg_tree,
					   const_cast<model_method *> (meth),
					   is_super);
}

void
tree_generator::visit_method_invocation (model_method_invocation *,
					 const model_method *meth,
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
(model_type_qualified_invocation *,
 const model_method *meth,
 const std::list<ref_expression> &args,
 bool super)
{
  handle_invocation (meth, meth->static_p () ? this_tree : NULL_TREE, args);
}

void
tree_generator::visit_super_invocation (model_super_invocation *,
					const model_method *meth,
					const std::list<ref_expression> &args)
{
  handle_invocation (meth, this_tree, args, true);
}

void
tree_generator::visit_this_invocation (model_this_invocation *,
				       const model_method *meth,
				       const std::list<ref_expression> &args)
{
  handle_invocation (meth, this_tree, args);
}

void
tree_generator::visit_new (model_new *,
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

  model_class *klassp = assert_cast<model_class *> (klass->type ());
  gcc_builtins->lay_out_class (klassp);
  current
    = gcc_builtins->map_new (klassp,
			     gcc_builtins->map_method (const_cast<model_method *>(constructor)),
			     arg_tree);
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
	current = build_new_array (elt_type->type (), ind_tree);
      else
	current = build_new_object_array (elt_type->type (), ind_tree);
    }
  else
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
    }

  if (init)
    init->visit (this);
}

void
tree_generator::visit_null_literal (model_null_literal *)
{
  current = null_pointer_node;
}

void
tree_generator::handle_inc_dec (tree_code op, const ref_expression &expr)
{
  expr->visit (this);
  tree expr_tree = current;
  current = build2 (op, gcc_builtins->map_type (expr->type ()), expr_tree,
		    integer_one_node);
  TREE_SIDE_EFFECTS (current) = 1;
}

void
tree_generator::visit_prefix_side_effect (model_prefix_plusplus *,
					  const ref_expression &expr)
{
  handle_inc_dec (PREINCREMENT_EXPR, expr);
}

void
tree_generator::visit_prefix_side_effect (model_prefix_minusminus *,
					  const ref_expression &expr)
{
  handle_inc_dec (PREDECREMENT_EXPR, expr);
}

void
tree_generator::visit_prefix_simple (model_prefix_plus *,
				     const ref_expression &expr)
{
  // Nothing special to do here.
  expr->visit (this);
}

void
tree_generator::visit_prefix_simple (model_prefix_minus *,
				     const ref_expression &expr)
{
  expr->visit (this);
  tree hold = current;
  current = build1 (NEGATE_EXPR, gcc_builtins->map_type (expr->type ()), hold);
  TREE_SIDE_EFFECTS (current) = TREE_SIDE_EFFECTS (hold);
}

void
tree_generator::visit_prefix_simple (model_bitwise_not *,
				     const ref_expression &expr)
{
  expr->visit (this);
  tree hold = current;
  current = build1 (BIT_NOT_EXPR, gcc_builtins->map_type (expr->type ()),
		    hold);
  TREE_SIDE_EFFECTS (current) = TREE_SIDE_EFFECTS (hold);
}

void
tree_generator::visit_prefix_simple (model_logical_not *,
				     const ref_expression &expr)
{
  expr->visit (this);
  tree hold = current;
  current = build1 (TRUTH_NOT_EXPR, type_jboolean, hold);
  TREE_SIDE_EFFECTS (current) = TREE_SIDE_EFFECTS (hold);
}

void
tree_generator::visit_postfix_side_effect (model_postfix_plusplus *,
					   const ref_expression &expr)
{
  handle_inc_dec (POSTINCREMENT_EXPR, expr);
}

void
tree_generator::visit_postfix_side_effect (model_postfix_minusminus *,
					   const ref_expression &expr)
{
  handle_inc_dec (POSTDECREMENT_EXPR, expr);
}

void
tree_generator::visit_this (model_this *)
{
  assert (this_tree != NULL_TREE);
  current = this_tree;
}

void
tree_generator::visit_simple_variable_ref (model_simple_variable_ref *,
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
tree_generator::build_new_array (model_type *elt_type, tree size)
{
  assert (elt_type->primitive_p ());
  model_type *array_type = elt_type->array ();

  tree elt_type_tree = gcc_builtins->map_type (elt_type);
  tree array_type_tree = gcc_builtins->map_type (array_type);

  tree insn = build3 (CALL_EXPR, array_type_tree, 
		      builtin_Jv_NewPrimArray,
		      tree_cons (NULL_TREE,
				 build_class_ref (elt_type_tree),
				 tree_cons (NULL_TREE, size, NULL_TREE)),
		      NULL_TREE);
  TREE_SIDE_EFFECTS (insn) = 1;

  return insn;
}

tree
tree_generator::build_new_object_array (model_type *elt_type, tree size)
{
  model_type *array_type = elt_type->array ();

  tree elt_type_tree = gcc_builtins->map_type (elt_type);
  tree array_type_tree = gcc_builtins->map_type (array_type);

  tree args = tree_cons (NULL_TREE, size,
			 tree_cons (NULL_TREE, elt_type_tree,
				    tree_cons (NULL_TREE, null_pointer_node,
					       NULL_TREE)));

  tree insn = build3 (CALL_EXPR, array_type_tree, builtin_Jv_NewObjectArray,
		      args, NULL_TREE);
  TREE_SIDE_EFFECTS (insn) = 1;

  return insn;
}

int
tree_generator::alloc_name_constant (classfile_type_constant, tree)
{
  abort ();			// FIXME
}

tree
tree_generator::build_ref_from_constant_pool (int)
{
  abort ();			// FIXME
}

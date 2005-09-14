// Bytecode generation.

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

#include "typedefs.hh"
#include "bytecode/outpool.hh"
#include "bytecode/locals.hh"
#include "bytecode/block.hh"
#include "bytecode/attribute.hh"
#include "bytecode/generate.hh"
#include "bytecode/byteutil.hh"
#include "bytecode/classwriter.hh"

/// Returns how many stack slots the given type takes.
static inline size_t
width_of_type (model_type *t)
{
  return wide_p (t) ? 2 : 1;
}

bytecode_generator::bytecode_generator (model_method *m,
					outgoing_constant_pool *cp)
  : method (m),
    cpool (cp),
    vars (this),
    this_index (-1),
    first_block (NULL),
    current_block (NULL),
    current_true_target (NULL),
    current_false_target (NULL),
    expr_target (IGNORE),
    stack_depth (0),
    max_stack (0),
    bytecode_length (0),
    string_plus_type (NULL),
    line_count (0)
{
  current_block = new_bytecode_block ();
  first_block = current_block;
}

bytecode_generator::~bytecode_generator ()
{
  for (std::list<bytecode_block *>::iterator i = allocated_blocks.begin ();
       i != allocated_blocks.end ();
       ++i)
    delete *i;
}

void
bytecode_generator::generate ()
{
  method->visit (this);

  bool changed;
  do
    {
      // Figure out which blocks are dead.
      for (bytecode_block *work = first_block; work; work = work->next ())
	work->reset ();

      // Mark the live blocks, starting with the start of the method.
      std::set<bytecode_block *> work;
      first_block->mark (work);
      // Mark the exception handlers.  Note that we only mark the
      // targets -- the exception regions should be live via some
      // other route.
      for (std::list<handler>::iterator i = handlers.begin ();
	   i != handlers.end ();
	   ++i)
	{
	  handler &hand (*i);
	  if (! hand.start)
	    continue;
	  hand.target->mark (work);
	}

      // Now process the work list.
      while (! work.empty ())
	{
	  std::set<bytecode_block *> new_work;
	  for (std::set<bytecode_block *>::const_iterator i = work.begin ();
	       i != work.end ();
	       ++i)
	    (*i)->mark (new_work);
	  work = new_work;
	}

      changed = false;

      // Update exception handlers.
      for (std::list<handler>::iterator i = handlers.begin ();
	   i != handlers.end ();
	   ++i)
	{
	  handler &hand (*i);

	  if (! hand.start)
	    continue;
	  bool rm = false;
	  if (! hand.start->live_p ())
	    rm = true;
	  else
	    {
	      hand.start = hand.start->update ();
	      hand.end = hand.end->update ();
	      if (hand.start == hand.end)
		rm = true;
	      else
		hand.target = hand.target->update ();
	    }

	  if (rm)
	    {
	      // Note that we don't necessarily know that the end
	      // target is dead, hence no assertion here.
	      hand.start = NULL;
	      hand.end = NULL;
	      hand.target = NULL;
	      hand.type = NULL;
	      changed = true;
	    }
	}

      // Skip leading empty block.
      first_block = first_block->update ();

      int pc = 0;
      int last_line = -1;
      line_count = 0;
      for (bytecode_block *work = first_block; work; work = work->next ())
	{
	  // Relocate this block.
	  if (work->relocate (pc, NULL))
	    changed = true;
	  if (work->get_line_number () != -1
	      && work->get_line_number () != last_line)
	    {
	      last_line = work->get_line_number ();
	      ++line_count;
	    }
	}
      bytecode_length = pc;
    }
  while (changed);

  // A post-pass to put all the live exception types into the
  // constant pool.
  for (std::list<handler>::iterator i = handlers.begin ();
       i != handlers.end ();
       ++i)
    {
      handler &hand (*i);
      if (! hand.start || ! hand.type)
	continue;
      cpool->add (hand.type);
    }

  if (global->get_compiler ()->target_debug ())
    {
      if (line_count > 0)
	attributes.push_back (new line_table_attribute (cpool, this));

      if (vars.update ())
	{
	  attributes.push_back (new local_variable_table_attribute (cpool,
								    &vars,
								    false));
	  if (global->get_compiler ()->target_15 ()
	      && vars.any_parameterized_p ())
	    attributes.push_back (new local_variable_table_attribute (cpool, &vars, true));
	}
    }
}

int
bytecode_generator::count_exception_handlers ()
{
  // Count the number of exception handlers we have.
  int count = 0;
  for (std::list<handler>::iterator i = handlers.begin ();
       i != handlers.end ();
       ++i)
    {
      handler &hand (*i);
      if (hand.start)
	{
	  ++count;
	  // An exception handler's end PC is not inclusive, so it can
	  // overflow even if the bytecode size does not.  So, we
	  // check it.
	  if (hand.end->get_pc () > 65535)
	    throw method->error ("method requires exception region "
				 "past 65535 bytes");
	}
    }
  return count;
}

void
bytecode_generator::write (bytecode_stream *out)
{
  int count = count_exception_handlers ();

  if (count > 65535)
    throw method->error ("method requires more than 65535 exception regions");
  if (bytecode_length > 65535)
    throw method->error ("bytecode contains more than 65535 bytes");
  if (max_stack > 65535)
    throw method->error ("bytecode uses more than 65535 stack slots");
  if (vars.get_max () > 65535)
    throw method->error ("bytecode uses more than 65535 local variable slots");

  out->put2 (max_stack);
  out->put2 (vars.get_max ());
  out->put4 (bytecode_length);

  // Find the byte offset for use when verifying.
  int start_offset = out->get_offset ();

  int pc = 0;
  for (bytecode_block *work = first_block; work; work = work->next ())
    {
      bool check = work->relocate (pc, out);
      assert (! check);
    }
  assert (pc == bytecode_length);

  // Also for use in verification.
  int end_offset = out->get_offset ();

  // Exception table.
  out->put2 (count);
  for (std::list<handler>::iterator i = handlers.begin ();
       i != handlers.end ();
       ++i)
    {
      handler &hand (*i);
      if (hand.start)
	{
	  out->put2 (hand.start->get_pc ());
	  out->put2 (hand.end->get_pc ());
	  out->put2 (hand.target->get_pc ());
	  out->put2 (hand.type ? cpool->add (hand.type) : 0);
	}
    }

  // Attributes.
  attributes.emit (*out);

  // If requested, verify what we just wrote.
  if (global->get_compiler ()->target_verify ())
    {
      ref_bytecode_block block
	= new model_bytecode_block (method->get_location ());
      block->set_max_stack (max_stack);
      block->set_max_locals (vars.get_max ());
      const uint8 *ptr = (const uint8 *) out->get_bytes ();
      block->set_bytecode (end_offset - start_offset + 1,
			   ptr + start_offset);

      model_unit_class *unit
	= cpool->get_fake_compilation_unit (method->get_location (),
					    method->get_declaring_class ());

      // Catch exceptions here and print the result.  This will let us
      // still generate the .class file -- this is very useful for
      // debugging.
      try
	{
	  block->verify (method, unit);
	}
      catch (exception_base &exc)
	{
	  std::cerr << exc;
	}
    }
}

void
bytecode_generator::write_line_table (bytecode_stream *out)
{
  out->put2 (line_count);
  int last_line = -1;
  for (bytecode_block *work = first_block; work; work = work->next ())
    {
      if (work->get_line_number () != -1
	  && work->get_line_number () != last_line)
	{
	  last_line = work->get_line_number ();
	  --line_count;
	  out->put2 (work->get_pc ());
	  out->put2 (last_line);
	}
    }
  assert (line_count == 0);
}

bytecode_block *
bytecode_generator::new_bytecode_block ()
{
  bytecode_block *result = new bytecode_block ();
  allocated_blocks.push_back (result);
  return result;
}

void
bytecode_generator::note_line (model_element *elt)
{
  int old = current_block->get_line_number ();
  int new_line = elt->get_location ().get_line ();
  if (old == new_line)
    {
      // Nothing.
    }
  else
    {
      if (old != -1)
	define (new_bytecode_block ());
      current_block->set_line_number (new_line);
    }
}



void
bytecode_generator::emit_saved_cleanup (const finally_handler &handler)
{
  // Visit the block to generate the code again.  This might be
  // inefficient, but keeping copies of the emitted code and cloning
  // it turned out to be quite complicated, due to embedded finally
  // blocks.
  if (handler.block)
    handler.block->visit (this);
  else if (handler.variable != -1)
    {
      // 'NULL' represents the handler for a synchronized block.
      model_class *obj = global->get_compiler ()->java_lang_Object ();
      emit_load (obj, handler.variable);
      emit (op_monitorexit);
      reduce_stack (obj);
    }
}

void
bytecode_generator::call_cleanups (const model_stmt *upto)
{
  // Move up the finally stack, cloning the 'finally' clauses and
  // inserting them.  We could use 'jsr', if we wanted, but it is
  // deprecated in 1.5 and there are some simple programs that cause
  // ordinary java compilers to generate code that ordinary verifiers
  // will reject.  In theory switching to 'jsr' would only require
  // changes in this method and some minor changes to 'finally' code
  // generation in visit_try.
  if (finally_stack.empty ())
    return;

  // Because we re-emit the code for 'finally' whenever we need it, we
  // must arrange on each such occasion for the finally stack to
  // appear as it did just outside the 'try' statement.  So, we save a
  // copy of the finally stack and pop off elements as we go along.
  bool last_was_ours = false;
  fstackt save = finally_stack;
  while (! last_was_ours && ! finally_stack.empty ())
    {
      finally_handler h = finally_stack.back ();
      last_was_ours = h.statement == upto;
      // Exit early if we found a target statement.
      if (h.statement == upto && h.variable == -2)
	{
	  assert (h.block == NULL);
	  break;
	}
      finally_stack.pop_back ();
      emit_saved_cleanup (h);
    }
  assert (last_was_ours || upto == NULL);
  finally_stack = save;
}



void
bytecode_generator::visit_method (model_method *meth,
				  const std::list<ref_variable_decl> &params,
				  const ref_block &block)
{
  assert (method == meth);

  if (! meth->static_p ())
    {
      this_index = vars.request ((model_variable_decl *) NULL);
      // This must always be true.
      assert (! this_index);
    }
  for (std::list<ref_variable_decl>::const_iterator i = params.begin ();
       i != params.end ();
       ++i)
    vars.request ((*i).get ());

  block->visit (this);

  // We need to emit a trailing "return" if we can fall off the end of
  // the method.
  if (meth->get_return_type () == primitive_void_type
      && block->can_complete_normally ())
    {
      emit (op_return);
      non_normal_completion (false);
    }
}

void
bytecode_generator::visit_assert (model_assert *stmt,
				  const ref_expression &first,
				  const ref_expression &second)
{
  if (! global->get_compiler ()->target_assert ())
    return;

  note_line (stmt);

  // There's no point in the assert if the expression is 'true'.
  if (first->constant_p () && jboolean (first->value ()))
    return;

  model_field *disabled
    = method->get_declaring_class ()->add_assert_members ();
  bytecode_block *done (new_bytecode_block ());
  bytecode_block *fail (new_bytecode_block ());

  // Test $assertionsDisabled and skip past the assert if it is true.
  emit (op_getstatic);
  emit2 (cpool->add (disabled->get_declaring_class (), disabled));
  emit_relocation (ifne, done);

  {
    branch_targets push (this, done, fail);
    first->visit (this);
  }
  define (fail);

  model_class *errclass
    = global->get_compiler ()->java_lang_AssertionError ();
  emit (op_new);
  int kindex = cpool->add (errclass);
  emit2 (kindex);
  increase_stack (errclass);

  emit (op_dup);
  increase_stack (errclass);

  model_type *exprtype = NULL;
  if (second)
    {
      push_expr_target where (this, ON_STACK);
      second->visit (this);
      exprtype = second->type ();
      if (! exprtype->primitive_p ())
	exprtype = global->get_compiler ()->java_lang_Object ();
      reduce_stack (exprtype);
    }
  emit (op_invokespecial);
  reduce_stack (errclass);

  model_method *meth = find_method ("<init>", errclass, exprtype,
				    primitive_void_type, stmt);
  int mindex = cpool->add (meth->get_declaring_class (), meth);
  emit2 (mindex);
  emit (op_athrow);
  reduce_stack (errclass);
  non_normal_completion (false);
  define (done);
}

void
bytecode_generator::handle_block_statements (const std::list<ref_stmt> &statements)
{
  // Note that we don't have to worry about unreachable code, since an
  // unreachable statement is an error that prevents code generation.
  int save = stack_depth;
  for (std::list<ref_stmt>::const_iterator i = statements.begin ();
       i != statements.end ();
       ++i)
    {
      (*i)->visit (this);
      // Between statements there should be nothing additional on the
      // stack.  We can't assert that this is ==0, due to how we
      // handle 'finally' at the moment.
      assert (stack_depth == save);
    }
}

void
bytecode_generator::visit_block (model_block *block,
				 const std::list<ref_stmt> &statements)
{
  vars.push_scope (block);
  handle_block_statements (statements);
  vars.remove (block);
}

void
bytecode_generator::visit_bytecode_block (model_bytecode_block *block,
					  int, int, int, const uint8 *)
{
  throw block->error ("can't generate bytecode from a bytecode block");
}

void
bytecode_generator::visit_break (model_break *brk, const ref_stmt &target)
{
  note_line (brk);

  model_stmt *real_target = target.get ();
  if (! real_target)
    real_target = brk->get_target ();

  call_cleanups (target.get ());

  // Find the target statement.
  target_map_type::iterator iter = target_map.find (real_target);
  assert (iter != target_map.end ());
  bytecode_block *t_bytes = (*iter).second.second;

  emit_relocation (reloc_goto, t_bytes);
}

void
bytecode_generator::visit_catch (model_catch *stmt,
				 const ref_variable_decl &var,
				 const ref_block &body)
{
  vars.push_scope (stmt);
  int slot = vars.request (var.get ());

  // Always store the exception object into the local variable slot.
  increase_stack (var->type ());
  emit_store (var->type (), slot);

  body->visit (this);
  vars.remove (stmt);
}

void
bytecode_generator::visit_continue (model_continue *stmt,
				    const ref_stmt &target)
{
  note_line (stmt);

  call_cleanups (target.get ());

  // Find the target statement.
  target_map_type::iterator iter = target_map.find (target.get ());
  assert (iter != target_map.end ());
  bytecode_block *t_bytes = (*iter).second.first;

  emit_relocation (reloc_goto, t_bytes);
}

void
bytecode_generator::visit_class_decl_stmt (model_class_decl_stmt *,
					   const ref_class &)
{
  // Nothing to do here.
}

void
bytecode_generator::visit_do (model_do *dstmt,
			      const ref_expression &cond,
			      const ref_stmt &body)
{
  note_line (dstmt);

  bytecode_block *test (new_bytecode_block ());
  bytecode_block *done (new_bytecode_block ());
  target_map[dstmt] = std::make_pair (test, done);

  // Push a dummy finally handler that tells call_cleanups to exit
  // early.
  finally_handler dummy (dstmt);
  stack_temporary<finally_handler> push (finally_stack, dummy);

  bytecode_block *top (new_bytecode_block ());
  define (top);
  body->visit (this);
  define (test);

  {
    branch_targets push (this, top, done);
    cond->visit (this);
  }

  define (done);
}

void
bytecode_generator::visit_empty (model_empty *)
{
  // Nothing.
}

void
bytecode_generator::visit_expression_stmt (model_expression_stmt *stmt,
					   const ref_expression &expr)
{
  note_line (stmt);

  push_expr_target push (this, IGNORE);
  expr->visit (this);
}

void
bytecode_generator::visit_for_enhanced (model_for_enhanced *fstmt,
					const ref_stmt &body,
					const ref_expression &container,
					const ref_variable_decl &var)
{
// for (s : foo) { }
// dummy = foo.iterator(); while (dummy.hasNext(); )
//        s = (type) foo.next ();
// for (s : foo[]) { }
//  int dummy = 0; while (dummy < foo.length) ... ++dummy

  note_line (fstmt);

  vars.push_scope (fstmt);

  // Push a dummy finally handler that tells call_cleanups to exit
  // early.
  finally_handler dummy (fstmt);
  stack_temporary<finally_handler> push (finally_stack, dummy);

  // We introduce a new scope so that the temporary locals are killed
  // at the right point.
  {
    temporary_local user_var (vars, var.get ());
    temporary_local iter_var (vars, NULL);

    bytecode_block *cond (new_bytecode_block ());
    bytecode_block *done (new_bytecode_block ());

    {
      push_expr_target push (this, ON_STACK);
      container->visit (this);
    }
    if (container->type ()->array_p ())
      {
	class_writer::check_type (fstmt, container->type ());

	bytecode_block *update (new_bytecode_block ());
	temporary_local array_var (vars, NULL);
	temporary_local arraylen_var (vars, NULL);

	// Store the result of the container expression.
	emit_store (container->type (), array_var);

	// Now compute and cache the array length.
	// FIXME: could dup above to be more efficient.
	emit_load (container->type (), array_var);
	emit (op_arraylength);
	reduce_stack (container->type ());
	increase_stack (primitive_int_type);
	emit_store (primitive_int_type, arraylen_var);

	// Initialize the "iterator".
	emit (op_iconst_0);
	increase_stack (primitive_int_type);
	emit_store (primitive_int_type, iter_var);

	// We put the condition at the end of the loop, so start
	// there.
	emit_relocation (reloc_goto, cond);

	bytecode_block *again = new_bytecode_block ();
	define (again);

	emit_load (container->type (), array_var);
	emit_load (primitive_int_type, iter_var);
	emit (op_iaload
	      + adjust_for_type (container->type ()->element_type ()));
	reduce_stack (primitive_int_type);
	reduce_stack (container->type ());
	increase_stack (container->type ()->element_type ());

	// If needed, emit a cast to the iteration variable type.
	{
	  push_expr_target push (this, ON_STACK);
	  emit_cast_maybe_boxing (fstmt, var->type (),
				  container->type ()->element_type ());
	}
	emit_store (var->type (), user_var);

	target_map[fstmt] = std::make_pair (update, done);
	body->visit (this);

	// Increment the counter and go back to the top of the loop.
	define (update);
	emit (op_iinc);
	emit (iter_var);
	emit (1);

	define (cond);
	emit_load (primitive_int_type, iter_var);
	emit_load (primitive_int_type, arraylen_var);
	emit_relocation (if_icmplt, again);
	reduce_stack (primitive_int_type);
	reduce_stack (primitive_int_type);
      }
    else
      {
	model_class *iterator_type
	  = global->get_compiler ()->java_util_Iterator ();
	model_class *object_type
	  = global->get_compiler ()->java_lang_Object ();
	model_class *iterable_type
	  = global->get_compiler ()->java_lang_Iterable ();

	model_method *iter_meth
	  = find_method ("iterator", iterable_type, NULL,
			 iterator_type, fstmt);
	model_method *has_next_meth
	  = find_method ("hasNext", iterator_type, NULL,
			 primitive_boolean_type, fstmt);
	// FIXME: wrong for generics?
	model_method *next_meth
	  = find_method ("next", iterator_type, NULL, object_type,
			 fstmt);

	// Call the 'iterator' method on the expression.  FIXME: could
	// sometimes emit a non-interface call here.
	emit (op_invokeinterface);
	emit (1);
	emit (0);
	emit2 (cpool->add (iterable_type, iter_meth));
	reduce_stack (iterable_type);
	increase_stack (iterator_type);
	emit_store (iterator_type, iter_var);

	define (cond);

	// The test is an interface call to hasNext().
	emit_load (iterator_type, iter_var);

	emit (op_invokeinterface);
	emit (1);
	emit (0);
	emit2 (cpool->add (iterator_type, has_next_meth));
	reduce_stack (iterator_type);
	increase_stack (primitive_boolean_type);

	emit_relocation (ifeq, done);
	reduce_stack (primitive_boolean_type);
	target_map[fstmt] = std::make_pair (cond, done);

	emit_load (iterator_type, iter_var);

	// Now get the actual value and cast it to the correct type.
	emit (op_invokeinterface);
	emit (1);
	emit (0);
	emit2 (cpool->add (iterator_type, next_meth));
	reduce_stack (iterator_type);
	increase_stack (object_type);

	// Cast to the user's type.
	{
	  push_expr_target push (this, ON_STACK);
	  emit_cast_maybe_boxing (fstmt, var->type (),
				  fstmt->get_element_type ());
	}

	emit_store (var->type (), user_var);
	body->visit (this);
	emit_relocation (reloc_goto, cond);
      }

    define (done);
  }

  vars.remove (fstmt);
}

void
bytecode_generator::visit_for (model_for *fstmt,
			       const ref_stmt &init,
			       const ref_expression &cond,
			       const ref_stmt &update,
			       const ref_stmt &body)
{
  note_line (fstmt);

  // Push a dummy finally handler that tells call_cleanups to exit
  // early.
  finally_handler dummy (fstmt);
  stack_temporary<finally_handler> push (finally_stack, dummy);

  vars.push_scope (fstmt);

  if (init)
    init->visit (this);

  bytecode_block *body_bytes (new_bytecode_block ());
  bytecode_block *update_bytes (new_bytecode_block ());
  bytecode_block *done (new_bytecode_block ());
  target_map[fstmt] = std::make_pair (update_bytes, done);

  // If there is a condition, we put it at the end and start with a
  // 'goto' to the check.  If there is no condition, we have an
  // infinite loop, so we can fall through to the body.
  bytecode_block *cond_bytes = NULL;
  if (cond)
    {
      cond_bytes = new_bytecode_block ();
      emit_relocation (reloc_goto, cond_bytes);
    }

  define (body_bytes);
  body->visit (this);
  define (update_bytes);
  if (update)
    update->visit (this);

  if (cond)
    {
      define (cond_bytes);
      branch_targets push2 (this, body_bytes, done);
      cond->visit (this);
    }
  else
    {
      // No condition means infinite loop.
      emit_relocation (reloc_goto, body_bytes);
    }

  define (done);

  vars.remove (fstmt);
}

void
bytecode_generator::visit_if (model_if *stmt,
			      const ref_expression &cond,
			      const ref_stmt &true_branch,
			      const ref_stmt &false_branch)
{
  note_line (stmt);

  if (cond->constant_p ())
    {
      if (jboolean (cond->value ()))
	true_branch->visit (this);
      else if (false_branch)
	false_branch->visit (this);
      return;
    }

  bytecode_block *true_bytes (new_bytecode_block ());
  bytecode_block *false_bytes (new_bytecode_block ());
  bytecode_block *done (new_bytecode_block ());

  {
    branch_targets push (this, true_bytes, false_bytes);
    cond->visit (this);
  }

  define (true_bytes);
  true_branch->visit (this);
  // If TRUE_BRANCH can't complete normally, then the 'goto' here
  // would never be reached.
  if (false_branch && true_branch->can_complete_normally ())
    emit_relocation (reloc_goto, done);

  define (false_bytes);
  if (false_branch)
    false_branch->visit (this);
  define (done);
}

void
bytecode_generator::visit_label (model_label *label, const ref_stmt &stmt)
{
  note_line (label);

  // Push a dummy finally handler that tells call_cleanups to exit
  // early.
  finally_handler dummy (label);
  stack_temporary<finally_handler> push (finally_stack, dummy);

  // We might not know the target of a break statement at semantic
  // analysis time, so we compute it here.
  if (label->get_break_target () == NULL)
    {
      bytecode_block *brk = new_bytecode_block ();
      target_map[label] = std::make_pair ((bytecode_block *) NULL, brk);
      stmt->visit (this);
      define (brk);
    }
  else
    stmt->visit (this);
}

void
bytecode_generator::visit_return (model_return *rtn,
				  const ref_expression &expr)
{
  note_line (rtn);

  int tmpvar = -1;
  if (expr)
    {
      push_expr_target push (this, ON_STACK);
      expr->visit (this);

      // If there are cleanups to call, make a temporary variable and
      // assign to it, then reload it before the return.
      if (! finally_stack.empty ())
	{
	  tmpvar = vars.request ();
	  emit_store (expr->type (), tmpvar);
	}
    }

  call_cleanups (NULL);

  if (tmpvar != -1)
    {
      assert (expr);
      emit_load (expr->type (), tmpvar);
      vars.remove (tmpvar);
    }

  if (! expr)
    emit (op_return);
  else
    {
      model_type *t = expr->type ();
      emit (op_ireturn + adjust_for_type (t));
      reduce_stack (t);
    }
  non_normal_completion (true);
}

// This is just a helper to let us sort a list in ascending order.
static bool
compare (std::pair<jint, model_switch_block *> one,
	 std::pair<jint, model_switch_block *> two)
{
  return one.first < two.first;
}

void
bytecode_generator::visit_switch (model_switch *swstmt,
				  const ref_expression &expr,
				  const std::list<ref_switch_block> &blocks)
{
  note_line (swstmt);

  // Push a dummy finally handler that tells call_cleanups to exit
  // early.
  finally_handler dummy (swstmt);
  stack_temporary<finally_handler> push (finally_stack, dummy);

  bytecode_block *finish = new_bytecode_block ();

  // First create a destination label for each branch of the switch.
  // Also compute an index map, the minimum and maximum label values,
  // and the number of labels.
  int len = 0;
  std::vector< std::pair<jint, model_switch_block *> > indices;
  std::map<model_switch_block *, bytecode_block *> block_map;
  jint min, max;
  bool min_max_set = false;
  bytecode_block *default_dest = NULL;
  for (std::list<ref_switch_block>::const_iterator i = blocks.begin ();
       i != blocks.end ();
       ++i)
    {
      model_switch_block *entry = (*i).get ();
      bytecode_block *dest = new_bytecode_block ();
      block_map[entry] = dest;
      if (entry == swstmt->get_default ())
	default_dest = dest;

      std::list<ref_expression> label_list ((*i)->get_labels ());
      for (std::list<ref_expression>::const_iterator j
	     = label_list.begin ();
	   j != label_list.end ();
	   ++j)
	{
	  ++len;

	  ref_expression expr = *j;
	  // FIXME: redeclare primitive types so we can do this
	  // without a cast.
	  model_primitive_base *intb
	    = assert_cast<model_primitive_base *> (primitive_int_type);
	  jint value = jint (intb->convert (expr->type (), expr->value ()));
	  indices.push_back (std::make_pair (value, entry));

	  if (! min_max_set)
	    {
	      min = value;
	      max = value;
	      min_max_set = true;
	    }
	  else
	    {
	      if (value < min)
		min = value;
	      if (value > max)
		max = value;
	    }
	}
    }

  // Generate code for the switch expression.  We could optimize here
  // if the expression is a constant.  Do we care?
  {
    // Note that if there are no labels, we just evaluate for side
    // effects.
    push_expr_target push (this, len == 0 ? IGNORE : ON_STACK);
    expr->visit (this);
  }

  target_map[swstmt] = std::make_pair ((bytecode_block *) NULL, finish);

  // Note we could do more code-gen optimizations here.  For instance
  // if there is only a 'default' label we can just inline the body.
  // If there is a single label and a default we can generate if-then
  // (provided there is no fall-through).  Etc.
  if (len == 0)
    {
      // No switch labels.  If there is 'default', emit it.
      if (default_dest)
	{
	  define (default_dest);
	  model_switch_block *entry = blocks.front ().get ();
	  assert (entry == swstmt->get_default ());
	  entry->visit (this);
	}
    }
  else if (len == 1 && ! default_dest)
    {
      // A single label (and no 'default' label), so we can emit a
      // simple test: if expr == value { body }.
      ref_switch_block solo = blocks.front ();
      {
	push_expr_target push (this, ON_STACK);
	visit_simple_literal (NULL, min);
      }
      emit_relocation (if_icmpne, finish);
      reduce_stack (primitive_int_type);
      reduce_stack (primitive_int_type);
      handle_block_statements (solo->get_statements ());
    }
  else
    {
      // If there's no 'default' label, unmatched entries go to end of
      // switch.
      if (! default_dest)
	default_dest = finish;

      std::sort (indices.begin (), indices.end (), compare);

      // Choose tableswitch or lookupswitch depending on density of
      // table.  This heuristic comes from the previous gcj front end.
      unsigned long delta = max - min;
      if (2 * (unsigned long) len >= delta)
	{
	  // tableswitch
	  emit (op_tableswitch);
	  reduce_stack (primitive_int_type);
	  // Make a block for a switch alignment relocation.
	  define (new_bytecode_block ());
	  emit_relocation (switch_align, NULL);
	  emit_relocation (switch_table, default_dest);
	  emit4 (min);
	  emit4 (max);

	  jint where = min;
	  for (std::vector< std::pair<jint, model_switch_block *> >::const_iterator i
		 = indices.begin ();
	       i != indices.end ();
	       ++i)
	    {
	      // Fill in gaps with branch to 'default' label.
	      while (where < (*i).first)
		{
		  emit_relocation (switch_table, default_dest);
		  ++where;
		}
	      model_switch_block *this_block = (*i).second;
	      emit_relocation (switch_table, block_map[this_block]);
	      ++where;
	    }
	  assert (where == max + 1);
	}
      else
	{
	  // lookupswitch
	  emit (op_lookupswitch);
	  reduce_stack (primitive_int_type);
	  // Make a block for a switch alignment relocation.
	  define (new_bytecode_block ());
	  emit_relocation (switch_align, NULL);
	  // Default label.
	  emit_relocation (switch_table, default_dest);
	  // Number of pairs.
	  emit4 (len);

	  for (std::vector< std::pair<jint, model_switch_block *> >::const_iterator i
		 = indices.begin ();
	       i != indices.end ();
	       ++i)
	    {
	      emit4 ((*i).first);
	      model_switch_block *this_block = (*i).second;
	      emit_relocation (switch_table, block_map[this_block]);
	    }
	}

      // Now emit code for each branch.
      for (std::list<ref_switch_block>::const_iterator i = blocks.begin ();
	   i != blocks.end ();
	   ++i)
	{
	  model_switch_block *this_block = (*i).get ();
	  bytecode_block *dest = block_map[this_block];
	  // Define the branch's label and emit the code for it.
	  define (dest);
	  this_block->visit (this);
	  // We don't have to do anything special here, as the block
	  // might fall through; if there is a 'break' it is
	  // represented explicitly in the model.
	}
    }

  define (finish);
}

void
bytecode_generator::visit_switch_block (model_switch_block *,
					const std::list<ref_stmt> &statements)
{
  handle_block_statements (statements);
}

void
bytecode_generator::visit_synchronized (model_synchronized *sync,
					const ref_expression &expr,
					const ref_stmt &body)
{
  note_line (sync);

  // This is where we store the result of the expression.
  temporary_local sync_obj (vars, NULL);
  {
    push_expr_target push (this, ON_STACK);
    expr->visit (this);
  }
  emit_store (expr->type (), sync_obj);

  finally_handler finally_info (sync, NULL, sync_obj);

  // Start of the 'try'.
  bytecode_block *try_zone = new_bytecode_block ();
  // End of the 'try'.
  bytecode_block *try_end = new_bytecode_block ();
  // The 'finally' exception handler.
  bytecode_block *finally_exc = new_bytecode_block ();

  {
    // Note a finally block for 'break'.
    stack_temporary<finally_handler> push (finally_stack, finally_info);

    // Enter the monitor.
    define (try_zone);
    emit_load (expr->type (), sync_obj);
    emit (op_monitorenter);
    reduce_stack (expr->type ());

    body->visit (this);

    // Make sure we add this after we've visited the body, so that
    // embedded handlers are emitted in the correct order.
    add_exception_handler (NULL, finally_exc, try_zone, try_end);
  }

  // For the normal exit, inline the finally handler.
  bytecode_block *done = NULL;
  if (body->can_complete_normally ())
    {
      emit_saved_cleanup (finally_info);
      done = new_bytecode_block ();
      emit_relocation (reloc_goto, done);
    }

  // This might seem like strange placement, but the current thinking
  // is that a monitorexit failure should just keep on trying.  So,
  // the inlined finally handler is inside the try.
  define (try_end);

  // Emit the finally handler as an exception handler.
  define (finally_exc);
  model_type *thr = global->get_compiler ()->java_lang_Throwable ();
  increase_stack (thr);
  emit_saved_cleanup (finally_info);
  emit (op_athrow);
  reduce_stack (thr);
  non_normal_completion (false);

  if (done)
    define (done);
}

void
bytecode_generator::visit_throw (model_throw *stmt,
				 const ref_expression &expr)
{
  note_line (stmt);

  push_expr_target where (this, ON_STACK);
  expr->visit (this);
  emit (op_athrow);
  reduce_stack (expr->type ());
  non_normal_completion (true);
}

void
bytecode_generator::visit_try (model_try *trystmt,
			       const ref_block &body,
			       const std::list<ref_catch> &catchers,
			       const ref_block &finally)
{
  note_line (trystmt);

  finally_handler finally_info (trystmt, finally.get ());

  bytecode_block *try_zone = new_bytecode_block ();
  bytecode_block *try_end = new_bytecode_block ();
  bytecode_block *done = NULL;

  {
    // Push the finally code onto the finally stack, and remove it
    // from the main line of code.  We introduce a new scope here so
    // that the finally handler won't be around when we go to emit it
    // later on.

    // This doesn't much penalize us if there is no finally handler.
    // It would, though, if we were emitting 'jsr's.
    stack_temporary<finally_handler> push (finally_stack, finally_info);

    // Emit the 'try' clause.
    define (try_zone);
    body->visit (this);
    define (try_end);
    if (body->can_complete_normally ())
      {
	// We just emit one copy of the cleanup inline, and one copy
	// as an exception handler.
	done = new_bytecode_block ();
	emit_relocation (reloc_goto, done);
      }

    for (std::list<ref_catch>::const_iterator i = catchers.begin ();
	 i != catchers.end ();
	 ++i)
      {
	bytecode_block *handler = new_bytecode_block ();
	add_exception_handler ((*i)->get_parameter_type (), handler,
			       try_zone, try_end);
	define (handler);
	(*i)->visit (this);
	if ((*i)->can_complete_normally ())
	  {
	    if (! done)
	      done = new_bytecode_block ();
	    emit_relocation (reloc_goto, done);
	  }
      }
  }

  if (finally)
    {
      // Emit the finally clause as an exception handler.
      bytecode_block *finally_exc = new_bytecode_block ();
      add_exception_handler (NULL, finally_exc, try_zone, try_end);
      define (finally_exc);
      model_type *thr = global->get_compiler ()->java_lang_Throwable ();
      increase_stack (thr);
      temporary_local finally_obj (vars, NULL);
      emit_store (thr, finally_obj);
      emit_saved_cleanup (finally_info);
      emit_load (thr, finally_obj);
      emit (op_athrow);
      reduce_stack (thr);
      non_normal_completion (false);
    }

  if (done)
    {
      // If the try clause or one of the exception handlers got here,
      // we want to emit the finally handler inline.
      define (done);
      emit_saved_cleanup (finally_info);
    }
}

void
bytecode_generator::visit_variable_stmt (model_variable_stmt *stmt,
					 const std::list<ref_variable_decl> &decls)
{
  note_line (stmt);

  for (std::list<ref_variable_decl>::const_iterator i = decls.begin ();
       i != decls.end ();
       ++i)
    {
      int n = vars.request ((*i).get ());
      class_writer::check_type ((*i).get (), (*i)->type ());
      ref_expression init = (*i)->get_initializer ();
      if (init)
	{
	  push_expr_target where (this, ON_STACK);
	  init->visit (this);
	  emit_store ((*i)->type (), n);
	}
    }
}

void
bytecode_generator::visit_while (model_while *wstmt,
				 const ref_expression &cond,
				 const ref_stmt &body)
{
  note_line (wstmt);

  // Push a dummy finally handler that tells call_cleanups to exit
  // early.
  finally_handler dummy (wstmt);
  stack_temporary<finally_handler> push (finally_stack, dummy);

  bytecode_block *body_bytes = new_bytecode_block ();
  bytecode_block *again = new_bytecode_block ();
  bytecode_block *done = new_bytecode_block ();

  target_map[wstmt] = std::make_pair (again, done);

  emit_relocation (reloc_goto, again);

  define (body_bytes);
  body->visit (this);

  define (again);

  {  
    branch_targets push2 (this, body_bytes, done);
    cond->visit (this);
  }

  define (done);
}



int
bytecode_generator::adjust_for_type (model_type *t) const
{
  if (t == primitive_short_type || t == primitive_boolean_type
      || t == primitive_char_type || t == primitive_byte_type
      || t == primitive_int_type)
    return 0;
  else if (t == primitive_long_type)
    return 1;
  else if (t == primitive_float_type)
    return 2;
  else if (t == primitive_double_type)
    return 3;
  else
    {
      assert (! t->primitive_p ());
      return 4;
    }
}

int
bytecode_generator::adjust_for_type_full (model_type *t) const
{
  if (t == primitive_int_type)
    return 0;
  else if (t == primitive_long_type)
    return 1;
  else if (t == primitive_float_type)
    return 2;
  else if (t == primitive_double_type)
    return 3;
  else if (t == primitive_boolean_type
	   || t == primitive_byte_type)
    return 5;
  else if (t == primitive_char_type)
    return 6;
  else if (t == primitive_short_type)
    return 7;
  else
    {
      assert (! t->primitive_p ());
      return 4;
    }
}



void
bytecode_generator::visit_array_initializer (model_array_initializer *initx,
					     const ref_forwarding_type &elt_type,
					     const std::list<ref_expression> &exprs)
{
  assert (expr_target == ON_STACK);

  model_type *element_type = elt_type->type ();
  model_type *array_type = element_type->array ();
  class_writer::check_type (initx, array_type);

  visit_simple_literal (NULL, jint (exprs.size ()));
  emit_new_array (element_type);
  reduce_stack (primitive_int_type);
  increase_stack (array_type);

  int index = 0;
  for (std::list<ref_expression>::const_iterator i = exprs.begin ();
       i != exprs.end ();
       ++i)
    {
      emit (op_dup);
      increase_stack (array_type);
      visit_simple_literal (NULL, index++);
      (*i)->visit (this);
      emit (op_iastore + adjust_for_type_full (element_type));
      reduce_stack (element_type);
      reduce_stack (primitive_int_type);
      reduce_stack (array_type);
    }
}

void
bytecode_generator::visit_array_ref (model_array_ref *,
				     const ref_expression &array,
				     const ref_expression &index)
{
  {
    push_expr_target where (this, ON_STACK);
    array->visit (this);
    index->visit (this);
  }

  if (expr_target != LEFT_HAND_SIDE)
    {
      reduce_stack (array->type ());
      reduce_stack (index->type ());
      increase_stack (array->type ()->element_type ());
      emit (op_iaload
	    + adjust_for_type_full (array->type ()->element_type ()));

      if (expr_target == CONDITIONAL)
	{
	  emit_relocation (ifne, current_true_target);
	  emit_relocation (reloc_goto, current_false_target);
	  reduce_stack (primitive_boolean_type);
	}
      else if (expr_target == IGNORE)
	emit_pop (array->type ()->element_type ());
    }
  else
    {
      // Do nothing, just leave the items on the stack.
      lhs_info.kind = ARRAY;
      lhs_info.index = -1;
      lhs_info.type = array->type ();
      lhs_info.field = NULL;
    }
}

void
bytecode_generator::binary_arith_operator (java_opcode base,
					   const ref_expression &lhs,
					   const ref_expression &rhs)
{
  assert (expr_target == ON_STACK || expr_target == STRING
	  || expr_target == CONDITIONAL);
  {
    push_expr_target push (this, ON_STACK);
    lhs->visit (this);
    rhs->visit (this);
  }

  assert (lhs->type () == rhs->type ());
  emit (base + adjust_for_type (lhs->type ()));
  reduce_stack (lhs->type ());

  if (expr_target == CONDITIONAL)
    {
      emit_relocation (ifne, current_true_target);
      emit_relocation (reloc_goto, current_false_target);
      reduce_stack (primitive_boolean_type);
    }
}

void
bytecode_generator::visit_arith_binary (model_minus *,
					const ref_expression &lhs,
					const ref_expression &rhs)
{
  binary_arith_operator (op_isub, lhs, rhs);
}

void
bytecode_generator::visit_arith_binary (model_mult *,
					const ref_expression &lhs,
					const ref_expression &rhs)
{
  binary_arith_operator (op_imul, lhs, rhs);
}

void
bytecode_generator::visit_arith_binary (model_div *,
					const ref_expression &lhs,
					const ref_expression &rhs)
{
  binary_arith_operator (op_idiv, lhs, rhs);
}

void
bytecode_generator::visit_arith_binary (model_mod *,
					const ref_expression &lhs,
					const ref_expression &rhs)
{
  binary_arith_operator (op_irem, lhs, rhs);
}

void
bytecode_generator::visit_arith_binary (model_and *,
					const ref_expression &lhs,
					const ref_expression &rhs)
{
  binary_arith_operator (op_iand, lhs, rhs);
}

void
bytecode_generator::visit_arith_binary (model_or *,
					const ref_expression &lhs,
					const ref_expression &rhs)
{
  binary_arith_operator (op_ior, lhs, rhs);
}

void
bytecode_generator::visit_arith_binary (model_xor *,
					const ref_expression &lhs,
					const ref_expression &rhs)
{
  binary_arith_operator (op_ixor, lhs, rhs);
}

model_class *
bytecode_generator::get_stringbuffer_class ()
{
  compiler *comp = global->get_compiler ();
  return (comp->target_15 () ? comp->java_lang_StringBuilder ()
	  : comp->java_lang_StringBuffer ());
}

void
bytecode_generator::create_stringbuffer (model_type *arg_type,
					 model_element *request)
{
  model_class *sbclass = get_stringbuffer_class ();

  // Create a new StringBuffer and leave a reference to it on
  // the stack.
  emit (op_new);
  emit2 (cpool->add (sbclass));
  increase_stack (sbclass);

  // This ensures that there is another reference to this
  // StringBuffer on the stack after we've initialized it.
  // If we have an argument, change [...arg new] to [...new new arg]
  emit (arg_type ? op_dup_x1 : op_dup);
  increase_stack (sbclass);
  if (arg_type)
    emit (op_swap);

  // FIXME: we could call <init>(String) if we discovered the
  // type of the left hand side, and if we knew it wasn't an
  // operator+.
  model_method *init = find_method ("<init>", sbclass, arg_type,
				    primitive_void_type, request);
  emit (op_invokespecial);
  emit2 (cpool->add (init->get_declaring_class (), init));
  reduce_stack (sbclass);
  if (arg_type)
    reduce_stack (arg_type);
}

void
bytecode_generator::close_stringbuffer (model_type *rhs_type,
					model_element *request)
{
  model_class *sbclass = get_stringbuffer_class ();
  model_type *strtype = global->get_compiler ()->java_lang_String ();

  // Note that append always leaves the StringBuffer on the
  // stack again.
  emit (op_invokevirtual);
  model_method *meth = find_method ("append", sbclass, rhs_type, sbclass,
				    request);
  int mindex = cpool->add (meth->get_declaring_class (), meth);
  emit2 (mindex);
  reduce_stack (sbclass);
  reduce_stack (rhs_type);
  increase_stack (sbclass);
  // Make sure to clear this when closing, so that callers
  // will not be confused.
  string_plus_type = NULL;

  // If we created the StringBuffer, then we will also close it.
  emit (op_invokevirtual);
  meth = find_method ("toString", sbclass, NULL, strtype, request);
  mindex = cpool->add (meth->get_declaring_class (), meth);
  emit2 (mindex);
  reduce_stack (sbclass);
  increase_stack (strtype);
}

void
bytecode_generator::visit_arith_binary (model_plus *model,
					const ref_expression &lhs,
					const ref_expression &rhs)
{
  if (model->type ()->primitive_p ())
    binary_arith_operator (op_iadd, lhs, rhs);
  else
    {
      // if no StringBuffer allocated, make one
      // ... or add lhs to existing one
      // if rhs is + operator with string type, leave StringBuffer
      // open when recursing

      model_type *lhs_type = lhs->type ();
      model_type *rhs_type = rhs->type ();

      model_class *sbclass = get_stringbuffer_class ();
      model_type *strtype = global->get_compiler ()->java_lang_String ();

      // If expr_target is STRING, then we have inherited a
      // StringBuffer and can find a reference to it on the stack.
      bool creating_buffer = expr_target != STRING;

      if (creating_buffer)
	create_stringbuffer (NULL, model);

      // We've either inherited or created a StringBuffer.  In either
      // case, a reference to it appears on the stack.
      {
	push_expr_target push (this, STRING);

	if (lhs_type == primitive_byte_type
	    || lhs_type == primitive_short_type)
	  lhs_type = primitive_int_type;
	else if (! lhs_type->primitive_p () && lhs_type != strtype)
	  lhs_type = global->get_compiler ()->java_lang_Object ();

	// The left hand side might itself be a String '+' operation,
	// in which case we will simply re-use this StringBuffer.  In
	// this situation, the right hand side of the sub-expression
	// will be left on the stack, and its type will end up in
	// string_plus_type.  So, if string_plus_type is set when
	// evaluating the left hand side, we use its type rather than
	// the actual type of the left hand side when considering our
	// next step.  A similar situation occurs below for the right
	// hand side, except that in that case we might also simply
	// leave the result on the stack rather than append it to the
	// StringBuffer.
	if (lhs->constant_p () && lhs->string_value () == "")
	  {
	    // Do nothing.  It is a common idiom to write ""+x, so we
	    // optimize away the empty string.
	  }
	else
	  {
	    string_plus_type = NULL;
	    lhs->visit (this);
	    if (string_plus_type)
	      lhs_type = string_plus_type;
	    emit (op_invokevirtual);
	    model_method *meth = find_method ("append", sbclass, lhs_type,
					      sbclass, model);
	    int mindex = cpool->add (meth->get_declaring_class (), meth);
	    emit2 (mindex);
	    reduce_stack (sbclass);
	    reduce_stack (lhs_type);
	    increase_stack (sbclass);
	  }

	// Stack: ... BUFFER
	if (rhs_type == primitive_byte_type
	    || rhs_type == primitive_short_type)
	  rhs_type = primitive_int_type;
	else if (! rhs_type->primitive_p () && rhs_type != strtype)
	  rhs_type = global->get_compiler ()->java_lang_Object ();
	string_plus_type = NULL;
	rhs->visit (this);
	if (string_plus_type)
	  rhs_type = string_plus_type;
      }

      // If we didn't create this StringBuffer, then we just leave
      // this object on the stack and let our caller -- which is
      // also a 'String +' -- handle calling append for us.
      if (creating_buffer)
	close_stringbuffer (rhs_type, rhs.get ());
      else
	string_plus_type = rhs_type;

      if (expr_target == IGNORE)
	emit_pop (strtype);
    }
}

void
bytecode_generator::arith_shift (java_opcode base,
				 const ref_expression &lhs,
				 const ref_expression &rhs)
{
  assert (expr_target == ON_STACK || expr_target == STRING);
  {
    push_expr_target push (this, ON_STACK);
    lhs->visit (this);
    // FIXME: sometimes people write '<< 0' to make source code line
    // up nicely.  We could optimize this case.
    rhs->visit (this);
  }

  emit_cast (primitive_int_type, rhs->type ());

  emit (base + adjust_for_type (lhs->type ()));
  reduce_stack (primitive_int_type);
}

void
bytecode_generator::visit_arith_shift (model_left_shift *,
				       const ref_expression &lhs,
				       const ref_expression &rhs)
{
  arith_shift (op_ishl, lhs, rhs);
}

void
bytecode_generator::visit_arith_shift (model_right_shift *,
				       const ref_expression &lhs,
				       const ref_expression &rhs)
{
  arith_shift (op_ishr, lhs, rhs);
}

void
bytecode_generator::visit_arith_shift (model_unsigned_right_shift *,
				       const ref_expression &lhs,
				       const ref_expression &rhs)
{
  arith_shift (op_iushr, lhs, rhs);
}

void
bytecode_generator::dereference_left_hand_side (left_hand_side_info *info)
{
  // This actually duplicates and dereferences the left hand side of
  // an assignment.
  switch (info->kind)
    {
    case ARRAY:
      emit (op_dup2);
      increase_stack (info->type);
      increase_stack (primitive_int_type);
      emit (op_iaload
	    + adjust_for_type_full (info->type->element_type ()));
      reduce_stack (primitive_int_type);
      reduce_stack (info->type);
      increase_stack (info->type->element_type ());
      break;
    case INSTANCE_FIELD:
      emit (op_dup);
      increase_stack (info->type);
      emit (op_getfield);
      emit2 (info->index);
      reduce_stack (info->type);
      increase_stack (info->field->type ());
      break;
    case VARIABLE:
      emit_load (info->type, info->index);
      break;
    case STATIC_FIELD:
      emit (op_getstatic);
      emit2 (info->index);
      increase_stack (info->field->type ());
      break;
    case METHOD_CALL:
      {
	model_class *k = dynamic_cast<model_class *> (info->type);
	model_method *access = k->get_accessor (info->field, false);
	// First duplicate.
	if (! info->field->static_p ())
	  {
	    emit (op_dup);
	    increase_stack (k);
	  }
	// Now dereference.
	assert (access->static_p ());
	emit (op_invokestatic);
	emit2 (cpool->add (k, access));
	if (! info->field->static_p ())
	  reduce_stack (k);
	increase_stack (access->get_return_type ());
      }
      break;
    }
}

void
bytecode_generator::duplicate_lhs_value (left_hand_side_info *info,
					 model_type *rhs_type)
{
  switch (info->kind)
    {
    case ARRAY:
      // We have  :  ... array index value
      // Change to:  ... value array index value
      emit (wide_p (rhs_type) ? op_dup2_x2 : op_dup_x2);
      increase_stack (info->type->element_type ());
      break;
    case INSTANCE_FIELD:
      // We have  :  ... object value
      // Change to:  ... value object value
      emit (wide_p (rhs_type) ? op_dup2_x1 : op_dup_x1);
      increase_stack (info->field->type ());
      break;
    case STATIC_FIELD:
    case VARIABLE:
      // Just duplicate the value.
      emit (wide_p (rhs_type) ? op_dup2 : op_dup);
      increase_stack (rhs_type);
      break;
    case METHOD_CALL:
      // For static fields, we just duplicate the value.
      // For non-static fields, we have: ... object value
      // and we want ... value object value
      if (info->field->static_p ())
	{
	  emit (wide_p (rhs_type) ? op_dup2 : op_dup);
	  increase_stack (rhs_type);
	}
      else
	{
	  emit (wide_p (rhs_type) ? op_dup2_x1 : op_dup_x1);
	  increase_stack (info->field->type ());
	}
      break;
    }
}

void
bytecode_generator::emit_lhs_store (left_hand_side_info *info,
				    model_type *rhs_type)
{
  switch (info->kind)
    {
    case ARRAY:
      // We have  :  ... array index value
      emit (op_iastore + adjust_for_type_full (rhs_type));
      reduce_stack (rhs_type);
      reduce_stack (primitive_int_type);
      reduce_stack (info->type);
      break;
    case INSTANCE_FIELD:
      // We have  :  ... object value
      emit (op_putfield);
      emit2 (info->index);
      reduce_stack (rhs_type);
      reduce_stack (info->type);
      break;
    case STATIC_FIELD:
      emit (op_putstatic);
      emit2 (info->index);
      reduce_stack (rhs_type);
      break;
    case VARIABLE:
      emit_store (rhs_type, info->index);
      break;
    case METHOD_CALL:
      {
	model_class *k = dynamic_cast<model_class *> (info->type);
	model_method *access = k->get_accessor (info->field, true);
	assert (access->static_p ());
	emit (op_invokestatic);
	emit2 (cpool->add (k, access));
	if (! info->field->static_p ())
	  reduce_stack (k);
	reduce_stack (rhs_type);
      }
      break;
    }
}

void
bytecode_generator::handle_op_assignment (java_opcode base,
					  const ref_expression &lhs,
					  const ref_expression &rhs)
{
  assert (expr_target == ON_STACK || expr_target == STRING
	  || expr_target == IGNORE || expr_target == CONDITIONAL);

  // Compute the left hand side.
  {
    push_expr_target where (this, LEFT_HAND_SIDE);
    lhs->visit (this);
  }

  // We save this now because evaluating the rhs may cause another
  // assignment to be emitted.
  left_hand_side_info local_info = lhs_info;

  bool is_shift = (base == op_ishl || base == op_ishr || base == op_iushr);

  // If we're processing op=, then get the value here.
  model_type *strtype = global->get_compiler ()->java_lang_String ();
  bool string_add = false;
  if (base != op_nop)
    {
      dereference_left_hand_side (&local_info);

      if (lhs->type () == strtype)
	{
	  // If we're processing String '+=', then we create a new
	  // StringBuffer with the left hand side as the its initial
	  // contents.
	  assert (base == op_iadd);
	  create_stringbuffer (strtype, lhs.get ());
	  string_add = true;
	}
      else if (lhs->type () != rhs->type ())
	{
	  // This means that the RHS was promoted, so we must promote
	  // the LHS here and then cast at the end.  However, this is
	  // not the case for shift operators, where the LHS and RHS
	  // don't undergo binary numeric promotion.
	  if (! is_shift)
	    emit_cast (rhs->type (), lhs->type ());
	}
    }

  // Compute the right hand side.
  {
    string_plus_type = NULL;
    push_expr_target where (this, string_add ? STRING : ON_STACK);
    rhs->visit (this);
  }

  // Emit the operation.
  model_type *rhs_type = rhs->type ();
  if (string_add)
    {
      if (string_plus_type != NULL)
	rhs_type = string_plus_type;
      else if (rhs_type == primitive_byte_type
	       || rhs_type == primitive_short_type)
	rhs_type = primitive_int_type;
      else if (! rhs_type->primitive_p () && rhs_type != strtype)
	rhs_type = global->get_compiler ()->java_lang_Object ();
      close_stringbuffer (rhs_type, rhs.get ());
      rhs_type = strtype;
    }
  else if (base != op_nop)
    {
      // For shift operators, we have to make sure the right hand side
      // is a plain 'int'.
      model_type *override_rhs_type = rhs_type;
      if (is_shift)
	{
	  emit_cast (primitive_int_type, rhs_type);
	  override_rhs_type = primitive_int_type;
	}

      emit (base + adjust_for_type (is_shift ? lhs->type () : rhs->type ()));
      reduce_stack (override_rhs_type);

      // Cast back to the result type if required.  Note that we
      // switch back to using 'rhs_type'.  Note also the special
      // treatment of shift operators, as above.
      if (lhs->type () != rhs_type && ! is_shift)
	{
	  emit_cast (lhs->type (), rhs_type);
	  // The result of the operation has the type of the left hand
	  // side.
	  rhs_type = lhs->type ();
	}
    }

  // If we need the resulting value, we keep a copy and rearrange the
  // stack to suit.
  if (expr_target != IGNORE)
    duplicate_lhs_value (&local_info, is_shift ? lhs->type () : rhs_type);

  // Emit the store.
  emit_lhs_store (&local_info, is_shift ? lhs->type () : rhs_type);

  if (expr_target == CONDITIONAL)
    {
      emit_relocation (ifne, current_true_target);
      emit_relocation (reloc_goto, current_false_target);
      reduce_stack (primitive_boolean_type);
    }
}

void
bytecode_generator::visit_assignment (model_assignment *,
				      const ref_expression &lhs,
				      const ref_expression &rhs)
{
  handle_op_assignment (op_nop, lhs, rhs);
}

void
bytecode_generator::visit_op_assignment (model_minus_equal *,
					 const ref_expression &lhs,
					 const ref_expression &rhs)
{
  handle_op_assignment (op_isub, lhs, rhs);
}

void
bytecode_generator::visit_op_assignment (model_mult_equal *,
					 const ref_expression &lhs,
					 const ref_expression &rhs)
{
  handle_op_assignment (op_imul, lhs, rhs);
}

void
bytecode_generator::visit_op_assignment (model_div_equal *,
					 const ref_expression &lhs,
					 const ref_expression &rhs)
{
  handle_op_assignment (op_idiv, lhs, rhs);
}

void
bytecode_generator::visit_op_assignment (model_and_equal *,
					 const ref_expression &lhs,
					 const ref_expression &rhs)
{
  handle_op_assignment (op_iand, lhs, rhs);
}

void
bytecode_generator::visit_op_assignment (model_or_equal *,
					 const ref_expression &lhs,
					 const ref_expression &rhs)
{
  handle_op_assignment (op_ior, lhs, rhs);
}

void
bytecode_generator::visit_op_assignment (model_xor_equal *,
					 const ref_expression &lhs,
					 const ref_expression &rhs)
{
  handle_op_assignment (op_ixor, lhs, rhs);
}

void
bytecode_generator::visit_op_assignment (model_mod_equal *,
					 const ref_expression &lhs,
					 const ref_expression &rhs)
{
  handle_op_assignment (op_irem, lhs, rhs);
}

void
bytecode_generator::visit_op_assignment (model_ls_equal *,
					 const ref_expression &lhs,
					 const ref_expression &rhs)
{
  handle_op_assignment (op_ishl, lhs, rhs);
}

void
bytecode_generator::visit_op_assignment (model_rs_equal *,
					 const ref_expression &lhs,
					 const ref_expression &rhs)
{
  handle_op_assignment (op_ishr, lhs, rhs);
}

void
bytecode_generator::visit_op_assignment (model_urs_equal *,
					 const ref_expression &lhs,
					 const ref_expression &rhs)
{
  handle_op_assignment (op_iushr, lhs, rhs);
}

void
bytecode_generator::visit_op_assignment (model_plus_equal *,
					 const ref_expression &lhs,
					 const ref_expression &rhs)
{
  handle_op_assignment (op_iadd, lhs, rhs);
}

model_field *
bytecode_generator::find_field (const std::string &name,
                                model_class *klass, model_type *type,
                                model_element *request)
{
  std::list<ref_field> fields = klass->get_fields ();
  for (std::list<ref_field>::const_iterator it = fields.begin ();
       it != fields.end ();
       ++it)
    {
      model_field *field = (*it).get ();
      if (field->get_name () == name && field->type ()->erasure () == type)
        return field;
    }
  throw request->error ("couldn't find field %1 of type %2 in class %3")
    % name % type % klass;
}

model_method *
bytecode_generator::find_method (const char *mname, model_class *klass,
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
      if (var->type ()->erasure () == argtype)
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

  if (result->get_return_type ()->erasure () != result_type)
    {
      throw request->error ("method %1 doesn't have expected return type"
			    " of %2")
	% result % result_type;
    }
  return result;
}

void
bytecode_generator::emit_cast (model_type *dest,
			       model_type *orig)
{
  if (dest->primitive_p ())
    {
      assert (orig->primitive_p ());

      // Promotion.
      if (orig == primitive_boolean_type || orig == primitive_byte_type
	  || orig == primitive_char_type || orig == primitive_short_type)
	orig = primitive_int_type;
      // No-op cast.
      if (dest == orig)
	return;

      // When casting from, e.g., float to byte, we must go via 'int'.
      model_type *inter = dest;
      if (inter == primitive_boolean_type || inter == primitive_byte_type
	  || inter == primitive_char_type || inter == primitive_short_type)
	inter = primitive_int_type;
      // We do some goofy computation here to avoid writing things out
      // by hand.
      if (orig != inter)
	{
	  // First emit the intermediate cast, eg 'd2i'.
	  int source_index = adjust_for_type (orig);
	  int inter_index = adjust_for_type (inter);
	  int offset = 3 * source_index + inter_index;
	  if (inter_index > source_index)
	    --offset;
	  emit (op_i2l + offset);
	  reduce_stack (orig);
	  increase_stack (inter);
	}

      if (inter != dest)
	{
	  // Now emit the final cast, eg 'i2b'.
	  // '5' is the type adjustment for 'byte'.
	  emit (op_i2b - 5 + adjust_for_type_full (dest));
	  reduce_stack (inter);
	  increase_stack (dest);
	}
    }
  else
    {
      // If the source is already assignable to the destination, then
      // we don't need to emit a cast check.
      if (! dest->assignable_from_p (orig))
	{
	  emit (op_checkcast);
	  int index = cpool->add (dest);
	  emit2 (index);
	}
    }
}

void
bytecode_generator::emit_cast_maybe_boxing (model_element *request,
					    model_type *dest_type,
					    model_type *expr_type)
{
  if (dest_type->primitive_p () != expr_type->primitive_p ())
    {
      if (dest_type->primitive_p ())
	{
	  // Unboxing conversion.  Call <type>Value() on the wrapper
	  // object, e.g. for Integer we call intValue().  Using
	  // get_pretty_name here is a bit of an abuse.  Note that
	  // Character doesn't have all the methods from Number, so we
	  // need a special case here.
	  model_type *tmp_dest_type = dest_type;
	  if (expr_type == global->get_compiler ()->java_lang_Character ())
	    tmp_dest_type = primitive_char_type;
	  std::string method_name = (tmp_dest_type->get_pretty_name ()
				     + "Value");
	  model_method *call
	    = find_method (method_name.c_str (),
			   assert_cast<model_class *> (expr_type),
			   NULL, tmp_dest_type, request);
	  std::list<ref_expression> args;
	  handle_invocation (op_invokevirtual, call->get_declaring_class (),
			     call, args);
	  if (tmp_dest_type != dest_type)
	    {
	      assert (tmp_dest_type == primitive_char_type);
	      emit_cast (dest_type, primitive_char_type);
	    }
	}
      else
	{
	  // Boxing conversion.  We call the static factory method
	  // valueOf(), which handles the caching required by boxing
	  // conversion.
	  model_class *dest_class = assert_cast<model_class *> (dest_type);
	  model_method *call = find_method ("valueOf", dest_class,
					    expr_type, dest_class,
					    request);
	  // We do pass an argument, but sneakily: we pushed it up
	  // above.
	  std::list<ref_expression> args;
	  handle_invocation (op_invokestatic, dest_class, call, args);
	  // We have to pop this ourselves due to lying to
	  // handle_invocation.
	  reduce_stack (expr_type);
	}
    }
  else
    {
      class_writer::check_type (request, dest_type);
      emit_cast (dest_type, expr_type);
      if (expr_target == IGNORE)
	{
	  // This can only happen when accessing a static member with
	  // a non-static left hand side.
	  assert (dest_type->reference_p ());
	  emit_pop (dest_type);
	}
    }
}

void
bytecode_generator::visit_cast (model_cast *cast_expr,
				const ref_forwarding_type &dest,
				const ref_expression &expr)
{
  assert (expr_target == ON_STACK || expr_target == STRING
	  || expr_target == CONDITIONAL || expr_target == IGNORE);
  {
    push_expr_target push (this, (expr_target == CONDITIONAL
				  ? CONDITIONAL
				  : ON_STACK));
    expr->visit (this);
  }

  emit_cast_maybe_boxing (cast_expr, dest->type (), expr->type ());
}

void
bytecode_generator::visit_class_ref (model_class_ref *ref,
				     const ref_forwarding_type &req)
{
  assert (expr_target == ON_STACK || expr_target == STRING
	  || expr_target == IGNORE);

  model_type *type = req->type ();
  class_writer::check_type (ref, type);
  if (type->primitive_p () || type == primitive_void_type)
    {
      model_class *wrapper = box_primitive_type (type);
      model_field *field
	= find_field ("TYPE", wrapper,
		      global->get_compiler ()->java_lang_Class (), ref);
      emit (op_getstatic);
      emit2 (cpool->add (wrapper, field));
    }
  else if (global->get_compiler ()->target_15 ())
    {
      int index = cpool->add (type);
      emit_ldc (index);
    }
  else
    {
      // Emit a call to 'class$ (String, bool)'; see class.cc to
      // understand the arguments.
      model_method *class_
	= method->get_declaring_class ()->add_class_members ();
      bool isarray = type->array_p ();
      if (! isarray)
	type = type->array ();

      model_class *strtype = global->get_compiler ()->java_lang_String ();
      increase_stack (strtype);
      increase_stack (primitive_int_type);

      // Strangely, Class.forName doesn't take a normal descriptor.
      // It expects qualified names to contain dots rather than slashes.
      std::string forName_class_name (type->get_descriptor ());
      replace_all (forName_class_name, '/', '.');

      emit_ldc (cpool->add (forName_class_name));
      emit (isarray ? op_iconst_1 : op_iconst_0);
      emit (op_invokestatic);
      emit2 (cpool->add (class_->get_declaring_class (), class_));

      reduce_stack (primitive_int_type);
      reduce_stack (strtype);
    }

  increase_stack (global->get_compiler ()->java_lang_Class ());

  // In the IGNORE case we must still evaluate the left hand side
  // since it might throw an exception if the class is not found.
  if (expr_target == IGNORE)
    emit_pop (global->get_compiler ()->java_lang_Class ());
}

void
bytecode_generator::full_comparison (relocation_kind base,
				     const ref_expression &lhs,
				     const ref_expression &rhs,
				     bytecode_block *true_targ,
				     bytecode_block *false_targ)
{
  {
    push_expr_target push (this, ON_STACK);
    lhs->visit (this);
    rhs->visit (this);
  }
  emit_relocation (base, true_targ);
  emit_relocation (reloc_goto, false_targ);

  reduce_stack (rhs->type ());
  reduce_stack (lhs->type ());
}

void
bytecode_generator::handle_comparison (java_opcode base,
				       const ref_expression &lhs,
				       const ref_expression &rhs)
{
  assert (expr_target == ON_STACK || expr_target == STRING
	  || expr_target == CONDITIONAL);

  bytecode_block *local_true = NULL;
  bytecode_block *local_false = NULL;
  if (expr_target == ON_STACK || expr_target == STRING)
    {
      local_true = new_bytecode_block ();
      local_false = new_bytecode_block ();
    }
  else
    {
      local_true = current_true_target;
      local_false = current_false_target;
    }

  if ((rhs->type ()->integral_p () && rhs->type () != primitive_long_type)
      || rhs->type () == primitive_boolean_type)
    {
      // FIXME: redeclare primitive types so we can do this
      // without a cast.
      model_primitive_base *intb
	= assert_cast<model_primitive_base *> (primitive_int_type);

      if (lhs->constant_p ()
	  && jint (intb->convert (lhs->type (), lhs->value ())) == 0)
	{
	  base = invert_if_opcode (zero_if_opcode (base));
	  {
	    push_expr_target push (this, ON_STACK);
	    rhs->visit (this);
	  }
	  emit_relocation (relocation_kind (base), local_true);
	  emit_relocation (reloc_goto, local_false);
	  reduce_stack (lhs->type ());
	}
      else if (rhs->constant_p ()
	       && jint (intb->convert (rhs->type (), rhs->value ())) == 0)
	{
	  base = zero_if_opcode (base);
	  {
	    push_expr_target push (this, ON_STACK);
	    lhs->visit (this);
	  }
	  emit_relocation (relocation_kind (base), local_true);
	  emit_relocation (reloc_goto, local_false);
	  reduce_stack (lhs->type ());
	}
      else
	full_comparison (relocation_kind (base), lhs, rhs,
			 local_true, local_false);
    }
  else if (rhs->type ()->numeric_p ())
    {
      java_opcode cmp;
      if (rhs->type () == primitive_long_type)
	cmp = op_lcmp;
      else
	{
	  if (base == op_if_icmplt || base == op_if_icmple)
	    cmp = op_fcmpg;
	  else
	    cmp = op_fcmpl;
	  if (wide_p (rhs->type ()))
	    {
	      // The '2' is the type adjustment for float.
	      cmp = java_opcode (cmp + 2);
	    }
	}

      {
	push_expr_target push (this, ON_STACK);
	lhs->visit (this);
	rhs->visit (this);
      }
      emit (cmp);
      reduce_stack (rhs->type ());
      reduce_stack (lhs->type ());

      base = zero_if_opcode (base);
      emit_relocation (relocation_kind (base), local_true);
      emit_relocation (reloc_goto, local_false);
    }
  else
    {
      assert (! rhs->type ()->primitive_p ());
      assert (base == op_if_icmpeq || base == op_if_icmpne);
      if (lhs->type () == null_type)
	{
	  base = java_opcode (op_ifnull + base - op_if_icmpeq);
	  {
	    push_expr_target push (this, ON_STACK);
	    rhs->visit (this);
	  }
	  emit_relocation (relocation_kind (base), local_true);
	  emit_relocation (reloc_goto, local_false);
	  reduce_stack (rhs->type ());
	}
      else if (rhs->type () == null_type)
	{
	  base = java_opcode (op_ifnull + base - op_if_icmpeq);
	  {
	    push_expr_target push (this, ON_STACK);
	    lhs->visit (this);
	  }
	  emit_relocation (relocation_kind (base), local_true);
	  emit_relocation (reloc_goto, local_false);
	  reduce_stack (lhs->type ());
	}
      else
	{
	  base = java_opcode (op_if_acmpeq + base - op_if_icmpeq);
	  full_comparison (relocation_kind (base), lhs, rhs,
			   local_true, local_false);
	}
    }

  if (expr_target == ON_STACK || expr_target == STRING)
    {
      // FIXME: should assert that the previous block can't fall
      // through to here.
      bytecode_block *done = new_bytecode_block ();
      // It's a little easier to optimize if we generate the false
      // target first.
      define (local_false);
      emit (op_iconst_0);
      emit_relocation (reloc_goto, done);
      define (local_true);
      emit (op_iconst_1);
      define (done);
      increase_stack (primitive_int_type);
    }
}

void
bytecode_generator::visit_comparison (model_equal *,
				      const ref_expression &lhs,
				      const ref_expression &rhs)
{
  handle_comparison (op_if_icmpeq, lhs, rhs);
}

void
bytecode_generator::visit_comparison (model_notequal *,
				      const ref_expression &lhs,
				      const ref_expression &rhs)
{
  handle_comparison (op_if_icmpne, lhs, rhs);
}

void
bytecode_generator::visit_comparison (model_lessthan *,
				      const ref_expression &lhs,
				      const ref_expression &rhs)
{
  handle_comparison (op_if_icmplt, lhs, rhs);
}

void
bytecode_generator::visit_comparison (model_greaterthan *,
				      const ref_expression &lhs,
				      const ref_expression &rhs)
{
  handle_comparison (op_if_icmpgt, lhs, rhs);
}

void
bytecode_generator::visit_comparison (model_lessthanequal *,
				      const ref_expression &lhs,
				      const ref_expression &rhs)
{
  handle_comparison (op_if_icmple, lhs, rhs);
}

void
bytecode_generator::visit_comparison (model_greaterthanequal *,
				      const ref_expression &lhs,
				      const ref_expression &rhs)
{
  handle_comparison (op_if_icmpge, lhs, rhs);
}

void
bytecode_generator::handle_valueof (model_type *t, model_element *request)
{
  model_type *argt = t;
  if (t->reference_p ())
    argt = global->get_compiler ()->java_lang_Object ();
  model_class *strtype = global->get_compiler ()->java_lang_String ();
  model_method *value = find_method ("valueOf", strtype, argt, strtype,
				     request);
  assert (value);

  emit (op_invokestatic);
  emit2 (cpool->add (value->get_declaring_class (), value));
  reduce_stack (t);
  increase_stack (strtype);
}

void
bytecode_generator::visit_conditional (model_conditional *cond_obj,
				       const ref_expression &cond,
				       const ref_expression &true_branch,
				       const ref_expression &false_branch)
{
  assert (expr_target == CONDITIONAL || expr_target == ON_STACK
	  || expr_target == STRING || expr_target == IGNORE);

  bytecode_block *true_bytes = new_bytecode_block ();
  bytecode_block *false_bytes = new_bytecode_block ();

  {
    branch_targets push (this, true_bytes, false_bytes);
    cond->visit (this);
  }
  define (true_bytes);

  if (expr_target == CONDITIONAL)
    {
      // The conditional itself evaluates to boolean type.  This means
      // that the subexpressions will emit 'goto's themselves and not
      // leave values around.
      assert (true_branch->type () == primitive_boolean_type);
      int save = stack_depth;
      true_branch->visit (this);
      // FIXME an assertion here about true_branch not completing
      // normally would be nice.
      define (false_bytes);
      false_branch->visit (this);
      assert (save == stack_depth);
    }
  else
    {
      // When we see a conditional in a String '+' sequence, we must
      // make sure both branches are Strings after the conditional is
      // finished.  Otherwise code like this will generate incorrect
      // bytecode:
      //    "x" + (cond ? "true" : ("" + 0)) + "y"
      // FIXME: we could do better by generating the 'append'
      // operation as part of the conditional somehow.
      model_type *strtype = global->get_compiler ()->java_lang_String ();
      bool is_str_type = true_branch->type () == strtype;

      bytecode_block *done = new_bytecode_block ();

      if (expr_target == STRING && is_str_type)
	string_plus_type = NULL;
      true_branch->visit (this);
      model_type *pop_type = true_branch->type ();
      if (expr_target == STRING && is_str_type
	  && string_plus_type != NULL
	  && string_plus_type != strtype)
	{
	  handle_valueof (string_plus_type, cond_obj);
	  pop_type = strtype;
	}
      emit_relocation (reloc_goto, done);

      define (false_bytes);
      if (expr_target == STRING && is_str_type)
	string_plus_type = NULL;
      false_branch->visit (this);
      if (expr_target == STRING && is_str_type
	  && string_plus_type != NULL
	  && string_plus_type != strtype)
	{
	  handle_valueof (string_plus_type, cond_obj);
	  string_plus_type = strtype;
	}
      define (done);

      // Choice of branch is arbitrary.  It might seem strange to
      // reduce here, but both the true and false branches pushed
      // something on the stack, so we need to do some bookkeeping.
      // FIXME this is not a good model...
      if (expr_target != IGNORE)
	reduce_stack (pop_type);
    }
}

void
bytecode_generator::visit_field_ref (model_field_ref *ref,
				     const ref_expression &expr,
				     const model_field *field)
{
  // From the binary compatibility spec, we must inline any 'final'
  // field that has an initializer which is a compile-time constant.
  // This applies to both instance and static fields.
  // FIXME: ugly cast here.
  bool should_inline = const_cast<model_field *> (field)->inlineable_p ();

  if (expr)
    {
      // We might see 'expr.FIELD', where FIELD is static, in which
      // case we evaluate the expression for its side effects.  If
      // we're inlining the field reference, then we also won't need
      // whatever value is computed here.
      push_expr_target p (this, ((should_inline || field->static_p ())
				 ? IGNORE : ON_STACK));
      // A very obscure case: if we have 'foo.bar', and bar is
      // inlineable, we must inline it but we must also null-check
      // 'foo'.
      if (should_inline && ! field->static_p ())
	emit_null_pointer_check (expr.get ());
      else
	expr->visit (this);
    }
  else
    assert (field->static_p ());

  model_class *accessed;
  if (should_inline)
    {
      model_expression *init = field->get_initializer ().get ();
      // This assertion should be true due to constant folding when
      // resolving the field.
      assert (dynamic_cast<model_literal_base *> (init));
      init->visit (this);
    }
  else if (trampoline_required_p (field, method->get_declaring_class (),
				  &accessed))
    {
      // Emit a call to the trampoline method.
      model_method *tm
	// FIXME: remove this cast.
	= accessed->get_accessor (const_cast<model_field *> (field),
				  expr_target == LEFT_HAND_SIDE);
      assert (tm->static_p ());

      if (expr_target == LEFT_HAND_SIDE)
	{
	  // We need to generate 'method (new_value)'.
	  lhs_info.kind = METHOD_CALL;
	  lhs_info.index = -1;
	  lhs_info.type = field->static_p () ? accessed : expr->type ();
	  // FIXME: cast
	  lhs_info.field = const_cast<model_field *> (field);
	}
      else
	{
	  std::list<ref_expression> nullargs;
	  handle_invocation (op_invokestatic, ref->get_qualifying_class (),
			     tm, nullargs, false, ! field->static_p ());
	}
    }
  else if (expr_target == LEFT_HAND_SIDE)
    {
      assert (! expr || ! expr->type ()->array_p ());
      lhs_info.kind = field->static_p () ? STATIC_FIELD : INSTANCE_FIELD;
      // fixme cast
      lhs_info.index = cpool->add (ref->get_qualifying_class (),
				   (model_field *) field);
      lhs_info.type = field->static_p () ? NULL : expr->type ();
      // fixme cast
      lhs_info.field = (model_field *) field;
    }
  else
    {
      if (expr && expr->type ()->array_p ())
	{
	  assert (field->get_name () == "length");
	  emit (op_arraylength);
	}
      else
	{
	  emit (field->static_p () ? op_getstatic : op_getfield);
	  // fixme cast
	  emit2 (cpool->add (ref->get_qualifying_class (),
			     (model_field *) field));
	}

      if (expr && ! field->static_p ())
	reduce_stack (expr->type ());
      increase_stack (field->type ());

      if (expr_target == CONDITIONAL)
	{
	  emit_relocation (ifne, current_true_target);
	  emit_relocation (reloc_goto, current_false_target);
	  reduce_stack (primitive_boolean_type);
	}
      else if (expr_target == IGNORE)
	emit_pop (field->type ());
    }
}

void
bytecode_generator::visit_field_initializer (model_field_initializer *,
					     model_field *field)
{
  assert (expr_target == IGNORE);

  if ((field->static_p () && field->constant_p ())
      || ! field->has_initializer_p ())
    {
      // No code needed.
      return;
    }

  left_hand_side_info info;
  if (field->static_p ())
    info.kind = STATIC_FIELD;
  else
    {
      emit_load (method->get_declaring_class (), this_index);
      info.kind = INSTANCE_FIELD;
    }

  assert (field->has_initializer_p ());
  {
    push_expr_target where (this, ON_STACK);
    field->get_initializer ()->visit (this);
  }

  info.index = cpool->add (field->get_declaring_class (), field);
  info.type = method->get_declaring_class ();
  info.field = field;

  emit_lhs_store (&info, field->type ());
}

void
bytecode_generator::visit_instanceof (model_instanceof *inst,
				      const ref_expression &expr,
				      const ref_forwarding_type &klass)
{
  assert (expr_target == ON_STACK || expr_target == STRING
	  || expr_target == CONDITIONAL);
  {
    push_expr_target p (this, ON_STACK);
    expr->visit (this);
  }
  emit (op_instanceof);
  class_writer::check_type (inst, klass->type ());
  int index = cpool->add (klass->type ());
  emit2 (index);

  reduce_stack (expr->type ());
  increase_stack (primitive_boolean_type);

  if (expr_target == CONDITIONAL)
    {
      emit_relocation (ifne, current_true_target);
      emit_relocation (reloc_goto, current_false_target);
      reduce_stack (primitive_boolean_type);
    }
}

void
bytecode_generator::handle_logical_binary (const ref_expression &lhs,
					   const ref_expression &rhs,
					   bool preserve_true)
{
  assert (expr_target == ON_STACK || expr_target == STRING
	  || expr_target == CONDITIONAL);

  // We have an outer layer of branch targets to support the case
  // where we are called with ON_STACK.
  bytecode_block *outer_true = NULL;
  bytecode_block *outer_false = NULL;
  if (expr_target == ON_STACK || expr_target == STRING)
    {
      outer_true = new_bytecode_block ();
      outer_false = new_bytecode_block ();
    }
  else
    {
      outer_true = current_true_target;
      outer_false = current_false_target;
    }

  {
    branch_targets outer (this, outer_true, outer_false);

    // This is tricky: we create a single new local target, and then
    // rewrite either the false or true branch of the LHS
    // appropriately.
    bytecode_block *local_block = new_bytecode_block ();
    {
      branch_targets p (this,
			preserve_true ? current_true_target : local_block,
			preserve_true ? local_block : current_false_target);
      lhs->visit (this);
    }

    // Taking the local branch always means that we must evaluate the
    // RHS.
    define (local_block);

    rhs->visit (this);
  }

  if (expr_target == ON_STACK || expr_target == STRING)
    {
      bytecode_block *done = new_bytecode_block ();
      define (outer_true);
      emit (op_iconst_1);
      emit_relocation (reloc_goto, done);
      define (outer_false);
      emit (op_iconst_0);
      define (done);
      increase_stack (primitive_int_type);
    }
}

void
bytecode_generator::visit_logical_binary (model_lor *,
					  const ref_expression &lhs,
					  const ref_expression &rhs)
{
  // Our 'true' branch is the same as the outer true branch.
  // Our 'false' branch is our second clause.
  handle_logical_binary (lhs, rhs, true);
}

void
bytecode_generator::visit_logical_binary (model_land *,
					  const ref_expression &lhs,
					  const ref_expression &rhs)
{
  // Our 'false' branch is the same as the outer false branch.
  // Our 'true' branch is our second clause.
  handle_logical_binary (lhs, rhs, false);
}

void
bytecode_generator::visit_simple_literal (model_literal_base *,
					  const jboolean &val)
{
  if (expr_target == IGNORE)
    ;
  else if (expr_target == CONDITIONAL)
    emit_relocation (reloc_goto, (val ? current_true_target
				  : current_false_target));
  else
    {
      assert (expr_target == ON_STACK || expr_target == STRING);
      emit (val ? op_iconst_1 : op_iconst_0);
      increase_stack (primitive_boolean_type);
    }
}

void
bytecode_generator::visit_simple_literal (model_literal_base *,
					  const jbyte &val)
{
  if (expr_target != IGNORE)
    {
      assert (expr_target == ON_STACK || expr_target == STRING);
      visit_simple_literal (NULL, jint (val));
    }
}

void
bytecode_generator::visit_simple_literal (model_literal_base *,
					  const jchar &val)
{
  if (expr_target != IGNORE)
    {
      assert (expr_target == ON_STACK || expr_target == STRING);
      visit_simple_literal (NULL, jint (val));
    }
}

void
bytecode_generator::visit_simple_literal (model_literal_base *,
					  const jshort &val)
{
  if (expr_target != IGNORE)
    {
      assert (expr_target == ON_STACK || expr_target == STRING);
      visit_simple_literal (NULL, jint (val));
    }
}

void
bytecode_generator::visit_simple_literal (model_literal_base *,
					  const jint &val)
{
  if (expr_target != IGNORE)
    {
      assert (expr_target == ON_STACK || expr_target == STRING);
      if (val >= -1 && val <= 5)
	emit (op_iconst_0 + val);
      else if (val >= -128 && val < 128)
	{
	  emit (op_bipush);
	  emit (val);
	}
      else if (val >= -32768 && val < 32768)
	{
	  emit (op_sipush);
	  emit2 (val);
	}
      else
	{
	  int index = cpool->add (val);
	  emit_ldc (index);
	}
      increase_stack (primitive_int_type);
    }
}

void
bytecode_generator::visit_simple_literal (model_literal_base *,
					  const jlong &val)
{
  if (expr_target != IGNORE)
    {
      assert (expr_target == ON_STACK || expr_target == STRING);
      if (val == 0)
	emit (op_lconst_0);
      else if (val == 1)
	emit (op_lconst_1);
      // FIXME use a define somewhere for this
      else if (val >= -32768 && val < 32768)
	{
	  visit_simple_literal (NULL, jint (val));
	  reduce_stack (primitive_int_type);
	  emit (op_i2l);
	}
      else
	{
	  int index = cpool->add (val);
	  emit_ldc2 (index);
	}
      increase_stack (primitive_long_type);
    }
}

void
bytecode_generator::visit_simple_literal (model_literal_base *,
					  const jfloat &val)
{
  if (expr_target != IGNORE)
    {
      assert (expr_target == ON_STACK || expr_target == STRING);
      // fixme probably should use bit values
      if (val == 0)
	emit (op_fconst_0);
      else if (val == 1)
	emit (op_fconst_1);
      else if (val == 2)
	emit (op_fconst_2);
      else
	{
	  int index = cpool->add (val);
	  emit_ldc (index);
	}
      increase_stack (primitive_float_type);
    }
}

void
bytecode_generator::visit_simple_literal (model_literal_base *,
					  const jdouble &val)
{
  if (expr_target != IGNORE)
    {
      assert (expr_target == ON_STACK || expr_target == STRING);
      // fixme probably should use bit values
      if (val == 0)
	emit (op_dconst_0);
      else if (val == 1)
	emit (op_dconst_1);
      else
	{
	  int index = cpool->add (val);
	  emit_ldc2 (index);
	}
      increase_stack (primitive_double_type);
    }
}

void
bytecode_generator::visit_string_literal (model_string_literal *lit,
					  const std::string &val)
{
  if (expr_target != IGNORE)
    {
      assert (expr_target == ON_STACK || expr_target == STRING);
      int index = cpool->add (val);
      emit_ldc (index);
      increase_stack (lit->type ());
    }
}

/// The count operand of the invokeinterface duplicates the information
/// about the number of arguments that's also available from the method's
/// descriptor. Wide types count double.
static size_t
invokeinterface_count (const std::list<ref_expression> &args)
{
  size_t count = 1; // For "this".
  for (std::list<ref_expression>::const_iterator it = args.begin ();
       it != args.end ();
       ++it)
    {
      count += width_of_type ((*it)->type ());
    }
  return count;
}

void
bytecode_generator::handle_invocation (java_opcode opcode,
				       model_class *qualifier,
				       const model_method *meth,
				       const std::list<ref_expression> &args,
				       bool null_check_semantics,
				       bool use_accessor)
{
  {
    assert (expr_target == IGNORE
	    || expr_target == ON_STACK
	    || expr_target == STRING
	    || (expr_target == CONDITIONAL
		&& meth->get_return_type () == primitive_boolean_type));
    push_expr_target p (this, ON_STACK);
    // FIXME: some code duplication here.
    for (std::list<ref_expression>::const_iterator i = args.begin ();
	 i != args.end ();
	 ++i)
      {
	if (null_check_semantics)
	  {
	    emit_null_pointer_check ((*i).get ());
	    // So we don't take this branch again.
	    null_check_semantics = false;
	  }
	else
	  (*i)->visit (this);
      }
  }

  int ii_count = invokeinterface_count (args);
  if (ii_count > 255)
    // FIXME: location should really be location of the call.
    throw method->error ("method invocation requires more than "
			 "255 words of method arguments");

  emit (opcode);

  // FIXME: cast is still ugly.
  emit2 (cpool->add (qualifier, const_cast<model_method *> (meth)));
  if (opcode == op_invokeinterface)
    {
      emit (ii_count);
      emit (0);
    }

  // Model stack effects of the call.
  for (std::list<ref_expression>::const_iterator i = args.begin ();
       i != args.end ();
       ++i)
    reduce_stack ((*i)->type ());
  if (! meth->static_p () || use_accessor)
    reduce_stack (meth->get_declaring_class ());
  if (meth->get_return_type () != primitive_void_type)
    increase_stack (meth->get_return_type ());

  if (expr_target == IGNORE && meth->get_return_type () != primitive_void_type)
    emit_pop (meth->get_return_type ());
  else if (expr_target == CONDITIONAL)
    {
      emit_relocation (ifne, current_true_target);
      emit_relocation (reloc_goto, current_false_target);
      reduce_stack (primitive_boolean_type);
    }
}

void
bytecode_generator::visit_method_invocation (model_method_invocation *inv,
					     const model_method *meth,
					     const ref_expression &this_expr,
					     const std::list<ref_expression> &args)
{
  if (this_expr)
    {
      // We might be calling a static method with a "non-static lhs".
      // In this situation we must still evaluate the lhs (for side
      // effects), then we can just drop the result.
      push_expr_target where (this, meth->static_p () ? IGNORE : ON_STACK);
      this_expr->visit (this);
    }

  model_class *accessed = NULL;
  bool nonstatic_accessor = false;
  if (trampoline_required_p (meth, method->get_declaring_class (), &accessed))
    {
      nonstatic_accessor = ! meth->static_p ();
      meth = accessed->get_accessor (const_cast<model_method *> (meth));
    }

  java_opcode opcode;
  if (meth->static_p ())
    opcode = op_invokestatic;
  else if (inv->get_qualifying_class ()->interface_p ())
    opcode = op_invokeinterface;
  else if ((meth->get_modifiers () & ACC_PRIVATE) != 0)
    opcode = op_invokespecial;
  else
    opcode = op_invokevirtual;
  handle_invocation (opcode, inv->get_qualifying_class (), meth, args,
		     false, nonstatic_accessor);
}

void
bytecode_generator::visit_type_qualified_invocation
   (model_type_qualified_invocation *inv,
    const model_method *meth,
    const std::list<ref_expression> &args,
    bool super)
{
  if (! meth->static_p ())
    emit_load (meth->get_declaring_class (), this_index);

  // FIXME: duplicate code.
  model_class *accessed = NULL;
  bool nonstatic_accessor = false;
  if (trampoline_required_p (meth, method->get_declaring_class (), &accessed))
    {
      nonstatic_accessor = ! meth->static_p ();
      meth = accessed->get_accessor (const_cast<model_method *> (meth));
    }

  handle_invocation (super ? op_invokespecial : op_invokestatic,
		     inv->get_qualifying_class (),
		     meth, args, false, nonstatic_accessor);
}

void
bytecode_generator::visit_super_invocation
    (model_super_invocation *inv,
     const model_method *meth,
     const std::list<ref_expression> &args)
{
  // FIXME: duplicate code.
  model_class *accessed;
  bool nonstatic_accessor = false;
  if (trampoline_required_p (meth, method->get_declaring_class (), &accessed))
    {
      nonstatic_accessor = ! meth->static_p ();
      meth = accessed->get_accessor (const_cast<model_method *> (meth));
    }

  emit_load (method->get_declaring_class (), this_index);
  handle_invocation (op_invokespecial, inv->get_qualifying_class (),
		     meth, args, inv->get_expression () != NULL,
		     nonstatic_accessor);
}

void
bytecode_generator::visit_this_invocation
    (model_this_invocation *inv,
     const model_method *meth,
     const std::list<ref_expression> &args)
{
  // Note: an accessor can never be needed here.
  emit_load (method->get_declaring_class (), this_index);
  handle_invocation (op_invokespecial, inv->get_qualifying_class (),
		     meth, args, false, inv->get_expression () != NULL);
}

void
bytecode_generator::emit_null_pointer_check (model_expression *expr)
{
  // In a couple of situations we might need to emit a null pointer
  // check.  We do this by calling getClass() on the object, then
  // ignoring the result.  We skip 'this' expressions, since we know
  // this will never be needed in those cases; we could add this in
  // other situations as well.  FIXME: should probably have a new 'can
  // be null' predicate on model_expression.
  assert (expr->type ()->reference_p ());

  bool should_check = (dynamic_cast<model_this *> (expr) == NULL);
  {
    // The caller will have set the target, but if we need a check
    // then we must override.
    assert (expr_target == IGNORE || expr_target == ON_STACK);
    push_expr_target push (this, should_check ? ON_STACK : expr_target);
    expr->visit (this);
  }
  if (! should_check)
    return;

  model_class *obj = global->get_compiler ()->java_lang_Object ();
  if (expr_target == ON_STACK)
    {
      emit (op_dup);
      increase_stack (obj);
    }
  model_method *meth
    = find_method ("getClass", obj, NULL,
		   global->get_compiler ()->java_lang_Class (),
		   expr);
  emit (op_invokevirtual);
  emit2 (cpool->add (obj, meth));
  emit (op_pop);
  reduce_stack (obj);
}

void
bytecode_generator::visit_new (model_new *new_expr,
			       const model_method *init_meth,
			       const ref_forwarding_type &klass,
			       const std::list<ref_expression> &args)
{
  assert (expr_target == ON_STACK || expr_target == STRING
	  || expr_target == IGNORE);

  // Stack: ... NEW
  emit (op_new);
  int kindex = cpool->add (klass->type ());
  emit2 (kindex);
  increase_stack (klass->type ());
  // Stack: ... NEW NEW
  emit (op_dup);
  increase_stack (klass->type ());

  {
    push_expr_target where (this, ON_STACK);
    // Stack: ... NEW NEW [ARGS]...
    // If the 'new' expression had a qualifier, we have to perform a
    // null pointer check on it.
    bool has_qualifier = (new_expr->get_expression () != NULL);
    for (std::list<ref_expression>::const_iterator i = args.begin ();
	 i != args.end ();
	 ++i)
      {
	if (has_qualifier)
	  {
	    emit_null_pointer_check ((*i).get ());
	    // So we don't take this branch again.
	    has_qualifier = false;
	  }
	else
	  (*i)->visit (this);
      }
  }
  // Stack: ... NEW-INITIALIZED
  emit (op_invokespecial);

  // If we need an accessor constructor, use it instead.
  model_class *accessed;
  if (trampoline_required_p (init_meth, method->get_declaring_class (),
			     &accessed))
    init_meth
      = accessed->get_accessor (const_cast<model_method *> (init_meth));

  int mindex = cpool->add (init_meth->get_declaring_class (),
			   const_cast<model_method *> (init_meth));
  emit2 (mindex);

  // Pop all the arguments and the class reference.
  for (std::list<ref_expression>::const_reverse_iterator i = args.rbegin ();
       i != args.rend ();
       ++i)
    reduce_stack ((*i)->type ());
  reduce_stack (klass->type ());

  if (expr_target == IGNORE)
    emit_pop (klass->type ());
}

void
bytecode_generator::visit_new_array (model_new_array *newarray,
				     const ref_forwarding_type &elt_type,
				     const std::list<ref_expression> &indices,
				     const ref_expression &init)
{
  model_type *et = elt_type->type ();
  model_type *atype = et;
  int len = 0;
  {
    assert (expr_target == ON_STACK || expr_target == STRING);
    push_expr_target push (this, ON_STACK);
    for (std::list<ref_expression>::const_iterator i = indices.begin ();
	 i != indices.end ();
	 ++i)
      {
	(*i)->visit (this);
	++len;
	atype = atype->array ();
      }
  }
  if (len > 1)
    {
      class_writer::check_type (newarray, atype);
      emit (op_multianewarray);
      int kindex = cpool->add (atype);
      emit2 (kindex);
      emit (len);
    }
  // The 'len == 0' case occurs for something like 'new Object[] {val}'.
  else if (len != 0)
    emit_new_array (et);

  for (std::list<ref_expression>::const_iterator i = indices.begin ();
       i != indices.end ();
       ++i)
    reduce_stack ((*i)->type ());
  if (len > 0)
    increase_stack (atype);

  if (init)
    {
      assert (len == 0);
      init->visit (this);
    }
}

void
bytecode_generator::visit_null_literal (model_null_literal *)
{
  assert (expr_target == ON_STACK || expr_target == STRING);
  emit (op_aconst_null);
  increase_stack (null_type);
}

void
bytecode_generator::visit_prefix_simple (model_prefix_plus *,
					 const ref_expression &expr)
{
  // Nothing special to do here.
  expr->visit (this);
}

void
bytecode_generator::visit_prefix_simple (model_prefix_minus *,
					 const ref_expression &expr)
{
  assert (expr_target == ON_STACK || expr_target == STRING);
  {
    push_expr_target push (this, ON_STACK);
    expr->visit (this);
    emit (op_ineg + adjust_for_type (expr->type ()));
  }
}

void
bytecode_generator::visit_prefix_simple (model_bitwise_not *,
					 const ref_expression &expr)
{
  assert (expr_target == ON_STACK || expr_target == STRING);
  {
    push_expr_target push (this, ON_STACK);
    expr->visit (this);
  }
  if (expr->type () == primitive_long_type)
    {
      emit (op_iconst_m1);
      emit (op_i2l);
      // This lets us compute the maximum correctly.
      increase_stack (primitive_long_type);
      reduce_stack (primitive_long_type);
      emit (op_lxor);
    }
  else
    {
      emit (op_iconst_m1);
      // This lets us compute the maximum correctly.
      increase_stack (primitive_int_type);
      reduce_stack (primitive_int_type);
      emit (op_ixor);
    }
}

void
bytecode_generator::visit_prefix_simple (model_logical_not *,
					 const ref_expression &expr)
{
  assert (expr_target == ON_STACK || expr_target == STRING
	  || expr_target == CONDITIONAL);

  if (expr_target == CONDITIONAL)
    {
      // Just reverse the current true and false targets and then keep
      // going.
      branch_targets push (this, current_false_target, current_true_target);
      expr->visit (this);
    }
  else
    {
      // Just xor with 1.
      expr->visit (this);
      emit (op_iconst_1);
      // This lets us compute the maximum correctly.
      increase_stack (primitive_int_type);
      reduce_stack (primitive_int_type);
      emit (op_ixor);
    }
}

void
bytecode_generator::handle_inc_dec (const ref_expression &expr,
				    bool prefix,
				    int delta)
{
  assert (expr_target == IGNORE || expr_target == ON_STACK
	  || expr_target == STRING);

  {
    push_expr_target push (this, LEFT_HAND_SIDE);
    expr->visit (this);
  }

  if (lhs_info.kind == VARIABLE
      && lhs_info.type->integral_p ()
      // We only allow this for 'int' types, due to overflow handling
      // for narrower types.
      && lhs_info.type == primitive_int_type)
    {
      assert (lhs_info.index >= 0);

      if (! prefix && expr_target != IGNORE)
	emit_load (lhs_info.type, lhs_info.index);
      if (lhs_info.index > 255)
	{
	  emit (op_wide);
	  emit (op_iinc);
	  emit2 (lhs_info.index);
	  emit2 (delta);
	}
      else
	{
	  emit (op_iinc);
	  emit (lhs_info.index);
	  emit (delta);
	}
      if (prefix && expr_target != IGNORE)
	emit_load (lhs_info.type, lhs_info.index);
      return;
    }

  // This duplicates the left-hand-side information and then
  // dereferences it.
  dereference_left_hand_side (&lhs_info);

  model_type *t = expr->type ();
  if (expr_target != IGNORE && ! prefix)
    duplicate_lhs_value (&lhs_info, t);

  {
    push_expr_target push (this, ON_STACK);
    if (t == primitive_double_type)
      visit_simple_literal (NULL, jdouble (delta));
    else if (t == primitive_float_type)
      visit_simple_literal (NULL, jfloat (delta));
    else if (t == primitive_long_type)
      visit_simple_literal (NULL, jlong (delta));
    else
      {
	assert (t == primitive_byte_type || t == primitive_short_type
		|| t == primitive_char_type || t == primitive_int_type);
	visit_simple_literal (NULL, jint (delta));
      }
  }

  emit (op_iadd + adjust_for_type (t));
  reduce_stack (t);

  // For a narrow integer type, we must narrow the value.  Do this
  // before duplicating, so we also leave the correct value on the
  // stack in the prefix case.
  if (t == primitive_byte_type || t == primitive_short_type
      || t == primitive_char_type)
    {
      // '5' is the type adjustment for 'byte'.
      emit (op_i2b - 5 + adjust_for_type_full (t));
    }

  if (expr_target != IGNORE && prefix)
    duplicate_lhs_value (&lhs_info, t);

  emit_lhs_store (&lhs_info, t);
}

void
bytecode_generator::visit_prefix_side_effect (model_prefix_plusplus *,
					      const ref_expression &expr)
{
  handle_inc_dec (expr, true, 1);
}

void
bytecode_generator::visit_prefix_side_effect (model_prefix_minusminus *,
					      const ref_expression &expr)
{
  handle_inc_dec (expr, true, -1);
}

void
bytecode_generator::visit_postfix_side_effect (model_postfix_plusplus *,
					       const ref_expression &expr)
{
  handle_inc_dec (expr, false, 1);
}

void
bytecode_generator::visit_postfix_side_effect (model_postfix_minusminus *,
					       const ref_expression &expr)
{
  handle_inc_dec (expr, false, -1);
}

void
bytecode_generator::visit_this (model_this *this_expr)
{
  assert (expr_target == ON_STACK || expr_target == STRING
	  || expr_target == IGNORE);
  if (expr_target != IGNORE)
    {
      // A plain 'this' just loads the variable.  For other things we
      // must work our way outward until we hit the correct class.
      model_class *rtype = assert_cast<model_class *> (this_expr->type ());
      model_class *prev = rtype;
      model_class *iter = method->get_declaring_class ();

      if (method->constructor_p () && ! this_expr->check_match (rtype, iter))
	{
	  // In <init>, we want to reference the argument
	  // corresponding to this$0, not the field.  This is more
	  // efficient, and also without it we can create unverifiable
	  // bytecode before invoking the superclass constructor.
	  // Obviously this doesn't make sense for a pure "this", only
	  // an outer "this".
	  model_constructor *cons
	    = assert_cast<model_constructor *> (method);
	  model_variable_decl *th0 = cons->get_this0_parameter ();
	  emit_load (th0->type (), vars.get_index (th0));
	  prev = iter;
	  iter = iter->get_lexically_enclosing_class ();
	}
      else
	emit_load (rtype, this_index);
      for (;
	   ! this_expr->check_match (rtype, iter);
	   iter = iter->get_lexically_enclosing_class ())
	{
	  assert (iter != NULL);

	  ref_field th0 = iter->get_this_0 ();
	  emit (op_getfield);
	  emit2 (cpool->add (th0->get_declaring_class (), th0.get ()));

	  reduce_stack (prev);
	  increase_stack (iter);
	  prev = iter;
	}
    }
}

void
bytecode_generator::visit_simple_variable_ref (model_simple_variable_ref *,
					       const model_variable_decl *decl)
{
  assert (expr_target == ON_STACK || expr_target == LEFT_HAND_SIDE
	  || expr_target == IGNORE || expr_target == CONDITIONAL
	  || expr_target == STRING);

  // Basically 'var.FIELD', where FIELD is static.
  if (expr_target == IGNORE)
    return;

  int n = vars.get_index ((model_variable_decl *) decl);
  if (expr_target == LEFT_HAND_SIDE)
    {
      lhs_info.kind = VARIABLE;
      lhs_info.index = n;
      lhs_info.type = decl->type ();
      lhs_info.field = NULL;
    }
 else
    {
      // FIXME cast
      emit_load (decl->type (), n);
    }

  if (expr_target == CONDITIONAL)
    {
      emit_relocation (ifne, current_true_target);
      emit_relocation (reloc_goto, current_false_target);
      reduce_stack (primitive_boolean_type);
    }
}



void
bytecode_generator::emit (jbyte byte)
{
  current_block->emit (byte & 0xff);
}

void
bytecode_generator::emit2 (int val)
{
  current_block->emit ((val & 0xff00) >> 8);
  current_block->emit (val & 0xff);
}

void
bytecode_generator::emit4 (int val)
{
  current_block->emit ((val >> 24) & 0xff);
  current_block->emit ((val >> 16) & 0xff);
  current_block->emit ((val >>  8) & 0xff);
  current_block->emit (val & 0xff);
}

void
bytecode_generator::emit_new_array (model_type *elt_type)
{
  if (elt_type->primitive_p ())
    {
      emit (op_newarray);

      jbyte val;
      if (elt_type == primitive_int_type)
	val = 10;
      else if (elt_type == primitive_long_type)
	val = 11;
      else if (elt_type == primitive_float_type)
	val = 6;
      else if (elt_type == primitive_double_type)
	val = 7;
      else if (elt_type == primitive_boolean_type)
	val = 4;
      else if (elt_type == primitive_byte_type)
	val = 8;
      else if (elt_type == primitive_char_type)
	val = 5;
      else if (elt_type == primitive_short_type)
	val = 9;
      else
	abort ();
      emit (val);
    }
  else
    {
      emit (op_anewarray);
      int kindex = cpool->add (elt_type);
      emit2 (kindex);
    }
}

void
bytecode_generator::add_exception_handler (model_type *type,
					   bytecode_block *target,
					   bytecode_block *start,
					   bytecode_block *end)
{
  handler hand;

  hand.type = type;
  hand.target = target;
  hand.start = start;
  hand.end = end;

  handlers.push_back (hand);
}

void
bytecode_generator::emit_relocation (relocation_kind what,
				     bytecode_block *dest)
{
  if (! current_block->emit_relocation (what, dest))
    non_normal_completion (true);
}

// Mark this block as finished and possibly start a new block.
void
bytecode_generator::non_normal_completion (bool make_new_block)
{
  current_block->set_can_complete_normally (false);
  if (make_new_block)
    {
      bytecode_block *next = new_bytecode_block ();
      define (next);
    }
}

void
bytecode_generator::emit_load (model_type *t, int index)
{
  assert (index >= 0);
  if (index < 4)
    emit (op_iload_0 + 4 * adjust_for_type (t) + index);
  else if (index > 255)
    {
      emit (op_wide);
      emit (op_iload + adjust_for_type (t));
      emit2 (index);
    }
  else
    {
      emit (op_iload + adjust_for_type (t));
      emit (index);
    }
  increase_stack (t);
}

void
bytecode_generator::emit_store (model_type *t, int index)
{
  assert (index >= 0);
  if (index < 4)
    emit (op_istore_0 + 4 * adjust_for_type (t) + index);
  else if (index > 255)
    {
      emit (op_wide);
      emit (op_istore + adjust_for_type (t));
      emit2 (index);
    }
  else
    {
      emit (op_istore + adjust_for_type (t));
      emit (index);
    }
  reduce_stack (t);
}

void
bytecode_generator::emit_ldc2 (int index)
{
  assert (index >= 0);
  emit (op_ldc2_w);
  emit2 (index);
}

void
bytecode_generator::emit_ldc (int index)
{
  assert (index >= 0);
  if (index < 256)
    {
      emit (op_ldc);
      emit (index);
    }
  else
    {
      emit (op_ldc_w);
      emit2 (index);
    }
}

void
bytecode_generator::emit_pop (model_type *t)
{
  emit (wide_p (t) ? op_pop2 : op_pop);
  reduce_stack (t);
}

void
bytecode_generator::define (bytecode_block *block)
{
  if (! first_block)
    first_block = current_block;
  current_block->set_next (block);
  current_block = block;
}



void
bytecode_generator::increase_stack (model_type *t)
{
  stack_depth += width_of_type (t);
  if (stack_depth > max_stack)
    max_stack = stack_depth;
}

void
bytecode_generator::reduce_stack (model_type *t)
{
  stack_depth -= width_of_type (t);
  assert (stack_depth >= 0);
}

// Definite assignment.

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

class variable_state
{
  typedef std::map<const model_variable_decl *, bool> map_type;

  // Two maps, one for definite assignment and one for definite
  // unassignment.
  map_type assign;
  map_type unassign;

  void intersect (map_type &dest, const map_type &other)
  {
    for (map_type::const_iterator i = other.begin ();
	 i != other.end ();
	 ++i)
      {
	// We don't have to do anything if the other's flag is true,
	// our existing state is fine.  If the variable was not
	// defined here, then we don't want to introduce it, even with
	// a 'false' mapping.
	if (! (*i).second && dest.find ((*i).first) != dest.end ())
	  dest[(*i).first] = false;
      }
  }

  void do_union (map_type &dest, const map_type &other)
  {
    for (map_type::const_iterator i = other.begin ();
	 i != other.end ();
	 ++i)
      {
	if ((*i).second)
	  dest[(*i).first] = true;
      }
  }

public:

  variable_state ()
  {
  }

  ~variable_state ()
  {
  }

  variable_state (const variable_state &other)
    : assign (other.assign),
      unassign (other.unassign)
  {
  }

  variable_state &operator= (const variable_state &other)
  {
    assign = other.assign;
    unassign = other.unassign;
    return *this;
  }

  variable_state &operator= (bool val)
  {
    // Can only set to 'true', never 'false'.
    assert (val);

    for (map_type::iterator i = assign.begin ();
	 i != assign.end ();
	 ++i)
      (*i).second = val;
    for (map_type::iterator i = unassign.begin ();
	 i != unassign.end ();
	 ++i)
      (*i).second = val;

    return *this;
  }

  variable_state &operator&= (const variable_state &other)
  {
    intersect (assign, other.assign);
    intersect (unassign, other.unassign);
    return *this;
  }

  void merge_finally (const variable_state &other)
  {
    do_union (assign, other.assign);
    intersect (unassign, other.unassign);
  }

  void set_definite_unassignment_from (const variable_state &other)
  {
    unassign = other.unassign;
  }

  void set_definite_assignment_from (const variable_state &other)
  {
    assign = other.assign;
  }

  // This updates our state with the delta between FIRST and SECOND.
  void set_definite_assignment_from (const variable_state &first,
				     const variable_state &second)
  {
    for (map_type::const_iterator i = second.assign.begin ();
	 i != second.assign.end ();
	 ++i)
      {
	if (! (*i).second)
	  continue;
	map_type::const_iterator it = first.assign.find ((*i).first);
	if (it != first.assign.end () && ! (*it).second)
	  {
	    assign[(*i).first] = true;
	    unassign[(*i).first] = false;
	  }
      }
  }

  void insert (const model_variable_decl *decl, bool is_set = false)
  {
    assert (assign.find (decl) == assign.end ());
    assert (unassign.find (decl) == unassign.end ());
    assign[decl] = is_set;
    unassign[decl] = ! is_set;
  }

  void set_assign (const model_variable_decl *decl)
  {
    assign[decl] = true;
    unassign[decl] = false;
  }

  bool assigned_p (const model_variable_decl *decl) const
  {
    map_type::const_iterator it = assign.find (decl);
    if (it == assign.end ())
      return false;
    return (*it).second;
  }

  bool unassigned_p (const model_variable_decl *decl) const
  {
    map_type::const_iterator it = unassign.find (decl);
    if (it == unassign.end ())
      return false;
    return (*it).second;
  }

  // Perform checking after a loop completes.  THIS is the state
  // before the loop.  OTHER is the state after the loop.  Look for
  // final variables which are definitely assigned and not definitely
  // unassigned in OTHER but are definitely unassigned here and issue
  // errors for them.
  void check_loop (model_element *request, const variable_state &other)
  {
    for (map_type::const_iterator i = other.unassign.begin ();
	 i != other.unassign.end ();
	 ++i)
      {
	if ((*i).first->final_p () && ! (*i).second)
	  {
	    if (other.assigned_p ((*i).first) && unassigned_p ((*i).first))
	      {
		std::cerr << request->error ("variable %1 might be assigned "
					     "multiple times")
		  // FIXME: need operator% for this.
		  % (*i).first->get_name ();
	      }
	  }
      }
  }

  /// For debugging convenience.  Prints the definitely [un]assigned
  /// variables in this state.
  void print () const
  {
    std::cout << "================" << std::endl;

    for (map_type::const_iterator i = assign.begin ();
	 i != assign.end ();
	 ++i)
      if ((*i).second)
        std::cout << "assigned: " << (*i).first->get_name () << std::endl;

    for (map_type::const_iterator i = unassign.begin ();
	 i != unassign.end ();
	 ++i)
      if ((*i).second)
        std::cout << "unassigned: " << (*i).first->get_name () << std::endl;
  }
};

/// This is used to keep track of a 'join'.  This is a point at which
/// multiple control paths meet.  At a join we merge states from these
/// various control paths.
class join_state
{
  // All state is public for ease of manipulation.  This class is
  // purely for implementation.
public:

  // The statement this represents.
  model_stmt *statement;

  // True if this has a possible 'continue' target.
  bool has_continue;

  // True if the 'continue' state has been set.
  bool is_continue_set;

  // All variable settings.
  variable_state state;
  variable_state continue_state;

  // Used when considering pending targets.
  struct pending_target
  {
    model_stmt *statement;
    variable_state state;
    bool is_continue;
  };

  typedef std::list<pending_target> pending_pairs_type;

  // For 'try' statements we keep a list of all pending targets
  // post-'finally'.
  pending_pairs_type pending_pairs;

  join_state (model_stmt *s, variable_state &init)
    : statement (s),
      has_continue (false),
      is_continue_set (false),
      state (init)
  {
    // We don't really want the state in 'init' -- we just want the
    // variable maps.
    state = true;
  }

  void merge (const variable_state &new_state)
  {
    state &= new_state;
  }

  void set_continue ()
  {
    assert (! has_continue);
    has_continue = true;
  }

  void merge_continue (const variable_state &new_state)
  {
    assert (has_continue);
    if (! is_continue_set)
      {
	is_continue_set = true;
	continue_state = new_state;
      }
    else
      continue_state &= new_state;
  }

  void add_pending (model_stmt *target, bool is_continue,
		    const variable_state &new_state)
  {
    merge (new_state);
    pending_target targ;
    targ.statement = target;
    targ.state = new_state;
    targ.is_continue = is_continue;
    pending_pairs.push_back (targ);
  }

  variable_state &get_state ()
  {
    return state;
  }

  variable_state &get_continue_state ()
  {
    assert (is_continue_set);
    return continue_state;
  }
};

/// This is a visitor that handles definite assignment calculations
/// for a given method.  It understands special rules for
/// constructors, static initializers, and instance initializers.
class definite_assignment_visitor : public visitor
{
  typedef enum
    {
      NONE,
      // For simple assignment, we visit the LHS twice, the first time
      // to check subexpressions, and the second time to check the
      // "expression itself".
      INITIAL_LHS,
      // Second phase of simple assignment.
      SIMPLE_LHS,
      // Visiting the LHS of compound assignment.
      COMPOUND_LHS
    } lhs_type;

  // This stores the current state, which is always used as the
  // 'before' state on entry to a visitor method.
  variable_state current;

  // These store the result of 'when true' and 'when false' for
  // boolean expressions.
  variable_state when_true;
  variable_state when_false;

  // This is true when visiting the left hand side of an assignment.
  lhs_type visiting_lhs;

  // Stack of all our join states.
  std::deque<join_state *> target_stack;

  // True if this is visiting <clinit>.
  bool static_init;
  // True if this is visiting a constructor.
  bool constructor;

  // Class declaring the method we're visiting.
  model_class *current_class;

  // The fields declared in that class.
  std::set<model_field *> fields;
  // The same, in order.
  std::list<ref_field> local_fields;

  // This keeps track of which fields have been declared.  It is only
  // used when handling a constructor or static initializer.
  std::set<model_field *> decl_fields;

  // True when we're in a field initializer.
  bool in_init;

  // Used for special handling of switch statements.
  variable_state *switch_state;

public:

  definite_assignment_visitor ()
    : visiting_lhs (NONE),
      static_init (false),
      constructor (false),
      current_class (NULL),
      in_init (false),
      switch_state (NULL)
  {
  }

  virtual ~definite_assignment_visitor ()
  {
    assert (switch_state == NULL);
  }

  void visit_method (model_method *meth,
		     const std::list<ref_variable_decl> &params,
		     const ref_block &block)
  {
    // Nothing to do for bytecode.
    if (meth->get_declaring_class ()->from_class_p ())
      return;

    if (current_class != NULL)
      {
	// We might be called recursively in a single case.
	assert (meth->instance_initializer_p () && params.empty ());
	in_init = true;
	block->visit (this);
	in_init = false;
	return;
      }

    assert (! meth->instance_initializer_p ());
    current_class = meth->get_declaring_class ();

    // Register the parameters.
    for (std::list<ref_variable_decl>::const_iterator i = params.begin ();
	 i != params.end ();
	 ++i)
      current.insert ((*i).get (), true);

    static_init = meth->static_initializer_p ();
    constructor = meth->constructor_p ();
    in_init = static_init;

    // We need special handling for <clinit> and constructors.
    if (static_init || constructor)
      {
	modifier_t flags = ACC_FINAL;
	if (static_init)
	  flags |= ACC_STATIC;

	local_fields = meth->get_declaring_class ()->get_fields ();
	for (std::list<ref_field>::const_iterator i = local_fields.begin ();
	     i != local_fields.end ();
	     ++i)
	  {
	    // Register constant fields as already declared.
	    if ((*i)->constant_p ())
	      decl_fields.insert ((*i).get ());

	    if (! (*i)->final_p ())
	      continue;

	    if (static_init)
	      {
		if (! (*i)->static_p () || (*i)->constant_p ())
		  continue;
	      }
	    else
	      {
		if ((*i)->static_p ())
		  continue;
	      }

	    // Don't pay attention to whether there is an initializer;
	    // this is handled by explicit assignments.
	    current.insert ((*i).get ());
	    fields.insert ((*i).get ());
	  }

	join_state save (NULL, current);
	stack_temporary<join_state *> push (target_stack, &save);
	block->visit (this);
	save.merge (current);

	for (std::set<model_field *>::const_iterator i = fields.begin ();
	     i != fields.end ();
	     ++i)
	  {
	    model_variable_decl *var = *i;
	    if (! save.get_state ().assigned_p (var))
	      {
		if (static_init)
		  std::cerr << var->error ("field %1 must be assigned "
					   "by a static initializer")
		    % var->get_name ();
		else if (constructor)
		  // FIXME: emit variable's location too?
		  std::cerr << meth->error ("field %1 must be assigned "
					    "in constructor or "
					    "instance initializer")
		    % var->get_name ();
	      }
	  }
      }
    else
      {
	block->visit (this);
      }
  }

  void visit_assert (model_assert *, const ref_expression &expr,
		     const ref_expression &result)
  {
    assert (visiting_lhs == NONE);
    variable_state save_DA = current;
    expr->visit (this);
    variable_state save = when_true;
    current = when_false;
    if (result)
      {
	visit (result);
	current = true;
      }
    current &= save;
    current.set_definite_assignment_from (save_DA);
  }

  void visit_block (model_block *, const std::list<ref_stmt> &statements)
  {
    ::visit (this, statements);
  }

  void visit_bytecode_block (model_bytecode_block *,
			     int, int, int, const uint8 *)
  {
    // Can't get here.
    abort ();
  }

  void visit_break (model_break *stmt, const ref_stmt &actual)
  {
    model_stmt *target = actual.get ();
    if (target == NULL)
      target = stmt->get_target ();
    propagate_branch (target);
    current = true;
  }

  void visit_catch (model_catch *, const ref_variable_decl &param,
		    const ref_block &block)
  {
    current.insert (param.get (), true);
    block->visit (this);
  }

  void visit_continue (model_continue *, const ref_stmt &actual)
  {
    propagate_branch (actual.get (), true);
    current = true;
  }

  void visit_class_decl_stmt (model_class_decl_stmt *stmt, const ref_class &k)
  {
    model_class *klass = k.get ();
    assert (klass->local_p ());
    // Captured variables must be DA at the declaration of the class,
    // not just at constructor invocations.
    std::list<model_variable_decl *> vars = klass->get_captured_variables ();
    for (std::list<model_variable_decl *>::const_iterator i = vars.begin ();
	 i != vars.end ();
	 ++i)
      handle_var_ref (stmt, *i);
  }

  void visit_do (model_do *dostmt, const ref_expression &expr,
		 const ref_stmt &stmt)
  {
    variable_state pre_loop = current;

    join_state save (dostmt, current);
    save.set_continue ();
    stack_temporary<join_state *> push (target_stack, &save);

    stmt->visit (this);
    save.merge_continue (current);
    current = save.get_continue_state ();
    // Don't use our 'visit' wrapper, it does the wrong thing for
    // booleans here.
    expr->visit (this);

    pre_loop.check_loop (dostmt, when_true);

    save.merge (when_false);
    current = save.get_state ();
  }

  void visit_empty (model_empty *)
  {
    // Nothing.
  }

  void visit_expression_stmt (model_expression_stmt *,
			      const ref_expression &expr)
  {
    visit (expr);
  }

  void visit_for_enhanced (model_for_enhanced *forstmt,
			   const ref_stmt &body, const ref_expression &expr,
			   const ref_variable_decl &var)
  {
    variable_state pre_loop = current;

    join_state save (forstmt, current);
    save.set_continue ();
    stack_temporary<join_state *> push (target_stack, &save);

    current.insert (var.get ());
    visit (expr);
    current.set_assign (var.get ());
    body->visit (this);

    save.merge_continue (current);
    current = save.get_continue_state ();
    pre_loop.check_loop (forstmt, current);
  }

  void visit_for (model_for *forstmt, const ref_stmt &init,
		  const ref_expression &cond, const ref_stmt &update,
		  const ref_stmt &body)
  {
    join_state save (forstmt, current);
    save.set_continue ();
    stack_temporary<join_state *> push (target_stack, &save);

    if (init)
      init->visit (this);
    variable_state pre_loop = current;

    if (cond)
      {
	// Don't use our 'visit' wrapper, it does the wrong thing for
	// booleans here.
	cond->visit (this);
	save.merge (when_false);
	current = when_true;
      }

    body->visit (this);
    save.merge_continue (current);
    current = save.get_continue_state ();
    if (update)
      update->visit (this);

    pre_loop.check_loop (forstmt, current);

    current = save.get_state ();
  }

  void visit_if (model_if *, const ref_expression &expr,
		 const ref_stmt &then_stmt, const ref_stmt &else_stmt)
  {
    assert (visiting_lhs == NONE);
    // Don't use our 'visit' wrapper, it does the wrong thing for
    // booleans here.
    expr->visit (this);

    variable_state save = when_false;

    current = when_true;
    then_stmt->visit (this);

    std::swap (current, save);
    if (else_stmt)
      else_stmt->visit (this);

    current &= save;
  }

  void visit_label (model_label *lstmt, const ref_stmt &stmt)
  {
    join_state save (lstmt, current);
    stack_temporary<join_state *> push (target_stack, &save);
    stmt->visit (this);
    save.merge (current);
    current = save.get_state ();
  }

  void visit_return (model_return *, const ref_expression &expr)
  {
    if (expr)
      visit (expr);
    propagate_branch ();
    current = true;
  }

  void visit_switch (model_switch *swstmt,
		     const ref_expression &expr,
		     const std::list<ref_switch_block> &blocks)
  {
    visit (expr);

    join_state save (swstmt, current);
    stack_temporary<join_state *> push (target_stack, &save);

    variable_state start = current;
    variable_state *save_sw = switch_state;
    switch_state = &start;

    for (std::list<ref_switch_block>::const_iterator i = blocks.begin ();
	 i != blocks.end ();
	 ++i)
      {
	// Due to the way that 'break' is handled, we don't need to
	// pay any special attention to whether a switch block falls
	// through.
	current &= start;
	(*i)->visit (this);
      }

    save.merge (current);
    if (swstmt->get_default () == NULL)
      save.merge (start);

    switch_state = save_sw;
    current = save.get_state ();
  }

  void visit_switch_block (model_switch_block *,
			   const std::list<ref_stmt> &statements)
  {
    visit_block (NULL, statements);
  }

  void visit_synchronized (model_synchronized *,
			   const ref_expression &expr,
			   const ref_stmt &stmt)
  {
    visit (expr);
    stmt->visit (this);
  }

  void visit_throw (model_throw *stmt, const ref_expression &expr)
  {
    visit (expr);
    current = true;
  }

  void visit_try (model_try *trystmt, const ref_block &try_block,
		  const std::list<ref_catch> &catchers,
		  const ref_block &finally)
  {
    join_state save (trystmt, current);
    variable_state pre_statement = current;
    // The state in SAVE is also touched by things like throw and
    // return which propagate upward.  We also need an accumulator
    // that doesn't take these into account.
    variable_state accum;

    {
      // We introduce a new scope here so that 'save' will be popped
      // from the stack at the appropriate time.
      stack_temporary<join_state *> push (target_stack, &save);

      try_block->visit (this);
      save.merge (current);
      accum = current;

      pre_statement.set_definite_unassignment_from (save.get_state ());

      for (std::list<ref_catch>::const_iterator i = catchers.begin ();
	   i != catchers.end ();
	   ++i)
	{
	  current = pre_statement;
	  (*i)->visit (this);
	  save.merge (current);
	  accum &= current;
	}
    }

    if (finally)
      {
	current = pre_statement;
	current.set_definite_unassignment_from (save.get_state ());
	finally->visit (this);
	accum.merge_finally (current);
      }

    // Propagate to all pending targets, but only if the finally
    // clause completes normally.
    if (! finally || finally->can_complete_normally ())
      {
	for (join_state::pending_pairs_type::const_iterator i
	       = save.pending_pairs.begin ();
	     i != save.pending_pairs.end ();
	     ++i)
	  {
	    current = (*i).state;
	    current.set_definite_assignment_from (save.get_state (), accum);
	    propagate_branch ((*i).statement, (*i).is_continue);
	  }
      }

    current = accum;
  }

  void visit_variable_stmt (model_variable_stmt *,
			    const std::list<ref_variable_decl> &decls)
  {
    assert (visiting_lhs == NONE);

    // See if our nearest scope is a 'switch' statement.
    variable_state *local_switch_state = NULL;
    if (! target_stack.empty ()
	 && dynamic_cast<model_switch *> (target_stack.back ()->statement))
      {
	assert (switch_state);
	local_switch_state = switch_state;
      }

    for (std::list<ref_variable_decl>::const_iterator i = decls.begin ();
	 i != decls.end ();
	 ++i)
      {
	model_variable_decl *decl = (*i).get ();
	current.insert (decl);
	// Final variables declared in a switch block are treated as
	// if they are blank finals.  So, we add them to the outer
	// scope so that they show up properly in subsequent blocks.
	if (local_switch_state)
	  local_switch_state->insert (decl);

	ref_expression init = decl->get_initializer ();
	if (init)
	  {
	    visit (init);

	    visiting_lhs = SIMPLE_LHS;
	    handle_var_ref (decl, decl);
	    visiting_lhs = NONE;
	  }
      }
  }

  void visit_while (model_while *whilestmt, const ref_expression &cond,
		    const ref_stmt &statement)
  {
    variable_state pre_loop = current;
    join_state save (whilestmt, current);
    save.set_continue ();
    stack_temporary<join_state *> push (target_stack, &save);

    // Don't use our 'visit' wrapper, it does the wrong thing for
    // booleans here.
    cond->visit (this);
    save.merge (when_false);

    current = when_true;
    statement->visit (this);

    save.merge_continue (current);
    pre_loop.check_loop (whilestmt, save.get_continue_state ());
    current = save.get_state ();
  }

  void visit_array_initializer (model_array_initializer *,
				const ref_forwarding_type &,
				const std::list<ref_expression> &exprs)
  {
    visit (exprs);
  }

  void visit_array_ref (model_array_ref *ref,
			const ref_expression &array,
			const ref_expression &index)
  {
    // Nothing to do for simple assignment's second phase.
    if (visiting_lhs == SIMPLE_LHS)
      return;

    visit (array);
    visit (index);

    if (ref->type () == primitive_boolean_type)
      {
	when_true = current;
	when_false = current;
      }
  }

  void visit_assignment (model_assignment *,
			 const ref_expression &lhs,
			 const ref_expression &rhs)
  {
    {
      lhs_type save = visiting_lhs;
      visiting_lhs = INITIAL_LHS;
      lhs->visit (this);
      visiting_lhs = save;
    }
    visit (rhs);
    visit (lhs, SIMPLE_LHS);
    if (lhs->type () == primitive_boolean_type)
      {
	when_true = current;
	when_false = current;
      }
  }

  // Helper for compound assignment.
  void handle_assignment (const ref_expression &lhs,
			  const ref_expression &rhs)
  {
    visit (lhs, COMPOUND_LHS);
    visit (rhs);
    if (lhs->type () == primitive_boolean_type)
      {
	when_true = current;
	when_false = current;
      }
  }

  void visit_op_assignment (model_minus_equal *,
			    const ref_expression &lhs,
			    const ref_expression &rhs)
  {
    handle_assignment (lhs, rhs);
  }

  void visit_op_assignment (model_mult_equal *,
			    const ref_expression &lhs,
			    const ref_expression &rhs)
  {
    handle_assignment (lhs, rhs);
  }

  void visit_op_assignment (model_div_equal *,
			    const ref_expression &lhs,
			    const ref_expression &rhs)
  {
    handle_assignment (lhs, rhs);
  }

  void visit_op_assignment (model_and_equal *,
			    const ref_expression &lhs,
			    const ref_expression &rhs)
  {
    handle_assignment (lhs, rhs);
  }

  void visit_op_assignment (model_or_equal *,
			    const ref_expression &lhs,
			    const ref_expression &rhs)
  {
    handle_assignment (lhs, rhs);
  }

  void visit_op_assignment (model_plus_equal *,
			    const ref_expression &lhs,
			    const ref_expression &rhs)
  {
    handle_assignment (lhs, rhs);
  }

  void visit_op_assignment (model_xor_equal *,
			    const ref_expression &lhs,
			    const ref_expression &rhs)
  {
    handle_assignment (lhs, rhs);
  }

  void visit_op_assignment (model_mod_equal *,
			    const ref_expression &lhs,
			    const ref_expression &rhs)
  {
    handle_assignment (lhs, rhs);
  }

  void visit_op_assignment (model_ls_equal *,
			    const ref_expression &lhs,
			    const ref_expression &rhs)
  {
    handle_assignment (lhs, rhs);
  }

  void visit_op_assignment (model_rs_equal *,
			    const ref_expression &lhs,
			    const ref_expression &rhs)
  {
    handle_assignment (lhs, rhs);
  }

  void visit_op_assignment (model_urs_equal *,
			    const ref_expression &lhs,
			    const ref_expression &rhs)
  {
    handle_assignment (lhs, rhs);
  }

  void visit_arith_binary (model_minus *,
			   const ref_expression &lhs,
			   const ref_expression &rhs)
  {
    visit (lhs);
    visit (rhs);
  }

  void visit_arith_binary (model_mult *,
			   const ref_expression &lhs,
			   const ref_expression &rhs)
  {
    visit (lhs);
    visit (rhs);
  }

  void visit_arith_binary (model_div *,
			   const ref_expression &lhs,
			   const ref_expression &rhs)
  {
    visit (lhs);
    visit (rhs);
  }

  void visit_arith_binary (model_mod *,
			   const ref_expression &lhs,
			   const ref_expression &rhs)
  {
    visit (lhs);
    visit (rhs);
  }

  void visit_arith_binary (model_and *,
			   const ref_expression &lhs,
			   const ref_expression &rhs)
  {
    visit (lhs);
    visit (rhs);
  }

  void visit_arith_binary (model_or *,
			   const ref_expression &lhs,
			   const ref_expression &rhs)
  {
    visit (lhs);
    visit (rhs);
  }

  void visit_arith_binary (model_xor *,
			   const ref_expression &lhs,
			   const ref_expression &rhs)
  {
    visit (lhs);
    visit (rhs);
  }

  void visit_arith_binary (model_plus *,
			   const ref_expression &lhs,
			   const ref_expression &rhs)
  {
    visit (lhs);
    visit (rhs);
  }

  void visit_arith_shift (model_left_shift *,
			  const ref_expression &lhs,
			  const ref_expression &rhs)
  {
    visit (lhs);
    visit (rhs);
  }

  void visit_arith_shift (model_right_shift *,
			  const ref_expression &lhs,
			  const ref_expression &rhs)
  {
    visit (lhs);
    visit (rhs);
  }

  void visit_arith_shift (model_unsigned_right_shift *,
			  const ref_expression &lhs,
			  const ref_expression &rhs)
  {
    visit (lhs);
    visit (rhs);
  }

  void visit_cast (model_cast *, const ref_forwarding_type &,
		   const ref_expression &expr)
  {
    // We don't use our 'visit' helper as the context may be boolean.
    assert (visiting_lhs == NONE);
    expr->visit (this);
  }

  void visit_class_ref (model_class_ref *, const ref_forwarding_type &)
  {
  }

  void handle_comparison (const ref_expression &lhs,
			  const ref_expression &rhs)
  {
    // FIXME: what about a comparison of booleans?
    visit (lhs);
    visit (rhs);
    when_false = current;
    when_true = current;
  }

  void visit_comparison (model_equal *,
			 const ref_expression &lhs,
			 const ref_expression &rhs)
  {
    handle_comparison (lhs, rhs);
  }

  void visit_comparison (model_notequal *,
			 const ref_expression &lhs,
			 const ref_expression &rhs)
  {
    handle_comparison (lhs, rhs);
  }

  void visit_comparison (model_lessthan *,
			 const ref_expression &lhs,
			 const ref_expression &rhs)
  {
    handle_comparison (lhs, rhs);
  }

  void visit_comparison (model_greaterthan *,
			 const ref_expression &lhs,
			 const ref_expression &rhs)
  {
    handle_comparison (lhs, rhs);
  }

  void visit_comparison (model_lessthanequal *,
			 const ref_expression &lhs,
			 const ref_expression &rhs)
  {
    handle_comparison (lhs, rhs);
  }

  void visit_comparison (model_greaterthanequal *,
			 const ref_expression &lhs,
			 const ref_expression &rhs)
  {
    handle_comparison (lhs, rhs);
  }

  void visit_conditional (model_conditional *,
			  const ref_expression &cond,
			  const ref_expression &true_expr,
			  const ref_expression &false_expr)
  {
    assert (visiting_lhs == NONE);
    cond->visit (this);

    variable_state save = when_false;
    current = when_true;
    if (true_expr->type () == primitive_boolean_type)
      {
	true_expr->visit (this);
	variable_state save_true = when_true;
	variable_state save_false = when_false;
	current = save;
	false_expr->visit (this);
	when_true &= save_true;
	when_false &= save_false;
      }
    else
      {
	visit (true_expr);
	std::swap (current, save);
	visit (false_expr);
	current &= save;
      }
  }

  void visit_field_ref (model_field_ref *ref,
			const ref_expression &expr,
			model_field *fld)
  {
    // For first phase of simple assignment, visit the expression.
    // For second phase, do everything else.  For ordinary references,
    // do everything.
    if (expr && visiting_lhs != SIMPLE_LHS)
      visit (expr);
    if (visiting_lhs == INITIAL_LHS)
      return;

    model_field *fld2 = (model_field *) fld;
    if (fields.find (fld2) != fields.end () && fld->final_p ())
      {
	if (fld->static_p ())
	  {
	    if (expr)
	      {
		if (visiting_lhs != NONE)
		  std::cerr << ref->error ("%<final%> field must be assigned "
					   "using simple name");
	      }
	    else if (visiting_lhs != NONE && ! static_init)
	      std::cerr << ref->error ("%<final%> field cannot be assigned "
				       "outside of static initializer");
	    else if (fld->get_declaring_class () == current_class
		     && ! ref->qualified_p ())
	      handle_var_ref (ref, fld);
	  }
	else
	  {
	    model_this *thx = dynamic_cast<model_this *> (expr.get ());
	    if (thx == NULL || thx->type () != current_class)
	      {
		if (visiting_lhs != NONE)
		  std::cerr << ref->error ("%<final%> field must be assigned "
					   "using simple name or %<this%>");
	      }
	    else if (visiting_lhs != NONE && ! constructor)
	      std::cerr << ref->error ("%<final%> field cannot be assigned "
				       "outside of constructor or "
				       "initializer");
	    else if (fld->get_declaring_class () == current_class)
	      handle_var_ref (ref, fld);
	  }
      }
    else if (visiting_lhs != NONE && fld->final_p ())
      {
	if (fld->static_p () && ! static_init)
	  std::cerr << ref->error ("%<final%> field cannot be assigned "
				   "outside of static initializer");
	else if (! fld->static_p () && ! constructor)
	  std::cerr << ref->error ("%<final%> field cannot be assigned "
				   "outside of constructor or "
				   "initializer");
      }

    if (((constructor && ! fld->static_p ())
	 || (static_init && fld->static_p ()))
	&& ! fld->synthetic_p ())
      {
	if (in_init
	    // There might be an assignment on the RHS of a field
	    // initializer.
	    && visiting_lhs != SIMPLE_LHS
	    && decl_fields.find (fld2) == decl_fields.end ()
	    && fld->get_declaring_class () == current_class
	    && ! ref->qualified_p ())
	  std::cerr << ref->error ("field %1 used before declaration")
	    % fld2;
      }

    if (fld->type () == primitive_boolean_type)
      {
	when_true = current;
	when_false = current;
      }
  }

  void visit_field_initializer (model_field_initializer *init,
				model_field *field)
  {
    if (field->static_p () && field->constant_p ())
      {
	// Ignore constant fields.
	return;
      }

    assert (visiting_lhs == NONE);
    if (field->has_initializer_p ())
      visit (field->get_initializer ());

    decl_fields.insert (field);

    // Any non-synthetic field here is a candidate for checking.
    if (field->has_initializer_p ())
      {
	visiting_lhs = SIMPLE_LHS;
	handle_var_ref (init, field);
	visiting_lhs = NONE;
      }
  }

  void visit_field (model_field *)
  {
    // Nothing.
  }

  void visit_instanceof (model_instanceof *,
			 const ref_expression &expr,
			 const ref_forwarding_type &)
  {
    visit (expr);
    when_true = current;
    when_false = current;
  }

  void visit_logical_binary (model_lor *,
			     const ref_expression &lhs,
			     const ref_expression &rhs)
  {
    assert (visiting_lhs == NONE);
    lhs->visit (this);
    variable_state save = when_true;
    current = when_false;
    rhs->visit (this);
    when_true &= save;
  }

  void visit_logical_binary (model_land *,
			     const ref_expression &lhs,
			     const ref_expression &rhs)
  {
    assert (visiting_lhs == NONE);
    lhs->visit (this);
    variable_state save = when_false;
    current = when_true;
    rhs->visit (this);
    when_false &= save;
  }

  void visit_simple_literal (model_literal_base *, const jboolean &val)
  {
    if (val)
      {
	// Double assignment here to make sure we get the correct
	// values.
	when_false = current;
	when_false = true;
	when_true = current;
      }
    else
      {
	when_false = current;
	// Double assignment here to make sure we get the correct
	// values.
	when_true = current;
	when_true = true;
      }
  }

  void visit_simple_literal (model_literal_base *, const jbyte &)
  {
    // Nothing.
  }

  void visit_simple_literal (model_literal_base *, const jchar &)
  {
    // Nothing.
  }

  void visit_simple_literal (model_literal_base *, const jshort &)
  {
    // Nothing.
  }

  void visit_simple_literal (model_literal_base *, const jint &)
  {
    // Nothing.
  }

  void visit_simple_literal (model_literal_base *, const jlong &)
  {
    // Nothing.
  }

  void visit_simple_literal (model_literal_base *, const jfloat &)
  {
    // Nothing.
  }

  void visit_simple_literal (model_literal_base *, const jdouble &)
  {
    // Nothing.
  }

  void visit_string_literal (model_string_literal *, const std::string &)
  {
    // Nothing.
  }

  void visit_method_invocation (model_method_invocation *,
				model_method *meth,
				const ref_expression &qual,
				const std::list<ref_expression> &exprs)
  {
    if (qual)
      visit (qual);
    visit (exprs);

    if (meth->instance_initializer_p ())
      {
	assert (constructor);
	assert (current_class == meth->get_declaring_class ());
	// Recurse into this method, as this will let us handle all
	// field initializations properly.
	model_method *m2 = (model_method *) meth;
	m2->visit (this);
      }

    if (meth->get_return_type () == primitive_boolean_type)
      {
	when_true = current;
	when_false = current;
      }
  }

  void visit_type_qualified_invocation (model_type_qualified_invocation *,
					const model_method *meth,
					const std::list<ref_expression> &exprs,
					bool)
  {
    visit (exprs);

    if (meth->get_return_type () == primitive_boolean_type)
      {
	when_true = current;
	when_false = current;
      }
  }

  void visit_super_invocation (model_super_invocation *,
			       const model_method *,
			       const std::list<ref_expression> &exprs)
  {
    assert (constructor);
    visit (exprs);
  }

  void visit_this_invocation (model_this_invocation *,
			      const model_method *,
			      const std::list<ref_expression> &exprs)
  {
    assert (constructor);
    visit (exprs);

    // Any fields declared in this class will have been set by the
    // alternate constructor.
    for (std::set<model_field *>::const_iterator i = fields.begin ();
	 i != fields.end ();
	 ++i)
      current.set_assign (*i);
  }

  void visit_new (model_new *, const model_method *,
		  const ref_forwarding_type &,
		  const std::list<ref_expression> &exprs)
  {
    visit (exprs);
  }

  void visit_new_array (model_new_array *,
			const ref_forwarding_type &,
			const std::list<ref_expression> &exprs,
			const ref_expression &init)
  {
    visit (exprs);
    if (init)
      visit (init);
  }

  void visit_null_literal (model_null_literal *)
  {
    // Nothing.
  }

  void visit_prefix_simple (model_prefix_plus *, const ref_expression &expr)
  {
    visit (expr);
  }

  void visit_prefix_simple (model_prefix_minus *, const ref_expression &expr)
  {
    visit (expr);
  }

  void visit_prefix_simple (model_bitwise_not *, const ref_expression &expr)
  {
    visit (expr);
  }

  void visit_prefix_simple (model_logical_not *, const ref_expression &expr)
  {
    assert (visiting_lhs == NONE);
    expr->visit (this);
    std::swap (when_false, when_true);
  }

  void visit_prefix_side_effect (model_prefix_plusplus *,
				 const ref_expression &expr)
  {
    visit (expr, COMPOUND_LHS);
  }

  void visit_prefix_side_effect (model_prefix_minusminus *,
				 const ref_expression &expr)
  {
    visit (expr, COMPOUND_LHS);
  }

  void visit_postfix_side_effect (model_postfix_plusplus *,
				  const ref_expression &expr)
  {
    visit (expr, COMPOUND_LHS);
  }

  void visit_postfix_side_effect (model_postfix_minusminus *,
				  const ref_expression &expr)
  {
    visit (expr, COMPOUND_LHS);
  }

  void visit_this (model_this *)
  {
    // Nothing.
  }

  void handle_var_ref (model_element *elt,
		       const model_variable_decl *decl)
  {
    if (visiting_lhs != SIMPLE_LHS)
      {
	if (! current.assigned_p (decl))
	  std::cerr << elt->error ("variable %1 is not definitely "
				   "assigned at this point")
	    // FIXME: should have its own operator
	    % decl->get_name ();
      }

    if (visiting_lhs != NONE)
      {
	if (decl->final_p () && ! current.unassigned_p (decl))
	  std::cerr << elt->error ("%<final%> variable %1 is not "
				   "definitely unassigned at this point")
	    % decl->get_name ();
	current.set_assign (decl);
      }
  }

  void visit_simple_variable_ref (model_simple_variable_ref *ref,
				  const model_variable_decl *decl)
  {
    // Nothing to do here -- we'll be visited again.
    if (visiting_lhs == INITIAL_LHS)
      return;

    handle_var_ref (ref, decl);

    if (decl->type () == primitive_boolean_type)
      {
	when_true = current;
	when_false = current;
      }
  }

  void visit_forwarding_type (model_forwarding_type *, model_type *)
  {
    // Nothing.
  }

  void visit_forwarding_resolved (model_forwarding_resolved *,
                                  model_type *)
  {
    // Nothing.
  }

  void visit_forwarding_owned (model_forwarding_owned *,
                               const ref_type &)
  {
    // Nothing.
  }

  void visit_forwarding_simple (model_forwarding_simple *,
                                const std::list<std::string> &)
  {
    // Nothing.
  }

  void
  visit_forwarding_array (model_forwarding_array *,
                          const owner<model_forwarding_type> &)
  {
    // Nothing.
  }

  void
  visit_forwarding_element (model_forwarding_element *,
                            const owner<model_forwarding_type> &)
  {
    // Nothing.
  }

  void visit_forwarding_full (model_forwarding_full *,
                              const std::string &)
  {
    // Nothing.
  }

  void
  visit_forwarding_inner (model_forwarding_inner *,
                          const std::list<std::string> &,
                          const owner<model_forwarding_type> &)
  {
    // Nothing.
  }

  void
  visit_forwarding_parameterized (model_forwarding_parameterized *,
                                  const owner<model_forwarding_type> &,
                                  const std::list< owner<model_forwarding_type> > &)
  {
    // Nothing.
  }

  void visit_variable_decl (model_variable_decl *,
                            const std::string &,
                            const ref_forwarding_type &,
                            const ref_expression &,
                            bool,
                            bool)
  {
    // Nothing.
  }

  void visit_parameter_decl (model_variable_decl *,
                             const std::string &,
                             const ref_forwarding_type &,
                             const ref_expression &,
                             bool,
                             bool)
  {
    // Nothing.
  }

  void visit_catch_decl (model_variable_decl *,
                         const std::string &,
                         const ref_forwarding_type &,
                         const ref_expression &,
                         bool,
                         bool)
  {
    // Nothing.
  }

  void visit_package (model_package *, const std::list<std::string> &)
  {
    // Nothing.
  }

  void visit_primitive (model_primitive_base *, const char *)
  {
    // Nothing.
  }

  void visit_type (model_type *, const std::string &)
  {
    // Nothing.
  }

  void visit_identifier (model_identifier *, const std::string &)
  {
    // Nothing.
  }
  
  void visit_annotation_member (model_annotation_member *,
                                const ref_forwarding_type &)
  {
    // Nothing.
  }

  void visit_annotation_value (model_annotation_value *,
                               const std::string &, const ref_expression &)
  {
    // Nothing.
  }

  void visit_import_single (model_import_single *,
                            const std::list<std::string> &, model_class *)
  {
    // Nothing.
  }

  void visit_import_on_demand (model_import_on_demand *,
                               const std::list<std::string> &, Iname *, bool)
  {
    // Nothing.
  }

  void visit_static_import_single (model_static_import_single *,
				   const std::list<std::string> &,
				   model_class *, const std::string &)
  {
    // Nothing.
  }

  void visit_static_import_on_demand (model_static_import_on_demand *,
				      const std::list<std::string> &,
				      model_class *)
  {
    // Nothing.
  }
  
  void visit_unit_source (model_unit_source *, model_package *,
			  const std::list<ref_class> &,
			  const std::string &,
			  bool, const std::list<ref_import> &)
  {
    // Nothing.
  }
  
  void visit_unit_class (model_unit_class *, model_package *,
			 const std::list<ref_class> &,
			 const std::string &, bool)
  {
    // Nothing.
  }
  
  void visit_unit_fake (model_unit_fake *, model_package *,
			const std::list<ref_class> &,
			const std::string &, bool)
  {
    // Nothing.
  }

  // This is a helper for visiting a subexpression.  It handles
  // boolean expressions for the caller, merging final states if
  // needed.
  void visit (const ref_expression &expr, lhs_type is_lhs = NONE)
  {
    // There are no exceptions thrown in this module, so we just
    // explicitly handle state.
    lhs_type save = visiting_lhs;
    visiting_lhs = is_lhs;

    expr->visit (this);
    if (expr->type () == primitive_boolean_type)
      {
	current = when_true;
	current &= when_false;
      }

    visiting_lhs = save;
  }

  // Another helper, that calls our visit helper for a list of
  // expressions.
  void visit (const std::list<ref_expression> &exprs)
  {
    for (std::list<ref_expression>::const_iterator i = exprs.begin ();
	 i != exprs.end ();
	 ++i)
      visit (*i);
  }

  void propagate_branch (model_stmt *target = NULL,
			 bool is_continue = false)
  {
    for (std::deque<join_state *>::reverse_iterator i = target_stack.rbegin ();
	 i != target_stack.rend ();
	 ++i)
      {
	join_state *state = *i;

	// See if we hit the target.
	if (target == state->statement)
	  {
	    if (is_continue)
	      state->merge_continue (current);
	    else
	      state->merge (current);
	    // We're done.
	    break;
	  }

	// If we hit a 'try' statement, record the state as pending
	// and handle it when processing the 'try'.
	if (dynamic_cast<model_try *> (state->statement) != NULL)
	  {
	    state->add_pending (target, is_continue, current);
	    break;
	  }
      }
  }
};

void
check_definite_assignment (model_method *meth)
{
  // Instance initializers are handled specially by the visitor, as a
  // side effect of visiting constructors.
  if (meth->instance_initializer_p ())
    return;

  definite_assignment_visitor checker;
  meth->visit (&checker);
}

/* Tree lowering pass.  This pass simplifies the tree representation built
   by the C-based front ends.  The structure of simplified, or
   language-independent, trees is dictated by the grammar described in this
   file.
   Copyright (C) 2002 Free Software Foundation, Inc.
   Lowering of expressions contributed by Sebastian Pop <s.pop@laposte.net>
   Re-written to support lowering of whole function trees, documentation
   and miscellaneous cleanups by Diego Novillo <dnovillo@redhat.com>

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */

#include "config.h"
#include "system.h"
#include "tree.h"
#include "errors.h"
#include "varray.h"
#include "c-tree.h"
#include "c-common.h"
#include "tree-simple.h"
#include "diagnostic.h"

/** The simplification pass converts the language-dependent trees
    (ld-trees) emitted by the parser into language-independent trees
    (li-trees) that are the target of SSA analysis and transformations.  

    Language-independent trees are based on the SIMPLE intermediate
    representation used in the McCAT compiler framework:

    "Designing the McCAT Compiler Based on a Family of Structured
    Intermediate Representations,"
    L. Hendren, C. Donawa, M. Emami, G. Gao, Justiani, and B. Sridharan,
    Proceedings of the 5th International Workshop on Languages and
    Compilers for Parallel Computing, no. 757 in Lecture Notes in
    Computer Science, New Haven, Connecticut, pp. 406-420,
    Springer-Verlag, August 3-5, 1992.

    http://www-acaps.cs.mcgill.ca/info/McCAT/McCAT.html   */

/* {{{ Local declarations.  */

static tree simplify_for_stmt        PARAMS ((tree, tree *, tree *));
static tree simplify_while_stmt      PARAMS ((tree, tree *, tree *));
static tree simplify_do_stmt         PARAMS ((tree));
static void simplify_switch_stmt     PARAMS ((tree, tree *, tree *));
static tree simplify_decl_stmt       PARAMS ((tree, tree *));
static tree new_simplified_if        PARAMS ((tree, tree, tree, tree *, tree *,
                                              int));
static tree simplify_expr            PARAMS ((tree, tree *, tree *, tree *,
                                              int (*) PARAMS ((tree)), int));
static tree simplify_array_ref       PARAMS ((tree, tree *, tree *, tree *));
static tree simplify_self_mod_expr   PARAMS ((tree, tree *, tree *, tree *));
static tree simplify_component_ref   PARAMS ((tree, tree *, tree *, tree *));
static tree simplify_call_expr       PARAMS ((tree, tree *, tree *, tree *));
static tree simplify_arglist         PARAMS ((tree, tree *, tree *, tree *));
static tree simplify_cond_expr       PARAMS ((tree, tree *, tree *, tree *));
static tree simplify_modify_expr     PARAMS ((tree, tree *, tree *, tree *));
static void make_type_writable       PARAMS ((tree));
static void add_tree                 PARAMS ((tree, tree *));
static void add_assignment_tree      PARAMS ((tree, tree, tree *));
static void insert_after_case_labels PARAMS ((tree, tree, int));
static tree insert_before_continue   PARAMS ((tree, tree));
static tree tree_last_decl           PARAMS ((tree scope));
static int  is_binop                 PARAMS ((tree));
static int  is_unop                  PARAMS ((tree));

/* }}} */

/** {{{ simplify_stmt()
  
    Entry point for the tree lowering pass.  Recursively scan
    STMT and convert it to a SIMPLE tree.

    SCOPE points to the scope enclosing STMT.  */

void 
simplify_stmt (stmt, scope)
     tree stmt;
     tree scope;
{
  tree next, prev, before, after, new_vars;
  
  /* Don't bother doing anything if the program has errors.  */
  if (errorcount || sorrycount)
    return;

  /* BEFORE and AFTER are tree chains that contain the
     side-effects of the simplified tree.  For instance, given the
     expression tree:

     		c = a + b++;

     After simplification, the tree will be re-written as:

     		t1 = a + b;	<-- BEFORE
     		c = t1;
     		b = b + 1;	<-- AFTER

     NEW_VARS is a tree chain holding all the artificial variables 
     declared during simplification.  */

  prev = stmt;
  while (stmt && stmt != error_mark_node)
    {
      before = NULL;
      after = NULL;
      new_vars = NULL;
      next = TREE_CHAIN (stmt);
      
      switch (TREE_CODE (stmt))
	{
	case COMPOUND_STMT:
	  simplify_stmt (COMPOUND_BODY (stmt), scope);
	  
	  /* Next iteration.  */
	  prev = stmt;
	  stmt = next;
	  continue;
	  
	case SCOPE_STMT:
	  if (SCOPE_BEGIN_P (stmt))
	    /* Save a pointer to the scope.  */
	    scope = stmt; 
	    	  
	  /* Next iteration.  */
	  prev = stmt;
	  stmt = next;
	  continue;

	case FOR_STMT:
	  stmt = simplify_for_stmt (stmt, &before, &new_vars);
	  break;
	  
	case WHILE_STMT:
	  stmt = simplify_while_stmt (stmt, &before, &new_vars);
	  break;

	case DO_STMT:
	  stmt = simplify_do_stmt (stmt);
	  break;

	case IF_STMT:
	  /* Construct a simplified version of this IF_STMT.  */
	  stmt = new_simplified_if (IF_COND (stmt), 
				    THEN_CLAUSE (stmt),
				    ELSE_CLAUSE (stmt),
				    &before, &new_vars, 1);
	  break;
	  
	case SWITCH_STMT:
	  simplify_switch_stmt (stmt, &before, &new_vars);
	  break;

	case EXPR_STMT:
	  {
	    tree expr = TREE_OPERAND (stmt, 0);
	    if (!is_simple_expr (expr))
	      TREE_OPERAND (stmt, 0) = simplify_expr (expr, &before, &after,
		                                      &new_vars, NULL, 0);
	    break;
	  }

	case DECL_STMT:
	  /* Simplify all the declarations at once.  */
	  stmt = simplify_decl_stmt (stmt, &after);
	  
	  if (after)
	    /* Insert AFTER in the tree.  */
	    {
	      tree end_after;
	      /* Save a pointer to the next stmt.  */
	      next = TREE_CHAIN (stmt);
	      
	      /* Include the nodes from AFTER list.  */
	      TREE_CHAIN (stmt) = after;
	      
	      end_after = update_line_number (after, STMT_LINENO (stmt));
	      
	      /* Chain the end of the AFTER block to the next stmt.  */
	      TREE_CHAIN (end_after) = next;
	    }
	  /* else {
	     no initial values, so nothing to be simplified;
	     }.  */

	  /* Go to the next statement to be simplified.  */
	  prev = stmt;
	  stmt = TREE_CHAIN (stmt);
	  continue;

	case RETURN_STMT:
	  if (TREE_CODE (TREE_TYPE (TREE_TYPE (current_function_decl))) != VOID_TYPE
	      && RETURN_EXPR (stmt))
	    {
	      tree e = RETURN_EXPR (stmt);
	      tree expr = TREE_OPERAND (e, 1);
	      if (!is_simple_expr (expr))
		TREE_OPERAND (e, 1) = simplify_expr (expr, &before, &after,
						     &new_vars, NULL, 0);
	    }
	  break;

	case FILE_STMT:
	case LABEL_STMT:
	case GOTO_STMT:
	case ASM_STMT:
	case CASE_LABEL:
	case CONTINUE_STMT:
	case BREAK_STMT:
	  /* Nothing to do : next iteration.  */
	  prev = stmt;
	  stmt = next;
	  continue;

	default:
	  prep_stmt (stmt);
	  error ("unhandled statement node in simplify_stmt():");
	  fprintf (stderr, "\n");
	  debug_tree (stmt);
	  fprintf (stderr, "\n");
	  abort ();
	  break;
	}

      /* If we created new temporary variables, create declarations for
	 them in the current scope.  */
      if (new_vars)
	{
	  tree last = declare_tmp_vars (new_vars, scope);
	  
	  /* If the previous statement was a declaration or a scope
	     statement, we need to re-set it to point to the last
	     temporary variable declaration.  Otherwise, they will be
	     overwritten by the next iteration.  */
	  if (TREE_CODE (prev) == DECL_STMT
	      || TREE_CODE (prev) == SCOPE_STMT)
	    prev = last;
	}
      
      /* Update line number on statements created after simplification.  */
      update_line_number (before, STMT_LINENO (stmt));
      update_line_number (after, STMT_LINENO (stmt));
      
      /* There are three tree chains that need to be dealt with: BEFORE,
	 STMT and AFTER.  The following code rewires the chains as described by
	 the following schema :

	 PREV -> NULL
	 PREV -> BEFORE -> NULL
	 PREV -> BEFORE -> STMT -> NULL
	 PREV -> BEFORE -> STMT -> AFTER -> NULL
	 PREV -> BEFORE -> STMT -> AFTER -> NEXT -> ...  */
      
      TREE_CHAIN (prev) = NULL_TREE;
      TREE_CHAIN (stmt) = NULL_TREE;
      chainon (prev, before);
      chainon (prev, stmt);
      chainon (prev, after);

      /* Next iteration.  */
      prev = tree_last (prev);
      stmt = next;
      TREE_CHAIN (prev) = stmt;
    }
}

/* }}} */

/** {{{  simplify_for_stmt()

    Simplifies the given FOR_STMT.
    Return a pointer to the simplified version of the FOR_STMT.  */

static tree
simplify_for_stmt (stmt, before_p, vars)
     tree stmt;
     tree *before_p, *vars;
{
  tree cond, body, loop;
  tree new_scope;
  tree expr_before = NULL_TREE, expr_after = NULL_TREE;

  /* Make sure that the loop body has a scope.  */
  tree_build_scope (&FOR_BODY (stmt));

  /* Save some pointers.  */
  cond = FOR_COND (stmt);
  body = FOR_BODY (stmt);
  new_scope = COMPOUND_BODY (body);
  
  /* Simplify FOR_INIT_STMT.  */
  if (EXPR_STMT_EXPR (FOR_INIT_STMT (stmt)))
    {
      tree init_before = NULL_TREE, init_after = NULL_TREE;
      simplify_expr (EXPR_STMT_EXPR (FOR_INIT_STMT (stmt)), &init_before, 
		     &init_after, vars, NULL, 0);
      
      /* Insert the simplification chains before the beginning of the loop.  */
      *before_p = chainon (*before_p, init_before);
      if (TREE_CODE (EXPR_STMT_EXPR (FOR_INIT_STMT (stmt))) == COMPOUND_EXPR)
	*before_p = chainon (*before_p, FOR_INIT_STMT (stmt));
      *before_p = chainon (*before_p, init_after);
    }
  
  /* Simplify FOR_EXPR.  */
  if (FOR_EXPR (stmt))
    {
      tree new_scope_vars = NULL;
      simplify_expr (FOR_EXPR (stmt), &expr_before, &expr_after,
	             &new_scope_vars, NULL, 0);

      /* If we created new temporary variables, create declarations for
	 them in the loop's scope.  */
      if (new_scope_vars)
	declare_tmp_vars (new_scope_vars, new_scope);
    }

  /* Step 0 : Check wether the loop condition is already simplified.  */
  if (is_simple_expr (cond))
    {
      /* The FOR_COND is already simplified, simplify just the FOR_BODY.  */
      tree reeval;

      /* Step 1 : Insert a simplified version of 'reeval;' in loop's body.  */
      reeval = chainon (expr_before, expr_after);
      if (reeval)
	insert_before_continue_end (reeval, FOR_BODY (stmt),
	                            STMT_LINENO (stmt));
      
      /* Step 2 : Construct a 'while (cond) {body}' statement.  */
      simplify_stmt (body, NULL_TREE); return build_stmt (WHILE_STMT, 
			 /* WHILE_COND */ FOR_COND (stmt),
			 /* WHILE_BODY */ body);
    }

  /* Step 1 : Insert a simplified version of 
     'expr_before; expr_after; if (cond) {} else break;' 
     in loop's body.  */
  {
    tree if_stmt, if_vars = NULL, before_if = NULL;
    
    /* Build 'if (cond) {} else break;'.  */
    if_stmt = new_simplified_if (/* IF_COND */ copy_node (cond), 
				 /* THEN_CLAUSE */ NULL_TREE, 
				 /* ELSE_CLAUSE */ build_stmt (BREAK_STMT), 
				 &before_if, &if_vars, 0);

    /* If we created new temporary variables, create declarations for them in the
       loop body.  */
    if (if_vars)
      declare_tmp_vars (if_vars, new_scope);

    /* Insert the 'reeval; if_stmt;' before CONTINUE_STMT and at the 
       end of the loop body.  */
    {
      tree reeval;
      reeval = chainon (expr_before, expr_after);
      if (before_if)
	/* There are some statements to be executed before the IF_STMT.  */
	reeval = chainon (reeval, before_if);
      reeval = chainon (reeval, if_stmt);
      if (reeval)
	insert_before_continue_end (reeval, body, STMT_LINENO (stmt));
    }
  }
  
  /* Step 2 : Construct a 'while (1!=0) {body}' statement.  */
  loop = build_stmt (WHILE_STMT, 
		     /* WHILE_COND */ build (NE_EXPR, integer_type_node, 
					     integer_one_node, integer_zero_node),
		     /* WHILE_BODY */ body);

  /* Step 3 : Include the loop in an IF_STMT : 'if (cond) while (1){body} else {}'  */
  return new_simplified_if (/* IF_COND */ cond, 
			    /* THEN_CLAUSE */ loop, 
			    /* ELSE_CLAUSE */ NULL_TREE, 
			    before_p, vars, 1);
}

/* }}} */

/** {{{  simplify_while_stmt()

    Simplifies a WHILE_STMT, returns a pointer to the simplified version.  */

static tree
simplify_while_stmt (stmt, before_p, vars)
     tree stmt;
     tree *before_p, *vars;
{
  tree cond, body, loop;

  /* Make sure that the loop body has a scope.  */
  tree_build_scope (&WHILE_BODY (stmt));

  /* Save some pointers.  */
  cond = WHILE_COND (stmt);
  body = WHILE_BODY (stmt);

  /* Step 0 : Check wether the loop condition is already simplified.  */
  if (is_simple_expr (cond))
    /* Simplify just the WHILE_BODY.  */
    {
      simplify_stmt (body, NULL_TREE);
      return stmt;
    }

  /* Step 1 : Insert a simplified version of 'if (cond) {} else break;' in the 
     loop body.  */
  {
    tree if_stmt, if_vars = NULL, before_if = NULL, reeval;

    /* Build 'if (cond) {} else break;'.  */
    if_stmt = new_simplified_if (/* IF_COND */ copy_node (cond), 
				 /* THEN_CLAUSE */ NULL_TREE, 
				 /* ELSE_CLAUSE */ build_stmt (BREAK_STMT), 
				 &before_if, &if_vars, 0);
    
    if (if_vars)
      declare_tmp_vars (if_vars, COMPOUND_BODY (body));
    reeval = chainon (before_if, if_stmt);
    insert_before_continue_end (reeval, body, STMT_LINENO (stmt));
  }
  
  /* Step 2 : Construct a 'while (1!=0) {body}' statement.  */
  loop = build_stmt (WHILE_STMT, 
		     /* WHILE_COND */ build (NE_EXPR, integer_type_node, 
					     integer_one_node, integer_zero_node),
		     /* WHILE_BODY */ body);

  /* Step 3 : Include the loop in an IF_STMT : 'if (cond) while (1) {body} else {}'  */
  return new_simplified_if (/* IF_COND */ cond,
			    /* THEN_CLAUSE */ loop,
			    /* ELSE_CLAUSE */ NULL_TREE,
			    before_p, vars, 1);
}

/* }}} */

/** {{{ simplify_do_stmt()

   Simplify a DO_STMT node.
   Return a pointer to the last statement of the simplified region.  */


static tree
simplify_do_stmt (stmt)
     tree stmt;
{
  tree cond, body;

  /* Make sure that the loop body has a scope.  */
  tree_build_scope (&DO_BODY (stmt));

  /* Save some pointers.  */
  cond = DO_COND (stmt);
  body = DO_BODY (stmt);

  /* Step 0 : Check wether the loop condition is already simplified.  */
  if (is_simple_expr (cond))
    {
      /* Simplify just the DO_BODY.  */
      simplify_stmt (body, NULL_TREE);
      return stmt;
    }

  /* Step 1 : Insert a simplified version of 'if (cond) {} else break;' in the 
     loop body.  */
  {
    tree if_stmt, if_vars = NULL, before_if = NULL, reeval;
    
    /* Build 'if (cond) {} else break;'.  */
    if_stmt = new_simplified_if (/* IF_COND */ copy_node (cond), 
				 /* THEN_CLAUSE */ NULL_TREE, 
				 /* ELSE_CLAUSE */ build_stmt (BREAK_STMT), 
				 /* Before */ &before_if, 
				 /* Variables */ &if_vars, 0);

    if (if_vars)
      declare_tmp_vars (if_vars, COMPOUND_BODY (body));
    reeval = chainon (before_if, if_stmt);
    insert_before_continue_end (reeval, body, STMT_LINENO (stmt));
  }
  
  /* Step 2 : Construct a 'while (1!=0) {body}' statement.  */
  simplify_stmt (body, NULL_TREE);
  return build_stmt (WHILE_STMT, 
		     /* WHILE_COND */ build (NE_EXPR, integer_type_node, 
					     integer_one_node, integer_zero_node),
		     /* WHILE_BODY */ body);
}


/* }}} */

/** {{{ new_simplified_if()
    
    Constructs a new IF_STMT with its IF_COND under a simplified form.  If the
    SIMPLIFY parameter is not null, then IF clauses are simplified.  Returns
    the simplified IF_STMT and stores all side effects that should happen
    before the evaluation of the condition in BEFORE_P.  Side effects that
    should happen after the evaluation of the condition are stored directly in
    IF clauses.  */

static tree 
new_simplified_if (cond, then_clause, else_clause, before_p, vars, simplify_clauses)
     tree cond;
     tree then_clause;
     tree else_clause;
     tree *before_p;
     tree *vars;
     int simplify_clauses;
{

  /* Construct a scope for each clause.  */
  tree_build_scope (&then_clause);
  tree_build_scope (&else_clause);
  
  if (simplify_clauses)
    {
      simplify_stmt (then_clause, NULL_TREE);
      simplify_stmt (else_clause, NULL_TREE);
    }
      		
  switch (TREE_CODE (cond))
    /* Simplify IF_COND.  */
    {
    case TRUTH_ANDIF_EXPR:
      {
	tree op0, op1, res, before_if = NULL, if_vars = NULL;
	op0 = TREE_OPERAND (cond, 0);
	op1 = TREE_OPERAND (cond, 1);
	/* 'if (op0) { if (op1) {then_clause} else {else_clause}} else {else_clause}'. 
	   The ELSE_CLAUSE is duplicated here.  */
	res = new_simplified_if 
	  (/* IF_COND */ op0, 
	   /* THEN_CLAUSE */ new_simplified_if (op1, then_clause, else_clause, 
						&before_if, &if_vars, 0),
	   /* ELSE_CLAUSE */ copy_stmt_chain (else_clause),
	   /* Before */ before_p,
	   /* Variables */ vars, 
	   /* Simplify */ 0);
	
	if (if_vars)
	  /* We created new variables, declare them.  */
	  declare_tmp_vars (if_vars, COMPOUND_BODY (THEN_CLAUSE (res)));
	
	if (before_if)
	  /* Pre evaluation of the condition given some statements.  */
	  {
	    tree if_op1, last_decl;
	    last_decl = tree_last_decl (COMPOUND_BODY (THEN_CLAUSE (res)));
	    if_op1 = TREE_CHAIN (last_decl);
	    TREE_CHAIN (last_decl) = NULL_TREE;
	    chainon (last_decl, before_if);
	    chainon (last_decl, if_op1);
	  }
	
	return res;
      }

    case TRUTH_ORIF_EXPR:
      {
	tree op0, op1, res, before_if = NULL, if_vars = NULL;
	op0 = TREE_OPERAND (cond, 0);
	op1 = TREE_OPERAND (cond, 1);
	/* 'if (op0) {then_clause} else {if (op1) {then_clause} else {else_clause}}'.
	   The THEN_CLAUSE is duplicated here.  */
	res = new_simplified_if 
	  (/* IF_COND */ op0, 
	   /* THEN_CLAUSE */ copy_stmt_chain (then_clause),
	   /* ELSE_CLAUSE */ new_simplified_if (op1, then_clause, else_clause, 
						&before_if, &if_vars, 0), 
	   /* Before */ before_p,
	   /* Variables */ vars, 
	   /* Simplify */ 0);
	
	if (if_vars)
	  /* We created new variables, declare them.  */
	  declare_tmp_vars (if_vars, COMPOUND_BODY (ELSE_CLAUSE (res)));
	
	if (before_if)
	  /* Pre evaluation of the condition given some statements.  */
	  {
	    tree if_op1, last_decl;
	    last_decl = tree_last_decl (COMPOUND_BODY (ELSE_CLAUSE (res)));
	    if_op1 = TREE_CHAIN (last_decl);
	    TREE_CHAIN (last_decl) = NULL_TREE;
	    chainon (last_decl, before_if);
	    chainon (last_decl, if_op1);
	  }
	
	return res;
      }

    default:
      {
	tree res;
	tree cond_after = NULL;

	/* Build 'if (cond) {then_clause;} else {else_clause;}'.  */
	res = build_stmt (IF_STMT, cond, then_clause, else_clause);

	/* Simplify if condition.  */
	{
	  tree repl;

	  /* The PRE evaluation of the condition is lifted up to the caller.  */
	  repl = simplify_expr (cond, before_p, &cond_after, vars, NULL, 0);
	  
	  if (is_simple_expr (repl))
	    /* The simplification introduced a new temporary variable.  */
	    IF_COND (res) = build (NE_EXPR, integer_type_node, repl,
		                   integer_zero_node);
	}
		
	if (cond_after)
	  /* Insert the POST evaluation of the condition in both clauses.  */
	  {
	    tree tmp, last_decl;

	    last_decl = tree_last_decl (COMPOUND_BODY (THEN_CLAUSE (res)));
	    tmp = TREE_CHAIN (last_decl);
	    TREE_CHAIN (last_decl) = NULL_TREE;
	    chainon (last_decl, copy_stmt_chain (cond_after));
	    chainon (last_decl, tmp);

	    last_decl = tree_last_decl (COMPOUND_BODY (ELSE_CLAUSE (res)));
	    tmp = TREE_CHAIN (last_decl);
	    TREE_CHAIN (last_decl) = NULL_TREE;
	    chainon (last_decl, cond_after);
	    chainon (last_decl, tmp);
	  }
	
	return res;
      }
    }
}

/* }}} */

/** {{{ simplify_switch_stmt()

    Simplifies a SWITCH_STMT node.  FIXME.  */

static void
simplify_switch_stmt (stmt, before_p, new_vars_p)
     tree stmt;
     tree *before_p;
     tree *new_vars_p;
{
  tree after_sw = NULL;

  SWITCH_COND (stmt) = simplify_expr (SWITCH_COND (stmt), before_p,
				      &after_sw, new_vars_p, NULL, 0);

  tree_build_scope (&SWITCH_BODY (stmt));

  insert_after_case_labels (after_sw, SWITCH_BODY (stmt), STMT_LINENO (stmt));

  simplify_stmt (SWITCH_BODY (stmt), NULL_TREE);
}

/* }}} */

/** {{{ simplify_decl_stmt()

    Simplifies a DECL_STMT node T.  This function scans the list of all
    declarations starting with T looking for initializers.
    
    If a declaration V has an initial value I, the expression 'V = I' is
    added to the AFTER_P list.  On return, all the statements in AFTER_P
    will be inserted immediately after all the DECL_STMTs in the same order
    that they appeared originally.

    The function returns the last DECL_STMT found.  */

static tree
simplify_decl_stmt (t, after_p)
     tree t;
     tree *after_p;
{
  tree prev;

  if (TREE_CODE (t) != DECL_STMT)
    abort ();

  prev = t;
  while (t && TREE_CODE (t) == DECL_STMT)
    {
      tree decl = DECL_STMT_DECL (t);

      /* If there is an initializer for the variable,  */
      if (DECL_INITIAL (decl)
	  /* and if the declaration is not declared as a const,  */	  
	  && !TREE_READONLY (decl))
	/* queue it up to be processed after all the declarations.  */
	{
	  add_assignment_tree (decl, DECL_INITIAL (decl), after_p);
	  DECL_INITIAL (decl) = NULL_TREE;
	}

      prev = t;
      t = TREE_CHAIN (t);
    }

  return prev;
}

/* }}} */

/** {{{ simplify_expr()

    Simplifies the expression tree rooted at EXPR.  Returns the simplified
    version of EXPR.

    BEFORE_P points to the list where side effects that must happen before
	EXPR should be stored.

    AFTER_P points to the list where side effects that must happen after
	EXPR should be stored.

    NEW_VARS_P points to the list where any temporary VAR_DECLs needed to
	model EXPR's side effects should be stored.

    SIMPLE_TEST_F points to a function that takes a tree T and
	returns non-zero if T is in the SIMPLE form requested by the
	caller.  The SIMPLE predicates are in tree-simple.c.

	This test is used twice.  Before simplification, the test is
	invoked to determine whether EXPR is already simple enough.  If
	that fails, EXPR is simplified according to its code and
	SIMPLE_TEST_F is called again.  If the test still fails, then a new
	temporary variable is created and assigned the value of the
	simplified expression (this is always safe because the
	simplification process guarantees that the simplified expression
	can be used as the RHS of a SIMPLE assignment).

	If SIMPLE_TEST_F is NULL, the predicate 'is_simple_expr()' is
	used.

    IS_LHS should be non-zero if the simplified expression needs to be
	modifiable.  This is relevant when we need to create a temporary
	variable.  If IS_LHS is true and a temporary variable needs to be
	created, then a new temporary pointer will be created.  Otherwise,
	assigning a value to the temporary variable will not modify the
	original expression.  */

static tree
simplify_expr (expr, before_p, after_p, new_vars_p, simple_test_f, is_lhs)
     tree expr;
     tree *before_p;
     tree *after_p;
     tree *new_vars_p;
     int (*simple_test_f) PARAMS ((tree));
     int is_lhs;
{
  enum tree_code code;
  tree expr_s, tmp, type;

  if (expr == NULL_TREE)
    abort ();

  if (simple_test_f == NULL)
    simple_test_f = is_simple_expr;

  if ((*simple_test_f) (expr))
    return expr;

  code = TREE_CODE (expr);

  /* First deal with the special cases.  */
  switch (code)
    {
    case POSTINCREMENT_EXPR:
    case POSTDECREMENT_EXPR:
    case PREINCREMENT_EXPR:
    case PREDECREMENT_EXPR:
      expr_s = simplify_self_mod_expr (expr, before_p, after_p, new_vars_p);
      break;

    case ARRAY_REF:
      expr_s = simplify_array_ref (expr, before_p, after_p, new_vars_p);
      break;

    case COMPONENT_REF:
      expr_s = simplify_component_ref (expr, before_p, after_p, new_vars_p);
      break;
      
    case COND_EXPR:
      expr_s = simplify_cond_expr (expr, before_p, after_p, new_vars_p);
      break;

    case CALL_EXPR:
      expr_s = simplify_call_expr (expr, before_p, after_p, new_vars_p);
      break;

    case TREE_LIST:
      expr_s = simplify_arglist (expr, before_p, after_p, new_vars_p);
      break;

    case STMT_EXPR:
      simplify_stmt (STMT_EXPR_STMT (expr), NULL_TREE);
      /* Special case.  Return before anaylzing the expression any
	 further.  Simplifying the body is enough.  */
      return expr;

    case MODIFY_EXPR:
    case INIT_EXPR:
      expr_s = simplify_modify_expr (expr, before_p, after_p, new_vars_p);
      break;

    default:
      {
	/* If EXPR does not need to be special-cased, handle it according to
	   its class.  */
	expr_s = expr;

	if (is_unop (expr_s))
	  {
	    tree t;

	    t = simplify_expr (TREE_OPERAND (expr_s, 0), before_p, after_p,
		               new_vars_p, is_simple_unary_expr, 0);
	    TREE_OPERAND (expr_s, 0) = t;
	  }
	else if (is_binop (expr_s))
	  {
	    tree lhs, rhs;

	    lhs = simplify_expr (TREE_OPERAND (expr_s, 0), before_p, after_p,
			         new_vars_p, is_simple_val, 0);

	    rhs = simplify_expr (TREE_OPERAND (expr_s, 1), before_p, after_p,
				 new_vars_p, is_simple_val, 0);

	    TREE_OPERAND (expr_s, 0) = lhs;
	    TREE_OPERAND (expr_s, 1) = rhs;
	  }
	else
	  {
	    error ("unhandled expression in simplify_expr():");
	    print_node_brief (stderr, "", expr_s, 0);
	    fputs ("\n", stderr);
	    debug_c_node (expr_s);
	    fputs ("\n", stderr);
	    abort ();
	  }
      }
    }

  /* Test the simplified expression, if it's sufficiently simple already,
     return it.  */
  if ((*simple_test_f) (expr_s))
    return expr_s;
  
  /* Otherwise, we need to create a new temporary to hold the simplified
     expression.  */
  if (!is_lhs)
    {
      /* The original expression is not a LHS, create a new temporary
	 variable T, emit 'T = EXPR_S' and return T.  */
      type = TREE_TYPE (expr_s);
      tmp = create_tmp_var (type, new_vars_p);
      add_assignment_tree (tmp, expr_s, before_p);
      expr_s = tmp;
    }
  else
    {
      /* The original expression was a LHS.  Create a temporary pointer T,
	 get the address of the simplified expression (T = &EXPR_S) and
	 return a reference to T (*T).  */
      type = TREE_TYPE (expr_s);
      if (POINTER_TYPE_P (type))
	{
	  /* If the expression is a pointer already, there is no need to
	     take the address of the expression.  */
	  tmp = create_tmp_var (type, new_vars_p);
	  add_assignment_tree (tmp, expr_s, before_p);
	  expr_s = tmp;
	}
      else
	{
	  /* Take the address of the simplified expression and return a
	     reference to it.  */
	  type = build_pointer_type (type);
	  tmp = create_tmp_var (type, new_vars_p);
	  add_assignment_tree (tmp, build1 (ADDR_EXPR, type, expr_s), before_p);
	  expr_s = build1 (INDIRECT_REF, type, tmp);
	}
    }


  return expr_s;
}

/* }}} */

/** {{{ simplify_array_ref ()

    Re-write EXPR (assumed to be an ARRAY_REF) so that it conforms to the
    SIMPLE grammar:

     arrayref
	    : varname reflist

     reflist
	    : '[' val ']'
	    | reflist '[' val ']'

    BEFORE_P points to the list where side effects that must happen before
	EXPR should be stored.

    AFTER_P points to the list where side effects that must happen after
	EXPR should be stored.

    NEW_VARS_P points to the list where any temporary VAR_DECLs needed to
	model EXPR's side effects should be stored.  */

static tree
simplify_array_ref (expr, before_p, after_p, new_vars_p)
     tree expr;
     tree *before_p;
     tree *after_p;
     tree *new_vars_p;
{
  tree base;
  varray_type dim_stack;

  if (TREE_CODE (expr) != ARRAY_REF)
    abort ();

  VARRAY_GENERIC_PTR_INIT (dim_stack, 10, "dim_stack");

  /* First find and simplify the base of the array.  While looking
     for the base of the array, push all the dimensions along the
     way, so that they can be simplified from left to right after the
     base.  */
  base = expr;
  VARRAY_PUSH_GENERIC_PTR (dim_stack, (PTR)&(TREE_OPERAND (expr, 1)));
  while (TREE_CODE (TREE_OPERAND (base, 0)) == ARRAY_REF)
    {
      base = TREE_OPERAND (base, 0);
      VARRAY_PUSH_GENERIC_PTR (dim_stack, (PTR)&(TREE_OPERAND (base, 1)));
    }

  /* After the loop above, 'base' contains the leftmost ARRAY_REF,
     and 'dim_stack' is a stack of pointers to all the dimensions in left
     to right order (the leftmost dimension is at the top of the
     stack).  */
  TREE_OPERAND (base, 0) = simplify_expr (TREE_OPERAND (base, 0), before_p,
                                          after_p, new_vars_p,
					  is_simple_varname, 1);

  /* Now simplify each of the dimensions from left to right.  */
  while (VARRAY_ACTIVE_SIZE (dim_stack) > 0)
    {
      tree *dim_p = (tree *)VARRAY_TOP_GENERIC_PTR (dim_stack);
      VARRAY_POP (dim_stack);
      *dim_p = simplify_expr (*dim_p, before_p, after_p, new_vars_p,
	                      is_simple_val, 0);
    }

  VARRAY_FREE (dim_stack);

  /* Return the simplified array expression.  */
  return expr;
}

/* }}} */

/** {{{ simplify_self_mod_expr ()

    Simplify self modifying expression EXPR (++, --, +=, -=).

    BEFORE_P points to the list where side effects that must happen before
	EXPR should be stored.

    AFTER_P points to the list where side effects that must happen after
	EXPR should be stored.

    NEW_VARS_P points to the list where any temporary VAR_DECLs needed to
	model EXPR's side effects should be stored.  */

static tree
simplify_self_mod_expr (expr, before_p, after_p, new_vars_p)
     tree expr;
     tree *before_p;
     tree *after_p;
     tree *new_vars_p;
{
  enum tree_code code;
  tree lhs, rhs, t1;

  code = TREE_CODE (expr);

  if (code != POSTINCREMENT_EXPR
      && code != POSTDECREMENT_EXPR
      && code != PREINCREMENT_EXPR
      && code != PREDECREMENT_EXPR)
    abort ();

  lhs = simplify_expr (TREE_OPERAND (expr, 0), before_p, after_p, new_vars_p,
                       is_simple_val, 1);

  rhs = simplify_expr (TREE_OPERAND (expr, 1), before_p, after_p, new_vars_p,
                       is_simple_val, 0);

  /* Determine whether we need to create a PLUS or a MINUS operation.  */
  if (code == PREINCREMENT_EXPR || code == POSTINCREMENT_EXPR)
    t1 = build (PLUS_EXPR, TREE_TYPE (expr), lhs, rhs);
  else
    t1 = build (MINUS_EXPR, TREE_TYPE (expr), lhs, rhs);

  /* Determine whether the new assignment should go before or after
     the simplified statement.  */
  if (code == PREINCREMENT_EXPR || code == PREDECREMENT_EXPR)
    add_assignment_tree (lhs, t1, before_p);
  else
    add_assignment_tree (lhs, t1, after_p);

  /* Return the result of the simplified expression.  */
  return lhs;
}

/* }}} */

/** {{{ simplify_component_ref()
    
    Simplify the structure reference given by EXPR to comply with:

      compref
	      : '(' '*' ID ')' '.' idlist
	      | idlist

      idlist
	      : idlist '.' ID
	      | ID

    BEFORE_P points to the list where side effects that must happen before
	EXPR should be stored.

    AFTER_P points to the list where side effects that must happen after
    	EXPR should be stored.

    NEW_VARS_P points to the list where any temporary VAR_DECLs needed to
	model EXPR's side effects should be stored.  */

static tree
simplify_component_ref (expr, before_p, after_p, new_vars_p)
     tree expr;
     tree *before_p;
     tree *after_p;
     tree *new_vars_p;
{
  tree lhs, rhs;

  if (TREE_CODE (expr) != COMPONENT_REF)
    abort ();

  /* Note that we always re-write into a SIMPLE address expression to avoid
     bitwise structure copies.  */
  lhs = simplify_expr (TREE_OPERAND (expr, 0), before_p, after_p, new_vars_p,
                       is_simple_compref_lhs, 1);

  rhs = simplify_expr (TREE_OPERAND (expr, 1), before_p, after_p, new_vars_p,
                       is_simple_id, 0);

  TREE_OPERAND (expr, 0) = lhs;
  TREE_OPERAND (expr, 1) = rhs;

  return expr;
}

/* }}} */

/** {{{ simplify_call_expr ()

    Simplify the function call given by EXPR to comply with:

      call_expr
	      : ID '(' arglist ')'

      arglist
	      : arglist ',' val
	      | val

    BEFORE_P points to the list where side effects that must happen before
	EXPR should be stored.

    AFTER_P points to the list where side effects that must happen after
    	EXPR should be stored.

    NEW_VARS_P points to the list where any temporary VAR_DECLs needed to
	model EXPR's side effects should be stored.  */

static tree
simplify_call_expr (expr, before_p, after_p, new_vars_p)
     tree expr;
     tree *before_p;
     tree *after_p;
     tree *new_vars_p;
{
  tree id, arglist;

  if (TREE_CODE (expr) != CALL_EXPR)
    abort ();

  id = simplify_expr (TREE_OPERAND (expr, 0), before_p, after_p, new_vars_p,
                      is_simple_id, 0);

  arglist = simplify_expr (TREE_OPERAND (expr, 1), before_p, after_p,
                           new_vars_p, is_simple_arglist, 0);

  TREE_OPERAND (expr, 0) = id;
  TREE_OPERAND (expr, 1) = arglist;

  return expr;
}

/* }}} */

/** {{{ simplify_arglist ()

    Simplify the argument list given by EXPR to comply with:

      arglist
	      : arglist ',' val
	      | val

    BEFORE_P points to the list where side effects that must happen before
	EXPR should be stored.

    AFTER_P points to the list where side effects that must happen after
    	EXPR should be stored.

    NEW_VARS_P points to the list where any temporary VAR_DECLs needed to
	model EXPR's side effects should be stored.  */

static tree
simplify_arglist (expr, before_p, after_p, new_vars_p)
     tree expr;
     tree *before_p;
     tree *after_p;
     tree *new_vars_p;
{
  tree op;

  if (TREE_CODE (expr) != TREE_LIST)
    abort ();

  for (op = expr; op; op = TREE_CHAIN (op))
    {
      tree arg = simplify_expr (TREE_VALUE (op), before_p, after_p, new_vars_p,
	                        is_simple_val, 0);
      TREE_VALUE (op) = arg;
    }

  return expr;
}

/* }}} */

/** {{{ simplify_cond_expr ()

    Convert the conditional expression EXPR '(p) ? a : b;' into

    if (p)
      t1 = a;
    else
      t1 = b;
    t1;

    BEFORE_P points to the list where side effects that must happen before
	EXPR should be stored.

    AFTER_P points to the list where side effects that must happen after
    	EXPR should be stored.

    NEW_VARS_P points to the list where any temporary VAR_DECLs needed to
	model EXPR's side effects should be stored.  */

static tree
simplify_cond_expr (expr, before_p, after_p, new_vars_p)
     tree expr;
     tree *before_p;
     tree *after_p;
     tree *new_vars_p;
{
  tree t1, t2, tmp, pred, tval, fval, new_if;

  tmp = create_tmp_var (TREE_TYPE (expr), new_vars_p);

  pred = simplify_expr (TREE_OPERAND (expr, 0), before_p, after_p, new_vars_p,
                        NULL, 0);

  tval = simplify_expr (TREE_OPERAND (expr, 1), before_p, after_p, new_vars_p,
                        NULL, 0);

  fval = simplify_expr (TREE_OPERAND (expr, 2), before_p, after_p, new_vars_p,
                        NULL, 0);

  /* Build the THEN_CLAUSE.  */
  t1 = build_modify_expr (tmp, NOP_EXPR, tval);
  t1 = build_stmt (EXPR_STMT, t1);

  /* Build the ELSE_CLAUSE.  */
  t2 = build_modify_expr (tmp, NOP_EXPR, fval);
  t2 = build_stmt (EXPR_STMT, t2);

  /* Build a new simplified IF_STMT, and insert it in the BEFORE_P chain.  */
  new_if = new_simplified_if (pred, t1, t2, before_p, new_vars_p, 0);
  add_tree (new_if, before_p);

  return tmp;
}

/* }}} */

/** {{{ simplify_modify_expr ()

    Simplify the assignment given in EXPR to comply with:

      modify_expr
	      : varname '=' rhs
	      | '*' ID '=' rhs


    BEFORE_P points to the list where side effects that must happen before
	EXPR should be stored.

    AFTER_P points to the list where side effects that must happen after
    	EXPR should be stored.

    NEW_VARS_P points to the list where any temporary VAR_DECLs needed to
	model EXPR's side effects should be stored.  */

static tree
simplify_modify_expr (expr, before_p, after_p, new_vars_p)
     tree expr;
     tree *before_p;
     tree *after_p;
     tree *new_vars_p;
{
  tree lhs, rhs;

  if (TREE_CODE (expr) != MODIFY_EXPR
      && TREE_CODE (expr) != INIT_EXPR)
    abort ();

  lhs = simplify_expr (TREE_OPERAND (expr, 0), before_p, after_p, new_vars_p,
                       is_simple_modify_expr_lhs, 1);

  rhs = simplify_expr (TREE_OPERAND (expr, 1), before_p, after_p, new_vars_p,
                       NULL, 0);

  TREE_OPERAND (expr, 0) = lhs;
  TREE_OPERAND (expr, 1) = rhs;

  return expr;
}

/* }}} */

/** {{{ create_tmp_var()

   Create a new temporary variable declaration of type TYPE.  Returns the
   newly created decl and adds it to the list pointed by NEW_VARS_P.  */

tree
create_tmp_var (type, new_vars_p)
     tree type;
     tree *new_vars_p;
{
  static unsigned int id_num = 1;
  char *tmp_name;
  tree tmp_var;

  ASM_FORMAT_PRIVATE_NAME (tmp_name, "T", id_num++);
  tmp_var = build_decl (VAR_DECL, get_identifier (tmp_name), type);

  /* The variable was declared by the compiler.  */
  DECL_ARTIFICIAL (tmp_var) = 1;

  /* Make the variable writable.  */
  TREE_READONLY (tmp_var) = 0;

  /* Make the type of the variable writable.  */
  make_type_writable (tmp_var);

  /* Set the declaration context.  */
  DECL_CONTEXT (tmp_var) = current_function_decl;
  DECL_EXTERNAL (tmp_var) = 0;
  TREE_STATIC (tmp_var) = 0;
  TREE_USED (tmp_var) = 1;

  /* Insert the declaration in the list of new variables.  */
  add_tree (tmp_var, new_vars_p);

  return tmp_var;
}

/* }}} */

/** {{{ make_type_writable()

    Change the flags for the type of the node T to make it writtable.  */

static void 
make_type_writable (t)
     tree t;
{
  if (t == NULL_TREE)
    abort();

  if (TYPE_READONLY (TREE_TYPE (t))
      || ((TREE_CODE (TREE_TYPE (t)) == RECORD_TYPE
	   || TREE_CODE (TREE_TYPE (t)) == UNION_TYPE)
	  && C_TYPE_FIELDS_READONLY (TREE_TYPE (t))))
    {
      /* Make a copy of the type declaration.  */
      TREE_TYPE (t) = build_type_copy (TREE_TYPE (t));
      TYPE_READONLY (TREE_TYPE (t)) = 0;
      
      /* If the type is a structure that contains a field readonly.  */
      if ((TREE_CODE (TREE_TYPE (t)) == RECORD_TYPE
	   || TREE_CODE (TREE_TYPE (t)) == UNION_TYPE)
	  && C_TYPE_FIELDS_READONLY (TREE_TYPE (t)))
	{
	  C_TYPE_FIELDS_READONLY (TREE_TYPE (t)) = 0;

	  /* Make the fields of the structure writable.  */
	  {
	    tree it;
	    it = TYPE_FIELDS (TREE_TYPE (t));
	    while (it)
	      {
		/* Make the field writable.  */
		TREE_READONLY (it) = 0;
		
		/* Make the type of the field writable.  */
		make_type_writable (it);
		it = TREE_CHAIN (it);
	      }
	  }
	}
    }
}

/* }}} */

/** {{{ declare_tmp_vars()

    Declares all the variables in VARS in SCOPE.  Returns the last DECL_STMT
    emitted.  */

tree
declare_tmp_vars (vars, scope)
     tree vars;
     tree scope;
{
  tree t, last;

  /* Add all the symbols to the BLOCK_VARS for the scope.  */
  if (SCOPE_STMT_BLOCK (scope) == NULL)
    SCOPE_STMT_BLOCK (scope) = build_block (vars, NULL, NULL, NULL, NULL);
  else
    chainon (BLOCK_VARS (SCOPE_STMT_BLOCK (scope)), vars);

  /* Find the last declaration statement in the scope.  Add all the new
     declarations after it.  */
  last = tree_last_decl (scope);

  for (t = vars; t; t = TREE_CHAIN (t))
    {
      tree decl, tmp;

      decl = build_stmt (DECL_STMT, t);
      STMT_LINENO (decl) = STMT_LINENO (scope);

      tmp = TREE_CHAIN (last);
      TREE_CHAIN (last) = decl;
      TREE_CHAIN (decl) = tmp;

      last = decl;
    }

  return last;
}

/* }}} */

/** {{{ tree_last_decl()
    
    Returns the last DECL_STMT in the scope SCOPE.  */

static tree
tree_last_decl (scope)
     tree scope;
{
  tree last;

  /* Be sure that we get a scope.  */
  if (!SCOPE_BEGIN_P (scope))
    abort ();

  /* Find the last declaration statement in the scope.  */
  last = scope;
  while (TREE_CHAIN (last) && TREE_CODE (TREE_CHAIN (last)) == DECL_STMT)
    last = TREE_CHAIN (last);

  return last;
}

/* }}} */

/** {{{ tree_build_scope()
   
   Replaces T; by a COMPOUND_STMT containing {T;}.  */

void
tree_build_scope (t)
     tree *t;
{
  if (*t == NULL_TREE)
    {
      /* Construct a compound statement and a scope.  */
      tree body, start_scope, end_scope;
      body = make_node (COMPOUND_STMT);
      start_scope = make_node (SCOPE_STMT);
      end_scope = make_node (SCOPE_STMT);
      TREE_LANG_FLAG_0 (start_scope) = 1;
      TREE_LANG_FLAG_0 (end_scope) = 0;
      TREE_OPERAND (body, 0) = start_scope;
      /* Construct an empty body.  */
      TREE_CHAIN (start_scope) = end_scope;
      /* Replace the node by the constructed body.  */
      *t = body;
      return;
    }

  if (TREE_CODE (*t) == COMPOUND_STMT)
    {
      if (COMPOUND_BODY (*t) == NULL_TREE)
	{
	  /* There's a compound statement, but no scope.  */
	  tree start_scope, end_scope;
	  start_scope = make_node (SCOPE_STMT);
	  end_scope = make_node (SCOPE_STMT);
	  TREE_LANG_FLAG_0 (start_scope) = 1;
	  TREE_LANG_FLAG_0 (end_scope) = 0;
	  TREE_OPERAND (*t, 0) = start_scope;
	  TREE_CHAIN (start_scope) = end_scope;
	}
      else
	/* The given NODE is actually a scope.  */
	return;
    }
  else
    {
      /* Construct a compound statement and a scope.  */
      tree body, start_scope, end_scope;
      body = make_node (COMPOUND_STMT);
      start_scope = make_node (SCOPE_STMT);
      end_scope = make_node (SCOPE_STMT);
      TREE_LANG_FLAG_0 (start_scope) = 1;
      TREE_LANG_FLAG_0 (end_scope) = 0;
      TREE_OPERAND (body, 0) = start_scope;
      TREE_CHAIN (start_scope) = *t;
      TREE_CHAIN (*t) = end_scope;
      *t = body;
    }
}

/* }}} */

/** {{{ add_tree()

   Add T to the list pointed by LIST_P.  */

static void
add_tree (t, list_p)
     tree t;
     tree *list_p;
{
  if (t == NULL || list_p == NULL)
    abort ();

  if (*list_p)
    chainon (*list_p, t);
  else
    *list_p = t;
}

/* }}} */

/** {{{ add_assignment_tree ()

   Add the expression statement 'LHS = RHS;' to the list of statements
   given by LIST_P.  Return the newly constructed statement.  */

static void
add_assignment_tree (lhs, rhs, list_p)
     tree lhs;
     tree rhs;
     tree *list_p;
{
  tree stmt;
  
  stmt = build_stmt (EXPR_STMT, build_modify_expr (lhs, NOP_EXPR, rhs));
  add_tree (stmt, list_p);
}

/* }}} */

/** {{{ insert_before_continue_end()
    
    Insert the REEVAL list after CASE_LABELs.  Set the line number of the REEVAL
    list to LINE.  This function is used for inserting post evaluation statements
    in a SWITCH_BODY.  */

static void
insert_after_case_labels (reeval, body, line)
     tree reeval, body;
     int line;
{
  tree next, node;
  node = body;

  if (reeval == NULL_TREE || node == NULL_TREE)
    return;
  
  if (TREE_CODE (node) == COMPOUND_STMT)
    node = COMPOUND_BODY (node);
  
  next = TREE_CHAIN (node);
  if (next == NULL_TREE)
    return;

  /* Update the line number information.  */
  {
    tree it;
    for (it = reeval; TREE_CHAIN (it); it = TREE_CHAIN (it))
      update_line_number (it, line);
  }

  /* Walk through each statement in the given block up to the last one, 
     searching for CASE_LABELs.  */
  
  while (TREE_CHAIN (next) != NULL_TREE)
	
    {
      if (TREE_CODE (node) == CASE_LABEL)
	/* Insert REEVAL after this node.  */
	{
	  TREE_CHAIN (node) = copy_stmt_chain (reeval);
	  node = tree_last (node);
	  TREE_CHAIN (node) = next;
	}
      node = TREE_CHAIN (node);
      next = TREE_CHAIN (next);
    }
}

/* }}} */

/** {{{ insert_before_continue_end()
    
    Insert the REEVAL list before CONTINUE_STMTs and at the end of the loop body BODY.  
    Set the line number of the REEVAL list to LINE.  */

void
insert_before_continue_end (reeval, body, line)
     tree reeval;
     tree body;
     int line;
{
  tree last, forelast;

  /* Update the line number information.  */
  {
    tree it;
    for (it = reeval; TREE_CHAIN (it); it = TREE_CHAIN (it))
      update_line_number (it, line);
  }

  /* Make sure that the loop body has a scope.  */
  tree_build_scope (&body);

  /* Insert the reevaluation list before every CONTINUE_STMT.  */
  /*  TREE_CHAIN (reeval) = NULL_TREE; */
  forelast = insert_before_continue (body, reeval);
  last = TREE_CHAIN (forelast);
  
  /* If the last statement of the WHILE_BODY is not a CONTINUE_STMT, 
     then insert reeval at the end of the loop block.  */
  if (TREE_CODE (forelast) != CONTINUE_STMT)
    {
      TREE_CHAIN (forelast) = copy_stmt_chain (reeval);
      forelast = tree_last (forelast);
      TREE_CHAIN (forelast) = last;
    }
}

/* }}} */

/** {{{ insert_before_continue()
    
    Insert the list REEVAL of statements before each CONTINUE_STMT in the block 
    pointed to by NODE.  At the end returns a pointer to the forelast node in 
    the block NODE.  The caller can insert then the last loop reevaluation at 
    the end of the loop block.  */

static tree
insert_before_continue (node, reeval)
     tree node, reeval;
{
  tree next;

  if (reeval == NULL_TREE || node == NULL_TREE)
    return NULL_TREE;
  
  if (TREE_CODE (node) == COMPOUND_STMT)
    node = COMPOUND_BODY (node);
  
  next = TREE_CHAIN (node);
  if (next == NULL_TREE)
    return NULL_TREE;

  /* Walk through each statement in the given block up to the last one, 
     searching for CONTINUE_STMTs.  */
  for ( ;TREE_CHAIN (next) != NULL_TREE; 
	node = TREE_CHAIN (node), next = TREE_CHAIN (next))
    {
      switch (TREE_CODE (next))
	{

	case CONTINUE_STMT:
	  /* Insert the reeval of statements before continue.  */
	  TREE_CHAIN (node) = copy_stmt_chain (reeval);
	  node = tree_last (node);
	  TREE_CHAIN (node) = next;
	  break;
	  
	case IF_STMT:
	  /* Be sure that the THEN_CLAUSE has a scope.  */
	  tree_build_scope (&THEN_CLAUSE (next));

	  /* Insert the code REEVAL in the block of the THEN_CLAUSE.  */
	  insert_before_continue (COMPOUND_BODY (THEN_CLAUSE (next)), reeval);
	  
	  /* Same thing for the ELSE_CLAUSE.  */
	  if (ELSE_CLAUSE (TREE_CHAIN (node)))
	    {
	      tree_build_scope (&ELSE_CLAUSE (next));
	      insert_before_continue (COMPOUND_BODY (ELSE_CLAUSE (next)), reeval);
	    }
	  break;

	case SWITCH_STMT:
	  /* Be sure that the SWITCH_BODY has a scope.  */
	  tree_build_scope (&SWITCH_BODY (next));

	  /* Insert the code REEVAL in the SWITCH_BODY.  */
	  insert_before_continue (COMPOUND_BODY (SWITCH_BODY (next)), reeval);
	  break;

	case COMPOUND_STMT:
	  /* Insert in the inner block.  */
	  insert_before_continue (COMPOUND_BODY (next), reeval);
	  break;

	default:
	  /* Don't enter in sub loops...  The continue statement has an effect 
	     only at a depth 1.  */
	  break;
	}
    }
  return node;
}

/* }}} */

/** {{{ copy_stmt_chain()
    
    Copy every statement from the chain CHAIN by calling COPY_STMT and return the 
    new chain.  */
    
tree
copy_stmt_chain (chain)
     tree chain;
{
  tree new_chain, res;

  if (chain == NULL_TREE)
    /* Nothing to copy.  */
    return NULL_TREE;
  
  new_chain = copy_stmt (chain);
  res = new_chain;

  while (TREE_CHAIN (chain))
    {
      chain = TREE_CHAIN (chain);
      TREE_CHAIN (new_chain) = copy_stmt (chain);
      new_chain = TREE_CHAIN (new_chain);
    }

  return res;
}

/* }}} */

/** {{{ copy_stmt()

    Copy a statement by using copy_node when needed, but ensuring that we copy 
    enough information in order to have distinct statements.  */

tree 
copy_stmt (node)
     tree node;
{
  tree res;

  if (node == NULL_TREE)
    return NULL_TREE;

  switch (TREE_CODE (node))
    {
    case COMPOUND_STMT:
      res = build_stmt (COMPOUND_STMT, 
			copy_stmt_chain (COMPOUND_BODY (node)));
      break;

    case FOR_STMT:
      res = build_stmt (FOR_STMT, 
			copy_stmt (FOR_INIT_STMT (node)),
			(FOR_COND (node) ? 
			 copy_node (FOR_COND (node)) : NULL_TREE),
			(FOR_EXPR (node) ? 
			 copy_node (FOR_EXPR (node)) : NULL_TREE),
			copy_stmt (FOR_BODY (node)));
      break;

    case WHILE_STMT:
      res = build_stmt (WHILE_STMT, 
			(WHILE_COND (node) ? 
			 copy_node (WHILE_COND (node)) : NULL_TREE),
			copy_stmt (WHILE_BODY (node)));
      break;

    case DO_STMT:
      res = build_stmt (DO_STMT, 
			(DO_COND (node) ? 
			 copy_node (DO_COND (node)) : NULL_TREE),
			copy_stmt (DO_BODY (node)));
      break;

    case IF_STMT:
      res = build_stmt (IF_STMT, 
			(IF_COND (node) ? 
			 copy_node (IF_COND (node)) : NULL_TREE),
			copy_stmt (THEN_CLAUSE (node)),
			copy_stmt (ELSE_CLAUSE (node)));
      break;

    case SWITCH_STMT:
      res = build_stmt (SWITCH_STMT,
			(SWITCH_COND (node) ? 
			 copy_node (SWITCH_COND (node)) : NULL_TREE),
			copy_stmt (SWITCH_BODY (node)));
      break;

    case EXPR_STMT:
      res = build_stmt (EXPR_STMT,
			(EXPR_STMT_EXPR (node) ? 
			 copy_node (EXPR_STMT_EXPR (node)) : NULL_TREE));
      break;

    case DECL_STMT:
      res = build_stmt (DECL_STMT,
			(DECL_STMT_DECL (node) ? 
			 copy_node (DECL_STMT_DECL (node)) : NULL_TREE));
      break;

    case RETURN_STMT:
      res = build_stmt (RETURN_STMT,
			(RETURN_EXPR (node) ? 
			 copy_node (RETURN_EXPR (node)) : NULL_TREE));
      break;

    case SCOPE_STMT:
      if (SCOPE_BEGIN_P (node))
	{
	  res = build_stmt (SCOPE_STMT,
			    (SCOPE_STMT_BLOCK (node) ? 
			     copy_list (SCOPE_STMT_BLOCK (node)) : 
			     NULL_TREE));
	  TREE_LANG_FLAG_0 (res) = 1;
	}
      else 
	{
	  res = build_stmt (SCOPE_STMT, NULL_TREE);
	  TREE_LANG_FLAG_0 (res) = 0;
	}
      break;

    case FILE_STMT:
    case LABEL_STMT:
    case GOTO_STMT:
    case ASM_STMT:
    case CASE_LABEL:
    case CONTINUE_STMT:
    case BREAK_STMT:
    default:
      res = copy_node (node);
      break;
    }
  
  /* Set the line number.  */
  STMT_LINENO (res) = STMT_LINENO (node);
  
  return res;
}

/* }}} */

/** {{{ update_line_number()

    Updates the STMT_LINENO of each stmt in the tree t to the line number LINE. 
    Returns the last stmt in the tree chain.  */

tree
update_line_number (t, line)
     tree t;
     int line;
{
  if (t == NULL_TREE)
    return NULL_TREE;

  for (; TREE_CHAIN (t); t = TREE_CHAIN (t))
    STMT_LINENO (t) = line;
  STMT_LINENO (t) = line;
  return t;
}

/* }}} */

/** {{{ is_binop ()

    Return non-zero if EXPR is a binary operation.  */

static int
is_binop (t)
     tree t;
{
  enum tree_code code = TREE_CODE (t);

  return (TREE_CODE_CLASS (code) == '2'
	  || TREE_CODE_CLASS (code) == '<'
	  || code == TRUTH_ANDIF_EXPR
	  || code == TRUTH_ORIF_EXPR
	  || code == TRUTH_AND_EXPR
	  || code == TRUTH_OR_EXPR
	  || code == TRUTH_XOR_EXPR);
}

/* }}} */

/** {{{ is_unop ()

    Return non-zero if EXPR is a unary operation.  */

static int
is_unop (t)
     tree t;
{
  enum tree_code code = TREE_CODE (t);

  return (TREE_CODE_CLASS (code) == '1'
	  || code == ADDR_EXPR
	  || code == INDIRECT_REF
	  || code == TRUTH_NOT_EXPR);
}

/* }}} */

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

static void simplify_stmt            PARAMS ((tree));
static tree simplify_for_stmt        PARAMS ((tree, tree *, tree *));
static tree simplify_while_stmt      PARAMS ((tree, tree *));
static tree simplify_do_stmt         PARAMS ((tree));
static tree simplify_switch_stmt     PARAMS ((tree, tree *));
static tree simplify_decl_stmt       PARAMS ((tree, tree *));
static tree simplify_if_stmt         PARAMS ((tree, tree *));
static tree simplify_expr            PARAMS ((tree, tree *, tree *,
                                              int (*) PARAMS ((tree)), int));
static tree simplify_array_ref       PARAMS ((tree, tree *, tree *));
static tree simplify_self_mod_expr   PARAMS ((tree, tree *, tree *));
static tree simplify_component_ref   PARAMS ((tree, tree *, tree *));
static tree simplify_call_expr       PARAMS ((tree, tree *, tree *));
static tree simplify_tree_list       PARAMS ((tree, tree *, tree *));
static tree simplify_cond_expr       PARAMS ((tree, tree *, tree *));
static tree simplify_modify_expr     PARAMS ((tree, tree *, tree *));
static tree simplify_boolean_expr    PARAMS ((tree, tree *, tree *));
static tree simplify_compound_expr   PARAMS ((tree, tree *, tree *));
static tree simplify_save_expr       PARAMS ((tree, tree *, tree *,
                                              int (*) PARAMS ((tree)), int));
static tree simplify_expr_wfl        PARAMS ((tree, tree *, tree *,
                                              int (*) PARAMS ((tree)), int));
static void make_type_writable       PARAMS ((tree));
static tree add_tree                 PARAMS ((tree, tree *));
static tree insert_before_continue   PARAMS ((tree, tree));
static void insert_before_first      PARAMS ((tree, tree));
static tree tree_last_decl           PARAMS ((tree));
static tree convert_to_stmt_chain    PARAMS ((tree, tree));
static int  stmt_has_effect          PARAMS ((tree));
static int  expr_has_effect          PARAMS ((tree));

/* }}} */


/* Simplification of statement trees.  */

/** {{{ simplify_tree ()

    Entry point to the simplification pass.  FN is the FUNCTION_DECL node
    for the function we want to simplify.  */

void
simplify_tree (fn)
     tree fn;
{
  tree fn_body;

  /* Don't bother doing anything if the program has errors.  */
  if (errorcount || sorrycount)
    return;

  fn_body = COMPOUND_BODY (DECL_SAVED_TREE (fn));
  if (fn_body == NULL)
    return;

  /* Create a new binding level for the temporaries created by the
     simplification process.  */
  pushlevel (0);

  /* Simplify the function's body.  */
  simplify_stmt (fn_body);

  /* Declare the new temporary variables.  */
  declare_tmp_vars (getdecls(), fn_body);

  /* Restore the binding level.  */
  poplevel (1, 1, 0);
}

/* }}} */

/** {{{ simplify_stmt ()
  
    Entry point for the tree lowering pass.  Recursively scan
    STMT and convert it to a SIMPLE tree.  */

static void 
simplify_stmt (stmt)
     tree stmt;
{
  tree next, prev, pre, post;

  /* PRE and POST are tree chains that contain the side-effects of the
     simplified tree.  For instance, given the expression tree:

     		c = ++a * 3 + b++;

     After simplification, the tree will be re-written as:

     		a = a + 1;
		t1 = a * 3;	<-- PRE
     		c = t1 + b;
     		b = b + 1;	<-- POST  */

  prev = stmt;
  while (stmt && stmt != error_mark_node)
    {
      pre = NULL;
      post = NULL;
      next = TREE_CHAIN (stmt);
      
      switch (TREE_CODE (stmt))
	{
	case COMPOUND_STMT:
	  simplify_stmt (COMPOUND_BODY (stmt));
	  prev = stmt;
	  stmt = next;
	  continue;
	  
	case SCOPE_STMT:
	  prev = stmt;
	  stmt = next;
	  continue;

	case FOR_STMT:
	  stmt = simplify_for_stmt (stmt, &pre, &post);
	  break;
	  
	case WHILE_STMT:
	  stmt = simplify_while_stmt (stmt, &pre);
	  break;

	case DO_STMT:
	  stmt = simplify_do_stmt (stmt);
	  break;

	case IF_STMT:
	  stmt = simplify_if_stmt (stmt, &pre);
	  break;
	  
	case SWITCH_STMT:
	  stmt = simplify_switch_stmt (stmt, &pre);
	  break;

	case EXPR_STMT:
	  EXPR_STMT_EXPR (stmt) = simplify_expr (EXPR_STMT_EXPR (stmt), &pre,
	                                         &post, is_simple_expr, 0);
	  break;

	case DECL_STMT:
	  /* Simplify all the declarations at once.  */
	  stmt = simplify_decl_stmt (stmt, &post);
	  
	  if (post)
	    {
	      tree last_decl;

	      /* Save a pointer to the next stmt.  */
	      next = TREE_CHAIN (stmt);
	      
	      /* Include the nodes from POST list.  */
	      post = convert_to_stmt_chain (post, stmt);
	      TREE_CHAIN (stmt) = post;
	      last_decl = tree_last (post);
	      
	      /* Chain the end of the POST block to the next stmt.  */
	      TREE_CHAIN (last_decl) = next;
	    }

	  /* Go to the next statement to be simplified.  */
	  prev = stmt;
	  stmt = TREE_CHAIN (stmt);
	  continue;

	case RETURN_STMT:
	  {
	    tree type = TREE_TYPE (TREE_TYPE (current_function_decl));
	    if (TREE_CODE (type) != VOID_TYPE
		&& RETURN_EXPR (stmt))
	      {
		tree expr;
		tree e = RETURN_EXPR (stmt);
		expr = simplify_expr (TREE_OPERAND (e, 1), &pre, &post,
		                      is_simple_val, 0);
		TREE_OPERAND (e, 1) = expr;
	      }
	    break;
	  }

	case FILE_STMT:
	case LABEL_STMT:
	case GOTO_STMT:
	case ASM_STMT:
	case CASE_LABEL:
	case CONTINUE_STMT:
	case BREAK_STMT:
	  /* Nothing to do: next iteration.  */
	  prev = stmt;
	  stmt = next;
	  continue;

	default:
	  prep_stmt (stmt);
	  fprintf (stderr, "unhandled statement node in simplify_stmt ():\n");
	  print_node_brief (stderr, "", stmt, 0);
	  debug_c_node (stmt);
	  fprintf (stderr, "\n");
	  abort ();
	  break;
	}

      /* PRE and POST contain a list of expressions for all the
	 side-effects in STMT.  Each expression must be converted into a
	 statement and chained so that:

	 	PREV -> STMT -> NEXT

         is re-written as:

	 	PREV -> PRE -> STMT -> POST -> NEXT

	 However, if STMT has been nullified, it is bypassed.  */

      pre = convert_to_stmt_chain (pre, stmt);
      post = convert_to_stmt_chain (post, stmt);
      
      TREE_CHAIN (prev) = NULL_TREE;
      TREE_CHAIN (stmt) = NULL_TREE;

      chainon (prev, pre);
      if (stmt_has_effect (stmt))
	{
	  chainon (prev, stmt);
	  chainon (stmt, post);
	}
      else
	chainon (prev, post);

      /* Next iteration.  Re-set PREV to the last statement of the chain
	 PREV -> PRE -> STMT -> POST.  */
      prev = tree_last (prev);
      stmt = next;
      TREE_CHAIN (prev) = next;
    }
}

/* }}} */

/** {{{  simplify_for_stmt ()

    Simplify a FOR_STMT and return the simplified version.  This will
    convert:

    	for (init; cond; expr)
	  {
	    body;
	  }

    into

    	pre_init_s;
	init_s;
	post_init_s;
	pre_cond_s;
	for ( ; cond_s; )
	  {
	    post_cond_s;
	    body;
	    pre_expr_s;
	    expr_s;
	    post_expr_s;
	    pre_cond_s;
	  }
	post_cond_s;

    where INIT_S, COND_S and EXPR_S are the simplified versions of INIT,
    COND and EXPR respectively.  PRE_*_S and POST_*_S are the side-effects
    for each expression in the header.

    Note that the above form is the more general case, other variations are
    possible if any of the PRE_*_S or POST_*S expressions are missing.

    PRE_P points to the list where side effects that must happen before
	STMT should be store.  These are all the expressions in
	FOR_INIT_STMT and the pre side-effects of the loop conditional.

    POST_P points to the list where side effects that must happen after
	STMT should be stored.  These are the post side effects for the
	loop conditional.  */

static tree
simplify_for_stmt (stmt, pre_p, post_p)
     tree stmt;
     tree *pre_p;
     tree *post_p;
{
  int init_is_simple, cond_is_simple, expr_is_simple;
  tree pre_init_s, init_s, post_init_s;
  tree pre_cond_s, cond_s, post_cond_s;
  tree pre_expr_s, expr_s, post_expr_s;

  /* Make sure that the loop body has a scope.  */
  tree_build_scope (&FOR_BODY (stmt));

  /* Simplify the loop's body.  */
  simplify_stmt (FOR_BODY (stmt));
   
  init_s = EXPR_STMT_EXPR (FOR_INIT_STMT (stmt));
  cond_s = FOR_COND (stmt);
  expr_s = FOR_EXPR (stmt);

  /* Check if we need to do anything.  */
  init_is_simple = (init_s == NULL_TREE || is_simple_exprseq (init_s));
  cond_is_simple = (cond_s == NULL_TREE || is_simple_condexpr (cond_s));
  expr_is_simple = (expr_s == NULL_TREE || is_simple_exprseq (expr_s));

  if (init_is_simple && cond_is_simple && expr_is_simple)
    return stmt;

  pre_init_s = NULL_TREE;
  post_init_s = NULL_TREE;
  pre_cond_s = NULL_TREE;
  post_cond_s = NULL_TREE;
  pre_expr_s = NULL_TREE;
  post_expr_s = NULL_TREE;

  /* Simplify FOR_COND.  */
  if (!cond_is_simple)
    cond_s = simplify_expr (cond_s, &pre_cond_s, &post_cond_s,
	                    is_simple_condexpr, 0);

  /* If the conditional contains PRE side effects, we need to simplify
     FOR_INIT_STMT outside the loop header, even if it's in SIMPLE form
     already.
     
     This is because we need to insert PRE_COND_S right after the
     initialization statements, and if PRE_COND_S contains statement trees,
     we cannot add them to a COMPOUND_EXPR:

	BEFORE				AFTER

					pre_init_s;
					init_s;
					post_cond_s;
					pre_cond_s;
	for (init; cond; ...)		for ( ; cond_s; ...)

     FIXME: Since FOR_INIT_STMT can be a COMPOUND_EXPR, it should be possible
	    to emit PRE_INIT_S, INIT_S, POST_COND_S and PRE_COND_S into a
	    COMPOUND_EXPR inside FOR_INIT_STMT.  However, this is not
	    possible if any of these elements contains statement trees.  */
  if (pre_cond_s && init_s != NULL_TREE)
    init_is_simple = 0;
  

  /* Simplify FOR_INIT_STMT.  Note that if FOR_INIT_STMT needs to be
     simplified, it's converted into a simple_expr because we need to move
     it out of the loop header (see previous FIXME note for future
     enhancement).  */
  if (!init_is_simple)
    init_s = simplify_expr (init_s, &pre_init_s, &post_init_s, is_simple_expr,
	                    0);


  /* Simplify FOR_EXPR.  Note that if FOR_EXPR needs to be simplified,
     it's converted into a simple_expr because we need to move it out of
     the loop header (see previous FIXME note for future enhancement).  */
  if (!expr_is_simple)
    expr_s = simplify_expr (expr_s, &pre_expr_s, &post_expr_s, is_simple_expr,
	                    0);
  

  /* Now that all the components are simplified, we have to build a new
     loop with all the side-effects in the right spots:

    	pre_init_s;
	init_s;
	post_init_s;
	pre_cond_s;
	for ( ; cond_s; )
	  {
	    post_cond_s;
	    body;
	    pre_expr_s;
	    expr_s;
	    post_expr_s;
	    pre_cond_s;
	  }
	post_cond_s;

     The above is the more general case, which produces a for() loop that
     doesn't resemble the original.  To minimize shape changes, we try to
     insert expressions in FOR_INIT_STMT and FOR_EXPR.  */

  /* Link PRE_INIT_S, INIT_S, POST_INIT_S and PRE_COND_S to make up
     a new FOR_INIT_STMT.  If the last tree in the list is an
     expression tree, it is emmitted inside FOR_INIT_STMT.  */
  add_tree (pre_init_s, pre_p);
  add_tree (init_s, pre_p);
  add_tree (post_init_s, pre_p);
  add_tree (pre_cond_s, pre_p);

  if (*pre_p)
    {
      tree prev, last, op;

      prev = NULL_TREE;
      for (op = *pre_p; TREE_CHAIN (op); op = TREE_CHAIN (op))
	prev = op;
      
      last = TREE_VALUE ((prev) ? TREE_CHAIN (prev) : *pre_p);

      if (!statement_code_p (TREE_CODE (last)))
	{
	  /* The last statement is an expression, emit it inside
	    FOR_INIT_STMT and remove the expression from PRE_P.  */
	  EXPR_STMT_EXPR (FOR_INIT_STMT (stmt)) = last;
	  if (prev)
	    TREE_CHAIN (prev) = NULL_TREE;
	  else
	    *pre_p = NULL_TREE;
	}
      else
	{
	  /* The last statement is not an expression, nullify FOR_INIT_STMT.
	    All the expressions in FOR_INIT_STMT and PRE_COND_S have been
	    emitted inside PRE_P already.  */
	  EXPR_STMT_EXPR (FOR_INIT_STMT (stmt)) = NULL_TREE;
	}
    }

  /* Build the new FOR_COND.  */
  {
    FOR_COND (stmt) = cond_s;

    /* Insert one copy of POST_COND_S in the loop body and another copy in
       POST_P.  */
    insert_before_first (convert_to_stmt_chain (deep_copy_list (post_cond_s),
					      stmt),
			 FOR_BODY (stmt));
    add_tree (post_cond_s, post_p);
  }


  /* Link PRE_EXPR_S, EXPR_S, POST_EXPR_S and a copy of PRE_COND_S to emit
     before every wrap-around point inside the loop body.  If the last tree
     in the list is an expression tree, it is emmitted inside FOR_EXPR.  */
  {
    tree expr_chain;

    expr_chain = pre_expr_s;
    add_tree (expr_s, &expr_chain);
    add_tree (post_expr_s, &expr_chain);
    add_tree (deep_copy_list (pre_cond_s), &expr_chain);

    if (expr_chain)
      {
	tree prev, op, last, stmt_chain;

	prev = NULL_TREE;
	for (op = expr_chain; TREE_CHAIN (op); op = TREE_CHAIN (op))
	  prev = op;
	last = TREE_VALUE ((prev) ? TREE_CHAIN (prev) : expr_chain);

	if (!statement_code_p (TREE_CODE (last)))
	  {
	    /* The last statement is an expression, emit it inside
	      FOR_EXPR and remove the expression from EXPR_CHAIN.  */
	    FOR_EXPR (stmt) = last;
	    if (prev)
	      TREE_CHAIN (prev) = NULL_TREE;
	    else
	      expr_chain = NULL_TREE;
	  }
	else
	  {
	    /* The last statement is not an expression, nullify FOR_EXPR.  */
	    FOR_EXPR (stmt) = NULL_TREE;
	  }

	stmt_chain = convert_to_stmt_chain (deep_copy_list (expr_chain), stmt);
	insert_before_continue_end (stmt_chain, FOR_BODY (stmt),
				    STMT_LINENO (stmt));
      }
  }


  return stmt;
}

/* }}} */

/** {{{  simplify_while_stmt ()

    Simplify a WHILE_STMT and return the simplified version.
    This will convert:

    	while (cond)
	  {
	    body;
	  }

    into

	pre_cond_s;
	T = cond_s;
	post_cond_s;
    	while (T)
	  {
	    body;
	    pre_cond_s;
	    T = cond_s;
	    post_cond_s;
	  }

    where COND_S is the simplified version of the loop predicate.
    PRE_COND_S and POST_COND_S are the pre and post side-effects produced
    by the simplification of the conditional.
    
    Both PRE and POST side-effects will be replicated at every wrap-around
    point inside the loop's body.

    STMT is the statement to simplify.

    PRE_P points to the list where side effects that must happen before
	STMT should be stored.  */

static tree
simplify_while_stmt (stmt, pre_p)
     tree stmt;
     tree *pre_p;
{
  tree post, cond_s, stmt_chain;

  post = NULL_TREE;

  /* Make sure that the loop body has a scope.  */
  tree_build_scope (&WHILE_BODY (stmt));
  
  /* Simplify the loop's body.  */
  simplify_stmt (WHILE_BODY (stmt)); 

  /* Check wether the loop condition is already simplified.  */
  if (is_simple_condexpr (WHILE_COND (stmt)))
    return stmt;
    
  /* Simplify the loop conditional.  Note that we simplify the conditional
     into a SIMPLE identifier so that the pre and post side-effects of the
     conditional can be computed before the loop header.  */
  cond_s = simplify_expr (WHILE_COND (stmt), pre_p, &post, is_simple_id, 0);
  WHILE_COND (stmt) = build (NE_EXPR, TREE_TYPE (WHILE_COND (stmt)),
	                     cond_s, integer_zero_node);
  add_tree (post, pre_p);

  /* Insert all the side-effects for the conditional before every
     wrap-around point in the loop body (i.e., before every first-level
     CONTINUE and before the end of the body).  */
  stmt_chain = convert_to_stmt_chain (deep_copy_list (*pre_p), stmt);
  insert_before_continue_end (stmt_chain, WHILE_BODY (stmt),
                              STMT_LINENO (stmt));


  return stmt;
}

/* }}} */

/** {{{ simplify_do_stmt ()

    Simplify a DO_STMT node and return the simplified statement.
    This will convert:

	do
	  {
	    body;
	  }
    	while (cond);

    into

    	do
	  {
	    body;
	    pre_cond_s;
	    T = cond_s;
	    post_cond_s;
	  }
    	while (T);

    where COND_S is the simplified version of the loop predicate.
    PRE_COND_S and POST_COND_S are the pre and post side-effects produced
    by the simplification of the conditional.
    
    Both PRE and POST side-effects will be replicated at every wrap-around
    point inside the loop's body.

    STMT is the statement to simplify.  */

static tree
simplify_do_stmt (stmt)
     tree stmt;
{
  tree cond_s, pre_cond_s, post_cond_s, stmt_chain;

  /* Make sure that the loop body has a scope.  */
  tree_build_scope (&DO_BODY (stmt));

  /* Simplify the loop's body.  */
  simplify_stmt (DO_BODY (stmt));

  /* Check wether the loop condition is already simplified.  */
  if (is_simple_condexpr (DO_COND (stmt)))
    return stmt;

  /* Simplify the loop conditional.  Note that we simplify the conditional
     into a SIMPLE identifier so that the pre and post side-effects of the
     conditional can be computed before the loop header.  */
  pre_cond_s = NULL;
  post_cond_s = NULL;
  cond_s = simplify_expr (DO_COND (stmt), &pre_cond_s, &post_cond_s, 
                          is_simple_id, 0);

  DO_COND (stmt) = build (NE_EXPR, TREE_TYPE (DO_COND (stmt)), cond_s, 
                          integer_zero_node);

  /* Insert all the side-effects for the conditional before every
     wrap-around point in the loop body (i.e., before every first-level
     CONTINUE and before the end of the body).  */
  add_tree (post_cond_s, &pre_cond_s);
  stmt_chain = convert_to_stmt_chain (deep_copy_list (pre_cond_s), stmt);
  insert_before_continue_end (stmt_chain, DO_BODY (stmt), STMT_LINENO (stmt));

  return stmt;
}


/* }}} */

/** {{{ simplify_if_stmt ()
    
    Simplify an IF_STMT and return the simplified version.
    This will convert:

    	if (cond)
	  {
	    then_clause;
	  }
	else
	  {
	    else_clause;
	  }

    into

	pre_cond_s;
	T = cond_s;
	post_cond_s;
    	if (T)
	  {
	    then_clause;
	  }
	else
	  {
	    else_clause;
	  }

    where COND_S is the simplified version of the predicate. PRE_COND_S and
    POST_COND_S are the pre and post side-effects produced by the
    simplification of the conditional.
    
    STMT is the statement to simplify.

    PRE_P points to the list where side effects that must happen before
	STMT should be stored.  */

static tree 
simplify_if_stmt (stmt, pre_p)
     tree stmt;
     tree *pre_p;
{
  tree cond_s, post_cond_s = NULL_TREE;

  /* Simplify each clause.  */
  if (THEN_CLAUSE (stmt))
    {
      tree_build_scope (&THEN_CLAUSE (stmt));
      simplify_stmt (THEN_CLAUSE (stmt));
    }

  if (ELSE_CLAUSE (stmt))
    {
      tree_build_scope (&ELSE_CLAUSE (stmt));
      simplify_stmt (ELSE_CLAUSE (stmt));
    }
      		
  /* Nothing to do if the condition is simple already.  */
  if (is_simple_condexpr (IF_COND (stmt)))
    return stmt;

  /* Simplify the conditional.  Force simplification to produce a SIMPLE
     identifier so that all the pre and post side-effects for the
     conditional can be evaluated before the if().  */
  cond_s = simplify_expr (IF_COND (stmt), pre_p, &post_cond_s, is_simple_id, 0);
  IF_COND (stmt) = build (NE_EXPR, TREE_TYPE (cond_s), cond_s,
                          integer_zero_node);


  /* Chain pre and post side-effects together.  Since the simplification of
     the conditional has produced an identifier, it is safe to emit the
     side-effects before the if() statement.  */
  add_tree (post_cond_s, pre_p);

  return stmt;
}

/* }}} */

/** {{{ simplify_switch_stmt ()

    Simplify a SWITCH_STMT and return the simplified version.
    This will convert:

    	switch (cond)
	  {
	    body;
	  }

    into

	pre_cond_s;
	T = cond_s;
	post_cond_s;
    	switch (T)
	  {
	    body;
	  }

    where COND_S is the simplified version of the predicate. PRE_COND_S and
    POST_COND_S are the pre and post side-effects produced by the
    simplification of the conditional.
    
    STMT is the statement to simplify.

    PRE_P points to the list where side effects that must happen before
	STMT should be stored.  */

static tree
simplify_switch_stmt (stmt, pre_p)
     tree stmt;
     tree *pre_p;
{
  tree cond_s, post_cond_s;

  simplify_stmt (SWITCH_BODY (stmt));

  if (is_simple_val (SWITCH_COND (stmt)))
    return stmt;

  /* Simplify the conditional.  Force simplification to produce a SIMPLE
     identifier so that all the pre and post side-effects for the
     conditional can be evaluated before the switch().  */
  post_cond_s = NULL_TREE;
  cond_s = simplify_expr (SWITCH_COND (stmt), pre_p, &post_cond_s, is_simple_id,
                          0);
  SWITCH_COND (stmt) = cond_s;


  /* Chain pre and post side-effects together.  Since the simplification of
     the conditional has produced an identifier, it is safe to emit the
     side-effects before the switch() statement.  */
  add_tree (post_cond_s, pre_p);

  return stmt;
}

/* }}} */

/** {{{ simplify_decl_stmt ()

    Simplifies a DECL_STMT node T.  This function scans the list of all
    declarations starting with T looking for initializers.
    
    If a declaration V has an initial value I, the expression 'V = I' is
    added to the POST_P list.  On return, all the statements in POST_P
    will be inserted immediately after all the DECL_STMTs in the same order
    that they appeared originally.

    The function returns the last DECL_STMT found.  */

static tree
simplify_decl_stmt (t, post_p)
     tree t;
     tree *post_p;
{
  tree prev;

  if (TREE_CODE (t) != DECL_STMT)
    abort ();

  prev = t;
  while (t && TREE_CODE (t) == DECL_STMT)
    {
      tree decl = DECL_STMT_DECL (t);

      if (DECL_INITIAL (decl)
	  && !TREE_READONLY (decl))
	{
	  /* If there is an initializer for the variable, and the
	     declaration is not a constant, queue it up to be processed
	     after all the declarations.  */
	  add_tree (build_modify_expr (decl, NOP_EXPR, DECL_INITIAL (decl)),
	            post_p);
	  DECL_INITIAL (decl) = NULL_TREE;
	}

      prev = t;
      t = TREE_CHAIN (t);
    }

  return prev;
}

/* }}} */


/* Simplification of expression trees.  */

/** {{{ simplify_expr ()

    Simplifies the expression tree rooted at EXPR.  Returns the simplified
    version of EXPR.

    PRE_P points to the list where side effects that must happen before
	EXPR should be stored.

    POST_P points to the list where side effects that must happen after
	EXPR should be stored.

    SIMPLE_TEST_F points to a function that takes a tree T and
	returns nonzero if T is in the SIMPLE form requested by the
	caller.  The SIMPLE predicates are in tree-simple.c.

	This test is used twice.  Before simplification, the test is
	invoked to determine whether EXPR is already simple enough.  If
	that fails, EXPR is simplified according to its code and
	SIMPLE_TEST_F is called again.  If the test still fails, then a new
	temporary variable is created and assigned the value of the
	simplified expression (this is always safe because the
	simplification process guarantees that the simplified expression
	can be used as the RHS of a SIMPLE assignment).

    NEEDS_LVALUE should be nonzero if the caller needs to simplify the
	expression into an lvalue. This is relevant when we need to create
	a temporary variable.  If NEEDS_LVALUE is true and a temporary
	variable needs to be created, then a new temporary pointer will be
	created. Otherwise, assigning a value to the temporary variable
	will not modify the original expression.  */

static tree
simplify_expr (expr, pre_p, post_p, simple_test_f, needs_lvalue)
     tree expr;
     tree *pre_p;
     tree *post_p;
     int (*simple_test_f) PARAMS ((tree));
     int needs_lvalue;
{
  enum tree_code code;
  tree expr_s, tmp, type;

  if (expr == NULL_TREE)
    abort ();

  if (simple_test_f == NULL)
    abort ();

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
      expr_s = simplify_self_mod_expr (expr, pre_p, post_p);
      break;

    case ARRAY_REF:
      expr_s = simplify_array_ref (expr, pre_p, post_p);
      break;

    case COMPONENT_REF:
      expr_s = simplify_component_ref (expr, pre_p, post_p);
      break;
      
    case COND_EXPR:
      expr_s = simplify_cond_expr (expr, pre_p, post_p);
      break;

    case CALL_EXPR:
      expr_s = simplify_call_expr (expr, pre_p, post_p);
      break;

    case TREE_LIST:
      expr_s = simplify_tree_list (expr, pre_p, post_p);
      break;

    case COMPOUND_EXPR:
      expr_s = simplify_compound_expr (expr, pre_p, post_p);
      break;

    case MODIFY_EXPR:
    case INIT_EXPR:
      expr_s = simplify_modify_expr (expr, pre_p, post_p);

      /* Special case.  If the caller wanted to simplify an assignment into
	 an RHS (e.g., to use it as a conditional expression), add the
	 assignment into PRE_P and return the LHS of the assignment.  */
      if (simple_test_f != is_simple_expr)
	{
	  add_tree (expr_s, pre_p);
	  expr_s = TREE_OPERAND (expr_s, 1);
	}
      break;

    case TRUTH_ANDIF_EXPR:
    case TRUTH_ORIF_EXPR:
      expr_s = simplify_boolean_expr (expr, pre_p, post_p);
      break;

    /* Address expressions must not be simplified.  If they are, we may
	end up taking the address of a temporary, instead of the address
	of the original object.  */
    case ADDR_EXPR:
      expr_s = expr;
      break;

    case NOP_EXPR:
    case CONVERT_EXPR:
    case FIX_TRUNC_EXPR:
    case FIX_CEIL_EXPR:
    case FIX_FLOOR_EXPR:
    case FIX_ROUND_EXPR:
      expr_s = expr;
      TREE_OPERAND (expr_s, 0) = simplify_expr (TREE_OPERAND (expr_s, 0),
	                                        pre_p, post_p,
						is_simple_varname,
						needs_lvalue);
      break;

    case INDIRECT_REF:
      expr_s = expr;
      TREE_OPERAND (expr_s, 0) = simplify_expr (TREE_OPERAND (expr_s, 0),
	                                        pre_p, post_p, is_simple_id,
						needs_lvalue);
      break;

    case NEGATE_EXPR:
      expr_s = expr;
      TREE_OPERAND (expr_s, 0) = simplify_expr (TREE_OPERAND (expr_s, 0),
	                                        pre_p, post_p, is_simple_val,
						needs_lvalue);
      break;

    /* Constants may not be allowed in certain contexts, we just pass them
       on so that they get assigned to a new temporary.  */
    case INTEGER_CST:
    case REAL_CST:
    case STRING_CST:
    case COMPLEX_CST:
      expr_s = expr;
      break;

    /* The following are special cases that are not handled by the original
       SIMPLE grammar.  */
    case STMT_EXPR:
      simplify_stmt (STMT_EXPR_STMT (expr));
      expr_s = expr;
      break;

    case SAVE_EXPR:
      expr_s = simplify_save_expr (expr, pre_p, post_p, simple_test_f,
	                           needs_lvalue);
      break;

    case EXPR_WITH_FILE_LOCATION:
      expr_s = simplify_expr_wfl (expr, pre_p, post_p, simple_test_f,
	                          needs_lvalue);
      break;


    /* If EXPR does not need to be special-cased, handle it according to
       its class.  */
    default:
      {
	expr_s = expr;

	if (TREE_CODE_CLASS (TREE_CODE (expr_s)) == '1')
	  {
	    tree t;

	    t = simplify_expr (TREE_OPERAND (expr_s, 0), pre_p, post_p,
		               is_simple_val, 0);
	    TREE_OPERAND (expr_s, 0) = t;
	  }
	else if (TREE_CODE_CLASS (TREE_CODE (expr_s)) == '2'
	         || TREE_CODE_CLASS (TREE_CODE (expr_s)) == '<')
	  {
	    tree lhs, rhs;

	    lhs = simplify_expr (TREE_OPERAND (expr_s, 0), pre_p, post_p,
			         is_simple_val, 0);

	    rhs = simplify_expr (TREE_OPERAND (expr_s, 1), pre_p, post_p,
				 is_simple_val, 0);

	    TREE_OPERAND (expr_s, 0) = lhs;
	    TREE_OPERAND (expr_s, 1) = rhs;
	  }
	else
	  {
	    fprintf (stderr, "unhandled expression in simplify_expr ():\n");
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
     expression.  At this point, the expression should be simple enough to
     qualify as a SIMPLE RHS.  Otherwise, simplification has failed.  */
  if (!is_simple_rhs (expr_s))
    {
      fprintf (stderr, "Could not convert expression into a SIMPLE RHS:\n");
      print_node_brief (stderr, "", expr_s, 0);
      fputs ("\n", stderr);
      debug_c_node (expr_s);
      fputs ("\n", stderr);
      abort ();
    }

  if (!needs_lvalue)
    {
      /* The original expression is not an lvalue, create a new temporary
	 variable T, emit 'T = EXPR_S' and return T.  */
      type = TREE_TYPE (expr_s);
      tmp = create_tmp_var (type);
      add_tree (build_modify_expr (tmp, NOP_EXPR, expr_s), pre_p);
      expr_s = tmp;
    }
  else
    {
      /* The original expression was an lvalue.  Create a temporary pointer
	 T, get the address of the simplified expression (T = &EXPR_S) and
	 return a reference to T (*T).  */
      type = TREE_TYPE (expr_s);
      if (POINTER_TYPE_P (type))
	{
	  /* If the expression is a pointer already, there is no need to
	     take the address of the expression.  */
	  tmp = create_tmp_var (type);
	  add_tree (build_modify_expr (tmp, NOP_EXPR, expr_s), pre_p);
	  expr_s = tmp;
	}
      else if (TREE_CODE (expr_s) == INDIRECT_REF)
	{
	  /* If the expression is an indirect reference, the address we
	     need is the actual reference.  */
	  type = TREE_TYPE (TREE_OPERAND (expr_s, 0));
	  tmp = create_tmp_var (type);
	  add_tree (build_modify_expr (tmp, NOP_EXPR, TREE_OPERAND (expr_s, 0)),
	            pre_p);
	  TREE_OPERAND (expr_s, 0) = tmp;
	}
      else
	{
	  /* Take the address of the simplified expression and return a
	     reference to it.  */
	  type = build_pointer_type (type);
	  tmp = create_tmp_var (type);
	  add_tree (build_modify_expr (tmp, NOP_EXPR,
		                       build1 (ADDR_EXPR, type, expr_s)),
                    pre_p);
	  expr_s = build1 (INDIRECT_REF, TREE_TYPE (type), tmp);
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

    PRE_P points to the list where side effects that must happen before
	EXPR should be stored.

    POST_P points to the list where side effects that must happen after
	EXPR should be stored.  */

static tree
simplify_array_ref (expr, pre_p, post_p)
     tree expr;
     tree *pre_p;
     tree *post_p;
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
  TREE_OPERAND (base, 0) = simplify_expr (TREE_OPERAND (base, 0), pre_p,
                                          post_p, is_simple_varname, 1);

  /* Now simplify each of the dimensions from left to right.  */
  while (VARRAY_ACTIVE_SIZE (dim_stack) > 0)
    {
      tree *dim_p = (tree *)VARRAY_TOP_GENERIC_PTR (dim_stack);
      VARRAY_POP (dim_stack);
      *dim_p = simplify_expr (*dim_p, pre_p, post_p, is_simple_val, 0);
    }

  VARRAY_FREE (dim_stack);

  /* Return the simplified array expression.  */
  return expr;
}

/* }}} */

/** {{{ simplify_self_mod_expr ()

    Simplify self modifying expression EXPR (++, --, +=, -=).

    PRE_P points to the list where side effects that must happen before
	EXPR should be stored.

    POST_P points to the list where side effects that must happen after
	EXPR should be stored.  */

static tree
simplify_self_mod_expr (expr, pre_p, post_p)
     tree expr;
     tree *pre_p;
     tree *post_p;
{
  enum tree_code code;
  tree lhs, lhs_val, rhs, t1;

  code = TREE_CODE (expr);

  if (code != POSTINCREMENT_EXPR
      && code != POSTDECREMENT_EXPR
      && code != PREINCREMENT_EXPR
      && code != PREDECREMENT_EXPR)
    abort ();

  /* The first operand needs to be simplified twice.  The first time to
     produce an lvalue suitable for the assignment and the second time to
     produce a SIMPLE value suitable for a binary expression.  */
  lhs = TREE_OPERAND (expr, 0);
  if (is_simple_val (lhs))
    lhs_val = lhs;
  else
    {
      lhs = simplify_expr (lhs, pre_p, post_p, is_simple_modify_expr_lhs, 1);
      lhs_val = simplify_expr (lhs, pre_p, post_p, is_simple_val, 0);
    }

  rhs = simplify_expr (TREE_OPERAND (expr, 1), pre_p, post_p, is_simple_val, 0);

  /* Determine whether we need to create a PLUS or a MINUS operation.  */
  if (code == PREINCREMENT_EXPR || code == POSTINCREMENT_EXPR)
    t1 = build (PLUS_EXPR, TREE_TYPE (expr), lhs_val, rhs);
  else
    t1 = build (MINUS_EXPR, TREE_TYPE (expr), lhs_val, rhs);

  /* Determine whether the new assignment should go before or after
     the simplified expression.  */
  if (code == PREINCREMENT_EXPR || code == PREDECREMENT_EXPR)
    add_tree (build_modify_expr (lhs, NOP_EXPR, t1), pre_p);
  else
    add_tree (build_modify_expr (lhs, NOP_EXPR, t1), post_p);

  /* Return the result of the simplified expression.  */
  return lhs;
}

/* }}} */

/** {{{ simplify_component_ref ()
    
    Simplify the structure reference given by EXPR to comply with:

      compref
	      : '(' '*' ID ')' '.' idlist
	      | idlist

      idlist
	      : idlist '.' ID
	      | ID

    PRE_P points to the list where side effects that must happen before
	EXPR should be stored.

    POST_P points to the list where side effects that must happen after
    	EXPR should be stored.  */

static tree
simplify_component_ref (expr, pre_p, post_p)
     tree expr;
     tree *pre_p;
     tree *post_p;
{
  tree lhs, rhs;

  if (TREE_CODE (expr) != COMPONENT_REF)
    abort ();

  /* Note that we always re-write into a SIMPLE address expression to avoid
     bitwise structure copies.  */
  lhs = simplify_expr (TREE_OPERAND (expr, 0), pre_p, post_p,
                       is_simple_compref_lhs, 1);

  rhs = simplify_expr (TREE_OPERAND (expr, 1), pre_p, post_p, is_simple_id, 0);

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

    PRE_P points to the list where side effects that must happen before
	EXPR should be stored.

    POST_P points to the list where side effects that must happen after
    	EXPR should be stored.  */

static tree
simplify_call_expr (expr, pre_p, post_p)
     tree expr;
     tree *pre_p;
     tree *post_p;
{
  tree id, arglist;

  if (TREE_CODE (expr) != CALL_EXPR)
    abort ();

  id = simplify_expr (TREE_OPERAND (expr, 0), pre_p, post_p, is_simple_id, 0);

  arglist = simplify_expr (TREE_OPERAND (expr, 1), pre_p, post_p,
                           is_simple_arglist, 0);

  TREE_OPERAND (expr, 0) = id;
  TREE_OPERAND (expr, 1) = arglist;

  return expr;
}

/* }}} */

/** {{{ simplify_tree_list ()

    Simplify the argument list given by EXPR to comply with:

      arglist
	      : arglist ',' val
	      | val

    PRE_P points to the list where side effects that must happen before
	EXPR should be stored.

    POST_P points to the list where side effects that must happen after
    	EXPR should be stored.  */

static tree
simplify_tree_list (expr, pre_p, post_p)
     tree expr;
     tree *pre_p;
     tree *post_p;
{
  tree op;

  if (TREE_CODE (expr) != TREE_LIST)
    abort ();

  for (op = expr; op; op = TREE_CHAIN (op))
    {
      tree arg;

      arg = simplify_expr (TREE_VALUE (op), pre_p, post_p, is_simple_val, 0);
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

    PRE_P points to the list where side effects that must happen before
	EXPR should be stored.

    POST_P points to the list where side effects that must happen after
    	EXPR should be stored.  */

static tree
simplify_cond_expr (expr, pre_p, post_p)
     tree expr;
     tree *pre_p;
     tree *post_p;
{
  tree t_then, t_else, tmp, pred, tval, fval, new_if;

  tmp = create_tmp_var (TREE_TYPE (expr));

  pred = simplify_expr (TREE_OPERAND (expr, 0), pre_p, post_p, is_simple_expr,
                        0);

  tval = simplify_expr (TREE_OPERAND (expr, 1), pre_p, post_p, is_simple_expr,
                        0);

  fval = simplify_expr (TREE_OPERAND (expr, 2), pre_p, post_p, is_simple_expr,
                        0);

  /* Build the THEN_CLAUSE 't1 = a;'.  */
  t_then = build_stmt (EXPR_STMT, build_modify_expr (tmp, NOP_EXPR, tval));

  /* Build the ELSE_CLAUSE 't1 = b;'.  */
  t_else = build_stmt (EXPR_STMT, build_modify_expr (tmp, NOP_EXPR, fval));

  /* Build a new IF_STMT, and insert it in the PRE_P chain.  */
  new_if = build_stmt (IF_STMT, pred, t_then, t_else);
  add_tree (new_if, pre_p);

  return tmp;
}

/* }}} */

/** {{{ simplify_modify_expr ()

    Simplify the assignment given in EXPR to comply with:

      modify_expr
	      : varname '=' rhs
	      | '*' ID '=' rhs


    PRE_P points to the list where side effects that must happen before
	EXPR should be stored.

    POST_P points to the list where side effects that must happen after
    	EXPR should be stored.  */

static tree
simplify_modify_expr (expr, pre_p, post_p)
     tree expr;
     tree *pre_p;
     tree *post_p;
{
  tree lhs, rhs;

  if (TREE_CODE (expr) != MODIFY_EXPR
      && TREE_CODE (expr) != INIT_EXPR)
    abort ();

  /* Break assignment chains (i.e., a = b = c = ...) into individual
     assignment expressions.  */
  rhs = TREE_OPERAND (expr, 1);
  if (TREE_CODE (rhs) == MODIFY_EXPR)
    {
      rhs = simplify_expr (rhs, pre_p, post_p, is_simple_expr, 0);
      add_tree (rhs, pre_p);
      TREE_OPERAND (expr, 1) = TREE_OPERAND (rhs, 0);
    }

  lhs = simplify_expr (TREE_OPERAND (expr, 0), pre_p, post_p, 
                       is_simple_modify_expr_lhs, 1);

  rhs = simplify_expr (TREE_OPERAND (expr, 1), pre_p, post_p, is_simple_rhs, 0);

  if (TREE_CODE (rhs) == MODIFY_EXPR)
    {
      add_tree (rhs, pre_p);
      rhs = TREE_OPERAND (rhs, 0);
    }

  TREE_OPERAND (expr, 0) = lhs;
  TREE_OPERAND (expr, 1) = rhs;

  return expr;
}

/* }}} */

/** {{{ simplify_boolean_expr ()

    Simplify TRUTH_ANDIF_EXPR and TRUTH_ORIF_EXPR expressions.  EXPR is
    the expression to simplify.

    Expressions of the form 'a && b' are simplified to:

    	T = a;
	if (T)
	  T = b;

    Expressions of the form 'a || b' are simplified to:

	T = a;
	if (!T)
	  T = b;

    In both cases, the expression 'T' is returned.


    PRE_P points to the list where side effects that must happen before
	EXPR should be stored.

    POST_P points to the list where side effects that must happen after
    	EXPR should be stored.  */

static tree
simplify_boolean_expr (expr, pre_p, post_p)
     tree expr;
     tree *pre_p;
     tree *post_p;
{
  enum tree_code code;
  tree t, lhs, if_body, if_cond;

  code = TREE_CODE (expr);
  if (code != TRUTH_ANDIF_EXPR && code != TRUTH_ORIF_EXPR)
    abort ();

  /* Simplify the first operand and assign its truth value to a new
     temporary.  */
  lhs = simplify_expr (TREE_OPERAND (expr, 0), pre_p, post_p, 
                       is_simple_condexpr, 0);
  t = create_tmp_var (TREE_TYPE (expr));
  add_tree (build_modify_expr (t, NOP_EXPR, lhs), pre_p);

  /* Build the body for the if() statement that conditionally evaluates the
     RHS of the expression.  Note that we first build the assignment
     surrounded by a new scope so that its simplified form is computed
     inside the new scope.  */
  if_body = build_stmt (EXPR_STMT, build_modify_expr (t, NOP_EXPR, 
	                                              TREE_OPERAND (expr, 1)));
  tree_build_scope (&if_body);
  simplify_stmt (if_body);

  /* Build the if() statement to conditionally evaluate the RHS.  */
  if (code == TRUTH_ANDIF_EXPR)
    if_cond = build (NE_EXPR, TREE_TYPE (expr), t, integer_zero_node);
  else
    if_cond = build (EQ_EXPR, TREE_TYPE (expr), t, integer_zero_node);

  add_tree (build_stmt (IF_STMT, if_cond, if_body, NULL_TREE), pre_p);

  return t;
}

/* }}} */

/** {{{ simplify_compound_expr ()

    Simplifies an expression sequence to comply with:

      exprseq
	      : exprseq ',' expr
	      | expr

    This function simplifies each expression and returns the last
    expression of the sequence in SIMPLE form.

    PRE_P points to the list where the side effects for all the expressions
	in the sequence will be emitted.

    POST_P points to the list where the post side effects for the last
	expression in the sequence are emitted.  */

static tree
simplify_compound_expr (expr, pre_p, post_p)
     tree expr;
     tree *pre_p;
     tree *post_p;
{
  tree *expr_s, *pre_expr_s, *post_expr_s;
  tree t, ret;
  int i, num;

  if (TREE_CODE (expr) != COMPOUND_EXPR)
    abort ();

  /* Count all the expressions in the sequence.  */
  num = 2;
  t = expr;
  while (TREE_OPERAND (t, 1)
         && TREE_CODE (TREE_OPERAND (t, 1)) == COMPOUND_EXPR)
    {
      num++;
      t = TREE_OPERAND (t, 1);
    }

  /* Collect all the expressions in the sequence into the EXPR_S array.  */
  expr_s = (tree *) xmalloc (num * sizeof (tree));
  memset (expr_s, 0, num * sizeof (tree));

  pre_expr_s = (tree *) xmalloc (num * sizeof (tree));
  memset (pre_expr_s, 0, num * sizeof (tree));

  post_expr_s = (tree *) xmalloc (num * sizeof (tree));
  memset (post_expr_s, 0, num * sizeof (tree));

  t = expr;
  for (i = 0; i < num; i++)
    {
      if (i < num - 1)
	{
	  expr_s[i] = TREE_OPERAND (t, 0);
	  t = TREE_OPERAND (t, 1);
	}
      else
	expr_s[i] = t;
    }


  /* Simplify each expression in the array.  Add all the side effects and
     the simplified expressions to PRE_P.  POST_P will contain the post
     side-effects of the last expression in the sequence.  After
     simplification, we return the last expression of the sequence.  */
  for (i = 0; i < num; i++)
    {
      expr_s[i] = simplify_expr (expr_s[i], &pre_expr_s[i], &post_expr_s[i],
	                         is_simple_expr, 0);

      /* Add the side-effects and the simplified expression to PRE_P.  
	 This is necessary because the comma operator represents a sequence
	 point.  */
      add_tree (pre_expr_s[i], pre_p);

      if (i < num - 1)
	{
	  add_tree (expr_s[i], pre_p);
	  add_tree (post_expr_s[i], pre_p);
	}
    }

  ret = expr_s[num - 1];
  add_tree (post_expr_s[num - 1], post_p);

  free (expr_s);
  free (pre_expr_s);
  free (post_expr_s);

  return ret;
}

/* }}} */

/** {{{ simplify_save_expr ()

    Simplify a SAVE_EXPR.  EXPR is the expression to simplify.  After
    simplification, all the nodes in PRE_P and POST_P are wrapped inside a
    SAVE_EXPR to preserve the original semantics.  The simplified
    expression is also returned inside a SAVE_EXPR node.

    PRE_P points to the list where side effects that must happen before
	EXPR should be stored.

    POST_P points to the list where side effects that must happen after
    	EXPR should be stored.

    SIMPLE_TEST_F points to a function that takes a tree T and
	returns nonzero if T is in the SIMPLE form requested by the
	caller.

    NEEDS_LVALUE should be nonzero if the caller needs to simplify the
	expression into an lvalue.  */

static tree
simplify_save_expr (expr, pre_p, post_p, simple_test_f, needs_lvalue)
     tree expr;
     tree *pre_p;
     tree *post_p;
     int (*simple_test_f) PARAMS ((tree));
     int needs_lvalue;
{
  tree op, expr_s;

  if (TREE_CODE (expr) != SAVE_EXPR)
    abort ();

  /* We should not be simplifying trees if RTL has already been
     generated.  */
  if (SAVE_EXPR_RTL (expr))
    abort ();

  expr_s = simplify_expr (TREE_OPERAND (expr, 0), pre_p, post_p, simple_test_f,
                          needs_lvalue);

  for (op = *pre_p; op; op = TREE_CHAIN (op))
    TREE_VALUE (op) = save_expr (TREE_VALUE (op));

  for (op = *post_p; op; op = TREE_CHAIN (op))
    TREE_VALUE (op) = save_expr (TREE_VALUE (op));

  return save_expr (expr_s);
}

/* }}} */

/** {{{ simplify_expr_wfl ()

    Simplify an EXPR_WITH_FILE_LOCATION.  EXPR is the expression to
    simplify. After simplification, all the nodes in PRE_P and POST_P are
    wrapped inside a EXPR_WITH_FILE_LOCATION node to preserve the original
    semantics.  The simplified expression is also returned inside an
    EXPR_WITH_FILE_LOCATION node.

    PRE_P points to the list where side effects that must happen before
	EXPR should be stored.

    POST_P points to the list where side effects that must happen after
    	EXPR should be stored.

    SIMPLE_TEST_F points to a function that takes a tree T and
	returns nonzero if T is in the SIMPLE form requested by the
	caller.

    NEEDS_LVALUE should be nonzero if the caller needs to simplify the
	expression into an lvalue.  */

static tree
simplify_expr_wfl (expr, pre_p, post_p, simple_test_f, needs_lvalue)
     tree expr;
     tree *pre_p;
     tree *post_p;
     int (*simple_test_f) PARAMS ((tree));
     int needs_lvalue;
{
  tree op, expr_s;
  const char *file;
  int line, col;

  if (TREE_CODE (expr) != EXPR_WITH_FILE_LOCATION)
    abort ();

  expr_s = simplify_expr (EXPR_WFL_NODE (expr), pre_p, post_p, simple_test_f,
                          needs_lvalue);

  file = EXPR_WFL_FILENAME (expr);
  line = EXPR_WFL_LINENO (expr);
  col = EXPR_WFL_COLNO (expr);

  for (op = *pre_p; op; op = TREE_CHAIN (op))
    TREE_VALUE (op) = build_expr_wfl (TREE_VALUE (op), file, line, col);

  for (op = *post_p; op; op = TREE_CHAIN (op))
    TREE_VALUE (op) = build_expr_wfl (TREE_VALUE (op), file, line, col);

  return build_expr_wfl (expr_s, file, line, col);
}

/* }}} */


/* Code generation.  */

/** {{{ tree_build_scope ()
   
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
      SCOPE_BEGIN_P (start_scope) = 1;
      SCOPE_BEGIN_P (end_scope) = 0;
      COMPOUND_BODY (body) = start_scope;
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
	  SCOPE_BEGIN_P (start_scope) = 1;
	  SCOPE_BEGIN_P (end_scope) = 0;
	  COMPOUND_BODY (*t) = start_scope;
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
      SCOPE_BEGIN_P (start_scope) = 1;
      SCOPE_BEGIN_P (end_scope) = 0;
      COMPOUND_BODY (body) = start_scope;
      TREE_CHAIN (start_scope) = *t;
      TREE_CHAIN (*t) = end_scope;
      *t = body;
    }
}

/* }}} */

/** {{{ add_tree ()

    Add T to the list container pointed by LIST_P.  If T is a TREE_LIST
    node, it is linked-in directly.  If T is an expression with no effects,
    it is ignored.
    
    Return the newly added list node or NULL_TREE if T was not added to
    LIST_P.  */

static tree
add_tree (t, list_p)
    tree t;
    tree *list_p;
{
  tree n;
  
  if (t == NULL_TREE)
    return NULL_TREE;

  if (TREE_CODE (t) != TREE_LIST)
    {
      /* Do nothing if T has no effect.  */
      if (statement_code_p (TREE_CODE (t)))
	{
	  if (!stmt_has_effect (t))
	    return NULL_TREE;
	}
      else
	{
	  if (!expr_has_effect (t))
	    return NULL_TREE;
	}

      n = build_tree_list (NULL_TREE, t);
    }
  else
    n = t;

  *list_p = chainon (*list_p, n);

  return n;
}

/* }}} */

/** {{{ insert_before_continue_end ()
    
    Insert the REEVAL list before CONTINUE_STMTs and at the end of the loop
    body BODY.  Set the line number of the REEVAL list to LINE.  */

void
insert_before_continue_end (reeval, body, line)
     tree reeval;
     tree body;
     int line;
{
  tree last, beforelast;

  if (reeval == NULL_TREE)
    return;

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
  beforelast = insert_before_continue (body, reeval);
  last = TREE_CHAIN (beforelast);
  
  /* If the last statement of the WHILE_BODY is not a CONTINUE_STMT, 
     then insert reeval at the end of the loop block.  */
  if (TREE_CODE (beforelast) != CONTINUE_STMT)
    {
      TREE_CHAIN (beforelast) = deep_copy_list (reeval);
      beforelast = tree_last (beforelast);
      TREE_CHAIN (beforelast) = last;
    }
}

/* }}} */

/** {{{ insert_before_continue ()
    
    Insert the statement list REEVAL before each CONTINUE_STMT in the block
    pointed to by NODE.  At the end returns a pointer to the beforelast
    node in the block NODE.  The caller can insert then the last loop
    reevaluation at the end of the loop block.  */

static tree
insert_before_continue (node, reeval)
     tree node;
     tree reeval;
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
	  TREE_CHAIN (node) = deep_copy_list (reeval);
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

/** {{{ insert_before_first ()

    Insert statement T before the first statement of the compound statement
    BODY.  */

static void
insert_before_first (t, body)
     tree t;
     tree body;
{
  tree first_stmt;

  if (TREE_CODE (body) != COMPOUND_STMT)
    abort ();

  if (t == NULL)
    return;

  first_stmt = COMPOUND_BODY (body);
  chainon (t, TREE_CHAIN (first_stmt));
  TREE_CHAIN (first_stmt) = t;
}

/* }}} */


/* Miscellaneous helpers.  */

/** {{{ create_tmp_var ()

   Create a new temporary variable declaration of type TYPE.  Returns the
   newly created decl and pushes it into the current binding.  */

tree
create_tmp_var (type)
     tree type;
{
  static unsigned int id_num = 1;
  char *tmp_name;
  tree tmp_var;

  ASM_FORMAT_PRIVATE_NAME (tmp_name, "T", id_num++);

  /* If the type is an array, use TYPE_POINTER_TO to create a valid pointer
     that can be used in the LHS of an assignment.  */
  if (TREE_CODE (type) == ARRAY_TYPE)
    type = TYPE_POINTER_TO (TREE_TYPE (type));

  tmp_var = build_decl (VAR_DECL, get_identifier (tmp_name), type);

  /* The variable was declared by the compiler.  */
  DECL_ARTIFICIAL (tmp_var) = 1;

  /* Make the variable writable.  */
  TREE_READONLY (tmp_var) = 0;

  /* Make the type of the variable writable.  */
  make_type_writable (tmp_var);

  DECL_EXTERNAL (tmp_var) = 0;
  TREE_STATIC (tmp_var) = 0;
  TREE_USED (tmp_var) = 1;

  pushdecl (tmp_var);

  return tmp_var;
}

/* }}} */

/** {{{ make_type_writable ()

    Change the flags for the type of the node T to make it writtable.  */

static void 
make_type_writable (t)
     tree t;
{
  if (t == NULL_TREE)
    abort ();

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

/** {{{ declare_tmp_vars ()

    Declares all the variables in VARS in SCOPE.  Returns the last DECL_STMT
    emitted.  */

tree
declare_tmp_vars (vars, scope)
     tree vars;
     tree scope;
{
  tree t, last;

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

/** {{{ tree_last_decl ()
    
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

/** {{{ deep_copy_list ()
    
    Copy every statement from the chain CHAIN by calling deep_copy_node.
    Return the new chain.  */

tree
deep_copy_list (chain)
     tree chain;
{
  tree new_chain, res;

  if (chain == NULL_TREE)
    /* Nothing to copy.  */
    return NULL_TREE;
  
  new_chain = deep_copy_node (chain);
  res = new_chain;

  while (TREE_CHAIN (chain))
    {
      chain = TREE_CHAIN (chain);
      TREE_CHAIN (new_chain) = deep_copy_node (chain);
      new_chain = TREE_CHAIN (new_chain);
    }

  return res;
}

/* }}} */

/** {{{ deep_copy_node ()

    Copy a statement by using copy_node when needed, but ensuring that we copy 
    enough information in order to have distinct statements.  */

tree 
deep_copy_node (node)
     tree node;
{
  tree res;

  if (node == NULL_TREE)
    return NULL_TREE;

  switch (TREE_CODE (node))
    {
    case COMPOUND_STMT:
      res = build_stmt (COMPOUND_STMT, 
			deep_copy_list (COMPOUND_BODY (node)));
      break;

    case FOR_STMT:
      res = build_stmt (FOR_STMT, 
			deep_copy_node (FOR_INIT_STMT (node)),
			(FOR_COND (node) ? 
			 copy_node (FOR_COND (node)) : NULL_TREE),
			(FOR_EXPR (node) ? 
			 copy_node (FOR_EXPR (node)) : NULL_TREE),
			deep_copy_node (FOR_BODY (node)));
      break;

    case WHILE_STMT:
      res = build_stmt (WHILE_STMT, 
			(WHILE_COND (node) ? 
			 copy_node (WHILE_COND (node)) : NULL_TREE),
			deep_copy_node (WHILE_BODY (node)));
      break;

    case DO_STMT:
      res = build_stmt (DO_STMT, 
			(DO_COND (node) ? 
			 copy_node (DO_COND (node)) : NULL_TREE),
			deep_copy_node (DO_BODY (node)));
      break;

    case IF_STMT:
      res = build_stmt (IF_STMT, 
			(IF_COND (node) ? 
			 copy_node (IF_COND (node)) : NULL_TREE),
			deep_copy_node (THEN_CLAUSE (node)),
			deep_copy_node (ELSE_CLAUSE (node)));
      break;

    case SWITCH_STMT:
      res = build_stmt (SWITCH_STMT,
			(SWITCH_COND (node) ? 
			 copy_node (SWITCH_COND (node)) : NULL_TREE),
			deep_copy_node (SWITCH_BODY (node)));
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

    case TREE_LIST:
      res = build_tree_list (deep_copy_node (TREE_PURPOSE (node)),
	                     deep_copy_node (TREE_VALUE (node)));
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
  if (statement_code_p (TREE_CODE (node)))
    STMT_LINENO (res) = STMT_LINENO (node);
  
  return res;
}

/* }}} */

/** {{{ update_line_number ()

    Updates the STMT_LINENO of each stmt in the tree t to the line number
    LINE.  Returns the last stmt in the tree chain.  */

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

/** {{{ convert_to_stmt_chain ()

    Convert the list of expressions LIST into a list of statements.  Each
    statement in the new list gets line number information from STMT.  */

static tree
convert_to_stmt_chain (list, stmt)
     tree list;
     tree stmt;
{
  tree op, stmt_list;

  stmt_list = NULL;
  for (op = list; op; op = TREE_CHAIN (op))
    {
      tree t, n;
      
      /* Only create a new statement for expression trees.  */
      t = TREE_VALUE (op);
      n = (statement_code_p (TREE_CODE (t))) ? t : build_stmt (EXPR_STMT, t);
      STMT_LINENO (n) = STMT_LINENO (stmt);

      /* Only add statements that have an effect.  */
      if (stmt_has_effect (n))
	stmt_list = chainon (stmt_list, n);
    }

  return stmt_list;
}

/* }}} */

/** {{{ stmt_has_effect ()

    Return nonzero if STMT has some effect (i.e., if it's not of the form
    'a;' where a is a non-volatile variable).  */
    
static int
stmt_has_effect (stmt)
     tree stmt;
{
  if (TREE_CODE (stmt) != EXPR_STMT)
    return 1;
  else
    return expr_has_effect (EXPR_STMT_EXPR (stmt));
}

/* }}} */

/** {{{ expr_has_effect ()

    Return nonzero if EXPR has some effect (e.g., it's not a single
    non-volatile VAR_DECL).  */

static int
expr_has_effect (expr)
     tree expr;
{
  return (TREE_SIDE_EFFECTS (expr)
	  || (TREE_CODE (expr) == CONVERT_EXPR
	      && VOID_TYPE_P (TREE_TYPE (expr))));
}

/* }}} */

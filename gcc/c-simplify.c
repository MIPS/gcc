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
#include "hard-reg-set.h"
#include "basic-block.h"
#include "tree-flow.h"
#include "tree-inline.h"
#include "diagnostic.h"
#include "langhooks.h"
#include "flags.h"
#include "rtl.h"
#include "toplev.h"

/*  The simplification pass converts the language-dependent trees
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

    http://www-acaps.cs.mcgill.ca/info/McCAT/McCAT.html

    Basically, we walk down simplifying the nodes that we encounter.  As we
    walk back up, we check that they fit our constraints, and copy them
    into temporaries if not.  */

/* Local declarations.  */

static void simplify_stmt            PARAMS ((tree *));
static void simplify_expr_stmt       PARAMS ((tree, tree *, tree *));
static void simplify_decl_stmt       PARAMS ((tree, tree *, tree *, tree *));
static void simplify_for_stmt        PARAMS ((tree, tree *));
static void simplify_while_stmt      PARAMS ((tree, tree *));
static void simplify_do_stmt         PARAMS ((tree));
static void simplify_if_stmt         PARAMS ((tree, tree *));
static void simplify_switch_stmt     PARAMS ((tree, tree *));
static void simplify_return_stmt     PARAMS ((tree, tree *));
static void simplify_stmt_expr       PARAMS ((tree *, tree *));
static void simplify_compound_literal_expr PARAMS ((tree *, tree *, tree *));
static void make_type_writable       PARAMS ((tree));
static tree insert_before_continue   PARAMS ((tree, tree));
static tree tree_last_decl           PARAMS ((tree));
static int  stmt_has_effect          PARAMS ((tree));
static int  expr_has_effect          PARAMS ((tree));
static tree mostly_copy_tree_r       PARAMS ((tree *, int *, void *));
static inline void remove_suffix     PARAMS ((char *, int));
static const char *get_name          PARAMS ((tree));
static int is_last_stmt_of_scope     PARAMS ((tree));
static tree tail_expression          PARAMS ((tree *, int));
static tree mark_not_simple_r        PARAMS ((tree *, int *, void *));

/* Should move to tree-simple.c when the target language loses the C-isms.  */
static void simplify_constructor     PARAMS ((tree, tree *, tree *));
static void simplify_array_ref       PARAMS ((tree *, tree *, tree *));
static void simplify_compound_lval   PARAMS ((tree *, tree *, tree *));
static void simplify_component_ref   PARAMS ((tree *, tree *, tree *));
static void simplify_call_expr       PARAMS ((tree *, tree *, tree *));
static void simplify_tree_list       PARAMS ((tree *, tree *, tree *));
static void simplify_modify_expr     PARAMS ((tree *, tree *, tree *));
static void simplify_compound_expr   PARAMS ((tree *, tree *, tree *));
static void simplify_save_expr       PARAMS ((tree *, tree *));
static void simplify_addr_expr       PARAMS ((tree *, tree *, tree *));
static void simplify_self_mod_expr   PARAMS ((tree *, tree *, tree *));
static void simplify_cond_expr       PARAMS ((tree *, tree *));
static void simplify_boolean_expr    PARAMS ((tree *, tree *));
static void simplify_expr_wfl        PARAMS ((tree *, tree *, tree *,
                                              int (*) (tree)));
static tree build_addr_expr	      PARAMS ((tree));


/* Local variables.  */
static FILE *dump_file;
static int dump_flags;

/* Simplification of statement trees.  */

/*  Entry point to the simplification pass.  FNDECL is the FUNCTION_DECL
    node for the function we want to simplify.  */

int
simplify_function_tree (fndecl)
     tree fndecl;
{
  tree fnbody, fntree;
  int done;

  /* Don't bother doing anything if the program has errors.  */
  if (errorcount || sorrycount)
    return 0;
  
  fntree = DECL_SAVED_TREE (fndecl);
  if (fntree == NULL_TREE || TREE_CODE (fntree) != COMPOUND_STMT)
    return 0;

  fnbody = COMPOUND_BODY (fntree);
  if (fnbody == NULL_TREE)
    return 0;

  /* Debugging dumps.  */
  dump_file = dump_begin (TDI_original, &dump_flags);
  if (dump_file)
    {
      fprintf (dump_file, "%s()\n", IDENTIFIER_POINTER (DECL_NAME (fndecl)));

      if (dump_flags & TDF_RAW)
	dump_node (fnbody, TDF_SLIM | dump_flags, dump_file);
      else
	print_c_tree (dump_file, fnbody);
      fprintf (dump_file, "\n");

      dump_end (TDI_original, dump_file);
    }

  /* Create a new binding level for the temporaries created by the
     simplification process.  */
  pushlevel (0);

  /* Simplify the function's body.  */
  done = simplify_expr (&fnbody, NULL, NULL, NULL, fb_rvalue);

  /* Return if simplification failed.  */
  if (!done)
    {
      poplevel (1, 1, 0);
      return 0;
    }

  /* Declare the new temporary variables.  */
  declare_tmp_vars (getdecls(), fnbody);

  /* Restore the binding level.  */
  poplevel (1, 1, 0);

  /* Debugging dump after simplification.  */
  dump_file = dump_begin (TDI_simple, &dump_flags);
  if (dump_file)
    {
      fprintf (dump_file, "%s()\n", IDENTIFIER_POINTER (DECL_NAME (fndecl)));

      if (dump_flags & TDF_RAW)
	dump_node (fnbody, TDF_SLIM | dump_flags, dump_file);
      else
	print_c_tree (dump_file, fnbody);
      fprintf (dump_file, "\n");

      dump_end (TDI_simple, dump_file);
    }

  return 1;
}


/*  Entry point for the tree lowering pass.  Recursively scan
    *STMT_P and convert it to a SIMPLE tree.  */

static void 
simplify_stmt (stmt_p)
     tree *stmt_p;
{
  /* PRE and POST are tree chains that contain the side-effects of the
     simplified tree.  For instance, given the expression tree:

     		c = ++a * 3 + b++;

     After simplification, the tree will be re-written as:

     		a = a + 1;
		t1 = a * 3;	<-- PRE
     		c = t1 + b;
     		b = b + 1;	<-- POST  */

  while (*stmt_p && *stmt_p != error_mark_node)
    {
      tree next, pre, post;
      int keep_stmt_p;
      tree stmt = *stmt_p;
      int saved_stmts_are_full_exprs_p;

      /* Set up context appropriately for handling this statement.  */
      saved_stmts_are_full_exprs_p = stmts_are_full_exprs_p ();
      prep_stmt (stmt);

      pre = NULL;
      post = NULL;

      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "# %d\nORIGINAL:\n", STMT_LINENO (stmt));
	  print_c_node (dump_file, stmt);
	  fprintf (dump_file, "\n");
	}

      switch (TREE_CODE (stmt))
	{
	case COMPOUND_STMT:
	  simplify_stmt (&COMPOUND_BODY (stmt));
	  stmt_p = &TREE_CHAIN (stmt);
	  goto cont;
	  
	case FOR_STMT:
	  simplify_for_stmt (stmt, &pre);
	  break;
	  
	case WHILE_STMT:
	  simplify_while_stmt (stmt, &pre);
	  break;

	case DO_STMT:
	  simplify_do_stmt (stmt);
	  break;

	case IF_STMT:
	  simplify_if_stmt (stmt, &pre);
	  break;
	  
	case SWITCH_STMT:
	  simplify_switch_stmt (stmt, &pre);
	  break;

	case EXPR_STMT:
	  simplify_expr_stmt (stmt, &pre, &post);
	  break;

	case RETURN_STMT:
	  simplify_return_stmt (stmt, &pre);
	  break;

	case DECL_STMT:
	  simplify_decl_stmt (stmt, &pre, &post, &post);
	  break;

	case CLEANUP_STMT:
	  /* XXX: need to clean up CLEANUP_STMT.  Idea: turn it into
	     an statement-expression and simplify that.  */
	  break;

	/* Statements that need no simplification.  */
	case LABEL_STMT:
	case GOTO_STMT:
	case ASM_STMT:
	case CASE_LABEL:
	case CONTINUE_STMT:
	case BREAK_STMT:
	case SCOPE_STMT:
	  stmt_p = &TREE_CHAIN (stmt);
	  goto cont;

	case FILE_STMT:
	  input_filename = FILE_STMT_FILENAME (stmt);
	  stmt_p = &TREE_CHAIN (stmt);
	  goto cont;

	default:
	  fprintf (stderr, "unhandled statement node in simplify_stmt ():\n");
	  debug_tree (stmt);
	  abort ();
	  break;
	}

      /* PRE and POST now contain a list of statements for all the
	 side-effects in STMT.  */

      /* Before re-chaining the side effects, determine if we are going to
	 keep the original statement or not.  If the statement had no
	 effect before simplification, we emit it anyway to avoid changing
	 the semantics of the original program.  */
      keep_stmt_p = stmt_has_effect (stmt);
      
      next = TREE_CHAIN (stmt);
      TREE_CHAIN (stmt) = NULL_TREE;

      /* Dump the side-effects and the simplified statement.  */
      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "# %d\nPRE:\n", STMT_LINENO (stmt));
	  print_c_tree (dump_file, pre);
	  fprintf (dump_file, "\n");

	  fprintf (dump_file, "SIMPLIFIED:\n");
	  print_c_tree (dump_file, stmt);
	  fprintf (dump_file, "\n");

	  fprintf (dump_file, "POST:\n");
	  print_c_tree (dump_file, post);
	  fprintf (dump_file, "\n");
	}

      if (keep_stmt_p)
	pre = chainon (pre, stmt);
      pre = chainon (pre, post);
      *stmt_p = pre;

      /* Next iteration.  Re-set PREV to the last statement of the chain
	 PREV -> PRE -> STMT -> POST.  */
      if (*stmt_p)
	{
	  stmt = tree_last (*stmt_p);
	  stmt_p = &TREE_CHAIN (stmt);
	}
      *stmt_p = next;

    cont:
      /* Restore saved state.  */
      current_stmt_tree ()->stmts_are_full_exprs_p
	= saved_stmts_are_full_exprs_p;
    }
}

/*  Simplify an EXPR_STMT node.

    STMT is the statement node.

    PRE_P points to the list where side effects that must happen before
	STMT should be stored.

    POST_P points to the list where side effects that must happen after
	STMT should be stored.  */

static void
simplify_expr_stmt (stmt, pre_p, post_p)
     tree stmt;
     tree *pre_p;
     tree *post_p;
{
  /* Simplification of a statement expression will nullify the
     statement if all its side effects are moved to *PRE_P and *POST_P.

     In this case we will not want to emit the simplified statement.
     However, we may still want to emit a warning, so we do that before
     simplification.  */
  if (extra_warnings || warn_unused_value)
    {
      const char *fname = DECL_SOURCE_FILE (current_function_decl);
      int lineno = STMT_LINENO (stmt);

      if (!stmt_has_effect (stmt))
	warning_with_file_and_line (fname, lineno, "statement with no effect");
      else if (warn_unused_value)
	{
	  set_file_and_line_for_stmt (fname, lineno);
	  warn_if_unused_value (EXPR_STMT_EXPR (stmt));
	}
    }

  walk_tree (&EXPR_STMT_EXPR (stmt), mostly_copy_tree_r, NULL, NULL);
  simplify_expr (&EXPR_STMT_EXPR (stmt), pre_p, post_p, is_simple_expr,
                 fb_rvalue);
}

/*  Simplify a FOR_STMT node.  This will convert:

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
	    body;
	    pre_expr_s;
	    expr_s;
	    post_expr_s;
	    pre_cond_s;
	  }

    where INIT_S, COND_S and EXPR_S are the simplified versions of INIT,
    COND and EXPR respectively.  PRE_*_S and POST_*_S are the side-effects
    for each expression in the header.

    Note that the above form is the more general case, other variations are
    possible if any of the PRE_*_S or POST_*S expressions are missing.

    The order in which the different pieces are simplified is also
    important.  Simplification should be done in the same order in which
    the loop will execute at runtime.

    PRE_P points to the list where side effects that must happen before
	STMT should be store.  These are all the expressions in
	FOR_INIT_STMT and the pre side-effects of the loop conditional.  */

static void
simplify_for_stmt (stmt, pre_p)
     tree stmt;
     tree *pre_p;
{
  int init_is_simple, cond_is_simple, expr_is_simple;
  tree pre_init_s, init_s, post_init_s;
  tree pre_cond_s, cond_s;
  tree pre_expr_s, expr_s, post_expr_s;

  /* Make sure that the loop body has a scope.  */
  tree_build_scope (&FOR_BODY (stmt));

  /* If FOR_INIT_STMT is a DECL_STMT (C99), move it outside the loop and
     replace it with an empty expression statement.  */

  if (TREE_CODE (FOR_INIT_STMT (stmt)) == DECL_STMT)
    {
      tree decl_s = FOR_INIT_STMT (stmt);
      pre_init_s = NULL_TREE;
      simplify_decl_stmt (decl_s, pre_p, &pre_init_s, &pre_init_s);
      add_tree (decl_s, pre_p);
      add_tree (pre_init_s, pre_p);
      FOR_INIT_STMT (stmt) = build_stmt (EXPR_STMT, NULL_TREE);
    }

  init_s = EXPR_STMT_EXPR (FOR_INIT_STMT (stmt));
  cond_s = FOR_COND (stmt);
  expr_s = FOR_EXPR (stmt);

  /* Check if we need to do anything.  */
  init_is_simple = (init_s == NULL_TREE || is_simple_exprseq (init_s));
  cond_is_simple = (cond_s == NULL_TREE || is_simple_condexpr (cond_s));
  expr_is_simple = (expr_s == NULL_TREE || is_simple_exprseq (expr_s));

  if (init_is_simple && cond_is_simple && expr_is_simple)
    {
      /* Nothing to do, simplify the body and return.  */
      simplify_stmt (&FOR_BODY (stmt));
      return;
    }

  pre_init_s = NULL_TREE;
  post_init_s = NULL_TREE;
  pre_cond_s = NULL_TREE;
  pre_expr_s = NULL_TREE;
  post_expr_s = NULL_TREE;

  /* Simplify FOR_INIT_STMT.  Note that we always simplify it, even if it's
     in SIMPLE form already.  This is because we need to insert PRE_COND_S
     right after the initialization statements, and if PRE_COND_S contains
     statement trees, we cannot add them to a COMPOUND_EXPR:

	BEFORE				AFTER

					pre_init_s;
					init_s;
					post_init_s;
					pre_cond_s;
	for (init; cond; ...)		for ( ; cond_s; ...)

     FIXME: Since FOR_INIT_STMT can be a COMPOUND_EXPR, it should be possible
	    to emit PRE_INIT_S, INIT_S, POST_INIT_S and PRE_COND_S into a
	    COMPOUND_EXPR inside FOR_INIT_STMT.  However, this is not
	    possible if any of these elements contains statement trees.  */
  walk_tree (&init_s, mostly_copy_tree_r, NULL, NULL);
  simplify_expr (&init_s, &pre_init_s, &post_init_s, is_simple_expr,
		 fb_rvalue);

  /* Simplify FOR_COND.  */
  if (!cond_is_simple)
    {
      walk_tree (&cond_s, mostly_copy_tree_r, NULL, NULL);
      simplify_expr (&cond_s, &pre_cond_s, NULL, is_simple_condexpr,
		     fb_rvalue);
    }

  /* Simplify the body of the loop.  */
  simplify_stmt (&FOR_BODY (stmt));

  /* Simplify FOR_EXPR.  Note that if FOR_EXPR needs to be simplified,
     it's converted into a simple_expr because we need to move it out of
     the loop header (see previous FIXME note for future enhancement).  */
  if (!expr_is_simple)
    {
      walk_tree (&expr_s, mostly_copy_tree_r, NULL, NULL);
      simplify_expr (&expr_s, &pre_expr_s, &post_expr_s, is_simple_expr,
		     fb_rvalue);
    }  

  /* Now that all the components are simplified, we have to build a new
     loop with all the side-effects in the right spots:

    	pre_init_s;
	init_s;
	post_init_s;
	pre_cond_s;
	for ( ; cond_s; )
	  {
	    body;
	    pre_expr_s;
	    expr_s;
	    post_expr_s;
	    pre_cond_s;
	  }

     The above is the more general case, which produces a for() loop that
     doesn't resemble the original.  To minimize shape changes, we try to
     insert expressions in FOR_INIT_STMT and FOR_EXPR.  */

  /* Link PRE_INIT_S, INIT_S, POST_INIT_S and a copy of PRE_COND_S to make
     up a new FOR_INIT_STMT.  If the last tree in the list is an expression
     tree, it is emmitted inside FOR_INIT_STMT.  We emit a copy of
     PRE_COND_S because we also need to emit it at every wrap-around point
     in the loop body.  */
  add_tree (pre_init_s, pre_p);
  add_tree (init_s, pre_p);
  add_tree (post_init_s, pre_p);
  add_tree (deep_copy_list (pre_cond_s), pre_p);

  EXPR_STMT_EXPR (FOR_INIT_STMT (stmt)) = deep_copy_list (tail_expression (pre_p, 0));

  /* Build the new FOR_COND.  */
  FOR_COND (stmt) = cond_s;

  /* Link PRE_EXPR_S, EXPR_S, POST_EXPR_S and PRE_COND_S to emit before
     every wrap-around point inside the loop body.  If the last tree in the
     list is an expression tree, it is emitted inside FOR_EXPR.  */
  {
    tree expr_chain;

    expr_chain = pre_expr_s;
    add_tree (expr_s, &expr_chain);
    add_tree (post_expr_s, &expr_chain);
    add_tree (pre_cond_s, &expr_chain);

    FOR_EXPR (stmt) = deep_copy_list (tail_expression (&expr_chain, 1));

    if (expr_chain)
      insert_before_continue_end (expr_chain, FOR_BODY (stmt));
  }
}


/*  Simplify a WHILE_STMT node.  This will convert:

    	while (cond)
	  {
	    body;
	  }

    into

	pre_cond_s;
    	while (cond_s)
	  {
	    body;
	    pre_cond_s;
	  }

    where COND_S is the simplified version of the loop predicate.
    PRE_COND_S are the pre side-effects produced by the simplification of
    the conditional.
    
    These side-effects will be replicated at every wrap-around point inside
    the loop's body.

    The order in which the different pieces are simplified is also
    important.  Simplification should be done in the same order in which
    the loop will execute at runtime.

    STMT is the statement to simplify.

    PRE_P points to the list where side effects that must happen before
	STMT should be stored.  */

static void
simplify_while_stmt (stmt, pre_p)
     tree stmt;
     tree *pre_p;
{
  tree cond_s, stmt_chain;
  tree pre_cond_s = NULL_TREE;

  cond_s = WHILE_COND (stmt);
  
  /* Make sure that the loop body has a scope.  */
  tree_build_scope (&WHILE_BODY (stmt));
  
  /* Simplify the body of the loop.  */
  simplify_stmt (&WHILE_BODY (stmt)); 

  /* Check wether the loop condition is already simplified.  */
  if (is_simple_condexpr (WHILE_COND (stmt)))
    {
      /* Nothing to do.  */
      return;
    }
    
  /* Simplify the loop conditional.  */
  walk_tree (&cond_s, mostly_copy_tree_r, NULL, NULL);
  simplify_expr (&cond_s, &pre_cond_s, NULL, is_simple_condexpr,
		 fb_rvalue);
  WHILE_COND (stmt) = cond_s;
  add_tree (pre_cond_s, pre_p);

  /* Insert all the side-effects for the conditional before every
     wrap-around point in the loop body (i.e., before every first-level
     CONTINUE and before the end of the body).  */
  stmt_chain = deep_copy_list (pre_cond_s);
  insert_before_continue_end (stmt_chain, WHILE_BODY (stmt));
}


/*  Simplify a DO_STMT node.  This will convert:

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
	  }
    	while (cond_s);

    where COND_S is the simplified version of the loop predicate.
    PRE_COND_S are the pre side-effects produced by the simplification of
    the conditional.

    These side-effects will be replicated at every wrap-around point inside
    the loop's body.

    The order in which the different pieces are simplified is also
    important.  Simplification should be done in the same order in which
    the loop will execute at runtime.

    STMT is the statement to simplify.  */

static void
simplify_do_stmt (stmt)
     tree stmt;
{
  tree cond_s, pre_cond_s, stmt_chain;

  /* Make sure that the loop body has a scope.  */
  tree_build_scope (&DO_BODY (stmt));

  /* Simplify the loop's body.  */
  simplify_stmt (&DO_BODY (stmt));

  /* Check wether the loop condition is already simplified.  */
  if (is_simple_condexpr (DO_COND (stmt)))
    return;

  /* Simplify the loop conditional.  */
  pre_cond_s = NULL;
  cond_s = DO_COND (stmt);
  walk_tree (&cond_s, mostly_copy_tree_r, NULL, NULL);
  simplify_expr (&cond_s, &pre_cond_s, NULL, is_simple_condexpr,
		 fb_rvalue);
  DO_COND (stmt) = cond_s;

  stmt_chain = deep_copy_list (pre_cond_s);
  insert_before_continue_end (stmt_chain, DO_BODY (stmt));
}


/*  Simplify an IF_STMT.  This will convert:

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
    	if (cond_s)
	  {
	    then_clause;
	  }
	else
	  {
	    else_clause;
	  }

    where COND_S is the simplified version of the predicate. PRE_COND_S are
    the pre side-effects produced by the simplification of the conditional.

    The order in which the different pieces are simplified is also
    important.  Simplification should be done in the same order in which
    the loop will execute at runtime.

    STMT is the statement to simplify.

    PRE_P points to the list where side effects that must happen before
	STMT should be stored.  */

static void
simplify_if_stmt (stmt, pre_p)
     tree stmt;
     tree *pre_p;
{
  tree cond_s;

  /* Make sure each clause is contained inside a scope.  */
  if (THEN_CLAUSE (stmt))
    tree_build_scope (&THEN_CLAUSE (stmt));

  if (ELSE_CLAUSE (stmt))
    tree_build_scope (&ELSE_CLAUSE (stmt));
      		
  if (! is_simple_condexpr (IF_COND (stmt)))
    {
      /* Simplify the conditional.  */
      cond_s = IF_COND (stmt);
      walk_tree (&cond_s, mostly_copy_tree_r, NULL, NULL);
      simplify_expr (&cond_s, pre_p, NULL, is_simple_condexpr,
		     fb_rvalue);
      IF_COND (stmt) = cond_s;
    }

  /* Simplify each of the clauses.  */
  if (THEN_CLAUSE (stmt))
    simplify_stmt (&THEN_CLAUSE (stmt));

  if (ELSE_CLAUSE (stmt))
    simplify_stmt (&ELSE_CLAUSE (stmt));
}


/*  Simplify a SWITCH_STMT.  This will convert:

    	switch (cond)
	  {
	    body;
	  }

    into

	pre_cond_s;
    	switch (cond_s)
	  {
	    body;
	  }

    where COND_S is the simplified version of the predicate. PRE_COND_S are
    the pre side-effects produced by the simplification of the conditional.
    
    The order in which the different pieces are simplified is also
    important.  Simplification should be done in the same order in which
    the loop will execute at runtime.

    STMT is the statement to simplify.

    PRE_P points to the list where side effects that must happen before
	STMT should be stored.  */

static void
simplify_switch_stmt (stmt, pre_p)
     tree stmt;
     tree *pre_p;
{
  if (!is_simple_val (SWITCH_COND (stmt)))
    {
      /* Simplify the conditional.  */
      walk_tree (&SWITCH_COND (stmt), mostly_copy_tree_r, NULL, NULL);
      simplify_expr (&SWITCH_COND (stmt), pre_p, NULL, is_simple_val,
		     fb_rvalue);
    }

  simplify_stmt (&SWITCH_BODY (stmt));
}


/*  Simplify a RETURN_STMT.  If the expression to be returned is not a
    SIMPLE value, it is assigned to a new temporary and the statement is
    re-written to return the temporary.

    PRE_P points to the list where side effects that must happen before
	STMT should be stored.  */

static void
simplify_return_stmt (stmt, pre_p)
     tree stmt;
     tree *pre_p;
{
  tree ret_expr = RETURN_STMT_EXPR (stmt);

  if (ret_expr)
    {
      if (VOID_TYPE_P (TREE_TYPE (TREE_TYPE (current_function_decl))))
	{
	  /* We are trying to return an expression in a void function.
	     Move the expression to before the return.  */
	  walk_tree (&ret_expr, mostly_copy_tree_r, NULL, NULL);
	  simplify_expr (&ret_expr, pre_p, NULL, is_simple_expr, fb_rvalue);
	  add_tree (ret_expr, pre_p);
	  RETURN_STMT_EXPR (stmt) = NULL_TREE;
	}
      else
	{
#if defined ENABLE_CHECKING
	  /* A return expression is represented by a MODIFY_EXPR node that
	     assigns the return value into a RESULT_DECL.  */
	  if (TREE_CODE (ret_expr) != MODIFY_EXPR
	      && TREE_CODE (ret_expr) != INIT_EXPR)
	    abort ();
#endif

	  /* The grammar calls for a simple VAL here, but the RETURN_STMT
	     already uses a MODIFY_EXPR, and using a full RHS allows us to
	     optimize returning a call to a function of struct type.  */
	  if (is_simple_rhs (TREE_OPERAND (ret_expr, 1)))
	    return;

	  walk_tree (&TREE_OPERAND (ret_expr, 1), mostly_copy_tree_r,
		     NULL, NULL);
	  simplify_expr (&TREE_OPERAND (ret_expr, 1), pre_p, NULL,
			 is_simple_rhs, fb_rvalue);
	}
    }
}

/*  Simplifies a DECL_STMT node T.

    If a declaration V has an initial value I, create an expression 'V = I'
    and insert it after the DECL_STMT.

    PRE_P is a queue for effects that should happen before the DECL_STMT.

    MID_P is a queue for effects that should happen after the DECL_STMT,
    but before uses of the initialized decl.

    POST_P is a queue for effects that should happen after uses of the
    initialized decl.

    Usually these last two will be the same, but they may need to be
    different if the DECL_STMT is somehow embedded in an expression.  */

static void
simplify_decl_stmt (t, pre_p, mid_p, post_p)
     tree t;
     tree *pre_p;
     tree *mid_p;
     tree *post_p;
{
  tree decl, init, mid, post;

#if defined ENABLE_CHECKING
  if (TREE_CODE (t) != DECL_STMT)
    abort ();
#endif

  if (is_simple_decl_stmt (t))
    return;

  /* We need to build these up internally first, in case mid_p and post_p
     are the same.  */
  mid = NULL_TREE;
  post = NULL_TREE;

  decl = DECL_STMT_DECL (t);
  init = DECL_INITIAL (decl);
  DECL_INITIAL (decl) = NULL_TREE;

  if (DECL_SIZE_UNIT (decl)
      && TREE_CODE (DECL_SIZE_UNIT (decl)) != INTEGER_CST)
    {
      walk_tree (&DECL_SIZE_UNIT (decl), mostly_copy_tree_r, NULL, NULL);
      simplify_expr (&DECL_SIZE_UNIT (decl), pre_p, &post, is_simple_val,
		    fb_rvalue);
    }

  if (init)
    {
      walk_tree (&init, mostly_copy_tree_r, NULL, NULL);
      simplify_expr (&init, &mid, &post, is_simple_initializer, fb_rvalue);
      init = build (INIT_EXPR, TREE_TYPE (decl), decl, init);
      add_tree (mid, mid_p);
      add_tree (init, mid_p);
    }

  add_tree (post, post_p);
}

/* Simplifies a CONSTRUCTOR node T.

   FIXME: Should dynamic initializations from a CONSTRUCTOR be broken
   up into multiple assignments?  */

static void
simplify_constructor (t, pre_p, post_p)
     tree t;
     tree *pre_p;
     tree *post_p;
{
  tree elt_list;

  if (is_simple_constructor (t))
    return;

  for (elt_list = CONSTRUCTOR_ELTS (t); elt_list;
       elt_list = TREE_CHAIN (elt_list))
    simplify_expr (&TREE_VALUE (elt_list), pre_p, post_p,
		   is_simple_constructor_elt, fb_rvalue);
}

/* Simplify a C99 compound literal expression.  This just means adding the
   DECL_STMT before the current EXPR_STMT and using its anonymous decl
   instead.  */

static void
simplify_compound_literal_expr (expr_p, pre_p, post_p)
     tree *expr_p;
     tree *pre_p;
     tree *post_p;
{
  tree decl_s = COMPOUND_LITERAL_EXPR_DECL_STMT (*expr_p);
  tree decl = DECL_STMT_DECL (decl_s);
  tree pre_init = NULL_TREE;

  simplify_decl_stmt (decl_s, pre_p, &pre_init, post_p);
  add_tree (decl_s, pre_p);
  add_tree (pre_init, pre_p);
  *expr_p = decl;
}

/* Simplification of expression trees.  */

/*  Simplifies the expression tree pointed by EXPR_P.  Return 0 if
    simplification failed.

    PRE_P points to the list where side effects that must happen before
	EXPR should be stored.

    POST_P points to the list where side effects that must happen after
	EXPR should be stored, or NULL if there is no suitable list.  In
	that case, we copy the result to a temporary, emit the
	post-effects, and then return the temporary.

    SIMPLE_TEST_F points to a function that takes a tree T and
	returns nonzero if T is in the SIMPLE form requested by the
	caller.  The SIMPLE predicates are in tree-simple.c.

	This test is used twice.  Before simplification, the test is
	invoked to determine whether *EXPR_P is already simple enough.  If
	that fails, *EXPR_P is simplified according to its code and
	SIMPLE_TEST_F is called again.  If the test still fails, then a new
	temporary variable is created and assigned the value of the
	simplified expression.

    FALLBACK tells the function what sort of a temporary we want.  If the 1
        bit is set, an rvalue is OK.  If the 2 bit is set, an lvalue is OK.
        If both are set, either is OK, but an lvalue is preferable.  */

int
simplify_expr (expr_p, pre_p, post_p, simple_test_f, fallback)
     tree *expr_p;
     tree *pre_p;
     tree *post_p;
     int (*simple_test_f) PARAMS ((tree));
     fallback_t fallback;
{
  tree tmp;
  tree internal_pre = NULL_TREE;
  tree internal_post = NULL_TREE;
  tree save_expr;
  int is_statement = (pre_p == NULL);

  if (*expr_p == NULL_TREE)
    return 1;

  /* Set up our internal queues if needed.  */
  if (pre_p == NULL)
    pre_p = &internal_pre;
  if (post_p == NULL)
    post_p = &internal_post;

  /* Loop over the specific simplifiers until the toplevel node remains the
     same.  */
  do
    {
      /* First do any language-specific simplification.  */
      (*lang_hooks.simplify_expr) (expr_p, pre_p, post_p);

      /* Then remember the expr.  */
      save_expr = *expr_p;

      switch (TREE_CODE (*expr_p))
	{
	  /* First deal with the special cases.  */

	case POSTINCREMENT_EXPR:
	case POSTDECREMENT_EXPR:
	case PREINCREMENT_EXPR:
	case PREDECREMENT_EXPR:
	  simplify_self_mod_expr (expr_p, pre_p, post_p);
	  break;

	case ARRAY_REF:
	  simplify_array_ref (expr_p, pre_p, post_p);
	  break;

	case COMPONENT_REF:
	  simplify_component_ref (expr_p, pre_p, post_p);
	  break;
      
	case COND_EXPR:
	  simplify_cond_expr (expr_p, pre_p);
	  break;

	case CALL_EXPR:
	  simplify_call_expr (expr_p, pre_p, post_p);
	  break;

	case TREE_LIST:
	  simplify_tree_list (expr_p, pre_p, post_p);
	  break;

	case COMPOUND_EXPR:
	  simplify_compound_expr (expr_p, pre_p, post_p);
	  break;

	case REALPART_EXPR:
	case IMAGPART_EXPR:
	  return simplify_expr (&TREE_OPERAND (*expr_p, 0), pre_p, post_p,
				simple_test_f, fallback);

	case MODIFY_EXPR:
	case INIT_EXPR:
	  simplify_modify_expr (expr_p, pre_p, post_p);
	  break;

	case TRUTH_ANDIF_EXPR:
	case TRUTH_ORIF_EXPR:
	  simplify_boolean_expr (expr_p, pre_p);
	  break;

	case TRUTH_NOT_EXPR:
	  tmp = TREE_OPERAND (*expr_p, 0);
	  simplify_expr (&tmp, pre_p, post_p, is_simple_id, fb_rvalue);
	  *expr_p = build (EQ_EXPR, TREE_TYPE (*expr_p), tmp, integer_zero_node);
	  break;

	case ADDR_EXPR:
	  simplify_addr_expr (expr_p, pre_p, post_p);
	  break;

	  /* va_arg expressions should also be left alone to avoid confusing the
	     vararg code.  FIXME: Is this really necessary?  */
	case VA_ARG_EXPR:
	  walk_tree (expr_p, mark_not_simple_r, NULL, NULL);
	  break;

	case NOP_EXPR:
	  if (VOID_TYPE_P (TREE_TYPE (*expr_p)))
	    {
	      /* Just strip a conversion to void and try again.  */
	      *expr_p = TREE_OPERAND (*expr_p, 0);
	      break;
	    }
	case CONVERT_EXPR:
	case FIX_TRUNC_EXPR:
	case FIX_CEIL_EXPR:
	case FIX_FLOOR_EXPR:
	case FIX_ROUND_EXPR:
	  simplify_expr (&TREE_OPERAND (*expr_p, 0), pre_p, post_p,
			 is_simple_varname, fb_rvalue);
	  break;

	case INDIRECT_REF:
	  simplify_expr (&TREE_OPERAND (*expr_p, 0), pre_p, post_p, is_simple_id,
			 fb_rvalue);
	  break;

	case NEGATE_EXPR:
	  simplify_expr (&TREE_OPERAND (*expr_p, 0), pre_p, post_p, is_simple_val,
			 fb_rvalue);
	  break;

	  /* Constants need not be simplified.  */
	case INTEGER_CST:
	case REAL_CST:
	case STRING_CST:
	case COMPLEX_CST:
	  break;

	case CONSTRUCTOR:
	  simplify_constructor (*expr_p, pre_p, post_p);
	  break;

	  /* The following are special cases that are not handled by the original
	     SIMPLE grammar.  */

	  /* SAVE_EXPR nodes are converted into a SIMPLE identifier and
	     eliminated.  */
	case SAVE_EXPR:
	  simplify_save_expr (expr_p, pre_p);
	  break;

	case EXPR_WITH_FILE_LOCATION:
	  simplify_expr_wfl (expr_p, pre_p, post_p, simple_test_f);
	  break;

	  /* FIXME: This breaks stage2.  I still haven't figured out why.  When
	     fixing remember to undo a similar change in
	     is_simple_unary_expr.  */
	case BIT_FIELD_REF:
#if 0
	  simplify_expr (&TREE_OPERAND (*expr_p, 0), pre_p, post_p, is_simple_id);
	  simplify_expr (&TREE_OPERAND (*expr_p, 1), pre_p, post_p, is_simple_val);
	  simplify_expr (&TREE_OPERAND (*expr_p, 2), pre_p, post_p, is_simple_val);
#else
	  walk_tree (expr_p, mark_not_simple_r, NULL, NULL);
#endif
	  break;

	case NON_LVALUE_EXPR:
	  simplify_expr (&TREE_OPERAND (*expr_p, 0), pre_p, post_p, simple_test_f,
			 fb_rvalue);
	  break;

	  /* If *EXPR_P does not need to be special-cased, handle it according to
	     its class.  */
	default:
	  {
	    if (TREE_CODE_CLASS (TREE_CODE (*expr_p)) == '1')
	      {
		simplify_expr (&TREE_OPERAND (*expr_p, 0), pre_p, post_p,
			       is_simple_val, fb_rvalue);
	      }
	    else if (TREE_CODE_CLASS (TREE_CODE (*expr_p)) == '2'
		     || TREE_CODE_CLASS (TREE_CODE (*expr_p)) == '<'
		     || TREE_CODE (*expr_p) == TRUTH_AND_EXPR
		     || TREE_CODE (*expr_p) == TRUTH_OR_EXPR
		     || TREE_CODE (*expr_p) == TRUTH_XOR_EXPR)
	      {
		simplify_expr (&TREE_OPERAND (*expr_p, 0), pre_p, post_p,
			       is_simple_val, fb_rvalue);

		simplify_expr (&TREE_OPERAND (*expr_p, 1), pre_p, post_p,
			       is_simple_val, fb_rvalue);
	      }
	  }
	}
    }
  /* If we replaced *expr_p, simplify again.  */
  while (*expr_p != save_expr);

  /* If we are simplifying at the statement level, we're done.  Tack
     everything together and replace the original statement with the
     simplified form.  */
  if (is_statement)
    {
#if 0
      /* Soon.  */
      add_tree (*expr_p, pre_p);
      add_tree (internal_post, pre_p);
      *expr_p = rationalize_compound_expr (internal_pre);
#endif
      return 1;
    }

  /* Otherwise we're simplifying a subexpression, so the resulting value is
     interesting.  */

  /* If it's sufficiently simple already, we're done.  Unless we are
     handling some post-effects internally; if that's the case, we need to
     copy into a temp before adding the post-effects to the tree.  */
  if (!internal_post && (*simple_test_f) (*expr_p))
    return 1;

  /* Otherwise, we need to create a new temporary for the simplified
     expression.  */

  /* We can't return an lvalue if we have an internal postqueue.  The
     object the lvalue refers to would (probably) be modified by the
     postqueue; we need to copy the value out first, which means an
     rvalue.  */
  if ((fallback & fb_lvalue) && !internal_post && is_simple_varname (*expr_p))
    {
      /* An lvalue will do.  Take the address of the expression, store it
	 in a temporary, and replace the expression with an INDIRECT_REF of
	 that temporary.  */
      tmp = build_addr_expr (*expr_p);
      simplify_expr (&tmp, pre_p, post_p, is_simple_id, fb_rvalue);
      *expr_p = build_indirect_ref (tmp, "");
    }
  else if ((fallback & fb_rvalue) && is_simple_rhs (*expr_p))
    {
#if defined ENABLE_CHECKING
      if (VOID_TYPE_P (TREE_TYPE (*expr_p)))
	abort ();
#endif

      /* An rvalue will do.  Assign the simplified expression into a new
	 temporary TMP and replace the original expression with TMP.  */
      *expr_p = get_initialized_tmp_var (*expr_p, pre_p);
    }
  else
    {
      fprintf (stderr, "simplification failed:\n");
      debug_c_tree (*expr_p);
      debug_tree (*expr_p);
      abort ();
    }

#if defined ENABLE_CHECKING
  /* Make sure the temporary matches our predicate.  */
  if (!(*simple_test_f) (*expr_p))
    abort ();
#endif

  if (internal_post)
    add_tree (internal_post, pre_p);

  return 1;
}

/* Do C-specific simplification.  Args are as for simplify_expr.  */

int
c_simplify_expr (expr_p, pre_p, post_p)
     tree *expr_p;
     tree *pre_p;
     tree *post_p;
{
  enum tree_code code = TREE_CODE (*expr_p);
  
  if (statement_code_p (code))
    {
      simplify_stmt (expr_p);
      return 1;
    }
  else switch (code)
    {
    case COMPOUND_LITERAL_EXPR:
      simplify_compound_literal_expr (expr_p, pre_p, post_p);
      return 1;

    case STMT_EXPR:
      simplify_stmt_expr (expr_p, pre_p);
      return 1;

    default:
      return 0;
    }
}

/*  Build an expression for the address of T.  Folds away INDIRECT_REF to
    avoid confusing the simplify process.  */

static tree
build_addr_expr (t)
     tree t;
{
  tree ptrtype = build_pointer_type (TREE_TYPE (t));
  if (TREE_CODE (t) == INDIRECT_REF)
    {
      t = TREE_OPERAND (t, 0);
      if (TREE_TYPE (t) != ptrtype)
	t = build1 (NOP_EXPR, ptrtype, t);
    }
  else
    t = build1 (ADDR_EXPR, ptrtype, t);

  return t;
}

  
/*  Re-write the ARRAY_REF node pointed by EXPR_P.

    PRE_P points to the list where side effects that must happen before
	*EXPR_P should be stored.

    POST_P points to the list where side effects that must happen after
	*EXPR_P should be stored.

    FIXME: ARRAY_REF currently doesn't accept a pointer as the array
    argument, so this simplification uses an INDIRECT_REF of ARRAY_TYPE.
    ARRAY_REF should be extended.  */

static void
simplify_array_ref (expr_p, pre_p, post_p)
     tree *expr_p;
     tree *pre_p;
     tree *post_p;
{
#if 1
  /* Handle array and member refs together for now.  When alias analysis
     improves, we may want to go back to handling them separately.  */
  simplify_compound_lval (expr_p, pre_p, post_p);
#else
  tree *p;
  varray_type dim_stack;

#if defined ENABLE_CHECKING
  if (TREE_CODE (*expr_p) != ARRAY_REF)
    abort ();
#endif

  VARRAY_GENERIC_PTR_INIT (dim_stack, 10, "dim_stack");

  /* Create a stack with all the dimensions of the array so that they can
     be simplified from left to right.  */
  for (p = expr_p; TREE_CODE (*p) == ARRAY_REF; p = &TREE_OPERAND (*p, 0))
    VARRAY_PUSH_GENERIC_PTR (dim_stack, (PTR) &TREE_OPERAND (*p, 1));

  /* After the loop above, 'p' points to the first non-ARRAY_REF,
     and 'dim_stack' is a stack of pointers to all the dimensions in left
     to right order (the leftmost dimension is at the top of the stack).

     Simplify the base, and then each of the dimensions from left to
     right.  */

  simplify_expr (p, pre_p, post_p, is_simple_min_lval, fb_lvalue);

  for (; VARRAY_ACTIVE_SIZE (dim_stack) > 0; VARRAY_POP (dim_stack))
    {
      tree *dim_p = (tree *)VARRAY_TOP_GENERIC_PTR (dim_stack);
      simplify_expr (dim_p, pre_p, post_p, is_simple_val, fb_rvalue);
    }
#endif
}

/* Simplify the COMPONENT_REF or ARRAY_REF node pointed by EXPR_P.

   PRE_P points to the list where side effects that must happen before
     *EXPR_P should be stored.

   POST_P points to the list where side effects that must happen after
     *EXPR_P should be stored.  */

static void
simplify_compound_lval (expr_p, pre_p, post_p)
     tree *expr_p;
     tree *pre_p;
     tree *post_p;
{
  tree *p;
  enum tree_code code;
  varray_type dim_stack;

#if defined ENABLE_CHECKING
  if (TREE_CODE (*expr_p) != ARRAY_REF && TREE_CODE (*expr_p) != COMPONENT_REF)
    abort ();
#endif

  /* Create a stack with all the array dimensions so that they can be
     simplified from left to right (to match user expectations).  */
  VARRAY_GENERIC_PTR_INIT (dim_stack, 10, "dim_stack");

  for (p = expr_p;
       TREE_CODE (*p) == ARRAY_REF || TREE_CODE (*p) == COMPONENT_REF;
       p = &TREE_OPERAND (*p, 0))
    {
      code = TREE_CODE (*p);
      if (code == ARRAY_REF)
	VARRAY_PUSH_GENERIC_PTR (dim_stack, (PTR) &TREE_OPERAND (*p, 1));
    }

  /* Now 'p' points to the first bit that isn't an ARRAY_REF or
     COMPONENT_REF, 'code' is the TREE_CODE of the last bit that was, and
     'dim_stack' is a stack of pointers to all the dimensions in left to
     right order (the leftmost dimension is at the top of the stack).

     Simplify the base, and then each of the dimensions from left to
     right.  */
  simplify_expr (p, pre_p, post_p, is_simple_min_lval,
		 code == COMPONENT_REF ? fb_either : fb_lvalue);

  for (; VARRAY_ACTIVE_SIZE (dim_stack) > 0; VARRAY_POP (dim_stack))
    {
      tree *dim_p = (tree *)VARRAY_TOP_GENERIC_PTR (dim_stack);
      simplify_expr (dim_p, pre_p, post_p, is_simple_val, fb_rvalue);
    }
}

/*  Simplify the self modifying expression pointed by EXPR_P (++, --, +=, -=).

    PRE_P points to the list where side effects that must happen before
	*EXPR_P should be stored.

    POST_P points to the list where side effects that must happen after
        *EXPR_P should be stored.  */

static void
simplify_self_mod_expr (expr_p, pre_p, post_p)
     tree *expr_p;
     tree *pre_p;
     tree *post_p;
{
  enum tree_code code;
  tree lhs, lvalue, rhs, t1;

  code = TREE_CODE (*expr_p);

#if defined ENABLE_CHECKING
  if (code != POSTINCREMENT_EXPR
      && code != POSTDECREMENT_EXPR
      && code != PREINCREMENT_EXPR
      && code != PREDECREMENT_EXPR)
    abort ();
#endif

  /* Simplify the LHS into a SIMPLE lvalue.  */
  lvalue = TREE_OPERAND (*expr_p, 0);
  simplify_expr (&lvalue, pre_p, post_p, is_simple_modify_expr_lhs,
		 fb_lvalue);

  /* Extract the operands to the arithmetic operation, including an rvalue
     version of our LHS.  */
  lhs = lvalue;
  /* Unshare it.  */
  walk_tree (&lhs, mostly_copy_tree_r, NULL, NULL);
  /* And reduce it to an ID.  */
  simplify_expr (&lhs, pre_p, post_p, is_simple_id, fb_rvalue);
  rhs = TREE_OPERAND (*expr_p, 1);
  simplify_expr (&rhs, pre_p, post_p, is_simple_val, fb_rvalue);

  /* Determine whether we need to create a PLUS or a MINUS operation.  */
  if (code == PREINCREMENT_EXPR || code == POSTINCREMENT_EXPR)
    t1 = build (PLUS_EXPR, TREE_TYPE (*expr_p), lhs, rhs);
  else
    t1 = build (MINUS_EXPR, TREE_TYPE (*expr_p), lhs, rhs);

#if defined ENABLE_CHECKING
  if (!is_simple_binary_expr (t1))
    abort ();
#endif

  /* Determine whether the new assignment should go before or after
     the simplified expression.  */
  if (code == PREINCREMENT_EXPR || code == PREDECREMENT_EXPR)
    add_tree (build_modify_expr (lvalue, NOP_EXPR, t1), pre_p);
  else
    add_tree (build_modify_expr (lvalue, NOP_EXPR, t1), post_p);

  /* Replace the original expression with the LHS of the assignment.  */
  *expr_p = lvalue;
}


/*  Simplify the COMPONENT_REF node pointed by EXPR_P.

    PRE_P points to the list where side effects that must happen before
	*EXPR_P should be stored.

    POST_P points to the list where side effects that must happen after
        *EXPR_P should be stored.  */

static void
simplify_component_ref (expr_p, pre_p, post_p)
     tree *expr_p;
     tree *pre_p;
     tree *post_p;
{
#if 1
  /* Handle array and member refs together for now.  When alias analysis
     improves, we may want to go back to handling them separately.  */
  simplify_compound_lval (expr_p, pre_p, post_p);
#else
  tree *p;

#if defined ENABLE_CHECKING
  if (TREE_CODE (*expr_p) != COMPONENT_REF)
    abort ();
#endif

  for (p = expr_p; TREE_CODE (*p) == COMPONENT_REF; p = &TREE_OPERAND (*p, 0))
    if (! is_simple_id (TREE_OPERAND (*p, 1)))
      /* The RHS of a COMPONENT_REF should always be a FIELD_DECL.  */
      abort ();

  /* Now we're down to the first bit that isn't a COMPONENT_REF.  */
  simplify_expr (p, pre_p, post_p, is_simple_min_lval, fb_either);
#endif
}


/*  Simplify the CALL_EXPR node pointed by EXPR_P.

    PRE_P points to the list where side effects that must happen before
	*EXPR_P should be stored.

    POST_P points to the list where side effects that must happen after
    	*EXPR_P should be stored.  */

static void
simplify_call_expr (expr_p, pre_p, post_p)
     tree *expr_p;
     tree *pre_p;
     tree *post_p;
{
#if defined ENABLE_CHECKING
  if (TREE_CODE (*expr_p) != CALL_EXPR)
    abort ();
#endif

  /* Some builtins cannot be simplified because they require specific
     arguments (e.g., __builtin_stdarg_start).

     FIXME: We should not need to do this!  Fix builtins so that they can
	    be simplified.  The question mark are MD builtins.  */
  if (!is_simplifiable_builtin (*expr_p))
    {
      /* Mark the whole expression not simplifiable.  */
      walk_tree (expr_p, mark_not_simple_r, NULL, NULL);
      return;
    }

  simplify_expr (&TREE_OPERAND (*expr_p, 0), pre_p, post_p, is_simple_id,
                 fb_rvalue);
  simplify_expr (&TREE_OPERAND (*expr_p, 1), pre_p, post_p, is_simple_arglist,
                 fb_rvalue);
}

/*  Simplify the TREE_LIST node pointed by EXPR_P.

    PRE_P points to the list where side effects that must happen before
	*EXPR_P should be stored.

    POST_P points to the list where side effects that must happen after
        *EXPR_P should be stored.  */

static void
simplify_tree_list (expr_p, pre_p, post_p)
     tree *expr_p;
     tree *pre_p;
     tree *post_p;
{
  tree op;

#if defined ENABLE_CHECKING
  if (TREE_CODE (*expr_p) != TREE_LIST)
    abort ();
#endif

  for (op = *expr_p; op; op = TREE_CHAIN (op))
    simplify_expr (&TREE_VALUE (op), pre_p, post_p, is_simple_val,
		   fb_rvalue);
}


/*  Convert the conditional expression pointed by EXPR_P '(p) ? a : b;'
    into

    if (p)			if (p)
      t1 = a;			  a;
    else		or	else
      t1 = b;			  b;
    t1;				(void)0;

    The second form is used when *EXPR_P is of type void.

    PRE_P points to the list where side effects that must happen before
	*EXPR_P should be stored.

    POST_P points to the list where side effects that must happen after
        *EXPR_P should be stored.  */

static void
simplify_cond_expr (expr_p, pre_p)
     tree *expr_p;
     tree *pre_p;
{
  tree t_then, t_else, tmp, pred, tval, fval, new_if, expr_type;

#if defined ENABLE_CHECKING
  if (TREE_CODE (*expr_p) != COND_EXPR)
    abort ();
#endif

  expr_type = TREE_TYPE (*expr_p);

  if (!VOID_TYPE_P (expr_type))
    tmp = create_tmp_var (TREE_TYPE (*expr_p), "iftmp");
  else
    tmp = void_zero_node;

  pred = TREE_OPERAND (*expr_p, 0);
  tval = TREE_OPERAND (*expr_p, 1);
  fval = TREE_OPERAND (*expr_p, 2);

  /* Build the THEN_CLAUSE 't1 = a;' or 'a;'.  */
  if (!VOID_TYPE_P (expr_type))
    t_then = build_stmt (EXPR_STMT, build_modify_expr (tmp, NOP_EXPR, tval));
  else
    t_then = build_stmt (EXPR_STMT, tval);
  tree_build_scope (&t_then);

  /* Build the ELSE_CLAUSE 't1 = b;' or 'b;'.  */
  if (!VOID_TYPE_P (expr_type))
    t_else = build_stmt (EXPR_STMT, build_modify_expr (tmp, NOP_EXPR, fval));
  else
    t_else = build_stmt (EXPR_STMT, fval);
  tree_build_scope (&t_else);

  /* Build a new IF_STMT, simplify it and insert it in the PRE_P chain.  */
  new_if = build_stmt (IF_STMT, pred, t_then, t_else);
  simplify_if_stmt (new_if, pre_p);
  add_tree (new_if, pre_p);

  /* Replace the original expression with the new temporary.  */
  *expr_p = tmp;
}


/*  Simplify the MODIFY_EXPR node pointed by EXPR_P.

    PRE_P points to the list where side effects that must happen before
	*EXPR_P should be stored.

    POST_P points to the list where side effects that must happen after
        *EXPR_P should be stored.  */

static void
simplify_modify_expr (expr_p, pre_p, post_p)
     tree *expr_p;
     tree *pre_p;
     tree *post_p;
{
#if defined ENABLE_CHECKING
  if (TREE_CODE (*expr_p) != MODIFY_EXPR
      && TREE_CODE (*expr_p) != INIT_EXPR)
    abort ();
#endif

  simplify_expr (&TREE_OPERAND (*expr_p, 0), pre_p, post_p,
		 is_simple_modify_expr_lhs, fb_lvalue);
  simplify_expr (&TREE_OPERAND (*expr_p, 1), pre_p, post_p, is_simple_rhs,
                 fb_rvalue);

  add_tree (*expr_p, pre_p);
  *expr_p = TREE_OPERAND (*expr_p, 0);
}


/*  Simplify TRUTH_ANDIF_EXPR and TRUTH_ORIF_EXPR expressions.  EXPR_P
    points to the expression to simplify.

    Expressions of the form 'a && b' are simplified to:

    	T = a;
	if (T)
	  T = b;

    Expressions of the form 'a || b' are simplified to:

	T = a;
	if (!T)
	  T = b;

    In both cases, the expression is re-written as 'T != 0'.

    PRE_P points to the list where side effects that must happen before
	*EXPR_P should be stored.

    POST_P points to the list where side effects that must happen after
        *EXPR_P should be stored.  */

static void
simplify_boolean_expr (expr_p, pre_p)
     tree *expr_p;
     tree *pre_p;
{
  enum tree_code code;
  tree t, lhs, rhs, if_body, if_cond, if_stmt;

  code = TREE_CODE (*expr_p);

#if defined ENABLE_CHECKING
  if (code != TRUTH_ANDIF_EXPR && code != TRUTH_ORIF_EXPR)
    abort ();
#endif

  /* First, make sure that our operands are truthvalues.  This should
     already be the case, but they may have the wrong type.  */
  lhs = (*lang_hooks.truthvalue_conversion) (TREE_OPERAND (*expr_p, 0));
  rhs = (*lang_hooks.truthvalue_conversion) (TREE_OPERAND (*expr_p, 1));

  /* Build 'T = a'  */
  t = get_initialized_tmp_var (lhs, pre_p);

  /* Build the body for the if() statement that conditionally evaluates the
     RHS of the expression.  Note that we first build the assignment
     surrounded by a new scope so that its simplified form is computed
     inside the new scope.  */
  if_body = build_stmt (EXPR_STMT, build_modify_expr (t, NOP_EXPR, rhs));
  tree_build_scope (&if_body);

  /* Build the statement 'if (T = a <comp> 0) T = b;'.  Where <comp> is
     NE_EXPR if we are processing && and EQ_EXPR if we are processing ||.

     Note that we are deliberately creating a non SIMPLE statement to
     explicitly expose the sequence points to the simplifier.  When the
     resulting if() statement is simplified, the side effects for the LHS
     of 'a && b' will be inserted before the evaluation of 'b'.  */
  if (code == TRUTH_ANDIF_EXPR)
    if_cond = t;
  else
    if_cond = build (EQ_EXPR, TREE_TYPE (t), t, integer_zero_node);

  if_stmt = build_stmt (IF_STMT, if_cond, if_body, NULL_TREE);

  /* Simplify the IF_STMT and insert it in the PRE_P chain.  */
  simplify_if_stmt (if_stmt, pre_p);
  add_tree (if_stmt, pre_p);

  /* If we're not actually looking for a boolean result, convert now.  */
  if (TREE_TYPE (t) != TREE_TYPE (*expr_p))
    {
      t = convert (TREE_TYPE (*expr_p), t);
      simplify_expr (&t, pre_p, NULL, is_simple_id, fb_rvalue);
    }

  /* Re-write the original expression to use T.  */
  *expr_p = t;
}


/*  Simplifies an expression sequence.  This function simplifies each
    expression and re-writes the original expression with the last
    expression of the sequence in SIMPLE form.

    PRE_P points to the list where the side effects for all the expressions
	in the sequence will be emitted.

    POST_P points to the list where the post side effects for the last
        expression in the sequence are emitted.  */

static void
simplify_compound_expr (expr_p, pre_p, post_p)
     tree *expr_p;
     tree *pre_p;
     tree *post_p;
{
  tree *expr_s, *pre_expr_s, *post_expr_s;
  tree t, ret;
  int i, num;

#if defined ENABLE_CHECKING
  if (TREE_CODE (*expr_p) != COMPOUND_EXPR)
    abort ();
#endif

  /* Count all the expressions in the sequence.  */
  num = 2;
  t = *expr_p;
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

  t = *expr_p;
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
      simplify_expr (&expr_s[i], &pre_expr_s[i], &post_expr_s[i],
	             is_simple_expr, fb_rvalue);

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

  *expr_p = ret;
}


/*  Simplify an EXPR_WITH_FILE_LOCATION.  EXPR_P points to the expression
    to simplify.

    After simplification, all the nodes in PRE_P and POST_P are wrapped inside
    a EXPR_WITH_FILE_LOCATION node to preserve the original semantics.  The
    simplified expression is also returned inside an EXPR_WITH_FILE_LOCATION
    node.

    PRE_P points to the list where side effects that must happen before
	*EXPR_P should be stored.

    POST_P points to the list where side effects that must happen after
    	*EXPR_P should be stored.

    SIMPLE_TEST_F points to a function that takes a tree T and
	returns nonzero if T is in the SIMPLE form requested by the
	caller.  */

static void
simplify_expr_wfl (expr_p, pre_p, post_p, simple_test_f)
     tree *expr_p;
     tree *pre_p;
     tree *post_p;
     int (*simple_test_f) PARAMS ((tree));
{
  tree op;
  const char *file;
  int line, col;
  tree pre = NULL_TREE;
  tree post = NULL_TREE;
  int saw_other;
  
#if defined ENABLE_CHECKING
  if (TREE_CODE (*expr_p) != EXPR_WITH_FILE_LOCATION)
    abort ();
#endif

  file = input_filename;
  input_filename = EXPR_WFL_FILENAME (*expr_p);

  line = lineno;
  lineno = EXPR_WFL_LINENO (*expr_p);

  col = EXPR_WFL_COLNO (*expr_p);

  simplify_expr (&EXPR_WFL_NODE (*expr_p), &pre, &post, simple_test_f,
		 fb_rvalue);

  saw_other = 0;
  for (op = pre; op; op = TREE_CHAIN (op))
    {
      if (TREE_CODE (op) == EXPR_STMT)
	EXPR_STMT_EXPR (op)
	  = build_expr_wfl (EXPR_STMT_EXPR (op), file, line, col);
      else
	saw_other = 1;
    }

  for (op = post; op; op = TREE_CHAIN (op))
    EXPR_STMT_EXPR (op)
      = build_expr_wfl (EXPR_STMT_EXPR (op), file, line, col);

  if (saw_other && strcmp (file, input_filename) != 0)
    {
      /* There are non-EXPR_STMT statements, so we need to wrap them in
	 FILE_STMTs.  */
      /* FIXME this causes some stuff from the expression containing an
	 inline call to be grouped with the code from the inline
	 function.  */

      tree fstmt = build_stmt (FILE_STMT, get_identifier (input_filename));
      /* Use pre_p here so it precedes the contents of our local 'pre'.  */
      add_tree (fstmt, pre_p);
  
      fstmt = build_stmt (FILE_STMT, get_identifier (file));
      add_tree (fstmt, &post);
    }

  lineno = line;
  input_filename = file;

  add_tree (pre, pre_p);
  add_tree (post, post_p);
}

/*  Simplify a SAVE_EXPR node.  EXPR_P points to the expression to
    simplify.  After simplification, EXPR_P will point to a new temporary
    that holds the original value of the SAVE_EXPR node.

    PRE_P points to the list where side effects that must happen before
        *EXPR_P should be stored.  */

static void
simplify_save_expr (expr_p, pre_p)
     tree *expr_p;
     tree *pre_p;
{
#if defined ENABLE_CHECKING
  if (TREE_CODE (*expr_p) != SAVE_EXPR)
    abort ();
#endif

  /* If the operand is already a SIMPLE temporary, just re-write the
     SAVE_EXPR node.  */
  if (is_simple_tmp_var (TREE_OPERAND (*expr_p, 0)))
    *expr_p = TREE_OPERAND (*expr_p, 0);
  else
    {
      TREE_OPERAND (*expr_p, 0) =
	get_initialized_tmp_var (TREE_OPERAND (*expr_p, 0), pre_p);
      *expr_p = TREE_OPERAND (*expr_p, 0);
    }
}

/* Simplify a STMT_EXPR.  EXPR_P points to the expression to simplify.
    After simplification, if the STMT_EXPR returns a value, EXPR_P will
    point to a new temporary that holds that value; otherwise it will be
    null.

    PRE_P points to the list where side effects that must happen before
      *EXPR_P should be stored.  */

static void
simplify_stmt_expr (expr_p, pre_p)
     tree *expr_p;
     tree *pre_p;
{
  tree body = STMT_EXPR_STMT (*expr_p);

  if (VOID_TYPE_P (TREE_TYPE (*expr_p)))
    *expr_p = void_zero_node;
  else
    {
      tree substmt, last_expr_stmt, last_expr, temp, mod;

      last_expr_stmt = NULL_TREE;
      for (substmt = COMPOUND_BODY (body); substmt;
	   substmt = TREE_CHAIN (substmt))
	{
	  if (TREE_CODE (substmt) == EXPR_STMT)
	    last_expr_stmt = substmt;
	}

#if defined ENABLE_CHECKING
      if (!last_expr_stmt)
	abort ();
#endif

      last_expr = EXPR_STMT_EXPR (last_expr_stmt);

      if (TREE_CHAIN (last_expr_stmt) == NULL_TREE
	  && TREE_CODE (last_expr) == INIT_EXPR)
	{
	  /* The C++ frontend already did this for us.  */;
	  temp = TREE_OPERAND (last_expr, 0);
	}
      else
	{
#if defined ENABLE_CHECKING
	  if (!is_last_stmt_of_scope (last_expr_stmt))
	    abort ();
#endif

	  temp = create_tmp_var (TREE_TYPE (last_expr), "retval");
	  mod = build (INIT_EXPR, TREE_TYPE (temp), temp, last_expr);
	  EXPR_STMT_EXPR (last_expr_stmt) = mod;
	}

      *expr_p = temp;
    }

  simplify_stmt (&body);
  add_tree (body, pre_p);
}

/*  Re-write the ADDR_EXPR node pointed by EXPR_P

    PRE_P points to the list where side effects that must happen before
	*EXPR_P should be stored.

    POST_P points to the list where side effects that must happen after
	*EXPR_P should be stored.  */

static void
simplify_addr_expr (expr_p, pre_p, post_p)
     tree *expr_p;
     tree *pre_p;
     tree *post_p;
{
  /* Check if we are dealing with an expression of the form '&*ptr'.
     While the front end folds away '&*ptr' into 'ptr', these
     expressions may be generated internally by the compiler (e.g.,
     builtins like __builtin_va_end).  */
  if (TREE_CODE (TREE_OPERAND (*expr_p, 0)) != INDIRECT_REF)
    simplify_expr (&TREE_OPERAND (*expr_p, 0), pre_p, post_p, 
		   is_simple_addr_expr_arg, fb_lvalue);
  else
    /* Fold &*EXPR into EXPR.  simplify_expr will re-simplify EXPR.  */
    *expr_p = TREE_OPERAND (TREE_OPERAND (*expr_p, 0), 0);
}


/* Code generation.  */

/*  Replaces T; by a COMPOUND_STMT containing {T;}.  */

void
tree_build_scope (t)
     tree *t;
{
  tree comp_stmt, start_scope, end_scope;

  /* If T already has a proper scope, do nothing.  */
  if (*t
      && TREE_CODE (*t) == COMPOUND_STMT
      && COMPOUND_BODY (*t))
    return;

  /* Create a new empty scope.  */
  comp_stmt = make_node (COMPOUND_STMT);

  start_scope = make_node (SCOPE_STMT);
  SCOPE_BEGIN_P (start_scope) = 1;

  end_scope = make_node (SCOPE_STMT);
  SCOPE_BEGIN_P (end_scope) = 0;

  COMPOUND_BODY (comp_stmt) = start_scope;

  if (*t)
    {
      /* If T is not empty, insert it inside the newly created scope.  Note
	 that we can't just join TREE_CHAIN(*T) to the closing scope
	 because even if T wasn't inside a scope, it might be a list of
	 statements.  */
      TREE_CHAIN (start_scope) = *t;
      chainon (*t, end_scope);
    }
  else
    {
      /* T is empty.  Simply join the start/end nodes.  */
      TREE_CHAIN (start_scope) = end_scope;
    }

  /* Set T to the newly constructed scope.  */
  *t = comp_stmt;
}


/*  Add T to the list container pointed by LIST_P.  If T is a TREE_LIST
    node, it is linked-in directly.  If T is an expression with no effects,
    it is ignored.
    
    Return the newly added list node or NULL_TREE if T was not added to
    LIST_P.  */

tree
add_tree (t, list_p)
    tree t;
    tree *list_p;
{
  if (t == NULL_TREE)
    return NULL_TREE;

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
      t = build_stmt (EXPR_STMT, t);
    }

  *list_p = chainon (*list_p, t);

  return t;
}


/*  Insert the REEVAL list before CONTINUE_STMTs and at the end of the loop
    body BODY.  Set the line number of the REEVAL list to LINE.  */

void
insert_before_continue_end (reeval, body)
     tree reeval;
     tree body;
{
  tree last, beforelast;

  if (reeval == NULL_TREE)
    return;

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


/*  Insert the statement list REEVAL before each CONTINUE_STMT in the block
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


/* Miscellaneous helpers.  */

/*  Create a new temporary variable declaration of type TYPE.  Returns the
    newly created decl and pushes it into the current binding.  */

tree
create_tmp_var (type, prefix)
     tree type;
     const char *prefix;
{
  static unsigned int id_num = 1;
  char *tmp_name;
  char *preftmp = NULL;
  tree tmp_var;

  if (prefix)
    {
      preftmp = ASTRDUP (prefix);
      remove_suffix (preftmp, strlen (preftmp));
      prefix = preftmp;
    }
  
  ASM_FORMAT_PRIVATE_NAME (tmp_name, (prefix ? prefix : "T"), id_num++);

#if defined ENABLE_CHECKING
  /* If the type is an array, something is wrong.  */
  if (TREE_CODE (type) == ARRAY_TYPE)
    abort ();
#endif

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

/* Create a new temporary alias variable declaration of type TYPE.  Returns
   the newly created decl.  Does NOT push it into the current binding.  */

tree
create_tmp_alias_var (type, prefix)
     tree type;
     const char *prefix;
{
  static unsigned int id_num = 1;
  char *tmp_name;
  char *preftmp = NULL;
  tree tmp_var;

  if (prefix)
    {
      preftmp = ASTRDUP (prefix);
      remove_suffix (preftmp, strlen (preftmp));
      prefix = preftmp;
    }
  
  ASM_FORMAT_PRIVATE_NAME (tmp_name, (prefix ? prefix : "T"), id_num++);

#if defined ENABLE_CHECKING
  /* If the type is an array, something is wrong.  */
  if (TREE_CODE (type) == ARRAY_TYPE)
    abort ();
#endif

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


  return tmp_var;
}

/*  Given a tree, try to return a useful variable name that we can use
    to prefix a temporary that is being assigned the value of the tree. 
    I.E. given  <temp> = &A, return A.  */
static const char *
get_name (t)
     tree t;
{
  tree stripped_decl;

  stripped_decl = t;
  STRIP_NOPS (stripped_decl);
  if (DECL_P (stripped_decl) && DECL_NAME (stripped_decl))
    return IDENTIFIER_POINTER (DECL_NAME (stripped_decl));
  else
    {
      switch (TREE_CODE (stripped_decl))
	{
	case ADDR_EXPR:
	  return get_name (TREE_OPERAND (stripped_decl, 0));
	  break;
	default:
	  return NULL;
	}
    }
}

/*  Returns a new temporary variable, initialized with VAL.  PRE_P and STMT
    are as in simplify_expr.  */

tree
get_initialized_tmp_var (val, pre_p)
     tree val;
     tree *pre_p;
{
  tree t, mod;
  const char *prefix = NULL;
  
  prefix = get_name (val);
  simplify_expr (&val, pre_p, NULL, is_simple_rhs, fb_rvalue);
  t = create_tmp_var (TREE_TYPE (val), prefix);
  mod = build (INIT_EXPR, TREE_TYPE (t), t, val);
  add_tree (mod, pre_p);

  return t;
}

/*  Returns true if T is a SIMPLE temporary variable, false otherwise.  */

bool
is_simple_tmp_var (t)
     tree t;
{
  /* FIXME this could trigger for other local artificials, too.  */
  return (TREE_CODE (t) == VAR_DECL && DECL_ARTIFICIAL (t)
	  && !TREE_STATIC (t) && !DECL_EXTERNAL (t));
}

/*  Change the flags for the type of the node T to make it writable.  */

static void 
make_type_writable (t)
     tree t;
{
#if defined ENABLE_CHECKING
  if (t == NULL_TREE)
    abort ();
#endif

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


/*  Declares all the variables in VARS in SCOPE.  Returns the last
    DECL_STMT emitted.  */

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


/*  Returns the last DECL_STMT in the scope SCOPE.  */

static tree
tree_last_decl (scope)
     tree scope;
{
  tree last;

  /* Be sure that we get a scope.  Ignore FILE_STMT nodes.  */
  while (TREE_CODE (scope) == FILE_STMT)
    scope = TREE_CHAIN (scope);

#if defined ENABLE_CHECKING
  /* In C99 mode, we can find DECL_STMT nodes before the body of the
     function.  In that case, we declare all the temporaries there.  */
  if (TREE_CODE (scope) != DECL_STMT
      && !SCOPE_BEGIN_P (scope))
    abort ();
#endif

  /* Find the last declaration statement in the scope.  */
  last = scope;
  while (TREE_CHAIN (last) && TREE_CODE (TREE_CHAIN (last)) == DECL_STMT)
    last = TREE_CHAIN (last);

  return last;
}


/*  Copy every statement from the chain CHAIN by calling deep_copy_node().
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


/*  Create a deep copy of NODE.  The only nodes that are not deep copied
    are declarations, constants and types.  */

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
      res = build_stmt (COMPOUND_STMT, deep_copy_list (COMPOUND_BODY (node)));
      break;

    case FOR_STMT:
      res = build_stmt (FOR_STMT, 
			deep_copy_node (FOR_INIT_STMT (node)),
			deep_copy_node (FOR_COND (node)),
			deep_copy_node (FOR_EXPR (node)),
			deep_copy_node (FOR_BODY (node)));
      break;

    case WHILE_STMT:
      res = build_stmt (WHILE_STMT, 
			deep_copy_node (WHILE_COND (node)),
			deep_copy_node (WHILE_BODY (node)));
      break;

    case DO_STMT:
      res = build_stmt (DO_STMT, 
			deep_copy_node (DO_COND (node)),
			deep_copy_node (DO_BODY (node)));
      break;

    case IF_STMT:
      res = build_stmt (IF_STMT, 
			deep_copy_node (IF_COND (node)),
			deep_copy_node (THEN_CLAUSE (node)),
			deep_copy_node (ELSE_CLAUSE (node)));
      break;

    case SWITCH_STMT:
      res = build_stmt (SWITCH_STMT,
			deep_copy_node (SWITCH_COND (node)),
			deep_copy_node (SWITCH_BODY (node)));
      break;

    case EXPR_STMT:
      res = build_stmt (EXPR_STMT, deep_copy_node (EXPR_STMT_EXPR (node)));
      break;

    case DECL_STMT:
      res = build_stmt (DECL_STMT, DECL_STMT_DECL (node));
      break;

    case RETURN_STMT:
      res = build_stmt (RETURN_STMT, deep_copy_node (RETURN_STMT_EXPR (node)));
      break;

    case TREE_LIST:
      res = build_tree_list (deep_copy_node (TREE_PURPOSE (node)),
	                     deep_copy_node (TREE_VALUE (node)));
      break;

    case SCOPE_STMT:
      if (SCOPE_BEGIN_P (node))
	{
	  /* ??? The sub-blocks and supercontext for the scope's BLOCK_VARS
		 should be re-computed after copying.  */
	  res = build_stmt (SCOPE_STMT,
			    deep_copy_list (SCOPE_STMT_BLOCK (node)));
	  SCOPE_BEGIN_P (res) = 1;
	}
      else 
	{
	  res = build_stmt (SCOPE_STMT, NULL_TREE);
	  SCOPE_BEGIN_P (res) = 0;
	}
      break;

    default:
      walk_tree (&node, copy_tree_r, NULL, NULL);
      res = node;
      break;
    }
  
  /* Set the line number.  */
  if (statement_code_p (TREE_CODE (node)))
    STMT_LINENO (res) = STMT_LINENO (node);
  
  return res;
}

/*  Return nonzero if STMT has some effect (i.e., if it's not of the form
    'a;' where a is a non-volatile variable).  */
    
static int
stmt_has_effect (stmt)
     tree stmt;
{
  if (TREE_CODE (stmt) != EXPR_STMT)
    return 1;
  else if (expr_has_effect (EXPR_STMT_EXPR (stmt)))
    return 1;

  return 0;
}


/*  Return nonzero if EXPR has some effect (e.g., it's not a single
    non-volatile VAR_DECL).  */

static int
expr_has_effect (expr)
     tree expr;
{
  return (TREE_SIDE_EFFECTS (expr)
	  || (TREE_CODE (expr) == CONVERT_EXPR
	      && VOID_TYPE_P (TREE_TYPE (expr))));
}


/* Similar to copy_tree_r() but do not copy SAVE_EXPR nodes.  These nodes
   model computations that should only be done once.  If we were to unshare
   something like SAVE_EXPR(i++), the simplification process would create
   wrong code.  */

static tree
mostly_copy_tree_r (tp, walk_subtrees, data)
     tree *tp;
     int *walk_subtrees;
     void *data;
{
  if (TREE_CODE (*tp) == SAVE_EXPR)
    *walk_subtrees = 0;
  else
    copy_tree_r (tp, walk_subtrees, data);

  return NULL_TREE;
}


/* Strip off a legitimate source ending from the input string NAME of
   length LEN.  Rather than having to know the names used by all of
   our front ends, we strip off an ending of a period followed by
   up to five characters.  (Java uses ".class".)  */

static inline void 
remove_suffix (name, len)
     char *name;
     int len;
{
  int i;

  for (i = 2;  i < 8 && len > i;  i++)
    {
      if (name[len - i] == '.')
	{
	  name[len - i] = '\0';
	  break;
	}
    }
}

/*  Return nonzero if STMT is the last statement of its scope.  */

static int
is_last_stmt_of_scope (stmt)
     tree stmt;
{
  return (TREE_CHAIN (stmt)
	  && TREE_CODE (TREE_CHAIN (stmt)) == SCOPE_STMT
	  && SCOPE_END_P (TREE_CHAIN (stmt)));
}

/* If the last entry in CHAIN is an expression, return it; otherwise,
   return NULL_TREE.

   If DECL_IS_BAD is nonzero, seeing a DECL_STMT causes us to bail out to
   avoid scoping problems.  */

static tree
tail_expression (chain, decl_is_bad)
     tree *chain;
     int decl_is_bad;
{
  if (*chain)
    {
      tree *first_expr = 0;

      for (; *chain; chain = &TREE_CHAIN (*chain))
	{
	  tree elt = *chain;
	  if (TREE_CODE (elt) == EXPR_STMT)
	    first_expr = chain;
	  else
	    {
	      if (decl_is_bad && (TREE_CODE (elt) == DECL_STMT))
		return NULL_TREE;
	      first_expr = 0;
	    }
	}

      if (first_expr)
	{
	  tree exprs = nreverse (*first_expr);
	  tree compexpr = EXPR_STMT_EXPR (exprs);
	  for (exprs = TREE_CHAIN (exprs); exprs; exprs = TREE_CHAIN (exprs))
	    compexpr = build (COMPOUND_EXPR, TREE_TYPE (compexpr),
			      EXPR_STMT_EXPR (exprs), compexpr);
	  *first_expr = 0;
	  return compexpr;
	}
    }
  return NULL_TREE;
}


/* Returns nonzero if STMT is a SIMPLE declaration, i.e. one with no
   initializer.

   This is not appropriate for static decls, so we leave them alone.  */

int
is_simple_decl_stmt (stmt)
     tree stmt;
{
  tree decl = DECL_STMT_DECL (stmt);
  tree init = DECL_INITIAL (decl);

  if (!is_simple_val (DECL_SIZE_UNIT (decl)))
    return 0;

  /* Plain decls are simple.  */
  if (init == NULL_TREE || init == error_mark_node)
    return 1;

  /* Don't mess with a compile-time initializer.  */
  if (TREE_STATIC (decl))
    return 1;

  return 0;
}


/* Callback for walk_tree.  Mark *TP and its sub-trees as not simplified.  */

static tree
mark_not_simple_r (tp, walk_subtrees, data)
     tree *tp;
     int *walk_subtrees;
     void *data ATTRIBUTE_UNUSED;
{
  switch (TREE_CODE_CLASS (TREE_CODE (*tp)))
    {
      case 'c':
      case 'd':
      case 't':
	/* Don't mark constants, declarations nor types.  */
	*walk_subtrees = 0;
	break;

      default:
	get_tree_ann (*tp)->flags |= TF_NOT_SIMPLE;
	break;
    }

  return NULL_TREE;
}

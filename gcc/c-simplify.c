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
#include "coretypes.h"
#include "tm.h"
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
#include "langhooks-def.h"
#include "flags.h"
#include "rtl.h"
#include "toplev.h"
#include "tree-dump.h"

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

void c_simplify_stmt          PARAMS ((tree *));
static void simplify_expr_stmt       PARAMS ((tree *));
static void simplify_decl_stmt       PARAMS ((tree *, tree *));
static void simplify_for_stmt        PARAMS ((tree *, tree *));
static void simplify_while_stmt      PARAMS ((tree *));
static void simplify_do_stmt         PARAMS ((tree *));
static void simplify_if_stmt         PARAMS ((tree *));
static void simplify_switch_stmt     PARAMS ((tree *));
static void simplify_return_stmt     PARAMS ((tree *));
static void simplify_stmt_expr       PARAMS ((tree *));
static void simplify_compound_literal_expr PARAMS ((tree *));
static void make_type_writable       PARAMS ((tree));
#if defined ENABLE_CHECKING
static int is_last_stmt_of_scope     PARAMS ((tree));
#endif
static void simplify_block	     PARAMS ((tree *, tree *));
static void simplify_cleanup	     PARAMS ((tree *, tree *));
static tree simplify_c_loop	     PARAMS ((tree, tree, tree, int));
static void push_context             PARAMS ((void));
static void pop_context              PARAMS ((void));
static tree c_build_bind_expr	     PARAMS ((tree, tree));
static void add_block_to_enclosing   PARAMS ((tree));
static tree mostly_copy_tree_r       PARAMS ((tree *, int *, void *));
static void simplify_condition		PARAMS ((tree *));

enum bc_t { bc_break = 0, bc_continue = 1 };
static tree begin_bc_block PARAMS ((enum bc_t));
static tree finish_bc_block PARAMS ((tree, tree));
static tree build_bc_goto PARAMS ((enum bc_t));

static struct c_simplify_ctx
{
  /* For handling break and continue.  */
  tree current_bc_label;
  tree bc_id[2];
} *ctxp;

static void
push_context ()
{
  if (ctxp)
    abort ();
  ctxp = (struct c_simplify_ctx *) xcalloc (1, sizeof (struct c_simplify_ctx));
  ctxp->bc_id[bc_continue] = get_identifier ("continue");
  ctxp->bc_id[bc_break] = get_identifier ("break");
}

static void
pop_context ()
{
  if (!ctxp || ctxp->current_bc_label)
    abort ();
  free (ctxp);
  ctxp = NULL;
}

/* Simplification of statement trees.  */

/* Convert the tree representation of FNDECL from C frontend trees to
   GENERIC.  */

void
c_genericize (fndecl)
     tree fndecl;
{
  FILE *dump_file;
  int dump_flags;

  /* Dump the C-specific tree IR.  */
  dump_file = dump_begin (TDI_original, &dump_flags);
  if (dump_file)
    {
      fprintf (dump_file, "\n;; Function %s",
	       (*lang_hooks.decl_printable_name) (fndecl, 2));
      fprintf (dump_file, " (%s)\n",
	       IDENTIFIER_POINTER (DECL_ASSEMBLER_NAME (fndecl)));
      fprintf (dump_file, ";; enabled by -%s\n", dump_flag_name (TDI_original));
      fprintf (dump_file, "\n");

      if (dump_flags & TDF_RAW)
	dump_node (DECL_SAVED_TREE (fndecl), TDF_SLIM | dump_flags, dump_file);
      else
	print_c_tree (dump_file, DECL_SAVED_TREE (fndecl));
      fprintf (dump_file, "\n");

      dump_end (TDI_original, dump_file);
    }

  /* Go ahead and simplify for now.  */
  push_context ();
  simplify_function_tree (fndecl);
  pop_context ();

  /* Dump the genericized tree IR.  */
  dump_function (TDI_generic, fndecl);
}

/*  Entry point for the tree lowering pass.  Recursively scan
    *STMT_P and convert it to a SIMPLE tree.  */

void 
c_simplify_stmt (stmt_p)
     tree *stmt_p;
{
  tree stmt, next;
  tree outer_pre = NULL_TREE;

  /* PRE and POST are tree chains that contain the side-effects of the
     simplified tree.  For instance, given the expression tree:

     		c = ++a * 3 + b++;

     After simplification, the tree will be re-written as:

     		a = a + 1;
		t1 = a * 3;	<-- PRE
     		c = t1 + b;
     		b = b + 1;	<-- POST  */

  for (stmt = *stmt_p; stmt && stmt != error_mark_node; stmt = next)
    {
      tree pre, post;
      int saved_stmts_are_full_exprs_p;
      const char *stmt_filename;
      int stmt_lineno;

      /* Set up context appropriately for handling this statement.  */
      saved_stmts_are_full_exprs_p = stmts_are_full_exprs_p ();
      prep_stmt (stmt);
      stmt_filename = input_filename;
      stmt_lineno = lineno;

      pre = NULL_TREE;
      post = NULL_TREE;

      next = TREE_CHAIN (stmt);

      switch (TREE_CODE (stmt))
	{
	case COMPOUND_STMT:
	  c_simplify_stmt (&COMPOUND_BODY (stmt));
	  stmt = COMPOUND_BODY (stmt);
	  break;

	case SCOPE_STMT:
	  simplify_block (&stmt, &next);
	  break;

	case FOR_STMT:
	  simplify_for_stmt (&stmt, &pre);
	  break;
	  
	case WHILE_STMT:
	  simplify_while_stmt (&stmt);
	  break;

	case DO_STMT:
	  simplify_do_stmt (&stmt);
	  break;

	case IF_STMT:
	  simplify_if_stmt (&stmt);
	  break;
	  
	case SWITCH_STMT:
	  simplify_switch_stmt (&stmt);
	  break;

	case EXPR_STMT:
	  simplify_expr_stmt (&stmt);
	  break;

	case RETURN_STMT:
	  simplify_return_stmt (&stmt);
	  break;

	case DECL_STMT:
	  simplify_decl_stmt (&stmt, &next);
	  break;

	case LABEL_STMT:
	  stmt = build1 (LABEL_EXPR, void_type_node, LABEL_STMT_LABEL (stmt));
	  break;

	case GOTO_STMT:
	  stmt = build1 (GOTO_EXPR, void_type_node, GOTO_DESTINATION (stmt));
	  break;

	case CASE_LABEL:
	  stmt = build (CASE_LABEL_EXPR, void_type_node,
			CASE_LOW (stmt), CASE_HIGH (stmt));
	  break;

	case CONTINUE_STMT:
	  stmt = build_bc_goto (bc_continue);
	  break;

	case BREAK_STMT:
	  stmt = build_bc_goto (bc_break);
	  break;

	case CLEANUP_STMT:
	  simplify_cleanup (&stmt, &next);
	  break;

	case ASM_STMT:
	  stmt = build (ASM_EXPR, void_type_node, ASM_STRING (stmt),
			ASM_OUTPUTS (stmt), ASM_INPUTS (stmt),
			ASM_CLOBBERS (stmt));
	  ASM_INPUT_P (stmt) = ASM_INPUT_P (*stmt_p);
	  ASM_VOLATILE_P (stmt) = ASM_VOLATILE_P (*stmt_p);
	  break;

	case FILE_STMT:
	  input_filename = FILE_STMT_FILENAME (stmt);
	  goto cont;

	default:
	  if (lang_simplify_stmt && (*lang_simplify_stmt) (&stmt, &next))
	    break;

	  fprintf (stderr, "unhandled statement node in c_simplify_stmt ():\n");
	  debug_tree (stmt);
	  abort ();
	  break;
	}

      /* PRE and POST now contain a list of statements for all the
	 side-effects in STMT.  */

      add_tree (stmt, &pre);
      add_tree (post, &pre);
      pre = rationalize_compound_expr (pre);
      annotate_all_with_file_line (&pre, stmt_filename, stmt_lineno);

      add_tree (pre, &outer_pre);
    cont:
      /* Restore saved state.  */
      current_stmt_tree ()->stmts_are_full_exprs_p
	= saved_stmts_are_full_exprs_p;
    }
  add_tree (stmt, &outer_pre);
  *stmt_p = rationalize_compound_expr (outer_pre);
}

static void
add_block_to_enclosing (block)
     tree block;
{
  tree enclosing;

  for (enclosing = gimple_current_bind_expr ();
       enclosing; enclosing = TREE_CHAIN (enclosing))
    if (BIND_EXPR_BLOCK (enclosing))
      break;

  enclosing = BIND_EXPR_BLOCK (enclosing);	 
  BLOCK_SUBBLOCKS (enclosing) = chainon (BLOCK_SUBBLOCKS (enclosing), block);
}

/* Genericize a scope by creating a new BIND_EXPR.
   BLOCK is either a BLOCK representing the scope or a chain of _DECLs.
     In the latter case, we need to create a new BLOCK and add it to the
     BLOCK_SUBBLOCKS of the enclosing block.
   BODY is a chain of C _STMT nodes for the contents of the scope, to be
     genericized.  */

static tree
c_build_bind_expr (block, body)
     tree block;
     tree body;
{
  tree decls, bind;

  if (block == NULL_TREE)
    decls = NULL_TREE;
  else if (TREE_CODE (block) == BLOCK)
    decls = BLOCK_VARS (block);
  else
    {
      decls = block;
      if (DECL_ARTIFICIAL (decls))
	block = NULL_TREE;
      else
	{
	  block = make_node (BLOCK);
	  BLOCK_VARS (block) = decls;
	  add_block_to_enclosing (block);
	}
    }

  bind = build (BIND_EXPR, void_type_node, decls, body, block);
  TREE_SIDE_EFFECTS (bind) = 1;

  gimple_push_bind_expr (bind);
  c_simplify_stmt (&BIND_EXPR_BODY (bind));
  gimple_pop_bind_expr ();

  return bind;
}

/* Genericize a syntactic block by removing the bracketing SCOPE_STMTs and
   wrapping the intervening code in a BIND_EXPR.  This function assumes
   that matching SCOPE_STMTs will always appear in the same statement
   sequence.  */

static void
simplify_block (stmt_p, next_p)
     tree *stmt_p;
     tree *next_p;
{
  tree *p;
  tree block;
  tree bind;
  int depth;

  if (!SCOPE_BEGIN_P (*stmt_p))
    abort ();

  block = SCOPE_STMT_BLOCK (*stmt_p);

  /* Find the matching ending SCOPE_STMT.  */
  depth = 1;
  for (p = &TREE_CHAIN (*stmt_p);; p = &TREE_CHAIN (*p))
    if (TREE_CODE (*p) == SCOPE_STMT)
      {
	if (SCOPE_BEGIN_P (*p))
	  ++depth;
	else if (--depth == 0)
	  break;
      }
  if (SCOPE_STMT_BLOCK (*p) != block)
    abort ();

  *next_p = TREE_CHAIN (*p);
  *p = NULL_TREE;

  bind = c_build_bind_expr (block, TREE_CHAIN (*stmt_p));
  *stmt_p = bind;
}

/* Genericize a CLEANUP_STMT.  Just wrap everything from here to the end of
   the block in a TRY_FINALLY_EXPR.  Or a TRY_CATCH_EXPR, if it's an
   EH-only cleanup.  */

static void
simplify_cleanup (stmt_p, next_p)
     tree *stmt_p;
     tree *next_p;
{
  tree stmt = *stmt_p;
  tree body = TREE_CHAIN (stmt);
  tree cleanup = CLEANUP_EXPR (stmt);
  enum tree_code code
    = (CLEANUP_EH_ONLY (stmt) ? TRY_CATCH_EXPR : TRY_FINALLY_EXPR);

  cleanup = maybe_protect_cleanup (cleanup);

  c_simplify_stmt (&body);

  *stmt_p = build (code, void_type_node, body, cleanup);
  *next_p = NULL_TREE;
}

/*  Simplify an EXPR_STMT node.

    STMT is the statement node.

    PRE_P points to the list where side effects that must happen before
	STMT should be stored.

    POST_P points to the list where side effects that must happen after
	STMT should be stored.  */

static void
simplify_expr_stmt (stmt_p)
     tree *stmt_p;
{
  tree stmt = EXPR_STMT_EXPR (*stmt_p);

  /* Simplification of a statement expression will nullify the
     statement if all its side effects are moved to *PRE_P and *POST_P.

     In this case we will not want to emit the simplified statement.
     However, we may still want to emit a warning, so we do that before
     simplification.  */
  if (stmt && (extra_warnings || warn_unused_value))
    {
      if (!TREE_SIDE_EFFECTS (stmt))
	{
	  if (stmt != empty_stmt_node
	      && !(TREE_CODE (stmt) == CONVERT_EXPR
		   && VOID_TYPE_P (TREE_TYPE (stmt))))
	    warning ("statement with no effect");
	}
      else if (warn_unused_value)
	{
	  /* Kludge for 20020220-2.c.  warn_if_unused_value shouldn't use
	     the stmt file location info.  */
	  set_file_and_line_for_stmt (input_filename, lineno);
	  warn_if_unused_value (stmt);
	}
    }

  if (stmts_are_full_exprs_p ())
    stmt = build1 (CLEANUP_POINT_EXPR, void_type_node, stmt);
  *stmt_p = stmt;
}

/* If the condition for a loop (or the like) is a decl, it will be a
   TREE_LIST where the TREE_PURPOSE is a DECL_STMT and the TREE_VALUE is
   a use of the decl.  Turn such a thing into a COMPOUND_EXPR.  */

static void
simplify_condition (cond_p)
     tree *cond_p;
{
  tree cond = *cond_p;
  if (cond && TREE_CODE (cond) == TREE_LIST)
    {
      tree decl = TREE_PURPOSE (cond);
      tree value = TREE_VALUE (cond);
      c_simplify_stmt (&decl);
      *cond_p = build (COMPOUND_EXPR, TREE_TYPE (value), decl, value);
    }
}

/* Begin a scope which can be exited by a break or continue statement.  BC
   indicates which.

   Just creates a label and pushes it into the current context.  */

static tree
begin_bc_block (bc)
     enum bc_t bc;
{
  tree label = build_decl (LABEL_DECL, ctxp->bc_id[bc], NULL_TREE);
  DECL_ARTIFICIAL (label) = 1;
  DECL_CONTEXT (label) = current_function_decl;
  TREE_CHAIN (label) = ctxp->current_bc_label;
  ctxp->current_bc_label = label;
  return label;
}

/* Finish a scope which can be exited by a break or continue statement.
   LABEL was returned from the most recent call to begin_bc_block.  BODY is
   an expression for the contents of the scope.

   If we saw a break (or continue) in the scope, append a LABEL_EXPR to
   body.  Otherwise, just forget the label.  */

static tree
finish_bc_block (label, body)
     tree label, body;
{
  if (label != ctxp->current_bc_label)
    abort ();

  if (TREE_USED (label))
    {
      tree expr = build1 (LABEL_EXPR, void_type_node, label);
      /* Clear the name so flow can delete the label.  */
      DECL_NAME (label) = NULL_TREE;
      add_tree (expr, &body);
    }

  ctxp->current_bc_label = TREE_CHAIN (label);
  TREE_CHAIN (label) = NULL_TREE;
  return body;
}

/* Build a GOTO_EXPR to represent a break or continue statement.  BC
   indicates which.  */

static tree
build_bc_goto (bc)
     enum bc_t bc;
{
  tree label;
  tree target_name = ctxp->bc_id[bc];

  /* Look for the appropriate type of label.  */
  for (label = ctxp->current_bc_label; ;
       label = TREE_CHAIN (label))
    if (DECL_NAME (label) == target_name)
      break;

  /* Mark the label used for finish_bc_block.  */
  TREE_USED (label) = 1;
  return build1 (GOTO_EXPR, void_type_node, label);
}

/* Build a generic representation of one of the C loop forms.  COND is the
   loop condition or NULL_TREE.  BODY is the (possibly compound) statement
   controlled by the loop.  INCR is the increment expression of a for-loop,
   or NULL_TREE.  COND_IS_FIRST indicates whether the condition is
   evaluated before the loop body as in while and for loops, or after the
   loop body as in do-while loops.  */

static tree
simplify_c_loop (cond, body, incr, cond_is_first)
     tree cond;
     tree body;
     tree incr;
     int cond_is_first;
{
  tree exit, cont_block, break_block, loop;
  const char *stmt_filename;
  int stmt_lineno;
  tree stuff;

  stmt_filename = input_filename;
  stmt_lineno = lineno;

  break_block = begin_bc_block (bc_break);

  loop = build (LOOP_EXPR, void_type_node, NULL_TREE);

  if (cond)
    {
      simplify_condition (&cond);
      exit = build_bc_goto (bc_break);
      exit = build (COND_EXPR, void_type_node, cond, empty_stmt_node, exit);
      exit = fold (exit);
    }
  else
    exit = NULL_TREE;

  cont_block = begin_bc_block (bc_continue);

  c_simplify_stmt (&body);

  body = finish_bc_block (cont_block, body);

  stuff = NULL_TREE;
  if (cond_is_first)
    {
      add_tree (exit, &stuff);
      add_tree (body, &stuff);
      add_tree (incr, &stuff);
    }
  else
    {
      add_tree (body, &stuff);
      add_tree (incr, &stuff);
      add_tree (exit, &stuff);
    }

  annotate_all_with_file_line (&stuff, stmt_filename, stmt_lineno);

  LOOP_EXPR_BODY (loop) = rationalize_compound_expr (stuff);

  loop = finish_bc_block (break_block, loop);

  return loop;
}

/* Simplify a FOR_STMT node.  Move the stuff in the for-init-stmt into the
   prequeue and hand off to simplify_c_loop.  */

static void
simplify_for_stmt (stmt_p, pre_p)
     tree *stmt_p;
     tree *pre_p;
{
  tree stmt = *stmt_p;

  tree init = FOR_INIT_STMT (stmt);
  c_simplify_stmt (&init);
  add_tree (init, pre_p);

  *stmt_p = simplify_c_loop (FOR_COND (stmt), FOR_BODY (stmt),
			     FOR_EXPR (stmt), 1);
}

/* Simplify a WHILE_STMT node.  */

static void
simplify_while_stmt (stmt_p)
     tree *stmt_p;
{
  tree stmt = *stmt_p;
  *stmt_p = simplify_c_loop (WHILE_COND (stmt), WHILE_BODY (stmt),
			     NULL_TREE, 1);
}

/*  Simplify a DO_STMT node.  */

static void
simplify_do_stmt (stmt_p)
     tree *stmt_p;
{
  tree stmt = *stmt_p;
  *stmt_p = simplify_c_loop (DO_COND (stmt), DO_BODY (stmt),
			     NULL_TREE, 0);
}

/* Genericize an IF_STMT by turning it into a COND_EXPR.  */

static void
simplify_if_stmt (stmt_p)
     tree *stmt_p;
{
  tree stmt = *stmt_p;
  tree then_ = THEN_CLAUSE (stmt);
  tree else_ = ELSE_CLAUSE (stmt);
  tree cond = IF_COND (stmt);

  simplify_condition (&cond);

  c_simplify_stmt (&then_);
  c_simplify_stmt (&else_);

  *stmt_p = build (COND_EXPR, void_type_node, cond, then_, else_);
}

/* Genericize a SWITCH_STMT by turning it into a SWITCH_EXPR.  */

static void
simplify_switch_stmt (stmt_p)
     tree *stmt_p;
{
  tree stmt = *stmt_p;
  tree body = SWITCH_BODY (stmt);
  tree break_block, switch_;
  tree cond = SWITCH_COND (stmt);

  simplify_condition (&cond);

  break_block = begin_bc_block (bc_break);

  c_simplify_stmt (&body);

  switch_ = build (SWITCH_EXPR, void_type_node, cond, body, NULL_TREE);

  switch_ = finish_bc_block (break_block, switch_);

  *stmt_p = switch_;
}

/* Genericize a RETURN_STMT by turning it into a RETURN_EXPR.  */

static void
simplify_return_stmt (stmt_p)
     tree *stmt_p;
{
  tree expr = RETURN_STMT_EXPR (*stmt_p);
  expr = build1 (RETURN_EXPR, void_type_node, expr);
  if (stmts_are_full_exprs_p ())
    expr = build1 (CLEANUP_POINT_EXPR, void_type_node, expr);
  *stmt_p = expr;
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
simplify_decl_stmt (stmt_p, next_p)
    tree *stmt_p;
    tree *next_p;
{
  tree stmt = *stmt_p;
  tree decl = DECL_STMT_DECL (stmt);
  tree pre = NULL_TREE;

  if (TREE_CODE (decl) == VAR_DECL && !DECL_EXTERNAL (decl))
    {
      tree init = DECL_INITIAL (decl);

      if (!TREE_CONSTANT (DECL_SIZE (decl)))
	{
	  /* This is a variable-sized decl.  We need to wrap it in a new
	     block so that we can simplify the expressions for calculating
	     its size, and so that any other local variables used in those
	     expressions will have been initialized.  */

	  /* FIXME break the allocation out into a separate statement.  */

	  tree size = DECL_SIZE (decl);
	  tree usize = DECL_SIZE_UNIT (decl);
	  tree bind;
	  tree *p;

	  size = get_initialized_tmp_var (size, &pre);
	  usize = get_initialized_tmp_var (usize, &pre);

	  /* Mark the size and unit size as being used in the 
	     VLA's declaration so they will not be deleted by
	     DCE.  */
	  set_vla_decl (size);
	  set_vla_decl (usize);

	  /* FIXME also simplify field sizes.  */
	  DECL_SIZE (decl) = TYPE_SIZE (TREE_TYPE (decl)) = size;
	  DECL_SIZE_UNIT (decl) = TYPE_SIZE_UNIT (TREE_TYPE (decl)) = usize;

	  /* Prune this decl and any others after it out of the enclosing
	     block.  */
	  for (p = &BIND_EXPR_VARS (gimple_current_bind_expr ());
	       *p != decl; p = &TREE_CHAIN (*p))
	    /* search */;
	  *p = NULL_TREE;
	  if (BLOCK_VARS (BIND_EXPR_BLOCK (gimple_current_bind_expr ()))
	      == decl)
	    BLOCK_VARS (BIND_EXPR_BLOCK (gimple_current_bind_expr ()))
	      = NULL_TREE;

	  bind = c_build_bind_expr (decl, TREE_CHAIN (stmt));

	  add_tree (bind, &pre);

	  if (next_p)
	    *next_p = NULL_TREE;
	}

      if (init && init != error_mark_node)
	{
	  if (!TREE_STATIC (decl))
	    {
	      DECL_INITIAL (decl) = NULL_TREE;
	      init = build (MODIFY_EXPR, void_type_node, decl, init);
	      if (stmts_are_full_exprs_p ())
		init = build1 (CLEANUP_POINT_EXPR, void_type_node, init);
	      /* FIXME: Shouldn't we simplify here?  */
	      add_tree (init, &pre);
	    }
	  else
	    {
	      /* We must still examine initializers for static variables
		 as they may contain a label address.  However, we must not
		 make any changes to the node or the queues.  So we
		 make a copy of the node before calling the simplifier
		 and we use throw-away queues.  */
	      tree pre = NULL;
	      tree post = NULL;
	      tree dummy_init = deep_copy_node (init);
	      simplify_expr (&dummy_init, &pre, &post,
			     is_simple_initializer,
			     fb_rvalue);
	    }
	}

      /* This decl isn't mentioned in the enclosing block, so add it to the
	 list of temps.  FIXME it seems a bit of a kludge to say that
	 anonymous artificial vars aren't pushed, but everything else is.  */
      if (DECL_ARTIFICIAL (decl) && DECL_NAME (decl) == NULL_TREE)
	gimple_add_tmp_var (decl);
    }

  *stmt_p = pre;
}

/* Simplification of expression trees.  */

/* Simplify a C99 compound literal expression.  This just means adding the
   DECL_STMT before the current EXPR_STMT and using its anonymous decl
   instead.  */

static void
simplify_compound_literal_expr (expr_p)
     tree *expr_p;
{
  tree decl_s = COMPOUND_LITERAL_EXPR_DECL_STMT (*expr_p);
  tree decl = DECL_STMT_DECL (decl_s);

  simplify_decl_stmt (&decl_s, NULL);
  *expr_p = decl_s ? decl_s : decl;
}

/* Do C-specific simplification.  Args are as for simplify_expr.  */

int
c_simplify_expr (expr_p, pre_p, post_p)
     tree *expr_p;
     tree *pre_p ATTRIBUTE_UNUSED;
     tree *post_p ATTRIBUTE_UNUSED;
{
  enum tree_code code = TREE_CODE (*expr_p);
  
  if (statement_code_p (code))
    {
      c_simplify_stmt (expr_p);
      return 1;
    }
  else switch (code)
    {
    case COMPOUND_LITERAL_EXPR:
      simplify_compound_literal_expr (expr_p);
      return 1;

    case STMT_EXPR:
      simplify_stmt_expr (expr_p);
      return 1;

    default:
      return 0;
    }
}

/* Simplify a STMT_EXPR.  EXPR_P points to the expression to simplify.
    After simplification, if the STMT_EXPR returns a value, EXPR_P will
    point to a new temporary that holds that value; otherwise it will be
    null.

    PRE_P points to the list where side effects that must happen before
      *EXPR_P should be stored.  */

static void
simplify_stmt_expr (expr_p)
     tree *expr_p;
{
  tree body = STMT_EXPR_STMT (*expr_p);
  if (VOID_TYPE_P (TREE_TYPE (*expr_p)))
    c_simplify_stmt (&body);
  else
    {
      tree substmt, last_expr_stmt, last_expr, bind;

      bind = NULL_TREE;	/* [GIMPLE] Avoid uninitialized use warning.  */

      /* Splice the last expression out of the STMT chain.  */
      last_expr_stmt = NULL_TREE;
      for (substmt = COMPOUND_BODY (body); substmt;
	   substmt = TREE_CHAIN (substmt))
	{
	  if (TREE_CODE (substmt) == EXPR_STMT)
	    last_expr_stmt = substmt;
	}

      last_expr = EXPR_STMT_EXPR (last_expr_stmt);
      if (stmts_are_full_exprs_p ())
	last_expr = build1 (CLEANUP_POINT_EXPR, TREE_TYPE (last_expr),
			    last_expr);
      EXPR_STMT_EXPR (last_expr_stmt) = empty_stmt_node;
#if defined ENABLE_CHECKING
      if (!is_last_stmt_of_scope (last_expr_stmt))
	abort ();
#endif

      /* Genericize the block.  */
      c_simplify_stmt (&body);

      /* Now retrofit that last expression into the BIND_EXPR.  */
      if (!STMT_EXPR_NO_SCOPE (*expr_p))
	{
	  bind = body;
	  substmt = BIND_EXPR_BODY (bind);
	}
      else
	substmt = body;

      if (substmt == empty_stmt_node)
	substmt = last_expr;
      else
	substmt = build (COMPOUND_EXPR, TREE_TYPE (last_expr),
			 substmt, last_expr);

      if (!STMT_EXPR_NO_SCOPE (*expr_p))
	{
	  BIND_EXPR_BODY (bind) = substmt;
	  TREE_TYPE (bind) = TREE_TYPE (body) = TREE_TYPE (last_expr);
	}
      else
	body = substmt;
    }

  *expr_p = body;
}

/* Code generation.  */

/* Miscellaneous helpers.  */

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
      walk_tree (&node, mostly_copy_tree_r, NULL, NULL);
      res = node;
      break;
    }
  
  /* Set the line number.  */
  if (statement_code_p (TREE_CODE (node)))
    STMT_LINENO (res) = STMT_LINENO (node);
  
  return res;
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
  enum tree_code code = TREE_CODE (*tp);
  /* Don't unshare decls, blocks, types and SAVE_EXPR nodes.  */
  if (TREE_CODE_CLASS (code) == 't'
      || TREE_CODE_CLASS (code) == 'd'
      || TREE_CODE_CLASS (code) == 'c'
      || TREE_CODE_CLASS (code) == 'b'
      || code == SAVE_EXPR
      || *tp == empty_stmt_node)
    *walk_subtrees = 0;
  else if (code == STMT_EXPR || code == SCOPE_STMT || code == BIND_EXPR)
    /* Unsharing STMT_EXPRs doesn't make much sense; they tend to be
       complex, so they shouldn't be shared in the first place.  Unsharing
       SCOPE_STMTs breaks because copy_tree_r zeroes out the block.  */
    abort ();
  else
    copy_tree_r (tp, walk_subtrees, data);

  return NULL_TREE;
}


#if defined ENABLE_CHECKING
/*  Return nonzero if STMT is the last statement of its scope.  */

static int
is_last_stmt_of_scope (stmt)
     tree stmt;
{
  return (TREE_CHAIN (stmt) == NULL_TREE
	  || (TREE_CODE (TREE_CHAIN (stmt)) == SCOPE_STMT
	      && SCOPE_END_P (TREE_CHAIN (stmt))));
}
#endif

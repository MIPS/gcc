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
#include "langhooks-def.h"
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

void c_simplify_stmt          PARAMS ((tree *));
static void simplify_expr_stmt       PARAMS ((tree *));
static void simplify_decl_stmt       PARAMS ((tree *, tree *));
static void simplify_for_stmt        PARAMS ((tree *, tree *));
static void simplify_while_stmt      PARAMS ((tree *));
static void simplify_do_stmt         PARAMS ((tree *));
static void simplify_if_stmt         PARAMS ((tree *));
static void simplify_switch_stmt     PARAMS ((tree *));
static void simplify_return_stmt     PARAMS ((tree *));
static void simplify_stmt_expr       PARAMS ((tree *, tree *));
static void simplify_compound_literal_expr PARAMS ((tree *, tree *));
static void make_type_writable       PARAMS ((tree));
static int  expr_has_effect          PARAMS ((tree));
static tree copy_if_shared_r         PARAMS ((tree *, int *, void *));
static tree unmark_visited_r         PARAMS ((tree *, int *, void *));
static void unshare_all_trees        PARAMS ((tree));
static inline void remove_suffix     PARAMS ((char *, int));
static const char *get_name          PARAMS ((tree));
static int is_last_stmt_of_scope     PARAMS ((tree));
static tree mark_not_simple_r        PARAMS ((tree *, int *, void *));
static void simplify_block	     PARAMS ((tree *, tree *));
static tree simplify_c_loop	     PARAMS ((tree, tree, tree, int));
static void push_context             PARAMS ((void));
static void pop_context              PARAMS ((void));

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
static void simplify_return_expr     PARAMS ((tree, tree *));
static tree build_addr_expr	     PARAMS ((tree));
static tree add_stmt_to_compound	PARAMS ((tree, tree));

typedef void foreach_stmt_fn PARAMS ((tree *));
void foreach_stmt PARAMS ((tree *, foreach_stmt_fn *));

static void wrap_with_wfl PARAMS ((tree *));
void wrap_all_with_wfl PARAMS ((tree *, const char *, int));

enum bc_t { bc_break = 0, bc_continue = 1 };
static tree begin_bc_block PARAMS ((enum bc_t));
static tree finish_bc_block PARAMS ((tree, tree));
static tree build_bc_goto PARAMS ((enum bc_t));

/* Local variables.  */
static FILE *dump_file;
static int dump_flags;

#define STRIP_WFL(NODE)					\
  do {							\
    while (TREE_CODE (NODE) == EXPR_WITH_FILE_LOCATION)	\
      NODE = EXPR_WFL_NODE (NODE);			\
  } while (0);

struct c_simplify_ctx
{
  /* For handling break and continue.  */
  tree current_bc_label;
  tree bc_id[2];

  tree current_bind_expr;
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

/*  Entry point to the simplification pass.  FNDECL is the FUNCTION_DECL
    node for the function we want to simplify.  */

int
simplify_function_tree (fndecl)
     tree fndecl;
{
  tree fnbody;
  tree block;
  int done;

  /* Don't bother doing anything if the program has errors.  */
  if (errorcount || sorrycount)
    return 0;
  
  /* FIXME.  Hack.  If this front end does not support simplification,
     do nothing.  */
  if (lang_hooks.simplify_expr == lhd_simplify_expr)
    return 0;

  fnbody = DECL_SAVED_TREE (fndecl);
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

  /* Unshare most shared trees in the body.  */
  unshare_all_trees (fnbody);

  /* Simplify the function's body.  */
  done = simplify_stmt (&fnbody);

  /* Unshare again, in case simplification was sloppy.  */
  unshare_all_trees (fnbody);

  DECL_SAVED_TREE (fndecl) = fnbody;

  /* Restore the binding level.  */
  block = poplevel (1, 1, 0);

  /* Declare the new temporary variables.  */
  declare_tmp_vars (BLOCK_VARS (block), fnbody);

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

  return done;
}

/* Simplify an expression which appears at statement context; usually, this
   means replacing it with a suitably simple COMPOUND_EXPR.  */

int
simplify_stmt (stmt_p)
     tree *stmt_p;
{
  return simplify_expr (stmt_p, NULL, NULL, is_simple_stmt, fb_rvalue);
}

/* Apply FN to each statement under *STMT_P, which may be a COMPOUND_EXPR
   or a single statement expr.  */

void
foreach_stmt (stmt_p, fn)
     tree *stmt_p;
     foreach_stmt_fn *fn;
{
  if (*stmt_p == NULL_TREE)
    return;

  for (; TREE_CODE (*stmt_p) == COMPOUND_EXPR;
       stmt_p = &TREE_OPERAND (*stmt_p, 1))
    fn (&TREE_OPERAND (*stmt_p, 0));
  fn (stmt_p);
}

const char *wfl_filename;
int wfl_lineno;

static void
wrap_with_wfl (stmt_p)
     tree *stmt_p;
{
  if (TREE_CODE (*stmt_p) != EXPR_WITH_FILE_LOCATION)
    {
      *stmt_p = build_expr_wfl (*stmt_p, wfl_filename, wfl_lineno, 0);
      EXPR_WFL_EMIT_LINE_NOTE (*stmt_p) = 1;
    }
}

void
wrap_all_with_wfl (stmt_p, file, line)
     tree *stmt_p;
     const char *file;
     int line;
{
  wfl_filename = file;
  wfl_lineno = line;
  foreach_stmt (stmt_p, wrap_with_wfl);
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

      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "# %d\nORIGINAL:\n", STMT_LINENO (stmt));
	  print_c_node (dump_file, stmt);
	  fprintf (dump_file, "\n");
	}

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
	  abort ();
	  goto cont;

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
	  fprintf (stderr, "unhandled statement node in c_simplify_stmt ():\n");
	  debug_tree (stmt);
	  abort ();
	  break;
	}

      /* PRE and POST now contain a list of statements for all the
	 side-effects in STMT.  */

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

      add_tree (stmt, &pre);
      add_tree (post, &pre);
      pre = rationalize_compound_expr (pre);
      wrap_all_with_wfl (&pre, stmt_filename, stmt_lineno);

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

  for (enclosing = ctxp->current_bind_expr;
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
      block = make_node (BLOCK);
      BLOCK_VARS (block) = decls;
      add_block_to_enclosing (block);
    }

  bind = build (BIND_EXPR, void_type_node, decls, body, block);
  TREE_SIDE_EFFECTS (bind) = 1;
  
  TREE_CHAIN (bind) = ctxp->current_bind_expr;
  ctxp->current_bind_expr = bind;

  c_simplify_stmt (&BIND_EXPR_BODY (bind));

  ctxp->current_bind_expr = TREE_CHAIN (ctxp->current_bind_expr);

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
	if (SCOPE_PARTIAL_P (*p))
	  /* Throw away partial scopes.  Hmm, that probably won't work;
	     we need to remember them to handle goto cleanups.  */
	  abort ();
	else if (SCOPE_BEGIN_P (*p))
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
      if (!expr_has_effect (stmt))
	warning ("statement with no effect");
      else if (warn_unused_value)
	warn_if_unused_value (stmt);
    }

  *stmt_p = stmt;
}

/* Begin a scope which can be exited by a break or continue statement.  BC
   indicates which.

   Just creates a label and pushes it into the current context.  */

static tree
begin_bc_block (bc)
     enum bc_t bc;
{
  tree label = build_decl (LABEL_DECL, ctxp->bc_id[bc], NULL_TREE);
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
      body = add_stmt_to_compound (body, expr);
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

  break_block = begin_bc_block (bc_break);

  loop = build (LOOP_EXPR, void_type_node, NULL_TREE);

  if (cond)
    {
#if 0
      exit = build (EXIT_EXPR, void_type_node, cond);
      EXIT_EXPR_IS_LOOP_COND (exit) = 1;
#else 
      exit = build_bc_goto (bc_break);
      exit = build (COND_EXPR, void_type_node, cond, empty_stmt_node, exit);
#endif
    }
  else
    exit = NULL_TREE;

  cont_block = begin_bc_block (bc_continue);

  c_simplify_stmt (&body);

  body = finish_bc_block (cont_block, body);

  body = add_stmt_to_compound (body, incr);
  if (cond_is_first)
    body = add_stmt_to_compound (exit, body);
  else
    body = add_stmt_to_compound (body, exit);

  LOOP_EXPR_BODY (loop) = body;

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

  c_simplify_stmt (&then_);
  c_simplify_stmt (&else_);

  *stmt_p = build (COND_EXPR, void_type_node, IF_COND (stmt), then_, else_);
}

/* Genericize a SWITCH_STMT by turning it into a SWITCH_EXPR.  */

static void
simplify_switch_stmt (stmt_p)
     tree *stmt_p;
{
  tree stmt = *stmt_p;
  tree body = SWITCH_BODY (stmt);
  tree break_block, switch_;

  break_block = begin_bc_block (bc_break);

  c_simplify_stmt (&body);

  switch_ = build (SWITCH_EXPR, void_type_node, SWITCH_COND (stmt), body,
		   NULL_TREE);

  switch_ = finish_bc_block (break_block, switch_);

  *stmt_p = switch_;
}

/* Genericize a RETURN_STMT by turning it into a RETURN_EXPR.  */

static void
simplify_return_stmt (stmt_p)
     tree *stmt_p;
{
  tree stmt = *stmt_p;
  *stmt_p = build (RETURN_EXPR, void_type_node, RETURN_STMT_EXPR (stmt));
}

/* Simplify a RETURN_EXPR.  If the expression to be returned is not a
   SIMPLE value, it is assigned to a new temporary and the statement is
   re-written to return the temporary.

   PRE_P points to the list where side effects that must happen before
       STMT should be stored.  */

static void
simplify_return_expr (stmt, pre_p)
     tree stmt;
     tree *pre_p;
{
  tree ret_expr = TREE_OPERAND (stmt, 0);

  if (ret_expr)
    {
      if (VOID_TYPE_P (TREE_TYPE (TREE_TYPE (current_function_decl))))
	{
	  /* We are trying to return an expression in a void function.
	     Move the expression to before the return.  */
	  simplify_stmt (&ret_expr);
	  add_tree (ret_expr, pre_p);
	  TREE_OPERAND (stmt, 0) = NULL_TREE;
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
simplify_decl_stmt (stmt_p, next_p)
    tree *stmt_p;
    tree *next_p;
{
  tree stmt = *stmt_p;
  tree decl = DECL_STMT_DECL (stmt);
  tree pre = NULL_TREE;

  if (TREE_CODE (decl) == VAR_DECL)
    {
      tree init = DECL_INITIAL (decl);

      if (!TREE_CONSTANT (DECL_SIZE (decl)))
	{
	  /* This is a variable-sized decl.  We need to wrap it in a new
	     block so that we can simplify the expressions for calculating
	     its size, and so that any other local variables used in those
	     expressions will have been initialized.  */

	  tree size = DECL_SIZE (decl);
	  tree usize = DECL_SIZE_UNIT (decl);
	  tree bind;
	  tree *p;

	  size = get_initialized_tmp_var (size, &pre);
	  usize = get_initialized_tmp_var (usize, &pre);

	  /* FIXME also simplify field sizes.  */
	  DECL_SIZE (decl) = TYPE_SIZE (TREE_TYPE (decl)) = size;
	  DECL_SIZE_UNIT (decl) = TYPE_SIZE_UNIT (TREE_TYPE (decl)) = usize;

	  /* Prune this decl and any others after it out of the enclosing block.  */
	  for (p = &BIND_EXPR_VARS (ctxp->current_bind_expr);
	       *p != decl; p = &TREE_CHAIN (*p))
	    /* search */;
	  *p = NULL_TREE;
	  if (BLOCK_VARS (BIND_EXPR_BLOCK (ctxp->current_bind_expr)) == decl)
	    BLOCK_VARS (BIND_EXPR_BLOCK (ctxp->current_bind_expr)) = NULL_TREE;

	  bind = c_build_bind_expr (decl, TREE_CHAIN (stmt));

	  add_tree (bind, &pre);

	  *next_p = NULL_TREE;
	}

      if (init && init != error_mark_node && !TREE_STATIC (decl))
	{
	  DECL_INITIAL (decl) = NULL_TREE;
	  add_tree (build (INIT_EXPR, void_type_node, decl, init), &pre);
	}
    }

  *stmt_p = pre;
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
simplify_compound_literal_expr (expr_p, pre_p)
     tree *expr_p;
     tree *pre_p;
{
  tree decl_s = COMPOUND_LITERAL_EXPR_DECL_STMT (*expr_p);
  tree decl = DECL_STMT_DECL (decl_s);

  /* This decl isn't mentioned in the enclosing block, so add it to
     the list of temps.  */
  pushdecl (decl);
  simplify_decl_stmt (&decl_s, NULL);
  add_tree (decl_s, pre_p);
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
      if (*expr_p == NULL_TREE)
	break;

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
	  if (is_statement)
	    foreach_stmt (expr_p, simplify_stmt);
	  else
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
	  recalculate_side_effects (*expr_p);
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
	  recalculate_side_effects (*expr_p);
	  break;

	case INDIRECT_REF:
	  simplify_expr (&TREE_OPERAND (*expr_p, 0), pre_p, post_p, is_simple_id,
			 fb_rvalue);
	  break;

	  /* Constants need not be simplified.  */
	case INTEGER_CST:
	case REAL_CST:
	case STRING_CST:
	case COMPLEX_CST:
	  break;

	case BIND_EXPR:
	  simplify_stmt (&BIND_EXPR_BODY (*expr_p));
	  recalculate_side_effects (*expr_p);
	  break;

	case LOOP_EXPR:
	  simplify_stmt (&LOOP_EXPR_BODY (*expr_p));
	  break;

	case SWITCH_EXPR:
	  simplify_expr (&SWITCH_COND (*expr_p), pre_p, NULL,
			 is_simple_val, fb_rvalue);
	  simplify_stmt (&SWITCH_BODY (*expr_p));
	  break;

	case LABELED_BLOCK_EXPR:
	  simplify_stmt (&LABELED_BLOCK_BODY (*expr_p));
	  recalculate_side_effects (*expr_p);
	  break;

	case EXIT_EXPR:
	  simplify_expr (&TREE_OPERAND (*expr_p, 0), pre_p, NULL,
			 is_simple_condexpr, fb_rvalue);
	  break;

	case GOTO_EXPR:
	  if (TREE_CODE (GOTO_DESTINATION (*expr_p)) != LABEL_DECL)
	    simplify_expr (&GOTO_DESTINATION (*expr_p), pre_p, NULL,
			   is_simple_val, fb_rvalue);
	  break;

	case LABEL_EXPR:
	case CASE_LABEL_EXPR:
	  break;

	case RETURN_EXPR:
	  simplify_return_expr (*expr_p, pre_p);
	  break;

	case CONSTRUCTOR:
	  simplify_constructor (*expr_p, pre_p, post_p);
	  break;

	  /* The following are special cases that are not handled by the
	     original SIMPLE grammar.  */

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
	  recalculate_side_effects (*expr_p);
	  break;

	case ASM_EXPR:
	  simplify_asm_expr (*expr_p, pre_p);
	  break;

	  /* If *EXPR_P does not need to be special-cased, handle it
	     according to its class.  */
	default:
	  {
	    if (TREE_CODE_CLASS (TREE_CODE (*expr_p)) == '1')
	      {
		simplify_expr (&TREE_OPERAND (*expr_p, 0), pre_p, post_p,
			       is_simple_val, fb_rvalue);
		recalculate_side_effects (*expr_p);
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
		recalculate_side_effects (*expr_p);
	      }
	    else if (TREE_CODE_CLASS (TREE_CODE (*expr_p)) == 'd'
		     || TREE_CODE_CLASS (TREE_CODE (*expr_p)) == 'c')
	      /* OK */;
	    else
	      /* Fail if we don't know how to handle this tree code.  */
	      abort ();
	  }
	}
    }
  /* If we replaced *expr_p, simplify again.  */
  while (*expr_p && *expr_p != save_expr);

  /* If we are simplifying at the statement level, we're done.  Tack
     everything together and replace the original statement with the
     simplified form.  */
  if (is_statement)
    {
      add_tree (*expr_p, pre_p);
      add_tree (internal_post, pre_p);
      tmp = rationalize_compound_expr (internal_pre);
      *expr_p = tmp;
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
     tree *post_p ATTRIBUTE_UNUSED;
{
  enum tree_code code = TREE_CODE (*expr_p);
  
  if (statement_code_p (code))
    {
      push_context ();
      c_simplify_stmt (expr_p);
      pop_context ();
      return 1;
    }
  else switch (code)
    {
    case COMPOUND_LITERAL_EXPR:
      simplify_compound_literal_expr (expr_p, pre_p);
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
    t1;

    The second form is used when *EXPR_P is of type void.

    PRE_P points to the list where side effects that must happen before
        *EXPR_P should be stored.  */

static void
simplify_cond_expr (expr_p, pre_p)
     tree *expr_p;
     tree *pre_p;
{
#if defined ENABLE_CHECKING
  if (TREE_CODE (*expr_p) != COND_EXPR)
    abort ();
#endif

  if (VOID_TYPE_P (TREE_TYPE (*expr_p)))
    {
      /* If this is already void, just make sure it's simple.  */
      simplify_expr (&TREE_OPERAND (*expr_p, 0), pre_p, NULL,
		     is_simple_condexpr, fb_rvalue);
      simplify_stmt (&TREE_OPERAND (*expr_p, 1));
      simplify_stmt (&TREE_OPERAND (*expr_p, 2));
     }
  else
    {
      tree t_then, t_else, tmp, pred, new_if;

      /* Otherwise, we need to copy the values into a temporary.  */
      tmp = create_tmp_var (TREE_TYPE (*expr_p), "iftmp");

      /* Simplify the condition.  */
      pred = TREE_OPERAND (*expr_p, 0);
      simplify_expr (&pred, pre_p, NULL, is_simple_condexpr, fb_rvalue);

      /* Build the then clause, 't1 = a;'.  */
      t_then = build_modify_expr (tmp, NOP_EXPR, TREE_OPERAND (*expr_p, 1));
      simplify_stmt (&t_then);

      /* Build the else clause, 't1 = b;'.  */
      t_else = build_modify_expr (tmp, NOP_EXPR, TREE_OPERAND (*expr_p, 2));
      simplify_stmt (&t_else);

      /* Build a new COND_EXPR and insert it in the PRE_P chain.  */
      new_if = build (COND_EXPR, void_type_node, pred, t_then, t_else);
      add_tree (new_if, pre_p);

      /* Replace the original expression with the new temporary.  */
      *expr_p = tmp;
    }
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
  if_body = build_modify_expr (t, NOP_EXPR, rhs);

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

  if_stmt = build (COND_EXPR, void_type_node, if_cond, if_body,
		   empty_stmt_node);
  /* Simplify the IF_STMT and insert it in the PRE_P chain.  */
  simplify_stmt (&if_stmt);
  add_tree (if_stmt, pre_p);

  /* If we're not actually looking for a boolean result, convert now.  */
  if (TREE_TYPE (t) != TREE_TYPE (*expr_p))
     t = convert (TREE_TYPE (*expr_p), t);

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
  const char *file;
  int line, col;
  tree pre = NULL_TREE;
  tree post = NULL_TREE;
  
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

  pre = rationalize_compound_expr (pre);
  wrap_all_with_wfl (&pre, EXPR_WFL_FILENAME (*expr_p),
		     EXPR_WFL_LINENO (*expr_p));
  post = rationalize_compound_expr (post);
  wrap_all_with_wfl (&post, EXPR_WFL_FILENAME (*expr_p),
		     EXPR_WFL_LINENO (*expr_p));
  
  lineno = line;
  input_filename = file;

  add_tree (pre, pre_p);
  add_tree (post, post_p);

  if (EXPR_WFL_NODE (*expr_p) == NULL_TREE)
    *expr_p = NULL_TREE;
  else
    TREE_SIDE_EFFECTS (*expr_p) = TREE_SIDE_EFFECTS (EXPR_WFL_NODE (*expr_p));
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
    *expr_p = empty_stmt_node;
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
    {
      simplify_expr (&TREE_OPERAND (*expr_p, 0), pre_p, post_p, 
		     is_simple_addr_expr_arg, fb_lvalue);
      TREE_SIDE_EFFECTS (*expr_p)
	= TREE_SIDE_EFFECTS (TREE_OPERAND (*expr_p, 0));
    }
  else
    /* Fold &*EXPR into EXPR.  simplify_expr will re-simplify EXPR.  */
    *expr_p = TREE_OPERAND (TREE_OPERAND (*expr_p, 0), 0);
}

static void
simplify_asm_expr (expr_p, pre_p)
     tree *expr_p;
     tree *pre_p;
{
  /* punt */
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

/* Add STMT to EXISTING if possible, otherwise create a new
   COMPOUND_EXPR and add STMT to it. */

static tree
add_stmt_to_compound (existing, stmt)
     tree existing, stmt;
{
  if (!stmt || !expr_has_effect (stmt))
    return existing;
  else if (existing)
    return build (COMPOUND_EXPR, void_type_node, existing, stmt);
  else
    return stmt;
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
  if (t == NULL_TREE || !expr_has_effect (t))
    return NULL_TREE;

  *list_p = add_stmt_to_compound (*list_p, t);
  return t;
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
  if (vars != NULL_TREE)
    {
      STRIP_WFL (scope);

      if (TREE_CODE (scope) != BIND_EXPR)
	abort ();

      BIND_EXPR_VARS (scope) = chainon (BIND_EXPR_VARS (scope), vars);
    }

  return NULL_TREE;
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

#if 0
/* Similar to copy_tree_r() but do not copy SAVE_EXPR nodes.  These nodes
   model computations that should only be done once.  If we were to unshare
   something like SAVE_EXPR(i++), the simplification process would create
   wrong code.  */

static tree mostly_copy_tree_r       PARAMS ((tree *, int *, void *));
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
#endif

/* Callback for walk_tree to unshare most of the shared trees rooted at
   *TP.  If *TP has been visited already (i.e., TREE_VISITED (*TP) == 1),
   then *TP is deep copied by calling copy_tree_r.
   
   This unshares the same trees as copy_tree_r with the exception of
   SAVE_EXPR nodes.  These nodes model computations that should only be
   done once.  If we were to unshare something like SAVE_EXPR(i++), the
   simplification process would create wrong code.  */

static tree
copy_if_shared_r (tp, walk_subtrees, data)
    tree *tp;
    int *walk_subtrees ATTRIBUTE_UNUSED;
    void *data ATTRIBUTE_UNUSED;
{
  enum tree_code code = TREE_CODE (*tp);

  /* If this node has been visited already, unshare it and stop traversing.  */
  if (TREE_VISITED (*tp)
      /* Don't unshare decls, blocks, types and SAVE_EXPR nodes.  */
      && TREE_CODE_CLASS (code) != 't'
      && TREE_CODE_CLASS (code) != 'd'
      && TREE_CODE_CLASS (code) != 'b'
      && code != SAVE_EXPR)
    {
      walk_tree (tp, copy_tree_r, NULL, NULL);
      return *tp;
    }
  else
    {
      /* Otherwise, mark the tree as visited and keep looking.  */
      TREE_VISITED (*tp) = 1;
      return NULL_TREE;
    }
}

static tree
unmark_visited_r (tp, walk_subtrees, data)
    tree *tp;
    int *walk_subtrees ATTRIBUTE_UNUSED;
    void *data ATTRIBUTE_UNUSED;
{
  if (TREE_VISITED (*tp))
    {
      TREE_VISITED (*tp) = 0;
      return NULL_TREE;
    }
  else
    return *tp;
}


/* Unshare T and all the trees reached from T via TREE_CHAIN.  */

static void
unshare_all_trees (t)
     tree t;
{
  walk_tree (&t, copy_if_shared_r, NULL, NULL);
  walk_tree (&t, unmark_visited_r, NULL, NULL);
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

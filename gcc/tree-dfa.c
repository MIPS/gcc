/* Data flow functions for trees.
   Copyright (C) 2001 Free Software Foundation, Inc.
   Contributed by Diego Novillo <dnovillo@redhat.com>

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#include "config.h"
#include "system.h"
#include "tree.h"
#include "rtl.h"
#include "tm_p.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "output.h"
#include "errors.h"
#include "expr.h"
#include "c-common.h"
#include "ggc.h"
#include "tree-optimize.h"
#include "tree-flow.h"

/* {{{ Local declarations.  */

/* Dump file and flags.  */
static FILE *dump_file;
static int dump_flags;


/* Local functions.  */
static void find_refs_in_stmt PARAMS ((tree, basic_block));
static tree find_refs_in_stmt_expr PARAMS ((tree *, int *, void *));
static void find_refs_in_expr PARAMS ((tree, enum varref_type, basic_block,
				       tree, tree));
static varref_node create_node PARAMS ((varref));

/* }}} */

/* {{{ Global symbols.  */

/* List of all symbols referenced in the function.  */

tree ref_symbols_list;

/* }}} */


/* Find variable references in the code.  */

/* {{{ tree_find_varrefs()

   Look for variable references in every block of the flowgraph.  */

void
tree_find_varrefs ()
{
  int i;

  dump_file = dump_begin (TDI_ssa, &dump_flags);

  for (i = 0; i < n_basic_blocks; i++)
    {
      basic_block bb = BASIC_BLOCK (i);
      tree t = bb->head_tree;

      while (t)
	{
	  /* The only non-statements that we can have in a block are the
	     expressions in control header blocks (FOR_COND, DO_COND, etc).
	     Those are handled when we process the associated control
	     statement.  */
	  if (statement_code_p (TREE_CODE (t)))
	    {
	      find_refs_in_stmt (t, bb);
	      if (t == bb->end_tree || is_ctrl_stmt (t))
		break;
	    }

	  t = TREE_CHAIN (t);
	}
    }

  /* Debugging dumps.  */
  if (dump_file && (dump_flags & TDF_VARREF))
    {
      tree sym_n;

      fprintf (dump_file, ";; Function %s\n\n", 
	       IDENTIFIER_POINTER (DECL_NAME (current_function_decl)));

      fprintf (dump_file, "Symbols referenced:\n");

      for (sym_n = ref_symbols_list; sym_n; sym_n = TREE_CHAIN (sym_n))
	{
	  tree sym = TREE_VALUE (sym_n);
	  print_node_brief (dump_file, "\t", sym, 0);
	  fputc ('\n', dump_file);
	  dump_varref_list (dump_file, "\t", TREE_REFS (sym), 4, 1);
	  fputc ('\n', dump_file);
	}
    }

  if (dump_file)
    dump_end (TDI_ssa, dump_file);
}

/* }}} */

/* {{{ find_refs_in_stmt()

   Walks T looking for variable references.  BB is the basic block that
   contains T.  */

static void
find_refs_in_stmt (t, bb)
     tree t;
     basic_block bb;
{
  enum tree_code code;

  if (t == NULL)
    return;

  code = TREE_CODE (t);

  if (code == EXPR_STMT)
    find_refs_in_expr (EXPR_STMT_EXPR (t), VARUSE, bb, t, t);

  else if (code == IF_STMT)
    find_refs_in_expr (IF_COND (t), VARUSE, bb, t, t);

  else if (code == SWITCH_STMT)
    find_refs_in_expr (SWITCH_COND (t), VARUSE, bb, t, t);

  else if (code == FOR_STMT)
    {
      find_refs_in_stmt (FOR_INIT_STMT (t), bb);
      find_refs_in_expr (FOR_COND (t), VARUSE, bb, t, t);
      find_refs_in_expr (FOR_EXPR (t), VARUSE, bb, t, t);
    }

  else if (code == WHILE_STMT)
    find_refs_in_expr (WHILE_COND (t), VARUSE, bb, t, t);

  else if (code == DO_STMT)
    find_refs_in_expr (DO_COND (t), VARUSE, bb, t, t);

  else if (code == ASM_STMT)
    {
      find_refs_in_expr (ASM_INPUTS (t), VARUSE, bb, t, t);
      find_refs_in_expr (ASM_OUTPUTS (t), VARDEF, bb, t, t);
      find_refs_in_expr (ASM_CLOBBERS (t), VARDEF, bb, t, t);
    }

  else if (code == RETURN_STMT)
    find_refs_in_expr (RETURN_EXPR (t), VARUSE, bb, t, t);

  else if (code == GOTO_STMT)
    find_refs_in_expr (GOTO_DESTINATION (t), VARUSE, bb, t, t);

  else if (code == DECL_STMT)
    {
      if (TREE_CODE (DECL_STMT_DECL (t)) == VAR_DECL)
	find_refs_in_expr (DECL_INITIAL (DECL_STMT_DECL (t)), VARDEF, bb, t,
			   t);
    }

  else if (code == LABEL_STMT)
    find_refs_in_expr (LABEL_STMT_LABEL (t), VARUSE, bb, t, t);

  else if (code == CONTINUE_STMT)
    ;				/* Nothing to do.  */

  else if (code == CASE_LABEL)
    ;				/* Nothing to do.  */

  else if (code == BREAK_STMT)
    ;				/* Nothing to do.  */

  else if (code == COMPOUND_STMT)
    /* FIXME - This is needed for C/C++ statement-expressions.  These
       should have their own subgraphs.  We are now considering
       all these references as if they have been made inside bb.  */
    walk_stmt_tree (&t, find_refs_in_stmt_expr, (void *) bb);

  else if (code == SCOPE_STMT)
    ;				/* Nothing to do.  FIXME - This should not
				   be needed.  See the note for code ==
				   COMPOUND_STMT.  */

  else
    {
      prep_stmt (t);
      error ("Unhandled expression in find_refs_in_stmt():");
      fprintf (stderr, "\n");
      tree_debug_bb (bb);
      fprintf (stderr, "\n");
      debug_tree (t);
      fprintf (stderr, "\n");
      abort ();
    }
}

/* Temporary hack.  This should not be needed once we lower statement
   expressions and treat them as proper sub-graphs of the main CFG.  */

static tree
find_refs_in_stmt_expr (tp, walk_subtrees, data)
     tree *tp;
     int *walk_subtrees ATTRIBUTE_UNUSED;
     void *data;
{
  basic_block bb = (basic_block) data;
  tree t = *tp;

  if (t == NULL)
    return NULL;

  if (TREE_CODE (t) == COMPOUND_STMT)
    find_refs_in_stmt (COMPOUND_BODY (t), bb);
  else
    find_refs_in_stmt (t, bb);

  return NULL;
}

/* }}} */

/* {{{ find_refs_in_expr()

   Recursively scan the expression tree EXPR looking for variable
   references. REF_TYPE indicates what type of reference should be created,
   BB, PARENT_STMT and PARENT_EXPR are the block, statement and expression
   trees containing EXPR.  */

static void
find_refs_in_expr (expr, ref_type, bb, parent_stmt, parent_expr)
     tree expr;
     enum varref_type ref_type;
     basic_block bb;
     tree parent_stmt;
     tree parent_expr;
{
  enum tree_code code;

  if (expr == NULL)
    return;

  code = TREE_CODE (expr);

  if (code == VAR_DECL
      || code == FUNCTION_DECL
      || code == PARM_DECL
      || code == FIELD_DECL)
    {
      create_varref (expr, ref_type, bb, parent_stmt, parent_expr);
      return;
    }

  switch (code)
    {
      /* Unary expressions.  */
    case COMPLEX_CST:
    case INTEGER_CST:
    case LABEL_DECL:
    case REAL_CST:
    case RESULT_DECL:
    case STRING_CST:
      /* These trees make no memory references.  */
      break;

    case ABS_EXPR:
    case ADDR_EXPR:
    case CONJ_EXPR:
    case CONVERT_EXPR:
    case EXIT_EXPR:
    case FFS_EXPR:
    case FIX_CEIL_EXPR:
    case FIX_FLOOR_EXPR:
    case FIX_ROUND_EXPR:
    case FIX_TRUNC_EXPR:
    case FLOAT_EXPR:
    case IMAGPART_EXPR:
    case INDIRECT_REF:
    case NEGATE_EXPR:
    case NON_LVALUE_EXPR:
    case NOP_EXPR:
    case REALPART_EXPR:
    case REFERENCE_EXPR:
      find_refs_in_expr (TREE_OPERAND (expr, 0), VARUSE, bb, parent_stmt,
			 expr);
      break;

    case BIT_NOT_EXPR:
    case TRUTH_NOT_EXPR:
    case VA_ARG_EXPR:
      find_refs_in_expr (TREE_OPERAND (expr, 0), VARDEF, bb, parent_stmt,
			 expr);
      break;

    case POSTDECREMENT_EXPR:
    case POSTINCREMENT_EXPR:
    case PREDECREMENT_EXPR:
    case PREINCREMENT_EXPR:
      find_refs_in_expr (TREE_OPERAND (expr, 0), VARUSE, bb, parent_stmt,
			 expr);
      find_refs_in_expr (TREE_OPERAND (expr, 0), VARDEF, bb, parent_stmt,
			 expr);
      break;


      /* Binary expressions.  */
    case ARRAY_REF:
    case BIT_AND_EXPR:
    case BIT_ANDTC_EXPR:
    case BIT_IOR_EXPR:
    case BIT_XOR_EXPR:
    case CEIL_DIV_EXPR:
    case CEIL_MOD_EXPR:
    case COMPLEX_EXPR:
    case COMPONENT_REF:
    case COMPOUND_EXPR:
    case COND_EXPR:
    case CONSTRUCTOR:
    case EQ_EXPR:
    case EXACT_DIV_EXPR:
    case FLOOR_DIV_EXPR:
    case FLOOR_MOD_EXPR:
    case GE_EXPR:
    case GT_EXPR:
    case LE_EXPR:
    case LROTATE_EXPR:
    case LSHIFT_EXPR:
    case LT_EXPR:
    case MAX_EXPR:
    case MIN_EXPR:
    case MINUS_EXPR:
    case MULT_EXPR:
    case NE_EXPR:
    case ORDERED_EXPR:
    case PLUS_EXPR:
    case RDIV_EXPR:
    case ROUND_DIV_EXPR:
    case ROUND_MOD_EXPR:
    case RROTATE_EXPR:
    case RSHIFT_EXPR:
    case TRUNC_DIV_EXPR:
    case TRUNC_MOD_EXPR:
    case TRUTH_AND_EXPR:
    case TRUTH_ANDIF_EXPR:
    case TRUTH_OR_EXPR:
    case TRUTH_ORIF_EXPR:
    case TRUTH_XOR_EXPR:
    case UNEQ_EXPR:
    case UNGE_EXPR:
    case UNGT_EXPR:
    case UNLE_EXPR:
    case UNLT_EXPR:
    case UNORDERED_EXPR:
      find_refs_in_expr (TREE_OPERAND (expr, 0), VARUSE, bb, parent_stmt,
			 expr);
      find_refs_in_expr (TREE_OPERAND (expr, 1), VARUSE, bb, parent_stmt,
			 expr);
      break;

    case INIT_EXPR:
    case MODIFY_EXPR:
      find_refs_in_expr (TREE_OPERAND (expr, 1), VARUSE, bb, parent_stmt,
			 expr);
      find_refs_in_expr (TREE_OPERAND (expr, 0), VARDEF, bb, parent_stmt,
			 expr);
      break;


      /* Ternary operations.  */
    case BIT_FIELD_REF:
    case SAVE_EXPR:
      find_refs_in_expr (TREE_OPERAND (expr, 0), VARUSE, bb, parent_stmt,
			 expr);
      find_refs_in_expr (TREE_OPERAND (expr, 1), VARUSE, bb, parent_stmt,
			 expr);
      find_refs_in_expr (TREE_OPERAND (expr, 2), VARUSE, bb, parent_stmt,
			 expr);
      break;

      /* N-ary operations.  */
    case CALL_EXPR:
      {
	tree op;

	find_refs_in_expr (TREE_OPERAND (expr, 0), VARUSE, bb, parent_stmt,
			   expr);
	for (op = TREE_OPERAND (expr, 1); op; op = TREE_CHAIN (op))
	  find_refs_in_expr (TREE_VALUE (op), VARUSE, bb, parent_stmt, expr);
	break;
      }

    case TREE_LIST:
      {
	tree op;

	for (op = expr; op; op = TREE_CHAIN (op))
	  find_refs_in_expr (TREE_VALUE (op), ref_type, bb, parent_stmt,
			     expr);
	break;
      }


      /* C/C++ statement-expressions.  */
    case STMT_EXPR:
      find_refs_in_stmt (STMT_EXPR_STMT (expr), bb);
      break;

    default:
      {
	prep_stmt (parent_stmt);
	error ("Unhandled expression in find_refs_in_expr():");
	debug_tree (expr);
	fputs ("\n", stderr);
	abort ();
      }
    }
}

/* }}} */


/* Create references and associations to symbols and basic blocks.  */

/* {{{ create_varref()

   Create a new variable reference for symbol SYM and add it to the list
   of references for SYM and the basic block that holds the reference.
   REF_TYPE is the type of reference to create (def, use, phi, etc).
   BB, PARENT_STMT and PARENT_EXPR give the exact location of the
   reference.  */

varref
create_varref (sym, ref_type, bb, parent_stmt, parent_expr)
     tree sym;
     enum varref_type ref_type;
     basic_block bb;
     tree parent_stmt;
     tree parent_expr;
{
  varref ref;
  int is_new;

  if (bb == NULL)
    abort ();

  ref = (varref) ggc_alloc (sizeof (*ref));
  memset ((void *) ref, 0, sizeof (*ref));

  VARREF_SYM (ref) = sym;
  VARREF_TYPE (ref) = ref_type;
  VARREF_STMT (ref) = parent_stmt;
  VARREF_BB (ref) = bb;
  VARREF_EXPR (ref) = parent_expr;

  /* Add the symbol to the global list of referenced symbols.  On
     return, IS_NEW will be 1 if this is the first time we see this
     symbol.  */
  is_new = add_ref_symbol (sym, &ref_symbols_list);

  /* Add this reference to the list of references for the symbol.  */
  add_ref_to_sym (ref, sym, is_new);

  /* Add this reference to the list of references for the basic block.  */
  add_ref_to_bb (ref, bb);

  return ref;
}

/* }}} */

/* {{{ add_ref_to_sym()

   Adds reference REF to the list of references made to symbol SYM.  */

void
add_ref_to_sym (ref, sym, is_new)
     varref ref;
     tree sym;
     int is_new;
{
  tree_ann ann = TREE_ANN (sym);
  varref_list ref_list = TREE_REFS (sym);

  if (ann && ref_list)
    {
      /* Sanity check.  If the symbol has annotations but we think
         it's new, then something is wrong.  */
      if (is_new)
	abort ();

      push_ref (ref_list, ref);
    }
  else
    {
      /* Similar sanity check.  If the symbol has no annotations nor
         references, there is something wrong.  */
      if (!is_new && TREE_ANN (sym) == NULL)
	abort ();

      get_tree_ann (sym)->refs = create_varref_list (ref);
    }
}

/* }}} */

/* {{{ add_ref_to_bb()

   Adds reference REF to the list of references made in basic block BB.  */

void
add_ref_to_bb (ref, bb)
     varref ref;
     basic_block bb;
{
  if (BB_ANN (bb) && BB_REFS (bb))
    push_ref (BB_REFS (bb), ref);
  else
    get_bb_ann (bb)->refs = create_varref_list (ref);
}

/* }}} */

/* {{{ add_ref_symbol()

   Adds a unique copy of symbol SYM to the list of symbols LISTP.  */

int
add_ref_symbol (sym, listp)
     tree sym;
     tree *listp;
{
  int is_new = 0;
  tree node = build_tree_list (NULL_TREE, sym);

  if (*listp == NULL_TREE)
    {
      *listp = node;
      is_new = 1;
    }
  else
    {
      tree last;
      tree list = *listp;

      if (!chain_member_value (sym, list))
	{
	  /* Add a new symbol to the list.  */
	  last = tree_last (list);
	  TREE_CHAIN (last) = node;
	  is_new = 1;
	}
    }

  return is_new;
}

/* }}} */

/* {{{ remove_ann_from_sym()

   Clear the annotations made to symbol SYM.  */

void
remove_ann_from_sym (sym)
     tree sym;
{
  tree_ann ann = TREE_ANN (sym);

  if (ann)
    {
      ann->bb = NULL;
      ann->refs = NULL;
      ann->currdef = NULL;
      sym->common.aux = NULL;
    }
}

/* }}} */


/* Manage annotations and lists of references.  */

/* {{{ get_tree_ann()

   Get the annotation for the given tree. Create a new one if necessary.  */

tree_ann
get_tree_ann (t)
     tree t;
{
  tree_ann ann = TREE_ANN (t);

  if (ann == NULL)
    {
      ann = (tree_ann) ggc_alloc (sizeof (*ann));
      memset ((void *) ann, 0, sizeof (*ann));
      t->common.aux = (void *) ann;
    }

  return ann;
}

/* }}} */

/* {{{ create_varref_list()

   Create a new list of variable references.  */

varref_list
create_varref_list (ref)
     varref ref;
{
  varref_list list;
  varref_node node;

  if (ref == NULL)
    abort ();

  list = (varref_list) ggc_alloc (sizeof (*list));
  memset ((void *) list, 0, sizeof (*list));

  node = create_node (ref);

  list->first = node;
  list->last = node;

  return list;
}

/* }}} */

/* {{{ push_ref()

   Push a variable reference to the end of the list.  */

void
push_ref (list, ref)
     varref_list list;
     varref ref;
{
  varref_node node, last;

  if (ref == NULL || list == NULL)
    abort ();

  node = create_node (ref);

  last = VARREF_LIST_LAST (list);
  last->next = node;
  node->prev = last;
  list->last = node;
}

/* }}} */

/* {{{ create_node()

   Create a node holder for a varref object.  */

static varref_node
create_node (ref)
     varref ref;
{
  varref_node node;

  if (ref == NULL)
    abort ();

  node = (varref_node) ggc_alloc (sizeof (*node));
  memset ((void *) node, 0, sizeof (*node));

  node->elem = ref;

  return node;
}

/* }}} */

/* {{{ delete_varref_list()

   Delete all the references to the symbols in *SYMLIST_P.  */

void
delete_varref_list (symlist_p)
     tree *symlist_p;
{
  tree sym_n;
  tree symlist;

  if (symlist_p == NULL)
    abort ();

  symlist = *symlist_p;

  for (sym_n = symlist; sym_n; sym_n = TREE_CHAIN (sym_n))
    {
      tree sym = TREE_VALUE (sym_n);
      remove_ann_from_sym (sym);
    }

  *symlist_p = NULL;
}

/* }}} */


/* Debugging functions.  */

/* {{{ dump_varref()

   Display variable reference REF on stream OUTF. PREFIX is a string that
   is prefixed to every line of output, and INDENT is the amount of left
   margin to leave.  If DETAILS is non-zero, the output is more verbose.  */

void
dump_varref (outf, prefix, ref, indent, details)
     FILE *outf;
     const char *prefix;
     varref ref;
     int indent;
     int details;
{
  int lineno, bbix;
  const char *sym, *type;
  char *s_indent;

  if (ref == NULL)
    return;

  s_indent = (char *) alloca ((size_t) indent + 1);
  memset ((void *) s_indent, ' ', (size_t) indent);
  s_indent[indent] = '\0';

  lineno = (VARREF_STMT (ref)) ? STMT_LINENO (VARREF_STMT (ref)) : -1;

  sym = (VARREF_SYM (ref))
    ? IDENTIFIER_POINTER (DECL_NAME (VARREF_SYM (ref))) : "nil";

  bbix = (VARREF_BB (ref)) ? VARREF_BB (ref)->index : -1;

  type = (VARREF_TYPE (ref) == VARDEF) ? "DEF" :
         (VARREF_TYPE (ref) == VARUSE) ? "USE" :
	 (VARREF_TYPE (ref) == VARPHI) ? "PHI" :
	 "???";

  fprintf (outf, "%s%s%s(%s): line %d, bb %d, ", s_indent, prefix, type,
	   sym, lineno, bbix);

  print_node_brief (outf, "", VARREF_EXPR (ref), 0);

  /* Dump specific contents for the different types of references.  */

  if (details)
    {
      if (VARREF_TYPE (ref) == VARPHI && VARPHI_CHAIN (ref))
	{
	  fputs (" phi-args:\n", outf);
	  dump_varref_list (outf, prefix, VARPHI_CHAIN (ref), indent + 4, 0);
	}
      else if (VARREF_TYPE (ref) == VARDEF && VARDEF_IMM_USES (ref))
	{
	  fputs (" immediate uses:\n", outf);
	  dump_varref_list (outf, prefix, VARDEF_IMM_USES (ref), indent + 4,
			    0);
	}
      else if (VARREF_TYPE (ref) == VARUSE && VARUSE_CHAIN (ref))
	{
	  fputs (" reaching def:\n", outf);
	  dump_varref (outf, prefix, VARUSE_CHAIN (ref), indent + 4, 0);
	}
    }

  fputc ('\n', outf);
  if (VARREF_EXPR (ref) == NULL)
    fputc ('\n', outf);
}

/* }}} */

/* {{{ debug_varref()

   Display variable reference REF on stderr.  */

void
debug_varref (ref)
     varref ref;
{
  dump_varref (stderr, "", ref, 0, 1);
}

/* }}} */

/* {{{ dump_varref_list()

   Display a list of variable references on stream OUTF. PREFIX is a string
   that is prefixed to every line of output, and INDENT is the amount of
   left margin to leave.  If DETAILS is non-zero, the output is more
   verbose.  */

void
dump_varref_list (outf, prefix, reflist, indent, details)
     FILE *outf;
     const char *prefix;
     varref_list reflist;
     int indent;
     int details;
{
  varref_node v;

  for (v = VARREF_LIST_FIRST (reflist); v; v = VARREF_NODE_NEXT (v))
    dump_varref (outf, prefix, VARREF_NODE_ELEM (v), indent, details);
}

/* }}} */

/* {{{ debug_varref_list()

   Dump REFLIST on stderr.  */

void
debug_varref_list (reflist)
     varref_list reflist;
{
  dump_varref_list (stderr, "", reflist, 0, 1);
}

/* }}} */

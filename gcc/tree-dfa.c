/* {{{ Data flow functions for trees.
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

/* Make sure that structures shared with the RTL optimizer use trees
   instead of rtx.  */
#define USE_TREE_IL 1

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
#include "tree-opt.h"
#include "tree-flow.h"

/* }}} */

/* {{{ Debugging macros.  */
#define DEBUG_TREE_DFA	/* Force debugging.  */

#ifdef DEBUG_TREE_DFA
#undef DEBUG_TREE_DFA
#define DEBUG_TREE_DFA(level) debug_tree_dfa (level, __FILE__, __LINE__)

static int debug_tree_dfa PARAMS ((int, const char *, int));

static int
debug_tree_dfa (level, filename, line)
    int level;
    const char *filename;
    int line;
{
  char *trigger = getenv ("DEBUG_TREE_DFA");
  if (trigger && atoi (trigger) >= level)
    {
      if (atoi (trigger) > level)
	{
	  fputs ("\n----------------------------------------------------------------------------\n\n", stderr);
	  fprintf (stderr, "%s:%d\n", filename, line);
	}

      return 1;
    }

  return 0;

}
#endif	/* DEBUG_TREE_DFA  */

/* }}} */

/* {{{ Local declarations.  */

static tree find_refs PARAMS ((tree *, int *, void *));
static void find_refs_in_expr PARAMS ((tree, enum varref_type, tree, tree));
static varref_node create_node PARAMS ((varref));

/* }}} */

/* {{{ Global symbols.  */

/* List of all symbols referenced in the function.  */

tree ref_symbols_list;

/* }}} */

/* Find variable references in the code.  */

/* {{{ tree_find_varrefs()

   Main entry point. Walks T looking for variable references.  PARENT_STMT
   is the statement that contains T (if any).  This is used to handle
   expression statements, properly.  Specifically, we need to keep track
   of which basic block holds the statement so that we can associate
   variable references to it.  */

void
tree_find_varrefs (t, parent_stmt)
    tree t;
    tree parent_stmt;
{
  walk_stmt_tree (&t, find_refs, (void *)parent_stmt);

#ifdef DEBUG_TREE_DFA
  if (DEBUG_TREE_DFA (1))
    {
      tree sym_n;

      fputc ('\n', stderr);
      fprintf (stderr, "## Function %s\n\n",
	       IDENTIFIER_POINTER (DECL_NAME (current_function_decl)));

      fputs ("Symbols referenced:\n", stderr);
      for (sym_n = ref_symbols_list; sym_n; sym_n = TREE_CHAIN (sym_n))
	{
	  varref_node i;
	  tree sym = TREE_VALUE (sym_n);
	  print_node_brief (stderr, "\t", sym, 0);
	  fputc ('\n', stderr);

	  for (i = VARREF_LIST_FIRST (TREE_REFS (sym)); i; i = VARREF_NODE_NEXT (i))
	    {
	      varref ref = VARREF_NODE_ELEM (i);
	      fputs ("\t    ", stderr);
	      debug_varref (ref);
	    }
	  fputc ('\n', stderr);
	}
      fputc ('\n', stderr);
    }
#endif
}

/* }}} */

/* {{{ find_refs()

   Callback for walk_stmt_tree().  */

static tree
find_refs (tp, walk_subtrees, data)
    tree *tp;
    int *walk_subtrees ATTRIBUTE_UNUSED;
    void *data;
{
  enum tree_code code;
  tree t = *tp;

  /* If T is contained within another tree, then use the parent tree
     when calling find_refs_in_expr().  This is needed so that we can
     obtain the basic block associated with T. This information is
     stored in T's parent statement.  */
  tree parent_stmt = (data) ? (tree)data : t;
  
  if (t == NULL)
    return NULL_TREE;

  code = TREE_CODE (t);

  if (code == EXPR_STMT)
    find_refs_in_expr (EXPR_STMT_EXPR (t), VARUSE, parent_stmt, parent_stmt);

  else if (code == IF_STMT)
    find_refs_in_expr (IF_COND (t), VARUSE, parent_stmt, parent_stmt);

  else if (code == SWITCH_STMT)
    find_refs_in_expr (SWITCH_COND (t), VARUSE, parent_stmt, parent_stmt);

  else if (code == FOR_STMT)
    {
      find_refs_in_expr (FOR_COND (t), VARUSE, parent_stmt, parent_stmt);
      find_refs_in_expr (FOR_EXPR (t), VARUSE, parent_stmt, parent_stmt);
    }

  else if (code == WHILE_STMT)
    find_refs_in_expr (WHILE_COND (t), VARUSE, parent_stmt, parent_stmt);

  else if (code == DO_STMT)
    find_refs_in_expr (DO_COND (t), VARUSE, parent_stmt, parent_stmt);

  else if (code == ASM_STMT)
    {
      find_refs_in_expr (ASM_INPUTS (t), VARUSE, parent_stmt, parent_stmt);
      find_refs_in_expr (ASM_OUTPUTS (t), VARDEF, parent_stmt, parent_stmt);
      find_refs_in_expr (ASM_CLOBBERS (t), VARDEF, parent_stmt, parent_stmt);
    }

  else if (code == RETURN_STMT)
    find_refs_in_expr (RETURN_EXPR (t), VARUSE, parent_stmt, parent_stmt);

  else if (code == GOTO_STMT)
    find_refs_in_expr (GOTO_DESTINATION (t), VARUSE, parent_stmt, parent_stmt);

  return NULL_TREE;
}
/* }}} */

/* {{{ find_refs_in_expr()

   Recursively scan the expression tree EXPR looking for variable
   references. REF_TYPE indicates what type of reference should be created,
   PARENT_STMT and PARENT_EXPR are the statement and expression trees
   containing EXPR.  The basic block holding EXPR is the one associated
   with PARENT_STMT.  */

static void
find_refs_in_expr (expr, ref_type, parent_stmt, parent_expr)
    tree expr;
    enum varref_type ref_type;
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
      varref ref = create_varref (expr, ref_type, BB_FOR_STMT (parent_stmt),
				  parent_stmt, parent_expr);

#ifdef DEBUG_TREE_DFA
      if (DEBUG_TREE_DFA (2))
	debug_varref (ref);
#endif

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
      find_refs_in_expr (TREE_OPERAND (expr, 0), VARUSE, parent_stmt, expr);
      break;

    case BIT_NOT_EXPR:
    case TRUTH_NOT_EXPR:
    case VA_ARG_EXPR:
      find_refs_in_expr (TREE_OPERAND (expr, 0), VARDEF, parent_stmt, expr);
      break;

    case POSTDECREMENT_EXPR:
    case POSTINCREMENT_EXPR:
    case PREDECREMENT_EXPR:
    case PREINCREMENT_EXPR:
      find_refs_in_expr (TREE_OPERAND (expr, 0), VARUSE, parent_stmt, expr);
      find_refs_in_expr (TREE_OPERAND (expr, 0), VARDEF, parent_stmt, expr);
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
      find_refs_in_expr (TREE_OPERAND (expr, 0), VARUSE, parent_stmt, expr);
      find_refs_in_expr (TREE_OPERAND (expr, 1), VARUSE, parent_stmt, expr);
      break;

    case INIT_EXPR:
    case MODIFY_EXPR:
      find_refs_in_expr (TREE_OPERAND (expr, 0), VARDEF, parent_stmt, expr);
      find_refs_in_expr (TREE_OPERAND (expr, 1), VARUSE, parent_stmt, expr);
      break;


    /* Ternary operations.  */
    case BIT_FIELD_REF:
    case SAVE_EXPR:
      find_refs_in_expr (TREE_OPERAND (expr, 0), VARUSE, parent_stmt, expr);
      find_refs_in_expr (TREE_OPERAND (expr, 1), VARUSE, parent_stmt, expr);
      find_refs_in_expr (TREE_OPERAND (expr, 2), VARUSE, parent_stmt, expr);
      break;

    /* N-ary operations.  */
    case CALL_EXPR:
      {
	tree op;

	find_refs_in_expr (TREE_OPERAND (expr, 0), VARUSE, parent_stmt, expr);
	for (op = TREE_OPERAND (expr, 1); op; op = TREE_CHAIN (op))
	  find_refs_in_expr (TREE_VALUE (op), VARUSE, parent_stmt, expr);
	break;
      }

    case TREE_LIST:
      {
	tree op;

	for (op = expr; op; op = TREE_CHAIN (op))
	  find_refs_in_expr (TREE_VALUE (op), ref_type, parent_stmt, expr);
	break;
      }


    /* C/C++ statement-expressions.  */
    case STMT_EXPR:
      tree_find_varrefs (STMT_EXPR_STMT (expr), parent_stmt);
      break;

    default:
      {
	prep_stmt (parent_stmt);
	error ("Unhandled expression in find_refs_in_expr():");
	debug_tree (expr);
	fputs ("\n", stderr);
	abort();
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
    {
      fputs ("Cannot determine basic block for variable reference:\n", stderr);
      debug_tree (sym);

      fputs ("\n\nReference made in expression:\n", stderr);
      debug_tree (parent_expr);

      fputs ("\n\nWhich is inside statement:\n", stderr);
      debug_tree (parent_stmt);

      fputs ("\n", stderr);
      abort ();
    }

  ref = (varref) ggc_alloc (sizeof (*ref));
  memset ((void *)ref, 0, sizeof (*ref));
  
  VARREF_SYM (ref) = sym;
  VARREF_TYPE (ref) = ref_type;
  VARREF_STMT (ref) = parent_stmt;
  VARREF_BLOCK (ref) = bb;
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
	abort();

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

      if (! chain_member_value (sym, list))
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
      memset ((void *)ann, 0, sizeof (*ann));
      t->common.aux = (void *)ann;
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
  memset ((void *)list, 0, sizeof (*list));

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
  memset ((void *)node, 0, sizeof (*node));

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

  s_indent = (char *) alloca ((size_t)indent + 1);
  memset ((void *)s_indent, ' ', (size_t)indent);
  s_indent[indent] = '\0';

  lineno = (VARREF_STMT (ref)) 
	   ? STMT_LINENO (VARREF_STMT (ref))
	   : -1;

  sym = (VARREF_SYM (ref)) 
	? IDENTIFIER_POINTER (DECL_NAME (VARREF_SYM (ref)))
	: "nil";

  bbix = (VARREF_BLOCK (ref)) 
	 ? VARREF_BLOCK (ref)->index
	 : -1;

  type = (VARREF_TYPE (ref) == VARDEF)
	 ? "DEF"
	 : (VARREF_TYPE (ref) == VARUSE)
	   ? "USE"
	   : (VARREF_TYPE (ref) == VARPHI)
	     ? "PHI"
	     : "???";
  
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
	  dump_varref_list (outf, prefix, VARDEF_IMM_USES (ref), indent + 4, 0);
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

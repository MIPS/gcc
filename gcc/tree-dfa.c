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
#include "ggc.h"
#include "tree-optimize.h"
#include "tree-flow.h"

/* This should be eventually be generalized to other languages, but
   this would require a shared function-as-trees infrastructure.  */
#include "c-common.h"

/* Local declarations.  */

/* Dump file and flags.  */
static FILE *dump_file;
static int dump_flags;

/* Local functions.  */
static void find_refs_in_stmt PARAMS ((tree, basic_block));
static void find_refs_in_expr PARAMS ((tree, enum treeref_type, basic_block,
				       tree, tree));
static void create_tree_ann PARAMS ((tree));
static void add_ref_symbol PARAMS ((tree));


/* Global declarations.  */

/* Array of all symbols referenced in the function.  */

varray_type referenced_symbols;


/* Find variable references in the code.  */

/*  Look for variable references in every block of the flowgraph.  */

void
tree_find_varrefs ()
{
  basic_block bb;

  FOR_EACH_BB (bb)
    {
      tree t = bb->head_tree;

      while (t)
	{
	  if (statement_code_p (TREE_CODE (t)))
	    {
	      find_refs_in_stmt (t, bb);
	      if (t == bb->end_tree || is_ctrl_stmt (t))
		break;
	    }
	  else
	    {
	      tree parent = BB_PARENT (bb)->head_tree;
	      find_refs_in_expr (t, VARUSE, bb, parent, t);
	    }

	  t = TREE_CHAIN (t);
	}
    }

  /* Debugging dumps.  */
  dump_file = dump_begin (TDI_ssa, &dump_flags);

  if (dump_file && (dump_flags & TDF_REFS))
    {
      size_t i;

      fprintf (dump_file, ";; Function %s\n\n",
	       IDENTIFIER_POINTER (DECL_NAME (current_function_decl)));

      fprintf (dump_file, "Symbols referenced:\n");

      for (i = 0; i < NREF_SYMBOLS; i++)
	{
	  tree sym = REF_SYMBOL (i);
	  print_node_brief (dump_file, "\t", sym, 0);
	  fputc ('\n', dump_file);
	  dump_varref_list (dump_file, "\t", TREE_REFS (sym), 4, 1);
	  fputc ('\n', dump_file);
	}

      dump_end (TDI_ssa, dump_file);
    }
}


/*  Walks T looking for variable references.  BB is the basic block that
    contains T.  */

static void
find_refs_in_stmt (t, bb)
     tree t;
     basic_block bb;
{
  enum tree_code code;

  if (t == NULL || t == error_mark_node)
    return;

  code = TREE_CODE (t);
  switch (code)
    {
    case EXPR_STMT:
      find_refs_in_expr (EXPR_STMT_EXPR (t), VARUSE, bb, t, t);
      break;

      /* The condition nodes for IF_STMT, SWITCH_STMT and WHILE_STMT are not
	 modeled in the flowgraph, so they need to be looked at separately.  */
    case IF_STMT:
      find_refs_in_expr (IF_COND (t), VARUSE, bb, t, t);
      break;
      
    case SWITCH_STMT:
      find_refs_in_expr (SWITCH_COND (t), VARUSE, bb, t, t);
      break;

    case WHILE_STMT:
      find_refs_in_expr (WHILE_COND (t), VARUSE, bb, t, t);
      break;

      /* There is no need to check the children nodes for DO_STMTs and
	 FOR_STMTs, because they're in separate basic blocks.  */
    case FOR_STMT:
    case DO_STMT:
      break;	                        /* Nothing to do.  */

    case ASM_STMT:
      find_refs_in_expr (ASM_INPUTS (t), VARUSE, bb, t, t);
      find_refs_in_expr (ASM_OUTPUTS (t), VARDEF, bb, t, t);
      find_refs_in_expr (ASM_CLOBBERS (t), VARDEF, bb, t, t);
      break;

    case RETURN_STMT:
      find_refs_in_expr (RETURN_EXPR (t), VARUSE, bb, t, t);
      break;

    case GOTO_STMT:
      find_refs_in_expr (GOTO_DESTINATION (t), VARUSE, bb, t, t);
      break;

    case DECL_STMT:
      if (TREE_CODE (DECL_STMT_DECL (t)) == VAR_DECL)
	{
	  tree decl = DECL_STMT_DECL (t);
	  if (DECL_INITIAL (decl))
	    find_refs_in_expr (decl, VARDEF, bb, t, t);
	}
      break;

    case LABEL_STMT:
      find_refs_in_expr (LABEL_STMT_LABEL (t), VARUSE, bb, t, t);
      break;

    case STMT_EXPR:
      find_refs_in_stmt (STMT_EXPR_STMT (t), bb);
      break;

    case CONTINUE_STMT:
    case CASE_LABEL:
    case BREAK_STMT:
    case COMPOUND_STMT:
    case SCOPE_STMT:
    case FILE_STMT:
      break;				/* Nothing to do.  */

    default:
      {
	prep_stmt (t);
	error ("unhandled statement node in find_refs_in_stmt():");
	fprintf (stderr, "\n");
	tree_debug_bb (bb);
	fprintf (stderr, "\n");
	debug_tree (t);
	fprintf (stderr, "\n");
	abort ();
      }
    }
}


/*  Recursively scan the expression tree EXPR looking for variable
    references.
   
    REF_TYPE indicates what type of reference should be created.

    BB, PARENT_STMT and PARENT_EXPR are the block, statement and expression
      trees containing EXPR.  */

static void
find_refs_in_expr (expr, ref_type, bb, parent_stmt, parent_expr)
     tree expr;
     enum treeref_type ref_type;
     basic_block bb;
     tree parent_stmt;
     tree parent_expr;
{
  enum tree_code code;

  if (expr == NULL || expr == error_mark_node)
    return;

  code = TREE_CODE (expr);

  if (code == VAR_DECL
      || code == FUNCTION_DECL
      || code == PARM_DECL
      || code == FIELD_DECL)
    {
      create_ref (expr, ref_type, bb, parent_stmt, parent_expr);
      return;
    }

  switch (code)
    {
      /* Structure and union references.  Use the same reference type that
	 was given by our caller.  */
    case COMPONENT_REF:
      find_refs_in_expr (TREE_OPERAND (expr, 0), ref_type, bb, parent_stmt,
			 expr);
      find_refs_in_expr (TREE_OPERAND (expr, 1), ref_type, bb, parent_stmt,
			 expr);
      break;

      /* Array references.  Use the same reference type for the array, but
	 default to VARUSE for the index expression.  */
    case ARRAY_REF:
      find_refs_in_expr (TREE_OPERAND (expr, 0), ref_type, bb, parent_stmt,
			 expr);
      find_refs_in_expr (TREE_OPERAND (expr, 1), VARUSE, bb, parent_stmt,
			 expr);
      break;

      /* Unary expressions.  */
    case COMPLEX_CST:
    case INTEGER_CST:
    case LABEL_DECL:
    case REAL_CST:
    case RESULT_DECL:
    case STRING_CST:
    case ADDR_EXPR:
      /* These trees make no memory references.  */
      break;

    case ABS_EXPR:
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
    case BIT_AND_EXPR:
    case BIT_ANDTC_EXPR:
    case BIT_IOR_EXPR:
    case BIT_XOR_EXPR:
    case CEIL_DIV_EXPR:
    case CEIL_MOD_EXPR:
    case COMPLEX_EXPR:
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
      find_refs_in_expr (TREE_OPERAND (expr, 0), ref_type, bb, parent_stmt,
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
	tree addr = TREE_OPERAND (expr, 0);
	tree decl;

	if (TREE_CODE (addr) == ADDR_EXPR)
	  decl = TREE_OPERAND (addr, 0);
	else
	  decl = addr;

	find_refs_in_expr (decl, VARUSE, bb, parent_stmt, expr);
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

      /* File and line number annotations.  */
    case EXPR_WITH_FILE_LOCATION:
      find_refs_in_expr (TREE_OPERAND (expr, 0), ref_type, bb, parent_stmt,
	                 expr);
      break;

    case COMPOUND_LITERAL_EXPR:
      break;

    default:
      {
	prep_stmt (parent_stmt);
	error ("unhandled expression in find_refs_in_expr():");
	debug_tree (expr);
	fputs ("\n", stderr);
	abort ();
      }
    }
}

/* Create and return an empty list of references.  */

ref_list
create_ref_list (void)
{
  ref_list list = xmalloc (sizeof (struct ref_list_priv));
  list->first = list->last = NULL;
  return list;
}

/* Free the nodes in LIST, but keep the empty list around.  
   (IE empty the list).  */
void 
empty_ref_list (list)
     ref_list list;
{
  struct ref_list_node *node;

  for (node = list->first; node; )
    {
      struct ref_list_node *tmp;
      tmp = node;
      node = node->next;
      free (tmp);
    }
  list->first = list->last = NULL;
}

/* Delete LIST, including the list itself.
   (IE destroy the list).  */
void
delete_ref_list (list)
     ref_list list;
{
  struct ref_list_node *node;

  for (node = list->first; node; )
    {
      struct ref_list_node *tmp;
      tmp = node;
      node = node->next;
      free (tmp);
    }
  free (list);
}

/* Remove REF from LIST.  */

void 
remove_ref_from_list (list, ref)
     ref_list list;
     varref ref;
{
  struct ref_list_node *tmp;
  if (!list || !list->first || !list->last)
    return;
  for (tmp = list->first; tmp; tmp = tmp->next)
    {
      if (tmp->ref == ref)
	{
	  if (tmp == list->first)
	    list->first = tmp->next;
	  if (tmp == list->last)
	    list->last = tmp->prev;
	  if (tmp->next)
	    tmp->next->prev = tmp->prev;
	  if (tmp->prev)
	    tmp->prev->next = tmp->next;
	  free (tmp);
	  return;
	}
    }
}

/* Add REF to the beginning of LIST.  */

void
add_ref_to_list_begin (list, ref)
     ref_list list;
     varref ref;
{
  struct ref_list_node *node = xmalloc (sizeof (struct ref_list_node));
  node->ref = ref;
  if (list->first == NULL)
    {
      node->prev = node->next = NULL;
      list->first = list->last = node;
      return;
    }
  node->prev = NULL;
  node->next = list->first;
  list->first->prev = node;
  list->first = node;
}

/* Add REF to the end of LIST.  */

void
add_ref_to_list_end (list, ref)
     ref_list list;
     varref ref;
{
  struct ref_list_node *node = xmalloc (sizeof (struct ref_list_node));
  node->ref = ref;
  if (list->first == NULL)
    {
      node->prev = node->next = NULL;
      list->first = list->last = node;
      return;
    }
  node->prev = list->last;
  node->next = NULL;
  list->last->next = node;
  list->last = node;
}

/* Create references and associations to symbols and basic blocks.  */

/*  Create a new variable reference for symbol SYM and add it to the list
    of references for SYM and the basic block that holds the reference.

    REF_TYPE is the type of reference to create (def, use, phi, etc).
    
    BB, PARENT_STMT and PARENT_EXPR give the exact location of the
      reference.  */

varref
create_ref (sym, ref_type, bb, parent_stmt, parent_expr)
     tree sym;
     enum treeref_type ref_type;
     basic_block bb;
     tree parent_stmt;
     tree parent_expr;
{
  varref ref;

  if (bb == NULL)
    abort ();

  ref = (varref) ggc_alloc (sizeof (*ref));
  memset ((void *) ref, 0, sizeof (*ref));

  VARREF_SYM (ref) = sym;
  VARREF_TYPE (ref) = ref_type;
  VARREF_STMT (ref) = parent_stmt;
  VARREF_BB (ref) = bb;
  VARREF_EXPR (ref) = parent_expr;

  /* Create containers according to the type of reference.  */
  if (ref_type == VARDEF || ref_type == VARPHI)
    {
      VARDEF_IMM_USES (ref) = create_ref_list ();
      VARDEF_RUSES (ref) = create_ref_list ();
      if (ref_type == VARPHI)
	{
	  VARRAY_GENERIC_PTR_INIT (VARDEF_PHI_CHAIN (ref), 3, "phi_chain");
	  VARRAY_BB_INIT (VARDEF_PHI_CHAIN_BB (ref), 3, "phi_chain_bb");
	}
    }
  else if (ref_type == VARUSE)
    VARUSE_RDEFS (ref) = create_ref_list ();
  else if (ref_type == EXPRPHI)
    {
      VARRAY_GENERIC_PTR_INIT (EXPRPHI_PHI_CHAIN (ref), 
			       last_basic_block, "ephi_chain");
      EXPRPHI_PROCESSED (ref) = BITMAP_XMALLOC ();
      EXPRPHI_DOWNSAFE (ref) = 1;
      EXPRPHI_CANBEAVAIL (ref) = 1;
      EXPRPHI_LATER (ref) = 1;
      EXPRPHI_EXTRANEOUS (ref) = 1;
    }
  if (sym)
    {
      /* Add the symbol to the list of symbols referenced in this function.  */
      add_ref_symbol (sym);
      
      /* Add this reference to the list of references for the symbol.  */
      add_ref_to_list_end (get_tree_ann (sym)->refs, ref);
      
      /* Add this reference to the list of references for the containing
	 statement.  */
      if (parent_stmt)
	add_ref_to_list_end (get_tree_ann (parent_stmt)->refs, ref);

    }

  /* Add this reference to the list of references for the basic
     block. */
  /* Make sure that PHI terms are added at the beginning of the list,
     otherwise FUD chaining will fail to link local uses to the PHI
     term in this basic block.  */
  if (ref_type == VARPHI || ref_type == EXPRPHI)
    add_ref_to_list_begin (get_bb_ann (bb)->refs, ref);
  else
    add_ref_to_list_end (get_bb_ann (bb)->refs, ref);

  return ref;
}


/*  Adds a unique copy of symbol SYM to the list of referenced symbols.  */

static void
add_ref_symbol (sym)
     tree sym;
{
  size_t i;

  /* Look for the SYM in the array of symbols referenced in the function.  */
  for (i = 0; i < NREF_SYMBOLS; i++)
    if (REF_SYMBOL (i) == sym)
      return;

  /* We didn't find the symbol.  Add it to the list and create a new
     annotation for the symbol.  */
  ADD_REF_SYMBOL (sym);
  create_tree_ann (sym);
}



/* Manage annotations.  */

/*  Get the annotation for the given tree.  Create a new one if necessary.  */

tree_ann
get_tree_ann (t)
     tree t;
{
  if (TREE_ANN (t) == NULL)
    create_tree_ann (t);

  return TREE_ANN (t);
}


/*  Create a new annotation for tree T.  */

static void
create_tree_ann (t)
     tree t;
{
  tree_ann ann = (tree_ann) ggc_alloc (sizeof (*ann));
  memset ((void *) ann, 0, sizeof (*ann));
  ann->refs = create_ref_list ();
  t->common.aux = (void *) ann;
}



/* Miscellaneous helpers.  */

/*  Return 1 if the function may call itself.
   
   ??? Currently this is very limited because we do not have call-graph
       information.  */

int
function_may_recurse_p ()
{
  basic_block bb;

  /* If we only make calls to pure and/or builtin functions, then the
     function is not recursive.  */
  FOR_EACH_BB (bb)
    {
      struct ref_list_node *tmp;
      varref ref;

      FOR_EACH_REF (ref, tmp, BB_REFS (bb))
	{
	  tree fcall = VARREF_SYM (ref);

	  if (fcall == current_function_decl
	      || (TREE_CODE (fcall) == FUNCTION_DECL
		  && ! DECL_IS_PURE (fcall)
		  && ! DECL_BUILT_IN (fcall)))
	    return 1;
	}
    }

  return 0;
}


/*  Push into variable array *FCALLS_P all the function call references made
    in this function.

    WHICH is a bitmask specifying the type of function call that the caller
      is interested in (see tree-flow.h).  */

void
get_fcalls (fcalls_p, which)
     varray_type *fcalls_p;
     unsigned which;
{
  basic_block bb;

  if (fcalls_p == NULL || *fcalls_p == NULL)
    abort ();

  FOR_EACH_BB (bb)
    {
      struct ref_list_node *tmp;
      varref ref;
      FOR_EACH_REF (ref, tmp, BB_REFS (bb))
	{
	  tree fcall = VARREF_SYM (ref);

	  if (TREE_CODE (fcall) == FUNCTION_DECL)
	    {
	      if ((which & FCALL_NON_PURE)
		  && ! DECL_IS_PURE (fcall)
		  && ! DECL_BUILT_IN (fcall))
		VARRAY_PUSH_GENERIC_PTR (*fcalls_p, ref);
	      
	      else if ((which & FCALL_PURE) && DECL_IS_PURE (fcall))
		VARRAY_PUSH_GENERIC_PTR (*fcalls_p, ref);

	      else if ((which & FCALL_BUILT_IN) && DECL_BUILT_IN (fcall))
		VARRAY_PUSH_GENERIC_PTR (*fcalls_p, ref);
	    }
	}
    }
}


/*  Returns the basic block containing the statement that declares DECL.  */

basic_block
find_declaration (decl)
     tree decl;
{
  basic_block bb;
  tree t;
  varref first_ref;

  /* Start with the first reference of DECL and walk the flowgraph
     backwards looking for a node with the scope block declaring the
     original variable.  */
  if (!TREE_REFS (decl) || !TREE_REFS (decl)->first)
    return NULL;
  first_ref = TREE_REFS (decl)->first->ref;
  t = VARREF_STMT (first_ref);
  FOR_BB_BETWEEN (bb, BB_FOR_STMT (t), ENTRY_BLOCK_PTR->next_bb, prev_bb)
    {
      if (TREE_CODE (bb->head_tree) == SCOPE_STMT
	  && SCOPE_STMT_BLOCK (bb->head_tree))
	{
	  tree block = SCOPE_STMT_BLOCK (bb->head_tree);
	  tree var;

	  for (var = BLOCK_VARS (block); var; var = TREE_CHAIN (var))
	    if (var == decl)
	      return bb;
	}
    }

  return NULL;
}



/* Debugging functions.  */

/*  Display variable reference REF on stream OUTF. PREFIX is a string that
    is prefixed to every line of output, and INDENT is the amount of left
    margin to leave.  If DETAILS is nonzero, the output is more verbose.  */

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

  sym = (VARREF_SYM (ref) 
	 && VARREF_TYPE (ref) != EXPRPHI 
	 && VARREF_TYPE (ref) != EXPRUSE
	 && VARREF_TYPE (ref) != EXPRKILL
	 && DECL_NAME (VARREF_SYM (ref)))
    ? IDENTIFIER_POINTER (DECL_NAME (VARREF_SYM (ref))) : "nil";

  bbix = (VARREF_BB (ref)) ? VARREF_BB (ref)->index : -1;

  type = (VARREF_TYPE (ref) == VARDEF) ? "DEF" :
    (VARREF_TYPE (ref) == VARUSE) ? "USE" :
    (VARREF_TYPE (ref) == VARPHI) ? "PHI" :
    (VARREF_TYPE (ref) == EXPRPHI) ? "EXPRPHI" :
    (VARREF_TYPE (ref) == EXPRUSE) ? "EXPRUSE" :
    (VARREF_TYPE (ref) == EXPRKILL) ? "EXPRKILL" :
    (VARREF_TYPE (ref) == EXPRINJ) ? "EXPRINJ" :
    "???";

  fprintf (outf, "%s%s%s(%s): line %d, bb %d, ", s_indent, prefix, type,
	   sym, lineno, bbix);

  if (VARREF_EXPR (ref))
    print_node_brief (outf, "", VARREF_EXPR (ref), 0);
  else
    fprintf (outf, "<nil>");

  /* Dump specific contents for the different types of references.  */
  if (details)
    {
      if (VARREF_TYPE (ref) == VARPHI && VARDEF_PHI_CHAIN (ref))
	{
	  fputs (" phi-args:\n", outf);
	  dump_varref_array (outf, prefix, VARDEF_PHI_CHAIN (ref), indent + 4, 0);
	}
      else if (VARREF_TYPE (ref) == EXPRPHI && EXPRPHI_PHI_CHAIN (ref))
	{
	  if (details)
	    fprintf (outf, " class:%d downsafe:%d can_be_avail:%d later:%d\n", 
		     EXPRREF_CLASS (ref), EXPRPHI_DOWNSAFE (ref), 
		     EXPRPHI_CANBEAVAIL (ref), EXPRPHI_LATER (ref));
	  fputs (" exprphi-args:\n", outf);
	  dump_varref_array (outf, prefix, EXPRPHI_PHI_CHAIN (ref), indent + 4, 1);
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
      else if (VARREF_TYPE (ref) == EXPRUSE && EXPRUSE_PHIOP (ref) == 1)
	{
	  char *temp_indent;
	  fprintf (outf, " class:%d has_real_use:%d  operand defined by:\n", 
		   EXPRREF_CLASS (ref), EXPRUSE_HAS_REAL_USE (ref));	  
	  temp_indent = (char *) alloca ((size_t) indent + 4 + 1);
	  memset ((void *) temp_indent, ' ', (size_t) indent + 4);
	  temp_indent[indent + 4] = '\0';
	  if (EXPRUSE_DEF (ref) == NULL)
	    fprintf (outf, "%snothing\n", temp_indent);
	  else
	    dump_varref (outf, prefix, EXPRUSE_DEF (ref), indent + 4, 0);
	}
      
	    
    }

  fputc ('\n', outf);
}


/*  Display variable reference REF on stderr.  */

void
debug_varref (ref)
     varref ref;
{
  dump_varref (stderr, "", ref, 0, 1);
}


/*  Display a list of variable references on stream OUTF. PREFIX is a
    string that is prefixed to every line of output, and INDENT is the
    amount of left margin to leave.  If DETAILS is nonzero, the output is
    more verbose.  */

void
dump_varref_list (outf, prefix, reflist, indent, details)
     FILE *outf;
     const char *prefix;
     ref_list reflist;
     int indent;
     int details;
{
  struct ref_list_node *tmp;
  varref ref;
  if (reflist == NULL)
    return;
  FOR_EACH_REF (ref, tmp, reflist)
      dump_varref (outf, prefix, ref, 
		   indent, details);
}



/*  Display a list of variable references on stream OUTF. PREFIX is a
    string that is prefixed to every line of output, and INDENT is the
    amount of left margin to leave.  If DETAILS is nonzero, the output is
    more verbose.  */

void
dump_varref_array (outf, prefix, reflist, indent, details)
     FILE *outf;
     const char *prefix;
     varray_type reflist;
     int indent;
     int details;
{
  size_t i;

  if (reflist == NULL)
    return;

  for (i = 0; i < VARRAY_SIZE (reflist); i++)
    if (VARRAY_GENERIC_PTR (reflist, i))
      dump_varref (outf, prefix, VARRAY_GENERIC_PTR (reflist, i), 
		   indent, details);
}


/*  Dump REFLIST on stderr.  */

void
debug_varref_list (reflist)
     ref_list reflist;
{
  dump_varref_list (stderr, "", reflist, 0, 1);
}


/* Dump REFLIST on stderr.  */

void
debug_varref_array (reflist)
    varray_type reflist;
{
  dump_varref_array (stderr, "", reflist, 0, 1);
}

/* Construction of the function call graph.
   Copyright (C) 2002 Free Software Foundation, Inc.
   Contributed by Sebastian Pop <s.pop@laposte.net>

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
#include "c-tree.h"
#include "c-common.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "tree-flow.h"
#include "diagnostic.h"


/* Static declarations.  */
static void construct_call_graph PARAMS ((output_buffer *, tree, HOST_WIDE_INT));
static void print_callee PARAMS ((output_buffer *, tree, int));

#define INDENT(SPACE) do { \
  int i; for (i = 0; i<SPACE; i++) output_add_space (buffer); } while (0)
#define NIY do { \
  debug_output_buffer (buffer); debug_tree (node); abort (); } while (0)


/* Print the call graph associated to the tree T, in the file FILE.  */

void
print_call_graph (file, t)
     FILE *file;
     tree t;
{
  output_buffer buffer_rec;
  output_buffer *buffer = &buffer_rec;

  init_output_buffer (buffer, /* prefix */NULL, /* line-width */0);
  output_clear_message_text (buffer);
  output_printf (buffer, "<file>\n");
  construct_call_graph (buffer, t, 0);
  output_printf (buffer, "</file>\n");
  fprintf (file, "%s", output_finalize_message (buffer));
}

/* Print the call graph on stderr.  */

void
debug_call_graph (t)
     tree t;
{
  print_call_graph (stderr, t);
}


/* Scan the tree T searching for callee/caller functions, then output found
   function calls/callers in the output_buffer BUFFER under the DTD format
   described above.
   Not Yet Implemented : the dump of global variables and their use.  */

static void
construct_call_graph (buffer, t, spc)
     output_buffer *buffer;
     tree t;
     HOST_WIDE_INT spc;
{
  static unsigned int decision_points;
  static unsigned int nb_statements;
  static unsigned int nb_calls;
  tree node = t;

  while (node && node != error_mark_node)
    {
      enum tree_code code = TREE_CODE (node);
#if 0
      if (is_ctrl_stmt (node)) decision_points++;
      if (is_exec_stmt (node)) nb_statements++;
#endif

      switch (code)
	{
	case TYPE_DECL:
	case FIELD_DECL:
	case VAR_DECL:
	case PARM_DECL:
	  /* Some nodes on which we need to stop the recursion.  */
	  return;

	case FUNCTION_DECL:
	  if (BUILT_IN_FRONTEND)

	  INDENT (spc);
	  output_printf (buffer, "<caller id = \"");
	  output_printf (buffer, get_name (node), 0);
	  output_printf (buffer, "\">\n");

	  /* What about definition of nested functions?  That will reset the
	     current value of decision_points and nb_statements...  */
	  decision_points = 0;
	  nb_statements = 0;
	  nb_calls = 0;
	  construct_call_graph (buffer, DECL_SAVED_TREE (node), spc+1);

	  /* Statements based statistics.  */
	  INDENT (spc+1);
	  output_printf (buffer, "<stats calls=\"%d\" decisions=\"%d\" stmts=\"%d\" Gilb=\"%f\"",
			 nb_calls, decision_points, nb_statements,
			 ((nb_statements == 0) ? 0.0 :
			  ((float)decision_points / (float)nb_statements)));

	  /* Control flow statistics.  */
	  init_flow ();
	  build_tree_cfg (DECL_SAVED_TREE (node));
	  output_printf (buffer, " CFG-edges=\"%d\" CFG-BB=\"%d\" McCabe=\"%d\">\n",
			 n_edges, n_basic_blocks, n_edges - n_basic_blocks + 2);
	  delete_tree_cfg ();

	  /* End of the node.  */
	  INDENT (spc);
	  output_printf (buffer, "</caller>\n");
	  return;

	case CALL_EXPR:
	  {
	    tree op0;
	    op0 = TREE_OPERAND (node, 0);

	    nb_calls++;
	    if (TREE_CODE (op0) == NON_LVALUE_EXPR)
	      op0 = TREE_OPERAND (op0, 0);

	    switch (TREE_CODE (op0))
	      {
	      case VAR_DECL:
	      case PARM_DECL:
		/* if (TREE_CODE (op0) == PARM_DECL)
		   This function pointer was received in parameter.
		   I think that this should not enter in the call graph.
		   Or otherwise it enters but with a special mark
		   saying that the name of this function is a parameter,
		   and thus for this name we don't expect a declaration.
		   Example:
		   /gcc/libiberty/splay_tree.c:splay_tree_foreach_helper ().  */
		print_callee (buffer, op0, spc);
		break;

	      case ADDR_EXPR:
	      case INDIRECT_REF:
	      case NOP_EXPR:
		print_callee (buffer, TREE_OPERAND (op0, 0), spc);
		break;

	      case COND_EXPR:
#if 0
		/* XXX: Why is this disabled?  */
		print_callee (buffer, TREE_OPERAND (TREE_OPERAND (op0, 0), 1), spc);
		print_callee (buffer, TREE_OPERAND (TREE_OPERAND (op0, 0), 2), spc);
#endif
		break;

	      case COMPONENT_REF:
		/* The function is a pointer contained in a structure.  */
		if (TREE_CODE (TREE_OPERAND (op0, 0)) == INDIRECT_REF ||
		    TREE_CODE (TREE_OPERAND (op0, 0)) == VAR_DECL)
		  print_callee (buffer, TREE_OPERAND (op0, 1), spc);
#if 0
		else
		/* We can have several levels of structures and a function
		   pointer inside.  This is not implemented yet...  */
		  NIY;
#endif
		break;

	      case ARRAY_REF:
		if (TREE_CODE (TREE_OPERAND (op0, 0)) == VAR_DECL)
		  print_callee (buffer, TREE_OPERAND (op0, 0), spc);
		else
		  print_callee (buffer, TREE_OPERAND (op0, 1), spc);
		break;

	      default:
		NIY;
	      }
	    /* Walk through function's arguments.  */
	    construct_call_graph (buffer, TREE_OPERAND (node, 1), spc);
	    node = TREE_CHAIN (node);
	    break;
	  }

	case METHOD_CALL_EXPR:
	  NIY;

	case ADDR_EXPR:
	  /* May be a function pointer passed as a parameter to a function.  */
	  if (TREE_CODE (TREE_OPERAND (node, 0)) == FUNCTION_DECL)
	    print_callee (buffer, TREE_OPERAND (node, 0), spc);
	  else
	    construct_call_graph (buffer, TREE_OPERAND (node, 0), spc);
	  node = TREE_CHAIN (node);
	  break;

	case ARRAY_REF:
	  if (TREE_CODE (TREE_OPERAND (node, 0)) == ARRAY_REF)
	    construct_call_graph (buffer, TREE_OPERAND (node, 0), spc);

	  construct_call_graph (buffer, TREE_OPERAND (node, 1), spc);
	  node = TREE_CHAIN (node);
	  break;

	case TREE_LIST:
	  construct_call_graph (buffer, TREE_VALUE (node), spc);
	  node = TREE_CHAIN (node);
	  break;

	case FIX_TRUNC_EXPR:
	case FIX_CEIL_EXPR:
	case FIX_FLOOR_EXPR:
	case FIX_ROUND_EXPR:
	case FLOAT_EXPR:
	case TRUTH_NOT_EXPR:
	case BIT_NOT_EXPR:
	case NE_EXPR:
	case INDIRECT_REF:
	case COMPOUND_STMT:
	case EXPR_STMT:
	case NOP_EXPR:
	case RETURN_STMT:
	  /* Unary nodes.  */
	  construct_call_graph (buffer, TREE_OPERAND (node, 0), spc);
	  node = TREE_CHAIN (node);
	  break;

	case DO_STMT:
	case WHILE_STMT:
	case SWITCH_STMT:
	case LSHIFT_EXPR:
	case RSHIFT_EXPR:
	case LROTATE_EXPR:
	case RROTATE_EXPR:
	case BIT_IOR_EXPR:
	case BIT_XOR_EXPR:
	case BIT_AND_EXPR:
	case BIT_ANDTC_EXPR:
	case TRUTH_ANDIF_EXPR:
	case TRUTH_ORIF_EXPR:
	case TRUTH_AND_EXPR:
	case TRUTH_OR_EXPR:
	case TRUTH_XOR_EXPR:
	case LT_EXPR:
	case LE_EXPR:
	case GT_EXPR:
	case GE_EXPR:
	case EQ_EXPR:
	case PLUS_EXPR:
	case MINUS_EXPR:
	case MULT_EXPR:
	case TRUNC_DIV_EXPR:
	case MODIFY_EXPR:
	  /* Binary nodes.  */
	  construct_call_graph (buffer, TREE_OPERAND (node, 0), spc);
	  construct_call_graph (buffer, TREE_OPERAND (node, 1), spc);
	  node = TREE_CHAIN (node);
	  break;

	case COND_EXPR:
	case IF_STMT:
	  /* Ternary nodes.  */
	  construct_call_graph (buffer, TREE_OPERAND (node, 0), spc);
	  construct_call_graph (buffer, TREE_OPERAND (node, 1), spc);
	  construct_call_graph (buffer, TREE_OPERAND (node, 2), spc);
	  node = TREE_CHAIN (node);
	  break;

	case FOR_STMT:
	  /* Quaternary nodes.  */
	  construct_call_graph (buffer, TREE_OPERAND (node, 0), spc);
	  construct_call_graph (buffer, TREE_OPERAND (node, 1), spc);
	  construct_call_graph (buffer, TREE_OPERAND (node, 2), spc);
	  construct_call_graph (buffer, TREE_OPERAND (node, 3), spc);
	  node = TREE_CHAIN (node);
	  break;

	default:
	  node = TREE_CHAIN (node);
	  break;
	}
    }
}


/* Print the callee function declaration.  */

static void
print_callee (buffer, node, spc)
     output_buffer *buffer;
     tree node;
     int spc;
{
  int i;

  /* Indent.  */
  for (i = 0; i<spc; i++)
    output_add_space (buffer);

  /* Print the node.  */
  output_printf (buffer, "<callee idref = \"");
  output_printf (buffer, get_name (node), 0);
  output_printf (buffer, "\"/>\n");
}

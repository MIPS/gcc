/* Pretty formatting of a tree in C syntax.
   Copyright (C) 2001 Free Software Foundation, Inc.
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
#include "errors.h"
#include "tree.h"
#include "c-tree.h"
#include "c-common.h"
#include "diagnostic.h"

/* To be declared in a .h after diagnostic.h since output_buffer is used.  
   Or we can declare them in diagnostic.h ?  */
extern void dump_c_tree PARAMS ((output_buffer *, tree, HOST_WIDE_INT));
extern int dump_c_node PARAMS ((output_buffer *, tree, HOST_WIDE_INT));
extern void print_declaration PARAMS ((output_buffer *, tree, HOST_WIDE_INT));
extern void print_function_decl PARAMS ((output_buffer *, tree, HOST_WIDE_INT));
extern void print_struct_decl PARAMS ((output_buffer *, tree, HOST_WIDE_INT));

static int op_prio              PARAMS ((tree));
static const char *op_symbol    PARAMS ((tree));
static void pretty_print_string PARAMS ((output_buffer *, const char*));
static void print_call_name     PARAMS ((output_buffer *, tree));

#define INDENT(SPACE) do { \
  int i; for (i = 0; i<SPACE; i++) output_add_space (buffer); } while (0)
#define NIY do { \
    output_add_string (buffer, "<<< Unknown tree: "); \
    output_add_string (buffer, tree_code_name[(int) TREE_CODE (node)]); \
    output_add_string (buffer, " >>>\n"); } while (0)

#define PRINT_FUNCTION_NAME(NODE)  output_printf             \
  (buffer, "%s", TREE_CODE (NODE) == NOP_EXPR ?              \
   IDENTIFIER_POINTER (DECL_NAME (TREE_OPERAND (NODE, 0))) : \
   IDENTIFIER_POINTER (DECL_NAME (NODE)))



/* Print the tree T in full, on file FILE.  */
 
void 
print_c_tree (file, t)
     FILE *file;
     tree t;
{
  output_buffer buffer_rec;
  output_buffer *buffer = &buffer_rec;
  
  init_output_buffer (buffer, /* prefix */NULL, /* line-width */0);
  output_clear_message_text (buffer);
  dump_c_tree (buffer, t, 0);
  fprintf (file, "%s", output_finalize_message (buffer));
}

/* Print the node T on file FILE.  */

void 
print_c_node (file, t)
     FILE *file;
     tree t;
{
  output_buffer buffer_rec;
  output_buffer *buffer = &buffer_rec;

  init_output_buffer (buffer, /* prefix */NULL, /* line-width */0);
  output_clear_message_text (buffer);
  dump_c_node (buffer, t, 0);
  fprintf (file, "%s", output_finalize_message (buffer));
}

/* Print the tree T in full, on stderr.  */

void 
debug_c_tree (t)
     tree t;
{
  print_c_tree (stderr, t);
}

/* Print the node T on stderr.  */

void 
debug_c_node (t)
     tree t;
{
  print_c_node (stderr, t);
}

/* Dump the tree T on the output_buffer BUFFER.  */
 
void 
dump_c_tree (buffer, t, spc)
     output_buffer *buffer;
     tree t;
     HOST_WIDE_INT spc;
{
  tree node = t;
  while (node && node != error_mark_node)
    {
      spc = dump_c_node (buffer, node, spc);
      switch (TREE_CODE (node))
	{
	case TYPE_DECL:
	case FIELD_DECL:
	case VAR_DECL:
	case PARM_DECL:
	  /* Some nodes on which we need to stop the recursive printing,
	     otherwise we print all declared vars in the scope.  */
	  return;
	default:
	  break;
	}
      node = TREE_CHAIN (node);
    }
}

/* Dump the node NODE on the output_buffer BUFFER, SPC spaces of indent.  */

int
dump_c_node (buffer, node, spc)
     output_buffer *buffer;
     tree node;
     HOST_WIDE_INT spc;
{
  tree type;
  tree op0, op1;

  if (node == NULL_TREE)
    return spc;

  /* Keep the following switch ordered as in 'tree.def' and 'c-common.def'.  */
  switch (TREE_CODE (node))
    {
    case ERROR_MARK:
      output_add_string (buffer, "<<< error >>>");
      break;

    case IDENTIFIER_NODE:
      output_add_string (buffer, IDENTIFIER_POINTER (node));
      break;

    case TREE_LIST:
      while (node && node != error_mark_node)
	{
	  if (TREE_PURPOSE (node))
	    {
	      dump_c_node (buffer, TREE_PURPOSE (node), spc);
	      output_add_space (buffer);
	    }
	  dump_c_node (buffer, TREE_VALUE (node), spc);
	  node = TREE_CHAIN (node);
	  if (node && TREE_CODE (node) == TREE_LIST)
	    {
	      output_add_character (buffer, ',');
	      output_add_space (buffer);
	    }
	}
      break;

    case TREE_VEC:
      dump_c_node (buffer, BINFO_TYPE (node), spc);
      break;

    case BLOCK:
      NIY;

    case VOID_TYPE:
    case INTEGER_TYPE:
    case REAL_TYPE:
    case COMPLEX_TYPE:
    case VECTOR_TYPE:
    case ENUMERAL_TYPE:
    case BOOLEAN_TYPE:
    case CHAR_TYPE:
      {
	unsigned int quals = TYPE_QUALS (node);
	char class;

	if (quals & TYPE_QUAL_CONST)
	  output_add_string (buffer, "const ");
	else if (quals & TYPE_QUAL_VOLATILE)
	  output_add_string (buffer, "volatile ");

	class = TREE_CODE_CLASS (TREE_CODE (node));

	if (class == 'd')
	  {
	    if (DECL_NAME (node))
	      output_add_string (buffer, 
				 IDENTIFIER_POINTER (DECL_NAME (node)));
	  }
	else if (class == 't')
	  {
	    if (TYPE_NAME (node))
	      {
		if (TREE_CODE (TYPE_NAME (node)) == IDENTIFIER_NODE)
		  output_add_string (buffer,
				     IDENTIFIER_POINTER (TYPE_NAME (node)));
		else if (TREE_CODE (TYPE_NAME (node)) == TYPE_DECL
			 && DECL_NAME (TYPE_NAME (node)))
		  output_add_string (buffer,
				     IDENTIFIER_POINTER (DECL_NAME (TYPE_NAME (node))));
	      }
	  }
	break;
      }
    case POINTER_TYPE:
      dump_c_node (buffer, TREE_TYPE (node), spc);
      output_add_string (buffer, " *");
      break;

    case OFFSET_TYPE:
      NIY;

    case REFERENCE_TYPE:
      /* FIXME : What is the exact syntax of this node for C? */
      dump_c_node (buffer, TREE_TYPE (node), spc);
      break;

    case METHOD_TYPE:
      output_add_string (buffer, IDENTIFIER_POINTER 
			 (DECL_NAME (TYPE_NAME (TYPE_METHOD_BASETYPE (node)))));
      output_add_string (buffer, "::");
      break;

    case FILE_TYPE:
      NIY;

    case ARRAY_TYPE:
      {
	tree tmp;

	/* Print the array type.  */
	dump_c_node (buffer, TREE_TYPE (node), spc);

	/* Print the dimensions.  */
	tmp = node;
	while (tmp && TREE_CODE (tmp) == ARRAY_TYPE)
	  {
	    output_add_character (buffer, '[');
	    output_decimal (buffer,
			    TREE_INT_CST_LOW (TYPE_SIZE (tmp)) / 
			    TREE_INT_CST_LOW (TYPE_SIZE (TREE_TYPE (tmp))));
	    output_add_character (buffer, ']');
	    tmp = TREE_TYPE (tmp);
	  }
	break;
      }

    case SET_TYPE:
      NIY;

    case RECORD_TYPE:
    case UNION_TYPE:
      /* Print the name of the structure.  */
      if (TYPE_NAME (node))
	{
	  if (TREE_CODE (node) == RECORD_TYPE)
	    output_add_string (buffer, "struct ");
	  else if (TREE_CODE (node) == UNION_TYPE)
	    output_add_string (buffer, "union ");

	  dump_c_node (buffer, TYPE_NAME (node), spc);
	}
      /* Don't print the contents of the structure since the user don't asked 
	 for explicitly.  */
      break;

    case QUAL_UNION_TYPE:
      NIY;

    case FUNCTION_TYPE:
      break;

    case LANG_TYPE:
      NIY;

    case INTEGER_CST:
      if (TREE_CODE (TREE_TYPE (node)) == POINTER_TYPE)
	/* In the case of a pointer, divise by the size of the pointed type.  */
	output_decimal (buffer,
			TREE_INT_CST_LOW (node) / 
			TREE_INT_CST_LOW (TYPE_SIZE_UNIT (TREE_TYPE (node))));
      else
	output_decimal (buffer, TREE_INT_CST_LOW (node));
      break;

    case REAL_CST:
      /* Code copied from print_node.  */
      {
	REAL_VALUE_TYPE d;
	if (TREE_OVERFLOW (node))
	  output_add_string (buffer, " overflow");
	
#if !defined(REAL_IS_NOT_DOUBLE) || defined(REAL_ARITHMETIC)
	d = TREE_REAL_CST (node);
	if (REAL_VALUE_ISINF (d))
	  output_add_string (buffer, " Inf");
	else if (REAL_VALUE_ISNAN (d))
	  output_add_string (buffer, " Nan");
	else
	  {
	    char string[100];
	    REAL_VALUE_TO_DECIMAL (d, "%e", string);
	    output_add_string (buffer, string);
	  }
#else
	{
	  HOST_WIDE_INT i;
	  unsigned char *p = (unsigned char *) &TREE_REAL_CST (node);
	  output_add_string (buffer, "0x");
	  for (i = 0; i < sizeof TREE_REAL_CST (node); i++)
	    output_formatted_integer (buffer, "%02x", *p++);
	}
#endif
	break;
      }

    case COMPLEX_CST:
      NIY;

    case STRING_CST:
      output_add_string (buffer, "\"");
      pretty_print_string (buffer, TREE_STRING_POINTER (node));
      output_add_string (buffer, "\"");
      break;

    case FUNCTION_DECL:
      /* Print the prototype of the function.  */
      INDENT (spc);
      
      /* Print the return type.  */
      dump_c_node (buffer, TREE_TYPE (TREE_TYPE (node)), spc);
      output_add_space (buffer);

      /* Print the namespace.  */
      dump_c_node (buffer, TREE_TYPE (node), spc);
	      
      /* Print the function name.  */
      output_add_string (buffer, IDENTIFIER_POINTER (DECL_NAME (node)));
      output_add_space (buffer);
      output_add_character (buffer, '(');
      
      /* Print the argument types.  The last element in the list is a 
	  VOID_TYPE.  The following avoid to print the last element.  */
      {
	tree tmp = TYPE_ARG_TYPES (TREE_TYPE (node));
	while (tmp && TREE_CHAIN (tmp) && tmp != error_mark_node)
	  {
	    dump_c_node (buffer, TREE_VALUE (tmp), spc);
	    tmp = TREE_CHAIN (tmp);
	    if (TREE_CHAIN (tmp) && TREE_CODE (TREE_CHAIN (tmp)) == TREE_LIST)
	      {
		output_add_character (buffer, ',');
		output_add_space (buffer);
	      }
	  }
      }
      output_add_character (buffer, ')');
      output_add_character (buffer, ';');
      output_add_newline (buffer);
      break;

    case LABEL_DECL:
      if (DECL_NAME (node))
	output_add_string (buffer, IDENTIFIER_POINTER (DECL_NAME (node)));
      break;

    case CONST_DECL:
      if (DECL_NAME (node))
	output_add_string (buffer, IDENTIFIER_POINTER (DECL_NAME (node)));
      break;

    case TYPE_DECL:
      if (strcmp (DECL_SOURCE_FILE (node), "<built-in>") == 0)
	{
	  /* Don't print the declaration of built-in types.  */
	  break;
	}
      if (DECL_NAME (node))
	{
	  output_add_string (buffer, IDENTIFIER_POINTER (DECL_NAME (node)));
	}
      else
	{
	  if (TYPE_METHODS (TREE_TYPE (node)))
	    {
	      /* The caller is a c++ function : all structures have at least 
		 4 methods. */
	      INDENT (spc);
	      output_add_string (buffer, "class ");
	      dump_c_node (buffer, TREE_TYPE (node), spc);
	    }
	  else
	    {
	      INDENT (spc);
	      output_add_string (buffer, "struct ");
	      dump_c_node (buffer, TREE_TYPE (node), spc);
	      output_add_character (buffer, ';');
	      output_add_newline (buffer);
	    }
	}
      break;

    case VAR_DECL:
    case PARM_DECL:
      if (DECL_NAME (node))
	output_add_string (buffer, IDENTIFIER_POINTER (DECL_NAME (node)));
      break;

    case RESULT_DECL:
      dump_c_node (buffer, TREE_TYPE (node), spc);      
      break;

    case FIELD_DECL:
      if (DECL_NAME (node))
	output_add_string (buffer, IDENTIFIER_POINTER (DECL_NAME (node)));
      break;

    case NAMESPACE_DECL:
      if (DECL_NAME (node))
	output_add_string (buffer, IDENTIFIER_POINTER (DECL_NAME (node)));
      break;

    case COMPONENT_REF:
      dump_c_node (buffer, TREE_OPERAND (node, 0), spc);
      output_add_character (buffer, '.');
      dump_c_node (buffer, TREE_OPERAND (node, 1), spc);
      break;

    case BIT_FIELD_REF:
      NIY;

    case BUFFER_REF:
      NIY;

    case ARRAY_REF:
      op0 = TREE_OPERAND (node, 0);
      dump_c_node (buffer, op0, spc);
      output_add_character (buffer, '[');
      dump_c_node (buffer, TREE_OPERAND (node, 1), spc);
      output_add_character (buffer, ']');
      break;

    case ARRAY_RANGE_REF:
      NIY;

    case CONSTRUCTOR:
      output_add_character (buffer, '{');
      dump_c_node (buffer, TREE_OPERAND (node, 1), spc);
      output_add_character (buffer, '}');
      break;

    case COMPOUND_EXPR:
      dump_c_node (buffer, TREE_OPERAND (node, 0), spc);
      output_add_character (buffer, ',');
      output_add_space (buffer);
      dump_c_node (buffer, TREE_OPERAND (node, 1), spc);
      break;

    case MODIFY_EXPR:
      dump_c_node (buffer, TREE_OPERAND (node, 0), spc);
      output_add_space (buffer);
      output_add_character (buffer, '=');
      output_add_space (buffer);
      dump_c_node (buffer, TREE_OPERAND (node, 1), spc);
      break;

    case INIT_EXPR:
      NIY;

    case TARGET_EXPR:
      NIY;

    case COND_EXPR:
      dump_c_node (buffer, TREE_OPERAND (node, 0), spc);
      output_add_space (buffer);
      output_add_character (buffer, '?');
      output_add_space (buffer);
      dump_c_node (buffer, TREE_OPERAND (node, 1), spc);
      output_add_space (buffer);
      output_add_character (buffer, ':');
      output_add_space (buffer);
      dump_c_node (buffer, TREE_OPERAND (node, 2), spc);
      break;

    case BIND_EXPR:
      NIY;

    case CALL_EXPR:
      print_call_name (buffer, node);

      /* Print parameters.  */
      output_add_space (buffer);
      output_add_character (buffer, '(');
      op1 = TREE_OPERAND (node, 1);
      if (op1)
	dump_c_node (buffer, op1, 0);
      output_add_character (buffer, ')');
      break;

    case METHOD_CALL_EXPR:
      NIY;

    case WITH_CLEANUP_EXPR:
      NIY;

    case CLEANUP_POINT_EXPR:
      NIY;

    case PLACEHOLDER_EXPR:
      NIY;

    case WITH_RECORD_EXPR:
      NIY;

      /* Binary arithmetic and logic expressions.  */
    case MULT_EXPR:
    case PLUS_EXPR:
    case MINUS_EXPR:
    case TRUNC_DIV_EXPR:
    case CEIL_DIV_EXPR:
    case FLOOR_DIV_EXPR:
    case ROUND_DIV_EXPR:
    case TRUNC_MOD_EXPR:
    case CEIL_MOD_EXPR:
    case FLOOR_MOD_EXPR:
    case ROUND_MOD_EXPR:
    case RDIV_EXPR:
    case EXACT_DIV_EXPR:
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
    case NE_EXPR:
    case UNLT_EXPR:
    case UNLE_EXPR:
    case UNGT_EXPR:
    case UNGE_EXPR:
    case UNEQ_EXPR:
      {
	const char *op = op_symbol (node);
	op0 = TREE_OPERAND (node, 0);
	op1 = TREE_OPERAND (node, 1);

	/* When the operands are expressions with less priority, 
	   keep semantics of the tree representation.  */
	if (op_prio (op0) < op_prio (node))
	  {
	    output_add_character (buffer, '(');
	    dump_c_node (buffer, op0, spc);
	    output_add_character (buffer, ')');
	  }
	else
	  dump_c_node (buffer, op0, spc);

	output_add_space (buffer);
	output_add_string (buffer, op);
	output_add_space (buffer);

	/* When the operands are expressions with less priority, 
	   keep semantics of the tree representation.  */
	if (op_prio (op1) < op_prio (node))
	  {
	    output_add_character (buffer, '(');
	    dump_c_node (buffer, op1, spc);
	    output_add_character (buffer, ')');
	  }
	else
	  dump_c_node (buffer, op1, spc);
      }
      break;

    case FIX_TRUNC_EXPR:
    case FIX_CEIL_EXPR:
    case FIX_FLOOR_EXPR:
    case FIX_ROUND_EXPR:
    case FLOAT_EXPR:
      dump_c_node (buffer, TREE_OPERAND (node, 0), spc);	  
      break;

      /* Unary arithmetic and logic expressions.  */
    case NEGATE_EXPR:
    case BIT_NOT_EXPR:
    case TRUTH_NOT_EXPR:
    case ADDR_EXPR:
    case REFERENCE_EXPR:
    case PREDECREMENT_EXPR:
    case PREINCREMENT_EXPR:
    case INDIRECT_REF:
      if (TREE_CODE (node) == ADDR_EXPR
	  && (TREE_CODE (TREE_OPERAND (node, 0)) == STRING_CST
	      || TREE_CODE (TREE_OPERAND (node, 0)) == FUNCTION_DECL))
	;	/* Do not output '&' for strings and function pointers.  */
      else
	output_add_string (buffer, op_symbol (node));

      if (op_prio (TREE_OPERAND (node, 0)) < op_prio (node))
	{
	  output_add_character (buffer, '(');
	  dump_c_node (buffer, TREE_OPERAND (node, 0), spc);
	  output_add_character (buffer, ')');
	}
      else
	dump_c_node (buffer, TREE_OPERAND (node, 0), spc);
      break;

    case POSTDECREMENT_EXPR:
    case POSTINCREMENT_EXPR:
      if (op_prio (TREE_OPERAND (node, 0)) < op_prio (node))
	{
	  output_add_character (buffer, '(');
	  dump_c_node (buffer, TREE_OPERAND (node, 0), spc);
	  output_add_character (buffer, ')');
	}
      else
	dump_c_node (buffer, TREE_OPERAND (node, 0), spc);
      output_add_string (buffer, op_symbol (node));
      break;

    case MIN_EXPR:
      /* #define MIN(X,Y) ((X) < (Y) ? (X) : (Y))  */
      dump_c_node (buffer, TREE_OPERAND (node, 0), spc);
      output_add_string (buffer, " < ");
      dump_c_node (buffer, TREE_OPERAND (node, 1), spc);
      output_add_string (buffer, " ? ");
      dump_c_node (buffer, TREE_OPERAND (node, 0), spc);
      output_add_string (buffer, " : ");
      dump_c_node (buffer, TREE_OPERAND (node, 1), spc);
      break;

    case MAX_EXPR:
      /* #define MAX(X,Y) ((X) > (Y) ? (X) : (Y))  */
      dump_c_node (buffer, TREE_OPERAND (node, 0), spc);
      output_add_string (buffer, " > ");
      dump_c_node (buffer, TREE_OPERAND (node, 1), spc);
      output_add_string (buffer, " ? ");
      dump_c_node (buffer, TREE_OPERAND (node, 0), spc);
      output_add_string (buffer, " : ");
      dump_c_node (buffer, TREE_OPERAND (node, 1), spc);
      break;

    case ABS_EXPR:
      /* n < 0 ? -n : n */
      dump_c_node (buffer, TREE_OPERAND (node, 0), spc);
      output_add_string (buffer, " < 0 ? -");
      dump_c_node (buffer, TREE_OPERAND (node, 0), spc);
      output_add_string (buffer, " : ");
      dump_c_node (buffer, TREE_OPERAND (node, 0), spc);
      break;
      
    case FFS_EXPR:
      NIY;

    case UNORDERED_EXPR:
      NIY;

    case ORDERED_EXPR:
      NIY;

    case IN_EXPR:
      NIY;

    case SET_LE_EXPR:
      NIY;

    case CARD_EXPR:
      NIY;

    case RANGE_EXPR:
      NIY;

    case CONVERT_EXPR:
    case NOP_EXPR:
      type = TREE_TYPE (node);
      if (type == ptr_type_node)
	{
	  /* Get the pointed-to type.  */
	  type = TREE_TYPE (type);
	  output_add_character (buffer, '(');
	  dump_c_node (buffer, type, spc);
	  output_add_string (buffer, " *)");
	}
      dump_c_node (buffer, TREE_OPERAND (node, 0), spc);
      break;

    case NON_LVALUE_EXPR:
      dump_c_node (buffer, TREE_OPERAND (node, 0), spc);
      break;

    case SAVE_EXPR:
      dump_c_node (buffer, TREE_OPERAND (node, 0), spc);
      break;

    case UNSAVE_EXPR:
      dump_c_node (buffer, TREE_OPERAND (node, 0), spc);
      break;

    case RTL_EXPR:
      NIY;

    case ENTRY_VALUE_EXPR:
      NIY;

    case COMPLEX_EXPR:
      NIY;

    case CONJ_EXPR:
      NIY;

    case REALPART_EXPR:
      NIY;

    case IMAGPART_EXPR:
      NIY;

    case VA_ARG_EXPR:
      NIY;

    case TRY_CATCH_EXPR:
      NIY;

    case TRY_FINALLY_EXPR:
      NIY;

    case GOTO_SUBROUTINE_EXPR:
      NIY;

    case LABEL_EXPR:
      INDENT (spc);
      dump_c_node (buffer, TREE_OPERAND (node, 0), spc);
      output_add_character (buffer, ':');
      output_add_character (buffer, ';');
      output_add_newline (buffer);
      break;

    case GOTO_EXPR:
      NIY;

    case EXIT_EXPR:
      NIY;

    case LOOP_EXPR:
      NIY;

    case LABELED_BLOCK_EXPR:
      NIY;

    case EXIT_BLOCK_EXPR:
      NIY;

    case EXPR_WITH_FILE_LOCATION:
      INDENT (spc);
      dump_c_node (buffer, TREE_OPERAND (node, 0), spc);
      break;

    case SWITCH_EXPR:
      NIY;

    case EXC_PTR_EXPR:
      NIY;

      /* Nodes from 'c-common.def'.  */

    case SRCLOC:
      NIY;

    case SIZEOF_EXPR:
      NIY;

    case ARROW_EXPR:
      NIY;

    case ALIGNOF_EXPR:
      NIY;

    case EXPR_STMT:
      INDENT (spc);
      dump_c_node (buffer, EXPR_STMT_EXPR (node), spc);
      output_add_character (buffer, ';');
      output_add_newline (buffer);
      break;

    case COMPOUND_STMT:
      dump_c_tree (buffer, COMPOUND_BODY (node), spc);
      break;

    case DECL_STMT:
      print_declaration (buffer, TREE_OPERAND (node, 0), spc);
      break;

    case IF_STMT:
      INDENT (spc);
      output_add_string (buffer, "if (");
      dump_c_node (buffer, IF_COND (node), spc);
      output_add_character (buffer, ')');
      output_add_newline (buffer);
      dump_c_node (buffer, THEN_CLAUSE (node), spc+2);
      if (ELSE_CLAUSE (node))
	{
	  INDENT (spc);
	  output_add_string (buffer, "else");
	  output_add_newline (buffer);
	  dump_c_node (buffer, ELSE_CLAUSE (node), spc+2);
	}
      break;

    case FOR_STMT:
      INDENT (spc);
      output_add_string (buffer, "for (");
      if (FOR_INIT_STMT (node))
	dump_c_node (buffer, EXPR_STMT_EXPR (FOR_INIT_STMT (node)), 0);
      output_add_character (buffer, ';');
      output_add_space (buffer);
      dump_c_node (buffer, FOR_COND (node), 0);
      output_add_character (buffer, ';');
      output_add_space (buffer);
      dump_c_node (buffer, FOR_EXPR (node), 0);
      output_add_character (buffer, ')');
      if (FOR_BODY (node))
	{
	  output_add_newline (buffer);
	  dump_c_node (buffer, FOR_BODY (node), spc+2);
	}
      else
	{ 
	  output_add_character (buffer, ';');
	  output_add_newline (buffer);
	}
      break;

    case WHILE_STMT:
      INDENT (spc);
      output_add_string (buffer, "while (");
      dump_c_node (buffer, WHILE_COND (node), spc);
      output_add_character (buffer, ')');
      output_add_newline (buffer);
      dump_c_node (buffer, WHILE_BODY (node), spc+2);
      break;

    case DO_STMT:
      INDENT (spc);
      output_add_string (buffer, "do");
      output_add_newline (buffer);
      dump_c_node (buffer, DO_BODY (node), spc+2);
      INDENT (spc);
      output_add_string (buffer, "while (");
      dump_c_node (buffer, DO_COND (node), spc);
      output_add_character (buffer, ')');
      output_add_character (buffer, ';');
      output_add_newline (buffer);
      break;

    case RETURN_STMT:
      INDENT (spc);
      output_add_string (buffer, "return");
      if (TREE_CODE (TREE_TYPE (TREE_TYPE (current_function_decl))) != VOID_TYPE)
	{
	  output_add_space (buffer);
	  if (TREE_CODE (RETURN_EXPR (node)) == MODIFY_EXPR)
	    dump_c_node (buffer, TREE_OPERAND (RETURN_EXPR (node), 1), spc);
	  else
	    dump_c_node (buffer, RETURN_EXPR (node), spc);
	}
      output_add_character (buffer, ';');
      output_add_newline (buffer);
      break;

    case BREAK_STMT:
      INDENT (spc);
      output_add_string (buffer, "break;");
      output_add_newline (buffer);
      break;

    case CONTINUE_STMT:
      INDENT (spc);
      output_add_string (buffer, "continue;");
      output_add_newline (buffer);
      break;

    case SWITCH_STMT:
      INDENT (spc);
      output_add_string (buffer, "switch (");
      dump_c_node (buffer, SWITCH_COND (node), spc);
      output_add_character (buffer, ')');
      output_add_newline (buffer);
      dump_c_node (buffer, SWITCH_BODY (node), spc+2);
      break;

    case GOTO_STMT:
      INDENT (spc);
      output_add_string (buffer, "goto ");
      dump_c_node (buffer, GOTO_DESTINATION (node), spc);
      output_add_character (buffer, ';');
      output_add_newline (buffer);
      break;

    case LABEL_STMT:
      INDENT (spc);
      dump_c_node (buffer, TREE_OPERAND (node, 0), spc);
      output_add_character (buffer, ':');
      output_add_character (buffer, ';');
      output_add_newline (buffer);
      break;

    case ASM_STMT:
      INDENT (spc);
      output_add_string (buffer, "__asm__");
      if (ASM_VOLATILE_P (node))
	output_add_string (buffer, " __volatile__");
      output_add_character (buffer, '(');
      dump_c_node (buffer, TREE_OPERAND (node, 1), spc);
      output_add_character (buffer, ':');
      dump_c_node (buffer, TREE_OPERAND (node, 2), spc);
      output_add_character (buffer, ':');
      dump_c_node (buffer, TREE_OPERAND (node, 3), spc);
      output_add_string (buffer, ");");
      output_add_newline (buffer);      
      break;
      
    case SCOPE_STMT:
      if (SCOPE_BEGIN_P (node))
	{
	  INDENT (spc);
	  output_add_character (buffer, '{');
	  output_add_newline (buffer);
	  spc += 2;
	}
      else
	{
	  spc -= 2;
	  INDENT (spc);
	  output_add_character (buffer, '}');
	  output_add_newline (buffer);
	}
      break;

    case CASE_LABEL:
      INDENT (spc-2);
      if (CASE_LOW (node) && CASE_HIGH (node))
	{
	  output_add_string (buffer, "case ");
	  dump_c_node (buffer, CASE_LOW (node), spc);
	  output_add_string (buffer, " ... ");
	  dump_c_node (buffer, CASE_HIGH (node), spc);
	}
      else if (CASE_LOW (node))
	{
	  output_add_string (buffer, "case ");
	  dump_c_node (buffer, CASE_LOW (node), spc);
	}
      else
	output_add_string (buffer, "default ");
      output_add_character (buffer, ':');
      output_add_newline (buffer);
      break;

    case STMT_EXPR:
      output_add_character (buffer, '(');
      output_add_newline (buffer);
      dump_c_node (buffer, STMT_EXPR_STMT (node), spc);
      INDENT (spc);
      output_add_character (buffer, ')');
      break;

    case FILE_STMT:
      // FIXME : What is the exact syntax of this node ? #line 123 ?
      output_printf (buffer, "<<< file : %s >>>\n", FILE_STMT_FILENAME (node));
      break;

    case COMPOUND_LITERAL_EXPR:
      NIY;

    default:
      NIY;
    }
  return spc;
}

/* Print the declaration of a variable.  */

void
print_declaration (buffer, t, spc)
     output_buffer *buffer;
     tree t;
     HOST_WIDE_INT spc;
{
  /* Don't print type declarations.  */
  if (TREE_CODE (t) == TYPE_DECL)
    return;
      
  INDENT (spc);
  
  if (DECL_REGISTER (t))
    output_add_string (buffer, "register ");

  /* Print the type and name.  */
  if (TREE_CODE (TREE_TYPE (t)) == ARRAY_TYPE)
    {
      tree tmp;
      
      /* Print array's type.  */
      tmp = TREE_TYPE (t);
      while (TREE_CODE (TREE_TYPE (tmp)) == ARRAY_TYPE)
	tmp = TREE_TYPE (tmp);
      dump_c_node (buffer, TREE_TYPE (tmp), spc);
      
      /* Print variable's name.  */
      output_add_space (buffer);
      dump_c_node (buffer, t, spc);
      
      /* Print the dimensions.  */
      tmp = TREE_TYPE (t);
      while (TREE_CODE (tmp) == ARRAY_TYPE)
	{
	  output_add_character (buffer, '[');
	  output_decimal (buffer,
			  TREE_INT_CST_LOW (TYPE_SIZE (tmp)) / 
			  TREE_INT_CST_LOW (TYPE_SIZE (TREE_TYPE (tmp))));
	  output_add_character (buffer, ']');
	  tmp = TREE_TYPE (tmp);
	}
    }
  else
    {
      /* Print type declaration.  */
      dump_c_node (buffer, TREE_TYPE (t), spc);
      
      /* Print variable's name.  */
      output_add_space (buffer);
      dump_c_node (buffer, t, spc);
    }
  
  /* The initial value of a function serves to determine wether the function 
     is declared or defined.  So the following does not apply to function
     nodes.  */
  if (TREE_CODE (t) != FUNCTION_DECL)
    {
      /* Print the initial value.  */
      if (DECL_INITIAL (t))
	{
	  output_add_space (buffer);
	  output_add_character (buffer, '=');
	  output_add_space (buffer);
	  dump_c_node (buffer, DECL_INITIAL (t), spc);
	}
    }
  
  output_add_character (buffer, ';');
  output_add_newline (buffer);
  
}

/* Print the declaration of a function given its declaration node.  */

void
print_function_decl (buffer, node, spc)
     output_buffer *buffer;
     tree node;
     HOST_WIDE_INT spc;
{
  if (TREE_CODE_CLASS (TREE_CODE (node)) != 'd')
    NIY;
  
  INDENT (spc);

  /* Print the return type.  */
  dump_c_node (buffer, TREE_TYPE (TREE_TYPE (node)), 0);
  output_add_space (buffer);
  /* Print the namespace.  */
  dump_c_node (buffer, TREE_TYPE (node), 0);
  /* Print function name.  */
  output_printf (buffer, "::%s (", TREE_CODE (node) == NOP_EXPR ?
		 IDENTIFIER_POINTER (DECL_NAME (TREE_OPERAND (node, 0))) :
		 IDENTIFIER_POINTER (DECL_NAME (node)));
  
  /* Print the functions arguments.  */
  {
    tree tmp = TYPE_ARG_TYPES (TREE_TYPE (node));
    while (tmp && TREE_CHAIN (tmp) && tmp != error_mark_node)
      {
	dump_c_node (buffer, TREE_VALUE (tmp), 0);
	tmp = TREE_CHAIN (tmp);
	if (TREE_CHAIN (tmp) && TREE_CODE (TREE_CHAIN (tmp)) == TREE_LIST)
	  {
	    output_add_character (buffer, ',');
	    output_add_space (buffer);
	  }
      }
  }
  output_add_character (buffer, ')');
}

/* Prints a structure : name, fields, and methods.  
   FIXME : Not Implemented Yet correctly.  */

void 
print_struct_decl (buffer, node, spc)
     output_buffer *buffer;
     tree node;
     HOST_WIDE_INT spc;
{
  /* Print the name of the structure.  */
  if (TYPE_NAME (node))
    {
      INDENT (spc);
      if (TREE_CODE (node) == RECORD_TYPE)
	output_add_string (buffer, "struct ");
      else if (TREE_CODE (node) == UNION_TYPE)
	output_add_string (buffer, "union ");
      else
	NIY;
      dump_c_node (buffer, TYPE_NAME (node), spc);
    }

  /* Print the contents of the structure.  */  
  output_add_newline (buffer);
  INDENT (spc);
  output_add_character (buffer, '{');
  output_add_newline (buffer);
  
  /* Print the fields of the structure.  */
  {
    tree tmp;
    tmp = TYPE_FIELDS (node);
    while (tmp)
      {
	/* Avoid to print recursively the structure.  */
	/* FIXME : Not implemented correctly..., 
	   what about the case when we have a cycle in the contain graph? ...  
	   Maybe this could be solved by looking at the scope in which the structure
	   was declared.  */
	if (TREE_TYPE (tmp) != node 
	    || (TREE_CODE (TREE_TYPE (tmp)) == POINTER_TYPE && 
		TREE_TYPE (TREE_TYPE (tmp)) != node))
	  print_declaration (buffer, tmp, spc+2);
	else
	  {
	    
	  }
	tmp = TREE_CHAIN (tmp);
      }
  }
  INDENT (spc);
  output_add_character (buffer, '}');
}

/* Return the priority of the operator OP.  

   From lowest to highest precedence with either left-to-right (L-R)
   or right-to-left (R-L) associativity]:

     1	[L-R] ,
     2	[R-L] = += -= *= /= %= &= ^= |= <<= >>= 
     3	[R-L] ?: 
     4	[L-R] || 
     5	[L-R] && 
     6	[L-R] | 
     7	[L-R] ^ 
     8	[L-R] & 
     9	[L-R] == != 
    10	[L-R] < <= > >= 
    11	[L-R] << >> 
    12	[L-R] + - 
    13	[L-R] * / % 
    14	[R-L] ! ~ ++ -- + - * & (type) sizeof 
    15	[L-R] fn() [] -> . 

   unary +, - and * have higher precedence than the corresponding binary
   operators.  */

static int
op_prio (op)
     tree op;
{
  if (op == NULL)
    abort ();

  switch (TREE_CODE (op))
    {
    case TREE_LIST:
    case COMPOUND_EXPR:
      return 1;

    case MODIFY_EXPR:
      return 2;

    case COND_EXPR:
      return 3;

    case TRUTH_OR_EXPR:
    case TRUTH_ORIF_EXPR:
      return 4;

    case TRUTH_AND_EXPR:
    case TRUTH_ANDIF_EXPR:
      return 5;

    case BIT_IOR_EXPR:
      return 6;

    case BIT_XOR_EXPR:
    case TRUTH_XOR_EXPR:
      return 7;

    case BIT_AND_EXPR:
      return 8;
	
    case EQ_EXPR:
    case NE_EXPR:
      return 9;

    case LT_EXPR:
    case LE_EXPR:
    case GT_EXPR:
    case GE_EXPR:
      return 10;

    case LSHIFT_EXPR:
    case RSHIFT_EXPR:
      return 11;

    case PLUS_EXPR:
    case MINUS_EXPR:
      return 12;

    case MULT_EXPR:
    case TRUNC_DIV_EXPR:
    case CEIL_DIV_EXPR:
    case FLOOR_DIV_EXPR:
    case ROUND_DIV_EXPR:
    case RDIV_EXPR:
    case EXACT_DIV_EXPR:
    case TRUNC_MOD_EXPR:
    case CEIL_MOD_EXPR:
    case FLOOR_MOD_EXPR:
    case ROUND_MOD_EXPR:
      return 13;

    case TRUTH_NOT_EXPR:
    case BIT_NOT_EXPR:
    case POSTINCREMENT_EXPR:
    case POSTDECREMENT_EXPR:
    case PREINCREMENT_EXPR:
    case PREDECREMENT_EXPR:
    case NEGATE_EXPR:
    case INDIRECT_REF:
    case ADDR_EXPR:
    case FLOAT_EXPR:
    case NOP_EXPR:
    case CONVERT_EXPR:
    case FIX_TRUNC_EXPR:
    case FIX_CEIL_EXPR:
    case FIX_FLOOR_EXPR:
    case FIX_ROUND_EXPR:
      return 14;

    case CALL_EXPR:
    case ARRAY_REF:
    case COMPONENT_REF:
      return 15;

      /* Special expressions.  */
    case STMT_EXPR:
    case MIN_EXPR:
    case MAX_EXPR:
    case ABS_EXPR:
      return 16;

    case SAVE_EXPR:
    case NON_LVALUE_EXPR:
      return op_prio (TREE_OPERAND (op, 0));

    case EXPR_WITH_FILE_LOCATION:
      return op_prio (EXPR_WFL_NODE (op));

    default:
      /* If OP is any type of expression operator, abort because we
	 should know what its relative precedence is.  Otherwise, return
	 an arbitrarily high precedence to avoid surrounding single
	 VAR_DECLs in ()s.  */
      if (TREE_CODE_CLASS (TREE_CODE (op)) == '<'
	  || TREE_CODE_CLASS (TREE_CODE (op)) == '1'
	  || TREE_CODE_CLASS (TREE_CODE (op)) == '2'
	  || TREE_CODE_CLASS (TREE_CODE (op)) == 'e')
	{
	  fputs ("\n", stderr);
	  fprintf (stderr, "unhandled expression in op_prio(): ");
	  print_node_brief (stderr, "", op, 0);
	  fputs ("\n", stderr);
	  return -1;
	}
      else
	return 9999;
    }
}


/* Return the symbol associated with operator OP.  */

static const char *
op_symbol (op)
     tree op;
{
  if (op == NULL)
    abort ();
  
  switch (TREE_CODE (op))
    {
    case MODIFY_EXPR:
      return "=";
      
    case TRUTH_OR_EXPR:
    case TRUTH_ORIF_EXPR:
      return "||";
      
    case TRUTH_AND_EXPR:
    case TRUTH_ANDIF_EXPR:
      return "&&";
      
    case BIT_IOR_EXPR:
      return "|";
      
    case TRUTH_XOR_EXPR:
    case BIT_XOR_EXPR:
      return "^";
      
    case ADDR_EXPR:
    case BIT_AND_EXPR:
      return "&";
      
    case EQ_EXPR:
    case UNEQ_EXPR:
      return "==";
      
    case NE_EXPR:
      return "!=";
      
    case LT_EXPR:
    case UNLT_EXPR:
      return "<";
      
    case LE_EXPR:
    case UNLE_EXPR:
      return "<=";
      
    case GT_EXPR:
    case UNGT_EXPR:
      return ">";
      
    case GE_EXPR:
    case UNGE_EXPR:
      return ">=";
      
    case LSHIFT_EXPR:
      return "<<";
      
    case RSHIFT_EXPR:
      return ">>";
      
    case PLUS_EXPR:
      return "+";
      
    case NEGATE_EXPR:
    case MINUS_EXPR:
      return "-";
      
    case BIT_NOT_EXPR:
      return "~";

    case TRUTH_NOT_EXPR:
      return "!";
      
    case MULT_EXPR:
    case INDIRECT_REF:
      return "*";
      
    case TRUNC_DIV_EXPR:
    case CEIL_DIV_EXPR:
    case FLOOR_DIV_EXPR:
    case ROUND_DIV_EXPR:
    case RDIV_EXPR:
    case EXACT_DIV_EXPR:
      return "/";
      
    case TRUNC_MOD_EXPR:
    case CEIL_MOD_EXPR:
    case FLOOR_MOD_EXPR:
    case ROUND_MOD_EXPR:
      return "%";
      
    case PREDECREMENT_EXPR:
      return " --";
      
    case PREINCREMENT_EXPR:
      return " ++";
      
    case POSTDECREMENT_EXPR:
      return "-- ";
      
    case POSTINCREMENT_EXPR:
      return "++ ";
      
    case REFERENCE_EXPR:
      return "";
      
    default:
      return "<<< ??? >>>";
    }
}

/* Prints the name of a CALL_EXPR.  */

static void 
print_call_name (buffer, node)
     output_buffer *buffer;
     tree node;
{
  tree op0;

  if (TREE_CODE (node) != CALL_EXPR)
    abort ();

  op0 = TREE_OPERAND (node, 0);

  if (TREE_CODE (op0) == NON_LVALUE_EXPR)
    op0 = TREE_OPERAND (op0, 0);

  switch (TREE_CODE (op0))
    {
    case VAR_DECL:
    case PARM_DECL:
      PRINT_FUNCTION_NAME (op0);
      break;
      
    case ADDR_EXPR:
    case INDIRECT_REF:
    case NOP_EXPR:
      PRINT_FUNCTION_NAME (TREE_OPERAND (op0, 0));
      break;
      
    case COND_EXPR:
      PRINT_FUNCTION_NAME (TREE_OPERAND (TREE_OPERAND (op0, 0), 1));
      PRINT_FUNCTION_NAME (TREE_OPERAND (TREE_OPERAND (op0, 0), 2));
      break;
      
    case COMPONENT_REF:
      /* The function is a pointer contained in a structure.  */
      if (TREE_CODE (TREE_OPERAND (op0, 0)) == INDIRECT_REF ||
	  TREE_CODE (TREE_OPERAND (op0, 0)) == VAR_DECL)
	PRINT_FUNCTION_NAME (TREE_OPERAND (op0, 1));
      /* else
	 We can have several levels of structures and a function 
	 pointer inside.  This is not implemented yet...  */
      //		  NIY;
      break;
      
    case ARRAY_REF:
      if (TREE_CODE (TREE_OPERAND (op0, 0)) == VAR_DECL)
	PRINT_FUNCTION_NAME (TREE_OPERAND (op0, 0));
      else
	PRINT_FUNCTION_NAME (TREE_OPERAND (op0, 1));
      break;
      
    default:
      NIY;
    }
}

/* Parses the string STR and replaces new-lines by '\n', tabs by '\t', ...  */

static void 
pretty_print_string (buffer, str) 
     output_buffer *buffer;
     const char *str;
{
  if (str == NULL)
    return;

  while (*str)
    {
      switch (str[0])
	{
	case '\b':
	  output_add_string (buffer, "\\b");
	  break;
	  
	case '\f':
	  output_add_string (buffer, "\\f");
	  break;
	  
	case '\n':
	  output_add_string (buffer, "\\n");
	  break;
	  
	case '\r':
	  output_add_string (buffer, "\\r");
	  break;
	  
	case '\t':
	  output_add_string (buffer, "\\t");
	  break;
	  
	case '\v':
	  output_add_string (buffer, "\\v");
	  break;
	  
	case '\\':
	  output_add_string (buffer, "\\\\");
	  break;

	case '\"':
	  output_add_string (buffer, "\\\"");
	  break;

	case '\'':
	  output_add_string (buffer, "\\'");
	  break;

	case '\0':
	  output_add_string (buffer, "\\0");
	  break;

	case '\1':
	  output_add_string (buffer, "\\1");
	  break;

	case '\2':
	  output_add_string (buffer, "\\2");
	  break;

	case '\3':
	  output_add_string (buffer, "\\3");
	  break;

	case '\4':
	  output_add_string (buffer, "\\4");
	  break;

	case '\5':
	  output_add_string (buffer, "\\5");
	  break;

	case '\6':
	  output_add_string (buffer, "\\6");
	  break;

	case '\7':
	  output_add_string (buffer, "\\7");
	  break;

	default:
	  output_add_character (buffer, str[0]);
	  break;
	}
      str++;
    }
}

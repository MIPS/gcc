/* Pretty formatting of GENERIC trees in C syntax.
   Copyright (C) 2001, 2002 Free Software Foundation, Inc.
   Adapted from c-pretty-print.c by Diego Novillo <dnovillo@redhat.com>

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
#include "errors.h"
#include "tree.h"
#include "diagnostic.h"
#include "real.h"
#include "hashtab.h"
#include "tree-flow.h"
#include "langhooks.h"

/* Local functions, macros and variables.  */
static int op_prio			PARAMS ((tree));
static const char *op_symbol		PARAMS ((tree));
static void pretty_print_string		PARAMS ((output_buffer *, const char*));
static void print_call_name		PARAMS ((output_buffer *, tree));
static void newline_and_indent		PARAMS ((output_buffer *, int));
static inline void maybe_init_pretty_print PARAMS ((void));
static void print_declaration		PARAMS ((output_buffer *, tree, int,
      						 int));
static void print_struct_decl		PARAMS ((output_buffer *, tree, int));
static void dump_block_info		PARAMS ((output_buffer *, basic_block,
      						 int));
static void do_niy			PARAMS ((output_buffer *, tree));
static void dump_vops			PARAMS ((output_buffer *, tree, int));

#define INDENT(SPACE) do { \
  int i; for (i = 0; i<SPACE; i++) output_add_space (buffer); } while (0)

#define NIY do_niy(buffer,node)

#define PRINT_FUNCTION_NAME(NODE)  output_printf             \
  (buffer, "%s", TREE_CODE (NODE) == NOP_EXPR ?              \
   (*lang_hooks.decl_printable_name) (TREE_OPERAND (NODE, 0), 1) : \
   (*lang_hooks.decl_printable_name) (NODE, 1))

#define MASK_POINTER(P)	((unsigned)((unsigned long)node & 0xffff))

static output_buffer buffer;
static int initialized = 0;
static int last_bb;
static bool dumping_stmts;

/* Try to print something for an unknown tree code.  */

static void
do_niy (buffer, node)
     output_buffer *buffer;
     tree node;
{
  int i, len;

  output_add_string (buffer, "<<< Unknown tree: ");
  output_add_string (buffer, tree_code_name[(int) TREE_CODE (node)]);

  len = first_rtl_op (TREE_CODE (node));
  for (i = 0; i < len; ++i)
    {
      newline_and_indent (buffer, 2);
      dump_generic_node (buffer, TREE_OPERAND (node, i), 2, 0);
    }

  output_add_string (buffer, " >>>\n");
}
     
void 
debug_generic_expr (t)
     tree t;
{
  print_generic_expr (stderr, t, TDF_VOPS);
  fprintf (stderr, "\n");
}

void 
debug_generic_stmt (t)
     tree t;
{
  print_generic_stmt (stderr, t, TDF_VOPS);
  fprintf (stderr, "\n");
}

/* Print tree T, and its successors, on file FILE.  FLAGS specifies details
   to show in the dump.  See TDF_* in tree.h.  */

void 
print_generic_stmt (file, t, flags)
     FILE *file;
     tree t;
     int flags;
{
  maybe_init_pretty_print ();
  dumping_stmts = true;
  dump_generic_node (&buffer, t, 0, flags);
  fprintf (file, "%s", output_finalize_message (&buffer));
  output_clear_message_text (&buffer);
}


/* Print a single expression T on file FILE.  FLAGS specifies details to show
   in the dump.  See TDF_* in tree.h.  */

void 
print_generic_expr (file, t, flags)
     FILE *file;
     tree t;
     int flags;
{
  maybe_init_pretty_print ();
  dumping_stmts = false;
  dump_generic_node (&buffer, t, 0, flags);
  fprintf (file, "%s", output_finalize_message (&buffer));
  output_clear_message_text (&buffer);
}


/* Dump the node NODE on the output_buffer BUFFER, SPC spaces of indent.
   FLAGS specifies details to show in the dump (see TDF_* in tree.h).  */

int
dump_generic_node (buffer, node, spc, flags)
     output_buffer *buffer;
     tree node;
     int spc;
     int flags;
{
  tree type;
  tree op0, op1;
  const char* str;

  if (node == NULL_TREE)
    return spc;

  if (node != empty_stmt_node && node != error_mark_node)
    {
      basic_block curr_bb = bb_for_stmt (node);

      if ((flags & TDF_BLOCKS) && curr_bb && curr_bb->index != last_bb)
	dump_block_info (buffer, curr_bb, spc);

      if ((flags & TDF_VOPS) && stmt_ann (node))
	dump_vops (buffer, node, spc);

      if (curr_bb && curr_bb->index != last_bb)
	last_bb = curr_bb->index;
    }

  switch (TREE_CODE (node))
    {
    case ERROR_MARK:
      output_add_string (buffer, "<<< error >>>");
      break;

    case IDENTIFIER_NODE:
      output_add_identifier (buffer, node);
      break;

    case TREE_LIST:
      while (node && node != error_mark_node)
	{
	  if (TREE_PURPOSE (node))
	    {
	      dump_generic_node (buffer, TREE_PURPOSE (node), spc, flags);
	      output_add_space (buffer);
	    }
	  dump_generic_node (buffer, TREE_VALUE (node), spc, flags);
	  node = TREE_CHAIN (node);
	  if (node && TREE_CODE (node) == TREE_LIST)
	    {
	      output_add_character (buffer, ',');
	      output_add_space (buffer);
	    }
	}
      break;

    case TREE_VEC:
      dump_generic_node (buffer, BINFO_TYPE (node), spc, flags);
      break;

    case BLOCK:
      NIY;
      break;

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
	else if (quals & TYPE_QUAL_RESTRICT)
	  output_add_string (buffer, "restrict ");
	else if (quals & TYPE_QUAL_BOUNDED)
	  output_add_string (buffer, "bounded ");

	class = TREE_CODE_CLASS (TREE_CODE (node));

	if (class == 'd')
	  {
	    if (DECL_NAME (node))
	      output_add_identifier (buffer, DECL_NAME (node));
	    else
              output_add_string (buffer, "<unnamed type decl>");
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
		else
                  output_add_string (buffer, "<unnamed type>");
	      }
	    else
              output_add_string (buffer, "<unnamed type>");
	  }
	break;
      }

    case POINTER_TYPE:
    case REFERENCE_TYPE:
      str = (TREE_CODE (node) == POINTER_TYPE ? "*" : "&");

      if (TREE_CODE (TREE_TYPE (node)) == FUNCTION_TYPE)
        {
	  tree fnode = TREE_TYPE (node);
	  dump_generic_node (buffer, TREE_TYPE (fnode), spc, flags);
	  output_add_space (buffer);
	  output_add_character (buffer, '(');
	  output_add_string (buffer, str);
	  if (TYPE_NAME (node) && DECL_NAME (TYPE_NAME (node)))
	    output_add_string (buffer, IDENTIFIER_POINTER (DECL_NAME (TYPE_NAME (node))));
	  else
	    output_printf (buffer, "<UPFN%x>", MASK_POINTER (node));
	  
	  output_add_character (buffer, ')');
          output_add_space (buffer);
	  output_add_character (buffer, '(');
	  /* Print the argument types.  The last element in the list is a 
	     VOID_TYPE.  The following avoid to print the last element.  */
	  {
	    tree tmp = TYPE_ARG_TYPES (fnode);
	    while (tmp && TREE_CHAIN (tmp) && tmp != error_mark_node)
	      {
		dump_generic_node (buffer, TREE_VALUE (tmp), spc, flags);
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
      else
        {
	  unsigned int quals = TYPE_QUALS (node);

          dump_generic_node (buffer, TREE_TYPE (node), spc, flags);
	  output_add_space (buffer);
	  output_add_string (buffer, str);
	  
	  if (quals & TYPE_QUAL_CONST)
	    output_add_string (buffer, " const");
	  else if (quals & TYPE_QUAL_VOLATILE)
	    output_add_string (buffer,  "volatile");
	  else if (quals & TYPE_QUAL_RESTRICT)
	    output_add_string (buffer, " restrict");
	  else if (quals & TYPE_QUAL_BOUNDED)
	    output_add_string (buffer, " bounded");
	}
      break;

    case OFFSET_TYPE:
      NIY;
      break;

    case METHOD_TYPE:
      output_add_string (buffer, IDENTIFIER_POINTER 
			 (DECL_NAME (TYPE_NAME (TYPE_METHOD_BASETYPE (node)))));
      output_add_string (buffer, "::");
      break;

    case FILE_TYPE:
      NIY;
      break;

    case ARRAY_TYPE:
      {
	tree tmp;

	/* Print the array type.  */
	dump_generic_node (buffer, TREE_TYPE (node), spc, flags);

	/* Print the dimensions.  */
	tmp = node;
	while (tmp && TREE_CODE (tmp) == ARRAY_TYPE)
	  {
	    output_add_character (buffer, '[');
	    if (TYPE_SIZE (tmp))
	      {
		tree size = TYPE_SIZE (tmp);
		if (TREE_CODE (size) == INTEGER_CST)
		  output_decimal (buffer,
				  TREE_INT_CST_LOW (TYPE_SIZE (tmp)) / 
				  TREE_INT_CST_LOW (TYPE_SIZE (TREE_TYPE (tmp))));
		else if (TREE_CODE (size) == MULT_EXPR)
		  dump_generic_node (buffer, TREE_OPERAND (size, 0), spc, flags);
		/* else punt.  */
	      }
	    output_add_character (buffer, ']');
	    tmp = TREE_TYPE (tmp);
	  }
	break;
      }

    case SET_TYPE:
      NIY;
      break;

    case RECORD_TYPE:
    case UNION_TYPE:
      /* Print the name of the structure.  */
      if (TREE_CODE (node) == RECORD_TYPE)
	output_add_string (buffer, "struct ");
      else if (TREE_CODE (node) == UNION_TYPE)
	output_add_string (buffer, "union ");

      if (TYPE_NAME (node))
	dump_generic_node (buffer, TYPE_NAME (node), spc, flags);
      else
	print_struct_decl (buffer, node, spc);
      break;

    case QUAL_UNION_TYPE:
      NIY;
      break;


    case LANG_TYPE:
      NIY;
      break;

    case INTEGER_CST:
      if (TREE_CODE (TREE_TYPE (node)) == POINTER_TYPE)
	{
	  /* In the case of a pointer, one may want to divide by the
	     size of the pointed-to type.  Unfortunately, this not
	     straightforward.  The C front-end maps expressions 

	     (int *) 5
	     int *p; (p + 5)

	     in such a way that the two INTEGER_CST nodes for "5" have
	     different values but identical types.  In the latter
	     case, the 5 is multipled by sizeof (int) in c-common.c
	     (pointer_int_sum) to convert it to a byte address, and
	     yet the type of the node is left unchanged.  Argh.  What
	     is consistent though is that the number value corresponds
	     to bytes (UNITS) offset.

             NB: Neither of the following divisors can be trivially
             used to recover the original literal:

             TREE_INT_CST_LOW (TYPE_SIZE_UNIT (TREE_TYPE (node)))
	     TYPE_PRECISION (TREE_TYPE (TREE_TYPE (node)))  */
	  output_decimal (buffer, TREE_INT_CST_LOW (node));
	  output_add_string (buffer, "B"); /* pseudo-unit */
	}
      else if (! host_integerp (node, 0))
	{
	  tree val = node;

	  if (tree_int_cst_sgn (val) < 0)
	    {
	      output_add_character (buffer, '-');
	      val = build_int_2 (-TREE_INT_CST_LOW (val),
				 ~TREE_INT_CST_HIGH (val)
				 + !TREE_INT_CST_LOW (val));
	    }
	  /* Would "%x%0*x" or "%x%*0x" get zero-padding on all
	     systems?  */
	  {
	    static char format[10]; /* "%x%09999x\0" */
	    if (!format[0])
	      sprintf (format, "%%x%%0%dx", HOST_BITS_PER_INT / 4);
	    sprintf (buffer->digit_buffer, format, TREE_INT_CST_HIGH (val),
		     TREE_INT_CST_LOW (val));
	    output_add_string (buffer, buffer->digit_buffer);
	  }
	}
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
	    real_to_decimal (string, &d, sizeof (string), 0, 1);
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
      output_add_string (buffer, "__complex__ (");
      dump_generic_node (buffer, TREE_REALPART (node), spc, flags);
      output_add_string (buffer, ", ");
      dump_generic_node (buffer, TREE_IMAGPART (node), spc, flags);
      output_add_string (buffer, ")");
      break;

    case STRING_CST:
      output_add_string (buffer, "\"");
      pretty_print_string (buffer, TREE_STRING_POINTER (node));
      output_add_string (buffer, "\"");
      break;

    case FUNCTION_TYPE:
      break;

    case FUNCTION_DECL:
      output_add_identifier (buffer, DECL_NAME (node));
      break;

    case LABEL_DECL:
      if (DECL_NAME (node))
	output_add_string (buffer, IDENTIFIER_POINTER (DECL_NAME (node)));
      else
        output_printf (buffer, "<UL%x>", MASK_POINTER (node));
      break;

    case CONST_DECL:
      if (DECL_NAME (node))
	output_add_string (buffer, IDENTIFIER_POINTER (DECL_NAME (node)));
      else
        output_printf (buffer, "<UC%x>", MASK_POINTER (node));
      break;

    case TYPE_DECL:
      if (strcmp (TREE_FILENAME (node), "<built-in>") == 0)
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
	      /* The type is a c++ class: all structures have at least 
		 4 methods. */
	      output_add_string (buffer, "class ");
	      dump_generic_node (buffer, TREE_TYPE (node), spc, flags);
	    }
	  else
	    {
	      output_add_string (buffer, "struct ");
	      dump_generic_node (buffer, TREE_TYPE (node), spc, flags);
	      output_add_character (buffer, ';');
	      output_add_newline (buffer);
	    }
	}
      break;

    case VAR_DECL:
    case PARM_DECL:
      if (DECL_NAME (node))
	output_add_string (buffer, IDENTIFIER_POINTER (DECL_NAME (node)));
      else
        output_printf (buffer, "<UV%x>", MASK_POINTER (node));
      break;

    case RESULT_DECL:
      dump_generic_node (buffer, TREE_TYPE (node), spc, flags);
      break;

    case FIELD_DECL:
      if (DECL_NAME (node))
	output_add_string (buffer, IDENTIFIER_POINTER (DECL_NAME (node)));
      else
	output_printf (buffer, "<UF%x>", MASK_POINTER (node));
      break;

    case NAMESPACE_DECL:
      if (DECL_NAME (node))
	output_add_string (buffer, IDENTIFIER_POINTER (DECL_NAME (node)));
      else
        output_printf (buffer, "<UN%x>", MASK_POINTER (node));
      break;

    case COMPONENT_REF:
      op0 = TREE_OPERAND (node, 0);
      str = ".";
      if (TREE_CODE (op0) == INDIRECT_REF)
	{
	  op0 = TREE_OPERAND (op0, 0);
	  str = "->";
	}
      if (op_prio (op0) < op_prio (node))
	output_add_character (buffer, '(');
      dump_generic_node (buffer, op0, spc, flags);
      if (op_prio (op0) < op_prio (node))
	output_add_character (buffer, ')');
      output_add_string (buffer, str);
      dump_generic_node (buffer, TREE_OPERAND (node, 1), spc, flags);
      break;

    case BIT_FIELD_REF:
      output_add_string (buffer, "BIT_FIELD_REF <");
      dump_generic_node (buffer, TREE_OPERAND (node, 0), spc, flags);
      output_add_string (buffer, ", ");
      dump_generic_node (buffer, TREE_OPERAND (node, 1), spc, flags);
      output_add_string (buffer, ", ");
      dump_generic_node (buffer, TREE_OPERAND (node, 2), spc, flags);
      output_add_string (buffer, ">");
      break;

    case BUFFER_REF:
      NIY;
      break;

    case ARRAY_REF:
      op0 = TREE_OPERAND (node, 0);
      if (op_prio (op0) < op_prio (node))
	output_add_character (buffer, '(');
      dump_generic_node (buffer, op0, spc, flags);
      if (op_prio (op0) < op_prio (node))
	output_add_character (buffer, ')');
      output_add_character (buffer, '[');
      dump_generic_node (buffer, TREE_OPERAND (node, 1), spc, flags);
      output_add_character (buffer, ']');
      break;

    case ARRAY_RANGE_REF:
      NIY;
      break;

    case CONSTRUCTOR:
      {
	tree lnode;
	bool is_struct_init = FALSE;
	output_add_character (buffer, '{');     
	/*	dump_generic_node (buffer, TREE_OPERAND (node, 1), spc);  */
	lnode = TREE_OPERAND (node, 1);
	if (TREE_CODE (TREE_TYPE (node)) == RECORD_TYPE
	    || TREE_CODE (TREE_TYPE (node)) == UNION_TYPE)
	  is_struct_init = TRUE;
	while (lnode && lnode != error_mark_node)
	  {
	    tree val;
	    if (TREE_PURPOSE (lnode) && is_struct_init)
	      {
		output_add_character (buffer, '.');
		dump_generic_node (buffer, TREE_PURPOSE (lnode), spc, flags);
		output_add_string (buffer, "=");
	      }
	    val = TREE_VALUE (lnode);
	    if (val && TREE_CODE (val) == ADDR_EXPR)
	      if (TREE_CODE (TREE_OPERAND (val, 0)) == FUNCTION_DECL)
		val = TREE_OPERAND (val, 0);
	    if (val && TREE_CODE (val) == FUNCTION_DECL)	      
	      {
		if (DECL_NAME (val))
		  output_add_string (buffer, IDENTIFIER_POINTER (DECL_NAME (val)));
		else
		  output_printf (buffer, "<UF%x>", MASK_POINTER (val));
	      }
	    else
	      {
		dump_generic_node (buffer, TREE_VALUE (lnode), spc, flags);
	      }
	    lnode = TREE_CHAIN (lnode);
	    if (lnode && TREE_CODE (lnode) == TREE_LIST)
	      {
		output_add_character (buffer, ',');
		output_add_space (buffer);
	      }
	  }
	output_add_character (buffer, '}');
      }
      break;

    case COMPOUND_EXPR:
      if (dumping_stmts)
	{
	  dump_generic_node (buffer, TREE_OPERAND (node, 0), spc, flags);
	  if (!(flags & TDF_SLIM))
	    {
	      output_add_character (buffer, ';');
	      newline_and_indent (buffer, spc);
	      dump_generic_node (buffer, TREE_OPERAND (node, 1), spc, flags);
	    }
	}
      else
	{
	  dump_generic_node (buffer, TREE_OPERAND (node, 0), spc, flags);
	  output_add_character (buffer, ',');
	  output_add_space (buffer);
	  dump_generic_node (buffer, TREE_OPERAND (node, 1), spc, flags);
	}
      break;

    case MODIFY_EXPR:
    case INIT_EXPR:
      dump_generic_node (buffer, TREE_OPERAND (node, 0), spc, flags);
      output_add_space (buffer);
      output_add_character (buffer, '=');
      output_add_space (buffer);
      dump_generic_node (buffer, TREE_OPERAND (node, 1), spc, flags);
      break;

    case TARGET_EXPR:
      dump_generic_node (buffer, TYPE_NAME (TREE_TYPE (node)), spc, flags);
      output_add_character (buffer, '(');
      dump_generic_node (buffer, TARGET_EXPR_INITIAL (node), spc, flags);
      output_add_character (buffer, ')');
      break;

    case COND_EXPR:
      if (TREE_TYPE (node) == void_type_node)
	{
	  output_add_string (buffer, "if (");
	  dump_generic_node (buffer, COND_EXPR_COND (node), spc, flags);
	  output_add_character (buffer, ')');
	  if (!(flags & TDF_SLIM))
	    {
	      if (COND_EXPR_THEN (node) == empty_stmt_node)
		{
		  output_add_character (buffer, ';');
		}
	      else
		{
		  newline_and_indent (buffer, spc+2);
		  output_add_character (buffer, '{');
		  newline_and_indent (buffer, spc+4);
		  dump_generic_node (buffer, TREE_OPERAND (node, 1), spc+4,
		                     flags);
		  newline_and_indent (buffer, spc+2);
		  output_add_character (buffer, '}');
		}
	      if (COND_EXPR_ELSE (node) != empty_stmt_node)
		{
		  newline_and_indent (buffer, spc);
		  output_add_string (buffer, "else");
		  newline_and_indent (buffer, spc+2);
		  output_add_character (buffer, '{');
		  newline_and_indent (buffer, spc+4);
		  dump_generic_node (buffer, TREE_OPERAND (node, 2), spc+4,
			             flags);
		  newline_and_indent (buffer, spc+2);
		  output_add_character (buffer, '}');
		}
	    }
	}
      else
	{
	  dump_generic_node (buffer, TREE_OPERAND (node, 0), spc, flags);
	  output_add_space (buffer);
	  output_add_character (buffer, '?');
	  output_add_space (buffer);
	  dump_generic_node (buffer, TREE_OPERAND (node, 1), spc, flags);
	  output_add_space (buffer);
	  output_add_character (buffer, ':');
	  output_add_space (buffer);
	  dump_generic_node (buffer, TREE_OPERAND (node, 2), spc, flags);
	}
      break;

    case BIND_EXPR:
      output_add_character (buffer, '{');
      if (!(flags & TDF_SLIM))
	{
	  if (BIND_EXPR_VARS (node))
	    {
	      output_add_newline (buffer);

	      for (op0 = BIND_EXPR_VARS (node); op0; op0 = TREE_CHAIN (op0))
		print_declaration (buffer, op0, spc+2, flags);
	    }

	  newline_and_indent (buffer, spc+2);
	  dump_generic_node (buffer, BIND_EXPR_BODY (node), spc+2, flags);
	  newline_and_indent (buffer, spc);
	  output_add_character (buffer, '}');
	}
      break;

    case CALL_EXPR:
      print_call_name (buffer, node);

      /* Print parameters.  */
      output_add_space (buffer);
      output_add_character (buffer, '(');
      op1 = TREE_OPERAND (node, 1);
      if (op1)
	dump_generic_node (buffer, op1, spc, flags);
      output_add_character (buffer, ')');
      break;

    case METHOD_CALL_EXPR:
      NIY;
      break;

    case WITH_CLEANUP_EXPR:
      NIY;
      break;

    case CLEANUP_POINT_EXPR:
      output_add_string (buffer, "<<cleanup_point ");
      dump_generic_node (buffer, TREE_OPERAND (node, 0), spc, flags);
      output_add_string (buffer, ">>");
      break;

    case PLACEHOLDER_EXPR:
      NIY;
      break;

    case WITH_RECORD_EXPR:
      NIY;
      break;

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
	    dump_generic_node (buffer, op0, spc, flags);
	    output_add_character (buffer, ')');
	  }
	else
	  dump_generic_node (buffer, op0, spc, flags);

	output_add_space (buffer);
	output_add_string (buffer, op);
	output_add_space (buffer);

	/* When the operands are expressions with less priority, 
	   keep semantics of the tree representation.  */
	if (op_prio (op1) < op_prio (node))
	  {
	    output_add_character (buffer, '(');
	    dump_generic_node (buffer, op1, spc, flags);
	    output_add_character (buffer, ')');
	  }
	else
	  dump_generic_node (buffer, op1, spc, flags);
      }
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
	  dump_generic_node (buffer, TREE_OPERAND (node, 0), spc, flags);
	  output_add_character (buffer, ')');
	}
      else
	dump_generic_node (buffer, TREE_OPERAND (node, 0), spc, flags);
      break;

    case POSTDECREMENT_EXPR:
    case POSTINCREMENT_EXPR:
      if (op_prio (TREE_OPERAND (node, 0)) < op_prio (node))
	{
	  output_add_character (buffer, '(');
	  dump_generic_node (buffer, TREE_OPERAND (node, 0), spc, flags);
	  output_add_character (buffer, ')');
	}
      else
	dump_generic_node (buffer, TREE_OPERAND (node, 0), spc, flags);
      output_add_string (buffer, op_symbol (node));
      break;

    case MIN_EXPR:
      /* #define MIN(X,Y) ((X) < (Y) ? (X) : (Y))  */
      dump_generic_node (buffer, TREE_OPERAND (node, 0), spc, flags);
      output_add_string (buffer, " < ");
      dump_generic_node (buffer, TREE_OPERAND (node, 1), spc, flags);
      output_add_string (buffer, " ? ");
      dump_generic_node (buffer, TREE_OPERAND (node, 0), spc, flags);
      output_add_string (buffer, " : ");
      dump_generic_node (buffer, TREE_OPERAND (node, 1), spc, flags);
      break;

    case MAX_EXPR:
      /* #define MAX(X,Y) ((X) > (Y) ? (X) : (Y))  */
      dump_generic_node (buffer, TREE_OPERAND (node, 0), spc, flags);
      output_add_string (buffer, " > ");
      dump_generic_node (buffer, TREE_OPERAND (node, 1), spc, flags);
      output_add_string (buffer, " ? ");
      dump_generic_node (buffer, TREE_OPERAND (node, 0), spc, flags);
      output_add_string (buffer, " : ");
      dump_generic_node (buffer, TREE_OPERAND (node, 1), spc, flags);
      break;

    case ABS_EXPR:
      /* n < 0 ? -n : n */
      dump_generic_node (buffer, TREE_OPERAND (node, 0), spc, flags);
      output_add_string (buffer, " < 0 ? -");
      dump_generic_node (buffer, TREE_OPERAND (node, 0), spc, flags);
      output_add_string (buffer, " : ");
      dump_generic_node (buffer, TREE_OPERAND (node, 0), spc, flags);
      break;
      
    case FFS_EXPR:
      NIY;
      break;

    case UNORDERED_EXPR:
      NIY;
      break;

    case ORDERED_EXPR:
      NIY;
      break;

    case IN_EXPR:
      NIY;
      break;

    case SET_LE_EXPR:
      NIY;
      break;

    case CARD_EXPR:
      NIY;
      break;

    case RANGE_EXPR:
      NIY;
      break;

    case FIX_TRUNC_EXPR:
    case FIX_CEIL_EXPR:
    case FIX_FLOOR_EXPR:
    case FIX_ROUND_EXPR:
    case FLOAT_EXPR:
    case CONVERT_EXPR:
    case NOP_EXPR:
      type = TREE_TYPE (node);
      op0 = TREE_OPERAND (node, 0);
      if (type != TREE_TYPE (op0))
	{
	  output_add_character (buffer, '(');
	  dump_generic_node (buffer, type, spc, flags);
	  output_add_string (buffer, ")");
	}
      if (op_prio (op0) < op_prio (node))
	output_add_character (buffer, '(');
      dump_generic_node (buffer, op0, spc, flags);
      if (op_prio (op0) < op_prio (node))
	output_add_character (buffer, ')');
      break;

    case NON_LVALUE_EXPR:
      dump_generic_node (buffer, TREE_OPERAND (node, 0), spc, flags);
      break;

    case SAVE_EXPR:
      output_add_string (buffer, "SAVE_EXPR <");
      dump_generic_node (buffer, TREE_OPERAND (node, 0), spc, flags);
      output_add_character (buffer, '>');
      break;

    case UNSAVE_EXPR:
      output_add_string (buffer, "UNSAVE_EXPR <");
      dump_generic_node (buffer, TREE_OPERAND (node, 0), spc, flags);
      output_add_character (buffer, '>');
      break;

    case RTL_EXPR:
      NIY;
      break;

    case ENTRY_VALUE_EXPR:
      NIY;
      break;

    case COMPLEX_EXPR:
      output_add_string (buffer, "__complex__ (");
      dump_generic_node (buffer, TREE_OPERAND (node, 0), spc, flags);
      output_add_string (buffer, ", ");
      dump_generic_node (buffer, TREE_OPERAND (node, 1), spc, flags);
      output_add_string (buffer, ")");
      break;

    case CONJ_EXPR:
      output_add_string (buffer, "__builtin_conjf (");
      dump_generic_node (buffer, TREE_OPERAND (node, 0), spc, flags);
      break;

    case REALPART_EXPR:
      output_add_string (buffer, "__real__ ");
      dump_generic_node (buffer, TREE_OPERAND (node, 0), spc, flags);
      break;

    case IMAGPART_EXPR:
      output_add_string (buffer, "__imag__ ");
      dump_generic_node (buffer, TREE_OPERAND (node, 0), spc, flags);
      break;

    case VA_ARG_EXPR:
      output_add_string (buffer, "__builtin_va_arg (");
      dump_generic_node (buffer, TREE_OPERAND (node, 0), spc, flags);
      output_add_string (buffer, ")");
      break;

    case TRY_FINALLY_EXPR:
    case TRY_CATCH_EXPR:
      output_add_string (buffer, "try");
      newline_and_indent (buffer, spc+2);
      output_add_string (buffer, "{");
      newline_and_indent (buffer, spc+4);
      dump_generic_node (buffer, TREE_OPERAND (node, 0), spc+4, flags);
      newline_and_indent (buffer, spc+2);
      output_add_string (buffer, "}");
      newline_and_indent (buffer, spc);
      output_add_string (buffer,
			 (TREE_CODE (node) == TRY_CATCH_EXPR) ? "catch" : "finally");
      newline_and_indent (buffer, spc+2);
      output_add_string (buffer, "{");
      newline_and_indent (buffer, spc+4);
      dump_generic_node (buffer, TREE_OPERAND (node, 1), spc+4, flags);
      newline_and_indent (buffer, spc+2);
      output_add_string (buffer, "}");
      break;

    case CATCH_EXPR:
      output_add_string (buffer, "catch (");
      dump_generic_node (buffer, CATCH_TYPES (node), spc+2, flags);
      output_add_string (buffer, ")");
      newline_and_indent (buffer, spc+2);
      output_add_string (buffer, "{");
      newline_and_indent (buffer, spc+4);
      dump_generic_node (buffer, CATCH_BODY (node), spc+4, flags);
      newline_and_indent (buffer, spc+2);
      output_add_string (buffer, "}");
      break;

    case EH_FILTER_EXPR:
      output_add_string (buffer, "<<<eh_filter (");
      dump_generic_node (buffer, EH_FILTER_TYPES (node), spc+2, flags);
      output_add_string (buffer, ")>>>");
      break;

    case GOTO_SUBROUTINE_EXPR:
      NIY;
      break;

    case LABEL_EXPR:
      op0 = TREE_OPERAND (node, 0);
      /* If this is for break or continue, don't bother printing it.  */
      if (DECL_NAME (op0))
	{
	  const char *name = IDENTIFIER_POINTER (DECL_NAME (op0));
	  if (strcmp (name, "break") == 0
	      || strcmp (name, "continue") == 0)
	    break;
	}
      dump_generic_node (buffer, op0, spc, flags);
      output_add_character (buffer, ':');
      output_add_character (buffer, ';');
      break;

    case LABELED_BLOCK_EXPR:
      op0 = LABELED_BLOCK_LABEL (node);
      /* If this is for break or continue, don't bother printing it.  */
      if (DECL_NAME (op0))
	{
	  const char *name = IDENTIFIER_POINTER (DECL_NAME (op0));
	  if (strcmp (name, "break") == 0
	      || strcmp (name, "continue") == 0)
	    {
	      dump_generic_node (buffer, LABELED_BLOCK_BODY (node), spc, flags);
	      break;
	    }
	}
      dump_generic_node (buffer, LABELED_BLOCK_LABEL (node), spc, flags);
      output_add_string (buffer, ": {");
      if (!(flags & TDF_SLIM))
	newline_and_indent (buffer, spc+2);
      dump_generic_node (buffer, LABELED_BLOCK_BODY (node), spc+2, flags);
      if (!flags)
	newline_and_indent (buffer, spc);
      output_add_character (buffer, '}');
      break;

    case EXIT_BLOCK_EXPR:
      op0 = LABELED_BLOCK_LABEL (EXIT_BLOCK_LABELED_BLOCK (node));
      /* If this is for a break or continue, print it accordingly.  */
      if (DECL_NAME (op0))
	{
	  const char *name = IDENTIFIER_POINTER (DECL_NAME (op0));
	  if (strcmp (name, "break") == 0
	      || strcmp (name, "continue") == 0)
	    {
	      output_add_string (buffer, name);
	      break;
	    }
	}
      output_add_string (buffer, "<<<exit block ");
      dump_generic_node (buffer, op0, spc, flags);
      output_add_string (buffer, ">>>");
      break;

    case EXC_PTR_EXPR:
      output_add_string (buffer, "<<<exception object>>>");
      break;

    case LOOP_EXPR:
      output_add_string (buffer, "while (1)");
      if (!(flags & TDF_SLIM))
	{
	  newline_and_indent (buffer, spc+2);
	  output_add_character (buffer, '{');
	  newline_and_indent (buffer, spc+4);
	  dump_generic_node (buffer, LOOP_EXPR_BODY (node), spc+4, flags);
	  newline_and_indent (buffer, spc+2);
	  output_add_character (buffer, '}');
	}
      break;

    case RETURN_EXPR:
      output_add_string (buffer, "return");
      op0 = TREE_OPERAND (node, 0);
      if (op0)
	{
	  output_add_space (buffer);
	  if (TREE_CODE (op0) == MODIFY_EXPR)
	    dump_generic_node (buffer, TREE_OPERAND (op0, 1), spc, flags);
	  else
	    dump_generic_node (buffer, op0, spc, flags);
	}
      output_add_character (buffer, ';');
      break;

    case EXIT_EXPR:
      output_add_string (buffer, "if (");
      dump_generic_node (buffer, TREE_OPERAND (node, 0), spc, flags);
      output_add_string (buffer, ") break;");
      break;

    case SWITCH_EXPR:
      output_add_string (buffer, "switch (");
      dump_generic_node (buffer, SWITCH_COND (node), spc, flags);
      output_add_character (buffer, ')');
      if (!(flags & TDF_SLIM))
	{
	  newline_and_indent (buffer, spc+2);
	  output_add_character (buffer, '{');
	  newline_and_indent (buffer, spc+4);
	  dump_generic_node (buffer, SWITCH_BODY (node), spc+4, flags);
	  newline_and_indent (buffer, spc+2);
	  output_add_character (buffer, '}');
	}
      break;

    case GOTO_EXPR:
      op0 = GOTO_DESTINATION (node);
      if (TREE_CODE (op0) != SSA_NAME
	  && DECL_P (op0)
	  && DECL_NAME (op0))
	{
	  const char *name = IDENTIFIER_POINTER (DECL_NAME (op0));
	  if (strcmp (name, "break") == 0
	      || strcmp (name, "continue") == 0)
	    {
	      output_add_string (buffer, name);
	      break;
	    }
	}
      output_add_string (buffer, "goto ");
      dump_generic_node (buffer, op0, spc, flags);
      output_add_character (buffer, ';');
      break;

    case ASM_EXPR:
      INDENT (spc);
      output_add_string (buffer, "__asm__");
      if (ASM_VOLATILE_P (node))
	output_add_string (buffer, " __volatile__");
      output_add_character (buffer, '(');
      dump_generic_node (buffer, ASM_STRING (node), spc, flags);
      output_add_character (buffer, ':');
      dump_generic_node (buffer, ASM_OUTPUTS (node), spc, flags);
      output_add_character (buffer, ':');
      dump_generic_node (buffer, ASM_INPUTS (node), spc, flags);
      if (ASM_CLOBBERS (node))
	{
	  output_add_character (buffer, ':');
	  dump_generic_node (buffer, ASM_CLOBBERS (node), spc, flags);
	}
      output_add_string (buffer, ");");
      if (!(flags & TDF_SLIM))
	output_add_newline (buffer);      
      break;
      
    case CASE_LABEL_EXPR:
      if (CASE_LOW (node) && CASE_HIGH (node))
	{
	  output_add_string (buffer, "case ");
	  dump_generic_node (buffer, CASE_LOW (node), spc, flags);
	  output_add_string (buffer, " ... ");
	  dump_generic_node (buffer, CASE_HIGH (node), spc, flags);
	}
      else if (CASE_LOW (node))
	{
	  output_add_string (buffer, "case ");
	  dump_generic_node (buffer, CASE_LOW (node), spc, flags);
	}
      else
	output_add_string (buffer, "default ");
      output_add_character (buffer, ':');
      break;

    case VTABLE_REF:
      output_add_string (buffer, "VTABLE_REF <(");
      dump_generic_node (buffer, TREE_OPERAND (node, 0), spc, flags);
      output_add_string (buffer, "),");
      dump_generic_node (buffer, TREE_OPERAND (node, 1), spc, flags);
      output_add_character (buffer, ',');
      dump_generic_node (buffer, TREE_OPERAND (node, 2), spc, flags);
      output_add_character (buffer, '>');
      break;

    case EPHI_NODE:
      {
	int i;
	
	output_add_string (buffer, " EPHI (");
	dump_generic_node (buffer, EREF_NAME (node), spc, flags);
	output_add_string (buffer, ") ");
	output_add_character (buffer, '[');
	output_add_string (buffer, "class:");
	output_decimal (buffer, EREF_CLASS (node));
	output_add_string (buffer, " downsafe:");
	output_decimal (buffer, EPHI_DOWNSAFE (node));
	output_add_string (buffer, " can_be_avail:");
	output_decimal (buffer, EPHI_CAN_BE_AVAIL (node));
	output_add_string (buffer, " later:");
	output_decimal (buffer, EPHI_LATER (node));
	output_add_string (buffer, " bb:");
	output_decimal (buffer, bb_for_stmt (node)->index);
	output_add_character (buffer, ']');
	output_add_string (buffer, " <");
	for (i = 0; i < EPHI_NUM_ARGS (node); i++)
	  {
	    newline_and_indent (buffer, spc + 4);
	    dump_generic_node (buffer, EPHI_ARG_DEF (node, i), 
			       spc + 4, flags);
	  }
	output_add_string (buffer, " >");
      }
      break;
    case EEXIT_NODE:
    case ELEFT_NODE:
    case EKILL_NODE:
      if (TREE_CODE (node) == EEXIT_NODE)
	output_add_string (buffer, "EEXIT (");
      else if (TREE_CODE (node) == ELEFT_NODE)
	output_add_string (buffer, "ELEFT (");
      else if (TREE_CODE (node) == EKILL_NODE)
	output_add_string (buffer, "EKILL (");
      dump_generic_node (buffer, EREF_NAME (node), spc, flags);
      output_add_string (buffer, ") ");
      output_add_character (buffer, '[');
      output_add_string (buffer, "class:");
      output_decimal (buffer, EREF_CLASS (node));
      output_add_string (buffer, " bb:");
      output_decimal (buffer, bb_for_stmt (node)->index);
      output_add_character (buffer, ']');
      break;
    case EUSE_NODE:
      output_add_string (buffer, "EUSE (");
      dump_generic_node (buffer, EREF_NAME (node), spc, flags);
      output_add_string (buffer, ") ");
      output_add_character (buffer, '[');
      output_add_string (buffer, "class:");
      output_decimal (buffer, EREF_CLASS (node));
      output_add_string (buffer, " phiop:");
      output_decimal (buffer, EUSE_PHIOP (node));
      output_add_string (buffer, " bb:");
      output_decimal (buffer, bb_for_stmt (node)->index);
      if (EUSE_PHIOP (node))
	{
	  output_add_string (buffer, " has real use:");
	  output_decimal (buffer, EUSE_HAS_REAL_USE (node));
	  if (!(flags & TDF_SLIM))
	    {
	      output_add_string (buffer, " defined by:");
	      newline_and_indent (buffer, spc + 4);
	      /* If you remove the TDF_SLIM, you will get infinite
		 loops due to cycles in the factored graph in some
		 cases. They are *supposed* to occur, but we don't track
		 which nodes we've already printed, thus, we need to
		 stop at one level of printing. */
	      dump_generic_node (buffer, EUSE_DEF (node),
				 spc + 4, flags | TDF_SLIM);
	    }
	  newline_and_indent (buffer, spc);
	}
      output_add_character (buffer, ']');
      
      break;
    case PHI_NODE:
      {
	int i;

	dump_generic_node (buffer, PHI_RESULT (node), spc, flags);
	output_add_string (buffer, " = PHI <");
	for (i = 0; i < PHI_NUM_ARGS (node); i++)
	  {
	    dump_generic_node (buffer, PHI_ARG_DEF (node, i),
			       spc, flags);
	    if (i < PHI_NUM_ARGS (node) - 1)
	      output_add_string (buffer, ", ");
	  }
	output_add_string (buffer, ">;");
      }
      break;

    case SSA_NAME:
      if (TREE_CODE (SSA_NAME_VAR (node)) == INDIRECT_REF)
	output_add_string (buffer, "(");
      dump_generic_node (buffer, SSA_NAME_VAR (node), spc, flags);
      if (TREE_CODE (SSA_NAME_VAR (node)) == INDIRECT_REF)
	output_add_string (buffer, ")");
      output_add_string (buffer, "_");
      output_decimal (buffer, SSA_NAME_VERSION (node));
      break;

    case VDEF_EXPR:
      dump_generic_node (buffer, VDEF_RESULT (node), spc, flags);
      output_add_string (buffer, " = VDEF <");
      dump_generic_node (buffer, VDEF_OP (node), spc, flags);
      output_add_string (buffer, ">;");
      output_add_newline (buffer);      
      break;

    default:
      NIY;
    }

  return spc;
}

/* Print the declaration of a variable.  */

static void
print_declaration (buffer, t, spc, flags)
     output_buffer *buffer;
     tree t;
     int spc;
     int flags;
{
  /* Don't print type declarations.  */
  if (TREE_CODE (t) == TYPE_DECL)
    return;
      
  INDENT (spc);
  
  if (DECL_REGISTER (t))
    output_add_string (buffer, "register ");

  if (TREE_PUBLIC (t) && DECL_EXTERNAL (t))
    output_add_string (buffer, "extern ");
  else if (TREE_STATIC (t))
    output_add_string (buffer, "static ");

  /* Print the type and name.  */
  if (TREE_CODE (TREE_TYPE (t)) == ARRAY_TYPE)
    {
      tree tmp;
      
      /* Print array's type.  */
      tmp = TREE_TYPE (t);
      while (TREE_CODE (TREE_TYPE (tmp)) == ARRAY_TYPE)
	tmp = TREE_TYPE (tmp);
      dump_generic_node (buffer, TREE_TYPE (tmp), spc, 0);
      
      /* Print variable's name.  */
      output_add_space (buffer);
      dump_generic_node (buffer, t, spc, 0);
      
      /* Print the dimensions.  */
      tmp = TREE_TYPE (t);
      while (TREE_CODE (tmp) == ARRAY_TYPE)
	{
	  output_add_character (buffer, '[');
	  if (TYPE_DOMAIN (tmp))
	    {
	      if (TREE_CODE (TYPE_SIZE (tmp)) == INTEGER_CST)
		output_decimal (buffer,
				TREE_INT_CST_LOW (TYPE_SIZE (tmp)) / 
				TREE_INT_CST_LOW (TYPE_SIZE (TREE_TYPE (tmp))));
	      else
		dump_generic_node (buffer, TYPE_SIZE_UNIT (tmp), spc, 0);
	    }
	  output_add_character (buffer, ']');
	  tmp = TREE_TYPE (tmp);
	}
    }
  else
    {
      /* Print type declaration.  */
      dump_generic_node (buffer, TREE_TYPE (t), spc, 0);
      
      /* Print variable's name.  */
      output_add_space (buffer);
      dump_generic_node (buffer, t, spc, 0);
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
	  dump_generic_node (buffer, DECL_INITIAL (t), spc, 0);
	}
    }
  
  output_add_character (buffer, ';');
  if (!(flags & TDF_SLIM))
    output_add_newline (buffer);
}


/* Prints a structure: name, fields, and methods.  
   FIXME: Still incomplete.  */

static void 
print_struct_decl (buffer, node, spc)
     output_buffer *buffer;
     tree node;
     int spc;
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
      dump_generic_node (buffer, TYPE_NAME (node), spc, 0);
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
	  print_declaration (buffer, tmp, spc+2, 0);
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
    return 9999;

  switch (TREE_CODE (op))
    {
    case TREE_LIST:
    case COMPOUND_EXPR:
    case BIND_EXPR:
      return 1;

    case MODIFY_EXPR:
    case INIT_EXPR:
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
    case LROTATE_EXPR:
    case RROTATE_EXPR:
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
    case TARGET_EXPR:
      return 14;

    case CALL_EXPR:
    case ARRAY_REF:
    case COMPONENT_REF:
      return 15;

      /* Special expressions.  */
    case MIN_EXPR:
    case MAX_EXPR:
    case ABS_EXPR:
    case REALPART_EXPR:
    case IMAGPART_EXPR:
      return 16;

    case SAVE_EXPR:
    case NON_LVALUE_EXPR:
      return op_prio (TREE_OPERAND (op, 0));

    default:
      /* Return an arbitrarily high precedence to avoid surrounding single
	 VAR_DECLs in ()s.  */
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
      dump_generic_node (buffer, TREE_OPERAND (op0, 0), 0, 0);
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
      else
	dump_generic_node (buffer, TREE_OPERAND (op0, 0), 0, 0);
      /* else
	 We can have several levels of structures and a function 
	 pointer inside.  This is not implemented yet...  */
      /*		  NIY;*/
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

static void
maybe_init_pretty_print ()
{
  last_bb = -1;

  if (!initialized)
    {
      init_output_buffer (&buffer, /* prefix */NULL, /* line-width */0);
      initialized = 1;
    }
}

static void
newline_and_indent (buffer, spc)
     output_buffer *buffer;
     int spc;
{
  output_add_newline (buffer);
  INDENT (spc);
}


static void
dump_block_info (buffer, bb, spc)
     output_buffer *buffer;
     basic_block bb;
     int spc;
{
  if (bb)
    {
      edge e;
      tree *stmt_p = bb->head_tree_p;
      int lineno;

      newline_and_indent (buffer, spc);
      output_formatted_scalar (buffer, "# BLOCK %d", bb->index);

      if (stmt_p
	  && is_exec_stmt (*stmt_p)
	  && (lineno = get_lineno (*stmt_p)) > 0)
	{
	  output_add_string (buffer, " (");
	  output_add_string (buffer, get_filename (*stmt_p));
	  output_formatted_scalar (buffer, ":%d", lineno);
	  output_add_string (buffer, ")");
	}

      output_add_string (buffer, ".  PRED:");
      for (e = bb->pred; e; e = e->pred_next)
	if (e->src)
	  output_formatted_scalar (buffer, " %d", e->src->index);

      output_add_string (buffer, ".  SUCC:");
      for (e = bb->succ; e; e = e->succ_next)
	if (e->dest)
	  output_formatted_scalar (buffer, " %d", e->dest->index);

      output_add_character (buffer, '.');

      newline_and_indent (buffer, spc);
    }
}


static void
dump_vops (buffer, stmt, spc)
     output_buffer *buffer;
     tree stmt;
     int spc;
{
  size_t i;
  basic_block bb;
  varray_type vdefs = vdef_ops (stmt);
  varray_type vuses = vuse_ops (stmt);

  bb = bb_for_stmt (stmt);
  if (bb && bb->index != last_bb && bb->aux)
    {
      tree phi;

      for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
	{
	  output_add_string (buffer, "#   ");
	  dump_generic_node (buffer, phi, spc, 0);
	  newline_and_indent (buffer, spc);
	}
    }

  if (vdefs || vuses)
    newline_and_indent (buffer, spc);

  if (vdefs)
    for (i = 0; i < VARRAY_ACTIVE_SIZE (vdefs); i++)
      {
	tree vdef = VARRAY_TREE (vdefs, i);
	output_add_string (buffer, "#   ");
	dump_generic_node (buffer, vdef, spc, 0);
	INDENT (spc);
      }

  if (vuses)
    for (i = 0; i < VARRAY_ACTIVE_SIZE (vuses); i++)
      {
	tree vuse = VARRAY_TREE (vuses, i);
	output_add_string (buffer, "#   VUSE <");
	dump_generic_node (buffer, vuse, spc, 0);
	output_add_string (buffer, ">;");
	newline_and_indent (buffer, spc);
      }
}

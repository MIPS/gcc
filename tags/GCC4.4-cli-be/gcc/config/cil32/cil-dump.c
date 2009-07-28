/* CIL IR dump.

   Copyright (C) 2009 Free Software Foundation, Inc.

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
Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.

Author:
   Andrea Ornstein
   Erven Rohou

Contact information at STMicroelectronics:
Andrea C. Ornstein      <andrea.ornstein@st.com>
Contact information at INRIA:
Erven Rohou             <erven.rohou@inria.fr>
*/

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "real.h"
#include "tree.h"
#include "langhooks.h"
#include "pointer-set.h"

#include "cil-builtins.h"
#include "cil-refs.h"
#include "cil-types.h"
#include "cil-stack.h"


void dump_cil (void);
static void dump_cil_stmt (const_cil_stmt stmt, cil_stack stack);
static void dump_label_name (const_tree);
static void dump_decl_name (const_tree);
static void dump_valuetype_name (const_tree);
static void dump_fun (const_tree);
static void dump_type (const_tree type);


static const char* const cil_names[] = {
#define CIL_INSTRDEF(A,B,C,D) D,
#include "cil-instr.def"
#undef CIL_INSTRDEF
};

static const char* const cil_type_names[] = {
#define CIL_TYPEDEF(A,B) B,
#include "cil-types.def"
#undef CIL_TYPEDEF
};

/* Dump the name of a label.  */

static void
dump_label_name (const_tree label)
{
  printf ("?L" HOST_WIDE_INT_PRINT_DEC, (HOST_WIDE_INT) LABEL_DECL_UID (label));
}

/* Dump the name of a _DECL node pointed by NODE.  */

static void
dump_decl_name (const_tree node)
{
  if (DECL_ASSEMBLER_NAME_SET_P (node) || DECL_NAME (node))
    printf ("'%s'",IDENTIFIER_POINTER (DECL_NAME (node)));
  else
    printf ("'?UNNAMED%d'", DECL_UID (node));
}

/* Dump the name of a valuetype.
   T must be an aggregate type or an enumeral type, since these are
   the types emitted as CIL valuetypes.  */

static void
dump_valuetype_name (const_tree t)
{
  tree name = TYPE_NAME (t);
  const char *str = NULL;


  if (TREE_CODE (name) == IDENTIFIER_NODE)
    str = IDENTIFIER_POINTER (name);
  else if (DECL_NAME (name))
    str = IDENTIFIER_POINTER (DECL_NAME (name));

  if (str)
    printf ("'%s'", str);
  else
    printf ("'?UNNAMED%d'", DECL_UID (name));
}

/* Dump the signature of function type FUN_TYPE.
   The function name that is dumped is taken from function_decl FUN.
   FUN must be a FUNCTION_DECL.   */

static void
dump_fun (const_tree fun)
{
  tree fun_type = TREE_TYPE (fun);
  tree args_type;
  tree last_arg_type = NULL;
  bool varargs = false;

  args_type = TYPE_ARG_TYPES (fun_type);

  last_arg_type = tree_last (args_type);

  if (last_arg_type && TREE_VALUE (last_arg_type) != void_type_node)
    {
      last_arg_type = NULL;
      varargs = true;
    }

  printf ("%s", varargs ? "vararg " : "");
  dump_type (TREE_TYPE (fun_type));
  printf (" '%s'(", IDENTIFIER_POINTER (DECL_NAME (fun)));


  while (args_type != last_arg_type)
    {
      dump_type (TREE_VALUE (args_type));
      args_type = TREE_CHAIN (args_type);

      if (args_type != last_arg_type)
	printf (", ");
    }

  printf (")");
}

/* Dump type TYPE.
   NODE must be a type node.   */

static void
dump_type (const_tree type)
{
  if (cil_value_type_p (type))
    {
      printf ("value_type ");
      dump_valuetype_name (TYPE_MAIN_VARIANT (type));
    }
  else if (cil_pointer_type_p (type))
    {
      if (TREE_CODE (TREE_TYPE (type)) == FUNCTION_TYPE)
        {
          printf ("method_pointer");
        }
      else
        {
          dump_type (TREE_TYPE (type));
          printf (" *");
        }
    }
  else
    {
      cil_type_t cil_type = type_to_cil (type);
      printf ("%s", cil_type_names [cil_type]);
    }
}

/* Dump a CIL statement: opcode and arguments.  If stack is not NULL, also dump
   the stack depth and the type of the top-of-stack BEFORE the statement is
   executed.  */

static void
dump_cil_stmt (const_cil_stmt stmt, cil_stack stack)
{
  enum cil_opcode opcode = cil_opcode (stmt);

  if (stack)
    {
      const char* top_type = "";
      size_t depth = cil_stack_depth (stack);
      if (depth)
	top_type = cil_type_names [cil_stack_top (stack)];
      else
	top_type = "<empty>";

      printf ("\n[%d] %-11s   %s", depth, top_type, cil_names[opcode]);
    }
  else
    {
      printf ("\n\t%s", cil_names[opcode]);
    }

  if (cil_prefix_volatile (stmt))
      printf (" volatile");
  if (cil_prefix_unaligned (stmt) != 0)
      printf (" unaligned %d", cil_prefix_unaligned (stmt));

  switch (opcode_arg_type (opcode))
    {
    case CIL_VAR:
      printf ("\t");
      dump_decl_name (cil_var (stmt));
      break;

    case CIL_TYPE:
      dump_type (cil_type (stmt));
      break;

    case CIL_FIELD:
      {
	tree field = cil_field (stmt);

	if (TREE_CODE (field) == STRING_CST)
	  {
	    tree domain = TYPE_DOMAIN (TREE_TYPE (field));
	    tree max = TYPE_MAX_VALUE (domain);
	    HOST_WIDE_INT fsize = tree_low_cst (max, 1) + 1;
	    printf ("string_type"HOST_WIDE_INT_PRINT_UNSIGNED" '?string%u'",
		    fsize, get_string_cst_id (field));
	  }
	else
	  {
	    printf(" ");

	    if (COMPLETE_TYPE_P (TREE_TYPE (field)))
	      dump_type (TREE_TYPE (field));
	    else
	      printf ("%s(incomplete_type)", cil_type_names [CIL_NATIVE_INT]);
	    printf (" ");

	    switch (opcode)
	      {
	      case CIL_LDFLD:
	      case CIL_LDFLDA:
	      case CIL_STFLD:
		dump_valuetype_name (DECL_FIELD_CONTEXT (field));
		printf ("::");
		break;
	      default:
		break;
	      }

	    dump_decl_name (field);
	  }
      }
      break;

    case CIL_LABEL:
      dump_label_name (cil_label (stmt));
      break;

    case CIL_LABELS:
      gcc_assert (opcode == CIL_SWITCH);
      {
	unsigned int n_cases = cil_switch_ncases (stmt);
	unsigned HOST_WIDE_INT i;

	printf ("\t(\n");
	printf ("\t\tdefault: ");
	dump_label_name (CASE_LABEL (cil_switch_default (stmt)));
	printf ("\n");

	for (i = 0; i < n_cases - 1; i++)
	  {
	    HOST_WIDE_INT lo = cil_switch_case_low (stmt, i);
	    HOST_WIDE_INT hi = cil_switch_case_high (stmt, i);

	    printf ("\t\t["HOST_WIDE_INT_PRINT_UNSIGNED"-"HOST_WIDE_INT_PRINT_UNSIGNED"]: ",
		    lo, hi);
	    dump_label_name (cil_switch_case_label (stmt, i));
	    printf ("\n");
	  }
	printf ("\t\t)");
      }
      break;

    case CIL_FUNC:
      dump_fun (cil_func (stmt));
      break;

    case CIL_FCALL:
      {
	tree fdecl = cil_call_fdecl (stmt);

	printf("\t");

	if (cil_prefix_tail (stmt))
	  printf ("tailcall");

	if (DECL_BUILT_IN (fdecl))
	  printf ("builtin ");

	if (cil_call_vararg_p (stmt))
	  printf("vararg ");

	if (DECL_BUILT_IN (fdecl) && DECL_BUILT_IN_CLASS (fdecl) == BUILT_IN_MD)
	  printf ("%s", IDENTIFIER_POINTER (DECL_NAME (fdecl)));
	else
	  dump_decl_name (fdecl);
      }
      break;

    case CIL_CST:
      switch (opcode)
	{
	case CIL_LDC_I4:
	case CIL_LDC_I8:
	  printf ("\t");
	  dump_double_int (stdout, TREE_INT_CST (cil_cst (stmt)), false);
	  break;

	case CIL_LDC_R4:
	case CIL_LDC_R8:
	  {
	    REAL_VALUE_TYPE d = TREE_REAL_CST (cil_cst (stmt));
	    tree type = TYPE_MAIN_VARIANT (TREE_TYPE (cil_cst (stmt)));
	    char string[100];
	    long buf[2] = { 0, 0};

	    real_to_target (buf, &d, TYPE_MODE (type));
	    real_to_decimal (string, &d, sizeof (string), 0, 1);
	    printf ("\t(%#08lx%08lx)\t/* %s */", buf[1], buf[0], string);
	  }
	  break;

	default:
	  gcc_unreachable ();
	}
      break;

    case CIL_STRING:
      gcc_assert (opcode == CIL_ASM);
      printf ("\n\t// BEGIN ASM"
	      "\n\t%s"
	      "\n\t// END ASM",
	      TREE_STRING_POINTER (cil_string (stmt)));
      break;

    case CIL_NONE:
      break;

    default:
      gcc_unreachable ();
    }
}


/* Dump the IR for a given basic block.  The state of the stack is updated
   after each statement.  */

static void
dump_cil_bb (basic_block bb, cil_stack stack)
{
  cil_stmt_iterator csi;
  cil_stmt stmt = NULL;
  tree label;

  /* Dump this block label */
  label = gimple_block_label (bb);
  printf ("\n");
  dump_label_name (label);
  printf (":\n");

  for (csi = csi_start_bb (bb); !csi_end_p (csi); csi_next (&csi))
    {
      stmt = csi_stmt (csi);
      dump_cil_stmt (stmt, stack);
      cil_stack_after_stmt (stack, stmt);
    }
}


/* Dump the IR for the current function.  */

void
dump_cil (void)
{
  basic_block bb;
  cil_bb_stacks bbs;
  cil_stack stack;

  bbs = cil_bb_stacks_create ();

  printf ("%s:", lang_hooks.decl_printable_name (current_function_decl, 1));

  FOR_EACH_BB (bb)
    {
      stack = cil_stack_for_bb (bbs, bb);
      dump_cil_bb (bb, stack);
    }
  printf ("\n");
}

/*
 * Local variables:
 * eval: (c-set-style "gnu")
 * End:
 */

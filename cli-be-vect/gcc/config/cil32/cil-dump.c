/* CIL IR dump.

   Copyright (C) 2009-2010 Free Software Foundation, Inc.

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


void dump_cil_function_to_file (tree fn, FILE *file, int flags);

void dump_cil_stmt (FILE * file, const_cil_stmt stmt, cil_stack stack);
void dump_cil_label_name (FILE *, const_tree);
void dump_cil_decl_name (FILE *, const_tree);
void dump_cil_valuetype_name (FILE *, const_tree);
void dump_cil_fun_signature (FILE *, const_tree);
void dump_cil_type (FILE *, const_tree type);


static const char* const cil_names[] = {
#define CIL_INSTRDEF(A,B,C,D) D,
#include "cil-instr.def"
#undef CIL_INSTRDEF
};

const char* const cil_type_names[] = {
#define CIL_TYPEDEF(A,B) B,
#include "cil-types.def"
#undef CIL_TYPEDEF
};

/* Dump the name of a label.  */

void
dump_cil_label_name (FILE * file, const_tree label)
{
  fprintf (file, "?L" HOST_WIDE_INT_PRINT_DEC, (HOST_WIDE_INT) LABEL_DECL_UID (label));
}

/* Dump the name of a _DECL node pointed by NODE.  */

void
dump_cil_decl_name (FILE * file, const_tree node)
{
  if (DECL_ASSEMBLER_NAME_SET_P (node) || DECL_NAME (node))
    fprintf (file, "'%s'",IDENTIFIER_POINTER (DECL_NAME (node)));
  else
    fprintf (file, "'?UNNAMED%d'", DECL_UID (node));
}

/* Dump the name of a valuetype.
   T must be an aggregate type or an enumeral type, since these are
   the types emitted as CIL valuetypes.  */

void
dump_cil_valuetype_name (FILE * file, const_tree t)
{
  tree name = TYPE_NAME (t);
  const char *str = NULL;

  if (name == NULL)
    {
      fprintf (file, "noname?");
      return;
    }

  if (TREE_CODE (name) == IDENTIFIER_NODE)
    str = IDENTIFIER_POINTER (name);
  else if (DECL_NAME (name))
    str = IDENTIFIER_POINTER (DECL_NAME (name));

  if (str)
    fprintf (file, "'%s'", str);
  else
    fprintf (file, "'?UNNAMED%d'", DECL_UID (name));
}

/* Dump the signature of function type FUN_TYPE.
   The function name that is dumped is taken from function_decl FUN.
   FUN must be a FUNCTION_DECL.   */

void
dump_cil_fun_signature (FILE * file, const_tree fun)
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

  fprintf (file, "%s", varargs ? "vararg " : "");
  dump_cil_type (file, TREE_TYPE (fun_type));
  fprintf (file, " '%s'(", IDENTIFIER_POINTER (DECL_NAME (fun)));


  while (args_type != last_arg_type)
    {
      dump_cil_type (file, TREE_VALUE (args_type));
      args_type = TREE_CHAIN (args_type);

      if (args_type != last_arg_type)
	fprintf (file, ", ");
    }

  fprintf (file, ")");
}

/* Dump type TYPE.
   NODE must be a type node.   */

void
dump_cil_type (FILE * file, const_tree type)
{
  if (cil_value_type_p (type))
    {
      fprintf (file, "value_type ");
      dump_cil_valuetype_name (file, TYPE_MAIN_VARIANT (type));
    }
  else if (cil_pointer_type_p (type))
    {
      if (TREE_CODE (TREE_TYPE (type)) == FUNCTION_TYPE)
        {
          fprintf (file, "method_pointer");
        }
      else
        {
          dump_cil_type (file, TREE_TYPE (type));
          fprintf (file, " *");
        }
    }
  else
    {
      cil_type_t cil_type = type_to_cil (type);
      fprintf (file, "%s", cil_type_names [cil_type]);
    }
}

/* Dump a CIL statement: opcode and arguments.  If stack is not NULL, also dump
   the stack depth and the type of the top-of-stack BEFORE the statement is
   executed.  */

void
dump_cil_stmt (FILE * file, const_cil_stmt stmt, cil_stack stack)
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

      fprintf (file, "\n[%d] %-14s   %s", depth, top_type, cil_names[opcode]);
    }
  else
    {
      fprintf (file, "\n\t%s", cil_names[opcode]);
    }

  if (cil_prefix_volatile (stmt))
    fprintf (file, " volatile");
  if (cil_prefix_unaligned (stmt) != 0)
    fprintf (file, " unaligned %d", cil_prefix_unaligned (stmt));
  if ((opcode == CIL_VEC_CTOR) && cil_short_ctor (stmt))
    fprintf (file, " short ctor");

  switch (opcode_arg_type (opcode))
    {
    case CIL_VAR:
      fprintf (file, "\t");
      dump_cil_decl_name (file, cil_var (stmt));
      break;

    case CIL_TYPE:
      dump_cil_type (file, cil_type (stmt));
      break;

    case CIL_FIELD:
      {
	tree field = cil_field (stmt);

	if (TREE_CODE (field) == STRING_CST)
	  {
	    tree domain = TYPE_DOMAIN (TREE_TYPE (field));
	    tree max = TYPE_MAX_VALUE (domain);
	    HOST_WIDE_INT fsize = tree_low_cst (max, 1) + 1;
	    fprintf (file, "string_type"HOST_WIDE_INT_PRINT_UNSIGNED" '?string%u'",
		    fsize, get_string_cst_id (field));
	  }
	else
	  {
	    fprintf (file, " ");

	    if (COMPLETE_TYPE_P (TREE_TYPE (field)))
	      dump_cil_type (file, TREE_TYPE (field));
	    else
	      fprintf (file, "%s(incomplete_type)", cil_type_names [CIL_NATIVE_INT]);
	    fprintf (file, " ");

	    switch (opcode)
	      {
	      case CIL_LDFLD:
	      case CIL_LDFLDA:
	      case CIL_STFLD:
		dump_cil_valuetype_name (file, DECL_FIELD_CONTEXT (field));
		fprintf (file, "::");
		break;
	      default:
		break;
	      }

	    dump_cil_decl_name (file, field);
	  }
      }
      break;

    case CIL_LABEL:
      dump_cil_label_name (file, cil_label (stmt));
      break;

    case CIL_LABELS:
      gcc_assert (opcode == CIL_SWITCH);
      {
	unsigned int n_cases = cil_switch_ncases (stmt);
	unsigned HOST_WIDE_INT i;

	fprintf (file, "\t(\n");
	fprintf (file, "\t\tdefault: ");
	dump_cil_label_name (file, CASE_LABEL (cil_switch_default (stmt)));
	fprintf (file, "\n");

	for (i = 0; i < n_cases - 1; i++)
	  {
	    HOST_WIDE_INT lo = cil_switch_case_low (stmt, i);
	    HOST_WIDE_INT hi = cil_switch_case_high (stmt, i);

	    fprintf (file, "\t\t["HOST_WIDE_INT_PRINT_UNSIGNED"-"HOST_WIDE_INT_PRINT_UNSIGNED"]: ",
		    lo, hi);
	    dump_cil_label_name (file, cil_switch_case_label (stmt, i));
	    fprintf (file, "\n");
	  }
	fprintf (file, "\t\t)");
      }
      break;

    case CIL_FUNC:
      dump_cil_fun_signature (file, cil_func (stmt));
      break;

    case CIL_FCALL:
      {
	tree fdecl = cil_call_fdecl (stmt);

	fprintf (file, "\t");

	if (cil_prefix_tail (stmt))
	  fprintf (file, "tailcall");

	if (DECL_BUILT_IN (fdecl))
	  fprintf (file, "builtin ");

	if (cil_call_vararg_p (stmt))
	  fprintf (file, "vararg ");

	if (DECL_BUILT_IN (fdecl) && DECL_BUILT_IN_CLASS (fdecl) == BUILT_IN_MD)
	  fprintf (file, "%s", IDENTIFIER_POINTER (DECL_NAME (fdecl)));
	else
	  dump_cil_decl_name (file, fdecl);
      }
      break;

    case CIL_CST:
      switch (opcode)
	{
	case CIL_LDC_I4:
	case CIL_LDC_I8:
	  fprintf (file, "\t");
	  dump_double_int (file, TREE_INT_CST (cil_cst (stmt)), false);
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
	    fprintf (file, "\t(%#08lx%08lx)\t/* %s */", buf[1], buf[0], string);
	  }
	  break;

	default:
	  gcc_unreachable ();
	}
      break;

    case CIL_STRING:
      gcc_assert (opcode == CIL_ASM);
      fprintf (file, "\n\t// BEGIN ASM"
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

void
dump_cil_bb (FILE * file, basic_block bb, cil_stack stack)
{
  cil_stmt_iterator csi;
  cil_stmt stmt = NULL;
  tree label;

  /* Dump this block label */
  label = gimple_block_label (bb);
  fprintf (file, "\n");
  dump_cil_label_name (file, label);
  fprintf (file, ":\n");

  for (csi = csi_start_bb (bb); !csi_end_p (csi); csi_next (&csi))
    {
      stmt = csi_stmt (csi);
      dump_cil_stmt (file, stmt, stack);
      cil_stack_after_stmt (stack, stmt);
    }
}


/* Dump the IR for the current function.  */

void
dump_cil_function_to_file (tree fn, FILE *file, int flags __attribute__((unused)))
{
  basic_block bb;
  cil_bb_stacks bbs;
  cil_stack stack;

  bbs = cil_bb_stacks_create ();

  fprintf (file, "%s:", lang_hooks.decl_printable_name (fn, 1));

  FOR_EACH_BB (bb)
    {
      stack = cil_stack_for_bb (bbs, bb);
      dump_cil_bb (file, bb, stack);
    }
  fprintf (file, "\n");
}

/*
 * Local variables:
 * eval: (c-set-style "gnu")
 * End:
 */

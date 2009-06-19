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

#include "cil-types.h"
#include "cil-stack.h"
#include "emit-cil.h"


void dump_cil (void);
static void dump_cil_stmt (const_cil_stmt stmt, cil_stack stack);


static const char* const cil_names[] = {
  [CIL_ADD] = "ADD",
  [CIL_AND] = "AND",
  [CIL_ARGLIST] = "ARGLIST",
  [CIL_BEQ] = "BEQ",
  [CIL_BGE] = "BGE",
  [CIL_BGE_UN] = "BGE_UN",
  [CIL_BGT] = "BGT",
  [CIL_BGT_UN] = "BGT_UN",
  [CIL_BLE] = "BLE",
  [CIL_BLE_UN] = "BLE_UN",
  [CIL_BLT] = "BLT",
  [CIL_BLT_UN] = "BLT_UN",
  [CIL_BNE_UN] = "BNE_UN",
  [CIL_BR] = "BR",
  [CIL_BREAK] = "BREAK",
  [CIL_BRFALSE] = "BRFALSE",
  [CIL_BRTRUE] = "BRTRUE",
  [CIL_CALL] = "CALL",
  [CIL_CALLI] = "CALLI",
  [CIL_CEQ] = "CEQ",
  [CIL_CGT] = "CGT",
  [CIL_CGT_UN] = "CGT_UN",
  [CIL_CKFINITE] = "CKFINITE",
  [CIL_CLT] = "CLT",
  [CIL_CLT_UN] = "CLT_UN",
  [CIL_CONV_I1] = "CONV_I1",
  [CIL_CONV_I2] = "CONV_I2",
  [CIL_CONV_I4] = "CONV_I4",
  [CIL_CONV_I8] = "CONV_I8",
  [CIL_CONV_R4] = "CONV_R4",
  [CIL_CONV_R8] = "CONV_R8",
  [CIL_CONV_U1] = "CONV_U1",
  [CIL_CONV_U2] = "CONV_U2",
  [CIL_CONV_U4] = "CONV_U4",
  [CIL_CONV_U8] = "CONV_U8",
  [CIL_CONV_I] = "CONV_I",
  [CIL_CONV_U] = "CONV_U",
  [CIL_CONV_R_UN] = "CONV_R_UN",
  [CIL_CPBLK] = "CPBLK",
  [CIL_DIV] = "DIV",
  [CIL_DIV_UN] = "DIV_UN",
  [CIL_DUP] = "DUP",
  [CIL_INITBLK] = "INITBLK",
  [CIL_INITOBJ] = "INITOBJ",
  [CIL_JMP] = "JMP",
  [CIL_LDARG] = "LDARG",
  [CIL_LDARGA] = "LDARGA",
  [CIL_LDC_I4] = "LDC_I4",
  [CIL_LDC_I8] = "LDC_I8",
  [CIL_LDC_R4] = "LDC_R4",
  [CIL_LDC_R8] = "LDC_R8",
  [CIL_LDFLD] = "LDFLD",
  [CIL_LDFLDA] = "LDFLDA",
  [CIL_LDFTN] = "LDFTN",
  [CIL_LDIND_I1] = "LDIND_I1",
  [CIL_LDIND_I2] = "LDIND_I2",
  [CIL_LDIND_I4] = "LDIND_I4",
  [CIL_LDIND_I8] = "LDIND_I8",
  [CIL_LDIND_U1] = "LDIND_U1",
  [CIL_LDIND_U2] = "LDIND_U2",
  [CIL_LDIND_U4] = "LDIND_U4",
  [CIL_LDIND_U8] = "LDIND_U8",
  [CIL_LDIND_R4] = "LDIND_R4",
  [CIL_LDIND_R8] = "LDIND_R8",
  [CIL_LDIND_I] = "LDIND_I",
  [CIL_LDLOC] = "LDLOC",
  [CIL_LDLOCA] = "LDLOCA",
  [CIL_LDOBJ] = "LDOBJ",
  [CIL_LDVEC] = "LDVEC",
  [CIL_LDSFLD] = "LDSFLD",
  [CIL_LDSFLDA] = "LDSFLDA",
  [CIL_LOCALLOC] = "LOCALLOC",
  [CIL_MUL] = "MUL",
  [CIL_NEG] = "NEG",
  [CIL_NEWOBJ] = "NEWOBJ",
  [CIL_NOT] = "NOT",
  [CIL_OR] = "OR",
  [CIL_POP] = "POP",
  [CIL_REM] = "REM",
  [CIL_REM_UN] = "REM_UN",
  [CIL_RET] = "RET",
  [CIL_SHL] = "SHL",
  [CIL_SHR] = "SHR",
  [CIL_SHR_UN] = "SHR_UN",
  [CIL_STARG] = "STARG",
  [CIL_STFLD] = "STFLD",
  [CIL_STIND_I1] = "STIND_I1",
  [CIL_STIND_I2] = "STIND_I2",
  [CIL_STIND_I4] = "STIND_I4",
  [CIL_STIND_I8] = "STIND_I8",
  [CIL_STIND_R4] = "STIND_R4",
  [CIL_STIND_R8] = "STIND_R8",
  [CIL_STIND_I] = "STIND_I",
  [CIL_STLOC] = "STLOC",
  [CIL_STOBJ] = "STOBJ",
  [CIL_STVEC] = "STVEC",
  [CIL_STSFLD] = "STSFLD",
  [CIL_SUB] = "SUB",
  [CIL_SWITCH] = "SWITCH",
  [CIL_VEC_CTOR] = "VEC_CTOR",
  [CIL_XOR] = "XOR",
  [CIL_ASM] = "ASM"
};


static const char* const cil_type_names[] = {
  [CIL_NO_TYPE] = "no type??",
  [CIL_INT8] = "int8",
  [CIL_INT16] = "int16",
  [CIL_INT32] = "int32",
  [CIL_INT64] = "int64",
  [CIL_NATIVE_INT] = "native_int",
  [CIL_UNSIGNED_INT8] = "uint8",
  [CIL_UNSIGNED_INT16] = "uint16",
  [CIL_UNSIGNED_INT32] = "uint32",
  [CIL_UNSIGNED_INT64] = "uint64",
  [CIL_NATIVE_UNSIGNED_INT] = "native_uint",
  [CIL_FLOAT] = "float",
  [CIL_FLOAT32] = "float32",
  [CIL_FLOAT64] = "float64",
  [CIL_POINTER] = "pointer",
  [CIL_V2SI] = "v2si",
  [CIL_V4HI] = "v4hi",
  [CIL_V8QI] = "v8qi",
  [CIL_V2SF] = "v2sf",
  [CIL_V2DI] = "v2di",
  [CIL_V4SI] = "v4si",
  [CIL_V8HI] = "v8hi",
  [CIL_V16QI] = "v16qi",
  [CIL_V2DF] = "v2df",
  [CIL_V4SF] = "v4sf",
  [CIL_VALUE_TYPE] = "value_type"
};


/* Dump a CIL statement: opcode and arguments.  If stack is not NULL, also dump
   the stack depth and the type of the top-of-stack BEFORE the statement is
   executed.  */

static void
dump_cil_stmt (const_cil_stmt stmt, cil_stack stack)
{
  enum cil_opcode opcode = cil_opcode (stmt);
  const char* top_type = "";

  if (stack)
    {
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

  emit_prefixes (stdout, stmt);

  switch (opcode)
    {
    case CIL_ADD:
    case CIL_AND:
    case CIL_ARGLIST:
    case CIL_CEQ:
    case CIL_CGT:
    case CIL_CGT_UN:
    case CIL_CKFINITE:
    case CIL_CLT:
    case CIL_CLT_UN:
    case CIL_CONV_I1:
    case CIL_CONV_I2:
    case CIL_CONV_I4:
    case CIL_CONV_I8:
    case CIL_CONV_R4:
    case CIL_CONV_R8:
    case CIL_CONV_U1:
    case CIL_CONV_U2:
    case CIL_CONV_U4:
    case CIL_CONV_U8:
    case CIL_CONV_I:
    case CIL_CONV_U:
    case CIL_CONV_R_UN:
    case CIL_CPBLK:
    case CIL_DIV:
    case CIL_DIV_UN:
    case CIL_DUP:
    case CIL_INITBLK:
    case CIL_LDIND_I1:
    case CIL_LDIND_I2:
    case CIL_LDIND_I4:
    case CIL_LDIND_I8:
    case CIL_LDIND_U1:
    case CIL_LDIND_U2:
    case CIL_LDIND_U4:
    case CIL_LDIND_U8:
    case CIL_LDIND_R4:
    case CIL_LDIND_R8:
    case CIL_LDIND_I:
    case CIL_LOCALLOC:
    case CIL_MUL:
    case CIL_NEG:
    case CIL_NOT:
    case CIL_OR:
    case CIL_POP:
    case CIL_REM:
    case CIL_REM_UN:
    case CIL_RET:
    case CIL_SHL:
    case CIL_SHR:
    case CIL_SHR_UN:
    case CIL_STIND_I1:
    case CIL_STIND_I2:
    case CIL_STIND_I4:
    case CIL_STIND_I8:
    case CIL_STIND_R4:
    case CIL_STIND_R8:
    case CIL_STIND_I:
    case CIL_SWITCH:
    case CIL_SUB:
    case CIL_XOR:
    case CIL_ASM:
      break;

    case CIL_BEQ:
    case CIL_BGE:
    case CIL_BGE_UN:
    case CIL_BGT:
    case CIL_BGT_UN:
    case CIL_BLE:
    case CIL_BLE_UN:
    case CIL_BLT:
    case CIL_BLT_UN:
    case CIL_BNE_UN:
    case CIL_BR:
    case CIL_BREAK:
    case CIL_BRFALSE:
    case CIL_BRTRUE:
      {
	dump_label_name (stdout, cil_label (stmt));
      }
      break;

    case CIL_CALL:
    case CIL_CALLI:
    case CIL_JMP:
    case CIL_NEWOBJ:
      {
	tree fdecl;

	printf("\t");

	if (DECL_BUILT_IN (cil_call_fdecl (stmt)))
	  printf ("builtin ");
	fdecl = cil_call_fdecl (stmt);

	if (cil_call_vararg_p (stmt))
	  printf("vararg ");

	if (DECL_BUILT_IN (fdecl))
	  {
	    if (DECL_BUILT_IN_CLASS (fdecl) == BUILT_IN_MD)
	      printf ("%s", IDENTIFIER_POINTER (DECL_NAME (fdecl)));
	    else
	      dump_decl_name (stdout, fdecl);
	  }
	else
	  {
	    dump_decl_name (stdout, fdecl);
	  }
      }
      break;

    case CIL_INITOBJ:
    case CIL_LDOBJ:
    case CIL_LDVEC:
    case CIL_STOBJ:
    case CIL_STVEC:
    case CIL_VEC_CTOR:
      {
	tree type = cil_type (stmt);
	if (TREE_CODE (type) == VECTOR_TYPE)
	  {
	    cil_type_t cil_type = scalar_to_cil (type);
	    printf (" %s ", cil_type_names [cil_type]);
	  }
      }
      break;

    case CIL_LDLOC:
    case CIL_LDLOCA:
    case CIL_STARG:
    case CIL_STLOC:
    case CIL_LDARG:
    case CIL_LDARGA:
      printf ("\t");
      dump_decl_name (stdout, cil_var (stmt));
      break;

    case CIL_LDC_I4:
    case CIL_LDC_I8:
      printf ("\t");
      dump_double_int (stdout, TREE_INT_CST (cil_cst (stmt)), false);
      break;

    case CIL_LDC_R4:
      {
	REAL_VALUE_TYPE d = TREE_REAL_CST (cil_cst (stmt));
	tree type = TYPE_MAIN_VARIANT (TREE_TYPE (cil_cst (stmt)));
	char string[100];
	long buf;

	real_to_target (&buf, &d, TYPE_MODE (type));
	real_to_decimal (string, &d, sizeof (string), 0, 1);
	printf ("\tfloat32(%#08lx)\t/* %s */", buf, string);
      }
      break;

    case CIL_LDC_R8:
      {
	REAL_VALUE_TYPE d = TREE_REAL_CST (cil_cst (stmt));
	tree type = TYPE_MAIN_VARIANT (TREE_TYPE (cil_cst (stmt)));
	char string[100];
	long buf[2];

	real_to_target (buf, &d, TYPE_MODE (type));
	real_to_decimal (string, &d, sizeof (string), 0, 1);
	printf ("\tfloat64(%#08lx%08lx)\t/* %s */", buf[1], buf[0], string);
      }
      break;

    case CIL_LDFLD:
    case CIL_LDFLDA:
    case CIL_LDSFLD:
      {
	emit_ldfld (stdout, stmt);
      }
      break;

    case CIL_LDSFLDA:
      {
	tree field = cil_field (stmt);
	if (TREE_CODE (field) == STRING_CST)
	  {
	    dump_string_type (stdout, TREE_TYPE (field));
	    fprintf (stdout, " ");
	    printf ("\t'?string%u'", get_string_cst_id (field));
	    dump_string_name (stdout, field);
	  }
	else
	  {
	    gcc_assert (TREE_CODE (field) == VAR_DECL);
	    if (COMPLETE_TYPE_P (TREE_TYPE (field)))
	      {
		printf ("\t%s", tree_code_name[TREE_CODE (TREE_TYPE (field))]);
		dump_type (stdout, TREE_TYPE (field), true, false);
	      }
	    else
	      printf ("\tnative int");
	    printf (" ");
	    dump_decl_name (stdout, field);
	  }
      }
      break;

    case CIL_LDFTN:
      /* FIXME: We shouldn't print the vararg ellipsis */
      dump_fun_type (stdout, TREE_TYPE (cil_func (stmt)), cil_func (stmt),
		     NULL, true);
      break;

    case CIL_STFLD:
      {
	emit_stfld (stdout, stmt);
      }
      break;

     case CIL_STSFLD:
      {
	emit_stsfld (stdout, stmt);
      }
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
  block_stmt_iterator bsi = bsi_start (bb);
  tree label;
  cil_stmt stmt = NULL;

  /* Dump this block label */
  label = LABEL_EXPR_LABEL (bsi_stmt (bsi));
  printf ("\n");
  printf ("?L" HOST_WIDE_INT_PRINT_DEC, LABEL_DECL_UID (label));
  if (DECL_NAME (label))
    printf ("_%s", IDENTIFIER_POINTER (DECL_NAME (label)));
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

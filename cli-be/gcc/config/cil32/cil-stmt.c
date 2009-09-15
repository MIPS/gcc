/* CIL statements, sequences and iterators implementation.

   Copyright (C) 2006-2009 Free Software Foundation, Inc.

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

Authors:
   Andrea Ornstein
   Erven Rohou
   Gabriele Svelto

Contact information at STMicroelectronics:
Andrea C. Ornstein      <andrea.ornstein@st.com>
Contact information at INRIA:
Erven Rohou             <erven.rohou@inria.fr>
*/

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "ggc.h"
#include "tree.h"

#include "cil-types.h"
#include "cil-refs.h"
#include "cil-stmt.h"

/******************************************************************************
 * Globals                                                                    *
 ******************************************************************************/

/* A cache of cil_seq objects.  Sequences are created and destroyed fairly
   often so it's better to keep the fred ones around.  */
static GTY ((deletable)) struct cil_seq_d *cil_seq_cache = NULL;


/* Returns the type of argument of the opcode specified by the OPCODE
   argument. */

enum cil_arg_type opcode_arg_types[] = {
#define CIL_INSTRDEF(A,B,C,D) B,
#include "cil-instr.def"
#undef CIL_INSTRDEF
};

/******************************************************************************
 * Local function prototypes                                                  *
 ******************************************************************************/

static void update_bb_for_cil_stmts (cil_seq_node, basic_block);
static void csi_insert_seq_nodes_before (cil_stmt_iterator *, cil_seq_node,
					 cil_seq_node,
					 enum csi_iterator_update);
static void csi_insert_seq_nodes_after (cil_stmt_iterator *, cil_seq_node,
					cil_seq_node, enum csi_iterator_update);

/******************************************************************************
 * CIL statement functions                                                    *
 ******************************************************************************/

/* Create a new CIL statement with no arguments.  */

cil_stmt
cil_build_stmt (enum cil_opcode opcode)
{
  cil_stmt stmt;

  gcc_assert (opcode_arg_type (opcode) == CIL_NONE);

  stmt = GGC_CNEW (struct cil_stmt_d);
  stmt->opcode = opcode;

  return stmt;
}

/* Create a new CIL statement with one argument. CIL SWITCH, CALL, CALLI and
   JMP statements cannot be built with this function and must be built with the
   appropriate functions.  */

cil_stmt
cil_build_stmt_arg (enum cil_opcode opcode, tree arg)
{
  cil_stmt stmt;

  gcc_assert (opcode_arg_type (opcode) != CIL_NONE);

  stmt = GGC_CNEW (struct cil_stmt_d);
  stmt->opcode = opcode;

  switch (opcode_arg_type (opcode))
    {
    case CIL_VAR:    stmt->arg.var = arg;    break;
    case CIL_TYPE:   stmt->arg.type = arg;   break;
    case CIL_FIELD:  stmt->arg.field = arg;  break;
    case CIL_LABEL:  stmt->arg.label = arg;  break;
    case CIL_FUNC:   stmt->arg.func = arg;   break;
    case CIL_CST:    stmt->arg.cst = arg;    break;
    case CIL_STRING: stmt->arg.str = arg;    break;
    default:         gcc_unreachable ();
    }

  return stmt;
}

/* Create a new CIL switch statement with the case / label couples taken from
   a TREE_VEC of CASE_LABELs as used in a GIMPLE SWITCH_EXPR.  */

cil_stmt
cil_build_switch (tree labels)
{
  cil_stmt stmt = GGC_CNEW (struct cil_stmt_d);
  cil_switch_arg arg = GGC_NEW (struct cil_switch_arg_d);
  unsigned int i, length = TREE_VEC_LENGTH (labels);

  gcc_assert (length != 0);
  arg->ncases = length;
  arg->cases = GGC_NEWVEC (tree, length);

  /* This sets the default case correctly as it is the last element in both the
     TREE_VEC and in the cases array.  */
  for (i = 0; i < length; i++)
    arg->cases[i] = TREE_VEC_ELT (labels, i);

  stmt->opcode = CIL_SWITCH;
  stmt->arg.labels = arg;

  return stmt;
}

/* Build a generic CIL call statement. The OPCODE parameter specifies the type
   of jump. The TYPE_OR_DECL parameter holds the function declaration or the
   function type depending if the call is direct or not.  */

cil_stmt
cil_build_call_generic (enum cil_opcode opcode, tree type_or_decl)
{
  cil_stmt stmt = GGC_CNEW (struct cil_stmt_d);
  cil_call_arg arg = GGC_CNEW (struct cil_call_arg_d);
  tree arg_types;
  size_t i;

  gcc_assert (opcode_arg_type (opcode) == CIL_FCALL);

  if (opcode != CIL_CALLI)
    {
      arg->ftype = TREE_TYPE (type_or_decl);
      arg->fdecl = type_or_decl;
    }
  else
    arg->ftype = type_or_decl;

  arg_types = TYPE_ARG_TYPES (arg->ftype);

  if (arg_types == NULL)
    {
      arg->nargs = 0;
      arg->arguments = NULL;
    }
  else
    {
      arg->nargs = list_length (arg_types) - 1;
      arg->arguments = GGC_NEWVEC (tree, arg->nargs);

      for (i = 0; i < arg->nargs; i++)
	{
	  arg->arguments[i] = TREE_VALUE (arg_types);
	  arg_types = TREE_CHAIN (arg_types);
	}
    }

  stmt->opcode = opcode;
  stmt->arg.fcall = arg;
  return stmt;
}

/* Build a generic CIL call statement taking extra arguments not specified in
   the signature. OPCODE specifies the type of jump. VA requires the call to
   be flagged as taking a variable number of arguments. TYPE_OR_DECL holds the
   function declaration or the function type depending if the call is direct or
   not. The ARGLIST must be a TREE_VEC holding the arguments passed after the
   ellipsis for a vararg call and all the arguments for a call lacking the
   prototype. This function is used both for creating vararg calls and calls to
   regular functions whose prototype is missing. */

cil_stmt
cil_build_call_generic_list (enum cil_opcode opcode, bool va,
			     tree type_or_decl, VEC (tree, heap) *arglist)
{
  cil_stmt stmt = GGC_CNEW (struct cil_stmt_d);
  cil_call_arg arg = GGC_CNEW (struct cil_call_arg_d);
  tree arg_types;
  size_t nargs_base;
  size_t i;

  gcc_assert (opcode == CIL_CALL || opcode == CIL_CALLI || opcode == CIL_JMP);

  if (opcode != CIL_CALLI)
    {
      arg->ftype = TREE_TYPE (type_or_decl);
      arg->fdecl = type_or_decl;
    }
  else
    arg->ftype = type_or_decl;

  arg_types = TYPE_ARG_TYPES (arg->ftype);

  nargs_base = list_length (arg_types);

  arg->nargs = nargs_base + VEC_length (tree, arglist);

  if (arg->nargs == 0)
    arg->arguments = NULL;
  else
    arg->arguments = GGC_NEWVEC (tree, arg->nargs);

  for (i = 0; i < nargs_base; i++)
    {
      arg->arguments[i] = TREE_VALUE (arg_types);
      arg_types = TREE_CHAIN (arg_types);
    }

  for ( ; i < arg->nargs; i++)
    arg->arguments[i] = VEC_index (tree, arglist, i - nargs_base);

  if (va)
    arg->vararg_p = true;
  else
    arg->missing_proto_p = true;

  stmt->opcode = opcode;
  stmt->arg.fcall = arg;
  return stmt;
}

/* Return the number of arguments accepted by the callee of a CIL CALL, CALLI,
   JMP or NEWOBJ statement. This does not include the arguments after the
   ellipsis in a variable argument call.  */

size_t
cil_call_nargs_base (const_cil_stmt stmt)
{
  tree arg_types;

  gcc_assert (opcode_arg_type (stmt->opcode) == CIL_FCALL);

  arg_types = TYPE_ARG_TYPES (stmt->arg.fcall->ftype);

  if (arg_types == NULL)
    return 0;
  else
    {
      if (cil_call_vararg_p (stmt))
	return list_length (arg_types);
      else
	return list_length (arg_types) - 1;
    }
}

/* Return the low value of the I-th case of CIL switch statement as an
   HOST_WIDE_INT instead of a tree.  */

HOST_WIDE_INT
cil_switch_case_low (const_cil_stmt stmt, size_t i)
{
  tree value;

  gcc_assert (stmt->opcode == CIL_SWITCH);
  gcc_assert (i < stmt->arg.labels->ncases);

  value = CASE_LOW (stmt->arg.labels->cases[i]);
  return tree_low_cst (value, TYPE_UNSIGNED (TREE_TYPE (value)));
}

/* Return the high value of the I-th case of CIL switch statement as an
   HOST_WIDE_INT instead of a tree. If the case doesn't represent a range then
   this function will return the case value (i.e. the low value). */

HOST_WIDE_INT
cil_switch_case_high (const_cil_stmt stmt, size_t i)
{
  tree value;

  gcc_assert (stmt->opcode == CIL_SWITCH);
  gcc_assert (i < stmt->arg.labels->ncases);

  if (CASE_HIGH (stmt->arg.labels->cases[i]))
    value = CASE_HIGH (stmt->arg.labels->cases[i]);
  else
    value = CASE_LOW (stmt->arg.labels->cases[i]);

  return tree_low_cst (value, TYPE_UNSIGNED (TREE_TYPE (value)));
}

/* Create a deep copy of the CIL statement pointed by STMT and return it.  */

cil_stmt
cil_copy_stmt (const_cil_stmt stmt)
{
  cil_stmt new_stmt;

  new_stmt = GGC_NEW (struct cil_stmt_d);
  memcpy (new_stmt, stmt, sizeof (struct cil_stmt_d));

  if (opcode_arg_type (stmt->opcode) == CIL_LABELS)
    {
      cil_switch_arg arg = GGC_NEW (struct cil_switch_arg_d);
      unsigned int ncases = stmt->arg.labels->ncases;

      arg->ncases = ncases;
      arg->cases = GGC_NEWVEC (tree, ncases);
      memcpy (arg->cases, stmt->arg.labels->cases, sizeof (tree) * ncases);
      new_stmt->arg.labels = arg;
    }
  else if (opcode_arg_type (stmt->opcode) == CIL_FCALL)
    {
      cil_call_arg arg = GGC_NEW (struct cil_call_arg_d);
      unsigned int nargs = stmt->arg.fcall->nargs;

      memcpy (arg, &stmt->arg.fcall, sizeof (struct cil_call_arg_d));

      if (nargs != 0)
	{
	  arg->arguments = GGC_NEWVEC (tree, nargs);
	  memcpy (arg->arguments, stmt->arg.fcall->arguments,
		  sizeof (tree) * nargs);
	}
    }

  return new_stmt;
}

/* Returns the tail. prefix to the CIL statement pointed by STMT.  */

bool
cil_prefix_tail (const_cil_stmt stmt)
{
  return (((stmt->opcode == CIL_CALL) || (stmt->opcode == CIL_CALLI))
	  && stmt->prefix_tail);
}

/* Sets the tail. prefix to STATUS in the CIL statment pointed by STMT.  */

void
cil_set_prefix_tail (cil_stmt stmt, bool status)
{
  gcc_assert ((stmt->opcode == CIL_CALL) || (stmt->opcode == CIL_CALLI));

  stmt->prefix_tail = status ? 1 : 0;
}

/* Returns the value of the unaligned. prefix of the CIL statement pointed by
   STMT. If the prefix is not specified the returned value will be 0,
   otherwise the alignment (either 1, 2 or 4) will be returned.  */

int
cil_prefix_unaligned (const_cil_stmt stmt)
{
  int result = 0;

  if (((stmt->opcode == CIL_CPBLK) || (stmt->opcode == CIL_INITBLK)
       || (stmt->opcode == CIL_LDOBJ) || (stmt->opcode == CIL_STOBJ)
       || (stmt->opcode == CIL_LDVEC) || (stmt->opcode == CIL_STVEC)
       || ((stmt->opcode >= CIL_LDIND_I1) && (stmt->opcode <= CIL_LDIND_I))
       || ((stmt->opcode >= CIL_STIND_I1) && (stmt->opcode <= CIL_STIND_I))
       || (stmt->opcode == CIL_LDFLD) || (stmt->opcode == CIL_STFLD))
      && stmt->prefix_unaligned)
    {
      result = stmt->alignment;
    }

  return result;
}

/* Sets the unaligned. prefix in the CIL statement pointed by STMT. The actual
   alignment is taken from ALIGNMENT and must be either 1, 2 or 4.  */

void
cil_set_prefix_unaligned (cil_stmt stmt, int alignment)
{
  gcc_assert ((stmt->opcode == CIL_CPBLK) || (stmt->opcode == CIL_INITBLK)
	      || (stmt->opcode == CIL_LDOBJ) || (stmt->opcode == CIL_STOBJ)
	      || (stmt->opcode == CIL_LDVEC) || (stmt->opcode == CIL_STVEC)
	      || ((stmt->opcode >= CIL_LDIND_I1) && (stmt->opcode <= CIL_LDIND_I))
	      || ((stmt->opcode >= CIL_STIND_I1) && (stmt->opcode <= CIL_STIND_I))
	      || (stmt->opcode == CIL_LDFLD) || (stmt->opcode == CIL_STFLD));
  gcc_assert ((alignment == 1) || (alignment == 2) || (alignment == 4));

  stmt->prefix_unaligned = alignment ? 1 : 0;
  stmt->alignment = alignment;
}

/* Sets the volatile. prefix to STATUS in the CIL statement pointed by STMT.  */

void
cil_set_prefix_volatile (cil_stmt stmt, bool status)
{
  gcc_assert ((stmt->opcode == CIL_CPBLK) || (stmt->opcode == CIL_INITBLK)
	      || (stmt->opcode == CIL_LDOBJ) || (stmt->opcode == CIL_STOBJ)
	      || (stmt->opcode == CIL_LDVEC) || (stmt->opcode == CIL_STVEC)
	      || ((stmt->opcode >= CIL_LDIND_I1) && (stmt->opcode <= CIL_LDIND_I))
	      || ((stmt->opcode >= CIL_STIND_I1) && (stmt->opcode <= CIL_STIND_I))
	      || (stmt->opcode == CIL_LDFLD) || (stmt->opcode == CIL_STFLD)
	      || (stmt->opcode == CIL_LDSFLD) || (stmt->opcode == CIL_STSFLD));

  stmt->prefix_volatile = status ? 1 : 0;
}

/* Returns the status of the volatile. prefix in the CIL statement pointed by
   STMT.  */

bool
cil_prefix_volatile (const_cil_stmt stmt)
{
  return (((stmt->opcode == CIL_CPBLK) || (stmt->opcode == CIL_INITBLK)
	   || (stmt->opcode == CIL_LDOBJ) || (stmt->opcode == CIL_STOBJ)
	   || (stmt->opcode == CIL_LDVEC) || (stmt->opcode == CIL_STVEC)
	   || ((stmt->opcode >= CIL_LDIND_I1) && (stmt->opcode <= CIL_LDIND_I))
	   || ((stmt->opcode >= CIL_STIND_I1) && (stmt->opcode <= CIL_STIND_I))
	   || (stmt->opcode == CIL_LDFLD) || (stmt->opcode == CIL_STFLD)
	   || (stmt->opcode == CIL_LDSFLD) || (stmt->opcode == CIL_STSFLD))
	  && stmt->prefix_volatile);
}

/* Sets the short ctor flag to STATUS in the CIL statement pointed by STMT.  */

void
cil_set_short_ctor (cil_stmt stmt, bool status)
{
  gcc_assert (stmt->opcode == CIL_VEC_CTOR);
  stmt->short_ctor = status ? 1 : 0;
}

/* Returns the status of the short ctor flag in the CIL statement pointed by
   STMT.  */

bool
cil_short_ctor (const_cil_stmt stmt)
{
  gcc_assert (stmt->opcode == CIL_VEC_CTOR);
  return stmt->short_ctor;
}

/* Returns TRUE if the CIL stsatement STMT represents a conversion, FALSE
   otherwise.  */

bool
cil_conversion_p (const_cil_stmt stmt)
{
  switch (cil_opcode (stmt))
    {
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
      return true;

    default:
      return false;
    }
}

/* Returns TRUE if the CIL statement STMT is a conditional branch instruction,
   FALSE otherwise.  */

bool
cil_cond_branch_p (const_cil_stmt stmt)
{
  switch (cil_opcode (stmt))
    {
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
      return true;

    default:
      return false;
    }
}

/* Returns TRUE if the CIL statement STMT is an indirect load, FALSE
   otherwise.  */

bool
cil_ldind_p (const_cil_stmt stmt)
{
  switch (cil_opcode (stmt))
    {
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
      return true;

    default:
      return false;
    }
}

/* Returns TRUE if the CIL statement STMT is an indirect store, FALSE
   otherwise.  */

bool
cil_stind_p (const_cil_stmt stmt)
{
  switch (cil_opcode (stmt))
    {
    case CIL_STIND_I1:
    case CIL_STIND_I2:
    case CIL_STIND_I4:
    case CIL_STIND_I8:
    case CIL_STIND_R4:
    case CIL_STIND_R8:
    case CIL_STIND_I:
      return true;

    default:
      return false;
    }
}

/* Returns TRUE if the CIL stsatement STMT pushes a single slot on the stack
   without side effects, false otherwise.  */

bool
cil_push_p (const_cil_stmt stmt)
{
  switch (cil_opcode (stmt))
    {
    case CIL_DUP:
    case CIL_LDARG:
    case CIL_LDARGA:
    case CIL_LDC_I4:
    case CIL_LDC_I8:
    case CIL_LDC_R4:
    case CIL_LDC_R8:
    case CIL_LDFTN:
    case CIL_LDLOC:
    case CIL_LDLOCA:
    case CIL_LDSFLD:
    case CIL_LDSFLDA:
      return true;

    default:
      return false;
    }
}

/******************************************************************************
 * CIL sequence functions                                                     *
 ******************************************************************************/

/* Allocate a new CIL sequence in GC memory and return it.  If there are free
   sequences in GIMPLE_SEQ_CACHE return one of those instead.  */

cil_seq
cil_seq_alloc (void)
{
  cil_seq seq = cil_seq_cache;

  if (seq)
    {
      cil_seq_cache = cil_seq_cache->next_free;
      gcc_assert (cil_seq_cache != seq);
      memset (seq, 0, sizeof (*seq));
    }
  else
    seq = GGC_CNEW (struct cil_seq_d);

  return seq;
}

/* Return SEQ to the free pool of CIL sequences.  */

void
cil_seq_free (cil_seq seq)
{
  if (seq == NULL)
    return;

  gcc_assert (cil_seq_first (seq) == NULL);
  gcc_assert (cil_seq_last (seq) == NULL);

  /* If this triggers, it's a sign that the same list is being freed
     twice.  */
  gcc_assert (seq != cil_seq_cache || cil_seq_cache == NULL);

  /* Add SEQ to the pool of free sequences.  */
  seq->next_free = cil_seq_cache;
  cil_seq_cache = seq;
}

/* Link CIL statement CS to the end of the sequence *SEQ_P.  If
   *SEQ_P is NULL, a new sequence is allocated.  */

void
cil_seq_add_stmt (cil_seq *seq_p, cil_stmt cs)
{
  cil_stmt_iterator si;

  if (cs == NULL)
    return;

  if (*seq_p == NULL)
    *seq_p = cil_seq_alloc ();

  si = csi_last (*seq_p);
  csi_insert_after (&si, cs, CSI_NEW_STMT);
}

/* Append sequence SRC to the end of sequence *DST_P.  If *DST_P is
   NULL, a new sequence is allocated.  */

void
cil_seq_add_seq (cil_seq *dst_p, cil_seq src)
{
  cil_stmt_iterator si;

  if (src == NULL)
    return;

  if (*dst_p == NULL)
    *dst_p = cil_seq_alloc ();

  si = csi_last (*dst_p);
  csi_insert_seq_after (&si, src, CSI_NEW_STMT);
}

/* Perform a deep copy of sequence SRC and return the result.  */

cil_seq
cil_seq_deep_copy (cil_seq src)
{
  cil_stmt_iterator csi;
  cil_seq new_seq = cil_seq_alloc ();
  cil_stmt stmt;

  for (csi = csi_start (src); !csi_end_p (csi); csi_next (&csi))
    {
      stmt = cil_copy_stmt (csi_stmt (csi));
      cil_seq_add_stmt (&new_seq, stmt);
    }

  return new_seq;
}

/* Returns the sequence of statements in BB.  */

cil_seq
cil_bb_seq (const basic_block bb)
{
  struct machine_function *machine = cfun->machine;
  struct cil_basic_block_d tmp;
  void **slot;

  tmp.bb = bb;
  slot = htab_find_slot (machine->bb_seqs, &tmp, INSERT);

  gcc_assert (*slot != NULL);
  return ((cil_basic_block) *slot)->seq;
}

/* Sets the sequence of statements in BB to SEQ.  */

void
cil_set_bb_seq (basic_block bb, cil_seq seq)
{
  struct machine_function *machine = cfun->machine;
  struct cil_basic_block_d tmp;
  void **slot;

  tmp.bb = bb;
  slot = htab_find_slot (machine->bb_seqs, &tmp, INSERT);

  if (*slot == NULL)
    {
      cil_basic_block cbb = GGC_NEW (struct cil_basic_block_d);

      cbb->bb = bb;
      cbb->seq = seq;
      *slot = cbb;
    }
  else
    ((cil_basic_block) *slot)->seq = seq;
}

/* Computes the stack depth at the end of the sequence pointed by SEQ.
   INIT specifies the initial stack depth of the sequence.
   If MAX is true then the maximum stack depth is returned, otherwise the depth
   at the end of the sequence is returned.  */

unsigned int
cil_seq_stack_depth (cil_seq seq, unsigned int init, bool max)
{
  cil_stmt_iterator i;
  unsigned int max_depth = init;
  unsigned int depth = init;
  unsigned int nargs;

  if (cil_seq_empty_p (seq))
    return init;

  for (i = csi_start (seq); !csi_end_p (i); csi_next (&i))
    {
      cil_stmt cs = csi_stmt (i);

      switch (cil_opcode (cs))
	{
	case CIL_CPBLK:
	case CIL_INITBLK:
	  gcc_assert (depth >= 3);
	  depth -= 3;
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
	case CIL_STFLD:
	case CIL_STIND_I1:
	case CIL_STIND_I2:
	case CIL_STIND_I4:
	case CIL_STIND_I8:
	case CIL_STIND_R4:
	case CIL_STIND_R8:
	case CIL_STIND_I:
	case CIL_STOBJ:
	  gcc_assert (depth >= 2);
	  depth -= 2;
	  break;

	case CIL_ADD:
	case CIL_AND:
	case CIL_CEQ:
	case CIL_CGT:
	case CIL_CGT_UN:
	case CIL_CLT:
	case CIL_CLT_UN:
	case CIL_DIV:
	case CIL_DIV_UN:
	case CIL_MUL:
	case CIL_OR:
	case CIL_REM:
	case CIL_REM_UN:
	case CIL_SHL:
	case CIL_SHR:
	case CIL_SHR_UN:
	case CIL_SUB:
	case CIL_XOR:
	  gcc_assert (depth >= 2);
	  depth--;
	  break;

	case CIL_BRFALSE:
	case CIL_BRTRUE:
	case CIL_INITOBJ:
	case CIL_POP:
	case CIL_STARG:
	case CIL_STLOC:
	case CIL_STSFLD:
	case CIL_SWITCH:
	  gcc_assert (depth >= 1);
	  depth--;
	  break;

	case CIL_CKFINITE:
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
	case CIL_LDFLD:
	case CIL_LDFLDA:
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
	case CIL_LDOBJ:
	case CIL_LOCALLOC:
	case CIL_NEG:
	case CIL_NOT:
	  gcc_assert (depth >= 1);
	  break;

	case CIL_BR:
	case CIL_BREAK:
	  break;

	case CIL_DUP:
	  gcc_assert (depth >= 1);
	  depth++;
	  max_depth = (depth > max_depth) ? depth : max_depth;
	  break;

	case CIL_ARGLIST:
	case CIL_LDARG:
	case CIL_LDARGA:
	case CIL_LDC_I4:
	case CIL_LDC_I8:
	case CIL_LDC_R4:
	case CIL_LDC_R8:
	case CIL_LDFTN:
	case CIL_LDLOC:
	case CIL_LDLOCA:
	case CIL_LDSFLD:
	case CIL_LDSFLDA:
	  depth++;
	  max_depth = (depth > max_depth) ? depth : max_depth;
	  break;

	case CIL_RET:
	    gcc_assert (depth == 0);
	  break;

	case CIL_RET_VAL:
          gcc_assert (depth == 1);
          depth--;
	  break;

	case CIL_CALL:
	case CIL_JMP:
	case CIL_NEWOBJ:
	case CIL_CALLI:
	  nargs = cil_call_nargs (cs);
	  gcc_assert (depth >= nargs);
	  depth -= nargs;

	  if (!VOID_TYPE_P (TREE_TYPE (cil_call_ftype (cs))))
	    {
	      depth++;
	      max_depth = (depth > max_depth) ? depth : max_depth;
	    }

	  break;

	case CIL_ASM:
	  /* TODO: Specify a way for asm statements to tell the compiler how
	     many stack slots they need.  */
	  break;

	default:
	  gcc_unreachable ();
	}
    }

  return max ? max_depth : depth;
}

/******************************************************************************
 * CIL statement iterator functions                                           *
 ******************************************************************************/

/* Move all statements in the sequence after I to a new sequence.
   Return this new sequence.  */

cil_seq
csi_split_seq_after (cil_stmt_iterator i)
{
  cil_seq_node cur, next;
  cil_seq old_seq, new_seq;

  cur = i.ptr;

  /* How can we possibly split after the end, or before the beginning?  */
  gcc_assert (cur && cur->next);
  next = cur->next;

  old_seq = i.seq;
  new_seq = cil_seq_alloc ();

  cil_seq_set_first (new_seq, next);
  cil_seq_set_last (new_seq, cil_seq_last (old_seq));
  cil_seq_set_last (old_seq, cur);
  cur->next = NULL;
  next->prev = NULL;

  return new_seq;
}

/* Move all statements in the sequence before I to a new sequence.
   Return this new sequence.  I is set to the head of the new list.  */

cil_seq
csi_split_seq_before (cil_stmt_iterator *i)
{
  cil_seq_node cur, prev;
  cil_seq old_seq, new_seq;

  cur = i->ptr;

  /* How can we possibly split after the end?  */
  gcc_assert (cur);
  prev = cur->prev;

  old_seq = i->seq;
  new_seq = cil_seq_alloc ();
  i->seq = new_seq;

  /* Set the limits on NEW_SEQ.  */
  cil_seq_set_first (new_seq, cur);
  cil_seq_set_last (new_seq, cil_seq_last (old_seq));

  /* Cut OLD_SEQ before I.  */
  cil_seq_set_last (old_seq, prev);
  cur->prev = NULL;

  if (prev)
    prev->next = NULL;
  else
    cil_seq_set_first (old_seq, NULL);

  return new_seq;
}

/* Replace the statement pointed-to by CSI to STMT.  */

void
csi_replace (cil_stmt_iterator *csi, cil_stmt stmt)
{
  cil_stmt orig_stmt = csi_stmt (*csi);

  if (stmt == orig_stmt)
    return;

  cil_set_locus (stmt, cil_locus (orig_stmt));
  cil_set_bb (stmt, csi_bb (*csi));

  *csi_stmt_ptr (csi) = stmt;
}

/* Insert statement STMT before the statement pointed-to by iterator
   I, update STMT's basic block.  M specifies how to update iterator
   I after insertion (see enum csi_iterator_update).  */

void
csi_insert_before (cil_stmt_iterator *i, cil_stmt stmt,
		   enum csi_iterator_update m)
{
  cil_seq_node n;

  n = GGC_NEW (struct cil_seq_node_d);
  n->prev = n->next = NULL;
  n->stmt = stmt;
  csi_insert_seq_nodes_before (i, n, n, m);
}

/* Inserts the sequence of statements SEQ before the statement pointed
   by iterator I.  M indicates what to do with the iterator after
   insertion (see enum csi_iterator_update).  */

void
csi_insert_seq_before (cil_stmt_iterator *i, cil_seq seq,
		       enum csi_iterator_update m)
{
  cil_seq_node first, last;

  /* Don't allow inserting a sequence into itself.  */
  gcc_assert (seq != i->seq);

  first = cil_seq_first (seq);
  last = cil_seq_last (seq);

  cil_seq_set_first (seq, NULL);
  cil_seq_set_last (seq, NULL);
  cil_seq_free (seq);

  /* Empty sequences need no work.  */
  if (!first || !last)
    {
      gcc_assert (first == last);
      return;
    }

  csi_insert_seq_nodes_before (i, first, last, m);
}

/* Insert statement STMT after the statement pointed-to by iterator I,
   update STMT's basic block.  M specifies how to update iterator I
   after insertion (see enum csi_iterator_update).  */

void
csi_insert_after (cil_stmt_iterator *i, cil_stmt stmt,
		  enum csi_iterator_update m)
{
  cil_seq_node n;

  n = GGC_NEW (struct cil_seq_node_d);
  n->prev = n->next = NULL;
  n->stmt = stmt;
  csi_insert_seq_nodes_after (i, n, n, m);
}

/* Links sequence SEQ after the statement pointed-to by iterator I.
   M is as in csi_insert_after.  */

void
csi_insert_seq_after (cil_stmt_iterator *i, cil_seq seq,
		      enum csi_iterator_update m)
{
  cil_seq_node first, last;

  /* Don't allow inserting a sequence into itself.  */
  gcc_assert (seq != i->seq);

  first = cil_seq_first (seq);
  last = cil_seq_last (seq);

  cil_seq_set_first (seq, NULL);
  cil_seq_set_last (seq, NULL);
  cil_seq_free (seq);

  /* Empty sequences need no work.  */
  if (!first || !last)
    {
      gcc_assert (first == last);
      return;
    }

  csi_insert_seq_nodes_after (i, first, last, m);
}

/* Remove the current stmt from the sequence.  The iterator is updated
   to point to the next statement.  */

void
csi_remove (cil_stmt_iterator *i)
{
  cil_seq_node cur, next, prev;
  cil_stmt stmt = csi_stmt (*i);

  /* Free all the data flow information for STMT.  */
  cil_set_bb (stmt, NULL);

  /* Update the iterator and re-wire the links in I->SEQ.  */
  cur = i->ptr;
  next = cur->next;
  prev = cur->prev;

  if (prev)
    prev->next = next;
  else
    cil_seq_set_first (i->seq, next);

  if (next)
    next->prev = prev;
  else
    cil_seq_set_last (i->seq, prev);

  i->ptr = next;
}

/* Finds iterator for STMT.  */

cil_stmt_iterator
csi_for_stmt (cil_stmt stmt)
{
  cil_stmt_iterator i;

  for (i = csi_start_bb (cil_bb (stmt)); !csi_end_p (i); csi_next (&i))
    if (csi_stmt (i) == stmt)
      return i;

  gcc_unreachable ();
}

/* Move the statement at FROM so it comes right after the statement at TO.  */

void
csi_move_after (cil_stmt_iterator *from, cil_stmt_iterator *to)
{
  cil_stmt stmt = csi_stmt (*from);
  csi_remove (from);

  /* We must have CSI_NEW_STMT here, as csi_move_after is sometimes used to
     move statements to an empty block.  */
  csi_insert_after (to, stmt, CSI_NEW_STMT);
}

/* Move the statement at FROM so it comes right before the statement
   at TO.  */

void
csi_move_before (cil_stmt_iterator *from, cil_stmt_iterator *to)
{
  cil_stmt stmt = csi_stmt (*from);
  csi_remove (from);

  /* For consistency with csi_move_after, it might be better to have
     CSI_NEW_STMT here; however, that breaks several places that expect
     that TO does not change.  */
  csi_insert_before (to, stmt, CSI_SAME_STMT);
}

/* Move the statement at FROM to the end of basic block BB.  */

void
csi_move_to_bb_end (cil_stmt_iterator *from, basic_block bb)
{
  cil_stmt_iterator last = csi_last (cil_bb_seq (bb));

  /* Have to check csi_end_p because it could be an empty block.  */
  if (!csi_end_p (last))
    csi_move_before (from, &last);
  else
    csi_move_after (from, &last);
}

/******************************************************************************
 * Helper functions                                                           *
 ******************************************************************************/

/* Set BB to be the basic block for all the statements in the list
   starting at FIRST and LAST.  */

static void
update_bb_for_cil_stmts (cil_seq_node first, basic_block bb)
{
  cil_seq_node n;

  for (n = first; n != NULL; n = n->next)
    cil_set_bb (n->stmt, bb);
}

/* Insert the sequence delimited by nodes FIRST and LAST before
   iterator I.  M specifies how to update iterator I after insertion
   (see enum csi_iterator_update).

   This routine assumes that there is a forward and backward path
   between FIRST and LAST (i.e., they are linked in a doubly-linked
   list).  Additionally, if FIRST == LAST, this routine will properly
   insert a single node.  */

static void
csi_insert_seq_nodes_before (cil_stmt_iterator *i, cil_seq_node first,
			     cil_seq_node last, enum csi_iterator_update m)
{
  basic_block bb;
  cil_seq_node cur = i->ptr;

  if ((bb = csi_bb (*i)) != NULL)
    update_bb_for_cil_stmts (first, bb);

  /* Link SEQ before CUR in the sequence.  */
  if (cur)
    {
      first->prev = cur->prev;

      if (first->prev)
	first->prev->next = first;
      else
	cil_seq_set_first (i->seq, first);

      last->next = cur;
      cur->prev = last;
    }
  else
    {
      gcc_assert (!cil_seq_first (i->seq));
      cil_seq_set_first (i->seq, first);
      cil_seq_set_last (i->seq, last);
    }

  /* Update the iterator, if requested.  */
  switch (m)
    {
    case CSI_NEW_STMT:
    case CSI_CONTINUE_LINKING:
      i->ptr = first;
      break;
    case CSI_SAME_STMT:
      break;
    default:
      gcc_unreachable ();
    }
}

/* Insert the sequence delimited by nodes FIRST and LAST after
   iterator I.  M specifies how to update iterator I after insertion
   (see enum csi_iterator_update).

   This routine assumes that there is a forward and backward path
   between FIRST and LAST (i.e., they are linked in a doubly-linked
   list).  Additionally, if FIRST == LAST, this routine will properly
   insert a single node.  */

static void
csi_insert_seq_nodes_after (cil_stmt_iterator *i, cil_seq_node first,
			    cil_seq_node last, enum csi_iterator_update m)
{
  basic_block bb;
  cil_seq_node cur = i->ptr;

  /* If the iterator is inside a basic block, we need to update the
     basic block information for all the nodes between FIRST and LAST.  */
  if ((bb = csi_bb (*i)) != NULL)
    update_bb_for_cil_stmts (first, bb);

  /* Link SEQ after CUR.  */
  if (cur)
    {
      last->next = cur->next;

      if (last->next)
	last->next->prev = last;
      else
	cil_seq_set_last (i->seq, last);

      first->prev = cur;
      cur->next = first;
    }
  else
    {
      gcc_assert (!cil_seq_last (i->seq));
      cil_seq_set_first (i->seq, first);
      cil_seq_set_last (i->seq, last);
    }

  /* Update the iterator, if requested.  */
  switch (m)
    {
    case CSI_NEW_STMT:
      i->ptr = first;
      break;
    case CSI_CONTINUE_LINKING:
      i->ptr = last;
      break;
    case CSI_SAME_STMT:
      gcc_assert (cur);
      break;
    default:
      gcc_unreachable ();
    }
}

#include "gt-cil-stmt.h"

/*
 * Local variables:
 * eval: (c-set-style "gnu")
 * End:
 */

/* Inlined functions for manipulating CIL statements.

   Copyright (C) 2006-2008 Free Software Foundation, Inc.

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
Erven Rohou             <erven.rohou@st.com>
*/

#ifndef CIL_STMT_INLINE_H
#define CIL_STMT_INLINE_H

/******************************************************************************
 * CIL statement inlined functions                                            *
 ******************************************************************************/

/* Return the code for CIL statement STMT. */

static inline enum cil_opcode
cil_opcode (const_cil_stmt stmt)
{
  return stmt->opcode;
}

/* Return the basic block holding the statement STMT.  */

static inline basic_block
cil_bb (const_cil_stmt stmt)
{
  return stmt->bb;
}

/* Set the basic block holding the statement STMT to BB. */

static inline void
cil_set_bb (cil_stmt stmt, basic_block bb)
{
  stmt->bb = bb;
}

/* Return locus information for statement STMT. */

static inline source_locus
cil_locus (const_cil_stmt stmt)
{
  return stmt->locus;
}

/* Set locus information for statement STMT. */

static inline void
cil_set_locus (cil_stmt stmt, source_locus locus)
{
  stmt->locus = locus;
}

/* Return the variable declaration (VAR_DECL) held by a CIL statement.  */

static inline tree
cil_var (const_cil_stmt stmt)
{
  gcc_assert (opcode_arg_type (stmt->opcode) == CIL_VAR);

  return stmt->arg.var;
}

/* Return the type definition (TYPE_DECL) held by a CIL statement.  */

static inline tree
cil_type (const_cil_stmt stmt)
{
  gcc_assert (opcode_arg_type (stmt->opcode) == CIL_TYPE);

  return stmt->arg.type;
}

/* Return the field declaration (FIELD_DECL) held by a CIL statement.  */

static inline tree
cil_field (const_cil_stmt stmt)
{
  gcc_assert (opcode_arg_type (stmt->opcode) == CIL_FIELD);

  return stmt->arg.field;
}

/* Return the label declaration (LABEL_DECL) held by a CIL statement.  */

static inline tree
cil_label (const_cil_stmt stmt)
{
  gcc_assert (opcode_arg_type (stmt->opcode) == CIL_LABEL);

  return stmt->arg.label;
}

/* Return the number of cases of a CIL switch statment.  */

static inline size_t
cil_switch_ncases (const_cil_stmt stmt)
{
  gcc_assert (stmt->opcode == CIL_SWITCH);

  return stmt->arg.labels->ncases;
}

/* Return the I-th case of a CIL switch statement.  The last entry is the
   default case.  */

static inline tree
cil_switch_case (const_cil_stmt stmt, size_t i)
{
  gcc_assert (stmt->opcode == CIL_SWITCH);
  gcc_assert (i < stmt->arg.labels->ncases);

  return stmt->arg.labels->cases[i];
}

/* Return the default case of a CIL switch statement.  */

static inline tree
cil_switch_default (const_cil_stmt stmt)
{
  gcc_assert (stmt->opcode == CIL_SWITCH);

  return stmt->arg.labels->cases[stmt->arg.labels->ncases - 1];
}

/* Return the I-th case label of CIL switch statement.  */

static inline tree
cil_switch_case_label (const_cil_stmt stmt, size_t i)
{
  gcc_assert (stmt->opcode == CIL_SWITCH);
  gcc_assert (i < stmt->arg.labels->ncases);

  return CASE_LABEL (stmt->arg.labels->cases[i]);
}

/* Return the function declaration of a CIL LDFTN statement.  */

static inline
tree cil_func (const_cil_stmt stmt)
{
  gcc_assert (stmt->opcode == CIL_LDFTN);

  return stmt->arg.func;
}

/* Builds a CIL CALL statement. The FDECL field holds the callee
   declaration.  */

static inline cil_stmt
cil_build_call (tree fdecl)
{
  return cil_build_call_generic (CIL_CALL, fdecl);
}

/* Builds a CIL CALLI statement. The FTYPE field holds the callee type.  */

static inline cil_stmt
cil_build_calli (tree ftype)
{
  return cil_build_call_generic (CIL_CALLI, ftype);
}

/* Builds a CIL JMP statement. The FDECL field holds the callee declaration.  */

static inline cil_stmt
cil_build_jmp (tree fdecl)
{
  return cil_build_call_generic (CIL_JMP, fdecl);
}

/* Builds a CIL CALL statement. The FDECL field holds the callee declaration.
   The ARGLIST must be a pointer to a VEC (tree, heap) holding the types of the
   arguments passed to the callee after the ellipsis.  */

static inline cil_stmt
cil_build_call_va (tree fdecl, VEC (tree, heap) *arglist)
{
  return cil_build_call_generic_list (CIL_CALL, true, fdecl, arglist);
}

/* Builds a CIL CALLI statement. The FTYPE field holds the callee type. The
   ARGLIST must be a pointer to a VEC (tree, heap) holding the types of the
   arguments passed to the callee after the ellipsis.  */

static inline cil_stmt
cil_build_calli_va (tree ftype, VEC (tree, heap) *arglist)
{
  return cil_build_call_generic_list (CIL_CALLI, true, ftype, arglist);
}

/* Builds a CIL JMP statement. The FDECL field holds the callee declaration.
   The ARGLIST must be a pointer to a VEC (tree, heap) holding the types of the
   arguments passed to the callee after the ellipsis.  */

static inline cil_stmt
cil_build_jmp_va (tree fdecl, VEC (tree, heap) *arglist)
{
  return cil_build_call_generic_list (CIL_JMP, true, fdecl, arglist);
}

/* Builds a CIL CALL statement for a function without a prototype. The FDECL
   field holds the callee declaration. The ARGLIST must be a pointer to a
   VEC (tree, heap) holding the types of all the arguments passed to the
   callee.  */

static inline cil_stmt
cil_build_call_mp (tree fdecl, VEC (tree, heap) *arglist)
{
  return cil_build_call_generic_list (CIL_CALL, false, fdecl, arglist);
}

/* Builds a CIL CALL statement for a function without a prototype. The FTYPE
   field holds the callee type. The ARGLIST must be a pointer to a
   VEC (tree, heap) holding the types of all the arguments passed to the
   callee.  */

static inline cil_stmt
cil_build_calli_mp (tree ftype, VEC (tree, heap) *arglist)
{
  return cil_build_call_generic_list (CIL_CALLI, false, ftype, arglist);
}

/* Builds a CIL CALL statement for a function without a prototype. The FDECL
   field holds the callee declaration. The ARGLIST must be a pointer to a
   VEC (tree, heap) holding the types of all the arguments passed to the
   callee.  */

static inline cil_stmt
cil_build_jmp_mp (tree fdecl, VEC (tree, heap) *arglist)
{
  return cil_build_call_generic_list (CIL_JMP, false, fdecl, arglist);
}

/* Return the type of the callee of a CIL CALL, CALLI or JMP statement.  */

static inline tree
cil_call_ftype (const_cil_stmt stmt)
{
  gcc_assert (stmt->opcode == CIL_CALL
	      || stmt->opcode == CIL_CALLI
	      || stmt->opcode == CIL_JMP);

  return stmt->arg.fcall->ftype;
}

/* Return the function declaration of the callee of a CIL CALL or JMP
   statement.  */

static inline tree
cil_call_fdecl (const_cil_stmt stmt)
{
  gcc_assert (stmt->opcode == CIL_CALL || stmt->opcode == CIL_JMP);

  return stmt->arg.fcall->fdecl;
}

/* Return the number of arguments passed to the callee of a CIL CALL, CALLI or
   JMP statement.  */

static inline size_t
cil_call_nargs (const_cil_stmt stmt)
{
  gcc_assert (stmt->opcode == CIL_CALL || stmt->opcode == CIL_CALLI);

  return stmt->arg.fcall->nargs;
}

/* Return the I-th argument passed to the callee of a CIL CALL, CALLI or JMP
   statement.  */

static inline tree
cil_call_arg_type (const_cil_stmt stmt, size_t i)
{
  gcc_assert (stmt->opcode == CIL_CALL
	      || stmt->opcode == CIL_CALLI
	      || stmt->opcode == CIL_JMP);
  gcc_assert (i < stmt->arg.fcall->nargs);

  return stmt->arg.fcall->arguments[i];
}

/* Set the type SC_TYPE of the static chain for the call STMT.  */

static inline void
cil_call_set_static_chain (cil_stmt stmt, tree sc_type)
{
  gcc_assert (stmt->opcode == CIL_CALL
	      || stmt->opcode == CIL_CALLI
	      || stmt->opcode == CIL_JMP);

  stmt->arg.fcall->static_chain_p = true;
  stmt->arg.fcall->static_chain = sc_type;
}

/* Return the type of the static chain for the call STMT if present.  */

static inline tree
cil_call_static_chain (const_cil_stmt stmt)
{
  gcc_assert (stmt->opcode == CIL_CALL
	      || stmt->opcode == CIL_CALLI
	      || stmt->opcode == CIL_JMP);

  if (stmt->arg.fcall->static_chain_p)
    return stmt->arg.fcall->static_chain;
  else
    return NULL_TREE;
}

/* Sets the type DUMMY_TYPE of a dummy argument passed to the call. This is
   currently used for passing the type of a va_arg() call. */

static inline void
cil_call_set_dummy_arg (cil_stmt stmt, tree dummy_type)
{
  gcc_assert (stmt->opcode == CIL_CALL
	      || stmt->opcode == CIL_CALLI
	      || stmt->opcode == CIL_JMP);

  stmt->arg.fcall->static_chain = dummy_type;
}

/* Returns the type of the dummy argument passed to a call. This is currently
   used for passing the type of a va_arg() call. */

static inline tree
cil_call_dummy_arg (const_cil_stmt stmt)
{
  gcc_assert (stmt->opcode == CIL_CALL
	      || stmt->opcode == CIL_CALLI
	      || stmt->opcode == CIL_JMP);
  gcc_assert (!stmt->arg.fcall->static_chain_p);

  return stmt->arg.fcall->static_chain;
}

/* Return true if the callee accepts a variable number of arguments.  */

static inline bool
cil_call_vararg_p (const_cil_stmt stmt)
{
  gcc_assert (stmt->opcode == CIL_CALL
	      || stmt->opcode == CIL_CALLI
	      || stmt->opcode == CIL_JMP);

  return stmt->arg.fcall->vararg_p;
}

/* Return true if the callee doesn't have a prototype.  */

static inline bool
cil_call_missing_proto_p (const_cil_stmt stmt)
{
  gcc_assert (stmt->opcode == CIL_CALL
	      || stmt->opcode == CIL_CALLI
	      || stmt->opcode == CIL_JMP);

  return stmt->arg.fcall->missing_proto_p;
}

/* Return a constant held by a CIL statement.  */

static inline tree
cil_cst (const_cil_stmt stmt)
{
  gcc_assert (opcode_arg_type (stmt->opcode) == CIL_CST);

  return stmt->arg.cst;
}

/* Return a string held by a CIL statement.  */

static inline tree
cil_string (const_cil_stmt stmt)
{
  gcc_assert (opcode_arg_type (stmt->opcode) == CIL_STRING);

  return stmt->arg.str;
}

/******************************************************************************
 * CIL sequence inlined functions                                             *
 ******************************************************************************/

/* Return the first node in CIL sequence S.  */

static inline cil_seq_node
cil_seq_first (const_cil_seq s)
{
  return s ? s->first : NULL;
}

/* Return the first statement in CIL sequence S.  */

static inline cil_stmt
cil_seq_first_stmt (const_cil_seq s)
{
  cil_seq_node n = cil_seq_first (s);
  return (n) ? n->stmt : NULL;
}

/* Return the last node in CIL sequence S.  */

static inline cil_seq_node
cil_seq_last (const_cil_seq s)
{
  return s ? s->last : NULL;
}

/* Return the last statement in CIL sequence S.  */

static inline cil_stmt
cil_seq_last_stmt (const_cil_seq s)
{
  cil_seq_node n = cil_seq_last (s);
  return (n) ? n->stmt : NULL;
}

/* Set the last node in CIL sequence S to LAST.  */

static inline void
cil_seq_set_last (cil_seq s, cil_seq_node last)
{
  s->last = last;
}

/* Set the first node in CIL sequence S to FIRST.  */

static inline void
cil_seq_set_first (cil_seq s, cil_seq_node first)
{
  s->first = first;
}

/* Return true if CIL sequence S is empty.  */

static inline bool
cil_seq_empty_p (const_cil_seq s)
{
  return s == NULL || s->first == NULL;
}

/******************************************************************************
 * CIL statement iterator inlined functions                                   *
 ******************************************************************************/

/* Return a new iterator pointing to SEQ's first statement.  */

static inline cil_stmt_iterator
csi_start (cil_seq seq)
{
  cil_stmt_iterator i;

  i.ptr = cil_seq_first (seq);
  i.seq = seq;
  i.bb = (i.ptr && i.ptr->stmt) ? cil_bb (i.ptr->stmt) : NULL;

  return i;
}

/* Return a new iterator pointing to the first statement in basic block BB.  */

static inline cil_stmt_iterator
csi_start_bb (basic_block bb)
{
  cil_stmt_iterator i;
  cil_seq seq;

  seq = cil_bb_seq (bb);
  i.ptr = cil_seq_first (seq);
  i.seq = seq;
  i.bb = bb;

  return i;
}

/* Return a new iterator initially pointing to SEQ's last statement.  */

static inline cil_stmt_iterator
csi_last (cil_seq seq)
{
  cil_stmt_iterator i;

  i.ptr = cil_seq_last (seq);
  i.seq = seq;
  i.bb = (i.ptr && i.ptr->stmt) ? cil_bb (i.ptr->stmt) : NULL;

  return i;
}


/* Return a new iterator pointing to the last statement in basic block BB.  */

static inline cil_stmt_iterator
csi_last_bb (basic_block bb)
{
  cil_stmt_iterator i;
  cil_seq seq;

  seq = cil_bb_seq (bb);
  i.ptr = cil_seq_last (seq);
  i.seq = seq;
  i.bb = bb;

  return i;
}

/* Return true if I is at the end of its sequence.  */

static inline bool
csi_end_p (cil_stmt_iterator i)
{
  return i.ptr == NULL;
}

/* Return true if I is one statement before the end of its sequence.  */

static inline bool
csi_one_before_end_p (cil_stmt_iterator i)
{
  return i.ptr != NULL && i.ptr->next == NULL;
}

/* Return true if I points to the first statement of its sequence.  */

static inline bool
csi_first_p (cil_stmt_iterator i)
{
  return i.ptr == cil_seq_first (i.seq);
}

/* Advance the iterator to the next CIL statement.  */

static inline void
csi_next (cil_stmt_iterator *i)
{
  i->ptr = i->ptr->next;
}

/* Advance the iterator to the previous CIL statement.  */

static inline void
csi_prev (cil_stmt_iterator *i)
{
  i->ptr = i->ptr->prev;
}

/* Return the current stmt.  */

static inline cil_stmt
csi_stmt (cil_stmt_iterator i)
{
  return i.ptr->stmt;
}

/* Return a pointer to the current stmt.  */

static inline cil_stmt *
csi_stmt_ptr (cil_stmt_iterator *i)
{
  return &i->ptr->stmt;
}

/* Return the basic block associated with this iterator.  */

static inline basic_block
csi_bb (cil_stmt_iterator i)
{
  return i.bb;
}

/* Return the sequence associated with this iterator.  */

static inline cil_seq
csi_seq (cil_stmt_iterator i)
{
  return i.seq;
}

#endif /* !CIL_STMT_INLINE_H */

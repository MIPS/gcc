/* Type related function prototypes and declarations.

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

#ifndef CIL_TYPES_H
#define CIL_TYPES_H

#include "coretypes.h"
#include "line-map.h"

/******************************************************************************
 * Misc types                                                                 *
 ******************************************************************************/

/* Referenced string entry.  */

struct str_ref_d GTY(())
{
  /* STRING_CST node.  */
  tree cst;

  /* Unique ID.  */
  size_t id;
};

typedef struct str_ref_d *str_ref;
typedef const struct str_ref_d *const_str_ref;

/* Mapping for label's whose addresses have been taken.  */

struct label_addr_d GTY(())
{
  /* LABEL_DECL node.  */
  tree label;

  /* Unique id as an INTEGER_CST of type intSI_type_node.  */
  tree id;
};

typedef struct label_addr_d *label_addr;
typedef const struct label_addr_d *const_label_addr;

/******************************************************************************
 * CIL statements                                                             *
 ******************************************************************************/

/* CIL opcodes, only the relevant opcodes are represented. Opcodes which have
   no equivalent in GIMPLE or are not useful for emission are not present.  */

enum cil_opcode
{
#define CIL_INSTRDEF(A,B,C,D) A,
#include "cil-instr.def"
#undef CIL_INSTRDEF
CIL_NUM_INSTRS
};

/* CIL statement argument type, used by the garbage collector.  */

enum cil_arg_type
{
#define CIL_ARGTYPE(A) A,
#include "cil-argdef.def"
#undef CIL_ARGTYPE
CIL_NUM_ARGTYPE
};

extern enum cil_arg_type opcode_arg_types[];

static inline enum cil_arg_type opcode_arg_type (enum cil_opcode cil_opcode)
{
  return opcode_arg_types[cil_opcode];
}

/* Represents the information provided by a CALL_EXPR when converted to a CIL
   CALL or CALLI instruction. The FTYPE fields points to the called function
   type, the FDECL field to the function declaration (or NULL_TREE for a CALLI),
   the ARGLIST holds the types of the arguments which were passed to the
   function (including those after the ellipsis in a variable argument call
   and the NARGS field holds their number. If the STATIC_CHAIN_P field is
   set then the STATIC_CHAIN field holds the type of the static chain. If
   STATIC_CHAIN is non-NULL and STATIC_CHAIN_P is not set then STATIC_CHAIN
   holds the type of a dummy argument (like the type passed to a va_arg() call)
   which shall be treated differently compared to the other arguments. The
   VARARG field is set to true if the callee accepts a variable number of
   arguments, false otherwise.  */

struct cil_call_arg_d GTY (())
{
  tree ftype;
  tree fdecl;
  tree * GTY ((length ("%h.nargs"))) arguments;
  size_t nargs;
  tree static_chain;
  unsigned vararg_p : 1;
  unsigned missing_proto_p : 1;
  unsigned static_chain_p : 1;
};

typedef struct cil_call_arg_d *cil_call_arg;

/* An array of CASE_LABEL_EXPRs used as the argument of CIL_SWITCH statements.
   The entry 0 is the default label. Check tree.def for more information on
   CASE_LABEL_EXPRs.  */

struct cil_switch_arg_d GTY (())
{
  size_t ncases;
  tree * GTY ((length ("%h.ncases"))) cases;
};

typedef struct cil_switch_arg_d *cil_switch_arg;

/* A CIL statement. The stack image pointed by the STACK field represents the
   various types held in the stack slots. This image is taken *before* the
   statement has executed.  */

struct cil_stmt_d GTY (())
{
  /* Opcode of this statement.  */
  ENUM_BITFIELD (cil_opcode) opcode : 8;

  /* Instruction prefixes. Unaligned and alignment can be specified on the same
     instruction depending on the opcode. For the alignment prefix the minimum
     alignment (1, 2 or 4) is also specified.  */
  unsigned prefix_tail : 1;
  unsigned prefix_unaligned : 1;
  unsigned alignment : 3;
  unsigned prefix_volatile : 1;
  unsigned short_ctor : 1;  /* short form of vector ctor */

  /* Statement argument if present. Can be a variable, type, field, label or
     function declaration; a list of switch labels, a function type descriptor
     or a constant (either integer or real).  */
  union
    {
      tree GTY ((tag ("CIL_VAR"))) var;
      tree GTY ((tag ("CIL_TYPE"))) type;
      tree GTY ((tag ("CIL_FIELD"))) field;
      tree GTY ((tag ("CIL_LABEL"))) label;
      cil_switch_arg GTY ((tag ("CIL_LABELS"))) labels;
      tree GTY ((tag ("CIL_FUNC"))) func;
      cil_call_arg GTY ((tag ("CIL_FCALL"))) fcall;
      tree GTY ((tag ("CIL_CST"))) cst;
      tree GTY ((tag ("CIL_STRING"))) str;
    } GTY ((desc ("opcode_arg_type (%0.opcode)"))) arg;

  /* Basic block holding this statement.  */
  struct basic_block_def *bb;

  /* Original location in the source.  */
  source_location locus;
};

typedef struct cil_stmt_d *cil_stmt;
typedef const struct cil_stmt_d *const_cil_stmt;

/******************************************************************************
 * CIL sequence                                                               *
 ******************************************************************************/

/* A node in a cil_seq_d.  */

struct cil_seq_node_d GTY((chain_next ("%h.next"), chain_prev ("%h.prev")))
{
  cil_stmt stmt;
  struct cil_seq_node_d *prev;
  struct cil_seq_node_d *next;
};

typedef struct cil_seq_node_d *cil_seq_node;

/* A double-linked sequence of CIL statements.  */

struct cil_seq_d GTY ((chain_next ("%h.next_free")))
{
  /* First and last statements in the sequence.  */
  cil_seq_node first;
  cil_seq_node last;

  /* Sequences are created/destroyed frequently.  To minimize
     allocation activity, deallocated sequences are kept in a pool of
     available sequences.  This is the pointer to the next free
     sequence in the pool.  */
  struct cil_seq_d *next_free;
};

typedef struct cil_seq_d *cil_seq;
typedef const struct cil_seq_d *const_cil_seq;

/* Mapping for basic blocks and CIL sequences.  */

struct cil_basic_block_d GTY (())
{
  /* Basic block, defined as a generic pointer in order not to pull more
     header files in there.  */
  void * GTY ((skip)) bb;

  /* CIL sequence associated with the basic block.  */
  cil_seq seq;
};

typedef struct cil_basic_block_d *cil_basic_block;
typedef const struct cil_basic_block_d *const_cil_basic_block;

/******************************************************************************
 * Per-function target specific global data                                   *
 ******************************************************************************/

struct htab;

/* Per-function machine data.  */
struct machine_function GTY(())
{
  unsigned int label_id;
  tree label_addrs;
  bool locals_init;

  /* The field has_vec tracks whether this function manipulates vector types.
     It is meant to trigger the pass cil-lower only when needed (not
     implemented yet).  */
  bool has_vec;

  /* Hash table used for mapping CIL sequences to GCC's basic blocks.  */
  struct htab * GTY ((param_is (struct cil_basic_block_d))) bb_seqs;
};

/******************************************************************************
 * Vector SIMD Type Selection                                                 *
 ******************************************************************************/

enum simd_backend {
  UNDEF_SIMD,
  GCC_SIMD,
  MONO_SIMD
};

extern enum simd_backend simd_type;

#endif /* !CIL_TYPES_H */

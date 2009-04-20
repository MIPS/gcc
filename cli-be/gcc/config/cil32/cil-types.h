/* Type related function prototypes and declarations.

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
  CIL_ADD, /* Add numeric values */
  CIL_AND, /* Logical and */
  CIL_ARGLIST, /* Get argument list */
  CIL_BEQ, /* Branch on equal */
  CIL_BGE, /* Branch on greater or equal */
  CIL_BGE_UN, /* Branch on greater or equal, unsigned or unordered */
  CIL_BGT, /* Branch on greater than */
  CIL_BGT_UN, /* Branch on greater than, unsigned or unordered */
  CIL_BLE, /* Branch on less or equal */
  CIL_BLE_UN, /* Branch on less or equal, unsigned or unordered */
  CIL_BLT, /* Branch on less than */
  CIL_BLT_UN, /* Branch on less than, unsigned or unordered */
  CIL_BNE_UN, /* Branch on not equal, unsigned or unordered */
  CIL_BR, /* Branch unconditional */
  CIL_BREAK, /* Breakpoint instruction */
  CIL_BRFALSE, /* Branch on false, null or zero */
  CIL_BRTRUE, /* Branch on non-false, non-null, or non-zero */
  CIL_CALL, /* Method call */
  CIL_CALLI, /* Indirect method call */
  CIL_CEQ, /* Compare equal */
  CIL_CGT, /* Compare greater than */
  CIL_CGT_UN, /* Compare greater than, unsigned or unordered */
  CIL_CKFINITE, /* Check for a finite real number */
  CIL_CLT, /* Compare less than */
  CIL_CLT_UN, /* Compare less than unordered */
  CIL_CONV_I1, /* Convert to int8 */
  CIL_CONV_I2, /* Convert to int16 */
  CIL_CONV_I4, /* Convert to int32 */
  CIL_CONV_I8, /* Convert to int64 */
  CIL_CONV_R4, /* Convert to float32 */
  CIL_CONV_R8, /* Convert to float64 */
  CIL_CONV_U1, /* Convert to unsigned int8 */
  CIL_CONV_U2, /* Convert to unsigned int16 */
  CIL_CONV_U4, /* Convert to unsigned int32 */
  CIL_CONV_U8, /* Convert to unsigned int64 */
  CIL_CONV_I, /* Convert to native int */
  CIL_CONV_U, /* Convert to unsigned native int */
  CIL_CONV_R_UN, /* Convert unsigned integer to floating-point */
  CIL_CPBLK, /* Copy data from memory to memory */
  CIL_DIV, /* Divide values */
  CIL_DIV_UN, /* Divide values, unsigned */
  CIL_DUP, /* Duplicate the value on top of the stack */
  CIL_INITBLK, /* Set all bytes in a block of memory to a given byte value */
  CIL_INITOBJ, /* Initialize the value at an address */
  CIL_JMP, /* Jump to method */
  CIL_LDARG, /* Load an argument on the operand stack */
  CIL_LDARGA, /* Load an argument's address */
  CIL_LDC_I4, /* Load an int32 numeric constant on the stack */
  CIL_LDC_I8, /* Load an int64 numeric constant on the stack */
  CIL_LDC_R4, /* Load a float32 numeric constant on the stack */
  CIL_LDC_R8, /* Load a float64 numeric constant on the stack */
  CIL_LDFLD, /* Load field of an object */
  CIL_LDFLDA, /* Load field address */
  CIL_LDFTN, /* Load method pointer */
  CIL_LDIND_I1, /* Indirect load value of type int8 */
  CIL_LDIND_I2, /* Indirect load value of type int16 */
  CIL_LDIND_I4, /* Indirect load value of type int32 */
  CIL_LDIND_I8, /* Indirect load value of type int64 */
  CIL_LDIND_U1, /* Indirect load value of type unsigned int8 */
  CIL_LDIND_U2, /* Indirect load value of type unsigned int16 */
  CIL_LDIND_U4, /* Indirect load value of type unsigned int32 */
  CIL_LDIND_U8, /* Indirect load value of type unsigned int64 */
  CIL_LDIND_R4, /* Indirect load value of type float32 */
  CIL_LDIND_R8, /* Indirect load value of type float64 */
  CIL_LDIND_I, /* Indirect load value of type native int */
  CIL_LDLOC, /* Load local variable onto the stack */
  CIL_LDLOCA, /* Load local variable address */
  CIL_LDOBJ, /* Copy a value from an address to the stack */
  CIL_LDSFLD, /* Load static field of a class */
  CIL_LDSFLDA, /* Load static field address */
  CIL_LOCALLOC, /* Allocate space from the local memory pool */
  CIL_MUL, /* Multiply values */
  CIL_NEG, /* Negate value */
  CIL_NOT, /* Bitwise complement */
  CIL_OR, /* Bitwise or */
  CIL_POP, /* Remove the top element of the stack */
  CIL_REM, /* Compute remainder */
  CIL_REM_UN, /* Compute integer remainder, unsigned */
  CIL_RET, /* Return from method */
  CIL_SHL, /* Shift integer left */
  CIL_SHR, /* Shift integer right */
  CIL_SHR_UN, /* Shift integer right, unsigned */
  CIL_STARG, /* Store a value in an argument slot */
  CIL_STFLD, /* Store field of an object */
  CIL_STIND_I1, /* Store int8 value indirect from stack */
  CIL_STIND_I2, /* Store int16 value indirect from stack */
  CIL_STIND_I4, /* Store int32 value indirect from stack */
  CIL_STIND_I8, /* Store int64 value indirect from stack */
  CIL_STIND_R4, /* Store float32 value indirect from stack */
  CIL_STIND_R8, /* Store float64 value indirect from stack */
  CIL_STIND_I, /* Store native int value indirect from stack */
  CIL_STLOC, /* Pop value from stack to local variable */
  CIL_STOBJ, /* Store a value at an address */
  CIL_STSFLD, /* Store static field of a class */
  CIL_SUB, /* Substract numeric value */
  CIL_SWITCH, /* Table switch based on value */
  CIL_XOR, /* Bitwise exclusive or */

  /* Artificial opcodes */
  CIL_ASM /* Artificial opcode representing ASM_EXPR statements */
};

/* CIL statement argument type, used by the garbage collector.  */

enum cil_arg_type
{
  CIL_VAR, /* Variable declaration */
  CIL_TYPE, /* Type declaration */
  CIL_FIELD, /* Field declaration */
  CIL_LABEL, /* Label declaration */
  CIL_LABELS, /* Switch case labels */
  CIL_FUNC, /* Function declaration */
  CIL_FCALL, /* Function call description */
  CIL_CST, /* Integer or real constant */
  CIL_STRING, /* A string (held in a tree, not a char *) */
  CIL_NONE /* No argument */
};

extern enum cil_arg_type opcode_arg_type (enum cil_opcode);

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

  /* Hash table used for mapping CIL sequences to GCC's basic blocks.  */
  struct htab * GTY ((param_is (struct cil_basic_block_d))) bb_seqs;
};

#endif /* !CIL_TYPES_H */

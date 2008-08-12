/* Variable tracking routines for the GNU compiler.
   Copyright (C) 2002, 2003, 2004, 2005, 2007, 2008
   Free Software Foundation, Inc.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   GCC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.  */

/* This file contains the variable tracking pass.  It computes where
   variables are located (which registers or where in memory) at each position
   in instruction stream and emits notes describing the locations.
   Debug information (DWARF2 location lists) is finally generated from
   these notes.
   With this debug information, it is possible to show variables
   even when debugging optimized code.

   How does the variable tracking pass work?

   First, it scans RTL code for uses, stores and clobbers (register/memory
   references in instructions), for call insns and for stack adjustments
   separately for each basic block and saves them to an array of micro
   operations.
   The micro operations of one instruction are ordered so that
   pre-modifying stack adjustment < use < use with no var < call insn <
     < set < clobber < post-modifying stack adjustment

   Then, a forward dataflow analysis is performed to find out how locations
   of variables change through code and to propagate the variable locations
   along control flow graph.
   The IN set for basic block BB is computed as a union of OUT sets of BB's
   predecessors, the OUT set for BB is copied from the IN set for BB and
   is changed according to micro operations in BB.

   The IN and OUT sets for basic blocks consist of a current stack adjustment
   (used for adjusting offset of variables addressed using stack pointer),
   the table of structures describing the locations of parts of a variable
   and for each physical register a linked list for each physical register.
   The linked list is a list of variable parts stored in the register,
   i.e. it is a list of triplets (reg, decl, offset) where decl is
   REG_EXPR (reg) and offset is REG_OFFSET (reg).  The linked list is used for
   effective deleting appropriate variable parts when we set or clobber the
   register.

   There may be more than one variable part in a register.  The linked lists
   should be pretty short so it is a good data structure here.
   For example in the following code, register allocator may assign same
   register to variables A and B, and both of them are stored in the same
   register in CODE:

     if (cond)
       set A;
     else
       set B;
     CODE;
     if (cond)
       use A;
     else
       use B;

   Finally, the NOTE_INSN_VAR_LOCATION notes describing the variable locations
   are emitted to appropriate positions in RTL code.  Each such a note describes
   the location of one variable at the point in instruction stream where the
   note is.  There is no need to emit a note for each variable before each
   instruction, we only emit these notes where the location of variable changes
   (this means that we also emit notes for changes between the OUT set of the
   previous block and the IN set of the current block).

   The notes consist of two parts:
   1. the declaration (from REG_EXPR or MEM_EXPR)
   2. the location of a variable - it is either a simple register/memory
      reference (for simple variables, for example int),
      or a parallel of register/memory references (for a large variables
      which consist of several parts, for example long long).

*/

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "tree.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "flags.h"
#include "output.h"
#include "insn-config.h"
#include "reload.h"
#include "sbitmap.h"
#include "alloc-pool.h"
#include "fibheap.h"
#include "hashtab.h"
#include "regs.h"
#include "expr.h"
#include "timevar.h"
#include "tree-pass.h"
#include "cselib.h"
#include "target.h"

/* Type of micro operation.  */
enum micro_operation_type
{
  MO_USE,	/* Use location (REG or MEM).  */
  MO_USE_NO_VAR,/* Use location which is not associated with a variable
		   or the variable is not trackable.  */
  MO_VAL_USE,	/* Use location which is associated with a value.  */
  MO_VAL_LOC,   /* Use location which appears in a debug insn.  */
  MO_VAL_SET,	/* Set location associated with a value.  */
  MO_SET,	/* Set location.  */
  MO_COPY,	/* Copy the same portion of a variable from one
		   location to another.  */
  MO_CLOBBER,	/* Clobber location.  */
  MO_CALL,	/* Call insn.  */
  MO_ADJUST	/* Adjust stack pointer.  */

};

static const char * const ATTRIBUTE_UNUSED
micro_operation_type_name[] = {
  "MO_USE",
  "MO_USE_NO_VAR",
  "MO_VAL_USE",
  "MO_VAL_LOC",
  "MO_VAL_SET",
  "MO_SET",
  "MO_COPY",
  "MO_CLOBBER",
  "MO_CALL",
  "MO_ADJUST"
};

/* Where shall the note be emitted?  BEFORE or AFTER the instruction.  */
enum emit_note_where
{
  EMIT_NOTE_BEFORE_INSN,
  EMIT_NOTE_AFTER_INSN
};

/* Structure holding information about micro operation.  */
typedef struct micro_operation_def
{
  /* Type of micro operation.  */
  enum micro_operation_type type;

  union {
    /* Location.  For MO_SET and MO_COPY, this is the SET that
       performs the assignment, if known, otherwise it is the target
       of the assignment.  For MO_VAL_USE and MO_VAL_SET, it is a
       CONCAT of the VALUE and the LOC associated with it.  For
       MO_VAL_LOC, it is a CONCAT of the VALUE and the VAR_LOCATION
       associated with it.  */
    rtx loc;

    /* Stack adjustment.  */
    HOST_WIDE_INT adjust;
  } u;

  /* The instruction which the micro operation is in, for MO_USE,
     MO_USE_NO_VAR, MO_CALL and MO_ADJUST, or the subsequent
     instruction or note in the original flow (before any var-tracking
     notes are inserted, to simplify emission of notes), for MO_SET
     and MO_CLOBBER.  */
  rtx insn;
} micro_operation;

/* A declaration of a variable, or an RTL value being handled like a
   declaration.  */
typedef struct decl_or_value
{
  /* An opaque pointer to the decl or the value.  */
  void *ptr;
} decl_or_value;

/* Structure for passing some other parameters to function
   emit_note_insn_var_location.  */
typedef struct emit_note_data_def
{
  /* The instruction which the note will be emitted before/after.  */
  rtx insn;

  /* Where the note will be emitted (before/after insn)?  */
  enum emit_note_where where;

  /* The variables and values active at this point.  */
  htab_t vars;
} emit_note_data;

/* Description of location of a part of a variable.  The content of a physical
   register is described by a chain of these structures.
   The chains are pretty short (usually 1 or 2 elements) and thus
   chain is the best data structure.  */
typedef struct attrs_def
{
  /* Pointer to next member of the list.  */
  struct attrs_def *next;

  /* The rtx of register.  */
  rtx loc;

  /* The declaration corresponding to LOC.  */
  decl_or_value dv;

  /* Offset from start of DECL.  */
  HOST_WIDE_INT offset;
} *attrs;

/* Structure holding the IN or OUT set for a basic block.  */
typedef struct dataflow_set_def
{
  /* Adjustment of stack offset.  */
  HOST_WIDE_INT stack_adjust;

  /* Attributes for registers (lists of attrs).  */
  attrs regs[FIRST_PSEUDO_REGISTER];

  /* Variable locations.  */
  htab_t vars;
} dataflow_set;

/* The structure (one for each basic block) containing the information
   needed for variable tracking.  */
typedef struct variable_tracking_info_def
{
  /* Number of micro operations stored in the MOS array.  */
  int n_mos;

  /* The array of micro operations.  */
  micro_operation *mos;

  /* The IN and OUT set for dataflow analysis.  */
  dataflow_set in;
  dataflow_set out;

  /* Has the block been visited in DFS?  */
  bool visited;
} *variable_tracking_info;

/* Structure for chaining the locations.  */
typedef struct location_chain_def
{
  /* Next element in the chain.  */
  struct location_chain_def *next;

  /* The location (REG, MEM or VALUE).  */
  rtx loc;

  /* The "value" stored in this location.  */
  rtx set_src;

  /* Initialized? */
  enum var_init_status init;
} *location_chain;

/* Structure describing one part of variable.  */
typedef struct variable_part_def
{
  /* Chain of locations of the part.  */
  location_chain loc_chain;

  /* Location which was last emitted to location list.  */
  rtx cur_loc;

  /* The offset in the variable.  */
  HOST_WIDE_INT offset;
} variable_part;

/* Maximum number of location parts.  */
#define MAX_VAR_PARTS 16

/* Structure describing where the variable is located.  */
typedef struct variable_def
{
  /* The declaration of the variable, or an RTL value being handled
     like a declaration.  */
  decl_or_value dv;

  /* Reference count.  */
  int refcount;

  /* Number of variable parts.  */
  int n_var_parts;

  /* The variable parts.  */
  variable_part var_part[1];
} *variable;
typedef const struct variable_def *const_variable;

/* Hash function for DECL for VARIABLE_HTAB.  */
#define VARIABLE_HASH_VAL(decl) (DECL_UID (decl))

/* Pointer to the BB's information specific to variable tracking pass.  */
#define VTI(BB) ((variable_tracking_info) (BB)->aux)

/* Macro to access MEM_OFFSET as an HOST_WIDE_INT.  Evaluates MEM twice.  */
#define INT_MEM_OFFSET(mem) (MEM_OFFSET (mem) ? INTVAL (MEM_OFFSET (mem)) : 0)

/* Alloc pool for struct attrs_def.  */
static alloc_pool attrs_pool;

/* Alloc pool for struct variable_def with MAX_VAR_PARTS entries.  */
static alloc_pool var_pool;

/* Alloc pool for struct variable_def with a single var_part entry.  */
static alloc_pool valvar_pool;

/* Alloc pool for struct location_chain_def.  */
static alloc_pool loc_chain_pool;

/* Changed variables, notes will be emitted for them.  */
static htab_t changed_variables;

/* Shall notes be emitted?  */
static bool emit_notes;

/* Scratch register bitmap used by cselib_expand_value_rtx.  */
static bitmap scratch_regs = NULL;

/* Variable used to tell whether cselib_process_insn called our hook.  */
static bool cselib_hook_called;

/* Local function prototypes.  */
static void stack_adjust_offset_pre_post (rtx, HOST_WIDE_INT *,
					  HOST_WIDE_INT *);
static void insn_stack_adjust_offset_pre_post (rtx, HOST_WIDE_INT *,
					       HOST_WIDE_INT *);
static void bb_stack_adjust_offset (basic_block);
static bool vt_stack_adjustments (void);
static rtx adjust_stack_reference (rtx, HOST_WIDE_INT);
static hashval_t variable_htab_hash (const void *);
static int variable_htab_eq (const void *, const void *);
static void variable_htab_free (void *);

static void init_attrs_list_set (attrs *);
static void attrs_list_clear (attrs *);
static attrs attrs_list_member (attrs, decl_or_value, HOST_WIDE_INT);
static void attrs_list_insert (attrs *, decl_or_value, HOST_WIDE_INT, rtx);
static void attrs_list_copy (attrs *, attrs);
static void attrs_list_union (attrs *, attrs);

static void vars_clear (htab_t);
static variable unshare_variable (void **slot, variable var,
				  enum var_init_status);
static int vars_copy_1 (void **, void *);
static void vars_copy (htab_t, htab_t);
static tree var_debug_decl (tree);
static void var_reg_set (dataflow_set *, rtx, enum var_init_status, rtx);
static void var_reg_delete_and_set (dataflow_set *, rtx, bool, 
				    enum var_init_status, rtx);
static void var_reg_delete (dataflow_set *, rtx, bool);
static void var_regno_delete (dataflow_set *, int);
static void var_mem_set (dataflow_set *, rtx, enum var_init_status, rtx);
static void var_mem_delete_and_set (dataflow_set *, rtx, bool, 
				    enum var_init_status, rtx);
static void var_mem_delete (dataflow_set *, rtx, bool);

static void dataflow_set_init (dataflow_set *, int);
static void dataflow_set_clear (dataflow_set *);
static void dataflow_set_copy (dataflow_set *, dataflow_set *);
static int variable_union_info_cmp_pos (const void *, const void *);
static int variable_union (void **, void *);
static void dataflow_set_union (dataflow_set *, dataflow_set *);
static variable find_canonical_value (variable, dataflow_set *, variable);
static void dataflow_set_remove_unavailable (dataflow_set *);
static bool variable_part_different_p (variable_part *, variable_part *);
static bool variable_different_p (variable, variable, bool);
static int dataflow_set_different_1 (void **, void *);
static int dataflow_set_different_2 (void **, void *);
static bool dataflow_set_different (dataflow_set *, dataflow_set *);
static void dataflow_set_destroy (dataflow_set *);

static bool contains_symbol_ref (rtx);
static bool track_expr_p (tree, bool);
static bool same_variable_part_p (rtx, tree, HOST_WIDE_INT);
static int count_uses (rtx *, void *);
static void count_uses_1 (rtx *, void *);
static void count_stores (rtx, const_rtx, void *);
static int add_uses (rtx *, void *);
static void add_uses_1 (rtx *, void *);
static void add_stores (rtx, const_rtx, void *);
static bool compute_bb_dataflow (basic_block);
static void vt_find_locations (void);

static void dump_attrs_list (attrs);
static int dump_variable_slot (void **, void *);
static void dump_variable (variable);
static void dump_vars (htab_t);
static void dump_dataflow_set (dataflow_set *);
static void dump_dataflow_sets (void);

static void variable_was_changed (variable, htab_t);
static void set_slot_part (dataflow_set *, rtx, void **,
			   decl_or_value, HOST_WIDE_INT,
			   enum var_init_status, rtx);
static void set_variable_part (dataflow_set *, rtx,
			       decl_or_value, HOST_WIDE_INT,
			       enum var_init_status, rtx);
static void clobber_variable_part (dataflow_set *, rtx,
				   decl_or_value, HOST_WIDE_INT, rtx);
static void delete_variable_part (dataflow_set *, rtx,
				  decl_or_value, HOST_WIDE_INT);
static int emit_note_insn_var_location (void **, void *);
static void emit_notes_for_changes (rtx, enum emit_note_where, htab_t vars);
static int emit_notes_for_differences_1 (void **, void *);
static int emit_notes_for_differences_2 (void **, void *);
static void emit_notes_for_differences (rtx, dataflow_set *, dataflow_set *);
static void emit_notes_in_bb (basic_block);
static void vt_emit_notes (void);

static bool vt_get_decl_and_offset (rtx, tree *, HOST_WIDE_INT *);
static void vt_add_function_parameters (void);
static void vt_initialize (void);
static void vt_finalize (void);

/* Given a SET, calculate the amount of stack adjustment it contains
   PRE- and POST-modifying stack pointer.
   This function is similar to stack_adjust_offset.  */

static void
stack_adjust_offset_pre_post (rtx pattern, HOST_WIDE_INT *pre,
			      HOST_WIDE_INT *post)
{
  rtx src = SET_SRC (pattern);
  rtx dest = SET_DEST (pattern);
  enum rtx_code code;

  if (dest == stack_pointer_rtx)
    {
      /* (set (reg sp) (plus (reg sp) (const_int))) */
      code = GET_CODE (src);
      if (! (code == PLUS || code == MINUS)
	  || XEXP (src, 0) != stack_pointer_rtx
	  || GET_CODE (XEXP (src, 1)) != CONST_INT)
	return;

      if (code == MINUS)
	*post += INTVAL (XEXP (src, 1));
      else
	*post -= INTVAL (XEXP (src, 1));
    }
  else if (MEM_P (dest))
    {
      /* (set (mem (pre_dec (reg sp))) (foo)) */
      src = XEXP (dest, 0);
      code = GET_CODE (src);

      switch (code)
	{
	case PRE_MODIFY:
	case POST_MODIFY:
	  if (XEXP (src, 0) == stack_pointer_rtx)
	    {
	      rtx val = XEXP (XEXP (src, 1), 1);
	      /* We handle only adjustments by constant amount.  */
	      gcc_assert (GET_CODE (XEXP (src, 1)) == PLUS &&
			  GET_CODE (val) == CONST_INT);
	      
	      if (code == PRE_MODIFY)
		*pre -= INTVAL (val);
	      else
		*post -= INTVAL (val);
	      break;
	    }
	  return;

	case PRE_DEC:
	  if (XEXP (src, 0) == stack_pointer_rtx)
	    {
	      *pre += GET_MODE_SIZE (GET_MODE (dest));
	      break;
	    }
	  return;

	case POST_DEC:
	  if (XEXP (src, 0) == stack_pointer_rtx)
	    {
	      *post += GET_MODE_SIZE (GET_MODE (dest));
	      break;
	    }
	  return;

	case PRE_INC:
	  if (XEXP (src, 0) == stack_pointer_rtx)
	    {
	      *pre -= GET_MODE_SIZE (GET_MODE (dest));
	      break;
	    }
	  return;

	case POST_INC:
	  if (XEXP (src, 0) == stack_pointer_rtx)
	    {
	      *post -= GET_MODE_SIZE (GET_MODE (dest));
	      break;
	    }
	  return;

	default:
	  return;
	}
    }
}

/* Given an INSN, calculate the amount of stack adjustment it contains
   PRE- and POST-modifying stack pointer.  */

static void
insn_stack_adjust_offset_pre_post (rtx insn, HOST_WIDE_INT *pre,
				   HOST_WIDE_INT *post)
{
  *pre = 0;
  *post = 0;

  if (GET_CODE (PATTERN (insn)) == SET)
    stack_adjust_offset_pre_post (PATTERN (insn), pre, post);
  else if (GET_CODE (PATTERN (insn)) == PARALLEL
	   || GET_CODE (PATTERN (insn)) == SEQUENCE)
    {
      int i;

      /* There may be stack adjustments inside compound insns.  Search
	 for them.  */
      for ( i = XVECLEN (PATTERN (insn), 0) - 1; i >= 0; i--)
	if (GET_CODE (XVECEXP (PATTERN (insn), 0, i)) == SET)
	  stack_adjust_offset_pre_post (XVECEXP (PATTERN (insn), 0, i),
					pre, post);
    }
}

/* Compute stack adjustment in basic block BB.  */

static void
bb_stack_adjust_offset (basic_block bb)
{
  HOST_WIDE_INT offset;
  int i;

  offset = VTI (bb)->in.stack_adjust;
  for (i = 0; i < VTI (bb)->n_mos; i++)
    {
      if (VTI (bb)->mos[i].type == MO_ADJUST)
	offset += VTI (bb)->mos[i].u.adjust;
      else if (VTI (bb)->mos[i].type != MO_CALL)
	{
	  if (MEM_P (VTI (bb)->mos[i].u.loc))
	    {
	      VTI (bb)->mos[i].u.loc
		= adjust_stack_reference (VTI (bb)->mos[i].u.loc, -offset);
	    }
	}
    }
  VTI (bb)->out.stack_adjust = offset;
}

/* Compute stack adjustments for all blocks by traversing DFS tree.
   Return true when the adjustments on all incoming edges are consistent.
   Heavily borrowed from pre_and_rev_post_order_compute.  */

static bool
vt_stack_adjustments (void)
{
  edge_iterator *stack;
  int sp;

  /* Initialize entry block.  */
  VTI (ENTRY_BLOCK_PTR)->visited = true;
  VTI (ENTRY_BLOCK_PTR)->out.stack_adjust = INCOMING_FRAME_SP_OFFSET;

  /* Allocate stack for back-tracking up CFG.  */
  stack = XNEWVEC (edge_iterator, n_basic_blocks + 1);
  sp = 0;

  /* Push the first edge on to the stack.  */
  stack[sp++] = ei_start (ENTRY_BLOCK_PTR->succs);

  while (sp)
    {
      edge_iterator ei;
      basic_block src;
      basic_block dest;

      /* Look at the edge on the top of the stack.  */
      ei = stack[sp - 1];
      src = ei_edge (ei)->src;
      dest = ei_edge (ei)->dest;

      /* Check if the edge destination has been visited yet.  */
      if (!VTI (dest)->visited)
	{
	  VTI (dest)->visited = true;
	  VTI (dest)->in.stack_adjust = VTI (src)->out.stack_adjust;
	  bb_stack_adjust_offset (dest);

	  if (EDGE_COUNT (dest->succs) > 0)
	    /* Since the DEST node has been visited for the first
	       time, check its successors.  */
	    stack[sp++] = ei_start (dest->succs);
	}
      else
	{
	  /* Check whether the adjustments on the edges are the same.  */
	  if (VTI (dest)->in.stack_adjust != VTI (src)->out.stack_adjust)
	    {
	      free (stack);
	      return false;
	    }

	  if (! ei_one_before_end_p (ei))
	    /* Go to the next edge.  */
	    ei_next (&stack[sp - 1]);
	  else
	    /* Return to previous level if there are no more edges.  */
	    sp--;
	}
    }

  free (stack);
  return true;
}

/* Adjust stack reference MEM by ADJUSTMENT bytes and make it relative
   to the argument pointer.  Return the new rtx.  */

static rtx
adjust_stack_reference (rtx mem, HOST_WIDE_INT adjustment)
{
  rtx addr, cfa, tmp;

#ifdef FRAME_POINTER_CFA_OFFSET
  adjustment -= FRAME_POINTER_CFA_OFFSET (current_function_decl);
  cfa = plus_constant (frame_pointer_rtx, adjustment);
#else
  adjustment -= ARG_POINTER_CFA_OFFSET (current_function_decl);
  cfa = plus_constant (arg_pointer_rtx, adjustment);
#endif

  addr = replace_rtx (copy_rtx (XEXP (mem, 0)), stack_pointer_rtx, cfa);
  tmp = simplify_rtx (addr);
  if (tmp)
    addr = tmp;

  return replace_equiv_address_nv (mem, addr);
}

/* Return true if a decl_or_value is a DECL or NULL.  */
static inline bool
dv_is_decl_p (decl_or_value dv)
{
  tree decl;

  if (!dv.ptr)
    return false;

  decl = (tree)dv.ptr;

  if (GET_CODE ((rtx)dv.ptr) == VALUE)
    return false;

  return true;
}

/* Return true if a decl_or_value is a VALUE rtl.  */
static inline bool
dv_is_value_p (decl_or_value dv)
{
  return dv.ptr && !dv_is_decl_p (dv);
}

/* Return the decl in the decl_or_value.  */
static inline tree
dv_as_decl (decl_or_value dv)
{
  gcc_assert (dv_is_decl_p (dv));
  return dv.ptr;
}

/* Return the value in the decl_or_value.  */
static inline rtx
dv_as_value (decl_or_value dv)
{
  gcc_assert (dv_is_value_p (dv));
  return (rtx)dv.ptr;
}

/* Return the opaque pointer in the decl_or_value.  */
static inline void *
dv_as_opaque (decl_or_value dv)
{
  return dv.ptr;
}

/* Return true if a decl_or_value must not have more than one variable
   part.  */
static inline bool
dv_onepart_p (decl_or_value dv)
{
  tree decl;

  if (!MAY_HAVE_DEBUG_INSNS)
    return false;

  if (dv_is_value_p (dv))
    return true;

  decl = dv_as_decl (dv);

  if (!decl)
    return true;

  return var_debug_value_for_decl (decl);
}

/* Return the variable pool to be used for dv, depending on whether it
   can have multiple parts or not.  */
static inline alloc_pool
dv_pool (decl_or_value dv)
{
  return dv_onepart_p (dv) ? valvar_pool : var_pool;
}

#define IS_DECL_CODE(C) ((C) == VAR_DECL || (C) == PARM_DECL \
			 || (C) == RESULT_DECL || (C) == COMPONENT_REF)

/* Check that VALUE won't ever look like a DECL.  */
static char check_value_is_not_decl [(!IS_DECL_CODE (VALUE))
				     ? 1 : -1] ATTRIBUTE_UNUSED;


/* Build a decl_or_value out of a decl.  */
static inline decl_or_value
dv_from_decl (tree decl)
{
  decl_or_value dv;
  gcc_assert (!decl || IS_DECL_CODE (TREE_CODE (decl)));
  dv.ptr = decl;
  return dv;
}

/* Build a decl_or_value out of a value.  */
static inline decl_or_value
dv_from_value (rtx value)
{
  decl_or_value dv;
  gcc_assert (value);
  dv.ptr = value;
  return dv;
}

static hashval_t
dv_htab_hash (decl_or_value dv)
{
  gcc_assert (dv.ptr);
  if (dv_is_value_p (dv))
    return -(hashval_t)(CSELIB_VAL_PTR (dv_as_value (dv))->value);
  else
    return (VARIABLE_HASH_VAL (dv_as_decl (dv)));
}

/* The hash function for variable_htab, computes the hash value
   from the declaration of variable X.  */

static hashval_t
variable_htab_hash (const void *x)
{
  const_variable const v = (const_variable) x;

  return dv_htab_hash (v->dv);
}

/* Compare the declaration of variable X with declaration Y.  */

static int
variable_htab_eq (const void *x, const void *y)
{
  const_variable const v = (const_variable) x;
  decl_or_value dv = *(decl_or_value const*)y;

  if (dv_as_opaque (v->dv) == dv_as_opaque (dv))
    return true;

#ifdef ENABLE_CHECKING
  {
    bool visv, dvisv;

    visv = dv_is_value_p (v->dv);
    dvisv = dv_is_value_p (dv);

    if (visv != dvisv)
      return false;

    if (visv)
      gcc_assert (CSELIB_VAL_PTR (dv_as_value (v->dv))
		  != CSELIB_VAL_PTR (dv_as_value (dv)));
    else
      gcc_assert (VARIABLE_HASH_VAL (dv_as_decl (v->dv))
		  != VARIABLE_HASH_VAL (dv_as_decl (dv)));
  }
#endif

  return false;
}

/* Free the element of VARIABLE_HTAB (its type is struct variable_def).  */

static void
variable_htab_free (void *elem)
{
  int i;
  variable var = (variable) elem;
  location_chain node, next;

  gcc_assert (var->refcount > 0);

  var->refcount--;
  if (var->refcount > 0)
    return;

  for (i = 0; i < var->n_var_parts; i++)
    {
      for (node = var->var_part[i].loc_chain; node; node = next)
	{
	  next = node->next;
	  pool_free (loc_chain_pool, node);
	}
      var->var_part[i].loc_chain = NULL;
    }
  pool_free (dv_pool (var->dv), var);
}

/* Initialize the set (array) SET of attrs to empty lists.  */

static void
init_attrs_list_set (attrs *set)
{
  int i;

  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    set[i] = NULL;
}

/* Make the list *LISTP empty.  */

static void
attrs_list_clear (attrs *listp)
{
  attrs list, next;

  for (list = *listp; list; list = next)
    {
      next = list->next;
      pool_free (attrs_pool, list);
    }
  *listp = NULL;
}

/* Return true if the pair of DECL and OFFSET is the member of the LIST.  */

static attrs
attrs_list_member (attrs list, decl_or_value dv, HOST_WIDE_INT offset)
{
  for (; list; list = list->next)
    if (dv_as_opaque (list->dv) == dv_as_opaque (dv) && list->offset == offset)
      return list;
  return NULL;
}

/* Insert the triplet DECL, OFFSET, LOC to the list *LISTP.  */

static void
attrs_list_insert (attrs *listp, decl_or_value dv,
		   HOST_WIDE_INT offset, rtx loc)
{
  attrs list;

  list = pool_alloc (attrs_pool);
  list->loc = loc;
  list->dv = dv;
  list->offset = offset;
  list->next = *listp;
  *listp = list;
}

/* Copy all nodes from SRC and create a list *DSTP of the copies.  */

static void
attrs_list_copy (attrs *dstp, attrs src)
{
  attrs n;

  attrs_list_clear (dstp);
  for (; src; src = src->next)
    {
      n = pool_alloc (attrs_pool);
      n->loc = src->loc;
      n->dv = src->dv;
      n->offset = src->offset;
      n->next = *dstp;
      *dstp = n;
    }
}

/* Add all nodes from SRC which are not in *DSTP to *DSTP.  */

static void
attrs_list_union (attrs *dstp, attrs src)
{
  for (; src; src = src->next)
    {
      if (!attrs_list_member (*dstp, src->dv, src->offset))
	attrs_list_insert (dstp, src->dv, src->offset, src->loc);
    }
}

/* Combine nodes that are not onepart nodes from SRC and SRC2 into
   *DSTP.  */

static void
attrs_list_mpdv_union (attrs *dstp, attrs src, attrs src2)
{
  gcc_assert (!*dstp);
  for (; src; src = src->next)
    {
      if (!dv_onepart_p (src->dv))
	attrs_list_insert (dstp, src->dv, src->offset, src->loc);
    }
  for (src = src2; src; src = src->next)
    {
      if (!dv_onepart_p (src->dv)
	  && !attrs_list_member (*dstp, src->dv, src->offset))
	attrs_list_insert (dstp, src->dv, src->offset, src->loc);
    }
}

/* Delete all variables from hash table VARS.  */

static void
vars_clear (htab_t vars)
{
  htab_empty (vars);
}

/* Return a copy of a variable VAR and insert it to dataflow set SET.  */

static variable
unshare_variable (void **slot, variable var,
		  enum var_init_status initialized)
{
  variable new_var;
  int i;

  new_var = pool_alloc (dv_pool (var->dv));
  new_var->dv = var->dv;
  new_var->refcount = 1;
  var->refcount--;
  new_var->n_var_parts = var->n_var_parts;

  for (i = 0; i < var->n_var_parts; i++)
    {
      location_chain node;
      location_chain *nextp;

      new_var->var_part[i].offset = var->var_part[i].offset;
      nextp = &new_var->var_part[i].loc_chain;
      for (node = var->var_part[i].loc_chain; node; node = node->next)
	{
	  location_chain new_lc;

	  new_lc = pool_alloc (loc_chain_pool);
	  new_lc->next = NULL;
	  if (node->init > initialized)
	    new_lc->init = node->init;
	  else
	    new_lc->init = initialized;
	  if (node->set_src && !(MEM_P (node->set_src)))
	    new_lc->set_src = node->set_src;
	  else
	    new_lc->set_src = NULL;
	  new_lc->loc = node->loc;

	  *nextp = new_lc;
	  nextp = &new_lc->next;
	}

      /* We are at the basic block boundary when copying variable description
	 so set the CUR_LOC to be the first element of the chain.  */
      if (new_var->var_part[i].loc_chain)
	new_var->var_part[i].cur_loc = new_var->var_part[i].loc_chain->loc;
      else
	new_var->var_part[i].cur_loc = NULL;
    }

  *slot = new_var;
  return new_var;
}

/* Add a variable from *SLOT to hash table DATA and increase its reference
   count.  */

static int
vars_copy_1 (void **slot, void *data)
{
  htab_t dst = (htab_t) data;
  variable src;
  void **dstp;

  src = (variable) *slot;
  src->refcount++;

  dstp = htab_find_slot_with_hash (dst, &src->dv,
				   dv_htab_hash (src->dv),
				   INSERT);
  *dstp = src;

  /* Continue traversing the hash table.  */
  return 1;
}

/* Copy all variables from hash table SRC to hash table DST.  */

static void
vars_copy (htab_t dst, htab_t src)
{
  vars_clear (dst);
  htab_traverse (src, vars_copy_1, dst);
}

/* Map a decl to its main debug decl.  */

static inline tree
var_debug_decl (tree decl)
{
  if (decl && DECL_P (decl)
      && DECL_DEBUG_EXPR_IS_FROM (decl) && DECL_DEBUG_EXPR (decl)
      && DECL_P (DECL_DEBUG_EXPR (decl)))
    decl = DECL_DEBUG_EXPR (decl);

  return decl;
}

/* Set the register LOC to contain DECL, OFFSET.  */

static void
var_reg_decl_set (dataflow_set *set, rtx loc, enum var_init_status initialized,
		  decl_or_value dv, HOST_WIDE_INT offset, rtx set_src)
{
  attrs node;
  bool decl_p = dv_is_decl_p (dv);

  if (decl_p)
    dv = dv_from_decl (var_debug_decl (dv_as_decl (dv)));

  for (node = set->regs[REGNO (loc)]; node; node = node->next)
    if (dv_as_opaque (node->dv) == dv_as_opaque (dv)
	&& node->offset == offset)
      break;
  if (!node)
    attrs_list_insert (&set->regs[REGNO (loc)], dv, offset, loc);
  set_variable_part (set, loc, dv, offset, initialized, set_src);
}

/* Set the register to contain REG_EXPR (LOC), REG_OFFSET (LOC).  */

static void
var_reg_set (dataflow_set *set, rtx loc, enum var_init_status initialized,
	     rtx set_src)
{
  tree decl = REG_EXPR (loc);
  HOST_WIDE_INT offset = REG_OFFSET (loc);

  var_reg_decl_set (set, loc, initialized,
		    dv_from_decl (decl), offset, set_src);
}

static int
get_init_value (dataflow_set *set, rtx loc, decl_or_value dv)
{
  void **slot;
  variable var;
  int i;
  int ret_val = VAR_INIT_STATUS_UNKNOWN;

  if (! flag_var_tracking_uninit)
    return VAR_INIT_STATUS_INITIALIZED;

  slot = htab_find_slot_with_hash (set->vars, &dv,
				   dv_htab_hash (dv), NO_INSERT);
  if (slot)
    {
      var = (variable) *slot;
      for (i = 0; i < var->n_var_parts && ret_val == VAR_INIT_STATUS_UNKNOWN; i++)
	{
	  location_chain nextp;
	  for (nextp = var->var_part[i].loc_chain; nextp; nextp = nextp->next)
	    if (rtx_equal_p (nextp->loc, loc))
	      {
		ret_val = nextp->init;
		break;
	      }
	}
    }

  return ret_val;
}

/* Delete current content of register LOC in dataflow set SET and set
   the register to contain REG_EXPR (LOC), REG_OFFSET (LOC).  If
   MODIFY is true, any other live copies of the same variable part are
   also deleted from the dataflow set, otherwise the variable part is
   assumed to be copied from another location holding the same
   part.  */

static void
var_reg_delete_and_set (dataflow_set *set, rtx loc, bool modify, 
			enum var_init_status initialized, rtx set_src)
{
  tree decl = REG_EXPR (loc);
  HOST_WIDE_INT offset = REG_OFFSET (loc);
  attrs node, next;
  attrs *nextp;

  decl = var_debug_decl (decl);

  if (initialized == VAR_INIT_STATUS_UNKNOWN)
    initialized = get_init_value (set, loc, dv_from_decl (decl));

  nextp = &set->regs[REGNO (loc)];
  for (node = *nextp; node; node = next)
    {
      next = node->next;
      if (dv_as_opaque (node->dv) != decl || node->offset != offset)
	{
	  delete_variable_part (set, node->loc, node->dv, node->offset);
	  pool_free (attrs_pool, node);
	  *nextp = next;
	}
      else
	{
	  node->loc = loc;
	  nextp = &node->next;
	}
    }
  if (modify)
    clobber_variable_part (set, loc, dv_from_decl (decl), offset, set_src);
  var_reg_set (set, loc, initialized, set_src);
}

/* Delete current content of register LOC in dataflow set SET.  If
   CLOBBER is true, also delete any other live copies of the same
   variable part.  */

static void
var_reg_delete (dataflow_set *set, rtx loc, bool clobber)
{
  attrs *reg = &set->regs[REGNO (loc)];
  attrs node, next;

  if (clobber)
    {
      tree decl = REG_EXPR (loc);
      HOST_WIDE_INT offset = REG_OFFSET (loc);

      decl = var_debug_decl (decl);

      clobber_variable_part (set, NULL, dv_from_decl (decl), offset, NULL);
    }

  for (node = *reg; node; node = next)
    {
      next = node->next;
      delete_variable_part (set, node->loc, node->dv, node->offset);
      pool_free (attrs_pool, node);
    }
  *reg = NULL;
}

/* Delete content of register with number REGNO in dataflow set SET.  */

static void
var_regno_delete (dataflow_set *set, int regno)
{
  attrs *reg = &set->regs[regno];
  attrs node, next;

  for (node = *reg; node; node = next)
    {
      next = node->next;
      delete_variable_part (set, node->loc, node->dv, node->offset);
      pool_free (attrs_pool, node);
    }
  *reg = NULL;
}

/* Set the location of DECL, OFFSET as the MEM LOC.  */

static void
var_mem_decl_set (dataflow_set *set, rtx loc, enum var_init_status initialized,
		  decl_or_value dv, HOST_WIDE_INT offset, rtx set_src)
{
  if (dv_is_decl_p (dv))
    dv = dv_from_decl (var_debug_decl (dv_as_decl (dv)));

  set_variable_part (set, loc, dv, offset,
		     initialized, set_src);
}

/* Set the location part of variable MEM_EXPR (LOC) in dataflow set
   SET to LOC.
   Adjust the address first if it is stack pointer based.  */

static void
var_mem_set (dataflow_set *set, rtx loc, enum var_init_status initialized, 
	     rtx set_src)
{
  tree decl = MEM_EXPR (loc);
  HOST_WIDE_INT offset = INT_MEM_OFFSET (loc);

  var_mem_decl_set (set, loc, initialized,
		    dv_from_decl (decl), offset, set_src);
}

/* Delete and set the location part of variable MEM_EXPR (LOC) in
   dataflow set SET to LOC.  If MODIFY is true, any other live copies
   of the same variable part are also deleted from the dataflow set,
   otherwise the variable part is assumed to be copied from another
   location holding the same part.
   Adjust the address first if it is stack pointer based.  */

static void
var_mem_delete_and_set (dataflow_set *set, rtx loc, bool modify, 
			enum var_init_status initialized, rtx set_src)
{
  tree decl = MEM_EXPR (loc);
  HOST_WIDE_INT offset = INT_MEM_OFFSET (loc);

  decl = var_debug_decl (decl);

  if (initialized == VAR_INIT_STATUS_UNKNOWN)
    initialized = get_init_value (set, loc, dv_from_decl (decl));

  if (modify)
    clobber_variable_part (set, NULL, dv_from_decl (decl), offset, set_src);
  var_mem_set (set, loc, initialized, set_src);
}

/* Delete the location part LOC from dataflow set SET.  If CLOBBER is
   true, also delete any other live copies of the same variable part.
   Adjust the address first if it is stack pointer based.  */

static void
var_mem_delete (dataflow_set *set, rtx loc, bool clobber)
{
  tree decl = MEM_EXPR (loc);
  HOST_WIDE_INT offset = INT_MEM_OFFSET (loc);

  decl = var_debug_decl (decl);
  if (clobber)
    clobber_variable_part (set, NULL, dv_from_decl (decl), offset, NULL);
  delete_variable_part (set, loc, dv_from_decl (decl), offset);
}

/* Map a value to a location it was just stored in.  */

static void
val_store (dataflow_set *set, rtx val, rtx loc, rtx insn)
{
  cselib_val *v = CSELIB_VAL_PTR (val);

  gcc_assert (cselib_preserved_value_p (v));

  if (dump_file)
    {
      fprintf (dump_file, "%i: ", INSN_UID (insn));
      print_inline_rtx (dump_file, val, 0);
      fprintf (dump_file, " stored in ");
      print_inline_rtx (dump_file, loc, 0);
      if (v->locs)
	{
	  struct elt_loc_list *l;
	  for (l = v->locs; l; l = l->next)
	    {
	      fprintf (dump_file, "\n%i: ", INSN_UID (l->setting_insn));
	      print_inline_rtx (dump_file, l->loc, 0);
	    }
	}
      fprintf (dump_file, "\n");
    }

  if (REG_P (loc))
    {
      var_regno_delete (set, REGNO (loc));
      var_reg_decl_set (set, loc, VAR_INIT_STATUS_INITIALIZED,
			dv_from_value (val), 0, NULL_RTX);
    }
  else if (MEM_P (loc))
    var_mem_decl_set (set, loc, VAR_INIT_STATUS_INITIALIZED,
		      dv_from_value (val), 0, NULL_RTX);
  else
    set_variable_part (set, loc, dv_from_value (val), 0,
		       VAR_INIT_STATUS_INITIALIZED, NULL_RTX);
}

/* Find the values in a given location and map the val to another
   value, if it is unique, or add the location as one holding the
   value.  */

static void
val_resolve (dataflow_set *set, rtx val, rtx loc, rtx insn)
{
  decl_or_value dv = dv_from_value (val);
  void **slot;

  if (dump_file)
    {
      fprintf (dump_file, "%i: ", INSN_UID (insn));
      print_inline_rtx (dump_file, val, 0);
      fputs (" is at ", dump_file);
      print_inline_rtx (dump_file, loc, 0);
      fputc ('\n', dump_file);
    }

  /* Reset this node, detaching all its equivalences.  */
  slot = htab_find_slot_with_hash (set->vars, &dv, dv_htab_hash (dv),
				   NO_INSERT);
  if (slot && ((variable)*slot)->n_var_parts)
    {
      variable var = (variable)*slot, canon;
      location_chain node;
      rtx cval;

      gcc_assert (var->n_var_parts == 1);

      canon = find_canonical_value (var, set, var);
      if (canon)
	cval = dv_as_value (canon->dv);
      else
	cval = NULL;

      for (node = var->var_part[0].loc_chain; node; node = node->next)
	if (GET_CODE (node->loc) == VALUE)
	  {
	    /* Redirect the equivalence link to the new canonical
	       value, or simply remove it if it would point at
	       itself.  */
	    if (cval && cval != node->loc)
	      set_variable_part (set, cval, dv_from_value (node->loc),
				 0, node->init, node->set_src);
	    delete_variable_part (set, val, dv_from_value (node->loc), 0);
	  }

      if (cval)
	{
	  decl_or_value cdv = dv_from_value (cval);

	  /* Keep the remaining values connected, accummulating links
	     in the canonical value.  */
	  for (node = var->var_part[0].loc_chain; node; node = node->next)
	    {
	      if (node->loc == cval)
		continue;
	      else if (GET_CODE (node->loc) == REG)
		var_reg_decl_set (set, node->loc, node->init, cdv, 0,
				  node->set_src);
	      else if (GET_CODE (node->loc) == MEM)
		var_mem_decl_set (set, node->loc, node->init, cdv, 0,
				  node->set_src);
	      else
		set_variable_part (set, node->loc, cdv, 0,
				   node->init, node->set_src);
	    }
	}

      clobber_variable_part (set, NULL, dv_from_value (val), 0, NULL);
    }


  if (REG_P (loc))
    {
      attrs node, found = NULL;

      for (node = set->regs[REGNO (loc)]; node; node = node->next)
	if (dv_is_value_p (node->dv)
	    && GET_MODE (dv_as_value (node->dv)) == GET_MODE (loc))
	  {
	    found = node;

	    /* Map incoming equivalences.  ??? Wouldn't it be nice if
	     we just started sharing the location lists?  Maybe a
	     circular list ending at the value itself or some
	     such.  */
	    set_variable_part (set, dv_as_value (node->dv),
			       dv_from_value (val), node->offset,
			       VAR_INIT_STATUS_INITIALIZED, NULL_RTX);
	    set_variable_part (set, val, node->dv, node->offset,
			       VAR_INIT_STATUS_INITIALIZED, NULL_RTX);
	  }

      /* If we didn't find any equivalence, we need to remember that
	 this value is held in the named register.  */
      if (!found)
	var_reg_decl_set (set, loc, VAR_INIT_STATUS_INITIALIZED,
			  dv_from_value (val), 0, NULL_RTX);
    }
  else if (MEM_P (loc))
    /* ??? Merge equivalent MEMs.  */
    var_mem_decl_set (set, loc, VAR_INIT_STATUS_INITIALIZED,
		      dv_from_value (val), 0, NULL_RTX);
  else
    /* ??? Merge equivalent expressions.  */
    set_variable_part (set, loc, dv_from_value (val), 0,
		       VAR_INIT_STATUS_INITIALIZED, NULL_RTX);
}

/* Initialize dataflow set SET to be empty. 
   VARS_SIZE is the initial size of hash table VARS.  */

static void
dataflow_set_init (dataflow_set *set, int vars_size)
{
  init_attrs_list_set (set->regs);
  set->vars = htab_create (vars_size, variable_htab_hash, variable_htab_eq,
			   variable_htab_free);
  set->stack_adjust = 0;
}

/* Delete the contents of dataflow set SET.  */

static void
dataflow_set_clear (dataflow_set *set)
{
  int i;

  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    attrs_list_clear (&set->regs[i]);

  vars_clear (set->vars);
}

/* Copy the contents of dataflow set SRC to DST.  */

static void
dataflow_set_copy (dataflow_set *dst, dataflow_set *src)
{
  int i;

  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    attrs_list_copy (&dst->regs[i], src->regs[i]);

  vars_copy (dst->vars, src->vars);
  dst->stack_adjust = src->stack_adjust;
}

/* Information for merging lists of locations for a given offset of variable.
 */
struct variable_union_info
{
  /* Node of the location chain.  */
  location_chain lc;

  /* The sum of positions in the input chains.  */
  int pos;

  /* The position in the chains of SRC and DST dataflow sets.  */
  int pos_src;
  int pos_dst;
};

/* Compare function for qsort, order the structures by POS element.  */

static int
variable_union_info_cmp_pos (const void *n1, const void *n2)
{
  const struct variable_union_info *i1 = n1;
  const struct variable_union_info *i2 = n2;

  if (i1->pos != i2->pos)
    return i1->pos - i2->pos;
  
  return (i1->pos_dst - i2->pos_dst);
}

/* Compute union of location parts of variable *SLOT and the same variable
   from hash table DATA.  Compute "sorted" union of the location chains
   for common offsets, i.e. the locations of a variable part are sorted by
   a priority where the priority is the sum of the positions in the 2 chains
   (if a location is only in one list the position in the second list is
   defined to be larger than the length of the chains).
   When we are updating the location parts the newest location is in the
   beginning of the chain, so when we do the described "sorted" union
   we keep the newest locations in the beginning.  */

static int
variable_union (void **slot, void *data)
{
  variable src, dst;
  void **dstp;
  dataflow_set *set = (dataflow_set *) data;
  int i, j, k;

  src = (variable) *slot;
  dstp = htab_find_slot_with_hash (set->vars, &src->dv,
				   dv_htab_hash (src->dv),
				   INSERT);
  if (!*dstp)
    {
      src->refcount++;

      /* If CUR_LOC of some variable part is not the first element of
	 the location chain we are going to change it so we have to make
	 a copy of the variable.  */
      for (k = 0; k < src->n_var_parts; k++)
	{
	  gcc_assert (!src->var_part[k].loc_chain
		      == !src->var_part[k].cur_loc);
	  if (src->var_part[k].loc_chain)
	    {
	      gcc_assert (src->var_part[k].cur_loc);
	      if (src->var_part[k].cur_loc != src->var_part[k].loc_chain->loc)
		break;
	    }
	}
      if (k < src->n_var_parts)
	{
	  enum var_init_status status = VAR_INIT_STATUS_UNKNOWN;
	  
	  if (! flag_var_tracking_uninit)
	    status = VAR_INIT_STATUS_INITIALIZED;

	  unshare_variable (dstp, src, status);
	}
      else
	*dstp = src;

      /* Continue traversing the hash table.  */
      return 1;
    }
  else
    dst = *dstp;

  gcc_assert (src->n_var_parts);

  /* Count the number of location parts, result is K.  */
  for (i = 0, j = 0, k = 0;
       i < src->n_var_parts && j < dst->n_var_parts; k++)
    {
      if (src->var_part[i].offset == dst->var_part[j].offset)
	{
	  i++;
	  j++;
	}
      else if (src->var_part[i].offset < dst->var_part[j].offset)
	i++;
      else
	j++;
    }
  k += src->n_var_parts - i;
  k += dst->n_var_parts - j;

  /* We track only variables whose size is <= MAX_VAR_PARTS bytes
     thus there are at most MAX_VAR_PARTS different offsets.  */
  gcc_assert (dv_onepart_p (dst->dv) ? k == 1 : k <= MAX_VAR_PARTS);

  if (dst->refcount > 1 && dst->n_var_parts != k)
    {
      enum var_init_status status = VAR_INIT_STATUS_UNKNOWN;
      
      if (! flag_var_tracking_uninit)
	status = VAR_INIT_STATUS_INITIALIZED;
      dst = unshare_variable (dstp, dst, status);
    }

  i = src->n_var_parts - 1;
  j = dst->n_var_parts - 1;
  dst->n_var_parts = k;

  for (k--; k >= 0; k--)
    {
      location_chain node, node2;

      if (i >= 0 && j >= 0
	  && src->var_part[i].offset == dst->var_part[j].offset)
	{
	  /* Compute the "sorted" union of the chains, i.e. the locations which
	     are in both chains go first, they are sorted by the sum of
	     positions in the chains.  */
	  int dst_l, src_l;
	  int ii, jj, n;
	  struct variable_union_info *vui;

	  /* If DST is shared compare the location chains.
	     If they are different we will modify the chain in DST with
	     high probability so make a copy of DST.  */
	  if (dst->refcount > 1)
	    {
	      for (node = src->var_part[i].loc_chain,
		   node2 = dst->var_part[j].loc_chain; node && node2;
		   node = node->next, node2 = node2->next)
		{
		  if (!((REG_P (node2->loc)
			 && REG_P (node->loc)
			 && REGNO (node2->loc) == REGNO (node->loc))
			|| rtx_equal_p (node2->loc, node->loc)))
		    {
		      if (node2->init < node->init)
		        node2->init = node->init;
		      break;
		    }
		}
	      if (node || node2)
		dst = unshare_variable (dstp, dst, VAR_INIT_STATUS_UNKNOWN);
	    }

	  src_l = 0;
	  for (node = src->var_part[i].loc_chain; node; node = node->next)
	    src_l++;
	  dst_l = 0;
	  for (node = dst->var_part[j].loc_chain; node; node = node->next)
	    dst_l++;
	  vui = XCNEWVEC (struct variable_union_info, src_l + dst_l);

	  /* Fill in the locations from DST.  */
	  for (node = dst->var_part[j].loc_chain, jj = 0; node;
	       node = node->next, jj++)
	    {
	      vui[jj].lc = node;
	      vui[jj].pos_dst = jj;

	      /* Value larger than a sum of 2 valid positions.  */
	      vui[jj].pos_src = src_l + dst_l;
	    }

	  /* Fill in the locations from SRC.  */
	  n = dst_l;
	  for (node = src->var_part[i].loc_chain, ii = 0; node;
	       node = node->next, ii++)
	    {
	      /* Find location from NODE.  */
	      for (jj = 0; jj < dst_l; jj++)
		{
		  if ((REG_P (vui[jj].lc->loc)
		       && REG_P (node->loc)
		       && REGNO (vui[jj].lc->loc) == REGNO (node->loc))
		      || rtx_equal_p (vui[jj].lc->loc, node->loc))
		    {
		      vui[jj].pos_src = ii;
		      break;
		    }
		}
	      if (jj >= dst_l)	/* The location has not been found.  */
		{
		  location_chain new_node;

		  /* Copy the location from SRC.  */
		  new_node = pool_alloc (loc_chain_pool);
		  new_node->loc = node->loc;
		  new_node->init = node->init;
		  if (!node->set_src || MEM_P (node->set_src))
		    new_node->set_src = NULL;
		  else
		    new_node->set_src = node->set_src;
		  vui[n].lc = new_node;
		  vui[n].pos_src = ii;
		  vui[n].pos_dst = src_l + dst_l;
		  n++;
		}
	    }

	  for (ii = 0; ii < src_l + dst_l; ii++)
	    vui[ii].pos = vui[ii].pos_src + vui[ii].pos_dst;

	  qsort (vui, n, sizeof (struct variable_union_info),
		 variable_union_info_cmp_pos);

	  /* Reconnect the nodes in sorted order.  */
	  for (ii = 1; ii < n; ii++)
	    vui[ii - 1].lc->next = vui[ii].lc;
	  vui[n - 1].lc->next = NULL;

	  dst->var_part[k].loc_chain = vui[0].lc;
	  dst->var_part[k].offset = dst->var_part[j].offset;

	  free (vui);
	  i--;
	  j--;
	}
      else if ((i >= 0 && j >= 0
		&& src->var_part[i].offset < dst->var_part[j].offset)
	       || i < 0)
	{
	  dst->var_part[k] = dst->var_part[j];
	  j--;
	}
      else if ((i >= 0 && j >= 0
		&& src->var_part[i].offset > dst->var_part[j].offset)
	       || j < 0)
	{
	  location_chain *nextp;

	  /* Copy the chain from SRC.  */
	  nextp = &dst->var_part[k].loc_chain;
	  for (node = src->var_part[i].loc_chain; node; node = node->next)
	    {
	      location_chain new_lc;

	      new_lc = pool_alloc (loc_chain_pool);
	      new_lc->next = NULL;
	      new_lc->init = node->init;
	      if (!node->set_src || MEM_P (node->set_src))
		new_lc->set_src = NULL;
	      else
		new_lc->set_src = node->set_src;
	      new_lc->loc = node->loc;

	      *nextp = new_lc;
	      nextp = &new_lc->next;
	    }

	  dst->var_part[k].offset = src->var_part[i].offset;
	  i--;
	}

      /* We are at the basic block boundary when computing union
	 so set the CUR_LOC to be the first element of the chain.  */
      if (dst->var_part[k].loc_chain)
	dst->var_part[k].cur_loc = dst->var_part[k].loc_chain->loc;
      else
	dst->var_part[k].cur_loc = NULL;
    }

  for (i = 0; i < src->n_var_parts && i < dst->n_var_parts; i++)
    {
      location_chain node, node2;
      for (node = src->var_part[i].loc_chain; node; node = node->next)
	for (node2 = dst->var_part[i].loc_chain; node2; node2 = node2->next)
	  if (rtx_equal_p (node->loc, node2->loc))
	    {
	      if (node->init > node2->init)
		node2->init = node->init;
	    }
    }

  /* Continue traversing the hash table.  */
  return 1;
}

/* Compute union of dataflow sets SRC and DST and store it to DST.  */

static void
dataflow_set_union (dataflow_set *dst, dataflow_set *src)
{
  int i;

  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    attrs_list_union (&dst->regs[i], src->regs[i]);

  htab_traverse (src->vars, variable_union, dst);
}

/* Whether the value is currently being expanded.  */
#define VALUE_RECURSED_INTO(x) \
  (RTL_FLAG_CHECK1 ("VALUE_RECURSED_INTO", (x), VALUE)->used)

/* Return a location list node whose loc is rtx_equal to LOC, in the
   location list of a one-part variable or value VAR, or in that of
   any values recursively mentioned in the location lists.  */

static location_chain
find_loc_in_1pdv (rtx loc, variable var, htab_t vars)
{
  location_chain node;

  if (!var)
    return NULL;

  gcc_assert (dv_onepart_p (var->dv));

  if (!var->n_var_parts)
    return NULL;

  gcc_assert (var->var_part[0].offset == 0);

  for (node = var->var_part[0].loc_chain; node; node = node->next)
    if (rtx_equal_p (loc, node->loc))
      return node;
    else if (GET_CODE (node->loc) == VALUE
	     && !VALUE_RECURSED_INTO (node->loc))
      {
	decl_or_value dv = dv_from_value (node->loc);
	void **slot = htab_find_slot_with_hash (vars, &dv, dv_htab_hash (dv),
						NO_INSERT);

	if (slot)
	  {
	    location_chain where;
	    VALUE_RECURSED_INTO (node->loc) = true;
	    if ((where = find_loc_in_1pdv (loc, (variable)*slot, vars)))
	      {
		VALUE_RECURSED_INTO (node->loc) = false;
		return where;
	      }
	    VALUE_RECURSED_INTO (node->loc) = false;
	  }
      }

  return NULL;
}

/* Hash table iteration argument passed to variable_merge.  */
struct dfset_merge
{
  /* The set in which the merge is to be inserted.  */
  dataflow_set *dst;
  /* The set that we're iterating in.  */
  dataflow_set *cur;
  /* The set that may contain the other dv we are to merge with.  */
  dataflow_set *src;
};

/* Insert LOC in *DNODE, if it's not there yet.  */

static void
insert_into_intersection (location_chain *dnode, rtx loc,
			  enum var_init_status status)
{
  location_chain node;

  for (node = *dnode; node; node = node->next)
    if (rtx_equal_p (node->loc, loc))
      {
	node->init = MIN (node->init, status);
	return;
      }

  node = pool_alloc (loc_chain_pool);

  node->loc = loc;
  node->set_src = NULL;
  node->init = status;
  node->next = *dnode;
  *dnode = node;
}

/* Insert in DVAR the intersection the locations present in both
   S1NODE and S2VAR, directly or indirectly.  S1NODE is from a
   variable in DSM->cur, whereas S2VAR is from DSM->src.  dvar is in
   DSM->dst.  */

static void
intersect_loc_chains (variable dvar, struct dfset_merge *dsm,
		      location_chain s1node, variable s2var)
{
  dataflow_set *s1set = dsm->cur;
  dataflow_set *s2set = dsm->src;
  location_chain found;

  for (; s1node; s1node = s1node->next)
    {
      if ((found = find_loc_in_1pdv (s1node->loc, s2var, s2set->vars)))
	{
	  insert_into_intersection (&dvar->var_part[0].loc_chain, s1node->loc,
				    MIN (s1node->init, found->init));
	  continue;
	}

      if (GET_CODE (s1node->loc) == VALUE
	  && !VALUE_RECURSED_INTO (s1node->loc))
	{
	  decl_or_value dv = dv_from_value (s1node->loc);
	  void **slot = htab_find_slot_with_hash (s1set->vars,
						  &dv, dv_htab_hash (dv),
						  NO_INSERT);

	  if (slot)
	    {
	      variable val = (variable)*slot;

	      if (val->n_var_parts == 1)
		{
		  VALUE_RECURSED_INTO (s1node->loc) = true;
		  intersect_loc_chains (dvar, dsm,
					val->var_part[0].loc_chain,
					s2var);
		  VALUE_RECURSED_INTO (s1node->loc) = false;
		}
	    }
	}

      /* ??? if the location is equivalent to any location in src,
	 searched recursively

	   add to dst the values needed to represent the equivalence

     telling whether locations S is equivalent to another dv's
     location list:

       for each location D in the list

         if S and D satisfy rtx_equal_p, then it is present

	 else if D is a value, recurse without cycles

	 else if S and D have the same CODE and MODE

	   for each operand oS and the corresponding oD

	     if oS and oD are not equivalent, then S an D are not equivalent

	     else if they are RTX vectors

	       if any vector oS element is not equivalent to its respective oD,
	       then S and D are not equivalent

   */


    }
}

/* Determine whether we're in the first pass of dataflow merging, that
   merges with missing values in incoming edges as union, or the
   second pass, that merges them as intersection.  */

static bool merge_with_missing_1pdv_as_union = false;

/* Combine variable or value in *S1SLOT (in DSM->cur) with the
   corresponding entry in DSM->src.  Multi-part variables are combined
   with variable_union, whereas onepart dvs are combined with
   intersection.  */

static int
variable_merge (void **s1slot, void *data)
{
  struct dfset_merge *dsm = (struct dfset_merge *)data;
  void **s2slot, **dstslot;
  variable s1var = (variable) *s1slot;
  variable s2var, dvar;
  decl_or_value dv = s1var->dv;
  bool onepart = dv_onepart_p (dv);
  hashval_t dvhash;
  location_chain node, node1, node2;
  int count1, count2;

  if (!onepart
      /* If the incoming onepart variable has an empty location list, then
	 the intersection will be just as empty.  */
      || !s1var->n_var_parts || !s1var->var_part[0].loc_chain)
    return variable_union (s1slot, dsm->dst);

  dvhash = dv_htab_hash (dv);

  s2slot = htab_find_slot_with_hash (dsm->src->vars, &dv, dvhash, NO_INSERT);
  if (!s2slot)
    {
      if (!onepart)
	return 1;

      if (merge_with_missing_1pdv_as_union)
	return variable_union (s1slot, dsm->dst);
      else
	s2var = NULL;
    }
  else
    {
      s2var = (variable) *s2slot;

      if (!onepart
	  /* If the onepart variable has an empty location list, then
	     the intersection will be just as empty.  */
	  || !s2var->n_var_parts || !s2var->var_part[0].loc_chain)
	return variable_union (s2slot, dsm->dst);
    }

  gcc_assert (onepart);
  gcc_assert (s1var->n_var_parts == 1);
  gcc_assert (s1var->var_part[0].offset == 0);
  gcc_assert (s2var->n_var_parts == 1);
  gcc_assert (s2var->var_part[0].offset == 0);

  count1 = 0;
  for (node = s1var->var_part[0].loc_chain; node; node = node->next)
    if (!find_loc_in_1pdv (node->loc, s2var, dsm->src->vars))
      break;
    else
      count1++;
  /* If we got to the end of the loop, we found out every one of the
     s1var locations was literally present in s2var locations,
     therefore the intersection is s1var.  */
  if (!node)
    return variable_union (s1slot, dsm->dst);
  node1 = node;

  if (s2var)
    {
      count2 = 0;
      for (node = s2var->var_part[0].loc_chain; node; node = node->next)
	if (!find_loc_in_1pdv (node->loc, s1var, dsm->cur->vars))
	  break;
	else
	  count2++;
      /* Likewise, if we got to the end of the loop, we found out every
	 one of the s2var locations was literally present in s1var's
	 locations, therefore the intersection is s2var.  */
      if (!node)
	return variable_union (s2slot, dsm->dst);
      node2 = node;
    }

  dstslot = htab_find_slot_with_hash (dsm->dst->vars, &dv, dvhash, INSERT);
  gcc_assert (!*dstslot);

  dvar = pool_alloc (dv_pool (dv));
  dvar->dv = dv;
  dvar->refcount = 1;
  dvar->n_var_parts = 1;
  dvar->var_part[0].offset = 0;
  dvar->var_part[0].cur_loc = NULL;
  dvar->var_part[0].loc_chain = NULL;

  *dstslot = dvar;

  intersect_loc_chains (dvar, dsm, s1var->var_part[0].loc_chain, s2var);

  return 1;
}

/* Add location information for variable or value in *SLOT (from
   DSM->SRC) to DATA->DST, unless we can tell it was already
   combined in variable_merge.  */

static int
variable_merge2 (void **slot, void *data)
{
  struct dfset_merge *dsm = (struct dfset_merge *)data;
  void **dstslot;
  variable var = (variable) *slot;
  variable dvar;
  decl_or_value dv = var->dv;
  bool onepart = dv_onepart_p (dv);
  hashval_t dvhash;

  if (!onepart)
    return variable_union (slot, dsm->dst);

  dvhash = dv_htab_hash (dv);

  dstslot = htab_find_slot_with_hash (dsm->cur->vars, &dv, dvhash, NO_INSERT);

  if (dstslot)
    /* We're done, this was taken care of in variable_merge().  */
    return 1;

  if (merge_with_missing_1pdv_as_union)
    return variable_union (slot, dsm->dst);

  dvar = pool_alloc (dv_pool (dv));
  dvar->dv = dv;
  dvar->refcount = 1;
  dvar->n_var_parts = 1;
  dvar->var_part[0].offset = 0;
  dvar->var_part[0].cur_loc = NULL;
  dvar->var_part[0].loc_chain = NULL;

  dstslot = htab_find_slot_with_hash (dsm->dst->vars, &dv, dvhash, INSERT);
  gcc_assert (!*dstslot);
  *dstslot = dvar;

  return 1;
}

/* Determine a total order between two distinct pointers.  Compare the
   pointers as integral types if size_t is wide enough, otherwise
   resort to bitwise memory compare.  The actual order does not
   matter, we just need to be consistent, so endianness is
   irrelevant.  */

static int
tie_break_pointers (const void *p1, const void *p2)
{
  gcc_assert (p1 != p2);

  if (sizeof (size_t) >= sizeof (void*))
    return (size_t)p1 < (size_t)p2 ? -1 : 1;
  else
    return memcmp (&p1, &p2, sizeof (p1));
}

/* Pick one out of a set of equivalent VALUEs to be used as the
   canonical value among them.  We choose the lowest-numbered VALUE
   reachable from VAR, except for OLD, using the RTX address as a
   tie-breaker.  The idea is to arrange them into a star topology,
   such that all of them are at most one step away from the canonical
   value, and the canonical value has backlinks to all of them, in
   addition to all the actual locations.  We don't enforce this
   topology throughout the entire dataflow analysis, though.  */

static variable
find_canonical_value (variable var, dataflow_set *set, variable old)
{
  location_chain node;
  variable canon = var;
  rtx cval;

  if (canon == old)
    canon = NULL;

  if (!var->n_var_parts)
    return canon;

  if (canon && dv_is_value_p (canon->dv))
    cval = dv_as_value (canon->dv);
  else
    cval = NULL;

  gcc_assert (var->n_var_parts == 1);

  for (node = var->var_part[0].loc_chain; node; node = node->next)
    if (GET_CODE (node->loc) == VALUE
	&& !VALUE_RECURSED_INTO (node->loc))
      {
	void **slot;
	variable tcanon;
	decl_or_value tdv = dv_from_value (node->loc);
	rtx tval;

	slot = htab_find_slot_with_hash (set->vars, &tdv,
					 dv_htab_hash (tdv), NO_INSERT);
	if (!slot)
	  continue;

	tcanon = (variable)*slot;
	VALUE_RECURSED_INTO (node->loc) = true;
	tcanon = find_canonical_value (tcanon, set, old);
	VALUE_RECURSED_INTO (node->loc) = false;

	if (!tcanon || tcanon == canon || tcanon == var)
	  continue;

	tval = dv_as_value (tcanon->dv);

	if (!cval
	    || CSELIB_VAL_PTR (tval)->value < CSELIB_VAL_PTR (cval)->value
	    || (CSELIB_VAL_PTR (tval)->value == CSELIB_VAL_PTR (cval)->value
		&& tie_break_pointers (tval, cval) < 0))
	  {
	    canon = tcanon;
	    cval = tval;
	  }
      }

  gcc_assert (canon != old || !canon);

  return canon;
}

/* Remove redundant entries from equivalence lists in onepart
   variables, canonicalizing equivalence sets into star shapes.  */

static int
variable_merge3 (void **slot, void *data)
{
  struct dfset_merge *dsm = (struct dfset_merge *)data;
  dataflow_set *set = dsm->dst;
  variable var = (variable) *slot;
  decl_or_value dv = var->dv;
  location_chain node;
  variable canon;
  decl_or_value cdv;
  rtx cval;
  void **cslot;
  bool has_canon = false;

  if (!dv_onepart_p (dv) || !var->n_var_parts)
    return 1;

  gcc_assert (var->n_var_parts == 1);

  canon = find_canonical_value (var, set, NULL);

  if (canon == var)
    {
      if (dv_is_decl_p (dv))
	return 1;

      cval = dv_as_value (dv);

      for (node = var->var_part[0].loc_chain; node; node = node->next)
	if (GET_CODE (node->loc) == VALUE)
	  {
	    decl_or_value ndv = dv_from_value (node->loc);
	    void **vslot = htab_find_slot_with_hash (set->vars, &ndv,
						     dv_htab_hash (ndv),
						     NO_INSERT);

	    if (!vslot)
	      return 1;

	    set_slot_part (set, cval, vslot, ndv, 0,
			   VAR_INIT_STATUS_INITIALIZED, NULL_RTX);
	  }

      return 1;
    }

  cdv = canon->dv;
  cval = dv_as_value (canon->dv);
  cslot = htab_find_slot_with_hash (set->vars, &cdv, dv_htab_hash (cdv),
				    NO_INSERT);

  /* It may be unshared.  */
  canon = NULL;

  for (node = var->var_part[0].loc_chain; node; node = node->next)
    if (node->loc != cval)
      set_slot_part (set, node->loc, cslot, cdv, 0,
		     node->init, NULL_RTX);
    else
      has_canon = true;

  if (dv_is_value_p (dv))
    set_slot_part (set, dv_as_value (dv), cslot, cdv, 0,
		   VAR_INIT_STATUS_INITIALIZED, NULL_RTX);

  if (!has_canon)
    set_slot_part (set, cval, slot, dv, 0, VAR_INIT_STATUS_INITIALIZED, NULL);

  /* ??? Introducing clobber_slot_part might speed things up a bit,
     but it's not as needed as set_slot_part, because it never looks
     up the hash table with INSERT, which may cause resizes even
     without actual inserts.  */
  clobber_variable_part (set, cval, dv, 0, NULL);

  var = (variable)*slot;
  gcc_assert (var->n_var_parts && var->var_part[0].loc_chain->loc == cval
	      && var->var_part[0].loc_chain->next == NULL);

  return 1;
}

/* Combine dataflow set information from SRC into DST, using PDST
   to carry over information across passes.  */

static void
dataflow_set_merge (dataflow_set *dst, dataflow_set *src)
{
  dataflow_set src2 = *dst;
  struct dfset_merge dsm;
  int i;

  dataflow_set_init (dst, MAX (htab_elements (src->vars),
			       htab_elements (src2.vars)));

  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    attrs_list_mpdv_union (&dst->regs[i], src->regs[i], src2.regs[i]);

  dsm.dst = dst;
  dsm.src = &src2;
  dsm.cur = src;

  htab_traverse (src2.vars, variable_merge, &dsm);

  htab_traverse (src->vars, variable_merge2, &dsm);

  htab_traverse (dst->vars, variable_merge3, &dsm);

  dataflow_set_destroy (&src2);
}

/* Hash table iteration argument passed to variable_post_merge.  */
struct dfset_post_merge
{
  /* The new input set for the current block.  */
  dataflow_set *set;
  /* The old input set for the current block.  */
  dataflow_set *old;
};

/* Check that onepart variables do not contain expressions, only
   values, and that values are linked to each other.  This is also
   supposed to check that stuff carried over from old is ok, when we
   do such things.  */

static int
variable_post_merge (void **slot, void *info ATTRIBUTE_UNUSED)
{
  struct dfset_post_merge *dfpm = (struct dfset_post_merge *)info;
  dataflow_set *set = dfpm->set;
  variable var = (variable)*slot;
  location_chain node;

  if (!dv_onepart_p (var->dv) || !var->n_var_parts)
    return 1;

  gcc_assert (var->n_var_parts == 1);

  if (dv_is_decl_p (var->dv))
    {
      /* ??? Before this point, we have to push non-VALUEs and other
	 equivalences into incoming or newly-generated VALUEs.  */
      for (node = var->var_part[0].loc_chain; node; node = node->next)
	gcc_assert (GET_CODE (node->loc) == VALUE
		    /* ??? Punt and accept MEMs for now.  */
		    || GET_CODE (node->loc) == MEM);
    }
  else
    {
      rtx val = dv_as_value (var->dv);

      for (node = var->var_part[0].loc_chain; node; node = node->next)
	if (GET_CODE (node->loc) == VALUE)
	  {
	    decl_or_value ndv = dv_from_value (node->loc);
	    variable nvar;
	    location_chain nnode;

	    slot = htab_find_slot_with_hash (set->vars, &ndv,
					     dv_htab_hash (ndv), NO_INSERT);
	    if (!slot)
	      continue;

	    nvar = (variable)*slot;
	    gcc_assert (nvar->n_var_parts == 1);
	    for (nnode = nvar->var_part[0].loc_chain; nnode;
		 nnode = nnode->next)
	      if (nnode->loc == val)
		break;

	    gcc_assert (nnode);
	  }
	else if (GET_CODE (node->loc) == REG)
	  attrs_list_insert (&set->regs[REGNO (node->loc)],
			     var->dv, 0, node->loc);
    }

  return 1;
}

/* Just checking stuff and registering register attributes for
   now.  */

static void
dataflow_post_merge_adjust (dataflow_set *set, dataflow_set *old)
{
  struct dfset_post_merge dfpm;

  dfpm.set = set;
  dfpm.old = old;

  htab_traverse (set->vars, variable_post_merge, &dfpm);
}

/* Information to be passed around in a hashtable walk, while removing
   unavailable values.  */

struct dataflow_set_unavailable
{
  /* The set whose variable hashtable we're walking.  */
  dataflow_set *set;

  /* Set to true when anything is removed.  */
  bool removed;
};

/* If a value is not available anywhere, remove it.  When some
   location that references this value is found, remove it as
   well.  */

static int
remove_unavailable_values (void **slot, void *data)
{
  struct dataflow_set_unavailable *dsu;
  dataflow_set *set;
  htab_t vars;
  variable var;

  dsu = (struct dataflow_set_unavailable *) data;
  set = dsu->set;
  vars = set->vars;
  var = (variable) *slot;

  if (dv_onepart_p (var->dv))
    {
      bool keep_any = false;
      location_chain loc = (var->n_var_parts
			    ? var->var_part[0].loc_chain : NULL);
      location_chain *locp;
      bool remove_any = !loc;
      decl_or_value dv;
      void **valslot;

      gcc_assert (remove_any || var->n_var_parts == 1);

      /* See whether there's anything to keep or to remove.  */
      for (; loc && !(remove_any && keep_any);
	   loc = loc->next)
	{
	  if (GET_CODE (loc->loc) != VALUE)
	    {
	      keep_any = true;
	      continue;
	    }

	  dv = dv_from_value (loc->loc);
	  valslot = htab_find_slot_with_hash (vars, &dv, dv_htab_hash (dv),
					      NO_INSERT);
	  if (valslot && ((variable)*valslot)->n_var_parts)
	    keep_any = true;
	  else
	    remove_any = true;
	}

      /* If we're not empty (implied by remove_any's initializer) and
	 there's nothing to remove, we're done.  */
      if (!remove_any)
	return 1;

      /* If there's nothing to keep, just remove it.  */
      if (!keep_any && var->refcount > 1)
	{
	  dsu->removed = true;
	  htab_clear_slot (vars, slot);
	  return 1;
	}

      if (var->refcount > 1)
	/* Unshare the variable before removing useless values.  */
	var = unshare_variable (slot, var, VAR_INIT_STATUS_UNKNOWN);

      for (locp = &var->var_part[0].loc_chain, loc = *locp;
	   loc; loc = *locp)
	{
	  if (GET_CODE (loc->loc) != VALUE)
	    {
	      locp = &loc->next;
	      continue;
	    }

	  dv = dv_from_value (loc->loc);
	  valslot = htab_find_slot_with_hash (vars, &dv, dv_htab_hash (dv),
					      NO_INSERT);
	  if (valslot && ((variable)*valslot)->n_var_parts)
	    locp = &loc->next;
	  else
	    {
	      *locp = loc->next;
	      pool_free (loc_chain_pool, loc);
	    }
	}

      if (!var->var_part[0].loc_chain)
	{
	  gcc_assert (!keep_any);
	  dsu->removed = true;
	  var->n_var_parts = 0;
	  variable_was_changed (var, vars);
	}
    }

  return 1;
}

/* Remove value entries that are not available anywhere, and adjust
   any value-tracking variables that refer to them.  */

static void
dataflow_set_remove_unavailable (dataflow_set *set)
{
  struct dataflow_set_unavailable dsu;
  htab_t vars;

  if (!MAY_HAVE_DEBUG_INSNS)
    return;

  dsu.set = set;
  vars = set->vars;
  do
    {
      dsu.removed = false;
      htab_traverse (vars, remove_unavailable_values, &dsu);
    }
  while (dsu.removed);
}

/* Return a node whose loc is a MEM that refers to EXPR in the
   location list of a one-part variable or value VAR, or in that of
   any values recursively mentioned in the location lists.  */

static location_chain
find_mem_expr_in_1pdv (tree expr, rtx val, htab_t vars)
{
  location_chain node;
  decl_or_value dv;
  void **slot;
  variable var;
  location_chain where = NULL;

  if (!val)
    return NULL;

  gcc_assert (GET_CODE (val) == VALUE);

  gcc_assert (!VALUE_RECURSED_INTO (val));

  dv = dv_from_value (val);
  slot = htab_find_slot_with_hash (vars, &dv, dv_htab_hash (dv), NO_INSERT);

  if (!slot)
    return NULL;

  var = (variable)*slot;

  gcc_assert (var);
  gcc_assert (dv_onepart_p (var->dv));

  if (!var->n_var_parts)
    return NULL;

  gcc_assert (var->var_part[0].offset == 0);

  VALUE_RECURSED_INTO (val) = true;

  for (node = var->var_part[0].loc_chain; node; node = node->next)
    if (MEM_P (node->loc) && MEM_EXPR (node->loc) == expr
	&& MEM_OFFSET (node->loc) == 0)
      {
	where = node;
	break;
      }
    else if (GET_CODE (node->loc) == VALUE
	     && !VALUE_RECURSED_INTO (node->loc)
	     && (where = find_mem_expr_in_1pdv (expr, node->loc, vars)))
      break;

  VALUE_RECURSED_INTO (val) = false;

  return where;
}

/* Remove all MEMs from the location list of a hash table entry for a
   one-part variable, except those whose MEM attributes map back to
   the variable itself, directly or within a VALUE.

   ??? We could also preserve MEMs that reference stack slots that are
   annotated as not addressable.  This is arguably even more reliable
   than the current heuristic.  */

static int
dataflow_set_preserve_mem_locs (void **slot, void *data)
{
  htab_t vars = (htab_t) data;
  variable var = (variable) *slot;

  if (dv_is_decl_p (var->dv) && dv_onepart_p (var->dv))
    {
      tree decl = dv_as_decl (var->dv);
      location_chain loc, *locp;

      if (!var->n_var_parts)
	return 1;

      gcc_assert (var->n_var_parts == 1);

      if (var->refcount > 1)
	{
	  for (loc = var->var_part[0].loc_chain; loc; loc = loc->next)
	    {
	      /* We want to remove a MEM that doesn't refer to DECL.  */
	      if (GET_CODE (loc->loc) == MEM
		  && (MEM_EXPR (loc->loc) != decl
		      || MEM_OFFSET (loc->loc)))
		break;
	      /* We want to move here a MEM that does refer to DECL.  */
	      else if (GET_CODE (loc->loc) == VALUE
		       && find_mem_expr_in_1pdv (decl, loc->loc, vars))
	      break;
	    }

	  if (!loc)
	    return 1;

	  var = unshare_variable (slot, var, VAR_INIT_STATUS_UNKNOWN);
	  gcc_assert (var->n_var_parts == 1);
	}

      for (locp = &var->var_part[0].loc_chain, loc = *locp;
	   loc; loc = *locp)
	{
	  if (GET_CODE (loc->loc) == VALUE)
	    {
	      location_chain mem_node = find_mem_expr_in_1pdv (decl, loc->loc,
							       vars);

	      /* ??? This picks up only one out of multiple MEMs that
		 refer to the same variable.  Do we ever need to be
		 concerned about dealing with more than one, or, given
		 that they should all map to the same variable
		 location, their addresses will have been merged and
		 they will be regarded as equivalent?  */
	      if (mem_node)
		{
		  loc->loc = mem_node->loc;
		  loc->set_src = mem_node->set_src;
		  loc->init = MIN (loc->init, mem_node->init);
		}
	    }

	  if (GET_CODE (loc->loc) != MEM
	      || (MEM_EXPR (loc->loc) == decl
		  && MEM_OFFSET (loc->loc) == 0))
	    {
	      locp = &loc->next;
	      continue;
	    }

	  *locp = loc->next;
	  pool_free (loc_chain_pool, loc);
	}

      if (!var->var_part[0].loc_chain)
	{
	  var->n_var_parts--;
	  variable_was_changed (var, vars);
	}
    }

  return 1;
}

/* Remove all MEMs from the location list of a hash table entry for a
   value.  */

static int
dataflow_set_remove_mem_locs (void **slot, void *data)
{
  htab_t vars = (htab_t) data;
  variable var = (variable) *slot;

  if (dv_is_value_p (var->dv))
    {
      location_chain loc, *locp;

      if (!var->n_var_parts)
	return 1;

      gcc_assert (var->n_var_parts == 1);

      if (var->refcount > 1)
	{
	  for (loc = var->var_part[0].loc_chain; loc; loc = loc->next)
	    if (GET_CODE (loc->loc) == MEM)
	      break;

	  if (!loc)
	    return 1;

	  var = unshare_variable (slot, var, VAR_INIT_STATUS_UNKNOWN);
	  gcc_assert (var->n_var_parts == 1);
	}

      for (locp = &var->var_part[0].loc_chain, loc = *locp;
	   loc; loc = *locp)
	{
	  if (GET_CODE (loc->loc) != MEM)
	    {
	      locp = &loc->next;
	      continue;
	    }

	  *locp = loc->next;
	  pool_free (loc_chain_pool, loc);
	}

      if (!var->var_part[0].loc_chain)
	{
	  var->n_var_parts--;
	  variable_was_changed (var, vars);
	}
    }

  return 1;
}

/* Remove all variable-location information about call-clobbered
   registers, as well as associations between MEMs and VALUEs.  */

static void
dataflow_set_clear_at_call (dataflow_set *set)
{
  int r;

  for (r = 0; r < FIRST_PSEUDO_REGISTER; r++)
    if (TEST_HARD_REG_BIT (call_used_reg_set, r))
      var_regno_delete (set, r);

  if (MAY_HAVE_DEBUG_INSNS)
    {
      htab_traverse (set->vars, dataflow_set_preserve_mem_locs, set->vars);
      htab_traverse (set->vars, dataflow_set_remove_mem_locs, set->vars);
    }
}

/* Flag whether two dataflow sets being compared contain different data.  */
static bool
dataflow_set_different_value;

static bool
variable_part_different_p (variable_part *vp1, variable_part *vp2)
{
  location_chain lc1, lc2;

  for (lc1 = vp1->loc_chain; lc1; lc1 = lc1->next)
    {
      for (lc2 = vp2->loc_chain; lc2; lc2 = lc2->next)
	{
	  if (REG_P (lc1->loc) && REG_P (lc2->loc))
	    {
	      if (REGNO (lc1->loc) == REGNO (lc2->loc))
		break;
	    }
	  if (rtx_equal_p (lc1->loc, lc2->loc))
	    break;
	}
      if (!lc2)
	return true;
    }
  return false;
}

/* Return true if variables VAR1 and VAR2 are different.
   If COMPARE_CURRENT_LOCATION is true compare also the cur_loc of each
   variable part.  */

static bool
variable_different_p (variable var1, variable var2,
		      bool compare_current_location)
{
  int i;

  if (var1 == var2)
    return false;

  if (var1->n_var_parts != var2->n_var_parts)
    return true;

  for (i = 0; i < var1->n_var_parts; i++)
    {
      if (var1->var_part[i].offset != var2->var_part[i].offset)
	return true;
      if (compare_current_location)
	{
	  if (!((REG_P (var1->var_part[i].cur_loc)
		 && REG_P (var2->var_part[i].cur_loc)
		 && (REGNO (var1->var_part[i].cur_loc)
		     == REGNO (var2->var_part[i].cur_loc)))
		|| rtx_equal_p (var1->var_part[i].cur_loc,
				var2->var_part[i].cur_loc)))
	    return true;
	}
      if (variable_part_different_p (&var1->var_part[i], &var2->var_part[i]))
	return true;
      if (variable_part_different_p (&var2->var_part[i], &var1->var_part[i]))
	return true;
    }
  return false;
}

/* Compare variable *SLOT with the same variable in hash table DATA
   and set DATAFLOW_SET_DIFFERENT_VALUE if they are different.  */

static int
dataflow_set_different_1 (void **slot, void *data)
{
  htab_t htab = (htab_t) data;
  variable var1, var2;

  var1 = (variable) *slot;
  var2 = htab_find_with_hash (htab, &var1->dv,
			      dv_htab_hash (var1->dv));
  if (!var2)
    {
      dataflow_set_different_value = true;

      if (dump_file)
	{
	  fprintf (dump_file, "dataflow difference found: removal of:\n");
	  dump_variable (var1);
	}

      /* Stop traversing the hash table.  */
      return 0;
    }

  if (variable_different_p (var1, var2, false))
    {
      dataflow_set_different_value = true;

      if (dump_file)
	{
	  fprintf (dump_file, "dataflow difference found: old and new follow:\n");
	  dump_variable (var1);
	  dump_variable (var2);
	}

      /* Stop traversing the hash table.  */
      return 0;
    }

  /* Continue traversing the hash table.  */
  return 1;
}

/* Compare variable *SLOT with the same variable in hash table DATA
   and set DATAFLOW_SET_DIFFERENT_VALUE if they are different.  */

static int
dataflow_set_different_2 (void **slot, void *data)
{
  htab_t htab = (htab_t) data;
  variable var1, var2;

  var1 = (variable) *slot;
  var2 = htab_find_with_hash (htab, &var1->dv,
			      dv_htab_hash (var1->dv));
  if (!var2)
    {
      dataflow_set_different_value = true;

      if (dump_file)
	{
	  fprintf (dump_file, "dataflow difference found: addition of:\n");
	  dump_variable (var1);
	}

      /* Stop traversing the hash table.  */
      return 0;
    }

  /* If both variables are defined they have been already checked for
     equivalence.  */
  gcc_assert (!variable_different_p (var1, var2, false));

  /* Continue traversing the hash table.  */
  return 1;
}

/* Return true if dataflow sets OLD_SET and NEW_SET differ.  */

static bool
dataflow_set_different (dataflow_set *old_set, dataflow_set *new_set)
{
  dataflow_set_different_value = false;

  htab_traverse (old_set->vars, dataflow_set_different_1, new_set->vars);
  if (!dataflow_set_different_value)
    {
      /* We have compared the variables which are in both hash tables
	 so now only check whether there are some variables in NEW_SET->VARS
	 which are not in OLD_SET->VARS.  */
      htab_traverse (new_set->vars, dataflow_set_different_2, old_set->vars);
    }
  return dataflow_set_different_value;
}

/* Free the contents of dataflow set SET.  */

static void
dataflow_set_destroy (dataflow_set *set)
{
  int i;

  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    attrs_list_clear (&set->regs[i]);

  htab_delete (set->vars);
  set->vars = NULL;
}

/* Return true if RTL X contains a SYMBOL_REF.  */

static bool
contains_symbol_ref (rtx x)
{
  const char *fmt;
  RTX_CODE code;
  int i;

  if (!x)
    return false;

  code = GET_CODE (x);
  if (code == SYMBOL_REF)
    return true;

  fmt = GET_RTX_FORMAT (code);
  for (i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
    {
      if (fmt[i] == 'e')
	{
	  if (contains_symbol_ref (XEXP (x, i)))
	    return true;
	}
      else if (fmt[i] == 'E')
	{
	  int j;
	  for (j = 0; j < XVECLEN (x, i); j++)
	    if (contains_symbol_ref (XVECEXP (x, i, j)))
	      return true;
	}
    }

  return false;
}

/* Shall EXPR be tracked?  */

static bool
track_expr_p (tree expr, bool need_rtl)
{
  rtx decl_rtl;
  tree realdecl;

  /* If EXPR is not a parameter or a variable do not track it.  */
  if (TREE_CODE (expr) != VAR_DECL && TREE_CODE (expr) != PARM_DECL)
    return 0;

  /* It also must have a name...  */
  if (!DECL_NAME (expr))
    return 0;

  /* ... and a RTL assigned to it.  */
  decl_rtl = DECL_RTL_IF_SET (expr);
  if (!decl_rtl && need_rtl)
    return 0;
  
  /* If this expression is really a debug alias of some other declaration, we 
     don't need to track this expression if the ultimate declaration is
     ignored.  */
  realdecl = expr;
  if (DECL_DEBUG_EXPR_IS_FROM (realdecl) && DECL_DEBUG_EXPR (realdecl))
    {
      realdecl = DECL_DEBUG_EXPR (realdecl);
      /* ??? We don't yet know how to emit DW_OP_piece for variable
	 that has been SRA'ed.  */
      if (!DECL_P (realdecl))
	return 0;
    }

  /* Do not track EXPR if REALDECL it should be ignored for debugging
     purposes.  */ 
  if (DECL_IGNORED_P (realdecl))
    return 0;

  /* Do not track global variables until we are able to emit correct location
     list for them.  */
  if (TREE_STATIC (realdecl))
    return 0;

  /* When the EXPR is a DECL for alias of some variable (see example)
     the TREE_STATIC flag is not used.  Disable tracking all DECLs whose
     DECL_RTL contains SYMBOL_REF.

     Example:
     extern char **_dl_argv_internal __attribute__ ((alias ("_dl_argv")));
     char **_dl_argv;
  */
  if (decl_rtl && MEM_P (decl_rtl)
      && contains_symbol_ref (XEXP (decl_rtl, 0)))
    return 0;

  /* If RTX is a memory it should not be very large (because it would be
     an array or struct).  */
  if (decl_rtl && MEM_P (decl_rtl))
    {
      /* Do not track structures and arrays.  */
      if (GET_MODE (decl_rtl) == BLKmode
	  || AGGREGATE_TYPE_P (TREE_TYPE (realdecl)))
	return 0;
      if (MEM_SIZE (decl_rtl)
	  && INTVAL (MEM_SIZE (decl_rtl)) > MAX_VAR_PARTS)
	return 0;
    }

  return 1;
}

/* Determine whether a given LOC refers to the same variable part as
   EXPR+OFFSET.  */

static bool
same_variable_part_p (rtx loc, tree expr, HOST_WIDE_INT offset)
{
  tree expr2;
  HOST_WIDE_INT offset2;

  if (! DECL_P (expr))
    return false;

  if (REG_P (loc))
    {
      expr2 = REG_EXPR (loc);
      offset2 = REG_OFFSET (loc);
    }
  else if (MEM_P (loc))
    {
      expr2 = MEM_EXPR (loc);
      offset2 = INT_MEM_OFFSET (loc);
    }
  else
    return false;

  if (! expr2 || ! DECL_P (expr2))
    return false;

  expr = var_debug_decl (expr);
  expr2 = var_debug_decl (expr2);

  return (expr == expr2 && offset == offset2);
}

/* LOC is a REG or MEM that we would like to track if possible.
   If EXPR is null, we don't know what expression LOC refers to,
   otherwise it refers to EXPR + OFFSET.  STORE_REG_P is true if
   LOC is an lvalue register.

   Return true if EXPR is nonnull and if LOC, or some lowpart of it,
   is something we can track.  When returning true, store the mode of
   the lowpart we can track in *MODE_OUT (if nonnull) and its offset
   from EXPR in *OFFSET_OUT (if nonnull).  */

static bool
track_loc_p (rtx loc, tree expr, HOST_WIDE_INT offset, bool store_reg_p,
	     enum machine_mode *mode_out, HOST_WIDE_INT *offset_out)
{
  enum machine_mode mode;

  if (expr == NULL || !track_expr_p (expr, true))
    return false;

  /* If REG was a paradoxical subreg, its REG_ATTRS will describe the
     whole subreg, but only the old inner part is really relevant.  */
  mode = GET_MODE (loc);
  if (REG_P (loc) && !HARD_REGISTER_NUM_P (ORIGINAL_REGNO (loc)))
    {
      enum machine_mode pseudo_mode;

      pseudo_mode = PSEUDO_REGNO_MODE (ORIGINAL_REGNO (loc));
      if (GET_MODE_SIZE (mode) > GET_MODE_SIZE (pseudo_mode))
	{
	  offset += byte_lowpart_offset (pseudo_mode, mode);
	  mode = pseudo_mode;
	}
    }

  /* If LOC is a paradoxical lowpart of EXPR, refer to EXPR itself.
     Do the same if we are storing to a register and EXPR occupies
     the whole of register LOC; in that case, the whole of EXPR is
     being changed.  We exclude complex modes from the second case
     because the real and imaginary parts are represented as separate
     pseudo registers, even if the whole complex value fits into one
     hard register.  */
  if ((GET_MODE_SIZE (mode) > GET_MODE_SIZE (DECL_MODE (expr))
       || (store_reg_p
	   && !COMPLEX_MODE_P (DECL_MODE (expr))
	   && hard_regno_nregs[REGNO (loc)][DECL_MODE (expr)] == 1))
      && offset + byte_lowpart_offset (DECL_MODE (expr), mode) == 0)
    {
      mode = DECL_MODE (expr);
      offset = 0;
    }

  if (offset < 0 || offset >= MAX_VAR_PARTS)
    return false;

  if (mode_out)
    *mode_out = mode;
  if (offset_out)
    *offset_out = offset;
  return true;
}

/* Return the MODE lowpart of LOC, or null if LOC is not something we
   want to track.  When returning nonnull, make sure that the attributes
   on the returned value are updated.  */

static rtx
var_lowpart (enum machine_mode mode, rtx loc)
{
  unsigned int offset, reg_offset, regno;

  if (!REG_P (loc) && !MEM_P (loc))
    return NULL;

  if (GET_MODE (loc) == mode)
    return loc;

  offset = byte_lowpart_offset (mode, GET_MODE (loc));

  if (MEM_P (loc))
    return adjust_address_nv (loc, mode, offset);

  reg_offset = subreg_lowpart_offset (mode, GET_MODE (loc));
  regno = REGNO (loc) + subreg_regno_offset (REGNO (loc), GET_MODE (loc),
					     reg_offset, mode);
  return gen_rtx_REG_offset (loc, mode, regno, offset);
}

/* Carry information about uses and stores while walking rtx.  */

struct count_use_info
{
  /* The insn where the RTX is.  */
  rtx insn;

  /* The basic block where insn is.  */
  basic_block bb;

  /* The array of n_sets sets in the insn, as determined by cselib.  */
  struct cselib_set *sets;
  int n_sets;

  /* True if we're counting stores, false otherwise.  */
  bool store_p;
};

/* Find a VALUE corresponding to X.   */

static inline cselib_val *
find_use_val (rtx x, enum machine_mode mode, struct count_use_info *cui)
{
  int i;

  if (cui->sets)
    {
      /* This is called after uses are set up and before stores are
	 processed bycselib, so it's safe to look up srcs, but not
	 dsts.  So we look up expressions that appear in srcs or in
	 dest expressions, but we search the sets array for dests of
	 stores.  */
      if (cui->store_p)
	{
	  for (i = 0; i < cui->n_sets; i++)
	    if (cui->sets[i].dest == x)
	      return cui->sets[i].src_elt;
	}
      else
	return cselib_lookup (x, mode, 0);
    }

  return NULL;
}

/* Replace all registers and addresses in an expression with VALUE
   expressions that map back to them, unless the expression is a
   register.  If no mapping is or can be performed, returns NULL.  */

static rtx
replace_expr_with_values (rtx loc)
{
  if (REG_P (loc))
    return NULL;
  else if (MEM_P (loc))
    {
      cselib_val *addr = cselib_lookup (XEXP (loc, 0), GET_MODE (loc), 0);
      if (addr)
	return replace_equiv_address_nv (loc, addr->val_rtx);
      else
	return NULL;
    }
  else
    return cselib_subst_to_values (loc);
}

/* Determine what kind of micro operation to choose for a USE.  Return
   MO_CLOBBER if no micro operation is to be generated.  */

static enum micro_operation_type
use_type (rtx *loc, struct count_use_info *cui, enum machine_mode *modep)
{
  tree expr;
  cselib_val *val;

  if (cui && cui->sets)
    {
      if (GET_CODE (*loc) == VAR_LOCATION)
	{
	  if (track_expr_p (PAT_VAR_LOCATION_DECL (*loc), false))
	    {
	      rtx ploc = PAT_VAR_LOCATION_LOC (*loc);
	      cselib_val *val = cselib_lookup (ploc, GET_MODE (*loc), 1);

	      /* ??? flag_float_store and volatile mems are never
		 given values, but we could in theory use them for
		 locations.  */
	      gcc_assert (val || 1);
	      return MO_VAL_LOC;
	    }
	  else
	    return MO_CLOBBER;
	}

      if ((REG_P (*loc) || MEM_P (*loc))
	  && (val = find_use_val (*loc, GET_MODE (*loc), cui)))
	{
	  if (modep)
	    *modep = GET_MODE (*loc);
	  if (cui->store_p)
	    {
	      if (REG_P (*loc)
		  || cselib_lookup (XEXP (*loc, 0), GET_MODE (*loc), 0))
		return MO_VAL_SET;
	    }
	  else if (!cselib_preserved_value_p (val))
	    return MO_VAL_USE;
	}
    }

  if (REG_P (*loc))
    {
      gcc_assert (REGNO (*loc) < FIRST_PSEUDO_REGISTER);

      expr = REG_EXPR (*loc);

      if (!expr)
	return MO_USE_NO_VAR;
      else if (var_debug_value_for_decl (var_debug_decl (expr)))
	return MO_CLOBBER;
      else if (track_loc_p (*loc, expr, REG_OFFSET (*loc),
			    false, modep, NULL))
	return MO_USE;
      else
	return MO_USE_NO_VAR;
    }
  else if (MEM_P (*loc))
    {
      expr = MEM_EXPR (*loc);

      if (!expr)
	return MO_CLOBBER;
      else if (var_debug_value_for_decl (var_debug_decl (expr)))
	return MO_CLOBBER;
      else if (track_loc_p (*loc, expr, INT_MEM_OFFSET (*loc),
			    false, modep, NULL))
	return MO_USE;
      else
	return MO_CLOBBER;
    }

  return MO_CLOBBER;
}

/* Log to OUT information about micro-operation MOPT involving X in
   INSN of BB.  */

static inline void
log_op_type (rtx x, basic_block bb, rtx insn,
	     enum micro_operation_type mopt, FILE *out)
{
  fprintf (out, "bb %i op %i insn %i %s ",
	   bb->index, VTI (bb)->n_mos - 1,
	   INSN_UID (insn), micro_operation_type_name[mopt]);
  print_inline_rtx (out, x, 2);
  fputc ('\n', out);
}

/* Count uses (register and memory references) LOC which will be tracked.
   INSN is instruction which the LOC is part of.  */

static int
count_uses (rtx *loc, void *cuip)
{
  struct count_use_info *cui = (struct count_use_info *) cuip;
  enum micro_operation_type mopt = use_type (loc, cui, NULL);

  if (mopt != MO_CLOBBER)
    {
      cselib_val *val;
      enum machine_mode mode = GET_MODE (*loc);

      VTI (cui->bb)->n_mos++;

      if (dump_file)
	log_op_type (*loc, cui->bb, cui->insn, mopt, dump_file);

      switch (mopt)
	{
	case MO_VAL_LOC:
	  loc = &PAT_VAR_LOCATION_LOC (*loc);
	  if (VAR_LOC_UNKNOWN_P (*loc))
	    break;
	  /* Fall through.  */

	case MO_VAL_USE:
	case MO_VAL_SET:
	  val = find_use_val (*loc, mode, cui);
	  if (val)
	    cselib_preserve_value (val);
	  else
	    gcc_assert (mopt == MO_VAL_LOC);

	  if (MEM_P (*loc)
	      && !REG_P (XEXP (*loc, 0)) && !MEM_P (XEXP (*loc, 0)))
	    {
	      val = cselib_lookup (XEXP (*loc, 0), mode, false);

	      if (val && !cselib_preserved_value_p (val))
		{
		  VTI (cui->bb)->n_mos++;
		  cselib_preserve_value (val);
		}
	    }
	  break;

	default:
	  break;
	}
    }

  return 0;
}

/* Helper function for finding all uses of REG/MEM in X in CUI's
   insn.  */

static void
count_uses_1 (rtx *x, void *cui)
{
  for_each_rtx (x, count_uses, cui);
}

/* Count stores (register and memory references) LOC which will be
   tracked.  CUI is a count_use_info object containing the instruction
   which the LOC is part of.  */

static void
count_stores (rtx loc, const_rtx expr ATTRIBUTE_UNUSED, void *cui)
{
  count_uses (&loc, cui);
}

/* Callback for cselib_record_sets_hook, that counts how many micro
   operations it takes for uses and stores in an insn after
   cselib_record_sets has analyzed the sets in an insn, but before it
   modifies the stored values in the internal tables, unless
   cselib_record_sets doesn't call it directly (perhaps because we're
   not doing cselib in the first place, in which case sets and n_sets
   will be 0).  */

static void
count_with_sets (rtx insn, struct cselib_set *sets, int n_sets)
{
  basic_block bb = BLOCK_FOR_INSN (insn);
  struct count_use_info cui;

  cselib_hook_called = true;

  cui.insn = insn;
  cui.bb = bb;
  cui.sets = sets;
  cui.n_sets = n_sets;

  cui.store_p = false;
  note_uses (&PATTERN (insn), count_uses_1, &cui);
  cui.store_p = true;
  note_stores (PATTERN (insn), count_stores, &cui);
}

/* Tell whether the CONCAT used to holds a VALUE and its location
   needs value resolution, i.e., an attempt of mapping the location
   back to other incoming values.  */
#define VAL_NEEDS_RESOLUTION(x) \
  (RTL_FLAG_CHECK1 ("VAL_NEEDS_RESOLUTION", (x), CONCAT)->volatil)
/* Whether the location in the CONCAT is a tracked expression, that
   should also be handled like a MO_USE.  */
#define VAL_HOLDS_TRACK_EXPR(x) \
  (RTL_FLAG_CHECK1 ("VAL_HOLDS_TRACK_EXPR", (x), CONCAT)->used)
/* Whether the location in the CONCAT should be handled like a MO_COPY
   as well.  */
#define VAL_EXPR_IS_COPIED(x) \
  (RTL_FLAG_CHECK1 ("VAL_EXPR_IS_COPIED", (x), CONCAT)->jump)
/* Whether the location in the CONCAT should be handled like a
   MO_CLOBBER as well.  */
#define VAL_EXPR_IS_CLOBBERED(x) \
  (RTL_FLAG_CHECK1 ("VAL_EXPR_IS_CLOBBERED", (x), CONCAT)->unchanging)

/* Add uses (register and memory references) LOC which will be tracked
   to VTI (bb)->mos.  INSN is instruction which the LOC is part of.  */

static int
add_uses (rtx *loc, void *data)
{
  enum machine_mode mode = VOIDmode;
  struct count_use_info *cui = (struct count_use_info *)data;
  enum micro_operation_type type = use_type (loc, cui, &mode);

  if (type != MO_CLOBBER)
    {
      basic_block bb = cui->bb;
      micro_operation *mo = VTI (bb)->mos + VTI (bb)->n_mos++;

      mo->type = type;
      mo->u.loc = type == MO_USE ? var_lowpart (mode, *loc) : *loc;
      mo->insn = cui->insn;

      if (type == MO_VAL_LOC)
	{
	  rtx oloc = *loc;
	  rtx vloc = PAT_VAR_LOCATION_LOC (oloc);
	  cselib_val *val;

	  gcc_assert (cui->sets);

	  if (MEM_P (vloc)
	      && !REG_P (XEXP (vloc, 0)) && !MEM_P (XEXP (vloc, 0)))
	    {
	      rtx mloc = vloc;
	      cselib_val *val = cselib_lookup (XEXP (mloc, 0),
					       GET_MODE (mloc), 0);

	      if (val && !cselib_preserved_value_p (val))
		{
		  micro_operation *mon = VTI (bb)->mos + VTI (bb)->n_mos++;
		  mon->type = mo->type;
		  mon->u.loc = mo->u.loc;
		  mon->insn = mo->insn;
		  cselib_preserve_value (val);
		  mo->type = MO_VAL_USE;
		  mloc = cselib_subst_to_values (XEXP (mloc, 0));
		  mo->u.loc = gen_rtx_CONCAT (mode, val->val_rtx, mloc);
		  if (dump_file)
		    log_op_type (mo->u.loc, cui->bb, cui->insn,
				 mo->type, dump_file);
		  mo = mon;
		}
	    }

	  if (!VAR_LOC_UNKNOWN_P (vloc)
	      && (val = find_use_val (vloc, GET_MODE (oloc), cui)))
	    {
	      enum machine_mode mode2;
	      enum micro_operation_type type2;
	      rtx nloc = replace_expr_with_values (vloc);

	      if (nloc)
		{
		  oloc = shallow_copy_rtx (oloc);
		  PAT_VAR_LOCATION_LOC (oloc) = nloc;
		}

	      oloc = gen_rtx_CONCAT (mode, val->val_rtx, oloc);

	      type2 = use_type (&vloc, 0, &mode2);

	      gcc_assert (type2 == MO_USE || type2 == MO_USE_NO_VAR
			  || type2 == MO_CLOBBER);

	      if (type2 == MO_CLOBBER
		  && !cselib_preserved_value_p (val))
		{
		  VAL_NEEDS_RESOLUTION (oloc) = 1;
		  cselib_preserve_value (val);
		}
	    }
	  else if (!VAR_LOC_UNKNOWN_P (vloc))
	    {
	      oloc = shallow_copy_rtx (oloc);
	      PAT_VAR_LOCATION_LOC (oloc) = gen_rtx_UNKNOWN_VAR_LOC ();
	    }

	  mo->u.loc = oloc;
	}
      else if (type == MO_VAL_USE)
	{
	  enum machine_mode mode2 = VOIDmode;
	  enum micro_operation_type type2;
	  cselib_val *val = find_use_val (*loc, GET_MODE (*loc), cui);
	  rtx vloc, oloc, nloc;

	  gcc_assert (cui->sets);

	  if (MEM_P (*loc)
	      && !REG_P (XEXP (*loc, 0)) && !MEM_P (XEXP (*loc, 0)))
	    {
	      rtx mloc = *loc;
	      cselib_val *val = cselib_lookup (XEXP (mloc, 0),
					       GET_MODE (mloc), 0);

	      if (val && !cselib_preserved_value_p (val))
		{
		  micro_operation *mon = VTI (bb)->mos + VTI (bb)->n_mos++;
		  mon->type = mo->type;
		  mon->u.loc = mo->u.loc;
		  mon->insn = mo->insn;
		  cselib_preserve_value (val);
		  mo->type = MO_VAL_USE;
		  mloc = cselib_subst_to_values (XEXP (mloc, 0));
		  mo->u.loc = gen_rtx_CONCAT (mode, val->val_rtx, mloc);
		  mo->insn = cui->insn;
		  if (dump_file)
		    log_op_type (mo->u.loc, cui->bb, cui->insn,
				 mo->type, dump_file);
		  mo = mon;
		}
	    }

	  type2 = use_type (loc, 0, &mode2);

	  gcc_assert (type2 == MO_USE || type2 == MO_USE_NO_VAR
		      || type2 == MO_CLOBBER);

	  if (type2 == MO_USE)
	    vloc = var_lowpart (mode2, *loc);
	  else
	    vloc = *loc;

	  nloc = replace_expr_with_values (vloc);
	  if (nloc)
	    vloc = nloc;

	  if (vloc != *loc)
	    oloc = gen_rtx_CONCAT (mode2, val->val_rtx, vloc);
	  else
	    oloc = val->val_rtx;

	  mo->u.loc = gen_rtx_CONCAT (mode, oloc, *loc);

	  if (type2 == MO_USE)
	    VAL_HOLDS_TRACK_EXPR (mo->u.loc) = 1;
	  if (!cselib_preserved_value_p (val))
	    {
	      VAL_NEEDS_RESOLUTION (mo->u.loc) = 1;
	      cselib_preserve_value (val);
	    }
	}
      else
	gcc_assert (type == MO_USE || type == MO_USE_NO_VAR);

      if (dump_file)
	log_op_type (mo->u.loc, cui->bb, cui->insn, mo->type, dump_file);
    }

  return 0;
}

/* Helper function for finding all uses of REG/MEM in X in insn INSN.  */

static void
add_uses_1 (rtx *x, void *cui)
{
  for_each_rtx (x, add_uses, cui);
}

/* Add stores (register and memory references) LOC which will be tracked
   to VTI (bb)->mos.  EXPR is the RTL expression containing the store.
   CUIP->insn is instruction which the LOC is part of.  */

static void
add_stores (rtx loc, const_rtx expr, void *cuip)
{
  enum machine_mode mode = VOIDmode;
  struct count_use_info *cui = (struct count_use_info *)cuip;
  basic_block bb = cui->bb;
  micro_operation *mo;
  rtx oloc = loc, nloc, src = NULL;
  enum micro_operation_type type = use_type (&loc, cui, &mode);
  bool track_p = false;
  cselib_val *v;
  bool resolve, preserve;

  if (type == MO_CLOBBER)
    return;

  if (REG_P (loc))
    {
      mo = VTI (bb)->mos + VTI (bb)->n_mos++;

      if ((GET_CODE (expr) == CLOBBER && type != MO_VAL_SET)
	  || !(track_p = use_type (&loc, NULL, NULL) == MO_USE)
	  || GET_CODE (expr) == CLOBBER)
	{
	  mo->type = MO_CLOBBER;
	  mo->u.loc = loc;
	}
      else
	{
	  if (GET_CODE (expr) == SET && SET_DEST (expr) == loc)
	    src = var_lowpart (mode, SET_SRC (expr));
	  loc = var_lowpart (mode, loc);

	  if (src == NULL)
	    {
	      mo->type = MO_SET;
	      mo->u.loc = loc;
	    }
	  else
	    {
	      if (SET_SRC (expr) != src)
		expr = gen_rtx_SET (VOIDmode, loc, src);
	      if (same_variable_part_p (src, REG_EXPR (loc), REG_OFFSET (loc)))
		mo->type = MO_COPY;
	      else
		mo->type = MO_SET;
	      mo->u.loc = CONST_CAST_RTX (expr);
	    }
	}
      mo->insn = cui->insn;
    }
  else if (MEM_P (loc)
	   && ((track_p = use_type (&loc, NULL, NULL) == MO_USE)
	       || cui->sets))
    {
      mo = VTI (bb)->mos + VTI (bb)->n_mos++;

      if (MEM_P (loc) && type == MO_VAL_SET
	  && !REG_P (XEXP (loc, 0)) && !MEM_P (XEXP (loc, 0)))
	{
	  rtx mloc = loc;
	  cselib_val *val = cselib_lookup (XEXP (mloc, 0), GET_MODE (mloc), 0);

	  if (val && !cselib_preserved_value_p (val))
	    {
	      cselib_preserve_value (val);
	      mo->type = MO_VAL_USE;
	      mloc = cselib_subst_to_values (XEXP (mloc, 0));
	      mo->u.loc = gen_rtx_CONCAT (mode, val->val_rtx, mloc);
	      mo->insn = cui->insn;
	      if (dump_file)
		log_op_type (mo->u.loc, cui->bb, cui->insn,
			     mo->type, dump_file);
	      mo = VTI (bb)->mos + VTI (bb)->n_mos++;
	    }
	}

      if (GET_CODE (expr) == CLOBBER || !track_p)
	{
	  mo->type = MO_CLOBBER;
	  mo->u.loc = track_p ? var_lowpart (mode, loc) : loc;
	}
      else
	{
	  if (GET_CODE (expr) == SET && SET_DEST (expr) == loc)
	    src = var_lowpart (mode, SET_SRC (expr));
	  loc = var_lowpart (mode, loc);

	  if (src == NULL)
	    {
	      mo->type = MO_SET;
	      mo->u.loc = loc;
	    }
	  else
	    {
	      if (SET_SRC (expr) != src)
		expr = gen_rtx_SET (VOIDmode, loc, src);
	      if (same_variable_part_p (SET_SRC (expr),
					MEM_EXPR (loc),
					INT_MEM_OFFSET (loc)))
		mo->type = MO_COPY;
	      else
		mo->type = MO_SET;
	      mo->u.loc = CONST_CAST_RTX (expr);
	    }
	}
      mo->insn = cui->insn;
    }
  else
    return;

  if (type != MO_VAL_SET)
    goto log_and_return;

  v = find_use_val (oloc, mode, cui);

  resolve = preserve = !cselib_preserved_value_p (v);

  nloc = replace_expr_with_values (oloc);
  if (nloc)
    oloc = nloc;

  if (resolve && GET_CODE (mo->u.loc) == SET)
    {
      nloc = replace_expr_with_values (SET_SRC (mo->u.loc));

      if (nloc)
	oloc = gen_rtx_SET (GET_MODE (mo->u.loc), oloc, nloc);
      else
	{
	  if (oloc == SET_DEST (mo->u.loc))
	    /* No point in duplicating.  */
	    oloc = mo->u.loc;
	  if (!REG_P (SET_SRC (mo->u.loc)))
	    resolve = false;
	}
    }
  else if (!resolve)
    {
      if (GET_CODE (mo->u.loc) == SET
	  && oloc == SET_DEST (mo->u.loc))
	/* No point in duplicating.  */
	oloc = mo->u.loc;
    }
  else
    resolve = false;

  loc = gen_rtx_CONCAT (mode, v->val_rtx, oloc);

  if (mo->u.loc != oloc)
    loc = gen_rtx_CONCAT (GET_MODE (mo->u.loc), loc, mo->u.loc);

  /* The loc of a MO_VAL_SET may have various forms:

     (concat val dst): dst now holds val

     (concat val (set dst src)): dst now holds val, copied from src

     (concat (concat val dstv)) dst): dst now holds val; dstv is dst
     after replacing mems and non-top-level regs with values.

     (concat (concat val dstv) (set dst src)): dst now holds val,
     copied from src.  dstv is a value-based representation of dst, if
     it differs from dst.  If resolution is needed, src is a REG.

     (concat (concat val (set dstv srcv)) (set dst src)): src
     copied to dst, holding val.  dstv and srcv are value-based
     representations of dst and src, respectively.

  */

  mo->u.loc = loc;

  if (track_p)
    VAL_HOLDS_TRACK_EXPR (loc) = 1;
  if (preserve)
    {
      VAL_NEEDS_RESOLUTION (loc) = resolve;
      cselib_preserve_value (v);
    }
  if (mo->type == MO_CLOBBER)
    VAL_EXPR_IS_CLOBBERED (loc) = 1;
  if (mo->type == MO_COPY)
    VAL_EXPR_IS_COPIED (loc) = 1;

  mo->type = MO_VAL_SET;

 log_and_return:
  if (dump_file)
    log_op_type (mo->u.loc, cui->bb, cui->insn, mo->type, dump_file);
}

/* Callback for cselib_record_sets_hook, that records as micro
   operations uses and stores in an insn after cselib_record_sets has
   analyzed the sets in an insn, but before it modifies the stored
   values in the internal tables, unless cselib_record_sets doesn't
   call it directly (perhaps because we're not doing cselib in the
   first place, in which case sets and n_sets will be 0).  */

static void
add_with_sets (rtx insn, struct cselib_set *sets, int n_sets)
{
  basic_block bb = BLOCK_FOR_INSN (insn);
  int n1, n2;
  struct count_use_info cui;

  cselib_hook_called = true;

  cui.insn = insn;
  cui.bb = bb;
  cui.sets = sets;
  cui.n_sets = n_sets;

  n1 = VTI (bb)->n_mos;
  cui.store_p = false;
  note_uses (&PATTERN (insn), add_uses_1, &cui);
  n2 = VTI (bb)->n_mos - 1;

  /* Order the MO_USEs to be before MO_USE_NO_VARs,
     MO_VAL_LOC and MO_VAL_USE.  */
  while (n1 < n2)
    {
      while (n1 < n2 && VTI (bb)->mos[n1].type == MO_USE)
	n1++;
      while (n1 < n2 && VTI (bb)->mos[n2].type != MO_USE)
	n2--;
      if (n1 < n2)
	{
	  micro_operation sw;

	  sw = VTI (bb)->mos[n1];
	  VTI (bb)->mos[n1] = VTI (bb)->mos[n2];
	  VTI (bb)->mos[n2] = sw;
	}
    }

  if (CALL_P (insn))
    {
      micro_operation *mo = VTI (bb)->mos + VTI (bb)->n_mos++;

      mo->type = MO_CALL;
      mo->insn = insn;

      if (dump_file)
	log_op_type (PATTERN (insn), bb, insn, mo->type, dump_file);
    }

  n1 = VTI (bb)->n_mos;
  /* This will record NEXT_INSN (insn), such that we can
     insert notes before it without worrying about any
     notes that MO_USEs might emit after the insn.  */
  cui.store_p = true;
  note_stores (PATTERN (insn), add_stores, &cui);
  n2 = VTI (bb)->n_mos - 1;

  /* Order the MO_CLOBBERs to be before MO_SETs.  */
  while (n1 < n2)
    {
      while (n1 < n2 && VTI (bb)->mos[n1].type == MO_CLOBBER)
	n1++;
      while (n1 < n2 && VTI (bb)->mos[n2].type != MO_CLOBBER)
	n2--;
      if (n1 < n2)
	{
	  micro_operation sw;

	  sw = VTI (bb)->mos[n1];
	  VTI (bb)->mos[n1] = VTI (bb)->mos[n2];
	  VTI (bb)->mos[n2] = sw;
	}
    }
}

static enum var_init_status
find_src_status (dataflow_set *in, rtx src)
{
  tree decl = NULL_TREE;
  enum var_init_status status = VAR_INIT_STATUS_UNINITIALIZED;

  if (! flag_var_tracking_uninit)
    status = VAR_INIT_STATUS_INITIALIZED;

  if (src && REG_P (src))
    decl = var_debug_decl (REG_EXPR (src));
  else if (src && MEM_P (src))
    decl = var_debug_decl (MEM_EXPR (src));

  if (src && decl)
    status = get_init_value (in, src, dv_from_decl (decl));

  return status;
}

/* SRC is the source of an assignment.  Use SET to try to find what
   was ultimately assigned to SRC.  Return that value if known,
   otherwise return SRC itself.  */

static rtx
find_src_set_src (dataflow_set *set, rtx src)
{
  tree decl = NULL_TREE;   /* The variable being copied around.          */
  rtx set_src = NULL_RTX;  /* The value for "decl" stored in "src".      */
  void **slot;
  variable var;
  location_chain nextp;
  int i;
  bool found;

  if (src && REG_P (src))
    decl = var_debug_decl (REG_EXPR (src));
  else if (src && MEM_P (src))
    decl = var_debug_decl (MEM_EXPR (src));

  if (src && decl)
    {
      decl_or_value dv = dv_from_decl (decl);

      slot = htab_find_slot_with_hash (set->vars, &dv,
				       VARIABLE_HASH_VAL (decl), NO_INSERT);

      if (slot)
	{
	  var = (variable) *slot;
	  found = false;
	  for (i = 0; i < var->n_var_parts && !found; i++)
	    for (nextp = var->var_part[i].loc_chain; nextp && !found; 
		 nextp = nextp->next)
	      if (rtx_equal_p (nextp->loc, src))
		{
		  set_src = nextp->set_src;
		  found = true;
		}
	      
	}
    }

  return set_src;
}

/* Compute the changes of variable locations in the basic block BB.  */

static bool
compute_bb_dataflow (basic_block bb)
{
  int i, n;
  bool changed;
  dataflow_set old_out;
  dataflow_set *in = &VTI (bb)->in;
  dataflow_set *out = &VTI (bb)->out;

  dataflow_set_init (&old_out, htab_elements (VTI (bb)->out.vars) + 3);
  dataflow_set_copy (&old_out, out);
  dataflow_set_copy (out, in);

  n = VTI (bb)->n_mos;
  for (i = 0; i < n; i++)
    {
      rtx insn = VTI (bb)->mos[i].insn;

      switch (VTI (bb)->mos[i].type)
	{
	  case MO_CALL:
	    dataflow_set_clear_at_call (out);
	    break;

	  case MO_USE:
	    {
	      rtx loc = VTI (bb)->mos[i].u.loc;
	      enum var_init_status status = VAR_INIT_STATUS_UNINITIALIZED;

	      if (! flag_var_tracking_uninit)
		status = VAR_INIT_STATUS_INITIALIZED;

	      if (GET_CODE (loc) == REG)
		var_reg_set (out, loc, status, NULL);
	      else if (GET_CODE (loc) == MEM)
		var_mem_set (out, loc, status, NULL);
	    }
	    break;

	  case MO_VAL_LOC:
	    {
	      rtx loc = VTI (bb)->mos[i].u.loc;
	      rtx val, vloc;
	      tree var;

	      if (GET_CODE (loc) == CONCAT)
		{
		  val = XEXP (loc, 0);
		  vloc = XEXP (loc, 1);
		}
	      else
		{
		  val = NULL_RTX;
		  vloc = loc;
		}

	      var = PAT_VAR_LOCATION_DECL (vloc);

	      clobber_variable_part (out, NULL_RTX,
				     dv_from_decl (var), 0, NULL_RTX);
	      if (val)
		{
		  if (VAL_NEEDS_RESOLUTION (loc))
		    val_resolve (out, val, PAT_VAR_LOCATION_LOC (vloc), insn);
		  set_variable_part (out, val, dv_from_decl (var), 0,
				     VAR_INIT_STATUS_INITIALIZED, NULL_RTX);
		}
	    }
	    break;

	  case MO_VAL_USE:
	    {
	      rtx loc = VTI (bb)->mos[i].u.loc;
	      rtx val, vloc, uloc;

	      vloc = uloc = XEXP (loc, 1);
	      val = XEXP (loc, 0);

	      if (GET_CODE (val) == CONCAT)
		{
		  vloc = XEXP (val, 1);
		  val = XEXP (val, 0);
		}

	      if (VAL_NEEDS_RESOLUTION (loc))
		val_resolve (out, val, vloc, insn);

	      if (VAL_HOLDS_TRACK_EXPR (loc))
		{
		  enum var_init_status status = VAR_INIT_STATUS_UNINITIALIZED;

		  if (! flag_var_tracking_uninit)
		    status = VAR_INIT_STATUS_INITIALIZED;

		  if (GET_CODE (uloc) == REG)
		    var_reg_set (out, uloc, status, NULL);
		  else if (GET_CODE (uloc) == MEM)
		    var_mem_set (out, uloc, status, NULL);
		}
	    }
	    break;

	  case MO_VAL_SET:
	    {
	      rtx loc = VTI (bb)->mos[i].u.loc;
	      rtx val, vloc, uloc;

	      vloc = uloc = XEXP (loc, 1);
	      val = XEXP (loc, 0);

	      if (GET_CODE (val) == CONCAT)
		{
		  vloc = XEXP (val, 1);
		  val = XEXP (val, 0);
		}

	      if (GET_CODE (vloc) == SET)
		{
		  rtx vsrc = SET_SRC (vloc);

		  gcc_assert (val != vsrc);
		  gcc_assert (vloc == uloc || VAL_NEEDS_RESOLUTION (loc));

		  vloc = SET_DEST (vloc);

		  if (VAL_NEEDS_RESOLUTION (loc))
		    val_resolve (out, val, vsrc, insn);
		}
	      else if (VAL_NEEDS_RESOLUTION (loc))
		{
		  gcc_assert (GET_CODE (uloc) == SET
			      && GET_CODE (SET_SRC (uloc)) == REG);
		  val_resolve (out, val, SET_SRC (uloc), insn);
		}

	      if (VAL_HOLDS_TRACK_EXPR (loc))
		{
		  if (VAL_EXPR_IS_CLOBBERED (loc))
		    {
		      if (REG_P (uloc))
			var_reg_delete (out, uloc, true);
		      else if (MEM_P (uloc))
			var_mem_delete (out, uloc, true);
		    }
		  else
		    {
		      bool copied_p = VAL_EXPR_IS_COPIED (loc);
		      rtx set_src = NULL;
		      enum var_init_status status = VAR_INIT_STATUS_INITIALIZED;

		      if (GET_CODE (uloc) == SET)
			{
			  set_src = SET_SRC (uloc);
			  uloc = SET_DEST (uloc);
			}

		      if (copied_p)
			{
			  if (flag_var_tracking_uninit)
			    status = find_src_status (in, set_src);

			  if (status == VAR_INIT_STATUS_UNKNOWN)
			    status = find_src_status (out, set_src);

			  set_src = find_src_set_src (in, set_src);
			}

		      if (REG_P (uloc))
			var_reg_delete_and_set (out, uloc, !copied_p,
						status, set_src);
		      else if (MEM_P (uloc))
			var_mem_delete_and_set (out, uloc, !copied_p,
						status, set_src);
		    }
		}
	      else if (REG_P (uloc))
		var_regno_delete (out, REGNO (uloc));

	      val_store (out, val, vloc, insn);
	    }
	    break;

	  case MO_SET:
	    {
	      rtx loc = VTI (bb)->mos[i].u.loc;
	      rtx set_src = NULL;

	      if (GET_CODE (loc) == SET)
		{
		  set_src = SET_SRC (loc);
		  loc = SET_DEST (loc);
		}

	      if (REG_P (loc))
		var_reg_delete_and_set (out, loc, true, VAR_INIT_STATUS_INITIALIZED,
					set_src);
	      else if (MEM_P (loc))
		var_mem_delete_and_set (out, loc, true, VAR_INIT_STATUS_INITIALIZED,
					set_src);
	    }
	    break;

	  case MO_COPY:
	    {
	      rtx loc = VTI (bb)->mos[i].u.loc;
	      enum var_init_status src_status;
	      rtx set_src = NULL;

	      if (GET_CODE (loc) == SET)
		{
		  set_src = SET_SRC (loc);
		  loc = SET_DEST (loc);
		}

	      if (! flag_var_tracking_uninit)
		src_status = VAR_INIT_STATUS_INITIALIZED;
	      else
		src_status = find_src_status (in, set_src);

	      if (src_status == VAR_INIT_STATUS_UNKNOWN)
		src_status = find_src_status (out, set_src);

	      set_src = find_src_set_src (in, set_src);

	      if (REG_P (loc))
		var_reg_delete_and_set (out, loc, false, src_status, set_src);
	      else if (MEM_P (loc))
		var_mem_delete_and_set (out, loc, false, src_status, set_src);
	    }
	    break;

	  case MO_USE_NO_VAR:
	    {
	      rtx loc = VTI (bb)->mos[i].u.loc;

	      if (REG_P (loc))
		var_reg_delete (out, loc, false);
	      else if (MEM_P (loc))
		var_mem_delete (out, loc, false);
	    }
	    break;

	  case MO_CLOBBER:
	    {
	      rtx loc = VTI (bb)->mos[i].u.loc;

	      if (REG_P (loc))
		var_reg_delete (out, loc, true);
	      else if (MEM_P (loc))
		var_mem_delete (out, loc, true);
	    }
	    break;

	  case MO_ADJUST:
	    out->stack_adjust += VTI (bb)->mos[i].u.adjust;
	    break;
	}
    }

  dataflow_set_remove_unavailable (out);
  changed = dataflow_set_different (&old_out, out);
  dataflow_set_destroy (&old_out);
  return changed;
}

/* Find the locations of variables in the whole function.  */

static void
vt_find_locations (void)
{
  fibheap_t worklist, pending, fibheap_swap;
  sbitmap visited, in_worklist, in_pending, sbitmap_swap;
  basic_block bb;
  edge e;
  int *bb_order;
  int *rc_order;
  int i;
  int htabsz = 0;

  /* Compute reverse completion order of depth first search of the CFG
     so that the data-flow runs faster.  */
  rc_order = XNEWVEC (int, n_basic_blocks - NUM_FIXED_BLOCKS);
  bb_order = XNEWVEC (int, last_basic_block);
  pre_and_rev_post_order_compute (NULL, rc_order, false);
  for (i = 0; i < n_basic_blocks - NUM_FIXED_BLOCKS; i++)
    bb_order[rc_order[i]] = i;
  free (rc_order);

  worklist = fibheap_new ();
  pending = fibheap_new ();
  visited = sbitmap_alloc (last_basic_block);
  in_worklist = sbitmap_alloc (last_basic_block);
  in_pending = sbitmap_alloc (last_basic_block);
  sbitmap_zero (in_worklist);

  if (MAY_HAVE_DEBUG_INSNS)
    {
      gcc_assert (!merge_with_missing_1pdv_as_union);
     repeat_with_intersect:
      merge_with_missing_1pdv_as_union = !merge_with_missing_1pdv_as_union;
    }

  if (merge_with_missing_1pdv_as_union)
    {
      FOR_EACH_BB (bb)
	fibheap_insert (pending, bb_order[bb->index], bb);
      sbitmap_ones (in_pending);
    }
  else
    {
      FOR_EACH_BB (bb)
	if (!single_pred_p (bb))
	  {
	    fibheap_insert (pending, bb_order[bb->index], bb);
	    SET_BIT (in_pending, bb->index);
	  }
    }

  while (!fibheap_empty (pending))
    {
      fibheap_swap = pending;
      pending = worklist;
      worklist = fibheap_swap;
      sbitmap_swap = in_pending;
      in_pending = in_worklist;
      in_worklist = sbitmap_swap;

      sbitmap_zero (visited);

      while (!fibheap_empty (worklist))
	{
	  bb = fibheap_extract_min (worklist);
	  RESET_BIT (in_worklist, bb->index);
	  if (!TEST_BIT (visited, bb->index))
	    {
	      bool changed;
	      edge_iterator ei;

	      SET_BIT (visited, bb->index);

	      if (VTI (bb)->in.vars)
		htabsz -= VTI (bb)->in.vars->size + VTI (bb)->out.vars->size;

	      /* Calculate the IN set as union of predecessor OUT sets.  */
	      if (MAY_HAVE_DEBUG_INSNS)
		{
		  bool first = true, adjust = false;
		  dataflow_set newin;

		  dataflow_set_init (&newin,
				     VTI (bb)->in.vars ?
				     htab_elements (VTI (bb)->in.vars)
				     : 3);

		  FOR_EACH_EDGE (e, ei, bb->preds)
		    {
		      if (first)
			{
			  dataflow_set_copy (&newin, &VTI (e->src)->out);
			  first = false;
			}
		      else
			{
			  dataflow_set_merge (&newin, &VTI (e->src)->out);
			  adjust = true;
			}
		    }

		  if (adjust)
		    dataflow_post_merge_adjust (&newin, &VTI (bb)->in);

		  dataflow_set_destroy (&VTI (bb)->in);
		  VTI (bb)->in = newin;
		}
	      else
		{
		  dataflow_set_clear (&VTI (bb)->in);
		  FOR_EACH_EDGE (e, ei, bb->preds)
		    dataflow_set_union (&VTI (bb)->in, &VTI (e->src)->out);
		}

	      changed = compute_bb_dataflow (bb);
	      htabsz += VTI (bb)->in.vars->size + VTI (bb)->out.vars->size;

	      if (dump_file)
		fprintf (dump_file,
			 "BB %i: in %i, out %i, tsz %i, rem %i\n",
			 bb->index, (int)VTI (bb)->in.vars->n_elements,
			 (int)VTI (bb)->out.vars->n_elements,
			 htabsz, (int)worklist->nodes);

	      if (changed)
		{
		  if (dump_file)
		    fprintf (dump_file, "BB %i ->", bb->index);

		  FOR_EACH_EDGE (e, ei, bb->succs)
		    {
		      if (e->dest == EXIT_BLOCK_PTR)
			continue;

		      if (dump_file)
			fprintf (dump_file, " %i", e->dest->index);

		      if (TEST_BIT (visited, e->dest->index))
			{
			  if (!TEST_BIT (in_pending, e->dest->index))
			    {
			      /* Send E->DEST to next round.  */
			      SET_BIT (in_pending, e->dest->index);
			      fibheap_insert (pending,
					      bb_order[e->dest->index],
					      e->dest);
			    }
			}
		      else if (!TEST_BIT (in_worklist, e->dest->index))
			{
			  /* Add E->DEST to current round.  */
			  SET_BIT (in_worklist, e->dest->index);
			  fibheap_insert (worklist, bb_order[e->dest->index],
					  e->dest);
			}
		    }

		  if (dump_file)
		    fputc ('\n', dump_file);
		}

	      if (dump_file)
		{
		  fprintf (dump_file, "BB %i IN:\n", bb->index);
		  dump_dataflow_set (&VTI (bb)->in);
		  fprintf (dump_file, "BB %i OUT:\n", bb->index);
		  dump_dataflow_set (&VTI (bb)->out);
		}
	    }
	}
    }

  if (MAY_HAVE_DEBUG_INSNS
      && merge_with_missing_1pdv_as_union)
    {
      if (dump_file)
	fprintf (dump_file,
		 "dataflow union completed, starting dataflow merge\n");
      goto repeat_with_intersect;
    }

  free (bb_order);
  fibheap_delete (worklist);
  fibheap_delete (pending);
  sbitmap_free (visited);
  sbitmap_free (in_worklist);
  sbitmap_free (in_pending);
}

/* Print the content of the LIST to dump file.  */

static void
dump_attrs_list (attrs list)
{
  for (; list; list = list->next)
    {
      if (dv_is_decl_p (list->dv))
	print_mem_expr (dump_file, dv_as_decl (list->dv));
      else
	print_rtl_single (dump_file, dv_as_value (list->dv));
      fprintf (dump_file, "+" HOST_WIDE_INT_PRINT_DEC, list->offset);
    }
  fprintf (dump_file, "\n");
}

/* Print the information about variable *SLOT to dump file.  */

static int
dump_variable_slot (void **slot, void *data ATTRIBUTE_UNUSED)
{
  variable var = (variable) *slot;

  dump_variable (var);

  /* Continue traversing the hash table.  */
  return 1;
}

/* Print the information about variable VAR to dump file.  */

static void
dump_variable (variable var)
{
  int i;
  location_chain node;

  if (dv_is_decl_p (var->dv))
    {
      const_tree decl = dv_as_decl (var->dv);

      if (DECL_NAME (decl))
	fprintf (dump_file, "  name: %s",
		 IDENTIFIER_POINTER (DECL_NAME (decl)));
      else
	fprintf (dump_file, "  name: D.%u", DECL_UID (decl));
      if (dump_flags & TDF_UID)
	fprintf (dump_file, " D.%u\n", DECL_UID (decl));
      else
	fprintf (dump_file, "\n");
    }
  else
    {
      fputc (' ', dump_file);
      print_rtl_single (dump_file, dv_as_value (var->dv));
    }

  for (i = 0; i < var->n_var_parts; i++)
    {
      fprintf (dump_file, "    offset %ld\n",
	       (long) var->var_part[i].offset);
      for (node = var->var_part[i].loc_chain; node; node = node->next)
	{
	  fprintf (dump_file, "      ");
	  if (node->init == VAR_INIT_STATUS_UNINITIALIZED)
	    fprintf (dump_file, "[uninit]");
	  print_rtl_single (dump_file, node->loc);
	}
    }
}

/* Print the information about variables from hash table VARS to dump file.  */

static void
dump_vars (htab_t vars)
{
  if (htab_elements (vars) > 0)
    {
      fprintf (dump_file, "Variables:\n");
      htab_traverse (vars, dump_variable_slot, NULL);
    }
}

/* Print the dataflow set SET to dump file.  */

static void
dump_dataflow_set (dataflow_set *set)
{
  int i;

  fprintf (dump_file, "Stack adjustment: " HOST_WIDE_INT_PRINT_DEC "\n",
	   set->stack_adjust);
  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    {
      if (set->regs[i])
	{
	  fprintf (dump_file, "Reg %d:", i);
	  dump_attrs_list (set->regs[i]);
	}
    }
  dump_vars (set->vars);
  fprintf (dump_file, "\n");
}

/* Print the IN and OUT sets for each basic block to dump file.  */

static void
dump_dataflow_sets (void)
{
  basic_block bb;

  FOR_EACH_BB (bb)
    {
      fprintf (dump_file, "\nBasic block %d:\n", bb->index);
      fprintf (dump_file, "IN:\n");
      dump_dataflow_set (&VTI (bb)->in);
      fprintf (dump_file, "OUT:\n");
      dump_dataflow_set (&VTI (bb)->out);
    }
}

/* Add variable VAR to the hash table of changed variables and
   if it has no locations delete it from hash table HTAB.  */

static void
variable_was_changed (variable var, htab_t htab)
{
  hashval_t hash = dv_htab_hash (var->dv);

  if (emit_notes)
    {
      void **slot;

      slot = htab_find_slot_with_hash (changed_variables,
				       &var->dv,
				       hash, INSERT);

      if (htab && var->n_var_parts == 0)
	{
	  variable empty_var;
	  void **old;

	  empty_var = pool_alloc (dv_pool (var->dv));
	  empty_var->dv = var->dv;
	  empty_var->refcount = 1;
	  empty_var->n_var_parts = 0;
	  *slot = empty_var;

	  old = htab_find_slot_with_hash (htab, &var->dv, hash,
					  NO_INSERT);
	  if (old)
	    htab_clear_slot (htab, old);
	}
      else
	{
	  *slot = var;
	}
    }
  else
    {
      gcc_assert (htab);
      if (var->n_var_parts == 0)
	{
	  void **slot = htab_find_slot_with_hash (htab, &var->dv,
						  hash, NO_INSERT);
	  if (slot)
	    htab_clear_slot (htab, slot);
	}
    }
}

/* Look for the index in VAR->var_part corresponding to OFFSET.
   Return -1 if not found.  If INSERTION_POINT is non-NULL, the
   referenced int will be set to the index that the part has or should
   have, if it should be inserted.  */

static inline int
find_variable_location_part (variable var, HOST_WIDE_INT offset,
			     int *insertion_point)
{
  int pos, low, high;

  /* Find the location part.  */
  low = 0;
  high = var->n_var_parts;
  while (low != high)
    {
      pos = (low + high) / 2;
      if (var->var_part[pos].offset < offset)
	low = pos + 1;
      else
	high = pos;
    }
  pos = low;

  if (insertion_point)
    *insertion_point = pos;

  if (pos < var->n_var_parts && var->var_part[pos].offset == offset)
    return pos;

  return -1;
}

static void
set_slot_part (dataflow_set *set, rtx loc, void **slot,
	       decl_or_value dv, HOST_WIDE_INT offset,
	       enum var_init_status initialized, rtx set_src)
{
  int pos;
  location_chain node, next;
  location_chain *nextp;
  variable var;
  
  gcc_assert (offset == 0 || !dv_onepart_p (dv));
  gcc_assert (loc != dv_as_opaque (dv));

  if (!*slot)
    {
      /* Create new variable information.  */
      var = pool_alloc (dv_pool (dv));
      var->dv = dv;
      var->refcount = 1;
      var->n_var_parts = 1;
      var->var_part[0].offset = offset;
      var->var_part[0].loc_chain = NULL;
      var->var_part[0].cur_loc = NULL;
      *slot = var;
      pos = 0;
    }
  else
    {
      int inspos = 0;

      var = (variable) *slot;
      gcc_assert (dv_as_opaque (var->dv) == dv_as_opaque (dv));

      pos = find_variable_location_part (var, offset, &inspos);

      if (pos >= 0)
	{
	  node = var->var_part[pos].loc_chain;

	  if (node
	      && ((REG_P (node->loc) && REG_P (loc)
		   && REGNO (node->loc) == REGNO (loc))
		  || rtx_equal_p (node->loc, loc)))
	    {
	      /* LOC is in the beginning of the chain so we have nothing
		 to do.  */
	      if (node->init < initialized)
		node->init = initialized;
	      if (set_src != NULL)
		node->set_src = set_src;

	      *slot = var;
	      return;
	    }
	  else
	    {
	      /* We have to make a copy of a shared variable.  */
	      if (var->refcount > 1)
		var = unshare_variable (slot, var, initialized);
	    }
	}
      else
	{
	  /* We have not found the location part, new one will be created.  */

	  /* We have to make a copy of the shared variable.  */
	  if (var->refcount > 1)
	    var = unshare_variable (slot, var, initialized);

	  /* We track only variables whose size is <= MAX_VAR_PARTS bytes
	     thus there are at most MAX_VAR_PARTS different offsets.  */
	  gcc_assert (var->n_var_parts < MAX_VAR_PARTS
		      && (!var->n_var_parts || !dv_onepart_p (var->dv)));

	  /* We have to move the elements of array starting at index
	     inspos to the next position.  */
	  for (pos = var->n_var_parts; pos > inspos; pos--)
	    var->var_part[pos] = var->var_part[pos - 1];

	  var->n_var_parts++;
	  var->var_part[pos].offset = offset;
	  var->var_part[pos].loc_chain = NULL;
	  var->var_part[pos].cur_loc = NULL;
	}
    }

  /* Delete the location from the list.  */
  nextp = &var->var_part[pos].loc_chain;
  for (node = var->var_part[pos].loc_chain; node; node = next)
    {
      next = node->next;
      if ((REG_P (node->loc) && REG_P (loc)
	   && REGNO (node->loc) == REGNO (loc))
	  || rtx_equal_p (node->loc, loc))
	{
	  /* Save these values, to assign to the new node, before
	     deleting this one.  */
	  if (node->init > initialized)
	    initialized = node->init;
	  if (node->set_src != NULL && set_src == NULL)
	    set_src = node->set_src;
	  pool_free (loc_chain_pool, node);
	  *nextp = next;
	  break;
	}
      else
	nextp = &node->next;
    }

  /* Add the location to the beginning.  */
  node = pool_alloc (loc_chain_pool);
  node->loc = loc;
  node->init = initialized;
  node->set_src = set_src;
  node->next = var->var_part[pos].loc_chain;
  var->var_part[pos].loc_chain = node;

  /* If no location was emitted do so.  */
  if (var->var_part[pos].cur_loc == NULL)
    {
      var->var_part[pos].cur_loc = loc;
      variable_was_changed (var, set->vars);
    }
}

/* Set the part of variable's location in the dataflow set SET.  The variable
   part is specified by variable's declaration in DV and offset OFFSET and the
   part's location by LOC.  */

static void
set_variable_part (dataflow_set *set, rtx loc,
		   decl_or_value dv, HOST_WIDE_INT offset,
		   enum var_init_status initialized, rtx set_src)
{
  void **slot = htab_find_slot_with_hash (set->vars, &dv,
					  dv_htab_hash (dv), INSERT);
  set_slot_part (set, loc, slot, dv, offset, initialized, set_src);
}

/* Remove all recorded register locations for the given variable part
   from dataflow set SET, except for those that are identical to loc.
   The variable part is specified by variable's declaration DECL and
   offset OFFSET.  */

static void
clobber_variable_part (dataflow_set *set, rtx loc, decl_or_value dv,
		       HOST_WIDE_INT offset, rtx set_src)
{
  void **slot;

  if (!dv_as_opaque (dv)
      || (!dv_is_value_p (dv) && ! DECL_P (dv_as_decl (dv))))
    return;

  slot = htab_find_slot_with_hash (set->vars, &dv,
				   dv_htab_hash (dv), NO_INSERT);
  if (slot)
    {
      variable var = (variable) *slot;
      int pos = find_variable_location_part (var, offset, NULL);

      if (pos >= 0)
	{
	  location_chain node, next;

	  /* Remove the register locations from the dataflow set.  */
	  next = var->var_part[pos].loc_chain;
	  for (node = next; node; node = next)
	    {
	      next = node->next;
	      if (node->loc != loc 
		  && (!flag_var_tracking_uninit
		      || !set_src 
		      || MEM_P (set_src)
		      || !rtx_equal_p (set_src, node->set_src)))
		{
		  if (REG_P (node->loc))
		    {
		      attrs anode, anext;
		      attrs *anextp;

		      /* Remove the variable part from the register's
			 list, but preserve any other variable parts
			 that might be regarded as live in that same
			 register.  */
		      anextp = &set->regs[REGNO (node->loc)];
		      for (anode = *anextp; anode; anode = anext)
			{
			  anext = anode->next;
			  if (dv_as_opaque (anode->dv) == dv_as_opaque (dv)
			      && anode->offset == offset)
			    {
			      pool_free (attrs_pool, anode);
			      *anextp = anext;
			    }
			  else
			    anextp = &anode->next;
			}
		    }

		  delete_variable_part (set, node->loc, dv, offset);
		}
	    }
	}
    }
}

/* Delete the part of variable's location from dataflow set SET.  The variable
   part is specified by variable's declaration DECL and offset OFFSET and the
   part's location by LOC.  */

static void
delete_variable_part (dataflow_set *set, rtx loc, decl_or_value dv,
		      HOST_WIDE_INT offset)
{
  void **slot;
    
  slot = htab_find_slot_with_hash (set->vars, &dv,
				   dv_htab_hash (dv), NO_INSERT);
  if (slot)
    {
      variable var = (variable) *slot;
      int pos = find_variable_location_part (var, offset, NULL);

      if (pos >= 0)
	{
	  location_chain node, next;
	  location_chain *nextp;
	  bool changed;

	  if (var->refcount > 1)
	    {
	      /* If the variable contains the location part we have to
		 make a copy of the variable.  */
	      for (node = var->var_part[pos].loc_chain; node;
		   node = node->next)
		{
		  if ((REG_P (node->loc) && REG_P (loc)
		       && REGNO (node->loc) == REGNO (loc))
		      || rtx_equal_p (node->loc, loc))
		    {
		      enum var_init_status status = VAR_INIT_STATUS_UNKNOWN;
		      if (! flag_var_tracking_uninit)
			status = VAR_INIT_STATUS_INITIALIZED;
		      var = unshare_variable (slot, var, status);
		      break;
		    }
		}
	    }

	  /* Delete the location part.  */
	  nextp = &var->var_part[pos].loc_chain;
	  for (node = *nextp; node; node = next)
	    {
	      next = node->next;
	      if ((REG_P (node->loc) && REG_P (loc)
		   && REGNO (node->loc) == REGNO (loc))
		  || rtx_equal_p (node->loc, loc))
		{
		  pool_free (loc_chain_pool, node);
		  *nextp = next;
		  break;
		}
	      else
		nextp = &node->next;
	    }

	  /* If we have deleted the location which was last emitted
	     we have to emit new location so add the variable to set
	     of changed variables.  */
	  if (var->var_part[pos].cur_loc
	      && ((REG_P (loc)
		   && REG_P (var->var_part[pos].cur_loc)
		   && REGNO (loc) == REGNO (var->var_part[pos].cur_loc))
		  || rtx_equal_p (loc, var->var_part[pos].cur_loc)))
	    {
	      changed = true;
	      if (var->var_part[pos].loc_chain)
		var->var_part[pos].cur_loc = var->var_part[pos].loc_chain->loc;
	    }
	  else
	    changed = false;

	  if (var->var_part[pos].loc_chain == NULL)
	    {
	      var->n_var_parts--;
	      while (pos < var->n_var_parts)
		{
		  var->var_part[pos] = var->var_part[pos + 1];
		  pos++;
		}
	    }
	  if (changed)
	    variable_was_changed (var, set->vars);
	}
    }
}

/* Callback for cselib_expand_value, that looks for expressions
   holding the value in the var-tracking hash tables.  */

static rtx
vt_expand_loc_callback (rtx x, bitmap regs, int max_depth, void *data)
{
  htab_t vars = (htab_t)data;
  decl_or_value dv;
  void **slot;
  variable var;
  location_chain loc;
  rtx result;

  gcc_assert (GET_CODE (x) == VALUE);

  if (VALUE_RECURSED_INTO (x))
    return NULL;

  dv = dv_from_value (x);
  slot = htab_find_slot_with_hash (vars, &dv, dv_htab_hash (dv), NO_INSERT);

  if (!slot)
    return NULL;

  var = (variable)*slot;

  if (var->n_var_parts == 0)
    return NULL;

  gcc_assert (var->n_var_parts == 1);

  VALUE_RECURSED_INTO (x) = true;
  result = NULL;

  for (loc = var->var_part[0].loc_chain; loc; loc = loc->next)
    {
      result = cselib_expand_value_rtx_cb (loc->loc, regs, max_depth,
					   vt_expand_loc_callback, vars);
      if (result)
	break;
    }

  VALUE_RECURSED_INTO (x) = false;
  return result;
}

/* Expand VALUEs in LOC, using VARS as well as cselib's equivalence
   tables.  */

static rtx
vt_expand_loc (rtx loc, htab_t vars)
{
  if (!MAY_HAVE_DEBUG_INSNS)
    return loc;

  loc = cselib_expand_value_rtx_cb (loc, scratch_regs, 5,
				    vt_expand_loc_callback, vars);

  if (loc && MEM_P (loc))
    loc = targetm.delegitimize_address (loc);

  return loc;
}

/* Emit the NOTE_INSN_VAR_LOCATION for variable *VARP.  DATA contains
   additional parameters: WHERE specifies whether the note shall be emitted
   before of after instruction INSN.  */

static int
emit_note_insn_var_location (void **varp, void *data)
{
  variable var = (variable) *varp;
  rtx insn = ((emit_note_data *)data)->insn;
  enum emit_note_where where = ((emit_note_data *)data)->where;
  htab_t vars = ((emit_note_data *)data)->vars;
  rtx note;
  int i, j, n_var_parts;
  bool complete;
  enum var_init_status initialized = VAR_INIT_STATUS_UNINITIALIZED;
  HOST_WIDE_INT last_limit;
  tree type_size_unit;
  HOST_WIDE_INT offsets[MAX_VAR_PARTS];
  rtx loc[MAX_VAR_PARTS];
  tree decl;

  if (dv_is_value_p (var->dv))
    goto clear;

  decl = dv_as_decl (var->dv);

  gcc_assert (decl);

  if (! flag_var_tracking_uninit)
    initialized = VAR_INIT_STATUS_INITIALIZED;

  complete = true;
  last_limit = 0;
  n_var_parts = 0;
  for (i = 0; i < var->n_var_parts; i++)
    {
      enum machine_mode mode, wider_mode;
      rtx loc2;

      if (last_limit < var->var_part[i].offset)
	{
	  complete = false;
	  break;
	}
      else if (last_limit > var->var_part[i].offset)
	continue;
      offsets[n_var_parts] = var->var_part[i].offset;
      loc2 = vt_expand_loc (var->var_part[i].loc_chain->loc, vars);
      if (!loc2)
	{
	  complete = false;
	  continue;
	}
      loc[n_var_parts] = loc2;
      mode = GET_MODE (loc[n_var_parts]);
      initialized = var->var_part[i].loc_chain->init;
      last_limit = offsets[n_var_parts] + GET_MODE_SIZE (mode);

      /* Attempt to merge adjacent registers or memory.  */
      wider_mode = GET_MODE_WIDER_MODE (mode);
      for (j = i + 1; j < var->n_var_parts; j++)
	if (last_limit <= var->var_part[j].offset)
	  break;
      if (j < var->n_var_parts
	  && wider_mode != VOIDmode
	  && (loc2 = vt_expand_loc (var->var_part[j].loc_chain->loc, vars))
	  && GET_CODE (loc[n_var_parts]) == GET_CODE (loc2)
	  && mode == GET_MODE (loc2)
	  && last_limit == var->var_part[j].offset)
	{
	  rtx new_loc = NULL;

	  if (REG_P (loc[n_var_parts])
	      && hard_regno_nregs[REGNO (loc[n_var_parts])][mode] * 2
		 == hard_regno_nregs[REGNO (loc[n_var_parts])][wider_mode]
	      && end_hard_regno (mode, REGNO (loc[n_var_parts]))
		 == REGNO (loc2))
	    {
	      if (! WORDS_BIG_ENDIAN && ! BYTES_BIG_ENDIAN)
		new_loc = simplify_subreg (wider_mode, loc[n_var_parts],
					   mode, 0);
	      else if (WORDS_BIG_ENDIAN && BYTES_BIG_ENDIAN)
		new_loc = simplify_subreg (wider_mode, loc2, mode, 0);
	      if (new_loc)
		{
		  if (!REG_P (new_loc)
		      || REGNO (new_loc) != REGNO (loc[n_var_parts]))
		    new_loc = NULL;
		  else
		    REG_ATTRS (new_loc) = REG_ATTRS (loc[n_var_parts]);
		}
	    }
	  else if (MEM_P (loc[n_var_parts])
		   && GET_CODE (XEXP (loc2, 0)) == PLUS
		   && GET_CODE (XEXP (XEXP (loc2, 0), 0)) == REG
		   && GET_CODE (XEXP (XEXP (loc2, 0), 1)) == CONST_INT)
	    {
	      if ((GET_CODE (XEXP (loc[n_var_parts], 0)) == REG
		   && rtx_equal_p (XEXP (loc[n_var_parts], 0),
				   XEXP (XEXP (loc2, 0), 0))
		   && INTVAL (XEXP (XEXP (loc2, 0), 1))
		      == GET_MODE_SIZE (mode))
		  || (GET_CODE (XEXP (loc[n_var_parts], 0)) == PLUS
		      && GET_CODE (XEXP (XEXP (loc[n_var_parts], 0), 1))
			 == CONST_INT
		      && rtx_equal_p (XEXP (XEXP (loc[n_var_parts], 0), 0),
				      XEXP (XEXP (loc2, 0), 0))
		      && INTVAL (XEXP (XEXP (loc[n_var_parts], 0), 1))
			 + GET_MODE_SIZE (mode)
			 == INTVAL (XEXP (XEXP (loc2, 0), 1))))
		new_loc = adjust_address_nv (loc[n_var_parts],
					     wider_mode, 0);
	    }

	  if (new_loc)
	    {
	      loc[n_var_parts] = new_loc;
	      mode = wider_mode;
	      last_limit = offsets[n_var_parts] + GET_MODE_SIZE (mode);
	      i = j;
	    }
	}
      ++n_var_parts;
    }
  type_size_unit = TYPE_SIZE_UNIT (TREE_TYPE (decl));
  if ((unsigned HOST_WIDE_INT) last_limit < TREE_INT_CST_LOW (type_size_unit))
    complete = false;

  if (where == EMIT_NOTE_AFTER_INSN)
    note = emit_note_after (NOTE_INSN_VAR_LOCATION, insn);
  else
    note = emit_note_before (NOTE_INSN_VAR_LOCATION, insn);

  if (! flag_var_tracking_uninit)
    initialized = VAR_INIT_STATUS_INITIALIZED;

  if (!complete)
    {
      NOTE_VAR_LOCATION (note) = gen_rtx_VAR_LOCATION (VOIDmode, decl,
						       NULL_RTX, (int) initialized);
    }
  else if (n_var_parts == 1)
    {
      rtx expr_list
	= gen_rtx_EXPR_LIST (VOIDmode, loc[0], GEN_INT (offsets[0]));

      NOTE_VAR_LOCATION (note) = gen_rtx_VAR_LOCATION (VOIDmode, decl,
						       expr_list, 
						       (int) initialized);
    }
  else if (n_var_parts)
    {
      rtx parallel;

      for (i = 0; i < n_var_parts; i++)
	loc[i]
	  = gen_rtx_EXPR_LIST (VOIDmode, loc[i], GEN_INT (offsets[i]));

      parallel = gen_rtx_PARALLEL (VOIDmode,
				   gen_rtvec_v (n_var_parts, loc));
      NOTE_VAR_LOCATION (note) = gen_rtx_VAR_LOCATION (VOIDmode, decl,
						       parallel, 
						       (int) initialized);
    }

 clear:
  htab_clear_slot (changed_variables, varp);

  /* When there are no location parts the variable has been already
     removed from hash table and a new empty variable was created.
     Free the empty variable.  */
  if (var->n_var_parts == 0)
    {
      pool_free (dv_pool (var->dv), var);
    }

  /* Continue traversing the hash table.  */
  return 1;
}

/* If *LOC is a VALUE present in changed_variables, set the bool DATA
   points to and stop searching.  */

static int
check_changed_value (rtx *loc, void *data)
{
  rtx x = *loc;
  bool *changedp = (bool *)data;
  decl_or_value dv;

  if (GET_CODE (x) != VALUE)
    return 0;

  dv = dv_from_value (x);
  if (!htab_find_slot_with_hash (changed_variables, &dv, dv_htab_hash (dv),
				 NO_INSERT))
    return 0;

  *changedp = true;
  return 1;
}

/* Mark a variable or a value that refers to values that have
   changed.  */

static int
check_changed_var (void **slot, void *data)
{
  variable var = (variable) *slot;
  bool *changedp = (bool *)data;
  location_chain loc;
  bool changed = false;

  if (var->n_var_parts != 1 || !dv_onepart_p (var->dv))
    return 1;

  if (htab_find_slot_with_hash (changed_variables, &var->dv,
				dv_htab_hash (var->dv), NO_INSERT))
    return 1;

  if (!dv_is_value_p (var->dv)
      && !var_debug_value_for_decl (dv_as_decl (var->dv)))
    return 1;

  for (loc = var->var_part[0].loc_chain; loc && !changed; loc = loc->next)
    for_each_rtx (&loc->loc, check_changed_value, &changed);

  /* ??? Is this really necessary?  Maybe the local table is redundant
     with the cselib table.  */
  if (!changed && dv_is_value_p (var->dv))
    {
      struct elt_loc_list *l;

      for (l = CSELIB_VAL_PTR (dv_as_value (var->dv))->locs;
	   l && !changed; l = l->next)
	for_each_rtx (&l->loc, check_changed_value, &changed);
    }

  if (changed)
    {
      variable_was_changed (var, NULL);
      *changedp = true;
    }

  return 1;
}

/* Emit NOTE_INSN_VAR_LOCATION note for each variable from a chain
   CHANGED_VARIABLES and delete this chain.  WHERE specifies whether the notes
   shall be emitted before of after instruction INSN.  */

static void
emit_notes_for_changes (rtx insn, enum emit_note_where where, htab_t vars)
{
  emit_note_data data;

  if (MAY_HAVE_DEBUG_INSNS)
    {
      bool more_changed;

      /* This is very inefficient.  Back-links from values to other
	 values referencing them would make things far more efficient,
	 but it's not clear that the additional memory use is worth
	 it.  */
      do
	{
	  more_changed = false;
	  htab_traverse (vars, check_changed_var, &more_changed);
	}
      while (more_changed);
    }

  data.insn = insn;
  data.where = where;
  data.vars = vars;
  htab_traverse (changed_variables, emit_note_insn_var_location, &data);
}

/* Add variable *SLOT to the chain CHANGED_VARIABLES if it differs from the
   same variable in hash table DATA or is not there at all.  */

static int
emit_notes_for_differences_1 (void **slot, void *data)
{
  htab_t new_vars = (htab_t) data;
  variable old_var, new_var;

  old_var = (variable) *slot;
  new_var = htab_find_with_hash (new_vars, &old_var->dv,
				 dv_htab_hash (old_var->dv));

  if (!new_var)
    {
      /* Variable has disappeared.  */
      variable empty_var;

      empty_var = pool_alloc (dv_pool (old_var->dv));
      empty_var->dv = old_var->dv;
      empty_var->refcount = 1;
      empty_var->n_var_parts = 0;
      variable_was_changed (empty_var, NULL);
    }
  else if (variable_different_p (old_var, new_var, true))
    {
      variable_was_changed (new_var, NULL);
    }

  /* Continue traversing the hash table.  */
  return 1;
}

/* Add variable *SLOT to the chain CHANGED_VARIABLES if it is not in hash
   table DATA.  */

static int
emit_notes_for_differences_2 (void **slot, void *data)
{
  htab_t old_vars = (htab_t) data;
  variable old_var, new_var;

  new_var = (variable) *slot;
  old_var = htab_find_with_hash (old_vars, &new_var->dv,
				 dv_htab_hash (new_var->dv));
  if (!old_var)
    {
      /* Variable has appeared.  */
      variable_was_changed (new_var, NULL);
    }

  /* Continue traversing the hash table.  */
  return 1;
}

/* Emit notes before INSN for differences between dataflow sets OLD_SET and
   NEW_SET.  */

static void
emit_notes_for_differences (rtx insn, dataflow_set *old_set,
			    dataflow_set *new_set)
{
  htab_traverse (old_set->vars, emit_notes_for_differences_1, new_set->vars);
  htab_traverse (new_set->vars, emit_notes_for_differences_2, old_set->vars);
  emit_notes_for_changes (insn, EMIT_NOTE_BEFORE_INSN, new_set->vars);
}

/* Emit the notes for changes of location parts in the basic block BB.  */

static void
emit_notes_in_bb (basic_block bb)
{
  int i;
  dataflow_set set;

  dataflow_set_init (&set, htab_elements (VTI (bb)->in.vars) + 3);
  dataflow_set_copy (&set, &VTI (bb)->in);

  for (i = 0; i < VTI (bb)->n_mos; i++)
    {
      rtx insn = VTI (bb)->mos[i].insn;

      switch (VTI (bb)->mos[i].type)
	{
	  case MO_CALL:
	    dataflow_set_clear_at_call (&set);
	    emit_notes_for_changes (insn, EMIT_NOTE_AFTER_INSN, set.vars);
	    break;

	  case MO_USE:
	    {
	      rtx loc = VTI (bb)->mos[i].u.loc;
	      enum var_init_status status = VAR_INIT_STATUS_UNINITIALIZED;

	      if (! flag_var_tracking_uninit)
		status = VAR_INIT_STATUS_INITIALIZED;
	      if (GET_CODE (loc) == REG)
		var_reg_set (&set, loc, status, NULL);
	      else
		var_mem_set (&set, loc, status, NULL);

	      emit_notes_for_changes (insn, EMIT_NOTE_AFTER_INSN, set.vars);
	    }
	    break;

	  case MO_VAL_LOC:
	    {
	      rtx loc = VTI (bb)->mos[i].u.loc;
	      rtx val, vloc;
	      tree var;

	      if (GET_CODE (loc) == CONCAT)
		{
		  val = XEXP (loc, 0);
		  vloc = XEXP (loc, 1);
		}
	      else
		{
		  val = NULL_RTX;
		  vloc = loc;
		}

	      var = PAT_VAR_LOCATION_DECL (vloc);

	      clobber_variable_part (&set, NULL_RTX,
				     dv_from_decl (var), 0, NULL_RTX);
	      if (val)
		{
		  if (VAL_NEEDS_RESOLUTION (loc))
		    val_resolve (&set, val, PAT_VAR_LOCATION_LOC (vloc), insn);
		  set_variable_part (&set, val, dv_from_decl (var), 0,
				     VAR_INIT_STATUS_INITIALIZED, NULL_RTX);
		}

#if 0 /* ??? Huh? */
	      for (next = NEXT_INSN (insn);
		   next && BLOCK_FOR_INSN (insn) == BLOCK_FOR_INSN (next);
		   next = NEXT_INSN (next))
		if (DEBUG_INSN_P (next))
		  insn = next;
		else if (!NOTE_P (next))
		  break;
#endif

	      emit_notes_for_changes (insn, EMIT_NOTE_AFTER_INSN, set.vars);
	    }
	    break;

	  case MO_VAL_USE:
	    {
	      rtx loc = VTI (bb)->mos[i].u.loc;
	      rtx val, vloc, uloc;

	      vloc = uloc = XEXP (loc, 1);
	      val = XEXP (loc, 0);

	      if (GET_CODE (val) == CONCAT)
		{
		  vloc = XEXP (val, 1);
		  val = XEXP (val, 0);
		}

	      if (VAL_NEEDS_RESOLUTION (loc))
		val_resolve (&set, val, vloc, insn);

	      if (VAL_HOLDS_TRACK_EXPR (loc))
		{
		  enum var_init_status status = VAR_INIT_STATUS_UNINITIALIZED;

		  if (! flag_var_tracking_uninit)
		    status = VAR_INIT_STATUS_INITIALIZED;

		  if (GET_CODE (uloc) == REG)
		    var_reg_set (&set, uloc, status, NULL);
		  else if (GET_CODE (uloc) == MEM)
		    var_mem_set (&set, uloc, status, NULL);
		}

	      emit_notes_for_changes (insn, EMIT_NOTE_BEFORE_INSN, set.vars);
	    }
	    break;

	  case MO_VAL_SET:
	    {
	      rtx loc = VTI (bb)->mos[i].u.loc;
	      rtx val, vloc, uloc;

	      vloc = uloc = XEXP (loc, 1);
	      val = XEXP (loc, 0);

	      if (GET_CODE (val) == CONCAT)
		{
		  vloc = XEXP (val, 1);
		  val = XEXP (val, 0);
		}

	      if (GET_CODE (vloc) == SET)
		{
		  rtx vsrc = SET_SRC (vloc);

		  gcc_assert (val != vsrc);
		  gcc_assert (vloc == uloc || VAL_NEEDS_RESOLUTION (loc));

		  vloc = SET_DEST (vloc);

		  if (VAL_NEEDS_RESOLUTION (loc))
		    val_resolve (&set, val, vsrc, insn);
		}
	      else if (VAL_NEEDS_RESOLUTION (loc))
		{
		  gcc_assert (GET_CODE (uloc) == SET
			      && GET_CODE (SET_SRC (uloc)) == REG);
		  val_resolve (&set, val, SET_SRC (uloc), insn);
		}

	      if (VAL_HOLDS_TRACK_EXPR (loc))
		{
		  if (VAL_EXPR_IS_CLOBBERED (loc))
		    {
		      if (REG_P (uloc))
			var_reg_delete (&set, uloc, true);
		      else if (MEM_P (uloc))
			var_mem_delete (&set, uloc, true);
		    }
		  else
		    {
		      bool copied_p = VAL_EXPR_IS_COPIED (loc);
		      rtx set_src = NULL;
		      enum var_init_status status = VAR_INIT_STATUS_INITIALIZED;

		      if (GET_CODE (uloc) == SET)
			{
			  set_src = SET_SRC (uloc);
			  uloc = SET_DEST (uloc);
			}

		      if (copied_p)
			{
			  status = find_src_status (&set, set_src);

			  set_src = find_src_set_src (&set, set_src);
			}

		      if (REG_P (uloc))
			var_reg_delete_and_set (&set, uloc, !copied_p,
						status, set_src);
		      else if (MEM_P (uloc))
			var_mem_delete_and_set (&set, uloc, !copied_p,
						status, set_src);
		    }
		}
	      else if (REG_P (uloc))
		var_regno_delete (&set, REGNO (uloc));

	      val_store (&set, val, vloc, insn);

	      emit_notes_for_changes (NEXT_INSN (insn), EMIT_NOTE_BEFORE_INSN,
				      set.vars);
	    }
	    break;

	  case MO_SET:
	    {
	      rtx loc = VTI (bb)->mos[i].u.loc;
	      rtx set_src = NULL;

	      if (GET_CODE (loc) == SET)
		{
		  set_src = SET_SRC (loc);
		  loc = SET_DEST (loc);
		}

	      if (REG_P (loc))
		var_reg_delete_and_set (&set, loc, true, VAR_INIT_STATUS_INITIALIZED, 
					set_src);
	      else
		var_mem_delete_and_set (&set, loc, true, VAR_INIT_STATUS_INITIALIZED, 
					set_src);

	      emit_notes_for_changes (NEXT_INSN (insn), EMIT_NOTE_BEFORE_INSN,
				      set.vars);
	    }
	    break;

	  case MO_COPY:
	    {
	      rtx loc = VTI (bb)->mos[i].u.loc;
	      enum var_init_status src_status;
	      rtx set_src = NULL;

	      if (GET_CODE (loc) == SET)
		{
		  set_src = SET_SRC (loc);
		  loc = SET_DEST (loc);
		}

	      src_status = find_src_status (&set, set_src);
	      set_src = find_src_set_src (&set, set_src);

	      if (REG_P (loc))
		var_reg_delete_and_set (&set, loc, false, src_status, set_src);
	      else
		var_mem_delete_and_set (&set, loc, false, src_status, set_src);

	      emit_notes_for_changes (NEXT_INSN (insn), EMIT_NOTE_BEFORE_INSN,
				      set.vars);
	    }
	    break;

	  case MO_USE_NO_VAR:
	    {
	      rtx loc = VTI (bb)->mos[i].u.loc;

	      if (REG_P (loc))
		var_reg_delete (&set, loc, false);
	      else
		var_mem_delete (&set, loc, false);

	      emit_notes_for_changes (insn, EMIT_NOTE_AFTER_INSN, set.vars);
	    }
	    break;

	  case MO_CLOBBER:
	    {
	      rtx loc = VTI (bb)->mos[i].u.loc;

	      if (REG_P (loc))
		var_reg_delete (&set, loc, true);
	      else
		var_mem_delete (&set, loc, true);

	      emit_notes_for_changes (NEXT_INSN (insn), EMIT_NOTE_BEFORE_INSN,
				      set.vars);
	    }
	    break;

	  case MO_ADJUST:
	    set.stack_adjust += VTI (bb)->mos[i].u.adjust;
	    break;
	}
    }
  dataflow_set_destroy (&set);
}

/* Emit notes for the whole function.  */

static void
vt_emit_notes (void)
{
  basic_block bb;
  dataflow_set *last_out;
  dataflow_set empty;

  gcc_assert (!htab_elements (changed_variables));

  /* Enable emitting notes by functions (mainly by set_variable_part and
     delete_variable_part).  */
  emit_notes = true;

  dataflow_set_init (&empty, 7);
  last_out = &empty;

  FOR_EACH_BB (bb)
    {
      /* Emit the notes for changes of variable locations between two
	 subsequent basic blocks.  */
      emit_notes_for_differences (BB_HEAD (bb), last_out, &VTI (bb)->in);

      /* Emit the notes for the changes in the basic block itself.  */
      emit_notes_in_bb (bb);

      last_out = &VTI (bb)->out;
    }
  dataflow_set_destroy (&empty);
  emit_notes = false;
}

/* If there is a declaration and offset associated with register/memory RTL
   assign declaration to *DECLP and offset to *OFFSETP, and return true.  */

static bool
vt_get_decl_and_offset (rtx rtl, tree *declp, HOST_WIDE_INT *offsetp)
{
  if (REG_P (rtl))
    {
      if (REG_ATTRS (rtl))
	{
	  *declp = REG_EXPR (rtl);
	  *offsetp = REG_OFFSET (rtl);
	  return true;
	}
    }
  else if (MEM_P (rtl))
    {
      if (MEM_ATTRS (rtl))
	{
	  *declp = MEM_EXPR (rtl);
	  *offsetp = INT_MEM_OFFSET (rtl);
	  return true;
	}
    }
  return false;
}

/* Insert function parameters to IN and OUT sets of ENTRY_BLOCK.  */

static void
vt_add_function_parameters (void)
{
  tree parm;
  
  for (parm = DECL_ARGUMENTS (current_function_decl);
       parm; parm = TREE_CHAIN (parm))
    {
      rtx decl_rtl = DECL_RTL_IF_SET (parm);
      rtx incoming = DECL_INCOMING_RTL (parm);
      tree decl;
      enum machine_mode mode;
      HOST_WIDE_INT offset;
      dataflow_set *out;
      decl_or_value dv;

      if (TREE_CODE (parm) != PARM_DECL)
	continue;

      if (!DECL_NAME (parm))
	continue;

      if (!decl_rtl || !incoming)
	continue;

      if (GET_MODE (decl_rtl) == BLKmode || GET_MODE (incoming) == BLKmode)
	continue;

      if (!vt_get_decl_and_offset (incoming, &decl, &offset))
	{
	  if (!vt_get_decl_and_offset (decl_rtl, &decl, &offset))
	    continue;
	  offset += byte_lowpart_offset (GET_MODE (incoming),
					 GET_MODE (decl_rtl));
	}

      if (!decl)
	continue;

      gcc_assert (parm == decl);

      if (!track_loc_p (incoming, parm, offset, false, &mode, &offset))
	continue;

      out = &VTI (ENTRY_BLOCK_PTR)->out;

      dv = dv_from_decl (parm);

      if (var_debug_value_for_decl (parm)
	  /* We can't deal with these right now, because this kind of
	     variable is single-part.  ??? We could handle parallels
	     that describe multiple locations for the same single
	     value, but ATM we don't.  */
	  && GET_CODE (incoming) != PARALLEL)
	{
	  cselib_val *val = cselib_lookup (var_lowpart (mode, incoming),
					   mode, true);

	  /* ??? Float-typed values in memory are not handled by
	     cselib.  */
	  if (val)
	    {
	      cselib_preserve_value (val);
	      set_variable_part (out, val->val_rtx, dv, offset,
				 VAR_INIT_STATUS_INITIALIZED, NULL);
	      dv = dv_from_value (val->val_rtx);
	    }
	}

      if (REG_P (incoming))
	{
	  incoming = var_lowpart (mode, incoming);
	  gcc_assert (REGNO (incoming) < FIRST_PSEUDO_REGISTER);
	  attrs_list_insert (&out->regs[REGNO (incoming)], dv, offset,
			     incoming);
	  set_variable_part (out, incoming, dv, offset,
			     VAR_INIT_STATUS_INITIALIZED, NULL);
	}
      else if (MEM_P (incoming))
	{
	  incoming = var_lowpart (mode, incoming);
	  set_variable_part (out, incoming, dv, offset,
			     VAR_INIT_STATUS_INITIALIZED, NULL);
	}
    }

  if (MAY_HAVE_DEBUG_INSNS)
    {
      cselib_preserve_only_values (true);
      cselib_reset_table_with_next_value (cselib_get_next_unknown_value ());
    }

}

/* Allocate and initialize the data structures for variable tracking
   and parse the RTL to get the micro operations.  */

static void
vt_initialize (void)
{
  basic_block bb;

  alloc_aux_for_blocks (sizeof (struct variable_tracking_info_def));

  if (MAY_HAVE_DEBUG_INSNS)
    {
      cselib_init (true);
      scratch_regs = BITMAP_ALLOC (NULL);
      valvar_pool = create_alloc_pool ("small variable_def pool",
				       sizeof (struct variable_def), 256);
    }
  else
    {
      scratch_regs = NULL;
      valvar_pool = NULL;
    }

  FOR_EACH_BB (bb)
    {
      rtx insn;
      HOST_WIDE_INT pre, post = 0;
      int count;
      unsigned int next_value_before = cselib_get_next_unknown_value ();
      unsigned int next_value_after = next_value_before;

      if (MAY_HAVE_DEBUG_INSNS)
	{
	  cselib_record_sets_hook = count_with_sets;
	  if (dump_file)
	    fprintf (dump_file, "first value: %i\n",
		     cselib_get_next_unknown_value ());
	}

      /* Count the number of micro operations.  */
      VTI (bb)->n_mos = 0;
      for (insn = BB_HEAD (bb); insn != NEXT_INSN (BB_END (bb));
	   insn = NEXT_INSN (insn))
	{
	  if (INSN_P (insn))
	    {
	      if (!frame_pointer_needed)
		{
		  insn_stack_adjust_offset_pre_post (insn, &pre, &post);
		  if (pre)
		    {
		      VTI (bb)->n_mos++;
		      if (dump_file)
			log_op_type (GEN_INT (pre), bb, insn,
				     MO_ADJUST, dump_file);
		    }
		  if (post)
		    {
		      VTI (bb)->n_mos++;
		      if (dump_file)
			log_op_type (GEN_INT (post), bb, insn,
				     MO_ADJUST, dump_file);
		    }
		}
	      cselib_hook_called = false;
	      if (MAY_HAVE_DEBUG_INSNS)
		{
		  cselib_process_insn (insn);
		  if (dump_file)
		    {
		      print_rtl_single (dump_file, insn);
		      dump_cselib_table (dump_file);
		    }
		}
	      if (!cselib_hook_called)
		count_with_sets (insn, 0, 0);
	      if (CALL_P (insn))
		{
		  VTI (bb)->n_mos++;
		  if (dump_file)
		    log_op_type (PATTERN (insn), bb, insn,
				 MO_CALL, dump_file);
		}
	    }
	}

      count = VTI (bb)->n_mos;

      if (MAY_HAVE_DEBUG_INSNS)
	{
	  cselib_preserve_only_values (false);
	  next_value_after = cselib_get_next_unknown_value ();
	  cselib_reset_table_with_next_value (next_value_before);
	  cselib_record_sets_hook = add_with_sets;
	  if (dump_file)
	    fprintf (dump_file, "first value: %i\n",
		     cselib_get_next_unknown_value ());
	}

      /* Add the micro-operations to the array.  */
      VTI (bb)->mos = XNEWVEC (micro_operation, VTI (bb)->n_mos);
      VTI (bb)->n_mos = 0;
      for (insn = BB_HEAD (bb); insn != NEXT_INSN (BB_END (bb));
	   insn = NEXT_INSN (insn))
	{
	  if (INSN_P (insn))
	    {
	      if (!frame_pointer_needed)
		{
		  insn_stack_adjust_offset_pre_post (insn, &pre, &post);
		  if (pre)
		    {
		      micro_operation *mo = VTI (bb)->mos + VTI (bb)->n_mos++;

		      mo->type = MO_ADJUST;
		      mo->u.adjust = pre;
		      mo->insn = insn;

		      if (dump_file)
			log_op_type (PATTERN (insn), bb, insn,
				     MO_ADJUST, dump_file);
		    }
		}

	      cselib_hook_called = false;
	      if (MAY_HAVE_DEBUG_INSNS)
		{
		  cselib_process_insn (insn);
		  if (dump_file)
		    {
		      print_rtl_single (dump_file, insn);
		      dump_cselib_table (dump_file);
		    }
		}
	      if (!cselib_hook_called)
		add_with_sets (insn, 0, 0);

	      if (!frame_pointer_needed && post)
		{
		  micro_operation *mo = VTI (bb)->mos + VTI (bb)->n_mos++;

		  mo->type = MO_ADJUST;
		  mo->u.adjust = post;
		  mo->insn = insn;

		  if (dump_file)
		    log_op_type (PATTERN (insn), bb, insn,
				 MO_ADJUST, dump_file);
		}
	    }
	}
      gcc_assert (count == VTI (bb)->n_mos);
      if (MAY_HAVE_DEBUG_INSNS)
	{
	  cselib_preserve_only_values (true);
	  gcc_assert (next_value_after == cselib_get_next_unknown_value ());
	  cselib_reset_table_with_next_value (next_value_after);
	  cselib_record_sets_hook = NULL;
	}
    }

  /* Init the IN and OUT sets.  */
  FOR_ALL_BB (bb)
    {
      VTI (bb)->visited = false;
      dataflow_set_init (&VTI (bb)->in, 7);
      dataflow_set_init (&VTI (bb)->out, 7);
    }

  attrs_pool = create_alloc_pool ("attrs_def pool",
				  sizeof (struct attrs_def), 1024);
  var_pool = create_alloc_pool ("variable_def pool",
				sizeof (struct variable_def)
				+ (MAX_VAR_PARTS - 1)
				* sizeof (((variable)NULL)->var_part[0]), 64);
  loc_chain_pool = create_alloc_pool ("location_chain_def pool",
				      sizeof (struct location_chain_def),
				      1024);
  changed_variables = htab_create (10, variable_htab_hash, variable_htab_eq,
				   NULL);
  vt_add_function_parameters ();
}

/* Get rid of all debug insns from the insn stream.  */

static void
delete_debug_insns (void)
{
  basic_block bb;
  rtx insn, next;

  if (!MAY_HAVE_DEBUG_INSNS)
    return;

  FOR_EACH_BB (bb)
    {
      FOR_BB_INSNS_SAFE (bb, insn, next)
	if (DEBUG_INSN_P (insn))
	  delete_insn (insn);
    }
}

/* Run a fast, BB-local only version of var tracking, to take care of
   information that we don't do global analysis on, such that not all
   information is lost.  If SKIPPED holds, we're skipping the global
   pass entirely, so we should try to use information it would have
   handled as well..  */

static void
vt_debug_insns_local (bool skipped ATTRIBUTE_UNUSED)
{
  /* ??? Just skip it all for now.  */
  delete_debug_insns ();
}

/* Free the data structures needed for variable tracking.  */

static void
vt_finalize (void)
{
  basic_block bb;

  FOR_EACH_BB (bb)
    {
      free (VTI (bb)->mos);
    }

  FOR_ALL_BB (bb)
    {
      dataflow_set_destroy (&VTI (bb)->in);
      dataflow_set_destroy (&VTI (bb)->out);
    }
  free_aux_for_blocks ();
  free_alloc_pool (attrs_pool);
  free_alloc_pool (var_pool);
  free_alloc_pool (loc_chain_pool);
  htab_delete (changed_variables);

  if (MAY_HAVE_DEBUG_INSNS)
    {
      free_alloc_pool (valvar_pool);
      cselib_finish ();
      BITMAP_FREE (scratch_regs);
      scratch_regs = NULL;
    }
}

/* The entry point to variable tracking pass.  */

unsigned int
variable_tracking_main (void)
{
  if (n_basic_blocks > 500 && n_edges / n_basic_blocks >= 20)
    {
      vt_debug_insns_local (true);
      return 0;
    }

  mark_dfs_back_edges ();
  vt_initialize ();
  if (!frame_pointer_needed)
    {
      if (!vt_stack_adjustments ())
	{
	  vt_finalize ();
	  vt_debug_insns_local (true);
	  return 0;
	}
    }

  vt_find_locations ();
  vt_emit_notes ();

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      dump_dataflow_sets ();
      dump_flow_info (dump_file, dump_flags);
    }

  vt_finalize ();
  vt_debug_insns_local (false);
  return 0;
}

static bool
gate_handle_var_tracking (void)
{
  return (flag_var_tracking);
}



struct tree_opt_pass pass_variable_tracking =
{
  "vartrack",                           /* name */
  gate_handle_var_tracking,             /* gate */
  variable_tracking_main,               /* execute */
  NULL,                                 /* sub */
  NULL,                                 /* next */
  0,                                    /* static_pass_number */
  TV_VAR_TRACKING,                      /* tv_id */
  0,                                    /* properties_required */
  0,                                    /* properties_provided */
  0,                                    /* properties_destroyed */
  0,                                    /* todo_flags_start */
  TODO_dump_func | TODO_verify_rtl_sharing,/* todo_flags_finish */
  'V'                                   /* letter */
};


/* Variable tracking routines for the GNU compiler.
   Copyright (C) 2002, 2003 Free Software Foundation, Inc.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   GCC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING.  If not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA.  */

/* This file contains the variable tracking pass.  It computes where
   a variable is located (which register or where in memory) at each position
   in instruction stream and emits notes describing the locations.
   Debug information is finally generated from these notes (this code is/will be
   located in files generating debug information).

   How the variable tracking pass works?

   First, it scans RTL code for uses and stores (register/memory references in
   instructions) separately for each basic block and saves them to an array (of
   struct location_def).  Uses and stores of one instruction are ordered so that
   use < set < clobber.

   Then, a (forward) dataflow analysis is performed to find out how locations
   of variables change through code and to propagate the variable locations
   along control flow graph.  Hybrid Search Algorithm is used for dataflow
   analysis (as well as in df.c).
   The IN and OUT sets for basic blocks consist of a link list for each
   physical register and a hash table of link lists for memory references.
   The link lists should be pretty short so it is a good data structure here.
   The IN set for basic block BB is computed as a union of OUT sets of BB's
   predecessors, the OUT set for BB is computed from the IN set for BB and
   the location references in BB (from array of locations created in the first
   step).
   REG_EXPR and MEM_EXPR are used to find out which variable is associated
   with a given register/memory.
   For a register, the link list contains the list of triplets
   (loc, decl, offset) called "attributes" (loc is the register RTX,
   decl and offset are from REG_ATTRS (loc)) describing variable parts which are
   currently in a given register.  There may be more than one such a variable
   part in a register, for example in the following example, register allocator
   may assign same register to variables A and B, and both of them are stored in
   the same register in CODE:

     if (cond)
       set A;
     else
       set B;
     CODE;
     if (cond)
       use A;
     else
       use B;

   For a memory, the link list are similar, one link list contains attributes
   for the same decl (they may have different offsets).

   Finally, the NOTE_INSN_VAR_LOCATION notes describing the variable locations
   are emitted to appropriate positions in RTL code.  Each such a note describes
   the location of one variable at the point in instruction stream where the
   note is.  There is no need to emit a note for each variable before each
   instruction, we only emit these notes where the location of variable changes
   (this means that we also emit notes for changes between the OUT set of the
   previous block and the IN set of the current block).

   The notes consist of two parts:
   1. the declaration (from REG_EXPR or MEM_EXPR)
   2. the location - it is either a simple register/memory reference (for
      simple variables, for example int),
      or a parallel of register/memory references (for a large variables
      which consist of several parts, for example long long).


   References:
   "Implementation Techniques for Efficient Data-Flow Analysis of Large
   Programs"
   D. C. Atkinson, W. G. Griswold.
   http://citeseer.nj.nec.com/atkinson01implementation.html

*/

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "tree.h"
#include "basic-block.h"
#include "output.h"
#include "insn-config.h"
#include "reload.h"
#include "sbitmap.h"
#include "alloc-pool.h"
#include "fibheap.h"
#include "hashtab.h"

/* The purpose which the location (REG or MEM) has in RTL.  */
enum location_type
{
  LT_USE,	/* Location is used in instruction.  */
  LT_SET,	/* Location is set by instruction.  */
  LT_CLOBBER	/* Location is clobbered by instruction.  */
};

/* Where shall the note be emitted?  BEFORE or AFTER the instruction.  */
enum where_emit_note
{
  EMIT_NOTE_BEFORE_INSN,
  EMIT_NOTE_AFTER_INSN
};

/* Structure for remembering the location (register or memory).  */
typedef struct location_def
{
  /* The location itself.  */
  rtx loc;

  /* The instruction that the location is in.  */
  rtx insn;

  /* Type of the location.  */
  enum location_type type;
} location;

/* Structure for passing some other parameters to function
   emit_note_if_var_changed.  */
typedef struct emit_note_data_def
{
  /* The instruction that the note will be emitted before/after.  */
  rtx insn;

  /* Where the note will be emitted (before/after insn)?  */
  enum where_emit_note where;
} emit_note_data;

/* The following structure contains needed information from REG_ATTRS and
   MEM_ATTRS and is common for REGs and MEMs.
   There is a separate link list of these structures for each (physical)
   register for attributes of variables which are stored in register
   and separate link list for each DECL_RTL (MEM_EXPR (mem)) for MEMs,
   so the link lists are pretty short (usually 1 or 2 elements) and thus
   link list is the best data structure.  */
typedef struct attrs_def
{
  /* Pointer to next member of the list.  */
  struct attrs_def *next;

  /* The rtx of register/memory.  */
  rtx loc;

  /* The declaration corresponding to LOC.  */
  tree decl;

  /* Offset from start of DECL.  */
  HOST_WIDE_INT offset;
} *attrs;

/* The structure (one for each basic block) containing the information
   needed for dataflow analysis (variable tracking).  */
typedef struct var_tracking_info_def
{
  /* Number of locations in the LOCS array.  */
  int n_locs;

  /* The array of locations.  */
  location *locs;

  /* Input attributes for registers (lists of attrs).  */
  attrs in[FIRST_PSEUDO_REGISTER];

  /* Input attributes for memory.  */
  htab_t mem_in;

  /* Output attributes for registers.  */
  attrs out[FIRST_PSEUDO_REGISTER];

  /* Output attributes for memory.  */
  htab_t mem_out;
} *var_tracking_info;

/* Structure for description of one part of variable location.  */
typedef struct location_part_def
{
  /* The location (REG or MEM).  */
  rtx loc;

  /* The offset in the variable.  */
  HOST_WIDE_INT offset;

  /* Shall this struct be deleted?  */
  bool delete_p;
} location_part;

/* Maximum number of location parts.  */
#define MAX_LOC_PARTS 16

/* Structure for description where the variable is located.  */
typedef struct variable_def
{
  /* The declaration of the variable.  */
  tree decl;

  /* Number of locations.  */
  int n_location_parts;

  /* The locations.  */
  location_part location_part[MAX_LOC_PARTS];

  /* Have the variable location parts changed because of expressions in current
     instruction?  */
  bool changed;
} *variable;

/* The hash table of STRUCT VARIABLE_DEF.  */
static htab_t variable_htab;

/* Hash function for DECL for MEM_IN and MEM_OUT sets.  */
#define MEM_HASH_VAL(decl) ((size_t) (decl))

/* Hash function for DECL for VARIABLE_HTAB.  */
#define VARIABLE_HASH_VAL(decl) ((size_t) (decl))

/* Get the pointer to the BB's information specific to var-tracking pass.  */
#define VTI(BB) ((var_tracking_info) (BB)->aux)

/* Alloc pool for struct attrs_def.  */
static alloc_pool attrs_pool;

/* Alloc pool for struct variable_def.  */
static alloc_pool var_pool;

/* Local function prototypes.  */
static hashval_t mem_htab_hash		PARAMS ((const void *));
static int mem_htab_eq			PARAMS ((const void *, const void *));
static hashval_t variable_htab_hash	PARAMS ((const void *));
static int variable_htab_eq		PARAMS ((const void *, const void *));
static void variable_htab_free		PARAMS ((void *));

static void init_attrs_list_set		PARAMS ((attrs *));
static void attrs_list_clear		PARAMS ((attrs *));
static attrs attrs_list_member		PARAMS ((attrs, tree, HOST_WIDE_INT));
static void attrs_list_insert		PARAMS ((attrs *, tree,
						 HOST_WIDE_INT, rtx));
static void attrs_list_delete		PARAMS ((attrs *, tree,
						 HOST_WIDE_INT));
static void attrs_list_copy		PARAMS ((attrs *, attrs));
static void attrs_list_union		PARAMS ((attrs *, attrs));
static bool attrs_list_different	PARAMS ((attrs, attrs));

static void attrs_htab_insert		PARAMS ((htab_t, tree, HOST_WIDE_INT,
						 rtx));
static void attrs_htab_delete		PARAMS ((htab_t, tree, HOST_WIDE_INT));
static int attrs_htab_different_1	PARAMS ((void **, void *));
static bool attrs_htab_different	PARAMS ((htab_t, htab_t));
static int attrs_htab_copy_1		PARAMS ((void **, void *));
static void attrs_htab_copy		PARAMS ((htab_t, htab_t));
static int attrs_htab_union_1		PARAMS ((void **, void *));
static void attrs_htab_union		PARAMS ((htab_t, htab_t));
static void attrs_htab_clear		PARAMS ((htab_t));
static void attrs_htab_cleanup		PARAMS ((void *));

static bool track_expr_p		PARAMS ((tree));
static void count_uses			PARAMS ((rtx *, void *));
static void count_stores		PARAMS ((rtx, rtx, void *));
static void add_uses			PARAMS ((rtx *, void *));
static void add_stores			PARAMS ((rtx, rtx, void *));
static bool compute_bb_dataflow		PARAMS ((basic_block));
static void hybrid_search		PARAMS ((basic_block, sbitmap,
						 sbitmap));
static void iterative_dataflow		PARAMS ((int *));
static inline void dump_attrs_list	PARAMS ((attrs));
static int dump_mem_attrs_list		PARAMS ((void **, void *));
static void dump_attrs			PARAMS ((void));

static void note_insn_var_location_emit	PARAMS ((rtx, enum where_emit_note,
						 variable));
static void set_location_part		PARAMS ((tree, HOST_WIDE_INT, rtx,
						 rtx, enum where_emit_note));
static void delete_location_part	PARAMS ((tree, HOST_WIDE_INT, rtx,
						 enum where_emit_note));
static int process_location_parts	PARAMS ((void **, void *));
static void emit_note_if_var_changed	PARAMS ((void **, void *));
static inline void process_bb_delete	PARAMS ((attrs *, htab_t, int,
						 tree, HOST_WIDE_INT, rtx,
						 enum where_emit_note));
static void process_bb			PARAMS ((basic_block));
static int mark_variables_for_deletion	PARAMS ((void **, void *));
static int add_and_unmark_variables	PARAMS ((void **, void *));
static void var_tracking_emit_notes	PARAMS ((void));

static bool get_decl_and_offset		PARAMS ((rtx, tree *,
						 HOST_WIDE_INT *));
static void insert_function_parameters	PARAMS ((void));
static void var_tracking_initialize	PARAMS ((void));
static void var_tracking_finalize	PARAMS ((void));

/* Return the hash value for memory reference X.  */

static hashval_t
mem_htab_hash (x)
     const void *x;
{
  const attrs list = (const attrs) x;

  return (MEM_HASH_VAL (list->decl));
}

/* Shall memory references X and Y go to the same hash slot?  */

static int
mem_htab_eq (x, y)
     const void *x;
     const void *y;
{
  const attrs list = (const attrs) x;
  const tree decl = (const tree) y;

  return (MEM_HASH_VAL (list->decl) == MEM_HASH_VAL (decl));
}

/* The hash function for variable_htab, computes the hash value
   of the variable X declaration.  */

static hashval_t
variable_htab_hash (x)
     const void *x;
{
  const variable v = (const variable) x;

  return (VARIABLE_HASH_VAL (v->decl));
}

/* Compare the declaration of variable X with declaration Y.  */

static int
variable_htab_eq (x, y)
     const void *x;
     const void *y;
{
  const variable v = (const variable) x;
  const tree decl = (const tree) y;

  return (VARIABLE_HASH_VAL (v->decl) == VARIABLE_HASH_VAL (decl));
}

/* Free the element of VARIABLE_HTAB (its type is struct variable_def).  */

static void
variable_htab_free (var)
     void *var;
{
  pool_free (var_pool, var);
}

/* Initialize the set (array) SET of attrs to empty lists.  */

static void
init_attrs_list_set (set)
     attrs *set;
{
  int i;

  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    set[i] = NULL;
}

/* Make the list *LISTP empty.  */

static void
attrs_list_clear (listp)
     attrs *listp;
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
attrs_list_member (list, decl, offset)
     attrs list;
     tree decl;
     HOST_WIDE_INT offset;
{
  for (; list; list = list->next)
    if (list->decl == decl && list->offset == offset)
      return list;
  return NULL;
}

/* Insert the triplet DECL, OFFSET, LOC to the list *LISTP.  */

static void
attrs_list_insert (listp, decl, offset, loc)
     attrs *listp;
     tree decl;
     HOST_WIDE_INT offset;
     rtx loc;
{
  attrs list;

  list = pool_alloc (attrs_pool);
  list->loc = loc;
  list->decl = decl;
  list->offset = offset;
  list->next = *listp;
  *listp = list;
}

/* Delete all occurrences of the pairs DECL and OFFSET from the list *LISTP.  */

static void
attrs_list_delete (listp, decl, offset)
     attrs *listp;
     tree decl;
     HOST_WIDE_INT offset;
{
  attrs list, next, prev = NULL;

  for (list = *listp; list; list = next)
    {
      next = list->next;
      if (list->decl == decl && list->offset == offset)
	{
	  if (prev)
	    prev->next = next;
	  else
	    *listp = next;
	  pool_free (attrs_pool, list);
	}
      else
	prev = list;
    }
}

/* Make a copy of all nodes from SRC and make a list *DSTP of the copies.  */

static void
attrs_list_copy (dstp, src)
     attrs *dstp;
     attrs src;
{
  attrs n;

  attrs_list_clear (dstp);
  for (; src; src = src->next)
    {
      n = pool_alloc (attrs_pool);
      n->loc = src->loc;
      n->decl = src->decl;
      n->offset = src->offset;
      n->next = *dstp;
      *dstp = n;
    }
}

/* Add all nodes from SRC which are not in *DSTP to *DSTP.  */

static void
attrs_list_union (dstp, src)
     attrs *dstp;
     attrs src;
{
  for (; src; src = src->next)
    {
      if (!attrs_list_member (*dstp, src->decl, src->offset))
	attrs_list_insert (dstp, src->decl, src->offset, src->loc);
    }
}

/* Return true if the lists A and B do not have the same DECL and OFFSET values
   in nodes.  */

static bool
attrs_list_different (a, b)
     attrs a;
     attrs b;
{
  attrs i;

  for (i = a; i; i = i->next)
    if (!attrs_list_member (b, i->decl, i->offset))
      return true;

  for (i = b; i; i = i->next)
    if (!attrs_list_member (a, i->decl, i->offset))
      return true;

  return false;
}

/* Insert the triplet DECL, OFFSET, LOC to appropriate list in HTAB.  */

static void
attrs_htab_insert (htab, decl, offset, loc)
     htab_t htab;
     tree decl;
     HOST_WIDE_INT offset;
     rtx loc;
{
  attrs node, *listp;

  listp = (attrs *) htab_find_slot_with_hash (htab, decl, MEM_HASH_VAL (decl),
					      NO_INSERT);
  if (!listp)
    {
      listp = (attrs *) htab_find_slot_with_hash (htab, decl,
						  MEM_HASH_VAL (decl), INSERT);
    }

  node = pool_alloc (attrs_pool);
  node->loc = loc;
  node->decl = decl;
  node->offset = offset;
  node->next = *listp;
  *listp = node;
}

/* Delete all occurrences of the pairs DECL and OFFSET from HTAB.  */

static void
attrs_htab_delete (htab, decl, offset)
     htab_t htab;
     tree decl;
     HOST_WIDE_INT offset;
{
  attrs list, next, prev;
  attrs *listp;

  listp = (attrs *) htab_find_slot_with_hash (htab, decl, MEM_HASH_VAL (decl),
					      NO_INSERT);
  if (!listp)
    return;

  for (list = *listp; list; list = list->next)
    if (list->offset != offset)
      break;

  if (list)	/* There is at least 1 node that will not be deleted.  */
    {
      prev = NULL;
      for (list = *listp; list; list = next)
	{
	  next = list->next;
	  if (list->offset == offset)
	    {
	      if (prev)
		prev->next = next;
	      else
		*listp = next;
	      pool_free (attrs_pool, list);
	    }
	  else
	    prev = list;
	}
    }
  else
    {
      htab_clear_slot (htab, (void **) listp);
    }
}

/* Flag whether two hash-tables being compared contain different data.  */
static bool
attrs_htab_different_value;

/* Compare one SLOT with the corresponding slot from hash-table DATA.  */

static int
attrs_htab_different_1 (slot, data)
     void **slot;
     void *data;
{
  htab_t htab = (htab_t) data;
  attrs list1, list2, list;

  list1 = *(attrs *) slot;
  list2 = (attrs) htab_find_with_hash (htab, list1->decl,
				       MEM_HASH_VAL (list1->decl));

#ifdef ENABLE_CHECKING
  if (!list1)
    abort ();
#endif
  for (; list1; list1 = list1->next)
    {
      for (list = list2; list; list = list->next)
	if (list->offset == list1->offset)
	  break;
      if (!list)
	{
	  attrs_htab_different_value = true;
	  return 0;
	}
    }
  return 1;
}

/* Return true if HTAB1 and HTAB2 contain different data.  */

static bool
attrs_htab_different (htab1, htab2)
     htab_t htab1;
     htab_t htab2;
{
  attrs_htab_different_value = false;
  htab_traverse (htab1, attrs_htab_different_1, htab2);
  if (!attrs_htab_different_value)
    htab_traverse (htab2, attrs_htab_different_1, htab1);

  return attrs_htab_different_value;
}

/* Copy one attribute list *SLOT to hash-table DATA.  */

static int
attrs_htab_copy_1 (slot, data)
     void **slot;
     void *data;
{
  htab_t dst = (htab_t) data;
  attrs src, *dstp, list;

  src = *(attrs *) slot;
  dstp = (attrs *) htab_find_slot_with_hash (dst, src->decl,
					     MEM_HASH_VAL (src->decl), INSERT);
  for (; src; src = src->next)
    {
      list = pool_alloc (attrs_pool);
      list->loc = src->loc;
      list->decl = src->decl;
      list->offset = src->offset;
      list->next = *dstp;
      *dstp = list;
    }
  return 1;
}

/* Copy the content of hash-table SRC to DST.  */

static void
attrs_htab_copy (dst, src)
     htab_t dst;
     htab_t src;
{
  attrs_htab_clear (dst);
  htab_traverse (src, attrs_htab_copy_1, dst);
}

/* Add all nodes from the list in SLOT which are not in hash DATA to DATA.  */

static int
attrs_htab_union_1 (slot, data)
     void **slot;
     void *data;
{
  htab_t htab = (htab_t) data;
  attrs src, *dstp, list;

  src = *(attrs *) slot;
  dstp = (attrs *) htab_find_slot_with_hash (htab, src->decl,
					     MEM_HASH_VAL (src->decl),
					     NO_INSERT);
  if (!dstp)
    {
      dstp = (attrs *) htab_find_slot_with_hash (htab, src->decl,
						 MEM_HASH_VAL (src->decl),
						 INSERT);
    }
  for (; src; src = src->next)
    {
      for (list = *dstp; list; list = list->next)
	if (list->offset == src->offset)
	  break;
      if (!list)
	{
	  list = pool_alloc (attrs_pool);
	  list->loc = src->loc;
	  list->decl = src->decl;
	  list->offset = src->offset;
	  list->next = *dstp;
	  *dstp = list;
	}
    }
  return 1;
}

/* Add all nodes from SRC which are not in hash DST to DST.  */

static void
attrs_htab_union (dst, src)
     htab_t dst;
     htab_t src;
{
  htab_traverse (src, attrs_htab_union_1, dst);
}

/* Delete all data from HTAB.  */

static void
attrs_htab_clear (htab)
     htab_t htab;
{
  htab_empty (htab);
}

/* Delete data from SLOT.  */

static void
attrs_htab_cleanup (slot)
     void *slot;
{
  attrs list, next;

  for (list = (attrs) slot; list; list = next)
    {
      next = list->next;
      pool_free (attrs_pool, list);
    }
}

/* Shall EXPR be tracked?  */

static bool
track_expr_p (expr)
     tree expr;
{
  rtx decl_rtl;

  /* If EXPR is not a parameter or a variable do not track it.  */
  if (TREE_CODE (expr) != VAR_DECL && TREE_CODE (expr) != PARM_DECL)
    return 0;

  /* It also must have a name...  */
  if (!DECL_NAME (expr))
    return 0;

  /* ... and a RTL assigned to it.  */
  decl_rtl = DECL_RTL (expr);
  if (!decl_rtl)
    return 0;

  /* If RTX is a memory it should not be very large (because it would be an array
     or struct).  */
  if (GET_CODE (decl_rtl) == MEM)
    {
      if (GET_MODE (decl_rtl) == BLKmode)
	return 0;
      if (MEM_SIZE (decl_rtl) && INTVAL (MEM_SIZE (decl_rtl)) > MAX_LOC_PARTS)
	return 0;
    }

  return 1;
}

/* Count uses (register and memory references) LOC which will be tracked.
   INSN is instruction which the LOC is part of.  */

static void
count_uses (loc, insn)
     rtx *loc;
     void *insn;
{
  basic_block bb = BLOCK_FOR_INSN ((rtx) insn);

  if (GET_CODE (*loc) == REG)
    {
#ifdef ENABLE_CHECKING
	if (REGNO (*loc) >= FIRST_PSEUDO_REGISTER)
	  abort ();
#endif
	VTI (bb)->n_locs++;
    }
  else if (GET_CODE (*loc) == MEM
	   && MEM_EXPR (*loc)
	   && track_expr_p (MEM_EXPR (*loc)))
    {
	  VTI (bb)->n_locs++;
    }
}

/* Count stores (register and memory references) LOC which will be tracked.
   INSN is instruction which the LOC is part of.  */

static void
count_stores (loc, expr, insn)
     rtx loc;
     rtx expr ATTRIBUTE_UNUSED;
     void *insn;
{
  count_uses (&loc, insn);
}

/* Add uses (register and memory references) LOC which will be tracked
   to VTI (bb)->locs.  INSN is instruction which the LOC is part of.  */

static void
add_uses (loc, insn)
     rtx *loc;
     void *insn;
{
  if (GET_CODE (*loc) == REG
      || (GET_CODE (*loc) == MEM
	  && MEM_EXPR (*loc)
	  && track_expr_p (MEM_EXPR (*loc))))
    {
      basic_block bb = BLOCK_FOR_INSN ((rtx) insn);
      location *l = VTI (bb)->locs + VTI (bb)->n_locs++;

      l->loc = *loc;
      l->insn = (rtx) insn;
      l->type = LT_USE;
    }
}

/* Add stores (register and memory references) LOC which will be tracked
   to VTI (bb)->locs. EXPR is the RTL expression containing the store.
   INSN is instruction which the LOC is part of.  */

static void
add_stores (loc, expr, insn)
     rtx loc;
     rtx expr;
     void *insn;
{
  if (GET_CODE (loc) == REG
      || (GET_CODE (loc) == MEM
	  && MEM_EXPR (loc)
	  && track_expr_p (MEM_EXPR (loc))))
    {
      basic_block bb = BLOCK_FOR_INSN ((rtx) insn);
      location *l = VTI (bb)->locs + VTI (bb)->n_locs++;

      l->loc = loc;
      l->insn = (rtx) insn;
      l->type = GET_CODE (expr) == CLOBBER ? LT_CLOBBER : LT_SET;
    }
}

/* Compute the changes of variable locations in the basic block BB.  */

static bool
compute_bb_dataflow (bb)
     basic_block bb;
{
  int i, n;
  bool changed;

  attrs old_out[FIRST_PSEUDO_REGISTER];
  htab_t old_mem_out;
  attrs *in = VTI (bb)->in;
  attrs *out = VTI (bb)->out;

  init_attrs_list_set (old_out);
  old_mem_out = htab_create (htab_elements (VTI (bb)->mem_out) + 3,
			     mem_htab_hash, mem_htab_eq, attrs_htab_cleanup);
  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    {
      attrs_list_copy (&old_out[i], out[i]);
      attrs_list_copy (&out[i], in[i]);
    }
  attrs_htab_copy (old_mem_out, VTI (bb)->mem_out);
  attrs_htab_copy (VTI (bb)->mem_out, VTI (bb)->mem_in);

  n = VTI (bb)->n_locs;
  for (i = 0; i < n; i++)
    {
      rtx loc = VTI (bb)->locs[i].loc;

      if (GET_CODE (loc) == REG)
	{
	  attrs_list_clear (&out[REGNO (loc)]);
	  if (VTI (bb)->locs[i].type == LT_USE
	      || VTI (bb)->locs[i].type == LT_SET)
	    {
	      if (REG_EXPR (loc) && track_expr_p (REG_EXPR (loc)))
		{
		  attrs_list_insert (&out[REGNO (loc)], REG_EXPR (loc),
				     REG_OFFSET (loc), loc);
		}
	    }
	}
      else if (GET_CODE (loc) == MEM
	       && MEM_EXPR (loc)
	       && track_expr_p (MEM_EXPR (loc)))
	{
	  int j;
	  tree decl = MEM_EXPR (loc);
	  HOST_WIDE_INT offset = MEM_OFFSET (loc) ? INTVAL (MEM_OFFSET (loc)) : 0;

	  attrs_htab_delete (VTI (bb)->mem_out, decl, offset);
	  if (VTI (bb)->locs[i].type == LT_USE
	      || VTI (bb)->locs[i].type == LT_SET)
	    {
	      /* We are storing a part of variable to memory. If variable
		 has several equivalent locations keep only the memory
		 location.  So remove variable's occurrences from registers.
	       */
	      for (j = 0; j < FIRST_PSEUDO_REGISTER; j++)
		attrs_list_delete (&out[j], decl, offset);
	      attrs_htab_insert (VTI (bb)->mem_out, decl, offset, loc);
	    }
	}
    }

  changed = false;
  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    if (attrs_list_different (old_out[i], out[i]))
      {
	changed = true;
	break;
      }
  if (!changed)
    changed = attrs_htab_different (old_mem_out, VTI (bb)->mem_out);

  attrs_list_clear (old_out);
  attrs_htab_clear (old_mem_out);
  htab_delete (old_mem_out);
  return changed;
}

/* Hybrid search.  It is the same as hybrid_search_bitmap in df.c but modified
   for different data structures.
   Search from basic block BB and use the bitmaps VISITED and PENDING
   to control the searching.  */

static void
hybrid_search (bb, visited, pending)
     basic_block bb;
     sbitmap visited;
     sbitmap pending;
{
  bool changed;
  edge e;
  int i;

  SET_BIT (visited, bb->index);
  if (TEST_BIT (pending, bb->index))
    {
      /* Clear the IN list set.  */
      for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
	attrs_list_clear (&VTI (bb)->in[i]);
      attrs_htab_clear (VTI (bb)->mem_in);

      /* Calculate the union of predecessor outs.  */
      for (e = bb->pred; e; e = e->pred_next)
	{
	  attrs *bb_in = VTI (bb)->in;
	  attrs *pred_out = VTI (e->src)->out;

	  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
	    attrs_list_union (&bb_in[i], pred_out[i]);
	  attrs_htab_union (VTI (bb)->mem_in, VTI (e->src)->mem_out);
	}

      RESET_BIT (pending, bb->index);
      changed = compute_bb_dataflow (bb);
      if (changed)
	{
	  for (e = bb->succ; e != 0; e = e->succ_next)
	    {
	      if (e->dest == EXIT_BLOCK_PTR || e->dest->index == bb->index)
		continue;
	      SET_BIT (pending, e->dest->index);
	    }
	}
    }
  for (e = bb->succ; e != 0; e = e->succ_next)
    {
      if (e->dest == EXIT_BLOCK_PTR || e->dest->index == bb->index)
	continue;
      if (!TEST_BIT (visited, e->dest->index))
	hybrid_search (e->dest, visited, pending);
    }
}

/* This function will perform iterative dataflow, producing the in and out
   sets.  It is the same as iterative_dataflow_bitmap in df.c but modified
   for different data structures.
   BB_ORDER is the order to iterate in (should map block numbers -> order).
   Because this is a forward dataflow problem we pass in a mapping of block
   number to rc_order (like df->inverse_rc_map).  */

static void
iterative_dataflow (bb_order)
     int *bb_order;
{
  fibheap_t worklist;
  basic_block bb;
  sbitmap visited, pending;

  pending = sbitmap_alloc (last_basic_block);
  visited = sbitmap_alloc (last_basic_block);
  sbitmap_zero (pending);
  sbitmap_zero (visited);
  worklist = fibheap_new ();

  FOR_EACH_BB (bb)
    {
      fibheap_insert (worklist, bb_order[bb->index], bb);
      SET_BIT (pending, bb->index);
    }
  while (sbitmap_first_set_bit (pending) != -1)
    {
      while (!fibheap_empty (worklist))
	{
	  bb = fibheap_extract_min (worklist);
	  if (!TEST_BIT (visited, bb->index))
	    hybrid_search (bb, visited, pending);
	}
      if (sbitmap_first_set_bit (pending) != -1)
	{
	  FOR_EACH_BB (bb)
	    {
	      fibheap_insert (worklist, bb_order[bb->index], bb);
	    }
	  sbitmap_zero (visited);
	}
      else
	{
	  break;
	}
    }
  sbitmap_free (pending);
  sbitmap_free (visited);
  fibheap_delete (worklist);
}

/* Print the content of the LIST to dump file.  */

static inline void
dump_attrs_list (list)
     attrs list;
{
  for (; list; list = list->next)
    {
      print_mem_expr (rtl_dump_file, list->decl);
      fprintf (rtl_dump_file, "+");
      fprintf (rtl_dump_file, HOST_WIDE_INT_PRINT_DEC, list->offset);
    }
}

/* Print the content of the list in SLOT to dump file.  */

static int
dump_mem_attrs_list (slot, data)
     void **slot;
     void *data ATTRIBUTE_UNUSED;
{
  dump_attrs_list (*(attrs *) slot);
  return 1;
}

/* Print the IN and OUT sets for each basic block to dump file.  */

static void
dump_attrs ()
{
  int i;
  basic_block bb;

  FOR_EACH_BB (bb)
    {
      fprintf (rtl_dump_file, "\nBasic block %d:\n", bb->index);

      fprintf (rtl_dump_file, "  IN: ");
      for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
	{
	  if (VTI (bb)->in[i])
	    {
	      fprintf (rtl_dump_file, "Reg %d:", i);
	      dump_attrs_list (VTI (bb)->in[i]);
	      fprintf (rtl_dump_file, "; ");
	    }
	}
      if (htab_elements (VTI (bb)->mem_in) > 0)
	{
	  fprintf (rtl_dump_file, "Mem:");
	  htab_traverse (VTI (bb)->mem_in, dump_mem_attrs_list, NULL);
	}
      fprintf (rtl_dump_file, "\n");

      fprintf (rtl_dump_file, "  OUT: ");
      for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
	{
	  if (VTI (bb)->out[i])
	    {
	      fprintf (rtl_dump_file, "Reg %d:", i);
	      dump_attrs_list (VTI (bb)->out[i]);
	      fprintf (rtl_dump_file, "; ");
	    }
	}
      if (htab_elements (VTI (bb)->mem_out) > 0)
	{
	  fprintf (rtl_dump_file, "Mem:");
	  htab_traverse (VTI (bb)->mem_out, dump_mem_attrs_list, NULL);
	}
      fprintf (rtl_dump_file, "\n");
    }
}

/* Emit the NOTE_INSN_VAR_LOCATION for variable VAR.  WHERE specifies
   whether the note shall be emitted before of after instruction INSN.  */

static void
note_insn_var_location_emit (insn, where, var)
     rtx insn;
     enum where_emit_note where;
     variable var;
{
  rtx note;
  int i;
  bool complete;
  HOST_WIDE_INT last_limit;
  tree type_size_unit;

#ifdef ENABLE_CHECKING
  if (!var->decl)
    abort ();
#endif

  complete = true;
  last_limit = 0;
  for (i = 0; i < var->n_location_parts; i++)
    {
      if (last_limit < var->location_part[i].offset)
	{
	  complete = false;
	  break;
	}
      last_limit = (var->location_part[i].offset
		    + GET_MODE_SIZE (GET_MODE (var->location_part[i].loc)));
    }
  type_size_unit = TYPE_SIZE_UNIT (TREE_TYPE (var->decl));
  if ((unsigned HOST_WIDE_INT) last_limit < TREE_INT_CST_LOW (type_size_unit))
    complete = false;

  if (where == EMIT_NOTE_AFTER_INSN)
    note = emit_note_after (NOTE_INSN_VAR_LOCATION, insn);
  else
    note = emit_note_before (NOTE_INSN_VAR_LOCATION, insn);

  if (!complete)
    {
      NOTE_VAR_LOCATION (note) = gen_rtx_VAR_LOCATION (VOIDmode, var->decl,
						       NULL_RTX);
    }
  else if (var->n_location_parts == 1)
    {
      rtx expr_list
	= gen_rtx_EXPR_LIST (VOIDmode,
			     var->location_part[0].loc,
			     GEN_INT (var->location_part[0].offset));

      NOTE_VAR_LOCATION (note) = gen_rtx_VAR_LOCATION (VOIDmode, var->decl,
						       expr_list);
    }
  else if (var->n_location_parts)
    {
      rtx argp[MAX_LOC_PARTS];
      rtx parallel;

      for (i = 0; i < var->n_location_parts; i++)
	argp[i] = gen_rtx_EXPR_LIST (VOIDmode,
				     var->location_part[i].loc,
				     GEN_INT (var->location_part[i].offset));
      parallel = gen_rtx_PARALLEL (VOIDmode,
				   gen_rtvec_v (var->n_location_parts, argp));
      NOTE_VAR_LOCATION (note) = gen_rtx_VAR_LOCATION (VOIDmode, var->decl,
						       parallel);
    }
}

/* Set the part of variable's location.  The variable part is specified
   by variable's declaration DECL and offset OFFSET and the part's location
   by LOC.  The INSN and WHERE parameters specify where the note will be
   emitted (see note_insn_var_location_emit).  */

static void
set_location_part (decl, offset, loc, insn, where)
     tree decl;
     HOST_WIDE_INT offset;
     rtx loc;
     rtx insn;
     enum where_emit_note where;
{
  int k;
  variable var = htab_find_with_hash (variable_htab, decl,
				      VARIABLE_HASH_VAL (decl));
  if (!var)
    {
      void **slot;
      /* Create new variable information.  */
      var = pool_alloc (var_pool);
      var->decl = decl;
      var->n_location_parts = 0;

      slot = htab_find_slot_with_hash (variable_htab, decl,
				       VARIABLE_HASH_VAL (decl), INSERT);
#ifdef ENABLE_CHECKING
      if (*slot)
	abort ();
#endif
      *slot = (void *) var;
      k = 0;
    }
  else
    {
      /* Find the location part.  */
      for (k = 0; k < var->n_location_parts; k++)
	if (var->location_part[k].offset == offset)
	  break;
    }
  if (k == var->n_location_parts)
    {
      /* Did not find the part, create new one.  */
      if (var->n_location_parts >= MAX_LOC_PARTS)
	abort ();
      var->n_location_parts++;
      var->location_part[k].offset = offset;
      var->location_part[k].loc = NULL;
    }

  /* Mark the location part that it should not be deleted.  */
  var->location_part[k].delete_p = false;
  if (var->location_part[k].loc != loc)
    {
      emit_note_data emit_note_data;
      void *slot;

      var->location_part[k].loc = loc;
      var->changed = true;
      emit_note_data.insn = insn;
      emit_note_data.where = where;
      slot = var;
      emit_note_if_var_changed ((void **) &slot, &emit_note_data);
      var = slot;
    }
}

/* Delete the part of variable's location.  The variable part is specified
   by variable's declaration DECL and offset OFFSET.
   The INSN and WHERE parameters specify where the note will be emitted
   (see note_insn_var_location_emit).  */

static void
delete_location_part (decl, offset, insn, where)
     tree decl;
     HOST_WIDE_INT offset;
     rtx insn;
     enum where_emit_note where;
{
  int i;
  void **slot = htab_find_slot_with_hash (variable_htab, decl,
					  VARIABLE_HASH_VAL (decl), NO_INSERT);
  if (slot && *slot)
    {
      variable var = (variable) *slot;

      /* Delete the location part.  */
      for (i = 0; i < var->n_location_parts; i++)
	if (var->location_part[i].offset == offset)
	  {
	    emit_note_data emit_note_data;

	    var->n_location_parts--;
	    var->location_part[i] = var->location_part[var->n_location_parts];
	    var->changed = true;
	    emit_note_data.insn = insn;
	    emit_note_data.where = where;
	    emit_note_if_var_changed (slot, &emit_note_data);
	    break;
	  }
    }
}

/* Delete marked location parts of a variable and emit notes if needed.
   **SLOT is a description of parts of the variable.
   AUX contains a pointer to data passed to functions emitting the notes.  */

static int
process_location_parts (slot, aux)
     void **slot;
     void *aux;
{
  variable var = *slot;
  int i;

  for (i = 0; i < var->n_location_parts; i++)
    if (var->location_part[i].delete_p)
      {
	var->n_location_parts--;
	var->location_part[i] = var->location_part[var->n_location_parts];
	var->changed = true;
      }
  emit_note_if_var_changed (slot, aux);
  return 1;
}

/* Emit NOTE_INSN_VAR_LOCATION notes for description **SLOT of a variable
   if any of the variable parts has changed.
   AUX contains data describing where the note will be emitted (see
   note_insn_var_location_emit).  */

static void
emit_note_if_var_changed (slot, aux)
     void **slot;
     void *aux;
{
  variable var = *slot;
  emit_note_data *data = (emit_note_data *) aux;

  if (var->changed)
    {
      var->changed = false;
      note_insn_var_location_emit (data->insn, data->where, var);
    }
  if (var->n_location_parts == 0)
    htab_clear_slot (variable_htab, slot);
}

/* Delete the location part of variable corresponding to DECL and OFFSET from
   all register locations (in LISTS) and memory locations (in HTAB) and emit
   notes before/after (parameter WHERE) INSN.  The location part which
   has the same decl and offset as LOC is deleted from HTAB only when
   DELETE_LOC_FROM_HTAB is true.  */

static inline void
process_bb_delete (lists, htab, delete_loc_from_htab, decl, offset, insn, where)
     attrs *lists;
     htab_t htab;
     int delete_loc_from_htab;
     tree decl;
     HOST_WIDE_INT offset;
     rtx insn;
     enum where_emit_note where;
{
  attrs list;
  int i;

  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    {
      for (list = lists[i]; list; list = list->next)
	if (list->offset == offset && list->decl == decl)
	  delete_location_part (list->decl, list->offset, insn, where);
      attrs_list_delete (&lists[i], decl, offset);
    }

  if (delete_loc_from_htab)
    {
      list = (attrs) htab_find_with_hash (htab, decl, MEM_HASH_VAL (decl));
      for (; list; list = list->next)
	if (list->offset == offset)
	  delete_location_part (list->decl, list->offset, insn, where);
      attrs_htab_delete (htab, decl, offset);
    }
}

/* Emit the notes for changes of location parts of a variable in the basic
   block BB.  */

static void
process_bb (bb)
     basic_block bb;
{
  int i;
  attrs reg[FIRST_PSEUDO_REGISTER];
  htab_t mem;

  init_attrs_list_set (reg);
  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    attrs_list_copy (&reg[i], VTI (bb)->in[i]);
  mem = htab_create (htab_elements (VTI (bb)->mem_in) + 3, mem_htab_hash,
			   mem_htab_eq, attrs_htab_cleanup);
  attrs_htab_copy (mem, VTI (bb)->mem_in);

  for (i = 0; i < VTI (bb)->n_locs; i++)
    {
      attrs l;
      rtx insn = VTI (bb)->locs[i].insn;
      rtx loc = VTI (bb)->locs[i].loc;

      if (GET_CODE (loc) == REG)
	{
	  tree decl = REG_EXPR (loc);
	  HOST_WIDE_INT offset = REG_OFFSET (loc);
	  enum where_emit_note where = EMIT_NOTE_AFTER_INSN;

	  switch (VTI (bb)->locs[i].type)
	    {
	      case LT_USE:
		where = EMIT_NOTE_BEFORE_INSN;
	      case LT_SET:
		for (l = reg[REGNO (loc)]; l; l = l->next)
		  if (l->decl != decl || l->offset != offset)
		    delete_location_part (l->decl, l->offset, insn, where);
		attrs_list_clear (&reg[REGNO (loc)]);
		if (decl)
		  {
		    if (track_expr_p (decl))
		      {
			set_location_part (decl, offset, loc, insn, where);
			attrs_list_insert (&reg[REGNO (loc)], decl, offset, loc);
		      }
		    else
		      {
			delete_location_part (decl, offset, insn, where);
		      }
		  }
		break;

	      case LT_CLOBBER:
		for (l = reg[REGNO (loc)]; l; l = l->next)
		  delete_location_part (l->decl, l->offset, insn,
					EMIT_NOTE_AFTER_INSN);
		attrs_list_clear (&reg[REGNO (loc)]);
		break;
	    }
	}
      else if (GET_CODE (loc) == MEM
	       && MEM_EXPR (loc)
	       && track_expr_p (MEM_EXPR (loc)))
	{
	  enum where_emit_note where = EMIT_NOTE_AFTER_INSN;
	  tree decl = MEM_EXPR (loc);
	  HOST_WIDE_INT offset = MEM_OFFSET (loc) ? INTVAL (MEM_OFFSET (loc)) : 0;

	  switch (VTI (bb)->locs[i].type)
	    {
	      case LT_USE:
		where = EMIT_NOTE_BEFORE_INSN;
	      case LT_SET:
		/* We are storing a part of variable to memory. If variable
		   has several equivalent locations keep only the memory
		   location.  So remove variable's occurrences from registers.
		 */
		process_bb_delete (reg, mem, false, decl, offset, insn, where);
		set_location_part (decl, offset, loc, insn, where);
		attrs_htab_insert (mem, decl, offset, loc);
		break;

	      case LT_CLOBBER:
		process_bb_delete (reg, mem, true, decl, offset, insn,
				   EMIT_NOTE_AFTER_INSN);
		break;
	    }
	}
    }
  htab_delete (mem);
}

/* Mark the variable location parts for the attributes from the list *SLOT
   to be deleted.  */

static int
mark_variables_for_deletion (slot, data)
     void **slot;
     void *data ATTRIBUTE_UNUSED;
{
  attrs list;

  for (list = *(attrs *) slot; list; list = list->next)
    {
      variable var = htab_find_with_hash (variable_htab, list->decl,
					  VARIABLE_HASH_VAL (list->decl));
      if (var)
	{
	  int k;
	  for (k = 0; k < var->n_location_parts; k++)
	    if (var->location_part[k].offset == list->offset)
	      {
		var->location_part[k].delete_p = true;
		break;
	      }
	}
    }
  return 1;
}

/* Add the variable location parts for attributes from the list *SLOT and emit
   notes before insn DATA.  */

static int
add_and_unmark_variables (slot, data)
     void **slot;
     void *data;
{
  attrs list;
  rtx insn = (rtx) data;

  for (list = *(attrs *) slot; list; list = list->next)
    {
      set_location_part (list->decl, list->offset, list->loc, insn,
			 EMIT_NOTE_BEFORE_INSN);
    }
  return 1;
}

/* Emit notes for the whole function.  */

static void
var_tracking_emit_notes ()
{
  basic_block bb;
  int i;
  attrs *last_out, *in;
  attrs empty[FIRST_PSEUDO_REGISTER];
  htab_t last_htab;

  init_attrs_list_set (empty);
  last_out = empty;
  last_htab = htab_create (7, mem_htab_hash, mem_htab_eq, attrs_htab_cleanup);

  FOR_EACH_BB (bb)
    {
      emit_note_data emit_note_data;

      /* Emit the notes for changes of variable locations between two
	 subsequent basic blocks.  */

      /* Mark the variables of previous OUT set.  */
      for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
	mark_variables_for_deletion ((void **) &last_out[i], NULL);
      htab_traverse (last_htab, mark_variables_for_deletion, NULL);

      /* Add the variables of IN set and unmark them.  */
      in = VTI (bb)->in;
      for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
	add_and_unmark_variables ((void **) &in[i], bb->head);
      htab_traverse (VTI (bb)->mem_in, add_and_unmark_variables, bb->head);

      /* Delete the marked location parts and emit the notes.  */
      emit_note_data.insn = bb->head;
      emit_note_data.where = EMIT_NOTE_BEFORE_INSN;
      htab_traverse (variable_htab, process_location_parts, &emit_note_data);

      /* Emit the notes for the changes in the basic block itself.  */
      process_bb (bb);

      last_out = VTI (bb)->out;
      attrs_htab_copy (last_htab, VTI (bb)->mem_out);
    }
  htab_delete (last_htab);
}

/* If there is a declaration and offset associated with register/memory RTL
   assign declaration to *DECLP and offset to *OFFSETP, and return true.  */

static bool
get_decl_and_offset (rtl, declp, offsetp)
     rtx rtl;
     tree *declp;
     HOST_WIDE_INT *offsetp;
{
  if (GET_CODE (rtl) == REG)
    {
      if (REG_ATTRS (rtl))
	{
	  *declp = REG_EXPR (rtl);
	  *offsetp = REG_OFFSET (rtl);
	  return true;
	}
    }
  else if (GET_CODE (rtl) == MEM)
    {
      if (MEM_ATTRS (rtl))
	{
	  *declp = MEM_EXPR (rtl);
	  *offsetp = MEM_OFFSET (rtl) ? INTVAL (MEM_OFFSET (rtl)) : 0;
	  return true;
	}
    }
  return false;
}

/* Insert function parameters to IN and OUT sets of ENTRY_BLOCK.  */

static void
insert_function_parameters ()
{
  tree parm;

  for (parm = DECL_ARGUMENTS (current_function_decl);
       parm; parm = TREE_CHAIN (parm))
    {
      rtx decl_rtl = DECL_RTL (parm);
      rtx incoming = DECL_INCOMING_RTL (parm);
      tree decl;
      HOST_WIDE_INT offset;

      if (!decl_rtl || !incoming)
	abort ();

      if (!get_decl_and_offset (incoming, &decl, &offset))
	if (!get_decl_and_offset (decl_rtl, &decl, &offset))
	  continue;

      if (parm != decl)
	abort ();

      incoming = eliminate_regs (incoming, 0, NULL_RTX);
      if (GET_CODE (incoming) == REG)
	{
	  attrs *in = VTI (ENTRY_BLOCK_PTR)->in;
	  attrs *out = VTI (ENTRY_BLOCK_PTR)->out;

	  if (REGNO (incoming) >= FIRST_PSEUDO_REGISTER)
	    abort ();
	  attrs_list_insert (&in[REGNO (incoming)], parm, offset, incoming);
	  attrs_list_insert (&out[REGNO (incoming)], parm, offset, incoming);
	}
      else if (GET_CODE (incoming) == MEM)
	{
	  htab_t mem_in = VTI (ENTRY_BLOCK_PTR)->mem_in;
	  htab_t mem_out = VTI (ENTRY_BLOCK_PTR)->mem_out;

	  attrs_htab_insert (mem_in, parm, offset, incoming);
	  attrs_htab_insert (mem_out, parm, offset, incoming);
	}
    }
}

/* Allocate and initialize the data structures for variable tracking
   and parse the RTL to get the REG and MEM references.  */

static void
var_tracking_initialize ()
{
  basic_block bb;

  alloc_aux_for_blocks (sizeof (struct var_tracking_info_def));

  FOR_EACH_BB (bb)
    {
      rtx insn;

      /* Count the number of stores.  */
      VTI (bb)->n_locs = 0;
      for (insn = bb->head; insn != NEXT_INSN (bb->end);
	   insn = NEXT_INSN (insn))
	{
	  if (INSN_P (insn))
	    {
	      note_uses (&PATTERN (insn), count_uses, insn);
	      note_stores (PATTERN (insn), count_stores, insn);
	    }
	}

      /* Add the stores to the array.  */
      VTI (bb)->locs = xmalloc (VTI (bb)->n_locs
				* sizeof (struct location_def));
      VTI (bb)->n_locs = 0;
      for (insn = bb->head; insn != NEXT_INSN (bb->end);
	   insn = NEXT_INSN (insn))
	{
	  if (INSN_P (insn))
	    {
	      int n1, n2;

	      n1 = VTI (bb)->n_locs;
	      note_uses (&PATTERN (insn), add_uses, insn);
	      note_stores (PATTERN (insn), add_stores, insn);
	      n2 = VTI (bb)->n_locs - 1;

	      /* Order the locations so that the locations of type LT_USE are
		 before others.  */
	      while (n1 < n2)
		{
		  while (n1 < n2 && VTI (bb)->locs[n1].type == LT_USE)
		    n1++;
		  while (n1 < n2 && VTI (bb)->locs[n2].type != LT_USE)
		    n2--;
		  if (n1 < n2)
		    {
		      location sw;

		      sw = VTI (bb)->locs[n1];
		      VTI (bb)->locs[n1] = VTI (bb)->locs[n2];
		      VTI (bb)->locs[n2] = sw;
		    }
		}

	      /* Now the LT_USEs are first, order the rest so that LT_SETs are
		 before LT_CLOBBERs.  */
	      n2 = VTI (bb)->n_locs - 1;
	      while (n1 < n2)
		{
		  while (n1 < n2 && VTI (bb)->locs[n1].type == LT_SET)
		    n1++;
		  while (n1 < n2 && VTI (bb)->locs[n2].type == LT_CLOBBER)
		    n2--;
		  if (n1 < n2)
		    {
		      location sw;

		      sw = VTI (bb)->locs[n1];
		      VTI (bb)->locs[n1] = VTI (bb)->locs[n2];
		      VTI (bb)->locs[n2] = sw;
		    }
		}
	    }
	}
    }

  /* Init the IN and OUT sets.  */
  FOR_BB_BETWEEN (bb, ENTRY_BLOCK_PTR, EXIT_BLOCK_PTR, next_bb)
    {
      init_attrs_list_set (VTI (bb)->in);
      init_attrs_list_set (VTI (bb)->out);
      VTI (bb)->mem_in = htab_create (7, mem_htab_hash, mem_htab_eq,
				      attrs_htab_cleanup);
      VTI (bb)->mem_out = htab_create (7, mem_htab_hash, mem_htab_eq,
				       attrs_htab_cleanup);
    }

  attrs_pool = create_alloc_pool ("attrs_def pool",
				  sizeof (struct attrs_def), 1024);
  var_pool = create_alloc_pool ("variable_def pool",
				sizeof (struct variable_def), 64);
  variable_htab = htab_create (37, variable_htab_hash, variable_htab_eq,
			       variable_htab_free);
  insert_function_parameters ();
}

/* Free the data structures needed for variable tracking.  */

static void
var_tracking_finalize ()
{
  basic_block bb;
  int i;

  FOR_EACH_BB (bb)
    {
      free (VTI (bb)->locs);
    }

  FOR_BB_BETWEEN (bb, ENTRY_BLOCK_PTR, EXIT_BLOCK_PTR, next_bb)
    {
      for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
	{
	  attrs_list_clear (&VTI (bb)->in[i]);
	  attrs_list_clear (&VTI (bb)->out[i]);
	}
      htab_delete (VTI (bb)->mem_in);
      htab_delete (VTI (bb)->mem_out);
    }
  free_aux_for_blocks ();
  htab_delete (variable_htab);
  free_alloc_pool (attrs_pool);
  free_alloc_pool (var_pool);
}

/* The entry point to variable tracking pass.  */

void
variable_tracking_main ()
{
  int i;
  int *rc_order;
  int *bb_order;

  var_tracking_initialize ();

  /* Compute reverse completion order of depth first search of the CFG
     so that the dataflow could possibly run faster.  */
  rc_order = (int *) xmalloc (n_basic_blocks * sizeof (int));
  bb_order = (int *) xmalloc (last_basic_block * sizeof (int));
  flow_depth_first_order_compute (NULL, rc_order);
  for (i = 0; i < n_basic_blocks; i++)
    bb_order[rc_order[i]] = i;
  free (rc_order);

  iterative_dataflow (bb_order);
  var_tracking_emit_notes ();

  if (rtl_dump_file)
    {
      dump_attrs ();
      dump_flow_info (rtl_dump_file);
    }

  free (bb_order);
  var_tracking_finalize ();
}

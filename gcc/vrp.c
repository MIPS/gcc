/* Value range propagation and dead branch elimination.
   Copyright (C) 2003 Free Software Foundation, Inc.

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

/* This file performs simple value range propagation.
   It also removes branches which can't be reached,
   all code from the following example may be removed:

   if (i < 0)
   {
     if (i > 5)
       printf ("foo");
   }

   Currently it only tracks integer registers.

   This pass performs a data-flow on value ranges.
   As usual it has IN and OUT sets for each basic block.  The set is a hash
   table, for each register there is a range which the value of the register
   can be in.  When the range is full (i.e. from min value for the type
   to the max value) it is not stored in the hash table to save memory
   (so when the range for register is not there the range is full).
   Moreover VRP has a set for each edge too because the value ranges are
   different when we go through a then edge or an else edge.

   The IN set for a BB is computed as "union bounds" of the sets on edges
   going to BB. "Union bound" means that when we have 2 intervals
   [from_1, to_1] and [from_2, to_2] the result is
   [MIN (from_1, from_2), MAX (to_1, to_2)].

   We are producing the OUT set of BB from IN set by scanning the instructions
   in BB.  When we find an insn assigning a CONST_INT or REG to REG
   we set the range according to SET_SRC (only simple cases of SET_DST and
   SET_SRC are handled, in the other cases we set the range to be full).

   Finally we update the sets on edges going from BB.  First we copy the OUT set
   to edges going out from BB. Then we update the ranges on edges according to
   a cond jump (if cond jump is in the end of BB).

   When we have computed the ranges we redirect "dead" edge for a cond jump
   to the other edge.  "Dead" edge is an edge which has an empty range for some
   register on it, i.e. we will never go through this edge because of the
   conditions.  */

#include "config.h"
#include "system.h"
#include "rtl.h"
#include "tree.h"
#include "basic-block.h"
#include "hard-reg-set.h"
#include "expr.h"
#include "output.h"
#include "alloc-pool.h"
#include "sbitmap.h"
#include "fibheap.h"
#include "hashtab.h"

/* Signed or unsigned integer.  */
typedef struct value_def
{
  /* Signed integer value.  */
  signed HOST_WIDE_INT si;

  /* Unsigned integer value.  */
  unsigned HOST_WIDE_INT ui;
} value;

/* Structure describing a range.  */
typedef struct range_def
{
  /* Register which contains a value from the range.  */
  rtx reg;

  /* Lower bound of the range.  */
  value from;

  /* Upper bound of the range.  */
  value to;

  /* Type of the range, see RANGE_* below.  */
  int type;
} range_t;

/* Range types.  */
#define RANGE_SIGNED_INT	1	/* Signed integer.  */
#define RANGE_UNSIGNED_INT	2	/* Unsigned integer.  */

/* Data for each edge.  */
typedef struct edge_info_def
{
  /* The list of ranges of registers.  */
  htab_t htab;
} edge_info;

/* Data for each basic block.  */
typedef struct bb_info_def
{
  /* Normalized condition (if any) for the branch in the end of BB.  */
  rtx cond;

  /* The IN list of ranges of registers for BB.  */
  htab_t in;

  /* The OUT list of ranges of registers for BB.  */
  htab_t out;
} bb_info;

/* Get a pointer to data for a basic block BB.  */
#define BBI(BB) ((bb_info *) (BB)->aux)

/* Get a pointer to data for an edge E.  */
#define EI(E) ((edge_info *) (E)->aux)

/* Alloc pool for range_t.  */
alloc_pool range_pool;

/* Local function prototypes.  */
static hashval_t range_hash		PARAMS ((const void *));
static int range_eq			PARAMS ((const void *, const void *));
static void range_del			PARAMS ((void *));
static bool range_is_empty		PARAMS ((const range_t *));
static int union_ranges			PARAMS ((void **, void *));
static void union_all_ranges		PARAMS ((htab_t, htab_t));
static bool ranges_differ		PARAMS ((const range_t *,
						 const range_t *));
static int compare_register_tables_1	PARAMS ((void **, void *));
static bool compare_register_tables	PARAMS ((htab_t, htab_t));
static int copy_range			PARAMS ((void **, void *));
static void copy_register_table		PARAMS ((htab_t, htab_t));
static void process_ranges_eq		PARAMS ((rtx, rtx, edge));
static void process_ranges_lt		PARAMS ((rtx, rtx, edge, edge));
static void process_ranges_ltu		PARAMS ((rtx, rtx, edge, edge));
static bool update_outgoing_edges	PARAMS ((basic_block, int));
static void compute_ranges_for_insn	PARAMS ((rtx, rtx, void *));
static int delete_call_used_regs	PARAMS ((void **, void *));
static bool compute_ranges_for_bb	PARAMS ((basic_block));
static void compute_ranges_for_pending	PARAMS ((basic_block, sbitmap,
						 sbitmap));
static void compute_ranges		PARAMS ((int *));
static bool edge_is_dead		PARAMS ((edge));
static bool redirect_edges		PARAMS ((void));

static int dump_range			PARAMS ((void **, void *));
static void dump_all_ranges		PARAMS ((FILE *, htab_t));
void debug_range			PARAMS ((range_t *));
void debug_all_ranges			PARAMS ((htab_t));

/* Hash function for register R.  */
#define HASH_REG(R) (REGNO (R))

/* Hash function for table of ranges. It is computed from the range XX.  */

static hashval_t
range_hash (xx)
     const void *xx;
{
  const range_t *r = xx;

  return HASH_REG (r->reg);
}

/* Return true when the range XX is the range for register YY.  */

static int
range_eq (xx, yy)
     const void *xx;
     const void *yy;
{
  const range_t *r = (const range_t *) xx;
  const rtx y = (const rtx) yy;

  return REGNO (r->reg) == REGNO (y);
}

/* Free the range X.  */

static void
range_del (x)
     void *x;
{
  pool_free (range_pool, x);
}

/* Return true when the range is empty, i.e. register can't have any value.  */

static bool
range_is_empty (r)
     const range_t *r;
{
  switch (r->type)
    {
      case RANGE_SIGNED_INT:
      case RANGE_SIGNED_INT | RANGE_UNSIGNED_INT:
	return (r->from.si > r->to.si);

      case RANGE_UNSIGNED_INT:
	return (r->from.ui > r->to.ui);
    }

  return false;
}

/* Data passed from union_all_ranges to union_ranges.  */

typedef struct union_ranges_data_def
{
  /* The list of ranges from one and second edge.  */
  htab_t t1;
  htab_t t2;
} union_ranges_data;

/* Unify range from *SLOT and the corresponding range from DATA.T2.  */

static int
union_ranges (slot, data)
     void **slot;
     void *data;
{
  union_ranges_data *d = data;
  htab_t t1 = d->t1;
  htab_t t2 = d->t2;
  range_t *r1 = *slot;
  range_t *r2;

  r2 = htab_find_with_hash (t2, r1->reg, HASH_REG (r1->reg));
  if (!r2)
    {
      /* Register from second edge may have any value so the result may have
	 also any value.  */
      htab_clear_slot (t1, slot);
      return 1;
    }

  if (range_is_empty (r1))
    {
      /* First range is empty so the union is the second range.  */
      *r1 = *r2;
      return 1;
    }

  /* Second range is empty so the union is already in the first range.  */
  if (range_is_empty (r2))
    return 1;

  r1->type &= r2->type;
  if (r1->type == 0)
    {
      /* The ranges are not compatible so make the union to be a full range
	 (i.e. delete it from the table).  */
      htab_clear_slot (t1, slot);
      return 1;
    }

  /* For compatible ranges make the bounds to contain both intervals.  */
  if ((r1->type & RANGE_SIGNED_INT) != 0)
    {
      if (r1->from.si > r2->from.si)
	r1->from.si = r2->from.si;
      if (r1->to.si < r2->to.si)
	r1->to.si = r2->to.si;
    }

  if ((r1->type & RANGE_UNSIGNED_INT) != 0)
    {
      if (r1->from.ui > r2->from.ui)
	r1->from.ui = r2->from.ui;
      if (r1->to.ui < r2->to.ui)
	r1->to.ui = r2->to.ui;
    }

  return 1;
}

/* Unify all ranges in table T1 with the corresponding ranges from T2.  */

static void
union_all_ranges (t1, t2)
     htab_t t1;
     htab_t t2;
{
  union_ranges_data data;

  data.t1 = t1;
  data.t2 = t2;
  htab_traverse (t1, union_ranges, &data);
}

/* Return true if ranges R1 and R2 differ.  */

static bool
ranges_differ (r1, r2)
     const range_t *r1;
     const range_t *r2;
{
  bool empty1;
  bool empty2;

  /* R1 and R2 do not have a common type.  */
  if ((r1->type & r2->type) == 0)
    return true;

  if (!rtx_equal_p (r1->reg, r2->reg))
    return true;

  empty1 = range_is_empty (r1);
  empty2 = range_is_empty (r2);

  if (empty1 != empty2)
    return true;

  if (empty1 && empty2)
    return false;

  if ((r1->type & r2->type & RANGE_SIGNED_INT) != 0)
  {
    if (r1->from.si != r2->from.si)
      return true;

    if (r1->to.si != r2->to.si)
      return true;
  }

  if ((r1->type & r2->type & RANGE_UNSIGNED_INT) != 0)
  {
    if (r1->from.ui != r2->from.ui)
      return true;

    if (r1->to.ui != r2->to.ui)
      return true;
  }

  return false;
}

/* Variable to hold the temporary result of compare_register_tables.  */
static bool compare_register_tables_value;

/* Compare the range *SLOT with the corresponding range in table DATA.  */

static int
compare_register_tables_1 (slot, data)
     void **slot;
     void *data;
{
  range_t *r1 = *slot;
  range_t *r2;
  htab_t htab = data;

  r2 = htab_find_with_hash (htab, r1->reg, HASH_REG (r1->reg));

  if (r2 == NULL || ranges_differ (r1, r2))
    {
      compare_register_tables_value = true;

      /* Stop traversing.  */
      return 0;
    }

  /* Continue traversing.  */
  return 1;
}

/* Return true when the tables T1 and T2 of ranges differ.  */

static bool
compare_register_tables (t1, t2)
     htab_t t1;
     htab_t t2;
{
  compare_register_tables_value = false;

  htab_traverse (t1, compare_register_tables_1, t2);
  if (!compare_register_tables_value)
    htab_traverse (t2, compare_register_tables_1, t1);

  return compare_register_tables_value;
}

/* Copy range *SRC_SLOT to table DATA.  */

static int
copy_range (src_slot, data)
     void **src_slot;
     void *data;
{
  range_t *src_r = *src_slot;
  range_t *dst_r;
  htab_t dst = data;
  void **dst_slot;

  dst_r = pool_alloc (range_pool);
  *dst_r = *src_r;

  dst_slot = htab_find_slot_with_hash (dst, src_r->reg, HASH_REG (src_r->reg),
				       INSERT);
  *dst_slot = dst_r;

  /* Continue traversing the hash table.  */
  return 1;
}

/* Copy the content of table SRC to DST.  */

static void
copy_register_table (dst, src)
     htab_t dst;
     htab_t src;
{
  htab_empty (dst);
  htab_traverse (src, copy_range, dst);
}

/* Update the ranges for OP0 and OP1 on edge E according to
   result of OP0 EQ OP1.  */

static void
process_ranges_eq (op0, op1, e)
     rtx op0;
     rtx op1;
     edge e;
{
  void **slot0;
  void **slot1;
  range_t *r0 = NULL;	/* Range for OP0.  */
  range_t *r1 = NULL;	/* Range for OP1.  */
  range_t tmp;

  slot0 = htab_find_slot_with_hash (EI (e)->htab, op0, HASH_REG (op0),
				    NO_INSERT);
  if (slot0)
    r0 = *slot0;

  if (GET_CODE (op1) == CONST_INT)
    {
      unsigned HOST_WIDE_INT mask
	= (unsigned HOST_WIDE_INT) GET_MODE_MASK (GET_MODE (op0));

      /* OP1 is a constant, create a temporary range for it.  */
      r1 = &tmp;
      tmp.reg = NULL;
      tmp.from.si = INTVAL (op1);
      tmp.to.si = INTVAL (op1);
      tmp.from.ui = (unsigned HOST_WIDE_INT) INTVAL (op1) & mask;
      tmp.to.ui = (unsigned HOST_WIDE_INT) INTVAL (op1) & mask;
      if (r0 != NULL)
	tmp.type = r0->type;
      else
	tmp.type = RANGE_SIGNED_INT | RANGE_UNSIGNED_INT;
    }
  else if (GET_CODE (op1) == REG
	   && GET_MODE (op0) == GET_MODE (op1))
    {
      slot1 = htab_find_slot_with_hash (EI (e)->htab, op1, HASH_REG (op1),
					NO_INSERT);
      if (slot1)
	r1 = *slot1;
    }
  else
    return;

  /* If both ranges are full ranges we have nothing to do.  */
  if (!r0 && !r1)
    return;

  if (r0)
    {
      if (r1)
	{
	  switch (r0->type & r1->type)
	    {
	      case 0:
		/* If the ranges are not compatible leave them as they are.  */
		break;

	      case RANGE_SIGNED_INT | RANGE_UNSIGNED_INT:
		/* When both types are (RANGE_SIGNED_INT | RANGE_UNSIGNED_INT)
		   both ranges are constants or a result of union of 2 constants
		   and thus the ranges are not empty.  */

		/* If both ranges are constants compare them,
		   otherwise leave ranges as they are because we do not know
		   how to compare them.  */
		if (r0->from.si == r0->to.si && r1->from.si == r1->to.si)
		  {
		    if (r0->from.si != r1->from.si)
		      {
			/* Make the ranges empty.  */
			r0->from.si = 1;
			r0->to.si = 0;
			r0->from.ui = 1;
			r0->to.ui = 0;
			r1->from.si = 1;
			r1->to.si = 0;
			r1->from.ui = 1;
			r1->to.ui = 0;
		      }
		  }

		break;

	      case RANGE_SIGNED_INT:
		r0->type = r1->type = r0->type & r1->type;

		/* If one range is empty make the other empty too.  */
		if (range_is_empty (r0))
		  {
		    r1->from.si = 1;
		    r1->to.si = 0;
		    return;
		  }

		if (range_is_empty (r1))
		  {
		    r0->from.si = 1;
		    r0->to.si = 0;
		    return;
		  }

		/* If OP1 EQ OP2 set both ranges to the intersection of them.  */
		if (r0->from.si < r1->from.si)
		  r0->from.si = r1->from.si;
		else
		  r1->from.si = r0->from.si;

		if (r0->to.si < r1->to.si)
		  r1->to.si = r0->to.si;
		else
		  r0->to.si = r1->to.si;

		break;

	      case RANGE_UNSIGNED_INT:
		r0->type = r1->type = r0->type & r1->type;

		/* If one range is empty make the other empty too.  */
		if (range_is_empty (r0))
		  {
		    r1->from.ui = 1;
		    r1->to.ui = 0;
		    return;
		  }

		if (range_is_empty (r1))
		  {
		    r0->from.ui = 1;
		    r0->to.ui = 0;
		    return;
		  }

		/* If OP1 EQ OP2 set both ranges to the intersection of them.  */
		if (r0->from.ui < r1->from.ui)
		  r0->from.ui = r1->from.ui;
		else
		  r1->from.ui = r0->from.ui;

		if (r0->to.ui < r1->to.ui)
		  r1->to.ui = r0->to.ui;
		else
		  r0->to.ui = r1->to.ui;

		break;
	    }
	}
      else
	{
	  /* R1 is a full range, intersection with R0 is R0.  */
	  r1 = pool_alloc (range_pool);
	  slot1 = htab_find_slot_with_hash (EI (e)->htab, op1, HASH_REG (op1),
					    INSERT);
	  *slot1 = r1;
	  *r1 = *r0;
	  r1->reg = op1;
	}
    }
  else if (r1)
    {
      /* R0 is a full range, intersection with R1 is R1.  */
      r0 = pool_alloc (range_pool);
      slot0 = htab_find_slot_with_hash (EI (e)->htab, op0, HASH_REG (op0),
					INSERT);
      *slot0 = r0;
      *r0 = *r1;
      r0->reg = op0;
    }
}

/* Update the ranges for OP0 and OP1 on edges THEN_EDGE and ELSE_EDGE according
   to result of OP0 LT OP1.  */

static void
process_ranges_lt (op0, op1, then_edge, else_edge)
     rtx op0;
     rtx op1;
     edge then_edge;
     edge else_edge;
{
  void **slot0t;
  void **slot0e;
  void **slot1t;
  void **slot1e;
  range_t *r0t = NULL;	/* Range for OP0 on THEN_EDGE.  */
  range_t *r0e = NULL;	/* Range for OP0 on ELSE_EDGE.  */
  range_t *r1t = NULL;	/* Range for OP1 on THEN_EDGE.  */
  range_t *r1e = NULL;	/* Range for OP1 on ELSE_EDGE.  */
  range_t tmp0, tmp1;
  HOST_WIDE_INT max_val = 0;	/* Set it to something to avoid a warning.  */
  HOST_WIDE_INT min_val = 0;

  /* If the registers are not compatible leave the ranges as they are.  */
  if (GET_CODE (op0) == REG && GET_CODE (op1) == REG
      && GET_MODE (op0) != GET_MODE (op1))
    return;

  if (GET_CODE (op0) == REG)
    {
      max_val = (HOST_WIDE_INT) ((unsigned HOST_WIDE_INT)
				 GET_MODE_MASK (GET_MODE (op0)) >> 1);
      min_val = -max_val - 1;

      /* Find the range for OP0 on THEN_EDGE and ELSE_EDGE.  */
      slot0t = htab_find_slot_with_hash (EI (then_edge)->htab, op0,
					 HASH_REG (op0), NO_INSERT);
      if (slot0t)
	r0t = *slot0t;
      slot0e = htab_find_slot_with_hash (EI (else_edge)->htab, op0,
					 HASH_REG (op0), NO_INSERT);
      if (slot0e)
	r0e = *slot0e;
    }
  if (GET_CODE (op1) == REG)
    {
      max_val = (HOST_WIDE_INT) ((unsigned HOST_WIDE_INT)
				 GET_MODE_MASK (GET_MODE (op1)) >> 1);
      min_val = -max_val - 1;

      /* Find the range for OP0 on THEN_EDGE and ELSE_EDGE.  */
      slot1t = htab_find_slot_with_hash (EI (then_edge)->htab, op1,
					 HASH_REG (op1), NO_INSERT);
      if (slot1t)
	r1t = *slot1t;
      slot1e = htab_find_slot_with_hash (EI (else_edge)->htab, op1,
					 HASH_REG (op1), NO_INSERT);
      if (slot1e)
	r1e = *slot1e;
    }

  if (GET_CODE (op0) == CONST_INT)
    {
      /* OP0 is a constant, create a temporary range for it.  */
      r0t = r0e = &tmp0;
      tmp0.reg = NULL;
      tmp0.from.si = INTVAL (op0);
      tmp0.to.si = INTVAL (op0);
      tmp0.type = RANGE_SIGNED_INT;
    }
  else if (GET_CODE (op0) != REG)
    return;

  if (GET_CODE (op1) == CONST_INT)
    {
      /* OP1 is a constant, create a temporary range for it.  */
      r1t = r1e = &tmp1;
      tmp1.reg = NULL;
      tmp1.from.si = INTVAL (op1);
      tmp1.to.si = INTVAL (op1);
      tmp1.type = RANGE_SIGNED_INT;
    }
  else if (GET_CODE (op1) != REG)
    return;

  /* If both ranges on TRUE_EDGE are full (and thus on ELSE_EDGE too)
     we have nothing to do.  */
  if (!r0t && !r1t)
    return;

  /* If the type of one range is not signed int we do not know how
     to shorten ranges so we leave them as they are.  */
  if (r0t && (r0t->type & RANGE_SIGNED_INT) == 0)
    return;

  if (r1t && (r1t->type & RANGE_SIGNED_INT) == 0)
    return;

  if (r0t)
    {
      if (r1t)
	{
	  r0t->type = r0e->type = RANGE_SIGNED_INT;
	  r1t->type = r1e->type = RANGE_SIGNED_INT;

	  /* If one range is empty make the other empty too.  */
	  if (range_is_empty (r0t))
	    {
	      r1t->from.si = 1;
	      r1t->to.si = 0;
	      r1e->from.si = 1;
	      r1e->to.si = 0;
	      return;
	    }

	  if (range_is_empty (r1t))
	    {
	      r0t->from.si = 1;
	      r0t->to.si = 0;
	      r0e->from.si = 1;
	      r0e->to.si = 0;
	      return;
	    }

	  if (r0t->from.si == max_val)
	    {
	      /* The condition OP0 LT OP1 may be never true. */
	      r0t->from.si = 1;
	      r0t->to.si = 0;
	      r1t->from.si = 1;
	      r1t->to.si = 0;
	    }
	  else if (r0t->from.si >= r1t->from.si)
	    r1t->from.si = r0t->from.si + 1;
	  else
	    r0e->from.si = r1e->from.si;

	  if (r1t->to.si == min_val)
	    {
	      /* The condition OP0 LT OP1 may be never true. */
	      r0t->from.si = 1;
	      r0t->to.si = 0;
	      r1t->from.si = 1;
	      r1t->to.si = 0;
	    }
	  else if (r0t->to.si >= r1t->to.si)
	    r0t->to.si = r1t->to.si - 1;
	  else
	    r1e->to.si = r0e->to.si;
	}
      else
	{
	  /* R1 is full interval, shorten it on THEN_EDGE and ELSE_EDGE.  */
	  r1t = pool_alloc (range_pool);
	  slot1t = htab_find_slot_with_hash (EI (then_edge)->htab, op1,
					     HASH_REG (op1), INSERT);
	  *slot1t = r1t;
	  r1e = pool_alloc (range_pool);
	  slot1e = htab_find_slot_with_hash (EI (else_edge)->htab, op1,
					     HASH_REG (op1), INSERT);
	  *slot1e = r1e;

	  r1t->reg = r1e->reg = op1;
	  r1t->type = r1e->type = RANGE_SIGNED_INT;
	  r0t->type = r0e->type = RANGE_SIGNED_INT;

	  /* If one range is empty make the other empty too.  */
	  if (range_is_empty (r0t))
	    {
	      r1t->from.si = 1;
	      r1t->to.si = 0;
	      r1e->from.si = 1;
	      r1e->to.si = 0;
	      return;
	    }

	  if (r0t->from.si == max_val)
	    {
	      r1t->from.si = 1;
	      r1t->to.si = 0;
	    }
	  else
	    {
	      r1t->from.si = r0t->from.si + 1;
	      r1t->to.si = max_val;
	    }

	  r1e->from.si = min_val;
	  r1e->to.si = r0e->to.si;
	}
    }
  else if (r1t)
    {
      /* R0 is full interval, shorten it on THEN_EDGE and ELSE_EDGE.  */
      r0t = pool_alloc (range_pool);
      slot0t = htab_find_slot_with_hash (EI (then_edge)->htab, op0,
					 HASH_REG (op0), INSERT);
      *slot0t = r0t;
      r0e = pool_alloc (range_pool);
      slot0e = htab_find_slot_with_hash (EI (else_edge)->htab, op0,
					 HASH_REG (op0), INSERT);
      *slot0e = r0e;

      r0t->reg = r0e->reg = op0;
      r0t->type = r0e->type = RANGE_SIGNED_INT;
      r1t->type = r1e->type = RANGE_SIGNED_INT;

      /* If one range is empty make the other empty too.  */
      if (range_is_empty (r1t))
	{
	  r0t->from.si = 1;
	  r0t->to.si = 0;
	  r0e->from.si = 1;
	  r0e->to.si = 0;
	  return;
	}

      if (r1t->to.si == min_val)
	{
	  r0t->from.si = 1;
	  r0t->to.si = 0;
	}
      else
	{
	  r0t->from.si = min_val;
	  r0t->to.si = r1t->to.si - 1;
	}

      r0e->from.si = r1e->from.si;
      r0e->to.si = max_val;
    }
}

/* Update the ranges for OP0 and OP1 on edges THEN_EDGE and ELSE_EDGE according
   to result of OP0 LTU OP1.
   This function is very similar to process_ranges_lt but it is working
   with unsigned intervals.  */

static void
process_ranges_ltu (op0, op1, then_edge, else_edge)
     rtx op0;
     rtx op1;
     edge then_edge;
     edge else_edge;
{
  void **slot0t;
  void **slot0e;
  void **slot1t;
  void **slot1e;
  range_t *r0t = NULL;	/* Range for OP0 on THEN_EDGE.  */
  range_t *r0e = NULL;	/* Range for OP0 on ELSE_EDGE.  */
  range_t *r1t = NULL;	/* Range for OP1 on THEN_EDGE.  */
  range_t *r1e = NULL;	/* Range for OP1 on ELSE_EDGE.  */
  range_t tmp0, tmp1;
  unsigned HOST_WIDE_INT max_val = 0;
  unsigned HOST_WIDE_INT min_val = 0;

  /* If the registers are not compatible leave the ranges as they are.  */
  if (GET_CODE (op0) == REG && GET_CODE (op1) == REG
      && GET_MODE (op0) != GET_MODE (op1))
    return;

  if (GET_CODE (op0) == REG)
    {
      max_val = (unsigned HOST_WIDE_INT) GET_MODE_MASK (GET_MODE (op0));

      /* Find the range for OP0 on THEN_EDGE and ELSE_EDGE.  */
      slot0t = htab_find_slot_with_hash (EI (then_edge)->htab, op0,
					 HASH_REG (op0), NO_INSERT);
      if (slot0t)
	r0t = *slot0t;
      slot0e = htab_find_slot_with_hash (EI (else_edge)->htab, op0,
					 HASH_REG (op0), NO_INSERT);
      if (slot0e)
	r0e = *slot0e;
    }
  if (GET_CODE (op1) == REG)
    {
      max_val = (unsigned HOST_WIDE_INT) GET_MODE_MASK (GET_MODE (op0));

      /* Find the range for OP0 on THEN_EDGE and ELSE_EDGE.  */
      slot1t = htab_find_slot_with_hash (EI (then_edge)->htab, op1,
					 HASH_REG (op1), NO_INSERT);
      if (slot1t)
	r1t = *slot1t;
      slot1e = htab_find_slot_with_hash (EI (else_edge)->htab, op1,
					 HASH_REG (op1), NO_INSERT);
      if (slot1e)
	r1e = *slot1e;
    }

  if (GET_CODE (op0) == CONST_INT)
    {
      /* OP0 is a constant, create a temporary range for it.  */
      r0t = r0e = &tmp0;
      tmp0.reg = NULL;
      tmp0.from.ui = (unsigned HOST_WIDE_INT) INTVAL (op0) & max_val;
      tmp0.to.ui = (unsigned HOST_WIDE_INT) INTVAL (op0) & max_val;
      tmp0.type = RANGE_UNSIGNED_INT;
    }
  else if (GET_CODE (op0) != REG)
    return;

  if (GET_CODE (op1) == CONST_INT)
    {
      /* OP1 is a constant, create a temporary range for it.  */
      r1t = r1e = &tmp1;
      tmp1.reg = NULL;
      tmp1.from.ui = (unsigned HOST_WIDE_INT) INTVAL (op1) & max_val;
      tmp1.to.ui = (unsigned HOST_WIDE_INT) INTVAL (op1) & max_val;
      tmp1.type = RANGE_UNSIGNED_INT;
    }
  else if (GET_CODE (op0) != REG)
    return;

  /* If both ranges on TRUE_EDGE are full (and thus on ELSE_EDGE too)
     we have nothing to do.  */
  if (!r0t && !r1t)
    return;

  /* If the type of one range is not unsigned int we do not know how
     to shorten ranges so we leave them as they are.  */
  if (r0t && (r0t->type & RANGE_UNSIGNED_INT) == 0)
    return;

  if (r1t && (r1t->type & RANGE_UNSIGNED_INT) == 0)
    return;

  if (r0t)
    {
      if (r1t)
	{
	  r0t->type = r0e->type = RANGE_UNSIGNED_INT;
	  r1t->type = r1e->type = RANGE_UNSIGNED_INT;

	  /* If one range is empty make the other empty too.  */
	  if (range_is_empty (r0t))
	    {
	      r1t->from.ui = 1;
	      r1t->to.ui = 0;
	      r1e->from.ui = 1;
	      r1e->to.ui = 0;
	      return;
	    }

	  if (range_is_empty (r1t))
	    {
	      r0t->from.ui = 1;
	      r0t->to.ui = 0;
	      r0e->from.ui = 1;
	      r0e->to.ui = 0;
	      return;
	    }

	  if (r0t->from.ui == max_val)
	    {
	      /* The condition OP0 LT OP1 may be never true. */
	      r0t->from.ui = 1;
	      r0t->to.ui = 0;
	      r1t->from.ui = 1;
	      r1t->to.ui = 0;
	    }
	  else if (r0t->from.ui >= r1t->from.ui)
	    r1t->from.ui = r0t->from.ui + 1;
	  else
	    r0e->from.ui = r1e->from.ui;

	  if (r1t->to.ui == min_val)
	    {
	      /* The condition OP0 LT OP1 may be never true. */
	      r0t->from.ui = 1;
	      r0t->to.ui = 0;
	      r1t->from.ui = 1;
	      r1t->to.ui = 0;
	    }
	  else if (r0t->to.ui >= r1t->to.ui)
	    r0t->to.ui = r1t->to.ui - 1;
	  else
	    r1e->to.ui = r0e->to.ui;
	}
      else
	{
	  /* R1 is full interval, shorten it on THEN_EDGE and ELSE_EDGE.  */
	  r1t = pool_alloc (range_pool);
	  slot1t = htab_find_slot_with_hash (EI (then_edge)->htab, op1,
					     HASH_REG (op1), INSERT);
	  *slot1t = r1t;
	  r1e = pool_alloc (range_pool);
	  slot1e = htab_find_slot_with_hash (EI (else_edge)->htab, op1,
					     HASH_REG (op1), INSERT);
	  *slot1e = r1e;

	  r1t->reg = r1e->reg = op1;
	  r1t->type = r1e->type = RANGE_UNSIGNED_INT;
	  r0t->type = r0e->type = RANGE_UNSIGNED_INT;

	  /* If one range is empty make the other empty too.  */
	  if (range_is_empty (r0t))
	    {
	      r1t->from.ui = 1;
	      r1t->to.ui = 0;
	      r1e->from.ui = 1;
	      r1e->to.ui = 0;
	      return;
	    }

	  if (r0t->from.ui == max_val)
	    {
	      r1t->from.ui = 1;
	      r1t->to.ui = 0;
	    }
	  else
	    {
	      r1t->from.ui = r0t->from.ui + 1;
	      r1t->to.ui = max_val;
	    }

	  r1e->from.ui = min_val;
	  r1e->to.ui = r0e->to.ui;
	}
    }
  else if (r1t)
    {
      /* R0 is full interval, shorten it on THEN_EDGE and ELSE_EDGE.  */
      r0t = pool_alloc (range_pool);
      slot0t = htab_find_slot_with_hash (EI (then_edge)->htab, op0,
					 HASH_REG (op0), INSERT);
      *slot0t = r0t;
      r0e = pool_alloc (range_pool);
      slot0e = htab_find_slot_with_hash (EI (else_edge)->htab, op0,
					 HASH_REG (op0), INSERT);
      *slot0e = r0e;

      r0t->reg = r0e->reg = op0;
      r0t->type = r0e->type = RANGE_UNSIGNED_INT;
      r1t->type = r1e->type = RANGE_UNSIGNED_INT;

      /* If one range is empty make the other empty too.  */
      if (range_is_empty (r1t))
	{
	  r0t->from.ui = 1;
	  r0t->to.ui = 0;
	  r0e->from.ui = 1;
	  r0e->to.ui = 0;
	  return;
	}

      if (r1t->to.ui == min_val)
	{
	  r0t->from.ui = 1;
	  r0t->to.ui = 0;
	}
      else
	{
	  r0t->from.ui = min_val;
	  r0t->to.ui = r1t->to.ui - 1;
	}

      r0e->from.ui = r1e->from.ui;
      r0e->to.ui = max_val;
    }
}

/* Update the ranges on the edges going out from BB.  */

static bool
update_outgoing_edges (bb, changed)
     basic_block bb;
     int changed;
{
  edge e;
  htab_t old_then = NULL;	/* Set it to something to avoid a warning.  */
  htab_t old_else = NULL;

  if (!changed && BBI (bb)->cond != NULL)
    {
      edge then_edge = BRANCH_EDGE (bb);
      edge else_edge = FALLTHRU_EDGE (bb);

      old_then = htab_create (EI (then_edge)->htab->n_elements, range_hash,
			      range_eq, range_del);
      copy_register_table (old_then, EI (then_edge)->htab);
      old_else = htab_create (EI (else_edge)->htab->n_elements, range_hash,
			      range_eq, range_del);
      copy_register_table (old_else, EI (else_edge)->htab);
    }

  /* Copy the BB's OUT set to edges.  */
  for (e = bb->succ; e; e = e->succ_next)
    copy_register_table (EI (e)->htab, BBI (bb)->out);

  /* Update outgoing edges.  */
  if (BBI (bb)->cond != NULL)
    {
      edge then_edge = BRANCH_EDGE (bb);
      edge else_edge = FALLTHRU_EDGE (bb);
      rtx op0 = XEXP (BBI (bb)->cond, 0);
      rtx op1 = XEXP (BBI (bb)->cond, 1);

      if (GET_CODE (op0) == REG
	  && (GET_CODE (op1) == REG
	      || GET_CODE (op1) == CONST_INT))
	{
	  switch (GET_CODE (BBI (bb)->cond))
	    {
	      case EQ:
		process_ranges_eq (op0, op1, then_edge);
		break;

	      case NE:
		/* OP0 NE OP1: True -> THEN_EDGE, False -> ELSE_EDGE
		   is equivalent to
		   OP0 EQ OP1: True -> ELSE_EDGE, False -> THEN_EDGE.  */
		process_ranges_eq (op0, op1, else_edge);
		break;

	      case LT:
		process_ranges_lt (op0, op1, then_edge, else_edge);
		break;

	      case GE:
		/* OP0 GE OP1: True -> THEN_EDGE, False -> ELSE_EDGE
		   is equivalent to
		   OP0 LT OP1: True -> ELSE_EDGE, False -> THEN_EDGE.  */
		process_ranges_lt (op0, op1, else_edge, then_edge);
		break;

	      case GT:
		/* OP0 GT OP1: True -> THEN_EDGE, False -> ELSE_EDGE
		   is equivalent to
		   OP1 LT OP0: True -> THEN_EDGE, False -> ELSE_EDGE.  */
		process_ranges_lt (op1, op0, then_edge, else_edge);
		break;

	      case LE:
		/* OP0 LE OP1: True -> THEN_EDGE, False -> ELSE_EDGE
		   is equivalent to
		   OP0 GT OP1: True -> ELSE_EDGE, False -> THEN_EDGE
		   is equivalent to
		   OP1 LT OP0: True -> ELSE_EDGE, False -> THEN_EDGE.  */
		process_ranges_lt (op1, op0, else_edge, then_edge);
		break;

	      case LTU:
		process_ranges_ltu (op0, op1, then_edge, else_edge);
		break;

	      case GEU:
		/* Similarly to GE.  */
		process_ranges_ltu (op0, op1, else_edge, then_edge);
		break;

	      case GTU:
		/* Similarly to GT.  */
		process_ranges_ltu (op1, op0, then_edge, else_edge);
		break;

	      case LEU:
		/* Similarly to LE.  */
		process_ranges_ltu (op1, op0, else_edge, then_edge);
		break;

	      default:
		break;
	    }
	}

      if (!changed)
	{
	  changed = compare_register_tables (old_then, EI (then_edge)->htab);
	  if (!changed)
	    changed = compare_register_tables (old_else, EI (else_edge)->htab);

	  htab_delete (old_then);
	  htab_delete (old_else);
	}
    }

  return changed;
}

/* Compute the effect of set to STORE in insn PATTERN and update the ranges
   in table DATA.  */

static void
compute_ranges_for_insn (store, pattern, data)
     rtx store;
     rtx pattern;
     void *data;
{
  htab_t htab = (htab_t) data;
  rtx set_src, set_dst;
  void **slot;
  range_t *dst_r, *src_r;
  unsigned HOST_WIDE_INT mask;

  set_dst = SET_DEST (pattern);
  if (GET_CODE (store) == REG)
    {
      if (set_dst != store || GET_CODE (pattern) == CLOBBER)
	{
	  /* SET_DST is a SUBREG, ZERO_EXTRACT, etc.
	     or we are clobbering the register.
	     We do not know the resulting range so delete the range
	     (i.e. make it to be a full range).  */

	  slot = htab_find_slot_with_hash (htab, store, HASH_REG (store),
					   NO_INSERT);
	  if (slot)
	    htab_clear_slot (htab, slot);
	  return;
	}

      mask = (unsigned HOST_WIDE_INT) GET_MODE_MASK (GET_MODE (set_dst));
      set_src = SET_SRC (pattern);
      switch (GET_CODE (set_src))
	{
	  case REG:
	    src_r = htab_find_with_hash (htab, set_src, HASH_REG (set_src));
	    if (src_r)
	      {
		/* Copy the range from SET_SRC to SET_DST.  */
		slot = htab_find_slot_with_hash (htab, set_dst,
						 HASH_REG (set_dst), NO_INSERT);

		if (slot)
		  dst_r = *slot;
		else
		  {
		    dst_r = pool_alloc (range_pool);
		    slot = htab_find_slot_with_hash (htab, set_dst,
						     HASH_REG (set_dst),
						     INSERT);
		    *slot = dst_r;
		  }

		*dst_r = *src_r;
		dst_r->reg = set_dst;
	      }
	    else
	      {
		/* SET_SRC has a full range.  */
		slot = htab_find_slot_with_hash (htab, set_dst,
						 HASH_REG (set_dst),
						 NO_INSERT);
		if (slot)
		  htab_clear_slot (htab, slot);
	      }
	    return;

	  case CONST_INT:
	    /* Set the range of SET_DST to be a single value.  */
	    slot = htab_find_slot_with_hash (htab, set_dst,
					     HASH_REG (set_dst), NO_INSERT);
	    if (slot)
	      dst_r = *slot;
	    else
	      {
		dst_r = pool_alloc (range_pool);
		slot = htab_find_slot_with_hash (htab, set_dst,
						 HASH_REG (set_dst), INSERT);
		*slot = dst_r;
	      }

	    /* We do not know whether the constant is signed or unsigned.  */
	    dst_r->type = RANGE_SIGNED_INT | RANGE_UNSIGNED_INT;
	    dst_r->reg = set_dst;
	    dst_r->from.si = INTVAL (set_src);
	    dst_r->to.si = INTVAL (set_src);
	    dst_r->from.ui = (unsigned HOST_WIDE_INT) INTVAL (set_src) & mask;
	    dst_r->to.ui = (unsigned HOST_WIDE_INT) INTVAL (set_src) & mask;
	    return;

	  default:
	    /* We do not know how the range changes so make it to be
	       a full range.  */
	    slot = htab_find_slot_with_hash (htab, set_dst,
					     HASH_REG (set_dst), NO_INSERT);
	    if (slot)
	      htab_clear_slot (htab, slot);
	    return;
	}
    }
}

/* Set the range of register *SLOT to be a full range
   if the register is in CALL_USED_REG_SET (i.e. delete such a register from
   hash table DATA.  */

static int
delete_call_used_regs (slot, data)
     void **slot;
     void *data;
{
  htab_t htab = data;
  range_t *r = *slot;
  int regno;

  regno = REGNO (r->reg);
  if (regno < FIRST_PSEUDO_REGISTER
      && TEST_HARD_REG_BIT (call_used_reg_set, regno))
    {
      htab_clear_slot (htab, slot);
    }

  /* Continue traversing.  */
  return 1;
}

/* Compute the effect of insns in block BB.  Update the edges going out
   from BB.  Return true if any range changes.  */

static bool
compute_ranges_for_bb (bb)
     basic_block bb;
{
  bool changed;
  htab_t new_out;
  rtx insn;

  new_out = htab_create (BBI (bb)->in->n_elements, range_hash, range_eq,
			 range_del);
  copy_register_table (new_out, BBI (bb)->in);

  for (insn = bb->head; insn != NEXT_INSN (bb->end); insn = NEXT_INSN (insn))
    {
      if (GET_CODE (insn) == CALL_INSN)
	{
	  /* Set the range to be a full range for the registers
	     which do not survive during the call.  */
	  htab_traverse (new_out, delete_call_used_regs, new_out);
	}
      if (INSN_P (insn))
	{
	  note_stores (PATTERN (insn), compute_ranges_for_insn, new_out);
	}
    }

  changed = compare_register_tables (new_out, BBI (bb)->out);

  htab_delete (BBI (bb)->out);
  BBI (bb)->out = new_out;

  changed |= update_outgoing_edges (bb, changed);

  return changed;
}

/* Compute the effect of blocks in PENDING set, starting in BB.  Do not
   recompute the ranges for blocks in VISITED set.
   This function is similar to hybrid_search_sbitmap in df.c
   but modified for different data structures.  */

static void
compute_ranges_for_pending (bb, visited, pending)
     basic_block bb;
     sbitmap visited;
     sbitmap pending;
{
  bool changed;
  edge e;

  SET_BIT (visited, bb->index);
  if (TEST_BIT (pending, bb->index))
    {
      /* Calculate the IN set as union of predecessor OUT sets.  */
      if ((e = bb->pred) != NULL)
	{
	  copy_register_table (BBI (bb)->in, EI (e)->htab);
	  for (e = e->pred_next; e; e = e->pred_next)
	    union_all_ranges (BBI (bb)->in, EI (e)->htab);
	}

      changed = compute_ranges_for_bb (bb);
      RESET_BIT (pending, bb->index);

      if (changed)
	{
	  for (e = bb->succ; e; e = e->succ_next)
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
	compute_ranges_for_pending (e->dest, visited, pending);
    }
}

/* Compute ranges for the whole function.  BB_ORDER is the order to iterate in
   (it maps block numbers -> order).  Because this is a forward data-flow problem
   we use the reverse completion DFS-order (rc_order).  */

static void
compute_ranges (bb_order)
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
	    compute_ranges_for_pending (bb, visited, pending);
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

/* Return true if edge E is dead (we will never go though this edge because
   of the conditions), i.e. (at least) one of the register ranges is empty.  */

static bool
edge_is_dead (e)
     edge e;
{
  rtx cond = BBI (e->src)->cond;
  rtx op;
  int i;

#ifdef ENABLE_CHECKING
  if (cond == NULL)
    abort ();
#endif

  for (i = 0; i < 2; i++)
    {
      op = XEXP (cond, i);
      if (GET_CODE (op) == REG)
	{
	  range_t *r;

	  r = htab_find_with_hash (EI (e)->htab, op, HASH_REG (op));
	  if (r && range_is_empty (r))
	    return true;
	}
    }

  return false;
}

/* Redirect dead edges to the other edges.  */

static bool
redirect_edges ()
{
  bool modified = false;
  basic_block bb;

  FOR_EACH_BB (bb)
    {
      if (any_condjump_p (bb->end)
	  && BBI (bb)->cond != NULL)
	{
	  edge then_edge = BRANCH_EDGE (bb);
	  edge else_edge = FALLTHRU_EDGE (bb);
	  bool then_dead;
	  bool else_dead;

	  if (then_edge->dest == EXIT_BLOCK_PTR
	      || else_edge->dest == EXIT_BLOCK_PTR)
	    continue;

	  /* If both edges are dead BB is dead too so it will be removed
	     and thus destinations of both edges too.
	     So perform the redirection only when there is exactly one dead
	     edge.  */

	  then_dead = edge_is_dead (then_edge);
	  else_dead = edge_is_dead (else_edge);
	  if (then_dead && !else_dead)
	    {
	      if (rtl_dump_file)
		{
		  fprintf (rtl_dump_file, "Redirecting edge %d->%d to %d\n",
			   then_edge->src->index, then_edge->dest->index,
			   else_edge->dest->index);
		}

	      if (!redirect_edge_and_branch (then_edge, else_edge->dest))
		abort ();

	      modified = true;
	    }
	  else if (!then_dead && else_dead)
	    {
	      if (rtl_dump_file)
		{
		  fprintf (rtl_dump_file, "Redirecting edge %d->%d to %d\n",
			   else_edge->src->index, else_edge->dest->index,
			   then_edge->dest->index);
		}

	      if (!redirect_edge_and_branch (else_edge, then_edge->dest))
		abort ();

	      modified = true;
	    }
	}
    }

  return modified;
}

/* Print a range *SLOT to file DATA.  */

static int
dump_range (slot, data)
     void **slot;
     void *data;
{
  FILE *file = data;
  range_t *r;

  if (!slot)
    return 1;

  r = *slot;

  print_inline_rtx (file, r->reg, 2);
  fprintf (file, "\n");

  if (r->type & RANGE_SIGNED_INT)
    {
      fprintf (file, "  signed [");
      fprintf (file, HOST_WIDE_INT_PRINT_DEC, r->from.si);
      fprintf (file, ", ");
      fprintf (file, HOST_WIDE_INT_PRINT_DEC, r->to.si);
      fprintf (file, "]\n");
    }

  if (r->type & RANGE_UNSIGNED_INT)
    {
      fprintf (file, "  unsigned [");
      fprintf (file, HOST_WIDE_INT_PRINT_UNSIGNED, r->from.ui);
      fprintf (file, ", ");
      fprintf (file, HOST_WIDE_INT_PRINT_UNSIGNED, r->to.ui);
      fprintf (file, "]\n");
    }

  return 1;
}

/* Print all ranges in table HTAB to file FILE.  */

static void
dump_all_ranges (file, htab)
     FILE *file;
     htab_t htab;
{
  htab_traverse (htab, dump_range, file);
  fprintf (file, "\n");
}

/* Print a range R to STDERR.  */

void
debug_range (r)
     range_t *r;
{
  void *x = (void *) r;

  dump_range (&x, stderr);
}

void
debug_all_ranges (htab)
     htab_t htab;
{
  dump_all_ranges (stderr, htab);
}

/* Compute the value ranges and remove unreachable edges.
   The main entry point of this file.  */

bool
value_range_propagation ()
{
  bool modified;
  basic_block bb;
  edge e;
  int *rc_order;
  int *bb_order;
  int i;

  /* Initialization.  */
  range_pool = create_alloc_pool ("range_t", sizeof (range_t), 250);
  alloc_aux_for_blocks (sizeof (bb_info));
  alloc_aux_for_edges (sizeof (edge_info));

  FOR_ALL_BB (bb)
    {
      if (bb != ENTRY_BLOCK_PTR && bb != EXIT_BLOCK_PTR)
	BBI (bb)->cond = get_condition (bb->end, NULL);
      else
	BBI (bb)->cond = NULL;

      BBI (bb)->in = htab_create (5, range_hash, range_eq, range_del);
      BBI (bb)->out = htab_create (5, range_hash, range_eq, range_del);
      for (e = bb->succ; e; e = e->succ_next)
	EI (e)->htab = htab_create (5, range_hash, range_eq, range_del);
    }

  /* Compute reverse completion order of depth first search of the CFG
     so that the data-flow could possibly run faster.  */
  rc_order = (int *) xmalloc (n_basic_blocks * sizeof (int));
  bb_order = (int *) xmalloc (last_basic_block * sizeof (int));
  flow_depth_first_order_compute (NULL, rc_order);
  for (i = 0; i < n_basic_blocks; i++)
    bb_order[rc_order[i]] = i;
  free (rc_order);

  if (rtl_dump_file)
    {
      dump_flow_info (rtl_dump_file);
    }

  compute_ranges (bb_order);
  free (bb_order);

  if (rtl_dump_file)
    {
      FOR_EACH_BB (bb)
	{
	  fprintf (rtl_dump_file, "Status at the end of BB %d:\n", bb->index);
	  dump_all_ranges (rtl_dump_file, BBI (bb)->out);
	}

      FOR_EACH_BB (bb)
	{
	  for (e = bb->succ; e; e = e->succ_next)
	    {
	      fprintf (rtl_dump_file, "Status on edge %d -> %d:\n",
		       e->src->index, e->dest->index);
	      dump_all_ranges (rtl_dump_file, EI (e)->htab);
	    }
	}
    }

  modified = redirect_edges ();

  /* Cleanup.  */
  FOR_ALL_BB (bb)
    {
      htab_delete (BBI (bb)->in);
      htab_delete (BBI (bb)->out);
      for (e = bb->succ; e; e = e->succ_next)
	htab_delete (EI (e)->htab);
    }

  free_aux_for_edges ();
  free_aux_for_blocks ();
  free_alloc_pool (range_pool);

  return modified;
}
